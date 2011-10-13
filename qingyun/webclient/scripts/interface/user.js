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
* @file user.js
* @brief Class defition for user list object and some functions assign
*
* Create a user list object used in rooms, contact list, online list, etc.
*/

/**
* @class UserListObj
* @brief User list object that contais pointer to HTML elements, some methods to add/remove/update user, show/hide list  and show/hide sort options;
* @see INTERFACE_CreateUserList
* @param Element parent node, where user list object will be append
* Class to organize the use of user lists;
*/
function UserListObj(Element)
{
	// Create HTML elements to user list
	var UserList = INTERFACE_CreateUserList(Element);
	
	// Attributes
	this.mainDiv = UserList.MainDiv; // div with sorts and userlist
	this.userList = UserList.UserList; //user list tbody
	this.sortNick = UserList.SortNick; //span element
	this.sortRating = UserList.SortRating; // option element

	//User list with pointer to user list item tr element
	//used to control users in list on interface
	this.users = new Array();

	// Methods
	this.show = INTERFACE_ShowUserList; // show main div
	this.hide = INTERFACE_HideUserList; // hide main div

	this.addUser = INTERFACE_AddUser; // add a user in list
	this.removeUser = INTERFACE_RemoveUser; // remove user from list
	this.updateUser = INTERFACE_UpdateUser; // update user status/rating
	this.findUser = INTERFACE_FindUser; // find user in list

	this.hideSort = INTERFACE_HideSort; // hide sort elements
	this.showSort = INTERFACE_ShowSort; // show sort elements

	this.focusNick = INTERFACE_FocusNick; // focus span element
	this.blurNick = INTERFACE_BlurNick; // blur focus span element

	this.focusRating = INTERFACE_FocusRating; // focus rating option
	this.blurRating = INTERFACE_BlurRating; // blur rating option

	this.hideList = INTERFACE_HideList; // hide users list
	this.showList = INTERFACE_ShowList; // show users list

	// set sort list function when click in (sortNick) span element
	this.setSortUserFunction = INTERFACE_SetSortUserFunction;
	// set sort list function when click in (sortRating) option element
	this.setSortRatingFunction = INTERFACE_SetSortRatingFunction;
}


/**
 * @brief	Show user list div
 * 
 * Show user list div changing element style display to block.
 *
 * @author	Rubens Suguimoto
 */
function INTERFACE_ShowUserList()
{
	this.mainDiv.style.display = "block";
}

/**
 * @brief	Hide user list div
 * 
 * Hide user list div changing element style display to none.
 *
 * @author	Rubens Suguimoto
 */
function INTERFACE_HideUserList()
{
	this.mainDiv.style.display = "none";
}


/**
 * @brief	Add one user in list
 * 
 * Add a user in user list creating a tr element and insert it in (this.users) array.
 *
 * @param 	Username The user's name 
 * @param 	Status	The user's status
 * @param 	Rating	User's rating that will be show
 * @param 	Type	User's type (teacher, admin, robot, etc.)
 * @author	Rubens Suguimoto
 * @see		INTERFACE_CreateUser
 */
function INTERFACE_AddUser(Username, Status, Rating, Type)
{
	var User;

	// User item that will be inserted in array (this.users)
	var UserObj = new Object();

	// Create tr element
	User = INTERFACE_CreateUser(Username, Status, Rating, Type, this.roomName);

	// Add user in users struct
	UserObj.Id = Username;
	UserObj.User = User;
	this.users.push(UserObj);

	//this.userList.insertBefore(User,null);
	this.userList.appendChild(User);
}

/**
 * @brief	Remove user from list
 * 
 * Remove a user from user list and remove from array. 
 *
 * @param 	Username The user's name 
 * 
 * @author	Rubens Suguimoto
 * @return	false - User is not founded in list, true otherwise
 */
function INTERFACE_RemoveUser(Username)
{
	var UserItem = this.findUser(Username);
	var i=0;


	// If user is not founded in user list, return false
	if(UserItem == null)
	{
		return false;
	}

	// remove from interface
	this.userList.removeChild(UserItem);


	// Find user in "users" list and remove from it
	while((Username != this.users[i].Id) && (i<this.users.length))
	{
		i++;
	}
	if(i< this.users.length)
	{
		//remove from array
		this.users.splice(i,1);
	}

	return true;
}

