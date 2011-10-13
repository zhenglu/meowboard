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
* This file contains OldGame 
*/

/**
* Handle Search Old Games Messages
*
* @param        XML The xml that contains the string 'search_game' in xmlns attribute
* @return       Buffer with the messages that must be send
* @author       Rubens;
*/
function OLDGAME_HandleSearchOldGame(XML)
{
	var Games, GameList;
	var i;
	var Buffer="";
	var GameList = new Array();
	var GameInfoTmp;
	var Players;
	var Result;
	var More;
	var Id, SearchGameWindow;
	var LoadingBox;


	// Get window's Id
	Id = XML.getAttribute("id").split("-")[1];

	SearchGameWindow = MainData.GetSearchGameInfo(Id);
	Games = XML.getElementsByTagName("game");
	if(XML.getElementsByTagName("more")[0] != undefined)
	{
		SearchGameWindow.More = true;
		More = true;
	}
	else
	{
		SearchGameWindow.More = false;
		More = false;
	}

	// Create a Array of game infos object
	for(i=0; i<Games.length; i++)
	{
		// Create a game object and set attributes (white, black,
		// winner, date, id, gametype, wintype)
		GameInfoTmp = new Object();
		Players = Games[i].getElementsByTagName("player");

		if(Players[0].getAttribute("role")=="white")
		{
			GameInfoTmp.white = Players[0].getAttribute("jid").split("@")[0];
			GameInfoTmp.black = Players[1].getAttribute("jid").split("@")[0];

			if(Players[0].getAttribute("score")=="1")
			{
				GameInfoTmp.winner = UTILS_GetText("oldgame_white");
			}
			else if(Players[1].getAttribute("score")=="1")
			{
				GameInfoTmp.winner = UTILS_GetText("oldgame_black");
			}
			else
			{
				GameInfoTmp.winner = UTILS_GetText("oldgame_draw");
			}
		}
		else
		{
			GameInfoTmp.white = Players[1].getAttribute("jid").split("@")[0];
			GameInfoTmp.black = Players[0].getAttribute("jid").split("@")[0];

			if(Players[0].getAttribute("score")=="1")
			{
				GameInfoTmp.winner = UTILS_GetText("oldgame_black");
			}
			else if(Players[1].getAttribute("score")=="1")
			{
				GameInfoTmp.winner = UTILS_GetText("oldgame_white");
			}
			else
			{
				GameInfoTmp.winner = UTILS_GetText("oldgame_draw");
			}

		}


		GameInfoTmp.date = UTILS_ConvertTimeStamp(Games[i].getAttribute("time_stamp"));
		GameInfoTmp.gametype = UTILS_GetGameCategory(Games[i].getAttribute("category"));
		GameInfoTmp.id = Games[i].getAttribute("id");
		GameInfoTmp.result = OLDGAME_GameResult(Games[i].getAttribute("result"));
	
		GameList.push(GameInfoTmp);
	}

	// Remove loading message
	INTERFACE_HideOldgameLoading();

//	WINDOW_OldGameResult(GameList);

	SearchGameWindow.Elements.SetResult(Id, GameList, More);
	SearchGameWindow.Elements.SetPlayer1(SearchGameWindow.P1);
	SearchGameWindow.Elements.SetPlayer2(SearchGameWindow.P2);
	SearchGameWindow.Elements.SetColor(SearchGameWindow.Color);
	SearchGameWindow.Elements.SetFrom(SearchGameWindow.From);
	SearchGameWindow.Elements.SetTo(SearchGameWindow.To);

	return Buffer;
}

