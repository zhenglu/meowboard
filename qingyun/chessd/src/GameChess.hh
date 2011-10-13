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

#ifndef GAMECHESS_HH
#define GAMECHESS_HH

#include "Game.hh"
#include "Util/Timer.hh"
#include "libchess/xiangqi.hh"
#include "XMPP/Jid.hh"
#include <vector>

/*
 * Instead of having GameChess and GameChessUntimed,
 * there should be Game Chess, GameChessUntimed and
 * GameChessTimed which includes standard, blitz and
 * lightining categories
*/
class GameChessUntimed : public Game {
	public:
		GameChessUntimed(const std::vector<GamePlayer>& _players, const XML::AttributeMap &__attributes);

		/*! \brief Constructor for adjourned games
		 * \description it assumes that adjourned games are not over */
		GameChessUntimed(XML::Tag* adjourned_game);

		virtual ~GameChessUntimed() {};

		//! \brief generates state tag as specified in chessd protocol
		virtual XML::Tag* state(const Util::Time& current_time) const;

		virtual XML::Tag* history() const;

        virtual const std::vector<GamePlayer>& players() const;

		virtual const std::string& category() const;

		/*! \brief function to return the title of the game
		 * \return returns the full jids of the player separated by an "x" */
		virtual const std::string& title() const;

		virtual bool isRated() const;

		/*! \brief The player has resigned */
		virtual void resign(const XMPP::Jid& player);

		/*! \brief The players have lost by W.O. */
		virtual void wo(const std::vector<XMPP::Jid>& wos);

		/*! \brief The player has called a flag */
		virtual void call_flag(const Util::Time& current_time); //TODO

		/*! \brief The players agreed on a draw */
		virtual void draw();

		virtual AdjournedGame* adjourn(const Util::Time& current_time);

		/*! \brief Has the game ended?
		 * \return true if the game is over of false if it is not over
		 * \description it checks if the time is over and the value of this->_done
		 * since this function can be called several times, this->done was created
		 * for optimizations, an it's set on functions move,resign and draw()
		 */
		virtual bool done(const Util::Time& current_time) ;

		virtual GameResult* result() const ;

		virtual XML::Tag* move(const XMPP::Jid& player, const std::string& movement, const Util::Time& time_stamp);

	protected:
		/*! \brief returns the end reason if the game has ended*/
		//virtual std::string doneEndReason() const;

		/*! \brief returns the team result list if the game has ended*/
		virtual std::vector<GamePlayerResult> donePlayerResultList() const;

		virtual int whoTimedOver() const { return -1;}

		int realDone();

		//stores the attributes of the game
		XML::AttributeMap game_attributes;
		
		//! \brief generates history tag as specified in chessd protocol
		virtual XML::Tag* generateHistoryTag(Util::Time time_passed=Util::Time()) const;

		//Chess class from libchess
		Xiangqi chess;

		//stores the time of the last move
		//at the beginning of the game or at restart time_of_last_time is 0
		Util::Time time_of_last_move;

		virtual void interpretHistoryMoves(const std::string& moves);

		bool auto_flag;
		/*
		 * for optimizations reasons, the reason why the game is over is set on move
		 * and when done is called, it's only needed to see the value of this variable
		 * and if the time is over
		*/
		END_CODE _done;

		/*
		 * if a player resigned, stores his color
		 * Chess::UNDEFINED for none resigned
		 * Chess::WHITE for white resigned
		 * Chess::BLACK for black resigned
		*/
		Xiangqi::Color _resign;

		//stores the title of the game "jid white x jid black"
		std::string _title;

		/*
		 * libchess is based in color, and this API is based on jid, so
		 * this map make the correlations between jid and color
		*/
		std::map<XMPP::Jid,Xiangqi::Color> colormap;

		/*
		 * the best structure to deal with chessplayers is the GamePlayer
		 * and this API is based on Player(JID), so this correlation is done here
		*/
		std::map<XMPP::Jid,GamePlayer*> standard_player_map;

		//a list of GamePlayers
		std::vector<GamePlayer> _players;

		//stores the history of the game
		std::string history_moves;

	private:
		//\brief set initial variables, it is just called in the constructor
		void setInitialVariables();
};

class GameChess: public GameChessUntimed {
	public:
		GameChess(const std::vector<GamePlayer>& _players, const XML::AttributeMap& category);

		/*! \brief Constructor for adjourned games
		 * \description it assumes that adjourned games are not over */
		GameChess(XML::Tag* adjourned_game);

		virtual XML::Tag* state(const Util::Time& current_time) const;

		virtual void call_flag(const Util::Time& current_time);

		virtual bool done(const Util::Time& current_time) ;

		virtual XML::Tag* move(const XMPP::Jid& player, const std::string& movement, const Util::Time& time_stamp);

	protected:
		virtual int whoTimedOver() const { return this->time_over;};

		virtual XML::Tag* generateHistoryTag(Util::Time time_passed=Util::Time()) const;

		virtual void interpretHistoryMoves(const std::string& moves);

		virtual std::vector<GamePlayerResult> donePlayerResultList() const;
	private:
		//\brief check if the time of current player is over,
		//if yes, then sed this->_done and this->time_over
		bool checkTimeOver(const Util::Time& current_time);

		/*
		 * tells if the time of one player is over
		 * -1 for no time over
		 *  0 white's time is over
		 *  1 black's time is over
		*/
		int time_over;

		/*
		 * store the initial time of the game
		*/
		int initial_time;

		/*
		 * The time is only counted after 2 moves after the game start or restart
		 * stores the number of turns since the restart or start of the game
		*/
		int turns_restart;

};

class ChessGameResult : public GameResult {
	public:
		ChessGameResult(const END_CODE endreason,const std::vector<GamePlayerResult>& l,const XML::AttributeMap& _game_attributes,XML::Tag *hist);

		virtual ~ChessGameResult();

		virtual const std::string& category() const;

		virtual const END_CODE end_reason() const;

		virtual const std::vector<GamePlayerResult>& players() const;

		virtual bool isRated() const;

		/*! \brief generates a History tag*/
		virtual XML::Tag* history() const;

		virtual void updateRating(std::map<XMPP::Jid, Rating>& ratings) const;

	protected:
		std::vector<GamePlayerResult> player_result_list;

	private:
		END_CODE _end_reason;

		XML::AttributeMap game_attributes;

		XML::Tag *_history;
};

class ChessAdjournedGame : public AdjournedGame {
	public:
		ChessAdjournedGame(XML::Tag* history, const std::vector<GamePlayer>& list, const std::string& category);

		~ChessAdjournedGame();

		virtual XML::Tag* history() const;

		virtual const std::vector<GamePlayer>& players() const;

        virtual const std::string& category() const;
	private:
		XML::Tag* _history;

		std::vector<GamePlayer> _players;

        std::string _category;
};

#endif
