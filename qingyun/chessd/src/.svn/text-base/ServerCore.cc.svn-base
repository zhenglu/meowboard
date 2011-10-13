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

#include "ServerCore.hh"
#include "Util/utils.hh"

#include "MatchManager.hh"
#include "AdminComponent.hh"
#include "ProfileManager.hh"
#include "TourneyManager.hh"
#include "AnnouncementManager.hh"

#include "Exception.hh"
#include "XMPP/Exception.hh"

using namespace std;
using namespace XML;
using namespace XMPP;
using namespace Util;
using namespace Threads;

#define XMLNS_CHESSD_GAME "http://c3sl.ufpr.br/chessd#game"


ServerCore::ServerCore(
        const XML::Tag& config,
        DatabaseManager& database_manager,
        const XMPP::ErrorHandler& handle_error) :
    ComponentBase(config, "Chessd"),
    node_name(config.getAttribute("component_name")),
    database_manager(database_manager),
    handle_error(handle_error),
    game_ids(0)
{
    /* set a presence handler */
    this->root_node.setPresenceHandler(
            boost::bind(&ServerCore::handlePresence, this, _1));

    /* instaciate the match module */
    this->modules.push_back(new MatchManager(
                *this, database_manager,
                boost::bind(&Node::sendStanza, &this->root_node, _1)));

    /* instaciate the admin module */
    this->modules.push_back(new AdminComponent(
                config.getAttribute("server_name"),
                database_manager,
                boost::bind(&Node::sendStanza, &this->root_node, _1)));

    /* instaciate the profile module */
    this->modules.push_back(new ProfileManager(
                database_manager,
                boost::bind(&Node::sendStanza, &this->root_node, _1)));

    /* instaciate the tourney manager module */
    this->modules.push_back(new TourneyManager(
                *this,
                database_manager,
                boost::bind(&Node::sendStanza, &this->root_node, _1)));

    /* instaciate the annoucement manager module */
    this->modules.push_back(new AnnouncementManager(
                *this,
                database_manager,
                boost::bind(&Node::sendStanza, &this->root_node, _1)));

    /* register the modules's namespaces */
    foreach(module, this->modules) {
        vector<string> namespaces = module->namespaces();
        foreach(ns, namespaces) {
            this->root_node.setIqHandler(
                    boost::bind(&ServerModule::handleStanza, &*module, _1), *ns);
            this->root_node.disco().features().insert(*ns);
        }
    }
    this->root_node.disco().features().insert(XMLNS_CHESSD_GAME);

    /* the iq handler for basic services */
    this->root_node.setIqHandler(boost::bind(&ServerCore::handleIq, this, _1), XMLNS_CHESSD_GAME);
}

ServerCore::~ServerCore() {
}

void ServerCore::onConnect() {
    /* init all modules */
    foreach(module, this->modules) {
        module->start();
    }
}

void ServerCore::onClose() {
    /* stop all games */
    foreach(game, this->game_rooms) {
        game->second->stop();
    }
    this->game_rooms.clear();
    /* stop all modules */
    foreach(module, this->modules) {
        module->stop();
    }
    this->modules.clear();
    /* notify users */
    ReadLock<map<Jid, UserStatus> > status(this->users_status);
    Stanza stanza("presence");
    stanza.subtype() = "unavailable";
    foreach(user, *status) {
        if(user->second.available) {
            stanza.to() = user->first;
            this->sendStanza(new Stanza(stanza));
        }
    }
}

void ServerCore::onError(const string& msg) {
    this->handle_error(msg);
}

XMPP::Jid ServerCore::createGame(Game* _game,
        const OnGameEnd& on_game_end) {

    auto_ptr<Game> game(_game);
    
    /* Acquire lock to user status */
    WriteLock<map<Jid, UserStatus> > status(this->users_status);

    /* Check if the players can play the game */
    foreach(player, game->players()) {
        map<Jid, UserStatus>::iterator it = status->find(player->jid);

        if(it == status->end()) {
            throw create_game_error("User is unable to play the game");
        }

        /* the player must be available and to play more than one game
         * at a time, it must be multigame user */
        if(not it->second.canPlay()) {
            throw create_game_error("User is unable to play the game");
        }
    }

    /* now we increment the players game count and notify the modules */
    foreach(player, game->players()) {
        map<Jid, UserStatus>::iterator it = status->find(player->jid);
        it->second.games_playing++;
        this->notifyUserStatus(it->first, it->second);
    }

    /* we don't need it anymore, so release the lock */
    status.release();

    /* Get an game id, do it atomically because this function
     * can be called by multiple threads */
    GameId game_id = __sync_fetch_and_add(&this->game_ids, 1);

    /* exec waits the message to be executed, we do this here
     * becuse we want to make sure that the game jid is visible 
     * when this function returns */
    this->dispatcher.exec(boost::bind(&ServerCore::_createGame, this,
                game.release(), game_id, on_game_end));

    /* Create the room jid */
    return Jid("game_" + to_string(game_id), this->node_name);
}