/**
 * @brief	Update user status and rating in list
 * 
 * Update user status and rating in user list. Find the user and set his type status and, if exists rating, update rating;
 *
 * @param 	Username The user's name
 * @param	Newstatus The new user's status
 * @param	Rating The user's rating
 * @param	NewType The new user's type
 * 
 * @author	Rubens Suguimoto
 * @return	false - User is not founded in list, true otherwise
 */
function INTERFACE_UpdateUser(Username, NewStatus, Rating, NewType)
{
	var Node = this.findUser(Username);
	var User, UserRating;

	if(Node == null)
	{
		return false;
	}
	
	// Get user and status icon element
	User = Node.getElementsByTagName("td")[0];
	UserRating = Node.getElementsByTagName("td")[1];

	// If 'NewType' is not passed, set normal user status
	if (NewType == null)
	{
		User.className = User.className.replace(/_.*/, "_"+NewStatus);
	}
	else
	{
//		if (NewType == "")
//			NewType="user";
		User.className = NewType+"_"+NewStatus;
	}
	
	if(Rating != null)
	{
		UserRating.innerHTML = Rating;
	}
	
	return true;
}

/**
 * @brief	Find user in user list
 * 
 * Search user in array (this.users);
 *
 * @param 	Username	The user's name
 * 
 * @author	Rubens Suguimoto
 * @return	null if user is not founded in list, else tr elemente if user item;
 */
function INTERFACE_FindUser(Username)
{
	var i=0;

	while((i<this.users.length) && (Username != this.users[i].Id))
	{
		i++;
	}

	if(i>= this.users.length)
	{
		return null;    
	}
	else
	{
		return this.users[i].User;
	}
}


/**
 * @brief	Set sort list by nick function to list
 * 
 * Set sort by nick function when click in sort span
 *
 * @param 	Func 	Function that contais sort instructions
 * 
 * @author	Rubens Suguimoto
 */
function INTERFACE_SetSortUserFunction(Func)
{
	UTILS_AddListener(this.sortNick , "click", Func, false);
}


/**
 * @brief	Set sort list by rating function to list
 * 
 * Set sort by rating function when click in sort option
 *
 * @param 	Func 	Function that contais sort instructions
 * 
 * @author	Rubens Suguimoto
 */
function INTERFACE_SetSortRatingFunction(Func)
{
	var TmpFunc = Func;

	// Get category to sort
	UTILS_AddListener(this.sortRating, "change", function(){ TmpFunc(this.value)}, false);
}

/**
 * @brief	Hide sort options
 * 
 * Hide sort span and option
 *
 * @author	Rubens Suguimoto
 */
function INTERFACE_HideSort()
{
	this.sortNick.style.display = "none";
	this.sortRating.style.display = "none";
}

/**
 * @brief	Show sort options
 * 
 * Show sort span and option
 *
 * @author	Rubens Suguimoto
 */
function INTERFACE_ShowSort()
{
	this.sortNick.style.display = "block";
	this.sortRating.style.display = "block";
}

/**
 * @brief	Hide user list element
 * 
 * Hide user list in interface
 *
 * @author	Rubens Suguimoto
 */
function INTERFACE_HideList()
{
	this.userList.parentNode.style.display = "none";
}

/**
 * @brief	Show user list element
 * 
 * Show user list in interface
 *
 * @author	Rubens Suguimoto
 */
function INTERFACE_ShowList()
{
	this.userList.parentNode.style.display = "block";
}

// focus span element
function INTERFACE_FocusNick()
{
	this.sortNick.className = "selected";
}

// blur focus span element
function INTERFACE_BlurNick()
{
	this.sortNick.className = "";
}

// focus rating option
function INTERFACE_FocusRating()
{
	this.sortRating.className = "selected";
}

// blur rating option
function INTERFACE_BlurRating()
{
	this.sortRating.className = "";
}


/**************************************
**** FUNCTION - CREATE HTML LIST
**************************************/

/**
 * @brief	Create user list in interface
 * 
 * Create user list HTML, and return div, list parent, sort span, sort rating option;
 *
 * @param	Element 	Parent node of list main div
 *
 * @author	Rubens Suguimoto
 * @return	Object that contains list main div, user list, sort span and sort rating option
 */
