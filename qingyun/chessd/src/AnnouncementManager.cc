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
#include "AnnouncementManager.hh"
#include "Util/utils.hh"

#include "MatchAnnouncementFactory.hh"

#include "XMPP/Exception.hh"
#include "Exception.hh"

#include "Util/Date.hh"

#include "GameException.hh"

#define XMLNS_CHESSD_MATCH_ANNOUNCEMENT     "http://c3sl.ufpr.br/chessd#match_announcement"

using namespace std;
using namespace boost;
using namespace XML;
using namespace XMPP;
using namespace Util;

AnnouncementManager::AnnouncementManager(
        ServerCore& game_manager,
        DatabaseManager& database,
        const StanzaHandler& send_stanza) :
    ServerModule(send_stanza),
    game_manager(game_manager),
    database(database),
    announcement_ids(0)
{
    /* erase all announcements in tha database */
    this->database.execTransaction(boost::bind(
                &DatabaseInterface::clearAnnouncements, _1));
}

AnnouncementManager::~AnnouncementManager() {
}

vector<string> AnnouncementManager::namespaces() const {
    vector<string> ret;
    ret.push_back(XMLNS_CHESSD_MATCH_ANNOUNCEMENT);
    return ret;
}

void AnnouncementManager::handleIq(const Stanza& stanza) {
    try {
        const Tag& query = stanza.firstTag();
        const string& command = query.name();
        if(command == "create") {
            this->handleCreate(stanza);
        } else if(command == "delete") {
            this->handleDelete(stanza);
        } else if(command == "accept") {
            this->handleAccept(stanza);
        } else if(command == "search") {
            this->handleSearch(stanza);
        } else {
            throw feature_not_implemented("Unknown command");
        }
    } catch(const xml_error& error) {
        throw bad_request("wrong format");
    }
}

void AnnouncementManager::handleCreate(const Stanza& stanza) {
    try {
        const Tag& query = stanza.firstTag();
        const Tag& announcement_tag = query.firstTag();

        int min_rating = 0, max_rating = 100000;

        /* create the announcment */
        TeamDatabase tmp;
        auto_ptr<MatchAnnouncement> announcement(
                MatchAnnouncementFactory::create(announcement_tag, tmp));

        /* sanity check */
        if(stanza.from() != announcement->players()[0].jid) {
            throw bad_request("You can not create an annoucement in the name of another player");
        }

        /* get rating limits */
        if(announcement_tag.hasAttribute("minimum_rating")) {
            min_rating = parse_string<int>(
                    announcement_tag.getAttribute("minimum_rating"));
        }
        if(announcement_tag.hasAttribute("maximum_rating")) {
            max_rating = parse_string<int>(
                    announcement_tag.getAttribute("maximum_rating"));
        }

        /* get an id */
        uint64_t id = this->announcement_ids++;

        /* insert the announcement into the database */
        this->database.queueTransaction(boost::bind(
                    &DatabaseInterface::insertAnnouncement, _1, id,
                    stanza.from().partial(), announcement->players()[0].time,
                    min_rating, max_rating, announcement->category()));

        /* insert to the annoucement list */
        this->announcements.insert(id, announcement.release());

        /* send a result with the annoucement id */
        auto_ptr<Stanza> result(stanza.createIQResult());
        TagGenerator generator;
        generator.openTag("create");
        generator.addAttribute("xmlns", XMLNS_CHESSD_MATCH_ANNOUNCEMENT);
        generator.openTag("announcement");
        generator.addAttribute("id", to_string(id));
        result->children().push_back(generator.getTag());
        this->sendStanza(result.release());

    } catch (const game_exception& error) {
        throw bad_request(error.what());
    }
}

void AnnouncementManager::handleSearch(const Stanza& stanza) {
    this->database.queueTransaction(boost::bind(
                &AnnouncementManager::searchAnnouncement, this, _1, stanza));
}

void AnnouncementManager::searchAnnouncement(DatabaseInterface& database, const Stanza& stanza) {
    try {
        try {
            const Tag& search_tag = stanza.firstTag().firstTag();

            /* parse the message */

            Time minimum_time = Time::Seconds(-1);

            Time maximum_time = Time::Seconds(-1);

            PartialJid player;

            PartialJid from = stanza.from();

            int offset, results;

            /* check time lower bound */
            if(search_tag.hasAttribute("minimum_time")) {
                minimum_time = Time::Seconds(search_tag.getAttribute("minimum_time"));
            }

            /* check time upper bound */
            if(search_tag.hasAttribute("maximum_time")) {
                maximum_time = Time::Seconds(search_tag.getAttribute("maximum_time"));
            }

            /* check for a player */
            if(search_tag.hasAttribute("player")) {
                player = PartialJid(search_tag.getAttribute("player"));
            }

            /* check for number of results */
            if(search_tag.hasAttribute("results")) {
                results = min(50, parse_string<int>(search_tag.getAttribute("results")));
            } else {
                results = 10;
            }

            /* check for offset */
            if(search_tag.hasAttribute("offset")) {
                offset = parse_string<int>(search_tag.getAttribute("offset"));
            } else {
                offset = 0;
            }

            /* perform the search */
            vector<int> ids = database.searchAnnouncement(from.full(),
                    player.full(), minimum_time, maximum_time, results, offset);

            TagGenerator generator;
            generator.openTag("search");
            generator.addAttribute("xmlns", XMLNS_CHESSD_MATCH_ANNOUNCEMENT);
            foreach(id, ids) {
				//XXX
				if(this->announcements.find(*id)!=this->announcements.end()) {
					ptr_map<uint64_t, MatchAnnouncement>::iterator it =
						this->announcements.find(*id);

					/* add announcement to the result */
					auto_ptr<Tag> announce_tag(it->second->notification());
					announce_tag->setAttribute("id", to_string(*id));
					generator.addChild(announce_tag.release());
				}
				else
					std::cerr << "Error on Announcement in getting id " << *id << std::endl;
            }

            /* create message */
            auto_ptr<Stanza> result(stanza.createIQResult());
            result->children().push_back(generator.getTag());

            /* send message  */
            this->sendStanza(result.release());
        } catch(const xml_error& error) {
            throw bad_request("wrong format");
        }
    } catch(const XMPP::xmpp_exception& error) {
        this->sendStanza(error.getErrorStanza(new Stanza(stanza)));
    }
}

