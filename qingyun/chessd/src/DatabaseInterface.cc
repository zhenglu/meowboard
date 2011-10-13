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

#include "DatabaseInterface.hh"

#include "Util/Date.hh"

using namespace boost::posix_time;
using namespace std;
using namespace Util;

DatabaseInterface::DatabaseInterface(pqxx::work& work) :
    work(work) { }

int DatabaseInterface::getUserId(const string& _username, bool create) {
    int user_id;
    pqxx::result result;
    string username = this->work.esc(_username);
    
    /* prepare the query */
    string query = "SELECT user_id "
                        "FROM users "
                        "WHERE user_name ='" + this->work.esc(username) + "'";
    
    /* execute the query */
    result = this->work.exec(query);

    /* if user is not in the database, create it */
    if(not result.empty()) {
        /* get the user id from te result */
        result[0]["user_id"].to(user_id);
    } else if(result.empty() and create) {
        /* prepare query */
        query = "INSERT INTO users (user_name) "
                "VALUES ('" + username + "') ";
        /* execute query */
        this->work.exec(query);
        /* get the id */
        this->work.exec("SELECT lastval()")[0][0].to(user_id);
    } else {
        throw user_not_found("User id not found");
    }

    return user_id;
}

string DatabaseInterface::getUsername(int user_id) {
    string query;
    string username;
    pqxx::result result;

    /* prepare the query */
    query = "SELECT user_name "
            "FROM users "
            "WHERE user_id=" + to_string(user_id);

    /* execute the query */
    result = this->work.exec(query);

    /* if the user is not found, throw an exception */
    if(result.empty()) {
        throw user_not_found("User id not found");
    }

    /* get the username from the result */
    result[0]["user_name"].to(username);

    return username;
}

vector<pair<string, PersistentRating> >
    DatabaseInterface::getRatings(const string& username,
                                  const string& category)
{
    string query;
    vector<pair<string, PersistentRating> > ratings;
    int user_id;

    try {
        user_id = this->getUserId(username, false);

        /* prepare SQL query */
        /* if no category is given, take all of them */
        if(category.empty()) {
            query =
                "SELECT * "
                " FROM player_rating "
                " WHERE user_id='" + to_string(user_id) + "'";
        } else {
            query = 
                "SELECT * "
                " FROM player_rating "
                " WHERE user_id='" + to_string(user_id) + "' AND";
            "       category='" + this->work.esc(category) + "'";
        }

        /* execute query */
        pqxx::result result = work.exec(query);

        /* read results */
        foreach(r, result) {
            PersistentRating rating;
            string category;
            r->at("category").to(category);
            r->at("rating").to(rating.rating);
            r->at("volatility").to(rating.volatility);
            r->at("wins").to(rating.wins);
            r->at("defeats").to(rating.defeats);
            r->at("draws").to(rating.draws);
            r->at("max_rating").to(rating.max_rating);
            rating.max_timestamp = from_time_t(r->at("max_timestamp").as<time_t>());
            r->at("last_game").to(rating.last_game);
            ratings.push_back(make_pair(category, rating));
        }
    } catch (const user_not_found&) {
        /* if the user is not found return an empty list */
    }
    return ratings;
}

string DatabaseInterface::getUserType(const string& user)
{
    /* prepare query */
    string query =
        "SELECT user_type"
        " FROM users"
        " WHERE user_name='" + this->work.esc(user) + "'";
    
    /* execute query */
    pqxx::result r = work.exec(query);

    /* read result */
    if(not r.empty()) {
        return r[0]["user_type"].c_str();
    } else {
        /* if nothing was found, default to user */
        return "user";
    }
}