/** 
* Start Game in OldGame Mode
* 
* @param        P1 = Player 1 Object (Name, Time, Color, Inc)
* @param        P2 = Player 2 Object (Name, Time, Color, Inc)
* @return       void 
* @see		MainData methods and Game Interface Object;
* @author       Rubens 
*/
function OLDGAME_StartOldGame(OldGameId, P1, P2)
{
	var GameDiv;
	var Index;
	var Color;
	var Buffer = "";

	// Remove welcome div
	INTERFACE_RemoveWelcome();

	// Hide current game
	if (MainData.CurrentOldGame != null)
	{
		//In this version, user can only see one OldGame
		//MainData.CurrentOldGame.Game.Hide();
		Buffer += OLDGAME_RemoveOldGame(OldGameId);

	}

	// Check if player is watch own old game
	if(P1.Name == MainData.Username)
	{
		Color = P1.Color;
	}
	else if (P2.Name == MainData.Username)
	{
		Color = P2.Color;
	}
	else //See anothers players old game
	{
		Color = "white";
	}

	// 38 -> default piece size
	GameDiv = new INTERFACE_GameBoardObj(OldGameId, P1, P2, Color, 38);
	Index = MainData.AddOldGame(P1, P2, Color, "none", GameDiv);

	// Show New Game
	GameDiv.Show();
	// Set Old Game Mode
	GameDiv.OldGameMode();

	//Change "X" close board button function when clicked
	GameDiv.EventButtons[GameDiv.EventButtons.length-1].onclick = function(){ OLDGAME_RemoveOldGame(Index)};

	Buffer += MESSAGE_GetProfile(P1.Name,MainData.Const.IQ_ID_OldGamePhoto);
	Buffer += MESSAGE_GetProfile(P2.Name,MainData.Const.IQ_ID_OldGamePhoto);

	//Change user status to observer
	//Buffer += CONTACT_ChangeStatus("","return");

	return Buffer;
}

/**
* Handle Game State
* It's a good ideia to read the server's documentation before reading the code above
*
* @param 	XML The xml that contains the game state
* @return 	void
* @author 	Ulysses and Rubens
*/
function OLDGAME_FetchOldGame(XML)
{
	var GameTag;
	var GamePos, PlayerTag;
	var Player1 = new Object();
	var Player2 = new Object();
	var Buffer = "";
	var History;

	GameTag = XML.getElementsByTagName("game")[0];

	//GamePos = MainData.OldGameList.length;
	GamePos = 0;

	PlayerTag = XML.getElementsByTagName("player");

	History = XML.getElementsByTagName("history")[0];

	Player1.Name = PlayerTag[0].getAttribute('jid').replace(/@.*/,"");
	Player1.Inc = PlayerTag[0].getAttribute('inc');
	Player1.Color = PlayerTag[0].getAttribute('color');
	Player1.Time = PlayerTag[0].getAttribute('time');
		
	Player2.Name = PlayerTag[1].getAttribute('jid').replace(/@.*/,"");
	Player2.Inc = PlayerTag[1].getAttribute('inc');
	Player2.Color = PlayerTag[1].getAttribute('color');
	Player2.Time = PlayerTag[1].getAttribute('time');

	// Open a board
	Buffer = OLDGAME_StartOldGame(GamePos, Player1, Player2);

	// Load history moves
	Buffer += OLDGAME_LoadGameHistory(GamePos, History, Player1, Player2);

	return Buffer;
}

/**
* Load all game history moves done in the game
*
* @param        GameId is the game identificator
* @param        HistoryXml is a XML that contains all games states
* @param        Player1 = Player 1 Object (Name, Time, Color, Inc)
* @param        Player2 = Player 2 Object (Name, Time, Color, Inc)
* @return       void
* @author       Rubens
*/
function OLDGAME_LoadGameHistory(GamePos, HistoryXml, Player1, Player2)
{
	var i;
	var StartP1Time, StartP2Time, HTurn, HTime, HBoard, HMove, HShortMove;
	var HPlayer1 = new Object();
	var HPlayer2 = new Object();
	var HistoryMoves;
	var Buffer;

	if(HistoryXml == undefined)
	{
		return "";
	}

	HistoryMoves = HistoryXml.getElementsByTagName("state");

	StartP1Time = HistoryXml.getElementsByTagName("player")[0].getAttribute("time");
	StartP2Time = HistoryXml.getElementsByTagName("player")[1].getAttribute("time");
	HPlayer1.Name = Player1.Name;
	HPlayer1.Inc = Player1.Inc;
	HPlayer1.Color = Player1.Color;
	HPlayer1.Time = StartP1Time;

	HPlayer2.Name = Player2.Name;
	HPlayer2.Inc = Player2.Inc;
	HPlayer2.Color = Player2.Color;
	HPlayer2.Time = StartP2Time;

	if(HistoryMoves.length == 0)
	{
		WINDOW_Alert(UTILS_GetText("game_observer_alert_title"), UTILS_GetText("oldgame_no_moves"));
	}
	else
	{
		// Load game history
		for(i=0 ; i<HistoryMoves.length; i++)
		{
			HTime = HistoryMoves[i].getAttribute("time");
			HTurn = HistoryMoves[i].getAttribute("turn");
			HBoard = HistoryMoves[i].getAttribute("board");
			HMove = HistoryMoves[i].getAttribute("move");
			HShortMove = HistoryMoves[i].getAttribute("short");

			if(HTurn == "white")
			{
				HPlayer2.Time = HTime;
			}
			else
			{
				HPlayer1.Time = HTime;
			}

			Buffer += OLDGAME_UpdateBoard(GamePos, HBoard, HMove, HShortMove, HPlayer1, HPlayer2, HTurn)
		}

		OLDGAME_FirstBoard();
	}

	MainData.CurrentOldGame.Game.HideLoadingMove();
	return Buffer;
}

