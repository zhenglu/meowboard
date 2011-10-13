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
*
* C3SL - Center for Scientific Computing and Free Software
*/

/**
* Interface functions that control game
*/ 

/***************************
** GAME BOARD OBJECT
*****************************/
function INTERFACE_GameBoardObj(GameID, Player1, Player2, YourColor, PieceSize)
{
	var Tmp;

	// Setting white and black players
	if (YourColor == "white")
	{
		if (Player1.Name == MainData.Username)
		{
			this.WhitePlayer = Player1;
			this.BlackPlayer = Player2;
		}
		else if(Player2.Name == MainData.Username)
		{
			this.WhitePlayer = Player2;
			this.BlackPlayer = Player1;
		}
		else // Observer Mode
		{
			this.WhitePlayer = Player1;
			this.BlackPlayer = Player2;
		}
	}
	else
	{
		if (Player1.Name == MainData.Username)
		{
			this.WhitePlayer = Player2;
			this.BlackPlayer = Player1;
		}
		else if(Player2.Name == MainData.Username)
		{
			this.WhitePlayer = Player1;
			this.BlackPlayer = Player2;
		}
		else // Observer Mode
		{
			this.WhitePlayer = Player1;
			this.BlackPlayer = Player2;
		}
	}

	if (PieceSize == null)
	{
		this.PieceSize = 38;
	}
	else
	{
		this.PieceSize = PieceSize;
	}


	Tmp = INTERFACE_CreateGame(GameID, this.WhitePlayer.Name, this.BlackPlayer.Name, YourColor, this.PieceSize);

	this.Time = new Object();
	this.name = new Object();
	this.photo = new Object();
	this.EventButtons = new Array();

	// Attributes
	this.Game = Tmp.GameDiv;
	this.Board = Tmp.Board;
	this.BoardBlocks = Tmp.BoardBlocks;
	this.Time.WTime = Tmp.WTimer;
	this.Time.BTime = Tmp.BTimer;
	this.photo.wphoto = Tmp.WPhoto;
	this.photo.bphoto = Tmp.BPhoto;
	this.MoveList = Tmp.MoveList;
	this.EventButtons.push(Tmp.OptionsButtons);
	this.EventButtons.push(Tmp.GameClose);
	this.LoadingMove = Tmp.LoadingMove;
	this.LeaveUser = Tmp.LeaveUserDiv;
	this.LeaveUserText = Tmp.LeaveUserText;
	this.tab = Tmp.Tab;

	this.Timer = null; // used to set interval
	this.Turn = "white";
	this.LastMove = null;


	this.MyColor = YourColor;
	this.Id = GameID;
	this.Finished = false;

	
	// Public methods
	this.Show = INTERFACE_ShowGame;
	this.Finish = INTERFACE_FinishGame;
	this.Hide = INTERFACE_HideGame;
	this.Remove = INTERFACE_RemoveGame;

	this.UpdateBoard = INTERFACE_UpdateBoard;
	this.UndoMove = INTERFACE_UndoMove;
	this.AddMove = INTERFACE_AddMove;

	this.SetTurn = INTERFACE_SetTurn;
	this.SetWTime = INTERFACE_SetWTime;
	this.SetBTime = INTERFACE_SetBTime;

	this.UpdateWTime = INTERFACE_UpdateWTime;
	this.UpdateBTime = INTERFACE_UpdateBTime;

	this.DecreaseTime = INTERFACE_DecreaseTime;
	this.StartTimer = INTERFACE_StartTimer;
	this.StopTimer = INTERFACE_StopTimer;

	this.RemovePiece = INTERFACE_RemovePiece;
	this.InsertPiece = INTERFACE_InsertPiece;

	this.SetBoard = INTERFACE_SetBoard;
	this.ClearBoard = INTERFACE_ClearBoard;

	this.SetWPhoto = INTERFACE_SetWPhoto;
	this.SetBPhoto = INTERFACE_SetBPhoto;

	this.removeMove = INTERFACE_RemoveMove;

	this.ObserverMode = INTERFACE_ObserverMode;
	this.OldGameMode = INTERFACE_OldGameMode;

	this.SetLastMove = INTERFACE_LastMove;
	this.FindBlock = INTERFACE_FindBlock;
	this.SetBlockBorder = INTERFACE_SetBlockBorder;
	this.RemoveBlockBorder = INTERFACE_RemoveBlockBorder;

	this.ShowLoadingMove = INTERFACE_ShowLoadingMove;
	this.HideLoadingMove = INTERFACE_HideLoadingMove;

	this.ShowLeaveUser = INTERFACE_ShowLeaveUser;
	this.HideLeaveUser = INTERFACE_HideLeaveUser;

	this.SetWTime();
	this.SetBTime();
}



