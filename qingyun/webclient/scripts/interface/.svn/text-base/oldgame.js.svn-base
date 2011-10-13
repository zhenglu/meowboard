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
*/

/*****************************
*	FUNCTIONS - WINDOW
******************************/

/**
*	Create elements of search old games window and returns div
*
* @return	Div; Array
* @see		WINDOW_OpeOldGameWindow();
* @author Danilo Kiyoshi Simizu Yorinori
*/
function INTERFACE_ShowOldGameWindow(Id)
{
	// Variables
	var Div;

	var FormDiv;

	var IdDiv;

	var Layer1Div;
	var L1LeftDiv;
	var Player1Label,Player1Input;
	var L1RightDiv;
	var PiecesLabel, WRadio, WImg, BRadio, BImg, RRadio, ALabel, RImg;

	var Layer2Div;
	var L2LeftDiv;
	var Player2Label, Br2, Player2Input;
	
	var L2RightDiv;
	var DateLabel, Br3, FromLabel, FromInput, ToLabel, ToInput;
	var FromDiv, ToDiv;
	var FormatLabel1,FormatLabel2;

	var ButtonsDiv;
	var Search, NewSearch;

	var ResultDiv;
	var SearchResultLabel;
	var TableDiv;
	var THead, TPlayer1Label, TPlayer2Label, TDateLabel, TResultLabel, TCategoryLabel, TExamine;
	var TBodyDiv,TBody, Table, TFoot; 
	var Hr, Prev, PageLabel, Next, Tr,Td;
	
	var NoResultDiv;
	var NoFound;

	var CloseDiv;
	var Close;

	var Buttons = new Array();
	var Elements = new Object();

	var Who;

	var LoadingBox;
	var NotFounded;

	// Main Div
	Div = UTILS_CreateElement('div','OldGamesDiv');

	// Id Div
	IdDiv = UTILS_CreateElement('div','OldGamesDivId'+Id);

	// Form Div
	FormDiv = UTILS_CreateElement('div','FormDiv');

	// Layer 1 Div
	Layer1Div= UTILS_CreateElement('div','Layer1Div');

	// Layer 1 Left Div
	L1LeftDiv= UTILS_CreateElement('div','L1LeftDiv','left_div');

	// Player 1 Form
	Player1Label= UTILS_CreateElement('p',null,null,UTILS_GetText('oldgame_player1'));
	Player1Input = UTILS_CreateElement('input','OldGameInput1');
	Player1Input.type = 'text';
	// End Player 1 Form
	// End Layer 1 Left Div

	// Layer 1 Right Div
	L1RightDiv= UTILS_CreateElement('div','L1RightDiv');

	// Pieces Form
	PiecesLabel = UTILS_CreateElement('p',null,null,UTILS_GetText('oldgame_pieces'));

	try
	//Fix radio button for IE
	{
		WRadio = document.createElement('<input class="radio" type="radio" name="color" value="white" />');
	}
	catch(err)
	{
		WRadio = UTILS_CreateElement('input',null,'radio');
		WRadio.type = "radio";
		WRadio.name = "color";
		WRadio.value = "white";
	}

	WImg = UTILS_CreateElement('img');
	WImg.src = "images/invite_white_pawn.png";
	
	try
	//Fix radio button for IE
	{
		BRadio = document.createElement('<input class="radio" type="radio" name="color" value="black" />');
	}
	catch(err)
	{
		BRadio = UTILS_CreateElement('input',null,'radio');
		BRadio.type = "radio";
		BRadio.name = "color";
		BRadio.value = "black";
	}

	BImg = UTILS_CreateElement('img');
	BImg.src = "images/invite_black_pawn.png";
	
	try
	//Fix radio button for IE
	{
		RRadio = document.createElement('<input class="radio" type="radio" name="color" value="both" checked="checked" />');
	}
	catch(err)
	{
		RRadio = UTILS_CreateElement('input',null,'radio');
		RRadio.type = "radio";
		RRadio.name = "color";
		RRadio.value = "both";
		RRadio.checked = true;
	}

	RImg = UTILS_CreateElement('img');
	RImg.src = "images/random.png";
	// End Pieces Form

	// Layer 2 Div
	Layer2Div= UTILS_CreateElement('div','Layer2Div');

	// Layer 2 Left Div
	L2LeftDiv= UTILS_CreateElement('div','L2LeftDiv','left_div');

	// Player 2 Form
	Player2Label= UTILS_CreateElement('p',null,null,UTILS_GetText('oldgame_player2'));
	Player2Input = UTILS_CreateElement('input');
	Player2Input.type = 'text';
	// End Player 2 Form
	// End Layer 2 Left Div

	// Layer 2 Right Div
	L2RightDiv= UTILS_CreateElement('div','L2RightDiv');

	// Date Form
	FromDiv = UTILS_CreateElement('div','FromDiv');
	ToDiv = UTILS_CreateElement('div','ToDiv');
	DateLabel = UTILS_CreateElement('p',null,null,UTILS_GetText('oldgame_date_form'));

	FromLabel	= UTILS_CreateElement('span',null,null,UTILS_GetText('oldgame_from'));
	FromInput	= UTILS_CreateElement('input');
	FromInput.size = "11";
	FromInput.maxLength = "10";
	FormatLabel1 = UTILS_CreateElement('span',null,'format',UTILS_GetText('oldgame_format'));

	ToLabel	= UTILS_CreateElement('span',null,null,UTILS_GetText('oldgame_to'));
	ToInput = UTILS_CreateElement('input');
	ToInput.size = "11";
	ToInput.maxLength = "10";
	FormatLabel2 = UTILS_CreateElement('span',null,'format',UTILS_GetText('oldgame_format'));

	Br2 = UTILS_CreateElement("br");
	Br3 = UTILS_CreateElement("br");
	// End Date Form
	// End Layer 2 Right Div
	// End Layer 2
	
	// Buttons Div
	ButtonsDiv = UTILS_CreateElement('div','ButtonsDiv');

	Search = UTILS_CreateElement('input', null, 'button');
	Search.type = "button";
	Search.value = UTILS_GetText("window_search");
	NewSearch	= UTILS_CreateElement('input',null,'button');
	NewSearch.type = "button";
	NewSearch.value = UTILS_GetText("oldgame_new_search");
	UTILS_AddListener(NewSearch,"click",	function() { Player1Input.value = ""; Player2Input.value = ""; RRadio.checked = true; FromInput.value = ""; ToInput.value = "" }, "false");
	// End ButtonsDiv;

	// Result Div
	ResultDiv = UTILS_CreateElement('div','ResultDiv');

	SearchResultLabel = UTILS_CreateElement('p',null,'result',UTILS_GetText("oldgame_search_result"));

	// Table Div
	TableDiv = UTILS_CreateElement('div','TableDiv');


	// TBody Div
	TBodyDiv = UTILS_CreateElement('div','TBodyDiv');
	
	// Table
	Table = UTILS_CreateElement('table');

	THead = UTILS_CreateElement('thead');
	Tr = UTILS_CreateElement('tr');
		TPlayer1Label = UTILS_CreateElement('td',null,'header',UTILS_GetText('oldgame_white_hd'));
		Tr.appendChild(TPlayer1Label);
		TPlayer2Label = UTILS_CreateElement('td',null,'header',UTILS_GetText('oldgame_black_hd'));
		Tr.appendChild(TPlayer2Label);
		TDateLabel =UTILS_CreateElement('td',null,'header',UTILS_GetText('oldgame_date'));
		Tr.appendChild(TDateLabel);
		TCategoryLabel = UTILS_CreateElement('td',null,'header',UTILS_GetText('oldgame_category'));
		Tr.appendChild(TCategoryLabel);
		TResultLabel = UTILS_CreateElement('td',null,'result_header',UTILS_GetText('oldgame_result'));
		Tr.appendChild(TResultLabel);
		TExamine = UTILS_CreateElement('td',null,'header_examine');
		Tr.appendChild(TExamine);
	THead.appendChild(Tr);
	Table.appendChild(THead);

	TBody = UTILS_CreateElement('tbody');
	Table.appendChild(TBody);

	TFoot = UTILS_CreateElement('tfoot');
	Tr = UTILS_CreateElement('tr');
	
	try
	{
			Td = document.createElement("<td class='tfoot_td' colspan='6' />");
	}
	catch(err)
	{
			Td = UTILS_CreateElement('td', null,'tfoot_td');
			Td.colSpan = 6;
	}

	Hr = UTILS_CreateElement('hr');
	Prev = UTILS_CreateElement('input', null, 'button');
	Prev.type = "button";
	Prev.value = UTILS_GetText("oldgame_prev");
	PageLabel = UTILS_CreateElement('span', null, 'page');
	Next	= UTILS_CreateElement('input',null,'button');
	Next.type = "button";
	Next.value = UTILS_GetText("oldgame_next");

	Td.appendChild(Hr);
	Td.appendChild(Prev);
	Td.appendChild(PageLabel);
	Td.appendChild(Next);

	Tr.appendChild(Td);
	TFoot.appendChild(Tr);
	Table.appendChild(TFoot);
	
	// End Table Div
	
	NoResultDiv = UTILS_CreateElement('div','NoResultDiv');
	NoFound = UTILS_CreateElement('p',null,null,UTILS_GetText("oldgame_no_result"));
	NoResultDiv.style.display = "none";

	// Close Button
	Close = UTILS_CreateElement('input', null, 'button');
	Close.type = "button";
	Close.value = UTILS_GetText("window_close");
	
	CloseDiv = UTILS_CreateElement('div','CloseDiv');

	// Loading Box
	LoadingBox = INTERFACE_CreateLoadingBox("oldgame_loading",UTILS_GetText("oldgame_loading"));
	LoadingBox.style.display = "block";

	// Mount Tree of Elements
	

	// Layer 1 Left Div
	L1LeftDiv.appendChild(Player1Label);
	L1LeftDiv.appendChild(Player1Input);

	// Layer 1 Right Div
	L1RightDiv.appendChild(PiecesLabel);
	L1RightDiv.appendChild(WRadio);
	L1RightDiv.appendChild(WImg);
	L1RightDiv.appendChild(BRadio);
	L1RightDiv.appendChild(BImg);
	L1RightDiv.appendChild(RRadio);
	L1RightDiv.appendChild(RImg);

	// Layer 1 Div
	Layer1Div.appendChild(L1LeftDiv);
	Layer1Div.appendChild(L1RightDiv);

	// Layer 2 Left Div
	L2LeftDiv.appendChild(Player2Label);
	L2LeftDiv.appendChild(Player2Input);

	// Layer 2 Right Div
	L2RightDiv.appendChild(DateLabel);

	FromDiv.appendChild(FromLabel);
	FromDiv.appendChild(FromInput);
	FromDiv.appendChild(Br2);
	FromDiv.appendChild(FormatLabel1);

	ToDiv.appendChild(ToLabel);
	ToDiv.appendChild(ToInput);
	ToDiv.appendChild(Br3);
	ToDiv.appendChild(FormatLabel2);

	L2RightDiv.appendChild(FromDiv);
	L2RightDiv.appendChild(ToDiv);

	// Layer 2 Div
	Layer2Div.appendChild(L2LeftDiv);
	Layer2Div.appendChild(L2RightDiv);

	// Form Div
	FormDiv.appendChild(Layer1Div);
	FormDiv.appendChild(Layer2Div);

	// Buttons Div
	ButtonsDiv.appendChild(Search);
	ButtonsDiv.appendChild(NewSearch);

	// TBody
	TBodyDiv.appendChild(Table);
	
	// Table Div
	TableDiv.appendChild(TBodyDiv);
	TableDiv.appendChild(LoadingBox);

	NoResultDiv.appendChild(NoFound);

	// ResultDiv
	ResultDiv.appendChild(SearchResultLabel);
	ResultDiv.appendChild(TableDiv);
	ResultDiv.appendChild(NoResultDiv);

	CloseDiv.appendChild(Close);

	// Main Div
	Div.appendChild(IdDiv);
	Div.appendChild(FormDiv);
	Div.appendChild(ButtonsDiv);
	Div.appendChild(ResultDiv);
	Div.appendChild(CloseDiv);

	Elements.Player1 = Player1Input;
	Elements.Player2 = Player2Input;
	Elements.Color = "";
	Elements.From = FromInput;
	Elements.To = ToInput;
	Elements.TBody = TBody;
	Elements.Table = Table
	Elements.TFoot = TFoot;
	Elements.Page = PageLabel;
	Elements.TableDiv = TableDiv;
	Elements.ResultDiv = ResultDiv;
	Elements.NoResultDiv = NoResultDiv;
	Elements.Prev = Prev;
	Elements.Next = Next;
	Elements.Search = Search;
	Elements.WRadio = WRadio;
	Elements.BRadio = BRadio;
	Elements.RRadio = RRadio;

	Elements.SetPlayer1 = INTERFACE_Player1Input;
	Elements.SetPlayer2 = INTERFACE_Player2Input;
	Elements.SetColor = INTERFACE_Color;
	Elements.SetFrom = INTERFACE_FromInput;
	Elements.SetTo = INTERFACE_ToInput;
	Elements.SetTable = INTERFACE_OldGameSetTable;
	Elements.SetResult = INTERFACE_OldGameSetResult;
	Elements.SetSearchButton = INTERFACE_SetSearchButton;
	Elements.SetPrevButton = INTERFACE_SetPrevButton;
	Elements.SetNextButton = INTERFACE_SetNextButton;
	
	Buttons.push(Close);

	return {Div:Div, Buttons:Buttons, Elements:Elements};
}