/**
* Update board in data struct and interface
*
* @param        GameId = Game number
* @param        BoardStr = Board status in a string
* @param        Move = Chess Move (Piece/Orig-Dest)
* @param        P1 = Player 1 Object (Name, Time, Color, Inc)
* @param        P2 = Player 2 Object (Name, Time, Color, Inc)
* @param        TurnColor = color ("white"/"black")
* @return       void
* @author       Rubens
*/
function OLDGAME_UpdateBoard(GamePos, BoardStr, Move, ShortMove, P1, P2, TurnColor)
{
	var NewBoardArray = UTILS_String2Board(BoardStr);
	var CurrentBoardArray;
	var Game;

	// Get game from GameList
	//Game = MainData.GetOldGame(GamePos);
	// In this version, user can only see one OldGame
	Game = MainData.CurrentOldGame;
	
//FIXME: comment this for useless	if (Game.CurrentMove != null)
//	{
//		CurrentBoardArray = Game.Moves[Game.CurrentMove].Board;
//	}
	// If there's no previous moves
//	else
//	{
	//	CurrentBoardArray = new Array("---------","---------","---------","---------","---------","---------","---------","---------","---------","---------");
	//}

	// Update data sctructure
	if (P1.Color == "white")
	{
		Game.AddMove(NewBoardArray, Move, ShortMove, P1.Time, P2.Time, TurnColor);
		Game.Game.UpdateWTime(P1.Time);
		Game.Game.UpdateBTime(P2.Time);
	}
	else
	{
		Game.AddMove(NewBoardArray, Move, ShortMove, P2.Time, P1.Time, TurnColor);
		Game.Game.UpdateWTime(P2.Time);
		Game.Game.UpdateBTime(P1.Time);
	}

	if((Move !="") || (ShortMove !=""))
	{
		Game.Game.AddMove(Game.Moves.length, Move, ShortMove, P1.Time, P2.Time);
	}

	return "";
}


/** 
* Change board GameMode to OldGame Mode when game is over
* 
* @param        Id is Game ID  
* @return       void 
* @see		MainData methods and Game Interface Object;
* @author       Rubens 
*/
function OLDGAME_EndGame(Id)
{
	var EndedGame;
	var PWName, PBName, Color;
	var NewOldGame;
	var i;
	var MoveObj;
	var Index;
	
	EndedGame = MainData.RemoveGame(Id);

	//Add game to oldgamelist, set this game to CurrentOldGame
	//and return EndedGame Pos;
	Index = MainData.PushOldGame(EndedGame);

	MainData.CurrentOldGame.Color = "none";

	PWName = MainData.CurrentOldGame.PW;
	PBName = MainData.CurrentOldGame.PB;
	Color = MainData.CurrentOldGame.BoardColor;

	NewOldGame = MainData.CurrentOldGame.Game;
	NewOldGame.OldGameMode();

	//Change "X" close board button function when clicked
	NewOldGame.EventButtons[NewOldGame.EventButtons.length-1].onclick = function(){ OLDGAME_RemoveOldGame(Index)};
	
	OLDGAME_LastBoard();
	
	//Reload all moves done in MoveList
	for(i=1 ; i<MainData.CurrentOldGame.Moves.length; i++)
	{
		MoveObj = MainData.CurrentOldGame.Moves[i];
		// i+1 is a QuickFix
		NewOldGame.AddMove((i+1), MoveObj.Move, MoveObj.ShortMove, MoveObj.PWTime, MoveObj.PBTime)
	}

	NewOldGame.HideLoadingMove();
}


