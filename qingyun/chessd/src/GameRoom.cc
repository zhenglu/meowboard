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

#include "GameRoom.hh"
#include "Util/utils.hh"
#include <boost/bind.hpp>
#include <boost/generator_iterator.hpp>

#include "XMPP/Exception.hh"
#include "GameException.hh"

#include "Util/Log.hh"

const char* XMLNS_GAME                  = "http://c3sl.ufpr.br/chessd#game";
const char* XMLNS_GAME_MOVE             = "http://c3sl.ufpr.br/chessd#game#move";
const char* XMLNS_GAME_RESIGN           = "http://c3sl.ufpr.br/chessd#game#resign";
const char* XMLNS_GAME_DRAW             = "http://c3sl.ufpr.br/chessd#game#draw";
const char* XMLNS_GAME_DRAW_DECLINE     = "http://c3sl.ufpr.br/chessd#game#draw-decline";
const char* XMLNS_GAME_CANCEL           = "http://c3sl.ufpr.br/chessd#game#cancel";
const char* XMLNS_GAME_CANCEL_DECLINE   = "http://c3sl.ufpr.br/chessd#game#cancel-decline";
const char* XMLNS_GAME_CANCELED         = "http://c3sl.ufpr.br/chessd#game#canceled";
const char* XMLNS_GAME_ADJOURN          = "http://c3sl.ufpr.br/chessd#game#adjourn";
const char* XMLNS_GAME_ADJOURN_DECLINE  = "http://c3sl.ufpr.br/chessd#game#adjourn-decline";
const char* XMLNS_GAME_START            = "http://c3sl.ufpr.br/chessd#game#start";
const char* XMLNS_GAME_STATE            = "http://c3sl.ufpr.br/chessd#game#state";
const char* XMLNS_GAME_END              = "http://c3sl.ufpr.br/chessd#game#end";

const int AGREEMENT_DRAW = 1;
const int AGREEMENT_CANCEL = 2;
const int AGREEMENT_ADJOURN = 3;
const int AGREEMENT_MOVE = 4;

using namespace XMPP;
using namespace Util;
using namespace std;

static const char* xmlns_table[] = {
    XMLNS_GAME_DRAW,
    XMLNS_GAME_CANCEL,
    XMLNS_GAME_ADJOURN,
    XMLNS_GAME_DRAW_DECLINE,
    XMLNS_GAME_CANCEL_DECLINE,
    XMLNS_GAME_ADJOURN_DECLINE
};

static const char game_end_type_table[][64] = {
    "normal",
    "canceled",
    "adjourned"
};

class xmpp_invalid_move : public xmpp_exception {
    public:
        xmpp_invalid_move(const string& what) : xmpp_exception(what) { }

        virtual Stanza* getErrorStanza(Stanza* stanza) const {
            return Stanza::createErrorStanza
                (stanza, "modify", "invalid-move", this->what(), XMLNS_GAME);
        }
};

class xmpp_game_over : public xmpp_exception {
    public:
        xmpp_game_over(const string& what) : xmpp_exception(what) { }

        virtual Stanza* getErrorStanza(Stanza* stanza) const {
            return Stanza::createErrorStanza
                (stanza, "modify", "game-over", this->what(), XMLNS_GAME);
        }
};

