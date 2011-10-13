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
* This file will handle all kind of game massages
*/


/**
* Handle Game Messages
*
* @param 	XML The xml that contains the string 'match' in xmlns attribute
* @return 	Buffer with the messages that must be send
* @author 	Ulysses
*/
function GAME_HandleGame(XML)
{
	var Query = XML.getElementsByTagName("query");
	var Xmlns;
	var Buffer = "";

	// Getting query xmlns
	if (Query.length > 0)
	{
		Xmlns = Query[0].getAttribute("xmlns");
	}
	else 
	{
		return "";
	}

	if (Xmlns.match(/\/chessd#game#state/))
	{
		Buffer += GAME_State(XML);
	}
	else if (Xmlns.match(/\/chessd#game#move/))
	{
		Buffer += GAME_Move(XML);
	}
	else if (Xmlns.match(/\/chessd#game#canceled/))
	{
		Buffer += GAME_End(XML);
	}
	else if (Xmlns.match(/\/chessd#game#end/))
	{
		Buffer += GAME_End(XML);
	}
	else if (Xmlns.match(/\/chessd#game#cancel/))
	{
		Buffer += GAME_HandleCancel(XML, Xmlns);
	}
	else if (Xmlns.match(/\/chessd#game#draw/))
	{
		Buffer += GAME_HandleDraw (XML, Xmlns);
	}
	else if (Xmlns.match(/\/chessd#game#adjourn/))
	{
		Buffer += GAME_HandleAdjourn(XML, Xmlns);
	}
	
	return Buffer;
}

/**
 * Handle game presence
 */
function GAME_HandlePresence(XML)
{
	var Username = XML.getAttribute("from").split("/")[1];
	var GameId = XML.getAttribute("from").split("@")[0];
	var Buffer = "";
	var Type = XML.getAttribute("type");

	var Game = MainData.GetGame(GameId);

	if(Game == null)
	{
		return Buffer;
	}
	if(Type == "unavailable")
	{
		// If some player leave from game, show a W.O. message;
		if (Game.PB == Username)
		{
			Game.Game.ShowLeaveUser("black");
		}
		else if (Game.PW == Username)
		{
			Game.Game.ShowLeaveUser("white");
		}
	}
	else
	{
		// If some player reenter in game, hide a W.O. message;
		if ((Game.PB == Username) || (Game.PW == Username))
		{
			Game.Game.HideLeaveUser();
		}

	}
	return Buffer;
}

/**
* Handle Game Move
* It's a good ideia to read the server's documentation before reading the code above
*
* @param 	XML The xml that contains the game move
* @return 	void
* @author 	Ulysses
*/
function GAME_Move(XML)
{
	var StateTag, Category, GameID;
	var BoardTag, FullMoves, Enpassant, Castle, Halfmoves, Board, Turn;
	var PlayerTag, MoveTag, Move, ShortMove, Type;
	var Player1 = new Object();
	var Player2 = new Object();
	var Buffer = "";

	Type = XML.getAttribute('type');
	GameID = XML.getAttribute("from").replace(/@.*/,"");

	StateTag = XML.getElementsByTagName("state");
	BoardTag = XML.getElementsByTagName("board");
	PlayerTag = XML.getElementsByTagName("player");
	MoveTag = XML.getElementsByTagName("move");

	// Get game move history
	Category = StateTag[0].getAttribute("category");


	// Get the pgn of last move
	if(MoveTag != null)
	{
		Move = MoveTag[0].getAttribute("long");
		ShortMove = MoveTag[0].getAttribute("short");
	}
	else
	{
		Move = "------";
		ShortMove = "------";
	}

	FullMoves = BoardTag[0].getAttribute("fullmoves");
	Enpassant = BoardTag[0].getAttribute("enpassant");
	Castle = BoardTag[0].getAttribute("castle");
	Halfmoves = BoardTag[0].getAttribute("halfmoves");
	Board = BoardTag[0].getAttribute("state");
	Turn = BoardTag[0].getAttribute("turn");

	Player1.Name = PlayerTag[0].getAttribute('jid').replace(/@.*/,"");
	Player1.Inc = PlayerTag[0].getAttribute('inc');
	Player1.Color = PlayerTag[0].getAttribute('color');
	Player1.Time = PlayerTag[0].getAttribute('time');
	
	Player2.Name = PlayerTag[1].getAttribute('jid').replace(/@.*/,"");
	Player2.Inc = PlayerTag[1].getAttribute('inc');
	Player2.Color = PlayerTag[1].getAttribute('color');
	Player2.Time = PlayerTag[1].getAttribute('time');

	
	// If it's the first board of the game
	if (MainData.FindGame(GameID) == null)
	{
		Buffer += GAME_StartGame(GameID, Player1, Player2);

		Buffer += GAME_UpdateBoard(GameID, Board, Move, ShortMove, Player1, Player2, Turn)
	}
	else
	{
		Buffer += GAME_UpdateBoard(GameID, Board, Move, ShortMove, Player1, Player2, Turn)
	}

	return Buffer;
}

/**
* Handle Game IQ Result
*
* @param 	XML	The xml that contains the game result
* @return 	void
* @author 	Rubens Suguimoto
*/
function GAME_HandleGameResult(XML)
{
	var Id = XML.getAttribute("id");
	var Buffer = "";
	var Room;
	var GameTag;
	var i;
	var To;

	// The code above is used to parse games that player is playing;
	// 
	if(Id == MainData.Const.IQ_ID_SearchCurrentGame)
	{
		var GameTag = XML.getElementsByTagName("game");

		if(GameTag.length != 0)
		{
			for(i=0; i< GameTag.length; i++)
			{
				Room = GameTag[i].getAttribute("room");
				MainData.Status = "playing"
				To = Room+"@"+MainData.GameComponent+"."+MainData.Host+"/"+MainData.Username;
				Buffer += MESSAGE_Presence(To);
			}
		}
	}
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
function GAME_State(XML)
{
	var GameID, PlayerTag;
	var Player1 = new Object();
	var Player2 = new Object();
	var Buffer = "";
	var History;
	var HistoryStates;

	var SPlayer;
	var State;
	var WTime, BTime;


	GameID = XML.getAttribute("from").replace(/@.*/,"");
	PlayerTag = XML.getElementsByTagName("player");

	// Get game move history
	History = XML.getElementsByTagName("history")[0];
	HistoryStates = History.getElementsByTagName("state");

	// Get players time left
	State = XML.getElementsByTagName("state")[0];
	SPlayer = XML.getElementsByTagName("player");
	if(SPlayer != null)
	{
		if(SPlayer[0].getAttribute("color") == "white")
		{
			WTime = SPlayer[0].getAttribute("time");
			BTime = SPlayer[1].getAttribute("time");
		}
		else
		{
			WTime = SPlayer[1].getAttribute("time");
			BTime = SPlayer[0].getAttribute("time");
		}
	}

	Player1.Name = PlayerTag[0].getAttribute('jid').replace(/@.*/,"");
	Player1.Inc = PlayerTag[0].getAttribute('inc');
	Player1.Color = PlayerTag[0].getAttribute('color');
	Player1.Time = PlayerTag[0].getAttribute('time');
		
	Player2.Name = PlayerTag[1].getAttribute('jid').replace(/@.*/,"");
	Player2.Inc = PlayerTag[1].getAttribute('inc');
	Player2.Color = PlayerTag[1].getAttribute('color');
	Player2.Time = PlayerTag[1].getAttribute('time');

	// First Board State (without history tag)
	if(HistoryStates.length <= 0)
	{
		Buffer = GAME_Move(XML);
	}
	else
	{
		// History moves
		if (MainData.FindGame(GameID) == null)
		{
			Buffer += GAME_StartGame(GameID, Player1, Player2);
			Buffer += GAME_LoadGameHistory(GameID, History, Player1, Player2);
			// Set time left for each player
			Buffer += GAME_SetLeftTime(GameID, WTime, BTime)
		}
		else
		{
			Buffer += GAME_LoadGameHistory(GameID, History, Player1, Player2);
			// Set time left for each player
			Buffer += GAME_SetLeftTime(GameID, WTime, BTime)
		}
	}

	return Buffer;
}
/**
* Handle Draw Request
*
* @param 	XML The xml that contains the draw message
* @return 	The message to accept or deny the draw request
* @author 	Ulysses
*/
function GAME_HandleDraw(XML, Xmlns)
{
	var GameID = XML.getAttribute("from").replace(/@.*/,"");
	var Oponent = MainData.GetOponent(GameID);
	var Title = UTILS_GetText("game_draw");
	var Text;
	var Button1, Button2;

	// If receive a draw decline message
	if (Xmlns.match(/-decline/))
	{
		Text = UTILS_GetText("game_draw_denied");
		Text = Text.replace(/%s/, UTILS_Capitalize(Oponent));

		// Show message as a default alert window
		WINDOW_Alert(Title, Text);
	}
	// User send a draw request
	else
	{
		Text = UTILS_GetText("game_draw_text");
		Text = Text.replace(/%s/, UTILS_Capitalize(Oponent));
		Button1 = new Object();
		Button2 = new Object();

		// Ok button
		Button1.Name = UTILS_GetText("window_accept");
		Button1.Func = function () {
			CONNECTION_SendJabber(MESSAGE_GameDrawAccept(GameID));
		}

		// Cancel button
		Button2.Name = UTILS_GetText("window_decline");
		Button2.Func = function () {
			CONNECTION_SendJabber(MESSAGE_GameDrawDeny(GameID));
		}

		// Show message as a default confirm window
		WINDOW_Confirm(Title, Text, Button1, Button2);
	}
	return "";
}

/**
* Handle Cancel Request
*
* @param 	XML The xml that contains the cancel message
* @return 	The XML to accept or deny the cancel request
* @author 	Ulysses
*/
function GAME_HandleCancel(XML, Xmlns)
{
	var GameID = XML.getAttribute("from").replace(/@.*/,"");
	var Oponent = MainData.GetOponent(GameID);
	var Title = UTILS_GetText("game_abort");
	var Text;
	var Button1, Button2;

	// If receive a draw decline message
	if (Xmlns.match(/-decline/))
	{
		Text = UTILS_GetText("game_abort_denied");
		Text = Text.replace(/%s/, UTILS_Capitalize(Oponent));

		// Show message as a default alert window
		WINDOW_Alert(Title, Text);
	}
	// User send a draw request
	else
	{
		Text = UTILS_GetText("game_abort_text");
		Text = Text.replace(/%s/, UTILS_Capitalize(Oponent));
		Button1 = new Object();
		Button2 = new Object();

		// Ok button
		Button1.Name = UTILS_GetText("window_accept");
		Button1.Func = function () {
			CONNECTION_SendJabber(MESSAGE_GameCancelAccept(GameID));
		}

		// Cancel button
		Button2.Name = UTILS_GetText("window_decline");
		Button2.Func = function () {
			CONNECTION_SendJabber(MESSAGE_GameCancelDeny(GameID));
		}

		// Show message as a default confirm window
		WINDOW_Confirm(Title, Text, Button1, Button2);
	}
	return "";
}

/**
* Handle Adjourn Request
*
* @param 	XML The xml that contains the adjourn message
* @return 	The XML to accept or deny the adjourn request
* @author 	Ulysses
*/
function GAME_HandleAdjourn(XML, Xmlns)
{
	var GameID = XML.getAttribute("from").replace(/@.*/,"");
	var Oponent = MainData.GetOponent(GameID);
	var Title = UTILS_GetText("game_adjourn");
	var Text;
	var Button1, Button2;

	// If receive a draw decline message
	if (Xmlns.match(/-decline/))
	{
		Text = UTILS_GetText("game_adjourn_denied");
		Text = Text.replace(/%s/, UTILS_Capitalize(Oponent));

		// Show message as a default alert window
		WINDOW_Alert(Title, Text);
	}
	// User send a draw request
	else
	{
		Text = UTILS_GetText("game_adjourn_text");
		Text = Text.replace(/%s/, UTILS_Capitalize(Oponent));
		Button1 = new Object();
		Button2 = new Object();

		// Ok button
		Button1.Name = UTILS_GetText("window_accept");
		Button1.Func = function () {
			CONNECTION_SendJabber(MESSAGE_GameAdjournAccept(GameID));
		}

		// Cancel button
		Button2.Name = UTILS_GetText("window_decline");
		Button2.Func = function () {
			CONNECTION_SendJabber(MESSAGE_GameAdjournDeny(GameID));
		}

		// Show message as a default confirm window
		WINDOW_Confirm(Title, Text, Button1, Button2);
	}
	return "";
}


/**
* Game Over
*
* @param 	XML The xml that contains the ending game message
* @return 	void
* @author 	Ulysses
*/
function GAME_End(XML)
{
	var PlayerTag, EndTag, Result;
	var Game, GameID, Reason, Player, Winner;
	var Title = UTILS_GetText("game_end_game");
	var Playing;
	var Text;

	// Get the room name
	GameID = XML.getAttribute("from").replace(/@.*/,"");

	Game = MainData.GetGame(GameID);
	Game.Game.StopTimer();

	// Hide loading box
	Game.Game.HideLoadingMove();
	// Hide leave user box
	Game.Game.HideLeaveUser();

	// Finish game in structure
	if (Game)
	{
		Game.Finished = true;
	}

	// If this end game message is from a current game then
	// show message
	if(MainData.CurrentGame.Id == GameID)
	{
		// Get the reason 
		EndTag = XML.getElementsByTagName("end");
		if (EndTag.length > 0)
		{
			// Get the reason from tag 'reason'
			Result = EndTag[0].getAttribute("result");
		}
		else
		{
			Result = "canceled";
		}
		
		// Show end game message to user
		WINDOW_Alert(Title, UTILS_GetText("game_result_"+Result));
	}

	if ((Game.PB != MainData.Username) && (Game.PW != MainData.Username))
	{
		Playing = false;
	}
	else
	{
		Playing = true;
	}

	OLDGAME_EndGame(GameID);

	// Set status avaialable for players
	if (Playing)
	{
		return CONTACT_ChangeStatus("available", "return");
	}
	// and do nothing for observers
	else
	{
		return "";
	}
}


/**
* Handle Game Error
*
* @param 	XML The xml that contains the error message
* @param	GameID - Id of the game that receives the message
* @return 	void
* @author 	Pedro
*/
function GAME_HandleGameError(XML)
{
	var Query = XML.getElementsByTagName("query");
	var Xmlns;
	var Buffer = "";
	var Game, GameID, Move;
	var Invalid, Over;

	// Getting query xmlns
	if (Query.length > 0)
	{
		Xmlns = Query[0].getAttribute("xmlns");
	}
	else 
	{
		return "";
	}

	// Getting game id
	GameID = XML.getAttribute("from").replace(/@.*/,"");

	if (Xmlns.match(/\/chessd#game#move/))
	{
		Invalid = XML.getElementsByTagName('invalid-move');
		Over = XML.getElementsByTagName('game-over');

		// If it's a invalid move
		if (Invalid.length > 0)
		{
			// Get game from GameList
			Game = MainData.GetGame(GameID);

			// Undo last move
			Game.Game.UndoMove();

			// Remove loading move message
			GAME_HideLoadingMove(GameID);
		}
		// If game is over
		else if (Over.length > 0)
		{
			return "";
		}
		// This case should happen when game is over in server
		// but not in interface
		else
		{
			WINDOW_Alert(UTILS_GetText("game_end_game"),UTILS_GetText("game_result_closed"));
			OLDGAME_EndGame(GameID);
		}
	}
	return "";
}

/**
* Start Game
*
* @param 	GameId = Game number
* @param 	P1 = Player 1 Object (Name, Time, Color, Inc)
* @param 	P2 = Player 2 Object (Name, Time, Color, Inc)
* @return 	void
* @author 	Rubens
*/
function GAME_StartGame(GameId, P1, P2)
{
	var GameDiv;
	var YourColor;
	var Buffer="";
	var RoomPos; 

	// Remove welcome div
	INTERFACE_RemoveWelcome();

	// Hide current game (this case should happen when player
	// is observing a game)
	if (MainData.CurrentGame != null)
	{

		//Quickfix to leave room when observer
		RoomPos = MainData.FindRoom(MainData.CurrentGame.Id)
		Buffer  += MESSAGE_Unavailable(MainData.RoomList[RoomPos].MsgTo);
		MainData.CurrentGame.Game.Hide();
		MainData.RemoveGame(MainData.CurrentGame.Id);
	}

	if (MainData.CurrentOldGame != null)
	{
		//Quickfix to leave room when observer
		RoomPos = MainData.FindRoom(MainData.CurrentOldGame.Id)
		if(RoomPos != null)
		{
			Buffer  += MESSAGE_Unavailable(MainData.RoomList[RoomPos].MsgTo);
		}

		MainData.CurrentOldGame.Game.Hide();
		// In this version, player can see only one oldgame
		//MainData.RemoveOldGame(MainData.CurrentOldGame.Id);
		MainData.RemoveOldGame(0);

	}

	if (P1.Name == MainData.Username)
	{
		YourColor = P1.Color;
	}
	else
	{
		YourColor = P2.Color;
	}
	// 38 -> default piece size
	GameDiv = new INTERFACE_GameBoardObj(GameId, P1, P2, YourColor);

	// Add game to data struct and set it to current game
	MainData.AddGame(GameId, P1.Name, P2.Name, YourColor, GameDiv);

	// Show New Game
	GameDiv.Show();

	// Get Players Photo
	Buffer += MESSAGE_GetProfile(P1.Name,MainData.Const.IQ_ID_GamePhoto);
	Buffer += MESSAGE_GetProfile(P2.Name,MainData.Const.IQ_ID_GamePhoto);

	// Set status to playing
	return CONTACT_ChangeStatus("playing", "return") + Buffer;

}

/**
* Start Game in Observer Mode
*
* @param 	GameId = Game number
* @param 	PWName is player white name
* @param 	PBName is player black name
* @return 	void
* @author 	Rubens
*/
function GAME_StartObserverGame(GameId, P1, P2)
{
	var GameDiv;
	var RoomPos;
	var Buffer = "";

	// Remove welcome div
	INTERFACE_RemoveWelcome();

	// Hide current game
	if (MainData.CurrentGame != null)
	{
		// In this version, player should be able to
		// observer just one game.
		GAME_RemoveGame(MainData.CurrentGame.Id);
	}

	if (MainData.CurrentOldGame != null)
	{
		OLDGAME_RemoveOldGame(0);
	}

	// 38 -> default piece size
	GameDiv = new INTERFACE_GameBoardObj(GameId, P1, P2, "white", 38);
	MainData.AddGame(GameId, P1.Name, P2.Name, "none", GameDiv);

	MainData.CurrentGame.Finished = true;

	// Show New Game
	GameDiv.Show();
	// Set Observer Mode
	GameDiv.ObserverMode();

	//Set Timers
	GameDiv.UpdateWTime(0);
	GameDiv.UpdateBTime(0);
	GameDiv.SetWTime();
	GameDiv.SetBTime();

	// Get players Photos
	Buffer += MESSAGE_GetProfile(P1.Name,MainData.Const.IQ_ID_GamePhoto);
	Buffer += MESSAGE_GetProfile(P2.Name,MainData.Const.IQ_ID_GamePhoto);

	// Send message to leave from old game observer room;
	CONNECTION_SendJabber(Buffer);

	// Send a message to get game moves
	ROOM_EnterRoomGame(GameId)
}

/**
* Update board in data struct and interface
*
* @param 	GameId = Game number
* @param 	BoardStr = Board status in a string
* @param 	Move = Chess Move (Piece/Orig-Dest)
* @param 	P1 = Player 1 Object (Name, Time, Color, Inc)
* @param 	P2 = Player 2 Object (Name, Time, Color, Inc)
* @param 	TurnColor = color ("white"/"black")
* @return 	void
* @author 	Rubens
*/
function GAME_UpdateBoard(GameId, BoardStr, Move, ShortMove, P1, P2, TurnColor)
{
	var NewBoardArray = UTILS_String2Board(BoardStr);
	var CurrentBoardArray;
	var Game;

	// Get game from GameList
	Game = MainData.GetGame(GameId);
	
	if (Game.CurrentMove != null)
	{
		CurrentBoardArray = Game.Moves[Game.CurrentMove].Board;
	}
	// if Move is first move in game
	else
	{
		CurrentBoardArray = new Array("---------","---------","---------","---------","---------","---------","---------","---------","---------","---------");
	}

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

	// Update turn in structure and interface
	Game.SetTurn(TurnColor);
	Game.Game.SetTurn(TurnColor);
	
	// Show new time
	Game.Game.SetWTime();
	Game.Game.SetBTime();

	// Update interface
	Game.Game.UpdateBoard(CurrentBoardArray, NewBoardArray, Game.YourColor);

	if((Move != "") || (ShortMove!= ""))
	{
		Game.Game.AddMove(Game.Moves.length, Move, ShortMove, P1.Time, P2.Time);
		Game.Game.SetLastMove(Move, Game.YourColor);
	}

	if (Game.Moves.length == 3)
	{
		Game.Game.StartTimer();
	}

	// Hide loading div
	Game.Game.HideLoadingMove();

	return "";
}

/**
* Remove a game from data struct and interface
* @param 	GameId is the game identificator
* @return 	void
* @author 	Rubens and Pedro
*/
function GAME_RemoveGame(GameID)
{
	var Game = MainData.GetGame(GameID);

	if (Game != null)
	{
		if (Game.Finished)
		{
			// Remove game board from interface
			Game.Game.StopTimer();
			Game.Game.Remove();
			
			// Remove board from data struct
			MainData.RemoveGame(GameID);

			// Send message to leave room
			ROOM_ExitRoom(GameID)
		}
		else
		{
			WINDOW_Alert(UTILS_GetText("game_remove_game_title"), UTILS_GetText("game_remove_game"));
		}
	}
}

/**
* Send a movement to server
*
* @param 	OldLine is line of piece origin position
* @param 	OldCol is column of piece origin position
* @param 	NewLine is line of piece dest position
* @param 	NewCol is line of piece dest position
* @return 	void
* @author	Pedro
*/
function GAME_SendMove(OldLine, OldCol, NewLine, NewCol)
{
	var GameID = MainData.CurrentGame.Id;
	var Move;
	var CurrentMove = MainData.CurrentGame.CurrentMove;
	var CurrentBoard = MainData.CurrentGame.Moves[CurrentMove].Board;
	var Promotion = "";

	// Create long notation
	if (MainData.CurrentGame.YourColor == "white")
	{
		Move = UTILS_HorizontalIndex(OldCol)+OldLine+UTILS_HorizontalIndex(NewCol)+NewLine;

	}
	else
	{
		Move = UTILS_HorizontalIndex(10-OldCol)+(9-OldLine)+UTILS_HorizontalIndex(10-NewCol)+(9-NewLine);

	}

	// Check for pawn promotion.
/*FIXME: not need promotion for xq*/
//	if(NewLine == 8)
//	{
//		if(MainData.CurrentGame.YourColor == "white")
//		{
//			if(CurrentBoard[8-OldLine][OldCol-1] == "P")
//			{
//				Promotion = MainData.CurrentGame.Promotion;
//			}

//		alert(CurrentBoard +"\n"+ (OldLine)+(OldCol)+(NewLine)+(NewCol) +"\n"+ CurrentBoard[8-OldLine][OldCol-1] +"\n"+ Promotion +"\n"+CurrentBoard[8-OldLine])

//		}
//		else
//		{
//			if(CurrentBoard[OldLine-1][8-OldCol] == "p")
//			{
//				Promotion = MainData.CurrentGame.Promotion;
//			}

//		alert(CurrentBoard +"\n"+ (OldLine)+(OldCol)+(NewLine)+(NewCol) +"\n"+ CurrentBoard[OldLine-1][8-OldCol] +"\n"+Promotion+"\n"+CurrentBoard[OldLine-1])
//		}
//	}

	// Send move for the current game
	CONNECTION_SendJabber(MESSAGE_GameMove(Move, MainData.CurrentGame.Id, Promotion));
}

/**
* Send a draw message to oponent
*
* @param 	GameId is the game identificator
* @return 	void
* @author	Pedro
*/
function GAME_SendDraw(GameID)
{
	CONNECTION_SendJabber(MESSAGE_GameRequestDraw(GameID));
}

/**
* Send a adjourn message to oponent
*
* @param 	GameId is the game identificator
* @return 	void
* @author	Pedro
*/
function GAME_SendCancel(GameID)
{
	CONNECTION_SendJabber(MESSAGE_GameRequestCancel(GameID));
}

/**
* Send a adjourn message to oponent
*
* @param 	GameId is the game identificator
* @return 	void
* @author	Pedro
*/
function GAME_SendAdjourn(GameID)
{
	CONNECTION_SendJabber(MESSAGE_GameRequestAdjourn(GameID));
}

function GAME_SearchCurrentGame()
{
	CONNECTION_SendJabber(MESSAGE_GameSearchCurrentGame());
}

/**
* Change current game piece promotion
*
* @param 	Piece	Piece char
* @return 	void
* @author	Rubens Suguimoto
*/
function GAME_ChangePromotion(Piece)
{
	MainData.CurrentGame.Promotion = Piece;
}

/**
* Send a resign message to oponent
*
* @param 	GameId is the game identificator
* @return 	void
* @author	Pedro
*/
function GAME_SendResign(GameID)
{
	var Title = UTILS_GetText("game_resign");
	var Text;
	var Button1, Button2;

	Text = UTILS_GetText("game_resign_confirm");
	Button1 = new Object();
	Button2 = new Object();

	// Ok button
	Button1.Name = UTILS_GetText("window_ok");
	Button1.Func = function () {
		CONNECTION_SendJabber(MESSAGE_GameResign(GameID));
	}

	// Cancel button
	Button2.Name = UTILS_GetText("window_cancel");
	
	// Show message as a default confirm window
	WINDOW_Confirm(Title, Text, Button1, Button2);
}

/**
* Load all game history moves done in the game
*
* @param 	GameId is the game identificator
* @param 	HistoryXml is a XML that contains all games states
* @param 	Player1 = Player 1 Object (Name, Time, Color, Inc)
* @param 	Player2 = Player 2 Object (Name, Time, Color, Inc)
* @return 	void
* @author	Rubens
*/
function GAME_LoadGameHistory(GameID, HistoryXml, Player1, Player2)
{
	var i;
	var StartP1Time, StartP2Time, HTurn, HTime, HBoard, HMove, HShort, HShortMove;
	var HPlayer1 = new Object();
	var HPlayer2 = new Object();
	var HistoryMoves;
	var Buffer = "";

	if(HistoryXml == undefined)
	{
		return Buffer;
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

		Buffer += GAME_UpdateBoard(GameID, HBoard, HMove, HShortMove, HPlayer1, HPlayer2, HTurn)
	}

	return Buffer;
}

/**
* Handle Game Players Photo
*
* @param 	XML The xml that contains vCard photo
* @return 	none
* @author 	Rubens
*/
function GAME_HandleVCardPhoto(XML)
{
	var Photo;
	var Player;
	var Binval;
	var PhotoType;
	var Img;

	// If there is no game opened, do nothing;
	if( MainData.CurrentGame == null)
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

	// Update current game player image
	// Player White
	if(MainData.CurrentGame.PW == Player)
	{
		MainData.CurrentGame.WPhoto = Img;
		MainData.CurrentGame.Game.SetWPhoto(Img);
	}
	// Player Black
	else if(MainData.CurrentGame.PB == Player)
	{
		MainData.CurrentGame.BPhoto = Img;
		MainData.CurrentGame.Game.SetBPhoto(Img);
	}
	/*
	else
	{
		//This case should not happen
	}
	*/
	return "";
}

function GAME_SetLeftTime(GameID, WTime, BTime)
{
	var Game = MainData.GetGame(GameID);

	if(Game != null)
	{
		Game.Game.UpdateWTime(WTime);
		Game.Game.UpdateBTime(BTime);

		Game.Game.SetWTime();
		Game.Game.SetBTime();
	}
}

function GAME_ShowLoadingMove(Id)
{
	var Game = MainData.GetGame(Id)

	Game.Game.ShowLoadingMove();
}

function GAME_HideLoadingMove(Id)
{
	var Game = MainData.GetGame(Id)

	Game.Game.HideLoadingMove();
}
// set the border of the block
// Line ranges from [0,9]
// Col ranges from [1,9]
function GAME_SetBlockBorder(Line, Col)
{
	var Game = MainData.CurrentGame;
	var BlockId;

	// Create long notation
	if (MainData.CurrentGame.YourColor == "white")
	{
		BlockId = UTILS_HorizontalIndex(Col)+Line;
	}
	else
	{
		BlockId = UTILS_HorizontalIndex(10-Col)+(9-Line);
	}

	Game.Game.SetBlockBorder(BlockId);
}

function GAME_RemoveBlockBorder(Line, Col)
{
	var Game = MainData.CurrentGame;
	var BlockId;

	// Create long notation
	if (MainData.CurrentGame.YourColor == "white")
	{
		BlockId = UTILS_HorizontalIndex(Col)+Line;
	}
	else
	{
		BlockId = UTILS_HorizontalIndex(10-Col)+(9-Line);
	}

	Game.Game.RemoveBlockBorder(BlockId);
}