function INTERFACE_CreateUserList(Element)
{
	var MainDiv;
	var Users, Table, Tbody;
	var OrderNick, OrderRating, OrderRatingOpt;
	
	MainDiv = UTILS_CreateElement("div",null,"UserList");

	OrderNick = UTILS_CreateElement("span", "order_nick", "selected", UTILS_GetText("room_order_nick"));
	OrderRating = UTILS_CreateElement("select", "order_rating", "", UTILS_GetText("room_order_rating"));
	OrderRatingOpt = UTILS_CreateElement("option", null, null, UTILS_GetText("contact_order_rating")+" ("+UTILS_GetText("game_type_lightning")+")");
	OrderRatingOpt.value = "lightning";
	OrderRating.appendChild(OrderRatingOpt);
	OrderRatingOpt = UTILS_CreateElement("option", null, null, UTILS_GetText("contact_order_rating")+" ("+UTILS_GetText("game_type_blitz")+")");
	OrderRatingOpt.value = "blitz";
	OrderRating.appendChild(OrderRatingOpt);

	OrderRatingOpt = UTILS_CreateElement("option", null, 'selected', UTILS_GetText("contact_order_rating")+" ("+UTILS_GetText("game_type_standard")+")");
	OrderRatingOpt.selected = true;
	OrderRatingOpt.value = "standard";
	OrderRating.appendChild(OrderRatingOpt);

	OrderRatingOpt = UTILS_CreateElement("option", null, null, UTILS_GetText("contact_order_rating")+" ("+UTILS_GetText("game_type_untimed")+")");
	OrderRatingOpt.value = "untimed";
	OrderRating.appendChild(OrderRatingOpt);


	OrderNick.onclick = function(){
		OrderNick.className = "selected";
		OrderRating.className = "";
	};

	OrderRating.onchange = function(){
		var Options;
		var i;
		var Select;
		
		OrderNick.className = "";
		OrderRating.className = "selected";
	
		Options = OrderRating.options;
		Select = OrderRating.selectedIndex;
		for(i=0; i< Options.length; i++)
		{
			if (i != Select)
			{
				Options[i].className = "not_selected";
			}
			else
			{
				Options[i].className = "selected";
			}
		}
	};

	// User list
	Users = UTILS_CreateElement("div",null,"UserTable");
	Table = UTILS_CreateElement("table");
	Tbody = UTILS_CreateElement("tbody");

	Table.appendChild(Tbody);
	Users.appendChild(Table);

	MainDiv.appendChild(OrderNick);
	MainDiv.appendChild(OrderRating);
	MainDiv.appendChild(Users);

	Element.insertBefore(MainDiv, Element.firstChild);

	return {MainDiv:MainDiv, UserList:Tbody, SortNick:OrderNick, SortRating:OrderRating};
}


/**
 * @brief	Create a user item and return it
 * 
 * Create a user tr element, contais username, status/type and rating, and return this element.
 *
 * @param 	Username	The user's name
 * @param	Status		The new user's status
 * @param	Rating		The user's rating
 * @param	Type		The new user's type
 *
 * @author	Rubens Suguimoto
 *
 * @return 	Tr HTML element with username, status/type and rating
 */
function INTERFACE_CreateUser(Username, Status, Rating, Type)
{
	var Tr, Td1, Td2;

	Tr = UTILS_CreateElement("tr",Username);

	// Default type
	if ((Type == null) || (Type== ""))
	{
		Type = "user";
	}

	// Create user and rating html elements
	if (Username.length > 10)
	{
		Td1 = UTILS_CreateElement("td", null, Type+"_"+Status, UTILS_ShortString(Username,8));
		Td1.onmouseover = function () { INTERFACE_ShowUserFullName(this, Username); }
		Td1.onmouseout = function () { INTERFACE_CloseUserFullName(); }
	}
	else
	{
		Td1 = UTILS_CreateElement("td", null, Type+"_"+Status, Username);
	}
	Td2 = UTILS_CreateElement("td", null, "rating", Rating);

	// Add onclick function to open user menu
//	Td1.onclick = function () { CONTACT_ShowUserMenu(this, Username); };
	Tr.appendChild(Td1);
	Tr.appendChild(Td2);

	Tr.onclick = function () { CONTACT_ShowUserMenu(this, Username); };
	return Tr;
}


