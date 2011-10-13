//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  Copyright (C) 2003 Pedro R de Andrade Neto
//  
//  $Id: Player.hh,v 1.3 2003/11/20 23:26:14 pedrorib Exp $
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
// Player.hh - Class header for the Player class
//
// Matthew E. Moses & Michael A. Long
//
// $Revision: 1.3 $
// $Date: 2003/11/20 23:26:14 $
//
// $Author: pedrorib $
// $Locker:  $
//
// $Log: Player.hh,v $
// Revision 1.3  2003/11/20 23:26:14  pedrorib
// Now compiles in g++ 3.3.2
//
// Revision 1.2  2003/11/18 18:21:13  pedrorib
// New version
//
// Revision 1.4  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.3  1998/02/12 18:44:25  mlong
// *** empty log message ***
//
// Revision 1.2  1997/11/11 16:48:06  chess
// *** empty log message ***
//
// Revision 1.2  1997/10/23 19:56:12  chess
// *** empty log message ***
//
// Revision 1.1  1997/05/16 03:22:36  chess
// Initial revision
//
//
//
//--------------------------------------------------------------------------

#ifndef PAIRING_PLAYER_HH
#define PAIRING_PLAYER_HH

#include <string>
namespace Pairing {
	class Player {
		public:
			Player() { }
			Player(int name, int);
			Player(int name, float);
			Player(int name, double);
			Player(int name, float, int);
			Player(int name, float, int, int);
			~Player();
			bool operator<(const Player& p) const {
				return this->floatValue > p.floatValue;
			}

		private:

		public:

			int name;
			float floatValue;
			int  value;
			int rating;

		private:

	};
	struct cmpFloatValueless {
		bool operator()(const Player& p1, const Player& p2) const {
			return p1.floatValue<p2.floatValue;
		}
	};
	struct cmpPlayerByName {
		bool operator()(const Player& p1, const Player& p2) const {
			return p1.name<p2.name;
		}
	};

}

#endif