/**
* Handle Game Players Photo
*
* @param        XML The xml that contains vCard photo
* @return       none
* @author       Rubens
*/
function OLDGAME_HandleVCardPhoto(XML)
{
	var Photo;
	var Player;
	var Binval;
	var PhotoType;
	var Img;

	if( MainData.CurrentOldGame == null)
	{
		return "";
	}

	// Get player image
	Photo = XML.getElementsByTagName("PHOTO")[0];

	// If player don't use any image, do nothing
	if(Photo == null)
	{
		return "";
	}

	// Get photo image 
	PhotoType = UTILS_GetNodeText(Photo.getElementsByTagName("TYPE")[0]);
	Binval = UTILS_GetNodeText(Photo.getElementsByTagName("BINVAL")[0]);
	Img = "data:"+PhotoType+";base64,"+Binval;

	Player = XML.getAttribute("from").split("@")[0];

	// Update current old game player image
	if(MainData.CurrentOldGame.PW.Name == Player)
	{
		MainData.CurrentOldGame.WPhoto = Img;
		MainData.CurrentOldGame.Game.SetWPhoto(Img);
	}
	else if(MainData.CurrentOldGame.PB.Name == Player)
	{
		MainData.CurrentOldGame.BPhoto = Img;
		MainData.CurrentOldGame.Game.SetBPhoto(Img);
	}

	return "";
}


/** 
* Remove OldGame board from interface and OldGameList
* 
* @param        Index is array index of OldGame
* @return       void 
* @see		MainData methods and Game Interface Object;
* @author       Rubens 
*/
function OLDGAME_RemoveOldGame(Index)
{
	var Room;
	var Buffer = "";
	var OldGame;

	OldGame = MainData.OldGameList[Index];

	//Remove Board from interface
	OldGame.Game.Remove();

	//Exit room if is there some room affiliated with oldgame;
	Room = MainData.GetRoom(OldGame.Id);
	if(Room != null)
	{
		// Send a message to leave from room
		Buffer += ROOM_ExitRoom(Room.Name);
	}

	MainData.RemoveOldGame(Index);

}

/** 
* Change current board to first game board
* 
* @param        void
* @return       void 
* @see		MainData methods and Game Interface Object;
* @author       Rubens 
*/
function OLDGAME_FirstBoard()
{
	var ObserverColor, Color, Board;
	var MovePos = MainData.CurrentOldGame.CurrentMove;
	var WTime, BTime;
	var Move;

	ObserverColor = MainData.CurrentOldGame.YourColor;
	Color = MainData.CurrentOldGame.BoardColor;
	Board = MainData.CurrentOldGame.Moves[0].Board;

	Move = MainData.CurrentOldGame.Moves[0].Move;

	WTime = MainData.CurrentOldGame.Moves[ 0 ].PWTime;
	BTime = MainData.CurrentOldGame.Moves[ 0 ].PBTime;

	MainData.CurrentOldGame.CurrentMove = 0;
	MainData.CurrentOldGame.Game.SetBoard(Board, Color, 38);
	MainData.CurrentOldGame.Game.UpdateWTime(WTime);
	MainData.CurrentOldGame.Game.UpdateBTime(BTime);
	MainData.CurrentOldGame.Game.SetWTime();
	MainData.CurrentOldGame.Game.SetBTime();
	MainData.CurrentOldGame.Game.SetLastMove(Move, ObserverColor);
}

/** 
* Change current board to previous game board
* 
* @param        void
* @return       void 
* @see		MainData methods and Game Interface Object;
* @author       Rubens 
*/
function OLDGAME_PrevBoard()
{
	var ObserverColor, OldBoard, Board;
	var MovePos = MainData.CurrentOldGame.CurrentMove;
	var WTime, BTime;
	var Move; 

	if(MovePos == 0)
	{
		return false;
	}

	ObserverColor = MainData.CurrentOldGame.YourColor;
	Color = MainData.CurrentOldGame.BoardColor;
	OldBoard = MainData.CurrentOldGame.Moves[MovePos].Board;
	Board = MainData.CurrentOldGame.Moves[MovePos -1].Board;
	Move = MainData.CurrentOldGame.Moves[MovePos - 1].Move;

	WTime = MainData.CurrentOldGame.Moves[ MovePos - 1 ].PWTime;
	BTime = MainData.CurrentOldGame.Moves[ MovePos - 1 ].PBTime;

	MainData.CurrentOldGame.CurrentMove = MovePos - 1;
	MainData.CurrentOldGame.Game.UpdateBoard(OldBoard, Board, Color, 38);
	MainData.CurrentOldGame.Game.UpdateWTime(WTime);
	MainData.CurrentOldGame.Game.UpdateBTime(BTime);
	MainData.CurrentOldGame.Game.SetWTime();
	MainData.CurrentOldGame.Game.SetBTime();
	MainData.CurrentOldGame.Game.SetLastMove(Move, ObserverColor);

	return true;
}