/**
 * @brief	Show user menu when click over user list item
 * 
 * Create a menu and show options to challenge, view profile, add/remove contact, admin options, etc.
 *
 * @param 	Obj	User list item
 * @param	Options	Array with user menu options;
 *
 * @author	Pedro
 *
 */
function INTERFACE_ShowUserMenu(Obj, Options)
{
	var Menu, Option, ParentNode, Pos, i;
	var Offset = 9;

	Menu = UTILS_CreateElement("div", "UserMenuDiv");

	// Creating options
	for (i=0; i < Options.length; i++)
	{
		// Create element
		// If Option is match request
		if (Options[i].Name == UTILS_GetText("usermenu_match"))
		{
			// test if match request was set, if not, set class as disabled
			if (Options[i].Func == null)
				Option = UTILS_CreateElement("p", null, 'option_disabled', Options[i].Name);
			// else, add this option normally
			else
				Option = UTILS_CreateElement("p", null, null, Options[i].Name);
		}
		else
		{
			Option = UTILS_CreateElement("p", null, null, Options[i].Name);
		}

		// Setting function
		Option.onclick = Options[i].Func;

		Menu.appendChild(Option);
	}
	// Get parent scrolling
	
	ParentNode = UTILS_GetParentDiv(Obj);

	if (MainData.Browser == 0)
	{
		// This a quick fix to contact list to open user menu correctly // TODO fix this properly
		if (UTILS_GetParentDiv(ParentNode.parentNode.parentNode.parentNode).className.match("Group") != null)
		{
			ParentNode = UTILS_GetParentDiv(ParentNode.parentNode.parentNode.parentNode.parentNode);
		}
		Offset = 0;
	}
	// This a quick fix to contact list to open user menu correctly // TODO fix this properly
	// Contact List
	else if (UTILS_GetParentDiv(ParentNode.parentNode.parentNode.parentNode).className.match("Group") != null)
	{
		ParentNode = UTILS_GetParentDiv(ParentNode.parentNode.parentNode.parentNode.parentNode);
		Offset = 2;
	}
	// Search User Result
	else if (ParentNode.id.match("ListDiv") != null) 
	{
		Offset= 3;
	}
	// Room and Online User List
	else if (ParentNode.className.match("UserTable") != null) 
	{
		if (MainData.Browser == 2)
		{
			Offset = 1; //
		}
	}

	// Get position of user list item
	Pos = UTILS_GetOffset(Obj);
	Menu.style.top = (Pos.Y+18-ParentNode.scrollTop-Offset)+"px";
	Menu.style.left = Pos.X+"px";
	
//	alert(ParentNode.scrollTop+" "+Pos.Y+" "+Offset+" "+Menu.style.top+"-"+Pos.X);

	document.body.appendChild(Menu);
}


/**
 * @brief	Hide user menu
 * 
 * Remove user menu from interface;
 *
 * @author	Pedro
 *
 */
function INTERFACE_HideUserMenu()
{
	var Menu = document.getElementById("UserMenuDiv");

	if (Menu != null)
	{
		Menu.parentNode.removeChild(Menu);
	}
}