/**
*	Set result table of old games search
*
* @param	GameList	Array of old games
* @return	boolean
* @see		WINDOW_SetResult();
* @author Danilo Kiyoshi Simizu Yorinori
*/
function INTERFACE_AddOldGameResult(White, Black, Date, GameType, WinType,  Id)
{
	var Tr, Td1, Td2, Td3, Td4, Td5, Td6, Img, i;
	
	Tr = UTILS_CreateElement('tr');
		Td1 = UTILS_CreateElement('td',null,null,White);
		Tr.appendChild(Td1);
		Td2 = UTILS_CreateElement('td',null,null,Black);
		Tr.appendChild(Td2);
		Td3 = UTILS_CreateElement('td',null,null,Date);
		Tr.appendChild(Td3);
		Td4 = UTILS_CreateElement('td',null,null,GameType);
		Tr.appendChild(Td4);
		Td5 = UTILS_CreateElement('td',null,'resulttd',WinType);
		Tr.appendChild(Td5);
		Td6 = UTILS_CreateElement('td',null,'examine');

			Img = UTILS_CreateElement("img",null,'examine_icon');
			Img.src = "images/oldgame_examine.png";
			Img.title = UTILS_GetText("oldgame_examine");
			UTILS_AddListener(Img, "click", function()
					{
						if(MainData.CurrentGame == null)
						{
							CONNECTION_SendJabber(MESSAGE_FetchOldGame(Id));
						}
						else
						{
							WINDOW_Alert(UTILS_GetText("game_observer_alert_title"), UTILS_GetText("game_oldgame_alert"));
						}
					}, false);

			Td6.appendChild(Img);
		Tr.appendChild(Td6);
	
	Tr.onmouseover = function() { for (i=0; i<6; i++) this.childNodes[i].style.backgroundColor = "#DAF3F5"; }
	Tr.onmouseout = function() { for (i=0; i<6; i++) this.childNodes[i].style.backgroundColor = "#FFFFFF" }

	return(Tr);
}

