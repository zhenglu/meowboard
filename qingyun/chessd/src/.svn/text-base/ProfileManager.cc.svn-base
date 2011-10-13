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
#include "ProfileManager.hh"
#include "Util/utils.hh"
#include "Util/Date.hh"

#include "Exception.hh"
#include "XMPP/Exception.hh"

#include "HistoryProcess.hh"

using namespace std;
using namespace XML;
using namespace XMPP;
using namespace Util;
using namespace Threads;

const char* XMLNS_CHESSD_INFO        = "http://c3sl.ufpr.br/chessd#info";
const char* XMLNS_CHESSD_PROFILE     = "http://c3sl.ufpr.br/chessd#profile";
const char* XMLNS_CHESSD_SEARCH_GAME = "http://c3sl.ufpr.br/chessd#search_game";
const char* XMLNS_CHESSD_SEARCH_USER = "http://c3sl.ufpr.br/chessd#search_user";
const char* XMLNS_CHESSD_FETCH_GAME  = "http://c3sl.ufpr.br/chessd#fetch_game";

PLAYER_COLOR translate_role(const std::string& role) {
    if(role == "white") {
        return WHITE;
    } else if(role == "black") {
        return BLACK;
    } else {
        return UNDEFINED;
    }
}

ProfileManager::ProfileManager(
        DatabaseManager& database,
        const XMPP::StanzaHandler& send_stanza) :
    ServerModule(send_stanza),
    database(database)
{ }

ProfileManager::~ProfileManager() {
}

vector<string> ProfileManager::namespaces() const {
    vector<string> ret;
    ret.push_back(XMLNS_CHESSD_INFO);
    ret.push_back(XMLNS_CHESSD_SEARCH_GAME);
    ret.push_back(XMLNS_CHESSD_FETCH_GAME);
    ret.push_back(XMLNS_CHESSD_PROFILE);
    ret.push_back(XMLNS_CHESSD_SEARCH_USER);
    return ret;
}

void ProfileManager::handleIq(const Stanza& iq) {
    try {
        const Tag& query = iq.firstTag();
        const string& xmlns = query.getAttribute("xmlns");

        if(xmlns == XMLNS_CHESSD_INFO) {
            this->handleRating(iq);
        } else if(xmlns == XMLNS_CHESSD_SEARCH_GAME) {
            this->handleSearchGame(iq);
        } else if(xmlns == XMLNS_CHESSD_FETCH_GAME) {
            this->handleFetchGame(iq);
        } else if(xmlns == XMLNS_CHESSD_SEARCH_USER) {
            this->handleSearchUser(iq);
        } else if(xmlns == XMLNS_CHESSD_PROFILE) {
            this->handleProfile(iq);
        } else {
            throw feature_not_implemented("Unknown xmlns");
        }
    } catch (const xml_error& error) {
        throw bad_request("Invalid format");
    }
}

void ProfileManager::handleRating(const Stanza& stanza) {
    const Tag& query = stanza.query();

    /* check if the format is correct */
    foreach(tag, query.tags()) {
        if(tag->name() == "rating") {
            if(not tag->hasAttribute("jid"))
                throw XMPP::bad_request("Invalid format");
        } else if(tag->name() == "type") {
            if(not tag->hasAttribute("jid"))
                throw XMPP::bad_request("Invalid format");
        } else {
            throw XMPP::bad_request("Invalid format");
        }
    }

    /* execute the transaction */
    this->database.queueTransaction(boost::bind(&ProfileManager::fetchRating, this, stanza, _1));
}

void ProfileManager::handleSearchGame(const Stanza& stanza) {
    /* execute the transaction */
    this->database.queueTransaction(boost::bind(&ProfileManager::searchGame, this, stanza, _1));
}

void ProfileManager::handleSearchUser(const Stanza& stanza) {
    /* execute the transaction */
    this->database.queueTransaction(boost::bind(&ProfileManager::searchUser, this, stanza, _1));
}

void ProfileManager::handleFetchGame(const Stanza& stanza) {
    /* execute the transaction */
    this->database.queueTransaction(boost::bind(&ProfileManager::fetchGame, this, stanza, _1));
}