PersistentRating
DatabaseInterface::getRatingForUpdate(const string& username,
                                   const string& category)
{
    int user_id = this->getUserId(username, true);

    /* prepare query */
    string query =
        "SELECT * "
        " FROM player_rating "
        " WHERE user_id=" + to_string(user_id) + " AND " +
        "       category='" + this->work.esc(category) + "' " +
        " FOR UPDATE";
    
    /* execute query */
    pqxx::result r = work.exec(query);

    /* read results */
    /* if not found, 
     * default to 0 */
    if(not r.empty()) {
        PersistentRating rating;
        r[0]["rating"].to(rating.rating);
        r[0]["volatility"].to(rating.volatility);
        r[0]["wins"].to(rating.wins);
        r[0]["defeats"].to(rating.defeats);
        r[0]["draws"].to(rating.draws);
        r[0]["max_rating"].to(rating.max_rating);
        rating.max_timestamp = from_time_t(r[0]["max_timestamp"].as<time_t>());
        r[0]["last_game"].to(rating.last_game);
        return rating;
    } else {
        return PersistentRating();
    }
}

void DatabaseInterface::setRating(const string& username,
                               const string& category,
                               const PersistentRating& rating)
{
    int user_id = this->getUserId(username, true);

    /* prepare query */
    string query =
        " UPDATE player_rating"
        " SET rating=" + to_string(rating.rating) +
        "    ,volatility=" + to_string(rating.volatility) +
        "    ,wins=" + to_string(rating.wins) +
        "    ,defeats=" + to_string(rating.defeats) +
        "    ,draws=" + to_string(rating.draws) +
        "    ,max_rating=" + to_string(rating.max_rating) +
        "    ,max_timestamp=" + to_string(Util::ptime_to_time_t(rating.max_timestamp)) +
        "    ,last_game=" + to_string(rating.last_game) +
        " WHERE user_id='" + to_string(user_id) + "' AND " +
        "       category='" + this->work.esc(category) + "'";
    
    /* execute query */
    pqxx::result r = work.exec(query);

    
    /* if the entry was not in the database
     * then create it */
    if(r.affected_rows() == 0) {

        query = 
            "INSERT INTO player_rating VALUES"
            "  ( " + to_string(user_id) + "," +
            "   '" + this->work.esc(category) + "'," +
            "    " + to_string(rating.rating) + ","
            "    " + to_string(rating.volatility) + ","
            "    " + to_string(rating.wins) + ","
            "    " + to_string(rating.draws) + ","
            "    " + to_string(rating.defeats) + ","
            "    " + to_string(rating.max_rating) + ","
            "    " + to_string(Util::ptime_to_time_t(rating.max_timestamp)) + ","
            "    " + to_string(rating.last_game) + ")";
        
        work.exec(query);
    }
}

int DatabaseInterface::insertGameResult(const GameResult& game_result)
{

    string category = game_result.category();
    Rating tmp;
    PersistentRating rating;
    PersistentGame game;

    /* create a map with the player's rating */
    if(game_result.isRated()) {
        map<XMPP::Jid, Rating> ratings;
        map<XMPP::Jid, PersistentRating> pratings;
        foreach(result, game_result.players()) {
            string name = result->player.jid.partial();
            rating = this->getRatingForUpdate(name, category);
            pratings.insert(make_pair(result->player.jid, rating));
            tmp.rating() = rating.rating;
            tmp.volatility() = rating.volatility;
            tmp.wins() = rating.wins;
            tmp.draws() = rating.draws;
            tmp.losses() = rating.defeats;
            tmp.last_game() = rating.last_game;
            ratings.insert(make_pair(result->player.jid, tmp));
        }

        /* update ratings */
        game_result.updateRating(ratings);

        /* create vector of persistent ratings */
        foreach(it, ratings) {
            rating.rating = it->second.rating();
            rating.volatility = it->second.volatility();
            rating.wins = it->second.wins();
            rating.draws = it->second.draws();
            rating.defeats = it->second.losses();
            rating.max_rating = pratings[it->first].max_rating;
            rating.max_timestamp = pratings[it->first].max_timestamp;
            rating.last_game = Util::ptime_to_time_t(ptime_local_time());
            if(rating.rating > rating.max_rating) {
                rating.max_rating = rating.rating;
                rating.max_timestamp = ptime_local_time();
            }
            this->setRating(it->first.partial(), category, rating);
        }
    }
    
    /* set game values */
    game.players = game_result.players();
    game.category = category;
    game.time_stamp = ptime_local_time();
    game.result = game_result.end_reason();

    /* get history */
    auto_ptr<XML::Tag> history(game_result.history());
    game.history = history->xml();

    /* insert to the database */
    return this->insertGame(game);
}

