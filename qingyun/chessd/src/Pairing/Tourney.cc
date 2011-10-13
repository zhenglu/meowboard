//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  Copyright (C) 2003 Pedro R de Andrade Neto
//  Copyright (C) 2007 Raphael H. Ribas
//  
//  $Id: Tourney.cc,v 1.3 2003/11/20 23:26:14 pedrorib Exp $
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
// Tourney.cc - Source file for Tourney class
//--------------------------------------------------------------------------

//static char RCSid[] = "$Id: Tourney.cc,v 1.3 2003/11/20 23:26:14 pedrorib Exp $";

#include "Tourney.hh"
#include "Util/utils.hh"
#include <vector>
#include <algorithm>

#ifndef log2
#define log2(x) (log(double(x))/log(2.0))
#endif

using namespace std;

namespace Pairing {

	//- Constructor ------------------------------------------------------------
	Tourney::Tourney(char style) {
		this->style = style;
		this->currentRound = 0;
		this->total_number_of_players=0;
	} //- End of Tourney

	//- Deconstructor ---------------------------------------------------------
	Tourney::~Tourney() { }

	//- AddPlayer ----------------------------------------------------------
	int Tourney::AddPlayer(int name, int rating, float score) {
		this->playerList.push_back(TourneyPlayers(name, rating, score));
		this->playerList_name[name]=&*(this->playerList.rbegin());
		this->total_number_of_players++;
		//SortPlayers();
		return(1);   // we entered the tourney
	}
	int Tourney::AddPlayer(const TourneyPlayers& tp) {
		this->playerList.push_back(tp);
		this->playerList_name[tp.name]=&*(this->playerList.rbegin());
		this->total_number_of_players++;
		//SortPlayers();
		return(1);   // we entered the tourney
	}

	//- RemovePlayer ----------------------------------------------------------
	void Tourney::RemovePlayer(int name) {
		TourneyPlayers *tp = NULL, *p=NULL;
		int seed=0;

		tp = GetPlayerByName(name);

		tp->activeFlag = 0;
		seed = tp->seed;

		// This code will go through a forfeited players list and give him a loss for current opponent
		// with a quick modification it will give losses for all games played as well...
		foreach(opp, tp->opponentList) {
			if(_BYE_ == opp->name) {              // If I am not _BYE_
				p = GetPlayerByName(opp->name);
				if(opp->floatValue == -1) {                      // floatValue stores the result for this player -1 means no result
					if(opp->value) {                               // if player leaving was white
						SetGameResult(tp->name, opp->name, 0);
					} else {
						SetGameResult(opp->name, tp->name, 1);
					}
					tp->AddBye();
				}
			}
		}  
		// Go through the player list and find _BYE_
		foreach(p, this->playerList) {
			if(p->name == _BYE_)  {                   // If we find him make sure he is active
				p->seed = seed;
				if(p->activeFlag == 0) {
					p->alive = 1;
					p->activeFlag = 1;
				} else {
					p->activeFlag = 0;
					p->alive = 0;
				}
				break;
			}
		}
		SortPlayers();

	}

	//- GetPlayer ----------------------------------------------------------
	TourneyPlayers *Tourney::GetPlayerByName(int name) {
		return this->playerList_name[name];
/*		foreach(player, this->playerList)
			if(player->name == name)
				return &*player;
		return NULL;
		*/
	}

	//- GetPlayer ----------------------------------------------------------
	TourneyPlayers *Tourney::GetPlayer(int i) {
		int count = 1;

		foreach(player, this->playerList)
			if(count == i)  
				return &*player;
			else
				count++;

		return NULL;
	}

	//- GetPlayerNumber ----------------------------------------------------------
	int Tourney::GetPlayerNumber(int name) {
		int i=1;
		foreach(player, this->playerList) {
			if(player->name == name)
				return i;
			i++;
		}

		return 99999;
	}

	//- GetRound ----------------------------------------------------------
	int Tourney::GetRound() {
		return currentRound;
	}//- end of GetRound --------------------------------------------------

	//- IsKnockOut --------------------------------------------------
	int Tourney::IsKnockOut() {
		if (style == 'k') return 1;
		return 0;
	}