/** 
* Change current board to next game board
* 
* @param        void
* @return       void 
* @see		MainData methods and Game Interface Object;
* @author       Rubens 
*/
function OLDGAME_NextBoard()
{
	var Color, ObserverColor, OldBoard, Board;

	var MovePos = MainData.CurrentOldGame.CurrentMove;
	var WTime, BTime;
	var Move; 

	if(MovePos == MainData.CurrentOldGame.Moves.length-1)
	{
		return false;
	}

	ObserverColor = MainData.CurrentOldGame.YourColor;
	Color = MainData.CurrentOldGame.BoardColor;
	OldBoard = MainData.CurrentOldGame.Moves[MovePos].Board;
	Board = MainData.CurrentOldGame.Moves[ MovePos + 1 ].Board;
	Move = MainData.CurrentOldGame.Moves[MovePos + 1].Move;

	WTime = MainData.CurrentOldGame.Moves[ MovePos + 1 ].PWTime;
	BTime = MainData.CurrentOldGame.Moves[ MovePos + 1 ].PBTime;

	MainData.CurrentOldGame.CurrentMove = MovePos + 1;
	MainData.CurrentOldGame.Game.UpdateBoard(OldBoard, Board, Color, 38);
	MainData.CurrentOldGame.Game.UpdateWTime(WTime);
	MainData.CurrentOldGame.Game.UpdateBTime(BTime);
	MainData.CurrentOldGame.Game.SetWTime();
	MainData.CurrentOldGame.Game.SetBTime();
	MainData.CurrentOldGame.Game.SetLastMove(Move, ObserverColor);

	return true;
}

/** 
* Change current board to last game board
* 
* @param        void
* @return       void 
* @see		MainData methods and Game Interface Object;
* @author       Rubens 
*/
function OLDGAME_LastBoard()
{
	var ObserverColor, Color, Board;
	var MoveListLen = MainData.CurrentOldGame.Moves.length;
	var WTime, BTime;
	var Move; 

	ObserverColor = MainData.CurrentOldGame.YourColor;
	Color = MainData.CurrentOldGame.BoardColor;
	Board = MainData.CurrentOldGame.Moves[MoveListLen-1].Board;
	Move = MainData.CurrentOldGame.Moves[MoveListLen -1].Move;

	WTime = MainData.CurrentOldGame.Moves[ MoveListLen - 1 ].PWTime;
	BTime = MainData.CurrentOldGame.Moves[ MoveListLen - 1 ].PBTime;

	MainData.CurrentOldGame.CurrentMove = MoveListLen-1;
	MainData.CurrentOldGame.Game.SetBoard(Board, Color, 38);
	MainData.CurrentOldGame.Game.UpdateWTime(WTime);
	MainData.CurrentOldGame.Game.UpdateBTime(BTime);
	MainData.CurrentOldGame.Game.SetWTime();
	MainData.CurrentOldGame.Game.SetBTime();
	MainData.CurrentOldGame.Game.SetLastMove(Move, ObserverColor);
}