void AnnouncementManager::handleDelete(const Stanza& stanza) {
    const Tag& query = stanza.firstTag();
    const Tag& announcement_tag = query.firstTag();
    uint64_t id = parse_string<uint64_t>(announcement_tag.getAttribute("id"));

    /* get the announcent by its id */
    ptr_map<uint64_t, MatchAnnouncement>::iterator it = this->announcements.find(id);

    /* check if the annoucement was found */
    if(it == this->announcements.end()) {
        throw item_not_found("No such id");
    }

    /* check if the requester is the owner */
    /* FIXME, using the first player is not a good idea */
    if(it->second->players()[0].jid != stanza.from()) {
        throw not_acceptable("Only the owner can do that");
    }

    /* erase from the list */
    this->announcements.erase(it);

    /* delete the announcement from the database */
    this->database.queueTransaction(boost::bind(
                &DatabaseInterface::eraseAnnouncement, _1, id));

    /* send a the result confirming */
    auto_ptr<Stanza> result(stanza.createIQResult());
    TagGenerator generator;
    generator.openTag("delete");
    generator.addAttribute("xmlns", XMLNS_CHESSD_MATCH_ANNOUNCEMENT);
    result->children().push_back(generator.getTag());
    this->sendStanza(result.release());
}

void AnnouncementManager::handleAccept(const Stanza& stanza) {
    try {
        const Tag& query = stanza.firstTag();
        const Tag& announcement_tag = query.firstTag();
        uint64_t id = parse_string<uint64_t>(announcement_tag.getAttribute("id"));

        /* get the announcent by its id */
        ptr_map<uint64_t, MatchAnnouncement>::iterator it = this->announcements.find(id);

        /* check if the annoucement was found */
        if(it == this->announcements.end()) {
            throw item_not_found("No such id");
        }

        /* check if the requester is not the owner */
        /* FIXME, using the first player is not a good idea */
        if(it->second->players()[0].jid == stanza.from()) {
            throw not_acceptable("You can't accept your own announcement");
        }

        /* create the game */
        auto_ptr<Game> game(it->second->createGame(stanza.from()));

        /* delete the annoucement */
        this->announcements.erase(it);

        /* delete the announcement from the database */
        this->database.queueTransaction(boost::bind(
                    &DatabaseInterface::eraseAnnouncement, _1, id));

        /* get the players list */
        vector<GamePlayer> players = game->players();

        /* create the game room */
        Jid game_room = this->game_manager.createGame(game.release());

        /* send a the result confirming */
        auto_ptr<Stanza> result(stanza.createIQResult());
        TagGenerator generator;
        generator.openTag("accept");
        generator.addAttribute("xmlns", XMLNS_CHESSD_MATCH_ANNOUNCEMENT);
        result->children().push_back(generator.getTag());
        this->sendStanza(result.release());

        /* send notifications */
        this->notifyGame(players, game_room);


    } catch (const create_game_error& error) {
        throw bad_request(error.what());
    }
}

void AnnouncementManager::notifyGame(const vector<GamePlayer>& players,
                                     const Jid& room) {

    /* create the notification message */
    Stanza stanza("iq");
    stanza.subtype() = "set";
    TagGenerator generator;
    generator.openTag("start_game");
    generator.addAttribute("xmlns", XMLNS_CHESSD_MATCH_ANNOUNCEMENT);
    generator.openTag("game_room");
    generator.addAttribute("jid", room.full());
    stanza.children().push_back(generator.getTag());

    /* send to all players */
    foreach(player, players) {
        stanza.to() = player->jid;
        this->sendIq(new Stanza(stanza));
    }
}

void AnnouncementManager::handleUserStatus(const Jid& user, const UserStatus& status) {
    if(not status.available) {
        ptr_map<uint64_t, MatchAnnouncement>::iterator it;
        for(it = this->announcements.begin(); it != this->announcements.end();) {
            if(it->second->players()[0].jid == user) {
                /* delete the announcement from the database */
                this->database.queueTransaction(boost::bind(
                            &DatabaseInterface::eraseAnnouncement, _1, it->first));
                /* erase the annoucement from the list */
                this->announcements.erase(it++);
            } else {
                ++it;
            }
        }
    }
}