	//- SortPlayers ----------------------------------
	void Tourney::SortPlayers()
	{
		int i=0;//, added=0;

//		this->sortList.clear();
		this->sortList=std::vector<Player>(total_number_of_players+has_bye);
//		this->sortList_value.clear();
		this->sortList_value=std::vector<int>(total_number_of_players+1);
//		this->sortList_name.clear();
		this->sortList_name=std::vector<Player*>(total_number_of_players+1);

		foreach(player, this->playerList) {
			if(player->activeFlag)
			{
				player->sortValue = (player->score + player->rating/10000.0);
				if(IsKnockOut())
				{
					player->sortValue = (player->alive * 10000) + (100 * player->CountOpponents());
					if(player->seed > 0)
						player->sortValue += (1.0/(float)player->seed);
				}
			} else
				player->sortValue = -1.0;
		}
		i=0;
		foreach(player, this->playerList) {
			/*
			added=0;
			foreach(s, this->sortList) {
				if(player->sortValue > s->floatValue)
				{
					sortList.insert(s, Player(player->name, player->sortValue));
					added = 1;
					break;
				}
			}
			if(!added)
			*/
			sortList[i]=Player(player->name, player->sortValue);
			i++;
		}
		std::stable_sort(sortList.begin(),sortList.end());

		i = 0;
		foreach(s, this->sortList) {
			s->value = i;
			this->sortList_name[s->name]=&*s;
			this->sortList_value[s->name]=s->value;

			/*if(gMamer.debugLevel >= 15) printf("%4d %-18s\n", s->value, s->name);*/
			i++;
		}

		if(style == 'k')
		{
			foreach(player, this->playerList)
				player->seed = GetSortValue(player->name);
		}
	}//- end of Sort Players ----------------------

	//- GetSortValue --------------------------------------------------------
	int Tourney::GetSortValue(int name) {
		return sortList_value[name];
		/*
		foreach(p, this->sortList)
			if(p->name == name)
				return p->value;
		return 0;
		*/
	}

	//- GetSortValueCount -------------------------------------------------------
	int Tourney::GetSortValueCount(double value) {
		int count=0;
		Player *s=NULL;

		foreach(player, this->playerList) {
			if(player->sortValue == value) {
				s = GetSortPlayerByName(player->name);
				if(s->value != 0)
					count++;
			}
		}

		return count;
	}

	//- GetSortPlayer ----------
	Player *Tourney::GetSortPlayerByName(int name)
	{
		return sortList_name[name];
		/*
		foreach(p, this->sortList)
			if(p->name == name)
				return &*p;

		return 0;
		*/
	}//- end of GetSortPlayer -----

	//- GetSortPlayer ------------------------------
	Player *Tourney::GetSortPlayer(int place)
	{
		if(place<(int)sortList.size())
			return &sortList[place];
		
//		foreach(p, this->sortList)
//			if(p->value == place)
//				return &*p;
		

		return 0;
	}//- end of GetSortPlayer -----

	//- GetPlayerCountTotal ----------------------------------------------------------
	int Tourney::GetPlayerCountTotal()
	{
		int count = 0;
		foreach(p, this->playerList)
			if(p->name != _BYE_)
				count++;
		return count;
	}//- end GetPlayerCountTotal ----------------------------------


	//- GetPlayerCount ----------------------------------------------------------
	int Tourney::GetPlayerCount()
	{
		int count = 0;
		// If this is a KO tourney count the players differently
		if(style == 'k') {
			foreach(p, this->playerList) {
				if(p->alive != 0)
					count++;
			}
		} else {
			foreach(p, this->playerList) {
				if(p->activeFlag != 0)
					count++;
			}
		}
		return count;
	}//- end GetPlayerCount ----------------------------------

	//- CloseAndStart ----------------------------------------------------------
	void Tourney::start(void) {
		int count = 0;

		int firstColor = (random() % (1024*2))/1024;
		foreach(player, this->playerList) {
			player->ChangeColorDue(firstColor);
			firstColor ^= 1;
		}
		currentRound = 0;
		count = GetPlayerCount();
		if(count % 2) count++;        // Because _BYE_ will be added

		MakeAssignments();
	}

	int Tourney::PopLastPairedPlayer()
	{
		int last=-1;

		if(not this->pairedPlayers.empty()) {
			last = this->pairedPlayers.back().second;
			this->pairedPlayers.pop_back();
		}

		return last;
	}