/*****************************
*	FUNCTIONS - WINDOW
******************************/
/**
*	Create elements of search user window and returns div
*
* @return	Div; Array
* @see		WINDOW_Invite();
* @author Danilo Kiyoshi Simizu Yorinori
*/
function INTERFACE_ShowSearchUserWindow()
{
	// Variables
	var Div;

	var FormDiv, Username,Input, Br;

	var OptionDiv;
	var OptionLabel, Br1;
	var Name,NameLabel, User,UserLabel, Both, BothLabel;

	var ButtonsDiv, Search, Cancel;

	var Buttons = new Array();

	// Main div
	Div = UTILS_CreateElement('div', 'SearchUserDiv');

	// FormDiv elements
	FormDiv = UTILS_CreateElement('div', 'FormDiv');

	Username = UTILS_CreateElement('span', null, 'header', UTILS_GetText("contact_user_t"));
	Br = UTILS_CreateElement('br');
	Input = UTILS_CreateElement('input', 'SearchUserInput');
	Input.size = "23";

	// OptionDiv Elements
	OptionDiv = UTILS_CreateElement('div','OptionDiv');

	OptionLabel = UTILS_CreateElement('span',null,null,UTILS_GetText("contact_search_by"));
	Br1 = UTILS_CreateElement('br');
	
	try
	//Fix radio button for IE
	{
		Name = document.createElement('<input class="radio" type="radio" name="search_user" />');
	}
	catch(err) 
	{
		Name = UTILS_CreateElement('input',null,'radio');
		Name.type = "radio";
		Name.name = "search_user";
	}
	NameLabel = UTILS_CreateElement('span',null,'label',UTILS_GetText("contact_by_name"));

	try
	//Fix radio button for IE
	{
		User = document.createElement('<input class="radio" type="radio" name="search_user" checked="checked" />');
	}
	catch(err) 
	{
		User= UTILS_CreateElement('input',null,'radio');
		User.type = "radio";
		User.name = "search_user";
		User.checked = true;
	}
	UserLabel = UTILS_CreateElement('span',null,'label',UTILS_GetText("contact_by_user"));

	Both= UTILS_CreateElement('input');
	Both.type = "radio";
	Both.name = "search_user";
	BothLabel = UTILS_CreateElement('span',null,'label',UTILS_GetText("contact_both"));


	// ButtonsDiv elements
	ButtonsDiv = UTILS_CreateElement('div','ButtonsDiv');

	Search = UTILS_CreateElement('input',null,'button');
	Search.type = "button";
	Search.value = UTILS_GetText("window_search");
	UTILS_AddListener(Search,"click",	function() { 
		var Option;
		if (Name.checked == true)
		{
			Option = 0;
		}
		else if (User.checked == true)
		{
			Option = 1;
		}
		else if (Both.checked == true)
		{
			Option = 2;
		}
		else
		{
			Option = 2;
		}
		CONNECTION_SendJabber(MESSAGE_SearchUser(Input.value,Option)); }, "false");
	Buttons.push(Search);

	Cancel = UTILS_CreateElement('input',null,'button');
	Cancel.type = "button";
	Cancel.value = UTILS_GetText("window_cancel");
	Buttons.push(Cancel);

	// Mount elements tree
	// ButtonsDiv elements
	ButtonsDiv.appendChild(Search);
	ButtonsDiv.appendChild(Cancel);

	// OptionDiv
	OptionDiv.appendChild(OptionLabel);
	OptionDiv.appendChild(Br1);
	OptionDiv.appendChild(User);
	OptionDiv.appendChild(UserLabel);
	OptionDiv.appendChild(Name);
	OptionDiv.appendChild(NameLabel);

//	OptionDiv.appendChild(Both);
//	OptionDiv.appendChild(BothLabel);
	
	// FormDiv elements
	FormDiv.appendChild(Username);
	FormDiv.appendChild(Br);
	FormDiv.appendChild(Input);

	// Main div elements
	Div.appendChild(FormDiv);
	Div.appendChild(OptionDiv);
	Div.appendChild(ButtonsDiv);

	return {Div:Div, Buttons:Buttons};
}



/**
*	Create elements of an user 
*
* @param	Username	User that will be inserted
* @return	Tr
* @author Danilo Kiyoshi Simizu Yorinori
*/
function INTERFACE_CreateUserElement(Username)
{
	var Tr, Td;

	Tr = UTILS_CreateElement("tr");

	if (Username.length > 14)
	{
		Td = UTILS_CreateElement("td", null, null, UTILS_ShortString(Username,14));
		Td.onmouseover = function () { INTERFACE_ShowUserFullName(this, Username); }
		Td.onmouseout = function () { INTERFACE_CloseUserFullName(); }
	}
	else
	{
		Td = UTILS_CreateElement("td", null, null, Username);
	}
	Td.onclick = function () { CONTACT_ShowUserMenu(this, Username); };
	Tr.appendChild(Td);

	return Tr;
}

