//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  Copyright (C) 2003 Pedro R de Andrade Neto
//  
//  $Id: Game.cc,v 1.3 2003/11/20 23:26:14 pedrorib Exp $
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
// Game.cc - Source file for the Game class
//
// Matthew E. Moses && Michael A. Long
//
// $Revision: 1.3 $
// $Date: 2003/11/20 23:26:14 $
//
// $Author: pedrorib $
// $Locker:  $
//
// $Log: Game.cc,v $
// Revision 1.3  2003/11/20 23:26:14  pedrorib
// Now compiles in g++ 3.3.2
//
// Revision 1.2  2003/11/18 18:21:13  pedrorib
// New version
//
// Revision 1.5  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.4  1998/09/22 15:48:40  mlong
// memory leaks fixed
// using insure to compile now.
//
// Revision 1.3  1998/09/10 19:57:17  mlong
// lots of little bug fixes and a few new features
//
// Revision 1.2  1998/04/29 15:23:19  mlong
// prepairing for the move to daimi
// new sorting routine.
//
// Revision 1.1  1997/07/18 15:42:13  chess
// Initial revision
//
//
//--------------------------------------------------------------------------

// static char RCSid[] = "$Id: Game.cc,v 1.3 2003/11/20 23:26:14 pedrorib Exp $";


#include "TourneyGame.hh"

namespace Pairing {

	//- Constructor -----------------------------------------------------------
	Game::Game(int wn, int bn) : whiteName(wn), blackName(bn)
	{
	}

	//- DeConstructor ---------------------------------------------------------
	Game::~Game()
	{
	}

}