/*
* Set the player 1 input
*
* @param User	String to be set
* @return void
* @author Danilo
*/
function INTERFACE_Player1Input(User) 
{
	this.Player1.value = User;
}

/*
* Set the player 2 input
*
* @param User	String to be set
* @return void
* @author Danilo
*/
function INTERFACE_Player2Input(User)
{
	this.Player2.value = User;
}

/*
* Check the appropriate color's checkbox
*
* @param Color Color's checkbox to be checked
* @return void
* @author Danilo
*/
function INTERFACE_Color(Color)
{
	if (Color == "white")
	{
		this.WRadio.checked = true;
		this.BRadio.checked = false;
		this.RRadio.checked = false;
	}
	else if (Color == "black")
	{
		this.WRadio.checked = false;
		this.BRadio.checked = true;
		this.RRadio.checked = false;
	}
	else if (Color == "")
	{
		this.WRadio.checked = false;
		this.BRadio.checked = false;
		this.RRadio.checked = true;
	}
}

/*
* Set the From input
*
* @param From	String to be set
* @return void
* @author Danilo
*/
function INTERFACE_FromInput(From)
{
	if (From == "")
	{
		this.From.value = "";
	}
	else
	{
		this.From.value = UTILS_ConvertTimeStamp(From);
	}
}