	void Tourney::ClearPairedPlayers()
	{
		this->pairedPlayers.clear();
	}

	int Tourney::UnpairAndCheckBye(void)
	{
		int playerCount = 0, byeFlag = 1;
		this->has_bye=0;
		std::list<TourneyPlayers>::iterator bye_pos;

		// Initialize a few things...make sure nobody is paired,
		foreach(tp, this->playerList) {
			UnPairPlayer(&*tp);
			if(tp->name == _BYE_) {
				byeFlag = 0;
				bye_pos=tp;
			}
		}
		if(byeFlag==0) {
			this->playerList.erase(bye_pos);
			byeFlag=1;
		}

		playerCount = GetPlayerCount();

		if((byeFlag) && (playerCount % 2))
		{
			has_bye=1;
			// we need to add a bye
			this->playerList.push_back(TourneyPlayers(_BYE_, 0, 0)); // add the bye to the tourney players list
			this->playerList_name[_BYE_]=&*(this->playerList.rbegin());
			playerCount++;
			this->playerList.back().seed = playerCount;
		}
		return playerCount;
	}

	int Tourney::TryToPair(int player_count, TourneyPlayers* tp, TourneyPlayers** opponent) {
		this->SetPairingScores(tp);
		for(;(tp->oppChoice < (int)potentialOpponentList.size()) and (*opponent = FindBestOpponent(tp,this->potentialOpponentList))==0 ;tp->oppChoice++);

		if(tp->oppChoice >= (int)potentialOpponentList.size()) //if couldn't pair the player no mather what
			return 0;

		return 1;
	}

	int Tourney::SwissAssign2(int playerCount) {
		TourneyPlayers *tp = NULL, *opponent = NULL;
		Player *p=NULL;
		int i=0;

		if(playerCount <= 0) return 0;

		foreach(player,this->sortList) {
			p=&*player;
			if((style == 'd') || (style == 'r')) {
				// If this is a round robin we don't need to or WANT to sort the players first
				tp = GetPlayer(player-this->sortList.begin());
			} else {
				p = GetSortPlayer(player-this->sortList.begin());
				tp = GetPlayerByName(p->name);
			}
			if( ( !tp->IsPaired() ) && tp->activeFlag && tp->alive) {
				while(this->TryToPair(playerCount,tp,&opponent)==0) {
					tp->oppChoice = 0;
					i = PopLastPairedPlayer();
					if(i < 0) return 0;

					player=this->sortList.begin()+i;

					if((style == 'd') || (style == 'r'))
						tp = GetPlayer(i);
					else {
						p = GetSortPlayer(i);
						tp = GetPlayerByName(p->name);
					}

					opponent = GetPlayerByName(tp->oppName);
					tp->RemoveLastOpponent();                           // removes the person we were planning on playing
					opponent->RemoveLastOpponent();                     // from both players!
					UnPairPlayer(tp);                                   // unpair us so we can be re-paired
					UnPairPlayer(opponent);
					tp->oppChoice++;                                    // try the next possible opponent
				}
				pairedPlayers.push_back(PairedPlayer(tp->name, i));
				PairPlayers(tp, opponent);
			}
			i++;
		}
		return 1;
	}