int DatabaseInterface::insertGame(const PersistentGame& game) {

    int game_id;
    string query;

    /* insert the game in the database */
    query =
            " INSERT INTO games (category, time_stamp, history, result) "
            " VALUES "
            "   ('" + this->work.esc(game.category) + "'" +
            "   , " + to_string(Util::ptime_to_time_t(game.time_stamp)) + "" +
            "   ,'" + this->work.esc(game.history) + "'"
            "   , " + to_string(int(game.result)) + ")";
    this->work.exec(query);

    /* get the game_id */
    this->work.exec("SELECT lastval()")[0][0].to(game_id);

 
    /* insert the players */
    foreach(result, game.players) {
        int user_id = this->getUserId(result->player.jid.partial(), true);
        query =
            " INSERT INTO game_players (game_id, user_id, result, role, time, inc)"
            " VALUES "
            "   ( " + to_string(game_id) +
            "   , " + to_string(user_id) +
            "   , " + to_string(int(result->result)) +
            "   , " + to_string(int(result->player.color)) + 
            "   , " + to_string(result->player.time.getSeconds()) +
            "   , " + to_string(result->player.inc.getSeconds()) + ")";
        work.exec(query);
    }

    return game_id;
}


string DatabaseInterface::getGameHistory(int game_id) {

    /* find the game in the database */
    string query = 
        "SELECT history FROM games WHERE game_id = " + to_string(game_id);
    pqxx::result result = this->work.exec(query);

    /* check result */
    if(result.empty()) {
        throw game_not_found("The requested game is not in the database");
    }

    return result[0]["history"].c_str();
}

vector<PersistentGame> DatabaseInterface::searchGames(
        const vector<pair<string, PLAYER_COLOR> > players,
        int time_begin, int time_end,
        int offset,
        int max_results)
{
    vector<PersistentGame> games;

    try {
        /* prepare sql query */
        string select =
            " SELECT games.game_id, category, time_stamp, games.result ";
        string from =
            " FROM games ";
        string where;

        /* XXX this is necessary in order to
         * accept an arbitrary number of players */
        for(int i=0;i<int(players.size());++i) {
            int user_id = this->getUserId(players[i].first, false);
            string id_str = Util::to_string(i);
            from += ", game_players g" + id_str + " ";
            if(not where.empty())
                where += " AND ";
            where += " games.game_id = g" + id_str + ".game_id AND g" +
                id_str + ".user_id = " + to_string(user_id) + " ";
            if(players[i].second != UNDEFINED) {
                where += " AND g" + id_str + ".role = "
                    + to_string(int(players[i].second)) + " ";
            }

        }
        /* set time interval search */
        if(time_begin != -1) {
            if(not where.empty())
                where += " AND ";
            where += " games.time_stamp >= " +
                to_string(time_begin) + " ";
        }

        /* set time interval search */
        if(time_end != -1) {
            if(not where.empty())
                where += " AND ";
            where += " games.time_stamp <= " +
                to_string(time_end) + " ";
        }

        if(not where.empty()) {
            where = " WHERE " + where;
        }


        string query = select + from + where +
            " ORDER BY game_id DESC " +
            " LIMIT " + to_string(max_results) +
            " OFFSET " + to_string(offset);

        /* execute query */
        pqxx::result result = this->work.exec(query);

        /* list results */
        foreach(r, result) {
            PersistentGame game;

            /*  parse values */
            r->at("game_id").to(game.id);
            game.result = END_CODE(r->at("result").as<int>());
            game.time_stamp = boost::posix_time::from_time_t(
                    r->at("time_stamp").as<int>());
            r->at("category").to(game.category);

            /* get players */
            string query =
                "SELECT * FROM game_players WHERE game_id = "
                + Util::to_string(game.id);
            pqxx::result result = this->work.exec(query);
            foreach(r, result) {
                GamePlayerResult result;
                int user_id;
                r->at("user_id").to(user_id);
                result.player.jid = XMPP::Jid(this->getUsername(user_id));
                result.player.color = PLAYER_COLOR(r->at("role").as<int>());
                result.player.time = Time::Seconds(r->at("time").as<int>());
                result.player.inc = Time::Seconds(r->at("inc").as<int>());
                result.result = GAME_RESULT(r->at("result").as<int>());
                game.players.push_back(result);
            }

            /* store game */
            games.push_back(game);
        }
    } catch (const user_not_found&) {
        /* ifthe user does not exists return an empty list */
    }
    return games;
}

