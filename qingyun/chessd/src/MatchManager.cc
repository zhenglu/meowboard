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
#include "MatchManager.hh"
#include "Threads/Task.hh"
#include "Util/utils.hh"

#include "MatchAdjournFactory.hh"
#include "MatchFactory.hh"

#include "XMPP/Exception.hh"
#include "Exception.hh"
#include "GameException.hh"

#include "Util/Date.hh"

#define XMLNS_MATCH         "http://c3sl.ufpr.br/chessd#match"
#define XMLNS_MATCH_OFFER   "http://c3sl.ufpr.br/chessd#match#offer"
#define XMLNS_MATCH_ACCEPT  "http://c3sl.ufpr.br/chessd#match#accept"
#define XMLNS_MATCH_DECLINE "http://c3sl.ufpr.br/chessd#match#decline"

#define XMLNS_ADJOURNED_LIST "http://c3sl.ufpr.br/chessd#adjourned#list"

using namespace std;
using namespace XML;
using namespace XMPP;
using namespace Util;

class match_error : public xmpp_exception {
    public:
        match_error(const std::string& what) :
            xmpp_exception(what) { }

        virtual Stanza* getErrorStanza(Stanza* stanza) const {
            return Stanza::createErrorStanza
                (stanza, "modify", "bad-request", this->what());
        }
};

static void eraseAdjGame(int adj_id, DatabaseInterface& database) {
    database.eraseAdjournedGame(adj_id);
}

/*! \brief A wrapper for an adjourned match that erases the
 * from the database when the game starts */
class AdjournedWrapper : public Match {
    public:
        AdjournedWrapper(int adj_id, DatabaseManager& database, Match* match) :
            adj_id(adj_id),
            database(database),
            match(match) { }

        ~AdjournedWrapper() { }

        const std::vector<GamePlayer>& players() const { return this->match->players(); }
        const std::string& category() const { return this->match->category(); }
        Tag* notification() const { return this->match->notification(); }

        Game* createGame() const {
            /* Erase the game from the database */
            this->database.queueTransaction(boost::bind(eraseAdjGame, adj_id, _1));
            /* Create the game */
            return match->createGame();
        }
    private:

        int adj_id;
        DatabaseManager& database;
        std::auto_ptr<Match> match;
};

MatchManager::MatchManager(
        ServerCore& game_manager,
        DatabaseManager& database,
        const StanzaHandler& send_stanza) :
    ServerModule(send_stanza),
    game_manager(game_manager),
    database(database) { }

MatchManager::~MatchManager() {
    vector<int> matchs = this->match_db.getActiveMatchs();
    foreach(match_id, matchs) {
        this->closeMatch(*match_id, false);
    }
}

void MatchManager::handleIq(const Stanza& stanza) {
    try {
        const Tag& query = stanza.firstTag();
        const string& xmlns = query.getAttribute("xmlns");
        if(xmlns == XMLNS_MATCH_OFFER) {
            this->handleOffer(stanza);
        } else if(xmlns == XMLNS_MATCH_ACCEPT) {
            this->handleAccept(stanza);
        } else if(xmlns == XMLNS_MATCH_DECLINE) {
            this->handleDecline(stanza);
        } else if(xmlns == XMLNS_ADJOURNED_LIST) {
            this->handleList(stanza);
        } else {
            throw feature_not_implemented("Unknown xmlns");
        }
    } catch(const xml_error& error) {
        throw bad_request("wrong format");
    }
}

vector<string> MatchManager::namespaces() const {
    vector<string> ret;
    ret.push_back(XMLNS_MATCH_OFFER);
    ret.push_back(XMLNS_MATCH_ACCEPT);
    ret.push_back(XMLNS_MATCH_DECLINE);
    ret.push_back(XMLNS_MATCH_DECLINE);
    ret.push_back(XMLNS_ADJOURNED_LIST);
    return ret;
}

void MatchManager::sendOfferResult(const Stanza& stanza, int match_id) {
    Stanza* resp = (stanza.createIQResult());

    TagGenerator generator;
    generator.openTag("query");
    generator.addAttribute("xmlns", XMLNS_MATCH_OFFER);
    generator.openTag("match");
    generator.addAttribute("id", to_string(match_id));
    resp->children().push_back(generator.getTag());

    this->sendStanza(resp);
}

void MatchManager::handleOffer(const Stanza& stanza) {
    try {

        /* parse message */
        const Tag& offer = stanza.query().findTag("match");

        /* Is the offer to resume an adjourned game? */
        if(offer.hasAttribute("adjourned_id")) {
            int adj_id = parse_string<int>(offer.getAttribute("adjourned_id"));
            this->delayed_offer.push_back(make_pair(adj_id, new Stanza(stanza)));
            this->database.queueTransaction(boost::bind(&MatchManager::loadAdjourned, this, adj_id, _1));
        } else {

            /* apply match rule */
            std::auto_ptr<Match> match(MatchFactory::create(offer, this->teams));

            /* The processOffer was part of this function.
             * It was separated due to the need to
             * load an adjourned game asynchronously */
            this->processOffer(stanza, match.release());
        }
    } catch (const xml_error& error) {
        throw bad_request(error.what());
    } catch (const game_exception& error) {
        throw bad_request(error.what());
    }
}