void ProfileManager::handleProfile(const Stanza& stanza) {
    if(stanza.subtype() == "set") {
        this->database.queueTransaction(boost::bind(&ProfileManager::updateProfile, this, stanza, _1));
    } else {
        this->database.queueTransaction(boost::bind(&ProfileManager::fetchProfile, this, stanza, _1));
    }
}

void ProfileManager::updateProfile(const Stanza& stanza, DatabaseInterface& database) {
    try{
        try {
            std::string user = stanza.from().partial();

            const Tag& query = stanza.query();

            foreach(tag, query.tags()) {
                if(tag->name() == "email") {
                    database.setUserEmail(user, tag->findCData().data());
                }
            }
        } catch (const XML::xml_error& error) {
            throw XMPP::bad_request("Invalid format");
        }
    } catch (const XMPP::xmpp_exception& error) {
        this->sendStanza(error.getErrorStanza(new Stanza(stanza)));
    }
}

void ProfileManager::searchGame(const Stanza& stanza, DatabaseInterface& database) {
    try  {
        try {
            XML::TagGenerator generator;
            std::vector<std::pair<std::string, PLAYER_COLOR> > players;
            unsigned int max_results = 50;
            unsigned int offset = 0;
            int time_begin = -1, time_end = -1;
            bool has_more = false;

            /* Parse request */
            const Tag& query = stanza.findTag("query");
            const Tag& search_tag = query.findTag("search");
            if(search_tag.hasAttribute("offset")) {
                offset = parse_string<int>(search_tag.getAttribute("offset"));
            }
            if(search_tag.hasAttribute("results")) {
                max_results = min(50, parse_string<int>(search_tag.getAttribute("results")));
            }
            foreach(field, search_tag.tags()) {
                if(field->name() == "player") {
                    const std::string& username = field->getAttribute("jid");
                    PLAYER_COLOR role = translate_role(field->getAttribute("role", ""));
                    players.push_back(make_pair(username, role));
                } else if (field->name() == "date") {
                    std::string begin = field->getAttribute("begin", "");
                    std::string end = field->getAttribute("end", "");
                    if(not begin.empty()) {
                        time_begin = xmpp_date_time_to_time_t(begin);
                    }
                    if(not end.empty()) {
                        time_end = xmpp_date_time_to_time_t(end);
                    }
                }
            }

            /* Search in the database */
            std::vector<PersistentGame> games =
                database.searchGames(players, time_begin, time_end,
                        offset, max_results + 1);

            if(games.size() > max_results) {
                has_more = true;
                games.pop_back();
            }

            /* Create result */
            generator.openTag("iq");
            generator.addAttribute("to", stanza.from().full());
            generator.addAttribute("from", stanza.to().full());
            generator.addAttribute("id", stanza.id());
            generator.addAttribute("type", "result");
            generator.openTag("query");
            generator.addAttribute("xmlns", XMLNS_CHESSD_SEARCH_GAME);

            foreach(game, games) {
                generator.openTag("game");
                generator.addAttribute("id", to_string(game->id));
                generator.addAttribute("category", game->category);
                generator.addAttribute("time_stamp", ptime_to_xmpp_date_time(game->time_stamp));
                generator.addAttribute("result", game_end_reason_table[game->result]);

                foreach(player, game->players) {
                    generator.openTag("player");
                    generator.addAttribute("jid", player->player.jid.partial());
                    generator.addAttribute("role", PLAYER_ROLE_NAME[player->player.color]);
                    generator.addAttribute("result", PLAYER_RESULT_NAME[player->result]);
                    generator.addAttribute("time", to_string(player->player.time.getSeconds()));
                    generator.addAttribute("inc", to_string(player->player.inc.getSeconds()));
                    generator.closeTag();
                }
                generator.closeTag();
            }

            if(has_more) {
                generator.openTag("more");
                generator.closeTag();
            }

            /* Send result */
            this->sendStanza(new XMPP::Stanza(generator.getTag()));
        } catch (const XML::xml_error& error) {
            throw XMPP::xmpp_exception("Bad format");
        } catch (const boost::bad_lexical_cast& error) {
            throw XMPP::xmpp_exception("Bad date");
        } catch (const std::out_of_range& error) {
            throw XMPP::xmpp_exception("Date is out of range");
        }
    } catch (const XMPP::xmpp_exception& error) {
        this->sendStanza(error.getErrorStanza(new Stanza(stanza)));
    }
}

