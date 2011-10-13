//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  Copyright (C) 2003 Pedro R de Andrade Neto
//  
//  $Id: TourneyPlayers.cc,v 1.3 2003/11/20 23:26:14 pedrorib Exp $
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
// TourneyPlayers.cc - Source file for the TourneyPlayers class
//
// Matthew E. Moses && Michael A. Long
//
// $Revision: 1.3 $
// $Date: 2003/11/20 23:26:14 $
//
// $Author: pedrorib $
// $Locker:  $
//
// $Log: TourneyPlayers.cc,v $
// Revision 1.3  2003/11/20 23:26:14  pedrorib
// Now compiles in g++ 3.3.2
//
// Revision 1.2  2003/11/18 18:21:13  pedrorib
// New version
//
// Revision 1.11  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.10  1999/01/02 21:46:48  mlong
// added bughouse support
//
// Revision 1.9  1998/10/07 21:20:51  mlong
// small bug fixes
//
// Revision 1.8  1998/09/22 19:30:50  mlong
// fixed color assign bug
//
// Revision 1.7  1998/09/10 19:57:17  mlong
// lots of little bug fixes and a few new features
//
// Revision 1.6  1998/04/29 15:23:19  mlong
// prepairing for the move to daimi
// new sorting routine.
//
//
// Revision 1.2  1997/10/08 21:03:35  chess
// preparing for move to oracle machine at eworks.
//
// Revision 1.1  1997/05/14 16:59:13  chess
// Initial revision
//
//
//--------------------------------------------------------------------------

// static char RCSid[] = "$Id: TourneyPlayers.cc,v 1.3 2003/11/20 23:26:14 pedrorib Exp $";

#include "TourneyPlayers.hh"
using namespace std;

#define foreach(it, cont) for(typeof((cont).begin()) it = (cont).begin(); it != (cont).end(); ++it)

namespace Pairing {
	// NEW
	TourneyPlayers::TourneyPlayers(int n, int r, float s) :
		perform(0),
		upset(0),
		name(n),
		rating(r),
		score(s),
		activeFlag(1),
		sortValue(r/10000.0),
		oppChoice(0),
		alive(1),
		seed(0),
		totalWhites(0),
		totalBlacks(0),
		consecutiveWhites(0),
		consecutiveBlacks(0),
		dueColor(0),
		paired(0),
		byeCount(0)
	{
	}

	//- DeConstructor ---------------------------------------------------------
	TourneyPlayers::~TourneyPlayers() {
	}

	//- AlreadyPlayed --------------------------------------------------------
	int TourneyPlayers::AlreadyPlayed(int oppName) {
		//return opponentList_map[oppName];
		
		int count=0;
		foreach(temp, this->opponentList) {
			if(oppName == temp->name) {
				count++;
			}
		}

		return count;
		
	}

	//- CountOpponents --------------------------------------------------------
	int TourneyPlayers::CountOpponents() {
		return this->opponentList.size();
	}

	//- AddBye ---------------------------------------------------------------
	void TourneyPlayers::AddBye() {
		byeCount++;
	}

	//- SubBye ---------------------------------------------------------------
	void TourneyPlayers::SubBye() {
		byeCount--;
	}

	//- GetByeCount ----------------------------------------------------------
	int TourneyPlayers::GetByeCount() {
		return byeCount;
	}

	//- Start of RemovePotentialOppList
/*	int TourneyPlayers::RemovePotentialOppList() {
		if(this->potentialOpponentList != 0) {
			delete this->potentialOpponentList;
			potentialOpponentList=0;
		}
		return 1;
	}*///- end of RemovePotentialOppList

	//- Start of RemoveFromOppList -------------------------------------------------
	void TourneyPlayers::RemoveFromOppList(int name) {
		foreach(p, this->opponentList) {
			if(p->name == name) {
				opponentList.erase(p);
				break;
			}
		}
	}//- end RemoveFromOppList -----------------------------------------------------