void MatchManager::processOffer(const Stanza& stanza, Match* _match) {
    try {
        std::auto_ptr<Match> match(_match);

        /* parse message */
        const Tag& offer = stanza.query().findTag("match");
        int match_id;

        /* is it a rematch? */
        if(offer.hasAttribute("id")) {
            match_id = parse_string<int>(offer.getAttribute("id"));
        } else {
            match_id = -1;
        }

        /* check if everyone is available and if the sender is in the match */
        bool valid = false;
        foreach(player, match->players()) {
            if(not this->canPlay(player->jid))
                throw match_error("User is unable to play the game");
            if(player->jid == stanza.from())
                valid = true;
        }
        if(not valid)
            throw match_error("You must be among the players in the match");

        /* check if there are no repeated users in the match */
        foreach(it1, match->players()) {
            foreach_it(it2, next(it1), match->players().end()) {
                if(it1->jid.parcialCompare(it2->jid))
                    throw match_error("Users must not repeat in the match");
            }
        }

        /* If an id is given, then it is a rematch */
        if(match_id != -1) {
            if(not this->match_db.hasMatch(match_id))
                throw match_error("Invalid match id");
            const Match& old_match = this->match_db.getMatch(match_id);

            std::vector<GamePlayer> old_player_list = old_match.players();
            std::vector<GamePlayer> player_list = match->players();

            sort(old_player_list.begin(), old_player_list.end());
            sort(player_list.begin(), player_list.end());

            if(old_player_list != player_list) {
                throw match_error("The players in the match must not change in a rematch");
            }
            this->match_db.replaceMatch(match_id, match.release());
        } else {
            match_id = this->match_db.insertMatch(match.release());
        }

        /* the offer is ok, now add it to the offer list and notify users */
        this->match_db.acceptMatch(match_id, stanza.from());
        this->sendOfferResult(stanza, match_id);
        this->notifyOffer(match_id, stanza.from());

    } catch (const xml_error& error) {
        throw bad_request(error.what());
    } catch (const game_exception& error) {
        throw bad_request(error.what());
    }
}

void MatchManager::resumeOffer(int adj_id, const std::string& history) {
    std::auto_ptr<Stanza> stanza;
    try {
        /* if the history is empty, it means that the id does not exist */
        if(history.empty()) {
            throw(xmpp_exception("Invalid adjourned game id"));
        }

        /* now we are to resume an adjourned game */
        /* find the offer message */
        foreach(offer, this->delayed_offer) {
            if(offer->first == adj_id) {
                stanza = std::auto_ptr<Stanza>(offer->second);
                this->delayed_offer.erase(offer);
                break;
            }
        }

        /* create the match and proces it */
        std::auto_ptr<Tag> history_tag(parseXmlString(history));
        std::auto_ptr<Match> match(
                new AdjournedWrapper(
                        adj_id,
                        this->database,
                        MatchAdjournFactory::create(history_tag.release())));
        this->processOffer(*stanza, match.release());

    } catch (xmpp_exception& error) {
        this->sendStanza(error.getErrorStanza(stanza.release()));
    }
}

void MatchManager::notifyOffer(int id, const Jid& requester) {
    /* get match */
    const Match& match = this->match_db.getMatch(id);

    /* create message */
    TagGenerator generator;
    Tag* tag = 0;
    Stanza stanza("iq");
    stanza.subtype() = "set";
    generator.openTag("query");
    generator.addAttribute("xmlns", XMLNS_MATCH_OFFER);
    tag = match.notification();
    tag->setAttribute("id", to_string(id));
    generator.addChild(tag);
    stanza.children().push_back(generator.getTag());

    /* send the message to the players */
    foreach(player, match.players()) {
        if(player->jid != requester) {
            stanza.to() = player->jid;
            this->sendIq(new Stanza(stanza));
        }
    }
}

void MatchManager::handleAccept(const Stanza& stanza) {
    try {
        /* get match */
        const Tag& match = stanza.query().findTag("match");
        /* parse message */
        int id = parse_string<int>(match.getAttribute("id"));
        /* update accepted */
        this->match_db.acceptMatch(id, stanza.from());
        /* reply result */
        this->sendStanza(stanza.createIQResult());
        /* check whether everyone has already accepted */
        if(this->match_db.isDone(id)) {
            this->closeMatch(id, true);
        }
    } catch (const xml_error& error) {
        throw bad_request("Invalid format");
    } catch (const user_error& error) {
        throw bad_request(error.what());
    }
}

void MatchManager::handleDecline(const Stanza& stanza) {
    try {
        /* get match */
        const Tag& match = stanza.query().findTag("match");
        /* parse message */
        int id = parse_string<int>(match.getAttribute("id"));
        /* sanity check */
        if(not this->match_db.hasPlayer(id, stanza.from()))
            throw match_error("Invalid match id");
        /* reply result */
        this->sendStanza(stanza.createIQResult());
        /* close the match */
        this->closeMatch(id, false);
    } catch (const xml_error& error) {
        throw bad_request("Invalid format");
    } catch (const user_error& error) {
        throw match_error(error.what());
    }
}