/** 
* Change current board to some game board
* 
* @param        NumBoard in Board Number
* @return       void 
* @see		MainData methods and Game Interface Object;
* @author       Rubens 
*/
function OLDGAME_GotoBoard(NumBoard)
{
	var ObserverColor, Color, Board;
	var WTime, BTime;
	var Move; 

	ObserverColor = MainData.CurrentOldGame.YourColor;
	Color = MainData.CurrentOldGame.BoardColor;
	Board = MainData.CurrentOldGame.Moves[NumBoard-1].Board;
	Move = MainData.CurrentOldGame.Moves[NumBoard -1].Move;

	WTime = MainData.CurrentOldGame.Moves[ NumBoard - 1 ].PWTime;
	BTime = MainData.CurrentOldGame.Moves[ NumBoard - 1 ].PBTime;

	MainData.CurrentOldGame.CurrentMove = NumBoard-1;
	MainData.CurrentOldGame.Game.SetBoard(Board, Color, 38);
	MainData.CurrentOldGame.Game.UpdateWTime(WTime);
	MainData.CurrentOldGame.Game.UpdateBTime(BTime);
	MainData.CurrentOldGame.Game.SetWTime();
	MainData.CurrentOldGame.Game.SetBTime();
	MainData.CurrentOldGame.Game.SetLastMove(Move, ObserverColor);
}

/**
* Create search game info in data Struct and send old game search message
*
* @return       void
* @author       Danilo
*/
function OLDGAME_OpenOldGameWindow(User)
{	
	if (document.getElementById("OldGamesDiv"))
		return;

	var Elements;
	var SearchInfo;
	var Id = MainData.SearchGameMaxId; 
	MainData.SearchGameMaxId++; 

	if (User == null)
	{
		User ="";
	}

	Elements = WINDOW_OldGame(Id);

	MainData.AddSearchGameInfo(Id, Elements, User);

	CONNECTION_SendJabber(MESSAGE_GetOldGames(Id,User,"",10,0));

	SearchInfo = MainData.GetSearchGameInfo(Id);
	SearchInfo.Elements.SetSearchButton(SearchInfo);
	SearchInfo.Elements.SetPrevButton(SearchInfo);
	SearchInfo.Elements.SetNextButton(SearchInfo);
}

/**
* Remove Search Game Info in data struct and set new old game window max id count
*
* @param				Id		Window's id
* @return       void
* @author       Danilo
*/
function OLDGAME_CloseWindow(Id)
{
	MainData.RemoveSearchGameInfo(Id);

	if (Id == MainData.SearchGameMaxId - 1)
		MainData.SearchGameMaxId--;
	else if (MainData.SearchGameInfoList.length == 0)
		MainData.SearchGameMaxId = 0;
}

/**
* Return apropriate result text according to result string 
*
* @param Result Result's string
* @return String
* @see OLDGAME_HandleSearchOldGame
* @author Danilo
*/
function OLDGAME_GameResult(Result)
{
	if ((Result == "") || (Result == null) || (Result == undefined))
		return "";
	else if (Result == "white-timeover")
		return UTILS_GetText("game_result_white-timeover");
	else if (Result == "black-timeover")
		return UTILS_GetText("game_result_black-timeover");
	else if (Result == "white-mated")
		return UTILS_GetText("game_result_white-mated");
	else if (Result == "black-mated")
		return UTILS_GetText("game_result_black-mated");
	else if (Result == "stalemate")
		return UTILS_GetText("game_result_stalemate");
	else if (Result == "white-resigned")
		return UTILS_GetText("game_result_white-resigned");
	else if (Result == "black-resigned")
		return UTILS_GetText("game_result_black-resigned");
	else if (Result == "draw-agreement")
		return UTILS_GetText("game_result_draw-agreement");
	else if (Result == "draw-repetition")
		return UTILS_GetText("game_result_draw-repetition");
	else if (Result == "draw-fifty-moves")
		return UTILS_GetText("game_result_draw-fifty-moves");
	else if (Result == "draw-impossible-mate")
		return UTILS_GetText("game_result_draw-impossible-mate");
	else if (Result == "draw-timeover")
		return UTILS_GetText("game_result_draw-timeover");
	else if (Result == "canceled-agreement")
		return UTILS_GetText("game_result_canceled_agreement");
	else if (Result == "canceled-timed-out")
		return UTILS_GetText("game_result_canceled-timed-out");
	else if (Result == "adjourned-agreement")
		return UTILS_GetText("game_result_adjourned-agreement");
	else if (Result == "adjourned-shutdown")
		return UTILS_GetText("game_result_adjourned-shutdown");
	else if (Result == "white-wo")
		return UTILS_GetText("game_result_white-wo");
	else if (Result == "black-wo")
		return UTILS_GetText("game_result_black-wo");
	else
		return null;
}