/*
* Set the To input
*
* @param To	String to be set
* @return void
* @author Danilo
*/
function INTERFACE_ToInput(To)
{
	if (To == "")
	{
		this.To.value = "";
	}
	else
	{
		this.To.value = UTILS_ConvertTimeStamp(To);
	}
}

/**
*	Display result of a old game search 
*
* @param	GameList	Array of old games
* @return	boolean
* @see		OLDGAME_HandleSearchOldGame();
* @author Danilo Kiyoshi Simizu Yorinori
*/
function INTERFACE_OldGameSetResult(Id, GameList, More)
{
	// if list of games is empty
	if (GameList.length == 0)
	{
		// undisplay result table
		this.TableDiv.style.display = "none";

		// show no found message
		this.NoResultDiv.style.display = "block";
	}
	// if list games contain some data
	else
	{
		// undisplay no result div
		this.NoResultDiv.style.display = "none";

		// display result table
		this.TableDiv.style.display = "block";

		//set result values in table
		this.SetTable(Id, GameList, More);
	}

	return true;
}

/**
*	Set result table of old games search
*
* @param	GameList	Array of old games
* @return	boolean
* @see		WINDOW_SetResult();
* @author Danilo Kiyoshi Simizu Yorinori
*/
function INTERFACE_OldGameSetTable(Id, GameList, More)
{
	var i=0;
	var SearchInfo = MainData.GetSearchGameInfo(Id);
	var GameLen = GameList.length;
	var Start = SearchInfo.Offset;
	var End = Start + GameLen; 
	var ParentDiv = UTILS_GetParentDiv(this.TBody);
	var P1Width = new Object();
	var P2Width = new Object();

	// Set the scroll bar to begin of result table
	ParentDiv.scrollTop = 0;

	// Remove old results
	while (this.TBody.childNodes.length != 0)
	{
		this.TBody.removeChild(this.TBody.childNodes[0]);
	}

	P1Width.Offset = this.Table.childNodes[0].rows[0].childNodes[0].offsetWidth;
	P1Width.Client = this.Table.childNodes[0].rows[0].childNodes[0].clientWidth;
	P2Width.Offset = this.Table.childNodes[0].rows[0].childNodes[1].offsetWidth;
	P2Width.Client = this.Table.childNodes[0].rows[0].childNodes[1].clientWidth;

	// Append new results
	for(i=0; i<GameLen ; i++)
	{
		this.TBody.appendChild(INTERFACE_AddOldGameResult(GameList[i].white, GameList[i].black, GameList[i].date, GameList[i].gametype,  GameList[i].result, GameList[i].id));

		// Break string if its break table result layout - White Player
		UTILS_BreakString(this.TBody.rows[i].childNodes[0],P1Width);
		// Break string if its break table result layout - Black Player
		UTILS_BreakString(this.TBody.rows[i].childNodes[1],P2Width);

		this.TBody.rows[i].childNodes[5].childNodes[0].onclick = function() { WINDOW_RemoveWindow(SearchInfo.Elements.WindowObj); OLDGAME_CloseWindow(Id); }
	}
	this.Page.innerHTML = Start+" - "+End;
	// Set buttons class
	if (SearchInfo.Offset == 0)
	{
		SearchInfo.Elements.Prev.className = "button_disabled";
	}
	else
	{
		SearchInfo.Elements.Prev.className = "button";
	}
	if (!More)
	{
		SearchInfo.Elements.Next.className = "button_disabled";
	}
	else
	{
		SearchInfo.Elements.Next.className = "button";
	}
	return true;
}