void MatchManager::closeMatch(int id, bool accepted) {
    std::auto_ptr<Match> match(this->match_db.closeMatch(id));
    if(accepted) {
        /* close all other matchs if the user is not multi game */
        foreach(player, match->players()) {
            if(this->isMultigameUser(player->jid)) {
                set<int> matchs = this->match_db.getPlayerMatchs(player->jid);
                foreach(id, matchs) {
                    this->closeMatch(*id, false);
                }
            }
        }
        /* create the game */
        Game* game = match->createGame();
        Jid game_room = this->game_manager.createGame(game);

        this->notifyGameStart(id, match.release(), game_room);
    } else {
        this->notifyResult(*match, id, accepted);
    }
}

void MatchManager::notifyGameStart(int match_id, Match* match, const Jid& jid) {
    /* the game has been created
     * we need to tell the game room
     * to the players */

    /* create message */
	TagGenerator generator;
    Stanza stanza("iq");
    stanza.subtype() = "set";
	generator.openTag("query");
	generator.addAttribute("xmlns", XMLNS_MATCH_ACCEPT);
	generator.openTag("match");
	generator.addAttribute("id", to_string(match_id));
	generator.addAttribute("room", jid.full());

    /* send message to the players */
    stanza.children().push_back(generator.getTag());
    foreach(player, match->players()) {
        stanza.to() = player->jid;
        this->sendIq(new Stanza(stanza));
    }
    delete match;
}

void MatchManager::notifyResult(const Match& match, int id, bool accepted) {
    /* create the message */
	TagGenerator generator;
    Stanza stanza("iq");
    stanza.subtype() = "set";
	generator.openTag("query");
	generator.addAttribute("xmlns", accepted ? XMLNS_MATCH_ACCEPT : XMLNS_MATCH_DECLINE);
	generator.openTag("match");
	generator.addAttribute("id", to_string<int>(id));

    /* send to the players */
    stanza.children().push_back(generator.getTag());
    foreach(player, match.players()) {
        stanza.to() = player->jid;
        this->sendIq(new Stanza(stanza));
    }
}

void MatchManager::handleList(const Stanza& query) {
    /* we have to consult the database here */
    this->database.queueTransaction(boost::bind(&MatchManager::listAdjournedGames, this, query, _1));
}

void MatchManager::listAdjournedGames(const Stanza& query, DatabaseInterface& database) {
    try {
        try {
            std::vector<std::string> players;
            int offset = 0;
            int max_results = 50;
            std::vector<PersistentAdjournedGame> games;
            TagGenerator generator;

            /* Parse request */
            const Tag& search_tag = query.query().findTag("search");
            offset = parse_string<int>(search_tag.getAttribute("offset"));
            max_results = min(max_results, parse_string<int>(search_tag.getAttribute("results")));
            players.push_back(query.from().partial());

            /* Search in the database */
            games = database.searchAdjournedGames(players, offset, max_results);

            /* Create result */
            std::auto_ptr<Stanza> resp(query.createIQResult());
            generator.openTag("query");
            generator.addAttribute("xmlns", XMLNS_ADJOURNED_LIST);
            foreach(game, games) {
                generator.openTag("game");
                generator.addAttribute("category", game->category);
                generator.addAttribute("id", to_string(game->id));
                generator.addAttribute("time_stamp", ptime_to_xmpp_date_time(game->time_stamp));
                foreach(player, game->players) {
                    generator.openTag("player");
                    generator.addAttribute("jid", player->jid.partial());
                    generator.addAttribute("role", PLAYER_ROLE_NAME[player->color]);
                    generator.addAttribute("time", to_string(player->time.getSeconds()));
                    generator.addAttribute("inc", to_string(player->inc.getSeconds()));
                    generator.closeTag();
                }
                generator.closeTag();
            }
            resp->children().push_back(generator.getTag());

            /* send the message */
            this->sendStanza(resp.release());
        } catch (const xml_error& error) {
            throw bad_request("Invalid format");
        }
    } catch (const xmpp_exception& error) {
        this->sendStanza(error.getErrorStanza(new Stanza(query)));
    }
}

void MatchManager::loadAdjourned(int game_id, DatabaseInterface& database) {
    try {
        /* get game from the database */
        std::string history = database.getAdjournedGameHistory(game_id);
        this->dispatcher.queue(boost::bind(&MatchManager::resumeOffer, this, game_id, history));

    } catch (const game_not_found& error) {

        /* on error send an empty history */
        this->dispatcher.queue(boost::bind(&MatchManager::resumeOffer, this, game_id, ""));
    }
}

void MatchManager::handleUserStatus(const Jid& user, const UserStatus& status) {
    /* if user has gone offline, close all is offers */
    if(not status.available) {
        set<int> matchs = this->match_db.getPlayerMatchs(user);
        foreach(id, matchs) {
            this->closeMatch(*id, false);
        }
    }
}