void DatabaseInterface::insertAdjournedGame(const PersistentAdjournedGame& game)
{
    int game_id;

    string query;

    /* insert game */
    query =
            " INSERT INTO adjourned_games (category, time_stamp, history) VALUES"
            "   ('" + this->work.esc(game.category) + "'" +
            "   , " + to_string(Util::ptime_to_time_t(game.time_stamp)) + "" +
            "   ,'" + this->work.esc(game.history) + "')";
    this->work.exec(query);

    /* get the game id */
    this->work.exec("SELECT lastval()")[0][0].to(game_id);
 
    /* insert players */
    foreach(player, game.players) {
        int user_id = this->getUserId(player->jid.partial(), true);
        query =
            " INSERT INTO adjourned_game_players (game_id, user_id, role, time, inc)"
            " VALUES "
            "   ( " + to_string(game_id) +
            "   , " + to_string(user_id) +
            "   , " + to_string(int(player->color)) + 
            "   , " + to_string(player->time.getSeconds()) +
            "   , " + to_string(player->inc.getSeconds()) + ")";
        work.exec(query);
    }
}

string DatabaseInterface::getAdjournedGameHistory(int game_id) {
    string query;
    
    /* find game */
    query = "SELECT history FROM adjourned_games WHERE game_id = " + Util::to_string(game_id);
    pqxx::result result = this->work.exec(query);

    if(result.empty()) {
        throw game_not_found("The requested game is not in the database");
    }

    return result[0][0].c_str();
}

vector<PersistentAdjournedGame> DatabaseInterface::searchAdjournedGames(
                const vector<string>& players,
                int offset,
                int max_results)
{
    vector<PersistentAdjournedGame> games;

    try {
        string select =
            " SELECT adjourned_games.game_id, category, time_stamp ";
        string from =
            " FROM adjourned_games ";
        string where;

        /* prepare sql query */
        for(int i=0;i<int(players.size());++i) {
            int user_id = this->getUserId(players[i], false);
            string id_str = Util::to_string(i);
            from += ", adjourned_game_players g" + id_str + " ";
            if(i > 0)
                where += " AND ";
            else
                where = " WHERE ";
            where += " adjourned_games.game_id = g" + id_str
                + ".game_id AND g" + id_str + ".user_id = '"
                + to_string(user_id) + "' ";
        }
        string query = select + from + where + " limit " + Util::to_string(max_results) + " offset " + Util::to_string(offset);

        /* search games */
        pqxx::result result = this->work.exec(query);


        /* chek each result */
        foreach(r, result) {
            PersistentAdjournedGame game;
            r->at("game_id").to(game.id);
            game.time_stamp = boost::posix_time::from_time_t(r->at("time_stamp").as<time_t>());
            r->at("category").to(game.category);

            /* get players */
            string query =
                "SELECT * FROM adjourned_game_players WHERE game_id = "
                + to_string(game.id);
            pqxx::result result = this->work.exec(query);

            foreach(r, result) {
                GamePlayer player;
                int user_id;
                r->at("user_id").to(user_id);
                player.jid = XMPP::Jid(this->getUsername(user_id));
                player.color = PLAYER_COLOR(r->at("role").as<int>());
                player.time = Time::Seconds(r->at("time").as<int>());
                player.inc = Time::Seconds(r->at("inc").as<int>());
                game.players.push_back(player);
            }
            games.push_back(game);
        }
    } catch(const user_not_found&) {
        /* if a user is not fiund, just return an empty list */
    }
    return games;
}