void ServerCore::_createGame(Game* game,
        uint32_t game_id,
        const OnGameEnd& on_game_end) {
    /* create the game jid */
    Jid room_jid = Jid("game_" + to_string(game_id), this->node_name);

    /* Create the game room */
    auto_ptr<GameRoom> game_room (new GameRoom(game, room_jid,
            this->database_manager,
            GameRoomHandlers(boost::bind(&ComponentBase::sendStanza, this, _1),
                boost::bind(&ServerCore::closeGame, this, game_id),
                boost::bind(&ServerCore::endGame, this, game_id, game->players()),
                on_game_end)));

    /* Register the node */
    this->root_node.setNodeHandler(room_jid.node(),
            boost::bind(&GameRoom::handleStanza, &*game_room, _1));

    /* Add the new jabber node to the disco */
    this->root_node.disco().items().insert(new XMPP::DiscoItem(game->title(),
                room_jid));

    game_rooms.insert(game_id, game_room);
}

void ServerCore::endGame(GameId room_id, const vector<GamePlayer>& players) {
    this->dispatcher.queue(boost::bind(&ServerCore::_endGame, this, room_id, players));
}

void ServerCore::_endGame(GameId room_id, const vector<GamePlayer>& players) {
    Jid room_jid = Jid("game_" + to_string(room_id), this->node_name);

    /* erase the room from the disco items
     * so it is not visible anymore */
    this->root_node.disco().items().erase(room_jid);

    /* Acquire lock to user status */
    WriteLock<map<Jid, UserStatus> > status(this->users_status);

    /* now we decrement the players game count and notify the modules */
    foreach(player, players) {
        map<Jid, UserStatus>::iterator it = status->find(player->jid);
        it->second.games_playing--;
        this->notifyUserStatus(it->first, it->second);
    }

    /* we don't need it anymore, so release the lock */
    status.release();

}

void ServerCore::closeGame(GameId room_id) {
    this->dispatcher.queue(boost::bind(&ServerCore::_closeGame, this, room_id));
}

void ServerCore::_closeGame(GameId room_id) {
    Jid room_jid = Jid("game_" + to_string(room_id), this->node_name);
    this->root_node.removeNodeHandler(room_jid.node());
    this->root_node.disco().items().erase(room_jid);
    this->game_rooms.erase(room_id);
}

void ServerCore::handlePresence(const Stanza& stanza) {
    Jid user = stanza.from();
    bool available = false, multigame = false;
    if(stanza.subtype().empty()) {
        available = true;
    } else if(stanza.subtype() == "unavailable") {
        available = false;
    } else {
        return;
    }
    if(available) {
        try {
            /* check the config */
            const XML::Tag& config = stanza.findTag("config");
            /* read multigame parameter */
            if(config.getAttribute("multigame", "false") == "true") {
                multigame = true;
            }
        } catch(const xml_error&) {
            /* the config is optional, so if not present just ignore it */
        }
        this->database_manager.queueTransaction(boost::bind(&DatabaseInterface::getUserId,
                    _1, stanza.from().partial(), true));
    }
    WriteLock<map<Jid, UserStatus> > status(this->users_status);
    (*status)[user].available = available;
    (*status)[user].multigame = multigame;
    this->notifyUserStatus(user, (*status)[user]);
}

void ServerCore::notifyUserStatus(const Jid& user_name, const UserStatus& status) {
    foreach(module, this->modules) {
        module->notifyUserStatus(user_name, status);
    }
}

void ServerCore::handleIq(const Stanza& stanza) {
    try {
        const Tag& query = stanza.firstTag();

        if(query.name() == "search") {
            this->handleSearch(stanza);
        } else {
            throw bad_request("Wrong format");
        }
    } catch(xml_error& error) {
        throw bad_request("Wrong format");
    }
}

void ServerCore::handleSearch(const Stanza& stanza) {
    /* temporary simplification of search
     * search for only one jid */
    const Tag& query = stanza.findTag("search");
    const Tag& game = query.findTag("game");
    const Tag& player = game.findTag("player");
    Jid player_jid(player.getAttribute("jid"));

    /* message header */
    TagGenerator generator;
    generator.openTag("search");
    generator.addAttribute("xmlns", XMLNS_CHESSD_GAME);

    /* search games */
    foreach(game_room, game_rooms) {
        const Game& game = game_room->second->game();
        bool match = false;

        if(not game_room->second->isActive()) {
            continue;
        }

        /* check if the game is a match */
        foreach(player, game.players()) {
            if(player->jid == player_jid) {
                match = true;
                break;
            }
        }

        /* add the game to the message */
        if(match) {
            generator.openTag("game");
            generator.addAttribute("room", "game_" +
                    to_string(game_room->first));
            generator.closeTag();
        }
    }

    /* send the result stanza */
    auto_ptr<Stanza> result(stanza.createIQResult());
    result->children().push_back(generator.getTag());
    this->root_node.sendStanza(result.release());
}