/**
*	Set function associated to previous button
*
* @param	Node	
* 				Oldgame object
* @void	
* @author Danilo Kiyoshi Simizu Yorinori
*/
function INTERFACE_SetPrevButton(Node)
{
	UTILS_AddListener(this.Prev,"click",
		function() { 
			if(Node.Offset != 0)
			{
				Node.Offset -= Node.NGames;
				CONNECTION_SendJabber(MESSAGE_GetOldGames(Node.Id,Node.P1, Node.P2,Node.NGames, Node.Offset, Node.Color, Node.To, Node.From)); 
			}
		}, "false");
}

/**
*	Set function associated to next button
*
* @param	Node	
* 				Oldgame object
* @void	
* @author Danilo Kiyoshi Simizu Yorinori
*/
function INTERFACE_SetNextButton(Node)
{
	UTILS_AddListener(this.Next,"click",
		function() { 
			if (Node.More)
			{
				Node.Offset += Node.NGames;
				CONNECTION_SendJabber(MESSAGE_GetOldGames(Node.Id,Node.P1, Node.P2,Node.NGames, Node.Offset, Node.Color, Node.To, Node.From)); 
			}
		}, "false");
}

/**
*	Set function associated to search button
*
* @param	Node	
* 				Oldgame object
* @void
* @author Danilo Kiyoshi Simizu Yorinori
*/
function INTERFACE_SetSearchButton(Node)
{
	UTILS_AddListener(this.Search,"click",	
		function() { 
			Node.P1 = Node.Elements.Player1.value;
			Node.P2 = Node.Elements.Player2.value;
			// Verify selected color
			if(Node.Elements.WRadio.checked == true)
				Node.Color = "white";
			else if(Node.Elements.BRadio.checked == true)
				Node.Color = "black";
			else
				Node.Color = "";
			// end selected color verification
			Node.From = UTILS_ConvertSearchDate(Node.Elements.From.value,"begin"); 
			if (Node.From == null)
			{
				WINDOW_Alert(UTILS_GetText("oldgame_invalid_date"),UTILS_GetText("oldgame_invalid_begin"));
				Node.From = "";
				return false;
			}
			Node.To = UTILS_ConvertSearchDate(Node.Elements.To.value,"end"); 
			if (Node.To == null) {
				WINDOW_Alert(UTILS_GetText("oldgame_invalid_date"),UTILS_GetText("oldgame_invalid_end"));
				Node.To = "";
				return false;
			}
			if (( Node.To != "") && (Node.From > Node.To))
			{
				WINDOW_Alert(UTILS_GetText("oldgame_invalid_date"),UTILS_GetText("oldgame_invalid_interval"));
				return false;
			}
			Node.Offset = 0;
			CONNECTION_SendJabber(MESSAGE_GetOldGames(Node.Id,Node.P1, Node.P2,Node.NGames, Node.Offset, Node.Color, Node.To, Node.From)); 

			// Show loading message
			INTERFACE_ShowOldgameLoading();

			return true;

		}, "false");
}

/**
* @author Rubens
*/
function INTERFACE_ShowOldgameLoading()
{
	var LoadingBox = document.getElementById("oldgame_loading");

	if(LoadingBox != null)
	{
		LoadingBox.style.display = "block";
	}
}

/**
* @author Rubens
*/
function INTERFACE_HideOldgameLoading()
{
	var LoadingBox = document.getElementById("oldgame_loading");

	if(LoadingBox != null)
	{
		LoadingBox.style.display = "none";
	}

}