/**
* Show this game in the interface
*
* @public
* @param	void
* @return	true if game is showed with sucess, else false
* @author	Rubens and Pedro
*/
function INTERFACE_ShowGame()
{
	var Div = document.getElementById("Center");

	if (Div)
	{
		Div.appendChild(this.Game);
		return true;
	}
	return false;
}


/**
* Hide this game in the interface
*
* @public
* @param	void
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_HideGame()
{
	var ParentGame = this.Game.parentNode;

	ParentGame.removeChild(this.Game);
}


/**
* Disable options and drag pieces
*
* @public
* @param	void
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_FinishGame()
{
	this.Finished = true;
}


/**
* Remove this game
*
* @public
* @param	void
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_RemoveGame()
{
	var GameObj = MainData.GetGame(this.Id);
	var Board = this.Game;

	
	// If game exists
	if (GameObj != null)
	{	
		// if board isn't opened, do nothing.
		if(Board == null)
		{
			return false
		}
		// else if game is no finished, show message.
		else if (GameObj.Finished == false)
		{
			WINDOW_Alert(UTILS_GetText("game_remove_game"));
			return false;
		}
	}

	//Remove board from interface
	Board.parentNode.removeChild(Board);
	return true;

}

/*
* Set game interface to observer mode (Move list without options)
*
* @public
* @param	void
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_ObserverMode()
{
	var MoveList = INTERFACE_CreateMoveList();
	var NewTab = INTERFACE_CreateOldGameTab(MoveList.Div);

	var TabParent = this.tab.parentNode;

	TabParent.removeChild(this.tab);
	TabParent.appendChild(NewTab);

	this.MoveList = MoveList.List;
	this.tab = NewTab;
}

/*
* Set game interface to oldgame mode(Observer mode with buttons to review(?) game)
*
* @public
* @param	void
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_OldGameMode()
{
	var MoveList = INTERFACE_CreateOldGameMoveList();
	var NewTab = INTERFACE_CreateOldGameTab(MoveList.Div);

	var TabParent = this.tab.parentNode;

	TabParent.removeChild(this.tab);
	TabParent.appendChild(NewTab);

	this.MoveList = MoveList.List;
	this.tab = NewTab;
}

/**
* Clean all pieces of a board
*
* @param	void
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_ClearBoard()
{
	var i,j;

	for(i=0 ; i<10 ; i++)
	{
		for(j=0 ; j<9 ; j++)
		{
			this.RemovePiece(i+1 ,j+1);
		}
	}
}

/**
* Clean the board and show 'BoardArray' in the screen
*
* @param	BoardArray is board in array of array format
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_SetBoard(BoardArray)
{
	var i,j;	
	var Piece;

	this.ClearBoard();

	for(i=0 ; i<10 ; i++)
	{
		for(j=0 ; j<9 ; j++)
		{
			Piece = BoardArray[i].charAt(j);
			if (Piece != "-")
			{
				this.InsertPiece(Piece, i+1, j+1, this.MyColor);
			}
		}
	}
}


/**
* Display a piece in specified Line and Col of the board
*
* @param	Piece is piece char
* @param 	Line is line where is the piece will be place
* @param 	Col is column where is the piece will be place
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_InsertPiece(Piece, Line, Col)
{
	var Board = this.Board; 
	var PieceImg = INTERFACE_NewPiece(Piece, this.MyColor, this.PieceSize);

	// If it's a white player
	if (this.MyColor == "white")
	{
		PieceImg.style.left = ((Col-1) * this.PieceSize)  +"px";
		PieceImg.style.top  = ((Line-1) * this.PieceSize) +"px";
	}
	// Black player
	else
	{
		PieceImg.style.left = ((9 - Col) * this.PieceSize) +"px";
		PieceImg.style.top  = ((10 - Line) * this.PieceSize)+"px";
	}
	PieceImg.id = this.Id+"_piece_"+UTILS_HorizontalIndex(Col)+(10-Line);
	

	//alert(Line+","+Col +"///"+ PieceImg.style.top +" - "+ PieceImg.style.left);
	Board.appendChild(PieceImg);
}


/**
* Remove a piece from the board
*
* @param 	Line is line where the piece was placed
* @param 	Col is column where the piece was placed
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_RemovePiece(Line, Col)
{
	var PieceId = this.Id+"_piece_"+UTILS_HorizontalIndex(Col)+(10-Line);
	var Piece = document.getElementById(PieceId);

	if (Piece != null)
	{
		Piece.parentNode.removeChild(Piece);
		return Piece;
	}
	else
	{
		return null;
	}
}

/**
* Update the board on the screen, where the pieces is diferrent
*
* @param 	OldBoard is current Board
* @param 	NewBoard is new board that will be show
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_UpdateBoard(OldBoard, NewBoard)
{
	var i,j;	
	var Piece;

	for (i=0 ; i<10 ; i++)
	{
		for (j=0 ; j<9 ; j++)
		{
			if (OldBoard[i].charAt(j) != NewBoard[i].charAt(j))
			{
				Piece = NewBoard[i].charAt(j);
				if (Piece == '-')
				{
					this.RemovePiece(i+1, j+1);
				}
				else
				{
					if (OldBoard[i].charAt(j)!= "-")
					{
						this.RemovePiece(i+1, j+1);
					}
					this.InsertPiece(Piece, i+1, j+1);
				}
			}	
		}
	}
}

/**
* Undo the last move done by the user
*
* @param 	void
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_UndoMove()
{
	var Game = MainData.GetGame(this.Id);

	if (Game == null)
	{
		return false;
	}

	try
	{
		this.SetBoard(Game.Moves[Game.Moves.length-1].Board);
	}
	catch(e)
	{
		return false;
	}
	return true;
}

/**
* Show turn info to user
*
* @param 	Color is the player color
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_SetTurn(Color)
{
	var TurnNode, OtherNode;

	if (Color == "white")
	{
		TurnNode = document.getElementById("PWName");
		OtherNode = document.getElementById("PBName");
	}
	else
	{
		TurnNode = document.getElementById("PBName");
		OtherNode = document.getElementById("PWName");
	}
	
	if (!TurnNode || !OtherNode)
	{
		return false;
	}
	// Set the turn player to bold, underlined
	TurnNode.style.fontWeight = "bold";
	TurnNode.style.textDecoration = "underline";

	// Set other player to normal
	OtherNode.style.fontWeight = "normal";
	OtherNode.style.textDecoration = "none";

	this.Turn = Color;
	return true;
}

/**
* Decrease user time. Executed each second
*
* @param 	void
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_DecreaseTime()
{
	if (MainData.CurrentGame.Game.Turn == "white")
	{
		MainData.CurrentGame.Game.WhitePlayer.Time -= 1;
		MainData.CurrentGame.Game.SetWTime();
	}
	else
	{
		MainData.CurrentGame.Game.BlackPlayer.Time -= 1;
		MainData.CurrentGame.Game.SetBTime();
	}
}

/**
* Start timer
*
* @param 	void
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_StartTimer()
{
	this.Timer = setInterval(this.DecreaseTime, 1000);
}

/**
* Stop timer
*
* @param 	void
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_StopTimer()
{
	if(this.Timer != undefined)
	{
		this.Timer = window.clearInterval(this.Timer);
	}
}


/**
* Update white timer
*
* @param 	NewTime is the new time to be update
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_UpdateWTime(NewTime)
{
	this.WhitePlayer.Time = NewTime;
}

/**
* Update black timer
*
* @param 	NewTime is the new time to be update
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_UpdateBTime(NewTime)
{
	this.BlackPlayer.Time = NewTime;
}

/**
* Show white timer on interface
*
* @param 	void
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_SetWTime()
{
	var min, sec;
	var minStr, secStr;

	if (this.WhitePlayer.Time <= 0)
	{
		this.Timer = clearInterval(this.Timer);
	}

	min = Math.floor(this.WhitePlayer.Time / 60);
	sec = this.WhitePlayer.Time % 60;

	if(min < 10)
	{
		minStr = "0"+min;
	}
	else
	{
		minStr = min;
	}

	if(sec < 10)
	{
		secStr = "0"+sec;
	}
	else
	{
		secStr = sec;
	}

	this.Time.WTime.innerHTML = minStr+":"+secStr;
}

/**
* Show black timer on interface
*
* @param 	void
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_SetBTime()
{
	var min, sec;
	var minStr, secStr;

	if (this.BlackPlayer.Time <= 0)
	{
		this.Timer = clearInterval(this.Timer);
	}

	min = Math.floor(this.BlackPlayer.Time / 60);
	sec = this.BlackPlayer.Time % 60;

	if(min < 10)
	{
		minStr = "0"+min;
	}
	else
	{
		minStr = min;
	}

	if(sec < 10)
	{
		secStr = "0"+sec;
	}
	else
	{
		secStr = sec;
	}

	this.Time.BTime.innerHTML = minStr+":"+secStr;
}

/**
* Show white avatar image (in base64)
*
* @param 	PhotoType is the image type (png/gif)
* @param 	PhotoStr is the image in base64 format string
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_SetWPhoto(Img)
{
	this.photo.wphoto.src = IMAGE_ImageDecode(Img);
}

/**
* Show black avatar image (in base64)
*
* @param 	PhotoType is the image type (png/gif)
* @param 	PhotoStr is the image in base64 format string
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_SetBPhoto(Img)
{
	this.photo.bphoto.src = IMAGE_ImageDecode(Img);
}

/**
* Show a new move in Move List
*
* @param 	NumTurn is the turn number
* @param 	Move is the move done
* @param 	WTime is white time when move is done
* @param 	BTime is black time when move is done
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_AddMove(NumTurn, Move, ShortMove, WTime, BTime)
{
	var ScrollTop, ScrollHeight, ClientHeight;
	var Num;
	var FullMove;
	var MoveSpan;
	var Item;

	// NumTurn-1 is a Quickfix to display moves in move list 
	// without the first board of game (contains no move and shortmove)
	FullMove = Math.ceil((NumTurn-1)/2);
	if ((NumTurn-1) % 2 == 1)
	{
		// Create a item on list 
		if(FullMove % 2 == 1)
		{
			Item = UTILS_CreateElement("li",this.Id+"_"+FullMove,"white",null);
		}
		else
		{
			Item = UTILS_CreateElement("li",this.Id+"_"+FullMove,"black",null);
		}
		Num = UTILS_CreateElement("p", null, null, FullMove+".");
		Item.appendChild(Num);
	}
	else
	{
		// TODO -> remove getElementById, and set list of pointer to
		// li HTML element in board object;

		// Get item from list 
		Item = document.getElementById(this.Id+"_"+((NumTurn-1)/2));
	}

	MoveSpan = UTILS_CreateElement("p", null, "move", ShortMove);

	Item.appendChild(MoveSpan);

	//Players can see old moves when game is finished
	//MainData.CurrentGame.Finished is not used here because
	//observer game set Finished = true;
	if(MainData.CurrentGame == null)
	{
		UTILS_AddListener(MoveSpan, "click", function(){ OLDGAME_GotoBoard(NumTurn); }, false);
	}

	this.MoveList.appendChild(Item);

	// Set Movelist scroll position;
	ScrollTop = this.MoveList.scrollTop;
	ScrollHeight = this.MoveList.scrollHeight;
	ClientHeight = this.MoveList.clientHeight;

	if((ScrollHeight > ClientHeight) && ((ScrollTop+1)>=(ScrollHeight-ClientHeight-20)))
	{
		this.MoveList.scrollTop += 20;
	}
}

/**
* Remove last move in Move List (this is should be used when players agree to return one move)
*
* @param 	void
* @return	void
* @author	Rubens and Pedro
*/
function INTERFACE_RemoveMove()
{
	this.MoveList.removeChild(this.MoveList.lastChild);
}