void DatabaseInterface::eraseAdjournedGame(int game_id) {
    string query;

    /* erase game */
    query =
            " DELETE FROM adjourned_games WHERE "
            "   game_id=" + to_string(game_id);
    this->work.exec(query);
}

vector<string> DatabaseInterface::getAdmins() {

    vector<string> admins;
    string admin;

    /* prepare query */
    string query =
        " SELECT user_name"
        " FROM users "
        " WHERE user_type='admin'";

    /* execute query */
    pqxx::result result = this->work.exec(query);
    
    /* read result */
    foreach(r, result) {
        admins.push_back(r->at("user_name").c_str());
    }
    
    return admins;
}

vector<string> DatabaseInterface::getBannedWords() {
	vector<string> banned_words;

	string query= "SELECT word FROM banned_words" ;

	pqxx::result result= this->work.exec(query);

	foreach(r,result)
		banned_words.push_back(r->at("word").c_str());

	return banned_words;
}

void DatabaseInterface::setUserEmail(const string& username, const string& email) {
    string query;

    /* update info game */
    query =
            " UPDATE users SET email='" + this->work.esc(email) + "' WHERE "
            "   user_name='" + this->work.esc(username) + "'";

    this->work.exec(query);
}

void DatabaseInterface::updateOnlineTime(const string& username,
                                         int increment) {
    string query;

    int id = this->getUserId(username, true);

    /* update info game */
    query =
            " UPDATE users SET online_time=online_time+" + to_string(increment) + " WHERE "
            "   user_id=" + to_string(id) + "";

    this->work.exec(query);
}

int DatabaseInterface::getOnlineTime(const string& user) {
    int time;

    /* prepare query */
    string query =
        " SELECT online_time"
        " FROM users "
        " WHERE user_name='" + this->work.esc(user) + "'";

    /* execute query */
    pqxx::result result = work.exec(query);
    
    /* get result */
    if(result.empty()) {
        return 0;
    } else {
        result[0]["online_time"].to(time);
    }
    
    return time;
}

void DatabaseInterface::banUser(const string& username,
                                const string& reason) {
    int user_id;
    try {
        user_id = getUserId(username, false);
    } catch (const user_not_found& error) {
        return;
    }
    /* If the user user is already banned, we just update the reason */

    /* Try to update first */

    /* prepare query */
    string query =
        "UPDATE banned_users SET reason='" + this->work.esc(reason) + "' "
        "WHERE user_id=" + to_string(user_id);

    /* execute query */
    pqxx::result result = work.exec(query);

    if(result.affected_rows() == 0) {

        /* The update has failed, which means that the user
         * is not in the banned list, so we add him now */
    
        /* prepare query */
        query =
            "INSERT INTO banned_users (user_id, reason) "
            "VALUES ("  + to_string(user_id) + "," +
            "'" + this->work.esc(reason) + "')";

        work.exec(query);
    }
}

void DatabaseInterface::unbanUser(const string& username) {
    int user_id;
    try {
        user_id = getUserId(username, false);
    } catch (const user_not_found& error) {
        return;
    }
    /* Just delete the row with the user_id */

    /* prepare query */
    string query =
        "DELETE FROM banned_users "
        "WHERE user_id=" + to_string(user_id);

    /* execute query */
    work.exec(query);
}


void DatabaseInterface::banWord(const string& word) {
	string query = 
		"INSERT INTO banned_words (word) " 
		"VALUES ('" + word + "')";

	work.exec(query);
}

void DatabaseInterface::unbanWord(const string& word) {
	string query = 
		"DELETE FROM banned_words " 
		"WHERE word='" + word + "'";

	work.exec(query);
}

vector<pair<string, string> > DatabaseInterface::searchBannedUsers (
        const string& username,
        int offset,
        int max_results) {

    /* Search banned_users by joining it with users table */

    /* prepare query */
    string query =
        " SELECT users.user_name, banned_users.reason FROM users, banned_users "
        " WHERE users.user_id=banned_users.user_id "
        " AND users.user_name LIKE '" + this->work.esc(username) + "%'";

    if(max_results != -1) {
        query +=
            " ORDER BY users.user_id "
            " LIMIT " + to_string(max_results) +
            " OFFSET " + to_string(offset);
    }

    /* execute query */
    pqxx::result results = this->work.exec(query);
    
    vector<pair<string, string> > ret;

    foreach(res, results) {
        ret.push_back(make_pair(res->at("user_name").c_str(),
                                res->at("reason").c_str()));
    }

    return ret;
}