	int Tourney::SwissAssign(int playerCount)
	{
		TourneyPlayers *tp = NULL, *opponent = NULL;
		Player *p=NULL;
		int everybodyPaired=0, i=0;

		if(playerCount <= 0) return 0;
		while(everybodyPaired == 0) {
			everybodyPaired = 0;
			if((style == 'd') || (style == 'r')) {
				tp = GetPlayer(i);   // If this is a round robin we don't need to or WANT to sort the players first
			} else {
				p = GetSortPlayer(i);
				tp = GetPlayerByName(p->name);
			}
			opponent = (TourneyPlayers *)NULL;
			// PrintPotentialLists();
			if(tp == NULL) return 0;
			if( ( !tp->IsPaired() ) && tp->activeFlag && tp->alive) {  // If I am not paired and I am active pair me
				SetPairingScores(tp);
				if((opponent = FindBestOpponent(tp,this->potentialOpponentList))) {
					// keep a list of paired players
					pairedPlayers.push_back(PairedPlayer(tp->name, i));
					//if(gMamer.debugLevel >= 5)
					//printf("Adding: %s %d to the paired list vs. %s\n", tp->name, i, opponent->name);
					everybodyPaired = PairPlayers(tp, opponent);          // Actually Pair us
					i++;                                                  // go to the next player
				} else {                                                // If no opponent for me try & another up the tree
					if(tp->oppChoice > playerCount) {                     // If I have tried all my opponents
						tp->oppChoice = 0;                                  // reset me so I can try again later
						i = PopLastPairedPlayer();                          // returns int of last paired & del's from paired list
						if(i < 0)  {                                       // this is really bad; means we can't even find
							return 0;                                         // an opp for 1st player.  Tourney has to be over now
						}
						if((style == 'd') || (style == 'r'))
							tp = GetPlayer(i);   // If this is a round robin we don't need to or WANT to sort the players first
						else {
							p = GetSortPlayer(i);
							tp = GetPlayerByName(p->name);
						}
						opponent = GetPlayerByName(tp->oppName);
						//if(gMamer.debugLevel >= 5)
						//printf("Un-Pairing: %s & %s choice #%d\n", tp->name, opponent->name, tp->oppChoice);
						tp->RemoveLastOpponent();                           // removes the person we were planning on playing
						opponent->RemoveLastOpponent();                     // from both players!
						UnPairPlayer(tp);                                   // unpair us so we can be re-paired
						UnPairPlayer(opponent);
						tp->oppChoice++;                                    // try the next possible opponent
					} else {
						tp->oppChoice++;                                    // Try my next opponent
					}
				}
			} else {                                                  // if I am already paired goto next player & pair him
				i++;
			}
		}
		return 1;
	}

	void Tourney::ColorizeTourney(void) {
		TourneyPlayers *opponent = NULL;

		foreach(tp, this->playerList) UnPairPlayer(&*tp);  // unpair all players so we can use that to tell

		// if they have been assiged a color
		foreach(tp, this->playerList) {
			if((!tp->IsPaired()) && (tp->activeFlag != 0) && (tp->alive != 0)) {
				opponent = GetPlayerByName(tp->oppName);
				AssignColors(&*tp, opponent);
				tp->NowPaired(true);               // mark BOTH players as having a color
				opponent->NowPaired(true);         // this is important for when we hit this player later in the Iter
			}
		}
	}

	int Tourney::SetByeResult(void) {

		foreach(tp, this->playerList) {
			if(tp->name == _BYE_) {                     // If I am the bye
				foreach(opp, tp->opponentList) { // find my current opponent in my oppList
					if(opp->name == tp->oppName) {        // & Give him a win
						if(opp->value)
							SetGameResult(tp->name, tp->oppName, 0);
						else
							SetGameResult(tp->oppName, tp->name, 1);
						tp->AddBye();
						return tp->oppName;
						/*gMamer.TellUser(ByeRound, opp->name);*/              // Tell him about it.
					}
				}
			}
		}
		return -1;
	}

	int Tourney::MakeAssignments(void)
	{
		int playerCount = 0;

		currentRound++;

		playerCount = UnpairAndCheckBye();  // Returns the # of active players in tourney

		SortPlayers();

		// Set up Offsets for swiss
		SetOffsets();

		// Set up the PairingScores
		//foreach(tp, this->playerList) { if(!tp->IsPaired()) SetPairingScores(&*tp); }

		foreach(tp, this->playerList) { UnPairPlayer(&*tp); tp->oppChoice = 0; }  // unpair all the players

		ClearPairedPlayers();

		if(SwissAssign2(playerCount) == 0)
			return 0;

		ColorizeTourney();


		return 1;
	}

	//- Start of FindBestOpponent
	TourneyPlayers *Tourney::FindBestOpponent(TourneyPlayers *tp, const std::vector<Player>& potentialOpponentList)
	{
		if(tp->oppChoice > (int)potentialOpponentList.size())
			return 0;

		TourneyPlayers* ans=0;
		if(0 == ((ans=GetPlayerByName(potentialOpponentList[tp->oppChoice].name))->IsPaired()))
			return ans;

		return 0;
		/*
		int i=0;
		foreach(tmp, tp->potentialOpponentList) {
			if( (i == tp->oppChoice) && (0 == GetPlayerByName(tmp->name)->IsPaired()) )
				return GetPlayerByName(tmp->name);
			i++;
		}
		return NULL;
		*/
	}