GameRoom::GameRoom(
        Game* game,
        const Jid& room_jid,
        DatabaseManager& database_manager,
        const GameRoomHandlers& handlers) :
    Muc(room_jid, game->title(), handlers.send_stanza),
    _game(game),
    database_manager(database_manager),
    handlers(handlers),
    game_active(true),
    start_time(Timer::now()),
    move_count(0)
{
    /* Set features */
    this->disco().features().insert(XMLNS_GAME);

    /* Set game iqs */
    this->setIqHandler(boost::bind(&GameRoom::handleGameIq, this, _1),
            XMLNS_GAME_MOVE);
    this->setIqHandler(boost::bind(&GameRoom::handleGameIq, this, _1),
            XMLNS_GAME_RESIGN);
    this->setIqHandler(boost::bind(&GameRoom::handleGameIq, this, _1),
            XMLNS_GAME_DRAW);
    this->setIqHandler(boost::bind(&GameRoom::handleGameIq, this, _1),
            XMLNS_GAME_DRAW_DECLINE);
    this->setIqHandler(boost::bind(&GameRoom::handleGameIq, this, _1),
            XMLNS_GAME_CANCEL);
    this->setIqHandler(boost::bind(&GameRoom::handleGameIq, this, _1),
            XMLNS_GAME_CANCEL_DECLINE);
    this->setIqHandler(boost::bind(&GameRoom::handleGameIq, this, _1),
            XMLNS_GAME_ADJOURN);
    this->setIqHandler(boost::bind(&GameRoom::handleGameIq, this, _1),
            XMLNS_GAME_ADJOURN_DECLINE);
    this->setIqHandler(boost::bind(&GameRoom::handleGameIq, this, _1),
            XMLNS_GAME_STATE);

    /* init agreements and timeouts */
    Time timeout = Timer::now() + Time::Minutes(5);
    foreach(player, game->players()) {
        this->agreement.insert(player->jid);
        this->all_players.insert(player->jid);
        this->player_timeout.insert(make_pair(player->jid, timeout));
    }

    /* set time check */
    this->dispatcher.schedule(boost::bind(&GameRoom::checkTime, this),
                              Timer::now() + Time::Seconds(10));

    /* start the dispatcher */
    this->dispatcher.start();

    this->setExtendedInfo();
}

GameRoom::~GameRoom() {
    /* stop the dispatcher */
    this->dispatcher.stop();
}

void GameRoom::stop() {
    this->dispatcher.exec(boost::bind(&GameRoom::onStop, this));
}

void GameRoom::setExtendedInfo() {
    /* Set extended info */
    XML::TagGenerator generator;
    generator.openTag("game");
	//FIXME
	//I think there should be a function fo get all atributes of the game
	//eliminating future problems here
    generator.addAttribute("category", this->game().category());
    generator.addAttribute("rated", (this->game().isRated()?"true":"false"));
    generator.addAttribute("moves", to_string(this->move_count));
    foreach(player, this->game().players()) {
        generator.openTag("player");
        generator.addAttribute("jid", player->jid.full());
        generator.addAttribute("role", PLAYER_ROLE_NAME[player->color]);
        generator.addAttribute("time", to_string(player->time.getSeconds()));
        generator.addAttribute("inc", to_string(player->inc.getSeconds()));
        generator.closeTag();
    }
    this->disco().setExtendedInfo(generator.getTag());
}

void GameRoom::onStop() {
    /* if the game is still active, ajourn it */
    if(this->game_active) {
        this->endGame(END_TYPE_ADJOURNED, END_ADJOURNED_SHUTDOWN);
    }
    /* close the room */
    Muc::close();
}

void GameRoom::checkTime() {
    Time now = Timer::now();

    /* check whether the time is over */
    if(this->game_active and this->_game->done(this->currentTime())) {
        this->endGame(END_TYPE_NORMAL);
    }

    /* check players timeout */
    if(this->game_active) {
        Time timeout = now;
        foreach(player, this->player_timeout) {
            timeout = min(timeout, player->second);
        }
        /* if someone timedout, end the game */
        if(timeout < now) {
            /* check all the players that timedout first */
            vector<Jid> timedout_players;
            foreach(player, this->player_timeout) {
                if(player->second == timeout) {
                    timedout_players.push_back(player->first);
                }
            }
            /* if everyone has timedout, cancel the game */
            if(timedout_players.size() == this->all_players.size()) {
                this->endGame(END_TYPE_CANCELED, END_CANCELED_TIMED_OUT);
            } else {
                /* give wo to absent users */
                this->_game->wo(timedout_players);
                this->endGame(END_TYPE_NORMAL);
            }
        }
    }

    /* check if we can close the game room
     * this is here to make sure the timer
     * will stop for sure */
    if(not this->game_active and this->occupants().size() == 0) {
        this->handlers.close_game();
    } else {
        /* set time check */
        this->dispatcher.schedule(boost::bind(&GameRoom::checkTime, this),
                Timer::now() + Time::Seconds(10));
    }
}