/**
*	Create elements of search user result window and returns div
*
* @param	UserList	List of users founded
* @return	Div; Array
* @see		WINDOW_Invite();
* @author Danilo Kiyoshi Simizu Yorinori
*/
function INTERFACE_ShowSearchUserResultWindow(UserList)
{
	// Variables
	var Div;
	var TopDiv;
	var ListDiv, Label,Table, TBody, Tr, Item, Br;
	var ButtonsDiv, Button;
	var Buttons = new Array();
	var i;

	// Main Div
	Div = UTILS_CreateElement('div', 'SearchUserDiv');

	TopDiv = UTILS_CreateElement('div', 'TopDiv');

	// Div of results
	ListDiv = UTILS_CreateElement('div', 'ListDiv');

	Table = UTILS_CreateElement('table');
	TBody = UTILS_CreateElement('tbody');

	if (UserList == null)
	{
		Label = UTILS_CreateElement('span', null, 'no_found', UTILS_GetText("contact_no_user_found"));
	}
	else
	{
		Label = UTILS_CreateElement('span', null, 'header', UTILS_GetText("contact_user_found"));
		
		for (i=0; i< UserList.length; i++)
		{
			// Insert each item of the user founded list in interface
			Tr = INTERFACE_CreateUserElement(UserList[i]);
			TBody.appendChild(Tr);
		}
	}
	Table.appendChild(TBody);

	Br = UTILS_CreateElement('br');

	// ButtonsDiv
	ButtonsDiv = UTILS_CreateElement('div','ButtonsDiv');

	Button = UTILS_CreateElement('input',null,'button');
	Button.type = "button";
	Button.value = UTILS_GetText("window_close");
	Buttons.push(Button);

	// Mount tree elements
	// ButtonsDiv elements
	ButtonsDiv.appendChild(Button);
	
	// TopDiv elements
	TopDiv.appendChild(Label);
	
	// Main div and result div elements
	Div.appendChild(TopDiv);
	Div.appendChild(Br);
	if (UserList != null)
	{
		ListDiv.appendChild(Table);
		Div.appendChild(ListDiv);
	}
	Div.appendChild(ButtonsDiv);

	return {Div:Div, Buttons:Buttons};
}

/**
 * @author	Danilo
 *
 */
function INTERFACE_ShowUserFullName(Obj,UserName)
{
	var Hint, Name, ParentNode, Pos, i;
	var Offset = 9;

	Hint = UTILS_CreateElement("div", "UserFullNameDiv");

	Name = UTILS_CreateElement("p", null, null, UserName);

	Hint.appendChild(Name);
	
	// Get parent scrolling
	
	ParentNode = UTILS_GetParentDiv(Obj);
	
	if (MainData.Browser == 0)
	{
		// This a quick fix to contact list to open user menu correctly // TODO fix this properly
		if (UTILS_GetParentDiv(ParentNode.parentNode.parentNode.parentNode).className.match("Group") != null)
		{
			ParentNode = UTILS_GetParentDiv(ParentNode.parentNode.parentNode.parentNode.parentNode);
		}
		Offset = 0;
	}
	// This a quick fix to contact list to open user menu correctly // TODO fix this properly
	// Contact List
	else if (UTILS_GetParentDiv(ParentNode.parentNode.parentNode.parentNode).className.match("Group") != null)
	{
		ParentNode = UTILS_GetParentDiv(ParentNode.parentNode.parentNode.parentNode.parentNode);
		Offset = 2; //ok
	}
	// Search User Result List
	else if (ParentNode.id.match("ListDiv") != null) 
	{
		Offset = 3; // ok
	}
	// Online and Room User List
	else if (ParentNode.className.match("UserTable") != null) 
	{
		if (MainData.Browser == 2)
		{
			Offset = 1; //
		}
		else
		{
			Offset = 9; // ok
		}
	}
	// User Nickname
	else if (ParentNode.id.match("UserInf") != null)
	{
		if (MainData.Browser == 2)
		{
			Offset = 3;
		}
		else
		{
			Offset = 6;
		}
	}


	// Get position of user list item
	Pos = UTILS_GetOffset(Obj);
	Hint.style.top = (Pos.Y+18-ParentNode.scrollTop-Offset)+"px";
	Hint.style.left = Pos.X+"px";
	Hint.style.width = UserName.length*6+'px';


	document.body.appendChild(Hint);
}

function INTERFACE_CloseUserFullName()
{
	var Hint = document.getElementById("UserFullNameDiv");
	if (Hint)
		document.body.removeChild(Hint);
}