	//- Start of SetOffsets  ------------------------------------
	void Tourney::SetOffsets() {
		
		TourneyPlayers** players = new TourneyPlayers*[this->playerList.size()+1];
		{
			int i=0;
			foreach(player,this->playerList)
				players[i++]=(&*player);
		}

		/*order them by Score, and if it is equal, order by rating*/
		std::sort(players,players+this->playerList.size(),byScoreAndRating());
		players[this->playerList.size()]=(new TourneyPlayers(0,0,-1));

		float score=-1;
		int count=0;
		int *places=new int[this->playerList.size()+1];
		int beg=0;
		for(int j=0;j<(int)this->playerList.size()+1;j++) {
			if(players[j]->score!=score) {
				if(j!=0)places[j-1]=1;
				int tmp=1;
				int i;
				for(i=j-2;i>=beg;i--) { //set places for everyone with the same rating
					places[i]=places[i+1];
					if(players[i]->rating!=players[i+1]->rating) {
						places[i]+=tmp;
						tmp=0;
					}
					tmp++;
				}

				//set the offset that is based in the number of people with the same score
				//and the place of that person within the people with the same score as his
				for(int i=0;beg!=j;beg++,i++) {
					players[beg]->offset=count+1;

					players[beg]->offset % 2 ? players[beg]->offset : players[beg]->offset--;
					players[beg]->offset/=2;
					if(places[beg] > players[beg]->offset)
						players[beg]->offset *= -1;
				}
				score=players[j]->score;
				count=0;
			}
			count++;
		}
		delete players[this->playerList.size()];
		delete places;
		delete players;
	/*	
		
		foreach(p,this->playerList) {
			TourneyPlayers* tp=&*p;
			int offset=2,place=1;
			foreach(opponent, this->playerList) {
				if((tp->name != opponent->name) && (tp->score == opponent->score)) {
					offset++;
					if(opponent->rating > tp->rating) place++;
				}
			}
			(offset % 2) ? offset : offset--;     // Makes the pairings move UP in a tie instead of down
			offset = offset / 2;                  // three players 1 = 2 = 3 = score;  will pair  1-2 instead of 1-3
			if(place > offset) { offset *= -1; }  // flip the offset so that it looks upward.
			tp->offset=offset;
		}
		*/
		
	}

	//- Start of SetPairingScores -------------------------------
	void Tourney::SetPairingScores(TourneyPlayers *tp) {
		double score;
		Player *t=NULL, *me=NULL;
		int /*offset=2, place=1, i=0, added=0,*/ timesPlayed=0;
		int offset=tp->offset;

		if(tp->activeFlag == 0) return;       // If I am NOT active move to next player, do NOT pair me
		if(tp->alive == 0) return;                           // if I am NOT alive don't pair me

//		tp->RemovePotentialOppList();
		this->potentialOpponentList.clear();

		me = GetSortPlayerByName(tp->name);
		foreach(opponent, this->playerList) {
			if(tp->name == opponent->name) continue;      // If this is me do NOT score me
			if(opponent->activeFlag == 0) continue;                  // If this person is NOT active move on
			if(opponent->IsPaired()) continue;				//if the opponent is already paired
			timesPlayed = tp->AlreadyPlayed(opponent->name);
			switch(style) {
				case 'd':                                                // If this is a double RR
					if(timesPlayed > 1) continue;                          //  and I have played this person more than once move on 
					break;
				case 'k':
					if(opponent->alive == 0) continue;                     // if this opponent is not alive move on
					break;
				default:  
					if(timesPlayed > 0) continue;                          // If I have already played this person move on
					break;
			}    

			t = GetSortPlayerByName(opponent->name);
			switch (style) {
				case 'r':                                                // If this is a RR
					score = GetPlayerNumber(tp->name) + currentRound;  // My location plus the round #
					offset = GetPlayerCount();                             // just using offset cause its available space
					if ((int)score > offset) 
						score = ((int)score % offset);
					score = abs((int)score - GetPlayerNumber(opponent->name));
					break;
				case 'd':                                                // If this is Double Round Robin
					score = GetPlayerNumber(tp->name) + (currentRound/2);  // My location plus the round #
					offset = GetPlayerCount();                             // just using offset cause its available space
					if ((int)score > offset)
						score = ((int)score % offset);
					score = abs((int)score - GetPlayerNumber(opponent->name));
					//score = GetPlayerNumber(opponent->name);  does the same thing as above but is slower in the pairings.
					break;
				case 'k':
					score = abs((tp->seed + opponent->seed) - (GetPlayerCount() + 1));
					break;
				default:
					score = ((abs(t->value - (me->value + offset))) * 1000) + ((fabs(opponent->score - tp->score)) * 10.0);
					score += abs(opponent->ColorDue() - tp->ColorDue()) + (abs(opponent->rating - tp->rating)) * 0.0001;
					break;
			}
			if(style != 'k')
				if((opponent->name == _BYE_) && (tp->GetByeCount())) score = 10000 * tp->GetByeCount();

			/*if(gMamer.debugLevel >= 15) 
			  printf("%s %s %f %d\n", me->name, opponent->name, score, offset);*/

			/*
			added=0;
			foreach(temp, tp->potentialOpponentList) {
				if(score < temp->floatValue) {
					tp->potentialOpponentList.insert(temp, Player(opponent->name, score));
					added = 1;
					break;
				}
			}
			if(!added) tp->potentialOpponentList.push_back(Player(opponent->name, score));*/
			this->potentialOpponentList.push_back(Player(opponent->name,score));

		}
		std::stable_sort(this->potentialOpponentList.begin(),this->potentialOpponentList.end(),cmpFloatValueless());
/*			i=0;	
			foreach(temp, tp->potentialOpponentList) temp->value = i++;
//			tp->value is only used in one function, so dont need to set it here
*/				
	}

