/*
 *   Copyright (c) 2007-2008 C3SL.
 *
 *   This file is part of Chessd.
 *
 *   Chessd is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Chessd is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 */

#include <memory>
#include "TourneyManager.hh"
#include "Util/utils.hh"

#include "TourneyFactory.hh"

#include "XMPP/Exception.hh"
#include "Exception.hh"
#include "TourneyException.hh"

#include "Util/Date.hh"

#define XMLNS_CHESSD_TOURNEY       "http://c3sl.ufpr.br/chessd#tourney"

using namespace std;
using namespace XML;
using namespace XMPP;
using namespace Util;

TourneyManager::TourneyManager(
        ServerCore& game_manager,
        DatabaseManager& database,
        const StanzaHandler& send_stanza) :
    ServerModule(send_stanza),
    tourney_ids(0),
    game_manager(game_manager),
    database(database),
    handleError(handleError)
{

    /* Set features */
    //this->root_node.disco().features().insert(XMLNS_CHESSD_TOURNEY);

    /* Set match iqs */
    //this->root_node.setIqHandler(boost::bind(&TourneyManager::handleTourney, this, _1),
    //        XMLNS_CHESSD_TOURNEY);
}

TourneyManager::~TourneyManager() {
}

vector<string> TourneyManager::namespaces() const {
    vector<string> ret;
    ret.push_back(XMLNS_CHESSD_TOURNEY);
    return ret;
}

void TourneyManager::handleIq(const Stanza& stanza) {
    try {
        const Tag& query = stanza.firstTag();

        /* Parse te message */
        if(query.name() == "create") {
            this->handleCreate(stanza);
        } else if(query.name() == "list") {
            this->handleList(stanza);
        } else if(query.name() == "join") {
            this->handleJoin(stanza);
        } else {
            throw bad_request("Invalid format");
        }

    } catch (const xml_error& error) {
        throw bad_request("Invalid format");
    }
}

void TourneyManager::handleCreate(const Stanza& stanza) {
    /* get tourney attributes */
    const Tag& query = stanza.firstTag();
    const Tag& tourney_attributes = query.firstTag();

    Time start_time = ptime_to_systime(xmpp_date_time_to_ptime(
            (tourney_attributes.getAttribute("start_time"))));

    if(start_time < Timer::now() + Time::Minutes(1)) {
        throw bad_request("Start time is too close");
    }

    /* create tourney */
    auto_ptr<TourneyStatus> tourney(new TourneyStatus);

    tourney->name = tourney_attributes.findTag("name").getCData();
    tourney->description =
        tourney_attributes.findTag("description").getCData();
    tourney->running = false;
    tourney->start_time = start_time;
    tourney->tourney = auto_ptr<Tourney>(
            TourneyFactory::create(tourney_attributes));
    tourney->owner = stanza.from();

    /* pick an id */
    uint64_t id = this->tourney_ids++;

    /* insert tourney to the tourney list */
    this->tourneys.insert(id, tourney.release());

    /* schedule tourney start */
    this->dispatcher.schedule(boost::bind(&TourneyManager::startTourney, this, id), start_time);


    /* send result with tourney id */
    auto_ptr<Stanza> result(stanza.createIQResult());
    TagGenerator generator;
    generator.openTag("create");
    generator.addAttribute("xmlns", XMLNS_CHESSD_TOURNEY);
    generator.openTag("tourney");
    generator.addAttribute("id", to_string(id));
    result->children().push_back(generator.getTag());
    this->sendStanza(result.release());
}

void TourneyManager::startTourney(uint64_t tourney_id) {
    if(this->tourneys.count(tourney_id) == 0) {
        /* the tourney has been canceled */
        return;
    }

    /* Set tourney status */
    TourneyStatus& tourney = *this->tourneys.find(tourney_id)->second;
    tourney.running = true;

    /* Start first round */
    this->startNextRound(tourney_id);
}