void ProfileManager::searchUser(const Stanza& stanza, DatabaseInterface& database) {
    try {
        try {
            XML::TagGenerator generator;

            /* Parse request */
            const Tag& query = stanza.findTag("query");
            const Tag& user_tag = query.findTag("user");
            string name = user_tag.getAttribute("name");
            vector<string> users = database.searchUser(name, 50);

            /* Create result */
            generator.openTag("iq");
            generator.addAttribute("to", stanza.from().full());
            generator.addAttribute("from", stanza.to().full());
            generator.addAttribute("id", stanza.id());
            generator.addAttribute("type", "result");
            generator.openTag("query");
            generator.addAttribute("xmlns", XMLNS_CHESSD_SEARCH_USER);
            foreach(it, users) {
                generator.openTag("user");
                generator.addAttribute("jid", *it);
                generator.closeTag();
            }

            /* send the result to the player */
            this->sendStanza(new XMPP::Stanza(generator.getTag()));
        } catch(const XML::xml_error& error) {
            throw XMPP::bad_request("Invalid format");
        }
    } catch (const XMPP::xmpp_exception& error) {
        this->sendStanza(error.getErrorStanza(new Stanza(stanza)));
    }
}

void ProfileManager::fetchGame(const Stanza& stanza, DatabaseInterface& database) {
    try {
        try {
            XML::TagGenerator generator;

            /* Parse request */
            const Tag& query = stanza.findTag("query");
            const Tag& game_tag = query.findTag("game");
            int game_id = parse_string<int>(game_tag.getAttribute("id"));
            std::string game_history = database.getGameHistory(game_id);

            /* Create result */
            generator.openTag("iq");
            generator.addAttribute("to", stanza.from().full());
            generator.addAttribute("from", stanza.to().full());
            generator.addAttribute("id", stanza.id());
            generator.addAttribute("type", "result");
            generator.openTag("query");
            generator.addAttribute("xmlns", XMLNS_CHESSD_FETCH_GAME);
            generator.addChild(HistoryProcess::generate(XML::parseXmlString(game_history)));

            /* send the result to the player */
            this->sendStanza(new XMPP::Stanza(generator.getTag()));
        } catch(const XML::xml_error& error) {
            throw XMPP::bad_request("Invalid format");
        } catch(const game_not_found&) {
            throw XMPP::bad_request("Game not found");
        }
    } catch (const XMPP::xmpp_exception& error) {
        this->sendStanza(error.getErrorStanza(new Stanza(stanza)));
    }
}

void ProfileManager::fetchRating(const Stanza& stanza, DatabaseInterface& database) {
    XML::TagGenerator generator;
    const Tag& query = stanza.findTag("query");
    std::string category, jid;

    /* create message */
    generator.openTag("iq");
    generator.addAttribute("to", stanza.from().full());
    generator.addAttribute("from", stanza.to().full());
    generator.addAttribute("id", stanza.id());
    generator.addAttribute("type", "result");
    generator.openTag("query");
    generator.addAttribute("xmlns", XMLNS_CHESSD_INFO);

    /* for each requested info, consult the database */
    foreach(tag, query.tags()) {
        if(tag->name() == "rating") {
            /* parse tag */
            category = tag->hasAttribute("category") ? tag->getAttribute("category") : "";
            jid = tag->getAttribute("jid");

            /* consult the database */
            std::vector<std::pair<std::string, PersistentRating> > ratings = database.getRatings(
                        jid, category);

            /* add result to the message */
            foreach(rating, ratings) {
                generator.openTag("rating");
                generator.addAttribute("jid", jid);
                generator.addAttribute("category", rating->first);
                generator.addAttribute("rating", to_string(rating->second.rating));
                generator.addAttribute("wins", to_string(rating->second.wins));
                generator.addAttribute("draws", to_string(rating->second.draws));
                generator.addAttribute("losses", to_string(rating->second.defeats));
                generator.addAttribute("max_rating", to_string(rating->second.max_rating));
                generator.addAttribute("max_timestamp", ptime_to_xmpp_date_time(rating->second.max_timestamp));
                generator.closeTag();
            }
        } else if(tag->name() == "type") {
            /* parse message */
            jid = tag->getAttribute("jid");

            /* consult the database */
            std::string type = database.getUserType(jid);

            /* create the tag */
            generator.openTag("type");
            generator.addAttribute("jid", jid);
            generator.addAttribute("type", type);
            generator.closeTag();
        }
    }
    generator.closeTag();
    generator.closeTag();
    this->sendStanza(new XMPP::Stanza(generator.getTag()));
}