void DatabaseInterface::clearAnnouncements() {
    /* prepare query */
    string query = "DELETE FROM annoucements";

    /* execute query */
    this->work.exec(query);
}

void DatabaseInterface::eraseAnnouncement(int id) {
    /* prepare query */
    string query = "DELETE FROM annoucements WHERE id = " + to_string(id);

    /* execute query */
    this->work.exec(query);
}

void DatabaseInterface::insertAnnouncement(int id, const string& username,
        Util::Time time, int min_rating, int max_rating,
        const string& category) {

    /* get user id */
    int user_id = this->getUserId(username, true);

    /* prepare query */
    string query = "INSERT INTO annoucements (id, user_id, time, min_rating, "
                                             "max_rating, category) "
        " VALUES "
        "(  " + to_string(id) +
        " , " + to_string(user_id) +
        " , " + to_string(time.getSeconds()) +
        " , " + to_string(min_rating) +
        " , " + to_string(max_rating) +
        " ,'" + this->work.esc(category) + "')";

    /* execute query */
    this->work.exec(query);
}

vector<int> DatabaseInterface::searchAnnouncement(const string& username,
        const string& announcer, Util::Time min_time, Util::Time max_time,
        int max_results, int offset) {

    int user_id = -1, annoucer_id = -1;
    
    /* prepare query */
    string select = "SELECT an.id ";
    string from = "FROM annoucements as an ";
    string where;

    vector<int> ret;

    try {

        /* search announcements from a specific user */
        if(not announcer.empty()) {
            annoucer_id = this->getUserId(announcer, false);
            where += "an.user_id = " + to_string(annoucer_id) + " AND ";
        } else if(not username.empty()) {
            /* put rating restrictions to the one who is searching */
            user_id = this->getUserId(username, false);
            from += ", player_rating as pr ";
            where += "pr.user_id = " + to_string(user_id) + " AND ";
            where += "an.user_id <> " + to_string(user_id) + " AND ";
            where += "pr.category = an.category AND ";
            where += "pr.rating >= an.min_rating AND ";
            where += "pr.rating <= an.max_rating AND ";
        }


        /* put time lower bound */
        if(min_time.getSeconds() >= 0) {
            where += "an.time >= " + to_string(min_time.getSeconds()) + " AND ";
        }

        /* put time upper bound */
        if(max_time.getSeconds() >= 0) {
            where += "an.time <= " + to_string(max_time.getSeconds()) + " AND ";
        }

        /* erase extra AND */
        if(not where.empty()) {
            where.resize(where.size() - 4);
            where = " WHERE " + where;
        }

        /* limit results and add the offset */
        where += "ORDER BY an.id ";
        where += "LIMIT " + to_string(max_results) + " ";
        where += "OFFSET " + to_string(offset) + " ";

        /* finish the query */
        string query;
        query += select;
        query += from;
        query += where;

        /*  execute thequery*/
        pqxx::result results = this->work.exec(query);

        /* add results to the answer */
        foreach(result, results) {
            ret.push_back(result->at(0).as<int>());
        }

    } catch (const user_not_found&) {
        /* do nothing */
    }

    return ret;
}

vector<string> DatabaseInterface::searchUser(const string& pattern, int max_results) {
    pqxx::result result;
    
    /* prepare the query */
    string query = "SELECT user_name "
                        "FROM users "
                        "WHERE user_name LIKE '" + this->work.esc(pattern) + "%' "
                        "LIMIT " + to_string(max_results);
    
    /* execute the query */
    result = this->work.exec(query);

    vector<string> ret;

    foreach(it, result) {
        ret.push_back(it->at(0).c_str());
    }
    return ret;

}