Stanza* GameRoom::createStateStanza() {
    auto_ptr<Stanza> stanza(new Stanza("iq"));
    XML::TagGenerator generator;

    stanza->subtype() = "set";
    generator.openTag("query");
    generator.addAttribute("xmlns", XMLNS_GAME_STATE);
    generator.addChild(this->_game->state(this->currentTime()));
    generator.addChild(this->_game->history());
    stanza->children().push_back(generator.getTag());
    return stanza.release();
}

void GameRoom::handleState(const Stanza& stanza) {
    auto_ptr<Stanza> result(this->createStateStanza());
    result->to() = stanza.from();
    result->id() = stanza.id();
    result->subtype() = "result";
    this->sendStanza(result.release());
}


void GameRoom::handleGameIq(const Stanza& stanza) {
    try {
        const string& xmlns = stanza.findTag("query").getAttribute("xmlns");

        /* is a room occupant? */
        if(not this->isOccupant(stanza.from()))
            throw not_acceptable("Only occupants are allowed to send queries to the game");

        /* is a state iq? */
        if(xmlns == XMLNS_GAME_STATE) {
            this->handleState(stanza);
            return;
        }

        /* is the sender a player and the game is active? */
        if(not has_key(this->all_players, stanza.from()))
            throw not_acceptable("Only players can do that");
        if(not this->game_active)
            throw not_acceptable("Game is not active");

        /* check xmlns and call the proper handler */
        if(xmlns == XMLNS_GAME_MOVE) {
            this->handleMove(stanza);
        } else if(xmlns == XMLNS_GAME_RESIGN) {
            this->handleResign(stanza);
        } else if(xmlns == XMLNS_GAME_DRAW) {
            this->handleDrawAccept(stanza);
        } else if(xmlns == XMLNS_GAME_DRAW_DECLINE) {
            this->handleDrawDecline(stanza);
        } else if(xmlns == XMLNS_GAME_CANCEL) {
            this->handleCancelAccept(stanza);
        } else if(xmlns == XMLNS_GAME_CANCEL_DECLINE) {
            this->handleCancelDecline(stanza);
        } else if(xmlns == XMLNS_GAME_ADJOURN) {
            this->handleAdjournAccept(stanza);
        } else if(xmlns == XMLNS_GAME_ADJOURN_DECLINE) {
            this->handleAdjournDecline(stanza);
        }

        /* check if the game is over */
        if(this->game_active and this->_game->done(this->currentTime())) {
            this->endGame(END_TYPE_NORMAL);
        }

        /* update the extended info */
        this->setExtendedInfo();
    } catch (const XML::xml_error& error) {
        throw bad_request("Bad format");
    }
}

void GameRoom::notifyState(const Jid& user) {
    auto_ptr<Stanza> stanza(this->createStateStanza());
    stanza->to() = user;
    stanza->subtype() = "set";
    this->sendIq(stanza.release());
}

Time GameRoom::currentTime() {
    return Timer::now() - this->start_time;
}

void GameRoom::handleMove(const Stanza& stanza) {
    try {
        /* parse message */
        const XML::Tag& move = stanza.query().findTag("move");
        string move_string = move.getAttribute("long");
        
        /* make the move */
        auto_ptr<XML::Tag> move_notification(this->_game->move(stanza.from(), move_string, this->currentTime()));
        this->move_count ++;
        this->agreement.agreed(AGREEMENT_MOVE, stanza.from());

        /* send the iq result */
        this->sendStanza(stanza.createIQResult());

        /* notify the players */
        this->notifyMove(move_notification.release());
    } catch (const time_over& error) {
        this->endGame(END_TYPE_NORMAL);
        throw xmpp_game_over("Time over");
    } catch (const game_exception& error) {
        throw xmpp_invalid_move(error.what());
    }
}

void GameRoom::handleResign(const Stanza& stanza) {
    this->_game->resign(stanza.from());
    this->sendStanza(stanza.createIQResult());
}

void GameRoom::handleDrawAccept(const Stanza& stanza) {

    /* send a iq result */
    this->sendStanza(stanza.createIQResult());

    /* check if this is the first offer, if so notify the other players */
    if(this->agreement.agreed_count(AGREEMENT_DRAW) == 0) {
        this->notifyRequest(REQUEST_DRAW, stanza.from());
    }

    /* set player status as agreed */
    this->agreement.agreed(AGREEMENT_DRAW, stanza.from());

    /* check if all players agreed on a draw */
    if(this->agreement.left_count(AGREEMENT_DRAW) == 0) {
        this->_game->draw();
    }
}