	//- Start of PairPlayers ----------------------------------
	int Tourney::PairPlayers(TourneyPlayers *p1, TourneyPlayers *p2) {

		p1->opponentList.push_back(Player(p2->name, -1.0, 0, p2->rating));
//		p1->opponentList_map[p2->name]++;//XXX be carefull with this
		p1->NowPaired(true);
		p1->oppName = p2->name;

		p2->opponentList.push_back(Player(p1->name, -1.0, 0, p1->rating));
//		p2->opponentList_map[p1->name]++;//XXX be carefull with this
		p2->NowPaired(true);
		p2->oppName = p1->name;

		foreach(tp, this->playerList) {
			if((!tp->IsPaired()) && (tp->activeFlag != 0) && (tp->alive != 0))
				return 0;
		}

		return 1;
	}

	//- Start of UnPairPlayer ----------------------------------
	void Tourney::UnPairPlayer(TourneyPlayers *p1) {
		if(p1 != NULL)
			p1->NowPaired(false);
	}//- end of UnPairPlayer

	//- AssignColors ----------------------------------------------------------
	void Tourney::AssignColors(TourneyPlayers *p1, TourneyPlayers *p2) {
		int p1Color=0;
		Player *opp1 = NULL, *opp2 = NULL;


		if(p1->ColorDue() != p2->ColorDue()) {
			if(p1->ColorDue())
				p1Color = 1;
		} else {
			if(p1->ColorDue()) {   // Both are due white; need to find out how due.
				if(p1->GetConsecutiveBlacks() > p2->GetConsecutiveBlacks()) {
					p1Color = 1;
				} else if(p1->GetConsecutiveBlacks() > p2->GetConsecutiveBlacks()) {
					p1Color = 0;
				} else if(p1->GetConsecutiveBlacks() == p2->GetConsecutiveBlacks()) {
					if(p1->GetTotalBlacks() > p2->GetTotalBlacks()) {
						p1Color = 1;
					} else if(p1->GetTotalBlacks() < p2->GetTotalBlacks()) {
						p1Color = 0;
					} else if(p1->GetTotalBlacks() == p2->GetTotalBlacks()) {
						if((p1->score * 10000 + p1->rating) >= (p2->score * 10000 + p2->rating))
							p1Color = 1;
						else
							p1Color = 0;
					}
				}
			} else {
				if(p1->GetConsecutiveWhites() > p2->GetConsecutiveWhites()) {
					p1Color = 0;
				} else if (p1->GetConsecutiveWhites() < p2->GetConsecutiveWhites()) {
					p1Color = 1;
				} else {
					if(p1->GetTotalWhites() > p2->GetTotalWhites()) {
						p1Color = 0;
					} else if(p1->GetTotalWhites() < p2->GetTotalWhites()) {
						p1Color = 1;
					} else {
						// When there is a straight up tie in all ways highest player gets due color. (black)
						if((p1->score * 10000 + p1->rating) < (p2->score * 10000 + p2->rating))
							p1Color = 1;
						else
							p1Color = 0;
					}
				}
			}
		}
		foreach(tmp, p1->opponentList)
			if(tmp->name == p2->name)
				opp1 = &*tmp;

		foreach(tmp, p2->opponentList)
			if(tmp->name == p1->name)
				opp2 = &*tmp;

		if(p1Color) {
			p1->AddWhite();
			p2->AddBlack();
			opp1->value = 1;
			AddGame(Game(p1->name, p2->name));
		} else {
			p1->AddBlack();
			p2->AddWhite();
			opp2->value = 1;
			AddGame(Game(p2->name, p1->name));
		}

	}

