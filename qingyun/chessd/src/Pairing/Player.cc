//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  Copyright (C) 2003 Pedro R de Andrade Neto
//  
//  $Id: Player.cc,v 1.3 2003/11/20 23:26:14 pedrorib Exp $
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
// Player.cc - Source file for the Player class
//
// Matthew E. Moses && Michael A. Long
//
// $Revision: 1.3 $
// $Date: 2003/11/20 23:26:14 $
//
// $Author: pedrorib $
// $Locker:  $
//
// $Log: Player.cc,v $
// Revision 1.3  2003/11/20 23:26:14  pedrorib
// Now compiles in g++ 3.3.2
//
// Revision 1.2  2003/11/18 18:21:13  pedrorib
// New version
//
// Revision 1.5  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.4  1998/09/10 19:57:17  mlong
// lots of little bug fixes and a few new features
//
// Revision 1.3  1998/02/12 18:44:04  mlong
// ..
//
// Revision 1.2  1997/10/08 21:03:35  chess
// preparing for move to oracle machine at eworks.
//
// Revision 1.1  1997/05/16 03:22:36  chess
// Initial revision
//
// Revision 1.1  1997/05/14 17:00:30  chess
// Initial revision
//
//
//--------------------------------------------------------------------------

// static char RCSid[] = "$Id: Player.cc,v 1.3 2003/11/20 23:26:14 pedrorib Exp $";

#include <cstring>

#include "Player.hh"

namespace Pairing {

	//- Constructor -----------------------------------------------------------
	Player::Player(int n, int wt) : name(n), value(wt) { }

	//- Constructor -----------------------------------------------------------
	Player::Player(int n, float wt) : name(n), floatValue(wt) { }

	//- Constructor -----------------------------------------------------------
	Player::Player(int n, double wt) : name(n), floatValue(wt)  { }

	//- Constructor -----------------------------------------------------------
	Player::Player(int n, float f, int i) :
		name(n),
		floatValue(f),
		value(i) { }

	//- Constructor -----------------------------------------------------------
	Player::Player(int n, float f, int i, int r) :
		name(n),
		floatValue(f),
		value(i),
		rating(r) { }

	//- DeConstructor ---------------------------------------------------------
	Player::~Player() {
	}

}