void GameRoom::handleDrawDecline(const Stanza& stanza) {
    this->agreement.clear(AGREEMENT_DRAW);
    this->sendStanza(stanza.createIQResult());
    this->notifyRequest(REQUEST_DRAW_DECLINE, stanza.from());
}

void GameRoom::handleCancelAccept(const Stanza& stanza) {

    /* send a iq result */
    this->sendStanza(stanza.createIQResult());

    /* set player status as agreed */
    if(this->agreement.agreed(AGREEMENT_CANCEL, stanza.from())) {
        /* check if all players agreed on a cancel 
         * or not all players moved yet */
        if(this->agreement.left_count(AGREEMENT_CANCEL) == 0 or
                this->agreement.left_count(AGREEMENT_MOVE) > 0) {
            this->endGame(END_TYPE_CANCELED, END_CANCELED_AGREEMENT);
        } else if(this->agreement.agreed_count(AGREEMENT_CANCEL) == 1) {
        /* check if this is the first offer, if so notify the other players */
            this->notifyRequest(REQUEST_CANCEL, stanza.from());
        }

    }

}

void GameRoom::handleCancelDecline(const Stanza& stanza) {
    this->agreement.clear(AGREEMENT_CANCEL);
    this->sendStanza(stanza.createIQResult());
    this->notifyRequest(REQUEST_CANCEL_DECLINE, stanza.from());
}

void GameRoom::handleAdjournAccept(const Stanza& stanza) {

    /* send a iq result */
    this->sendStanza(stanza.createIQResult());

    /* check if this is the first offer, if so notify the other players */
    if(this->agreement.agreed_count(AGREEMENT_ADJOURN) == 0) {
        this->notifyRequest(REQUEST_ADJOURN, stanza.from());
    }

    /* set player status as agreed */
    this->agreement.agreed(AGREEMENT_ADJOURN, stanza.from());

    /* check if all players agreed on a draw */
    if(this->agreement.left_count(AGREEMENT_ADJOURN) == 0) {
        this->endGame(END_TYPE_ADJOURNED, END_ADJOURNED_AGREEMENT);
    }

}

void GameRoom::handleAdjournDecline(const Stanza& stanza) {
    this->agreement.clear(AGREEMENT_ADJOURN);
    this->sendStanza(stanza.createIQResult());
    this->notifyRequest(REQUEST_ADJOURN_DECLINE, stanza.from());
}

void GameRoom::notifyRequest(GameRequest request, const Jid& requester) {
    /* create message */
    Stanza stanza("iq");
    XML::Tag* query = new XML::Tag("query");
    stanza.subtype()="set";
    query->setAttribute("xmlns", xmlns_table[request]);
    stanza.children().push_back(query);
    /* send message to occupants */
    foreach(player, this->all_players) {
        if(*player != requester) {
            stanza.to() = *player;
            this->sendIq(new Stanza(stanza));
        }
    }
}

Stanza* GameRoom::createResultStanza(const string& lang) {

    XML::TagGenerator tag_generator;
    auto_ptr<Stanza> stanza(new Stanza("iq"));
    stanza->subtype() = "set";
    stanza->lang() = lang;
    tag_generator.openTag("query");
    tag_generator.addAttribute("xmlns", XMLNS_GAME_END);
    /* TODO */
    tag_generator.openTag("end");
    tag_generator.addAttribute("type", game_end_type_table[this->end_type]);
    tag_generator.addAttribute("result", game_end_reason_table[this->result_reason]);

    /* results are available only if the game ended normally, 
     * eg. was not canceled nor adjourned */
    if(this->end_type == END_TYPE_NORMAL) {
        foreach(player, this->players_result) {
            tag_generator.openTag("player");
            tag_generator.addAttribute("jid", player->player.jid.full());
            tag_generator.addAttribute("role", PLAYER_ROLE_NAME[player->player.color]);
            tag_generator.addAttribute("result", PLAYER_RESULT_NAME[player->result]);
            tag_generator.closeTag();
        }
    }

    tag_generator.closeTag();
    stanza->children().push_back(tag_generator.getTag());
    return stanza.release();
}