	//- AddGame -----------------------------------------------------------------
	void Tourney::AddGame(const Game& g) {
		int added = 0, score=0, compScore=0;

		score = GetSortValue(g.whiteName) + GetSortValue(g.blackName);

		foreach(temp, this->gameList) {
			compScore = GetSortValue(temp->whiteName) + GetSortValue(temp->blackName);
			if(score < compScore) {
				gameList.insert(temp, g);
				added = 1;
				break;
			}
		}
		if(!added) gameList.push_back(g);
	}

	list<Game>& Tourney::getGames() {
		return this->gameList;
	}

	list<TourneyPlayers>& Tourney::getPlayers() {
		return this->playerList;
	}

	const list<TourneyPlayers>& Tourney::getPlayers() const {
		return this->playerList;
	}

	//- SetGameResult --------------------------------------------
	int Tourney::SetGameResult(int white, int black, int result) {
		Player *opp1 = NULL, *opp2 = NULL;
		TourneyPlayers *tp1 = NULL, *tp2 = NULL;

		tp1 = GetPlayerByName(white);
		tp2 = GetPlayerByName(black);

		//if((style == 'k') && ((tp1->alive == 0) || (tp2->alive == 0))) result = -1;

		foreach(tmp, tp1->opponentList)
			if(tmp->name == black) opp1 = &*tmp;

		foreach(tmp, tp2->opponentList)
			if(tmp->name == white) opp2 = &*tmp;

		//if((NULL == opp1) || (NULL == opp2)) { return -1; }

		switch (result)
		{    // set the result
			case 1:
				opp1->floatValue = 1.0;
				opp2->floatValue = 0.0;
				if(tp1->seed > tp2->seed)
                    tp1->seed = tp2->seed;
				if(style == 'k') {
					tp2->alive = 0;
				}
				break;
			case 0:
				opp1->floatValue = 0.0;
				opp2->floatValue = 1.0;
				if(tp2->seed > tp1->seed)
                    tp2->seed = tp1->seed;
				if(style == 'k') {
					tp1->alive = 0;
				}
				break;
			case 2:
				opp1->floatValue = 0.5;
				opp2->floatValue = 0.5;
                if(abs(tp1->rating - tp2->rating) >= 200) {
                    if(tp1->rating > tp2->rating) {
                        // tp1 is higher by more than 200  meaning tp2 wins
                        if(tp2->seed > tp1->seed)
                            tp2->seed = tp1->seed;
                        if(style == 'k') {
                            tp1->alive = 0;
                        }
                    } else {
                        // tp2 is higher by more than 200  meaning tp1 wins
                        if(tp1->seed > tp2->seed)
                            tp1->seed = tp2->seed;
                        if(style == 'k') {
                            tp2->alive = 0;
                        }
                    }
                } else {
					if(tp2->seed > tp1->seed)
                        tp2->seed = tp1->seed;
					if(style == 'k') {
						tp1->alive = 0;
					}
				}
				break;
			default:
				break;
		}
		tp1->CalculateScore();
		tp2->CalculateScore();

		foreach(g, this->gameList) {
			if((g->whiteName == white) && (g->blackName == black)) {
				this->gameList.erase(g);
				break;
			}
		}

		return this->gameList.empty() ? 2 : 1;
	} //- End SetGameResult --------------------------------------

}
