//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  Copyright (C) 2003 Pedro R de Andrade Neto
//  
//  $Id: TourneyPlayers.hh,v 1.3 2003/11/20 23:26:14 pedrorib Exp $
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
// TourneyPlayers.hh - Class header for the TourneyPlayers class
//
// Matthew E. Moses & Michael A. Long
//
// $Revision: 1.3 $
// $Date: 2003/11/20 23:26:14 $
//
// $Author: pedrorib $
// $Locker:  $
//
// $Log: TourneyPlayers.hh,v $
// Revision 1.3  2003/11/20 23:26:14  pedrorib
// Now compiles in g++ 3.3.2
//
// Revision 1.2  2003/11/18 18:21:13  pedrorib
// New version
//
// Revision 1.10  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.9  1999/01/02 21:47:05  mlong
// added bughouse support
//
// Revision 1.8  1998/10/07 21:22:39  mlong
// small bug fixes
//
// Revision 1.7  1998/09/22 19:32:33  mlong
// fixed color assign bug.
//
// Revision 1.6  1998/09/10 19:58:41  mlong
// lots of little bug fixes and a few new features.
//
// Revision 1.5  1998/04/29 15:24:07  mlong
// prepairing for the move to daimi
// new sorting routine.
//
// Revision 1.4  1998/02/12 18:44:25  mlong
// *** empty log message ***
//
// Revision 1.3  1997/10/28 21:03:48  mlong
// *** empty log message ***
//
// Revision 1.2  1997/10/23 19:56:12  chess
// *** empty log message ***
//
// Revision 1.1  1997/05/14 16:59:13  chess
// Initial revision
//
//
//--------------------------------------------------------------------------

#ifndef _TOURNEYPLAYERS_
#define _TOURNEYPLAYERS_

#include "Player.hh"

#include <string>
#include <list>
#include <set>
#include <map>

namespace Pairing {
#define _BYE_ 0

	class TourneyPlayers {
		public:
			TourneyPlayers(int name = 0, int rating = 0, float score = 0);
			~TourneyPlayers();

			void AddWhite();
			void AddBlack();

			int GetConsecutiveWhites() const;
			int GetConsecutiveBlacks() const;
			int GetTotalWhites();
			int GetTotalBlacks();
			int ColorDue() const { return this->dueColor; }

			void AddBye();
			void SubBye();
			int GetByeCount();

			bool IsPaired();
			void NowPaired(bool);

			int AlreadyPlayed(int name);
			int CountOpponents();
			void CalculateScore();

			//int RemovePotentialOppList();
			//Player *GetOpponentPlayer(const std::string&);
			void RemoveFromOppList(int name);
			void RemoveLastOpponent();

			void ChangeColorDue(int);

			float getScore() const { return this->score; }
			int getName() const { return this->name; }

		private:
			void CalculatePerform();
			void ClearWhites();
			void ClearBlacks();

		public:

			int perform;
			int upset;
			int name;
			int rating;
			float score;
			int activeFlag;
			double sortValue;
			int oppChoice;
			int offset;

			int alive;
			int seed;

			int oppName;

			std::list<Player> opponentList;
//			std::list<Player> potentialOpponentList;
//			std::map<int/*name*/,int/*times player*/> opponentList_map;
//			std::multiset<Player,cmpFloatValueless> potentialOpponentList;

		private:

			int totalWhites;
			int totalBlacks;
			int consecutiveWhites;
			int consecutiveBlacks;
			int dueColor;
			bool paired;

			int byeCount;

	};
	struct byScoreAndRating {
		bool operator()(const TourneyPlayers* a, const TourneyPlayers* b) {
			if(a->score==b->score)
				return a->rating < b->rating;
			return a->score < b->score;
		}
	};

}

#endif