Stanza* GameRoom::createMoveStanza(XML::Tag* move_tag) {
    XML::TagGenerator tag_generator;
    Stanza* stanza = new Stanza("iq");
    stanza->subtype() = "set";
    tag_generator.openTag("query");
    tag_generator.addAttribute("xmlns", XMLNS_GAME_MOVE);
    tag_generator.addChild(move_tag);
    tag_generator.addChild(this->_game->state(this->currentTime()));
    stanza->children().push_back(tag_generator.getTag());
    return stanza;
}

void GameRoom::notifyMove(XML::Tag* move_tag) {
    /* create message */
    auto_ptr<Stanza> stanza(createMoveStanza(move_tag));
    /* send to all occupants */
    this->broadcastIq(*stanza);
}

void storeResult(const GameResult& result, int& game_id, DatabaseInterface& database) {
    game_id = database.insertGameResult(result);
}

void storeAdjourned(const AdjournedGame& adj_game, DatabaseInterface& database) {
    PersistentAdjournedGame game;
    auto_ptr<XML::Tag> history(adj_game.history());

    /* set game values */
    game.category = adj_game.category();
    game.players = adj_game.players();
    game.history = history->xml();
    game.time_stamp = boost::posix_time::second_clock::local_time();
    
    /* insert game to the database */
    database.insertAdjournedGame(game);
}

void GameRoom::broadcastResultStanza() {
    foreach(occupant, this->occupants()) {
        this->notifyResult(occupant->jid());
    }
}

void GameRoom::endGame(GameEndType type, END_CODE end_code) {
    /* set game status */
    this->game_active = false;
    this->end_type = type;

    /* hide the game from other users */
    this->handlers.hide_game();

    int game_id = -1;

    foreach(player, this->_game->players()) {
        this->players_result.push_back(GamePlayerResult(*player));
    }

    if(type == END_TYPE_NORMAL) {
        /* take the game result */
        auto_ptr<GameResult> result(this->_game->result());

        /* set result */
        this->result_reason = result->end_reason();
        this->players_result = result->players();

        /* store game in the database */
        this->database_manager.execTransaction(
                boost::bind(storeResult, boost::ref(*result), boost::ref(game_id), _1));

    } else if(type == END_TYPE_CANCELED) {
        /* set result */
        this->result_reason = end_code;

    } else if(type == END_TYPE_ADJOURNED) {
        /* get the adjourned game */
        auto_ptr<AdjournedGame> adj_game(this->_game->adjourn(
                    this->currentTime()));

        /* set result */
        this->result_reason = end_code;

        /* store game */
        this->database_manager.execTransaction(boost::bind(storeAdjourned, boost::ref(*adj_game), _1));
    }

    if(not handlers.report_result.empty()) {
        handlers.report_result(game_id, this->players_result);
    }

    /* notify result */
    this->broadcastResultStanza();
}

void GameRoom::notifyResult(const Jid& user) {
    auto_ptr<Stanza> stanza(this->createResultStanza(this->occupants().find_jid(user)->lang()));
    stanza->to() = user;
    this->sendIq(stanza.release());
}

void GameRoom::notifyUserStatus(const Jid& jid, const string& nick, bool available) {
    if(available) {
        /* send to the user the game state */
        this->notifyState(jid);
        if(not this->game_active) {
            this->notifyResult(jid);
        }

        /* if the user is a player, update timeout to infinite */
        if(this->all_players.count(jid) > 0) {
            this->player_timeout[jid] = Timer::now() + Time::Hours(1000000);
        }
    } else {
        /* if the user is a player, update timeout 5 minutes from now */
        if(this->all_players.count(jid) > 0) {
            this->player_timeout[jid] = Timer::now() + Time::Minutes(5);
        }
    }
}

void GameRoom::handleStanza(Stanza* stanza) throw() {
    this->dispatcher.queue(boost::bind(&GameRoom::_handleStanza, this, stanza));
}

void GameRoom::_handleStanza(Stanza* stanza) {
    Muc::handleStanza(stanza);
}
