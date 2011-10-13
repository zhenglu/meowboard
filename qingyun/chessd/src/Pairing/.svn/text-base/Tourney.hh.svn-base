//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  Copyright (C) 2003 Pedro R de Andrade Neto
//
//  $Id: Tourney.hh,v 1.3 2003/11/20 23:26:14 pedrorib Exp $
//
//  mamer is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published
//  by the Free Software Foundation; either version 2 of the License,
//  or (at your option) any later version.
//
//  mamer is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty
//  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//  See the GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with mamer; if not, write to the Free Software Foundation,
//  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//--------------------------------------------------------------------------
// Tourney.hh - Class header for the Tourney class
//
// Matthew E. Moses & Michael A. Long
//
// $Revision: 1.3 $
// $Date: 2003/11/20 23:26:14 $
//
// $Author: pedrorib $
// $Locker:  $
//
//--------------------------------------------------------------------------

#ifndef _TOURNEY_
#define _TOURNEY_

//#include <sys/param.h>
//#include <sys/types.h>

#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <cstring>

#include <ctime>
#include <unistd.h>
#include <cmath>

#include <list>
#include <map>
#include <vector>

#include "TourneyPlayers.hh"
#include "TourneyGame.hh"

namespace Pairing {
	typedef std::pair<int/*name*/, int> PairedPlayer ;

	class Tourney {
		public:
			/* number and variante is relevant anymore */
			/* What does style means? */
			Tourney(char style);
			~Tourney();

			/* This is not necessary anymore, substitute this for a function
			 * to get the pairings instead */

			std::list<Game>& getGames();

			std::list<TourneyPlayers>& getPlayers();
			const std::list<TourneyPlayers>& getPlayers() const;

			/* This may stay alive */
			int IsKnockOut();

			/* This stats the tourney in fact */
			void start(void);

			/* add and remove players, this may stay like this */
			int AddPlayer(int name, int rating, float score = 0.0);
			int AddPlayer(const TourneyPlayers& tp);
			void RemovePlayer(int name);

			/* sort player stuff, seems important */
			void SortPlayers();
			Player *GetSortPlayerByName(int name);
			Player *GetSortPlayer(int);

			/* Retrieve a player in 3 different ways */
			TourneyPlayers *GetPlayerByName(int name);
			TourneyPlayers *GetPlayer(int);

			/* get stuff */
			int GetPlayerCount();
			int GetPlayerCountTotal();
			int GetVariant();
			int GetRound();

			/* Pair players */
			int MakeAssignments();

			/*result:
			 * 1 white wins
			 * 0 black wins
			 * 2 draw*/
			int SetGameResult(int white, int black, int result);
			int SetByeResult();

		private:
			/* Set the torunamets properties */

			int GetSortValueCount(double);
			int GetSortValue(int name);

			/* pairing stuff */
			void SetOffsets();
			void SetPairingScores(TourneyPlayers *);
			TourneyPlayers *FindBestOpponent(TourneyPlayers *,const std::vector<Player>&);
			int PairPlayers(TourneyPlayers *, TourneyPlayers *);
			void UnPairPlayer(TourneyPlayers *);
			void AssignColors(TourneyPlayers *, TourneyPlayers *);
			void AddGame(const Game& game);
			int PopLastPairedPlayer();
			void ClearPairedPlayers();
			int TryToPair(int player_count, TourneyPlayers* tp, TourneyPlayers** opponent) ;

			/* pairing stuff */
			int SwissAssign(int);
			int SwissAssign2(int);
			int UnpairAndCheckBye();
			void ColorizeTourney();
			int GetPlayerNumber(int name);


			/*LinkList<TourneyPlayers> playerList;
			  LinkList<Game> gameList;
			  LinkList<Player> sortList;
			  LinkList<PairedPlayer> pairedPlayers;*/

			std::list<TourneyPlayers> playerList;
			std::list<Game> gameList;
//			std::list<Player> sortList;
			std::vector<Player> sortList;
			std::map<int/*name*/,TourneyPlayers*> playerList_name;
//			std::map<int/*name*/,Player*> sortList_name;
			std::vector<Player*> sortList_name;
//			std::map<int/*name*/,int> sortList_value;
			std::vector<int> sortList_value;
			
			std::list<PairedPlayer> pairedPlayers;
			std::vector<Player> potentialOpponentList;

			int total_number_of_players;

			int currentRound;

			char style;

			int has_bye;
	};

}

#endif

//---------------------------------------------------------
// $Log: Tourney.hh,v $
// Revision 1.3  2003/11/20 23:26:14  pedrorib
// Now compiles in g++ 3.3.2
//
// Revision 1.2  2003/11/18 18:21:13  pedrorib
// New version
//
// Revision 1.17  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.16  2002/08/08 01:45:15  rha
// Merge in changes made to mamer outside the RCS
// environment from March 1999 to January 2000.
//
// Revision 1.15  1999/01/02 21:47:05  mlong
// added bughouse support
//
// Revision 1.14  1998/10/26 17:37:25  mlong
// *** empty log message ***
//
// Revision 1.13  1998/10/07 21:22:39  mlong
// small bug fixes
//
// Revision 1.12  1998/09/22 15:49:03  mlong
// memory leaks fixed
// using insure to compile
// also added support for gametype
//
// Revision 1.11  1998/09/10 19:58:41  mlong
// lots of little bug fixes and a few new features.
//
// Revision 1.10  1998/04/29 15:24:07  mlong
// prepairing for the move to daimi
// new sorting routine.
//
// Revision 1.9  1998/04/18 18:46:31  mlong
// fixed delete bug and
// added delete tourney function
//
// Revision 1.8  1998/02/12 18:44:25  mlong
// *** empty log message ***
//
// Revision 1.7  1997/10/28 21:03:48  mlong
// *** empty log message ***
//
// Revision 1.6  1997/10/23 19:56:44  mlong
// *** empty log message ***
//
// Revision 1.5  1997/10/23 19:37:22  chess
// lots of new stuff
//
// Revision 1.4  1997/05/15 18:29:12  chess
//  added pending and TourneyPlayers support
// added HandleGetPlayerInfo & HandleGetGameInfo
//
// Revision 1.3  1997/05/02 23:55:18  chess
// added TourneyParameters class include
//
// Revision 1.2  1996/10/01 20:14:43  moses
// added methods
//
// Revision 1.1  1996/09/30  20:52:48  moses
// Initial revision
//
//--------------------------------------------------------------------------
