/**
* CHESSD - WebClient
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/


/**
* Drag board pieces 
*/

//This variable is used to save original position of piece when move it
var OldPos;

/**
* Start drag a piece when click or click and hold button a button over a piece.
* If click for second time, when click without hold button, then release piece
* and send move to server. If click and hold button was set and release the button
* drop piece and send move to server;
*/
function UTILS_StartDragPiece(Obj, Size, event)
{
	var MousePos;
	var Offset, OffsetLeft, OffsetTop, OffsetBoard;
	
	var BoardPieceOffset;

	if(MainData.CurrentGame == null)
	{
		return false;
	}

	// If ut's not your turn, don't drag
	if ((!MainData.CurrentGame.IsYourTurn) || (MainData.CurrentGame.Finished))
	{
		return false;
	}

	//Obj.style.position = "absolute";

	// Set BoardPiece OffSet
	BoardPieceOffset = new Object;

	if(MainData.Browser == 0) //IE, Internet Explorer
	{
	/*FIXME: only support zone=100% */

		BoardPieceOffset.y = -406;
		BoardPieceOffset.x = 260;
	}
	else if(MainData.Browser == 2) // FF3 Firefox
	{
		BoardPieceOffset.y = -411;
		BoardPieceOffset.x = 260;
	}
	else if(MainData.Browser == 1) // FF2
	{
		BoardPieceOffset.y = -300;
		BoardPieceOffset.x = 0;
	}

	// Getting offsets
	Offset = Size / 2;
	OffsetBoard = UTILS_GetOffset(MainData.CurrentGame.Game.Board);


	// Add half of the piece size and others elements margin and borders
	OffsetTop = OffsetBoard.Y + Offset + 38 + BoardPieceOffset.y;

	OffsetLeft = OffsetBoard.X + Offset - 257 + BoardPieceOffset.x;
	
	//alert(OffsetTop +" = " +OffsetBoard.Y +" + "+ Offset+" + 38 + "+BoardPieceOffset.y +"\n"+OffsetLeft +" = " +OffsetBoard.X +" + "+ Offset+" + 257 + "+BoardPieceOffset.x);

	// Drag piece
	if(document.body.onmouseup == null) // Quick fix to not use global variable
	{
		// Get Obj position
		OldPos = new Object();
		OldPos.x = Obj.style.left.replace(/px/, "");
		OldPos.y = Obj.style.top.replace(/px/, "");

		// Show block border of origin
		GAME_SetBlockBorder((9 - OldPos.y/Size), (OldPos.x/Size + 1));
	
		document.body.onmousemove = function (ev) {
			MousePos = UTILS_GetMouseCoords(ev);

			Obj.style.top = (MousePos.y-OffsetTop)+"px";
			Obj.style.left = (MousePos.x-OffsetLeft)+"px";

			// Change cursor to move
			document.body.style.cursor = "move";
			// Set piece zIndex
			Obj.style.zIndex = 1;

			// If mousedown was set, and piece was moved
			// then set mouseup to stop drag when release the button
			// action: stroke click, keep moving to another place,
			// and then release this click.
			document.body.onmouseup = function(evt){
				var NewPos = new Object();
				var NewCol, NewLine, OldCol, OldLine;
				// Getting mouse coord
				MousePos = UTILS_GetMouseCoords(evt);
				MousePos.x = MousePos.x - OffsetLeft + (Size/2);
				MousePos.y = MousePos.y - OffsetTop + (Size/2);
				/*
				MousePos.x += Size/2;
				MousePos.y += Size/2;
				*/
				// If release outside the board
				if (MousePos.x < 0 || MousePos.x > 9*Size || MousePos.y < 0 || MousePos.y > 10*Size)
				{
					NewPos.x = OldPos.x;
					NewPos.y = OldPos.y;
				}
				else
				{
					NewPos.x = MousePos.x - (MousePos.x % Size);
					NewPos.y = MousePos.y - (MousePos.y % Size);
				}
				// Previous position
				OldLine = 9 - (OldPos.y / Size);
				OldCol = (OldPos.x / Size) + 1;

				// NewPosition
				NewLine = 9 - (NewPos.y / Size);
				NewCol = (NewPos.x / Size) + 1;
				// alert("found mouse up"+OldLine+" "+OldCol+" "+NewLine+" "+NewCol);

				// Set object in the new position
				Obj.style.top =  NewPos.y +"px";
				Obj.style.left = NewPos.x +"px";

				// Remove listener
				//Obj.onmouseup = null;
				document.body.onmousemove = null;
				document.body.onmouseup = null;

				// If piece has been moved
				if ((NewCol != OldCol) || (NewLine != OldLine))
				{
					// Send movement
					GAME_SendMove(OldLine, OldCol, NewLine, NewCol);
					// Show loading move message
					GAME_ShowLoadingMove(MainData.CurrentGame.Id);
				}

				// Return to deafult cursor
				document.body.style.cursor = "default";

				// Remove block border of origin
				GAME_RemoveBlockBorder(OldLine, OldCol);

				delete OldPos;

				return false;
			}
			return false;
		}
	}
	else // Stop drag when click for second time
	{
		var NewPos = new Object();
		var NewCol, NewLine, OldCol, OldLine;

		// Getting mouse coord
		MousePos = UTILS_GetMouseCoords(event);
		MousePos.x = MousePos.x - OffsetLeft + (Size/2);
		MousePos.y = MousePos.y - OffsetTop + (Size/2);
		/*
		MousePos.x += Size/2;
		MousePos.y += Size/2;
		*/
		// If release outside the board
		if (MousePos.x < 0 || MousePos.x > 10*Size || MousePos.y < 0 || MousePos.y > 9*Size)
		{
			NewPos.x = OldPos.x;
			NewPos.y = OldPos.y;
		}
		else
		{
			NewPos.x = MousePos.x - (MousePos.x % Size);
			NewPos.y = MousePos.y - (MousePos.y % Size);
		}
		// Previous position
		OldLine = 9 - (OldPos.y / Size);
		OldCol = (OldPos.x / Size) + 1;

		// NewPosition
		NewLine = 9 - (NewPos.y / Size);
		NewCol = (NewPos.x / Size) + 1;

		// Set object in the new position
		Obj.style.top =  NewPos.y +"px";
		Obj.style.left = NewPos.x +"px";

		// Remove listener
		//Obj.onmouseup = null;
		document.body.onmousemove = null;
		document.body.onmouseup = null;

		// If piece has been moved
		if ((NewCol != OldCol) || (NewLine != OldLine))
		{
			// Send movement
			GAME_SendMove(OldLine, OldCol, NewLine, NewCol);

			// Show loading move message
			GAME_ShowLoadingMove(MainData.CurrentGame.Id);

		}

		// Return to deafult cursor
		document.body.style.cursor = "default";

		// Remove block border of origin
		GAME_RemoveBlockBorder(OldLine, OldCol);

		delete OldPos;

		return false;
	}
	return true;
}


/**
* Return mouse coords
*/
function UTILS_GetMouseCoords(ev)
{
	var X, Y;
	ev = ev || window.event;

	if (ev.pageX || ev.pageY)
	{
		X = ev.pageX;
		Y = ev.pageY;
	}
	else
	{
		X = ev.clientX;
		Y = ev.clientY;
	}
	return {x: X, y:Y};
}