void TourneyManager::startNextRound(uint64_t tourney_id) {
    /* check if the tourney exists */
    if(this->tourneys.count(tourney_id) == 0) {
        return;
    }

    /* get the tourney */
    TourneyStatus& tourney = *this->tourneys.find(tourney_id)->second;

    try {

        /* create the games */
        auto_ptr<vector<Game*> > games(tourney.tourney->match());

        if(games.get() == 0) {
            /* not ready yet */
            return;
        }

        /* create the notification message */
        TagGenerator generator;
        generator.openTag("start_game");
        generator.addAttribute("xmlns", XMLNS_CHESSD_TOURNEY);
        generator.openTag("game");
        generator.addAttribute("tourney_id", to_string(tourney_id));

        Stanza notification("iq");
        notification.subtype() = "set";
        notification.children().push_back(generator.getTag());

        Tag& game_tag = notification.firstTag().firstTag();

        /* send notifications to all players */
        foreach(game, *games) {
            /* TODO handle exception */

            /* get the players */
            vector<GamePlayer> players = (*game)->players();

            /* creat the game in the server core */
            Jid game_room = this->game_manager.createGame(*game,
                    boost::bind(&TourneyManager::reportResult, this,
                        tourney_id, _1, _2));

            /* set the game_room */
            game_tag.setAttribute("game_room", game_room.full());

            /* send note to the players */
            foreach(player, players) {
                notification.to() = player->jid;
                this->sendIq(new Stanza(notification));
            }
        }
    } catch(const tourney_over& over) {
        /* the tourney is over */
    }
}

void TourneyManager::reportResult(uint64_t tourney_id,
                                  int game_id,
                                  const vector<GamePlayerResult>& results) {
    /* tunnel to the manager's thread,
     * this function may be called from
     * another thread */
    this->dispatcher.queue(boost::bind(&TourneyManager::_reportResult,
                this, tourney_id, game_id, results));
}

void TourneyManager::_reportResult(uint64_t tourney_id,
                                   int game_id,
                                   const vector<GamePlayerResult>& results) {

    if(this->tourneys.count(tourney_id) == 0) {
        /* the tourney has been canceled */
        return;
    }

    /* the the tourney instance */
    TourneyStatus& tourney = *this->tourneys.find(tourney_id)->second;

    /* add the result in the tourney */
    tourney.tourney->addResult(results);

    /* start the next round if all games are over */
    this->startNextRound(tourney_id);
}



void TourneyManager::handleList(const Stanza& stanza) {
    /* send result with all tourney ids */
    auto_ptr<Stanza> result(stanza.createIQResult());
    TagGenerator generator;
    generator.openTag("list");
    generator.addAttribute("xmlns", XMLNS_CHESSD_TOURNEY);
    foreach(tourney, this->tourneys) {
        generator.openTag("tourney");
        generator.addAttribute("id", to_string(tourney->first));
        generator.addAttribute("running", tourney->second->running?"true":"false");
        generator.addAttribute("start_time",
                ptime_to_xmpp_date_time(
                    systime_to_ptime(tourney->second->start_time)));
        generator.closeTag();
    }
    result->children().push_back(generator.getTag());
    this->sendStanza(result.release());
}

void TourneyManager::handleJoin(const Stanza& stanza) {
    const Tag& query = stanza.firstTag();

    /* get tourney id */
    const Tag& tourney_attributes = query.firstTag();
    uint64_t id = parse_string<uint64_t>(tourney_attributes.getAttribute("id"));

    /* check if tourney exists */
    if(this->tourneys.count(id) == 0) {
        throw bad_request("Invalid tourney id");
    }

    /* get tourney */
    TourneyStatus& tourney = *this->tourneys.find(id)->second;

    if(tourney.running) {
        throw bad_request("Tourney already in progress");
    }

    /* insert player into tourney */
    /* FIXME: take partial jid and take correct rating */
    tourney.tourney->addPlayer(TourneyPlayer(stanza.from(), Rating()));

    /* send result confirming action */
    auto_ptr<Stanza> result(stanza.createIQResult());
    TagGenerator generator;
    generator.openTag("join");
    generator.addAttribute("xmlns", XMLNS_CHESSD_TOURNEY);
    result->children().push_back(generator.getTag());
    this->sendStanza(result.release());
}

void TourneyManager::onError(const std::string& error) {
    this->handleError(error);
}

void TourneyManager::onClose() {
}