	//- Start of RemoveLastOpponent -------------------------------------------------
	void TourneyPlayers::RemoveLastOpponent() {
		//opponentList_map[this->opponentList.back().name]--;
		this->opponentList.pop_back();
	}//- end RemoveLastOpponent -----------------------------------------------------

	//- Start GetOpponentPlayer --------------------------------------------------------
	/*Player *TourneyPlayers::GetOpponentPlayer(const std::string&oppName) {
	  Player *temp=NULL;
	  LinkListIter<Player> opponentIter(opponentList);

	  while((temp = opponentIter.Next())) {
	  if(!strcmp(oppName, temp->name)) {
	  return temp;
	  }
	  }

	  return NULL;
	  }*/ //- end of GetOpponentPlayer

	//- IsPaired ---------------------------------
	bool TourneyPlayers::IsPaired(void) {
		return paired;
	}//- end IsPaired

	//- NowPaired ---------------------------------
	void TourneyPlayers::NowPaired(bool value) {
		paired = value;
	}//- end NowPaired

	//-ChangeColorDue ---------------------------------
	void TourneyPlayers::ChangeColorDue(int value) {
		dueColor = value;
	}//- end ToggleColorDue

	//- AddWhite ---------------------------------
	void TourneyPlayers::AddWhite(void) {
		consecutiveWhites++;
		totalWhites++;
		ChangeColorDue(0);
		ClearBlacks();
	}//- end AddWhite

	//- AddBlack ---------------------------------
	void TourneyPlayers::AddBlack(void) {
		consecutiveBlacks++;
		totalBlacks++;
		ChangeColorDue(1);
		ClearWhites();
	}//- end AddBlack

	//- ClearWhites ---------------------------------
	void TourneyPlayers::ClearWhites(void) {
		consecutiveWhites = 0;
	}//- end ClearWhites

	//- ClearBlacks ---------------------------------
	void TourneyPlayers::ClearBlacks(void) {
		consecutiveBlacks = 0;
	}//- end ClearBlacks

	//- GetTotalWhites ---------------------------------
	int TourneyPlayers::GetTotalWhites(void) {
		return totalWhites;
	}//- end GetTotalWhites

	//- GetTotalBlacks ---------------------------------
	int TourneyPlayers::GetTotalBlacks(void) {
		return totalBlacks;
	}//- end GetTotalBlacks

	//- GetConsecutiveWhites ---------------------------------
	int TourneyPlayers::GetConsecutiveWhites(void) const {
		return consecutiveWhites;
	}//- end GetConsecutiveWhites

	//- GetConsecutiveBlacks ---------------------------------
	int TourneyPlayers::GetConsecutiveBlacks(void) const {
		return consecutiveBlacks;
	}//- end GetConsecutiveBlacks

	//- CalculateScore --------------------------------------
	void TourneyPlayers::CalculateScore(void) {
		score = 0.0;

		foreach(opp, this->opponentList) {
			if(opp->floatValue > 0.0) {
				score += opp->floatValue;
			}
		}
		CalculatePerform();
	}//- End of CalculateScore -------------------

	//- CalculatePerform --------------------------------------
	void TourneyPlayers::CalculatePerform(void) {
		int total=0, counter=0, upsetpoints=0, rtng=0;

		foreach(opp, this->opponentList) {
			if(opp->name == _BYE_) continue;
			rtng = opp->rating;
			if(opp->rating <= 0) rtng = 1675;
			if(opp->floatValue == 0.5) {
				total += rtng;
				if(rtng > rating)
					upsetpoints += (rtng - rating)/2;
			} else if(opp->floatValue == 1.0) {
				total += (rtng + 400);
				if(rtng > rating)
					upsetpoints += (rtng - rating);
			} else if(opp->floatValue == 0.0) {
				total += (rtng - 400);
			}
			counter++;
		}

		if(counter > 0)
			perform = total/counter;
		else
			perform = 0;
		upset = upsetpoints;
	}//- End of CalculatePerform -------------------


}