void ProfileManager::fetchProfile(const Stanza& stanza, DatabaseInterface& database) {
    XML::TagGenerator generator;
    const Tag& query = stanza.findTag("query");
    std::string category, user;

    ReadLock<map<PartialJid, Time> > logons(this->last_logons);

    /* create message */
    generator.openTag("iq");
    generator.addAttribute("to", stanza.from().full());
    generator.addAttribute("from", stanza.to().full());
    generator.addAttribute("id", stanza.id());
    generator.addAttribute("type", "result");
    generator.openTag("query");
    generator.addAttribute("xmlns", XMLNS_CHESSD_PROFILE);

    /* for each requested info, consult the database */
    foreach(tag, query.tags()) {
        if(tag->name() == "profile") {
            /* parse tag */
            user = tag->getAttribute("jid");

            /* get the user ratings */
            std::vector<std::pair<std::string, PersistentRating> > ratings = database.getRatings(
                        user, category);

            /* create the tag */
            generator.openTag("profile");
            generator.addAttribute("jid", user);

            /* add result to the message */
            foreach(rating, ratings) {
                generator.openTag("rating");
                generator.addAttribute("category", rating->first);
                generator.addAttribute("rating", to_string(rating->second.rating));
                generator.addAttribute("wins", to_string(rating->second.wins));
                generator.addAttribute("draws", to_string(rating->second.draws));
                generator.addAttribute("losses", to_string(rating->second.defeats));
                generator.addAttribute("max_rating", to_string(rating->second.max_rating));
                generator.addAttribute("max_timestamp", ptime_to_xmpp_date_time(rating->second.max_timestamp));
                generator.closeTag();
            }

            /* get the user type */
            std::string type = database.getUserType(user);

            /* add the type to the message */
            generator.openTag("type");
            generator.addAttribute("type", type);
            generator.closeTag();

            /* get the user's online time */
            int online_time = database.getOnlineTime(user);

            generator.openTag("online_time");
            generator.addAttribute("seconds", to_string(online_time));
            generator.closeTag();

            /* get the user's uptime */
            if_find(it, PartialJid(user), *logons) {
                int uptime = (Timer::now() - it->second).getSeconds();
                generator.openTag("uptime");
                generator.addAttribute("seconds", to_string(uptime));
                generator.closeTag();
            }

            generator.closeTag(); // profile
        }
    }
    generator.closeTag();
    generator.closeTag();
    this->sendStanza(new XMPP::Stanza(generator.getTag()));
}

void ProfileManager::handleUserStatus(const Jid& user, const UserStatus& status) {

    WriteLock<map<PartialJid, Time> > logons(this->last_logons);

    if(status.available) {
        /* the user is online */
        if(logons->find(user) == logons->end()) {
            logons->insert(make_pair(user, Timer::now()));
        }
    } else {
        /* the user went offline */
        if(logons->find(user) != logons->end()) {
            this->database.queueTransaction(
                boost::bind(
                    &ProfileManager::updateOnlineTime,
                    this,
                    user,
                    (Timer::now() -
                     logons->find(user)->second).getSeconds(),
                    _1));
            logons->erase(user);
        }
    }
}

void ProfileManager::updateOnlineTime(const XMPP::PartialJid& user,
                                       int increment,
                                       DatabaseInterface& database) {
    database.updateOnlineTime(user.full(), increment);
}

void ProfileManager::onStop() {
    /* stop counting online time, set every one to offline */
    WriteLock<map<PartialJid, Time> > logons(this->last_logons);
    Time now = Timer::now();
    foreach(logon, *logons) {
        this->database.queueTransaction(
                boost::bind(
                    &ProfileManager::updateOnlineTime,
                    this, logon->first,
                    (now - logon->second).getSeconds(), _1));
    }
    logons->clear();
}
