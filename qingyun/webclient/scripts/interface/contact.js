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
* Control interface of contact list
*/

/*******************************************
 ******* FUNCTIONS - CONTACT OBJECT
 * ***************************************/

function ContactObj()
{
	var ContactContent = INTERFACE_CreateContactContent();
	// Attributes
	this.div = ContactContent.MainDiv;
	this.listDiv = ContactContent.ListDiv;
	this.loadingDiv = ContactContent.LoadingDiv;

	this.groups = new Array();
	
	//Create sort options
	this.sort = new UserListObj(this.div);
	this.sort.show();
	this.sort.hideList();
	this.sort.setSortUserFunction(CONTACT_SortUsersByNick);
	this.sort.setSortRatingFunction(CONTACT_SortUsersByRating);

	// Methods
	this.addGroup = INTERFACE_AddContactGroup;
	this.removeGroup = INTERFACE_RemoveContactGroup;
	this.findGroup = INTERFACE_FindContactGroup;
	this.getGroup = INTERFACE_GetContactGroup;
	this.findUserGroup = INTERFACE_FindContactUserGroup;

	this.addUser = INTERFACE_AddContactUser;
	this.removeUser = INTERFACE_RemoveContactUser;
	this.updateUser = INTERFACE_UpdateContactUser;

	this.show = INTERFACE_ShowContactList;
	this.hide = INTERFACE_HideContactList;

	this.showLoading = INTERFACE_ShowContactLoading;
	this.hideLoading = INTERFACE_HideContactLoading;
}

function INTERFACE_AddContactGroup(GroupName)
{
	//TODO -> Create contact Group object in other file
	var Group = new Object();
	var GroupInterface = INTERFACE_CreateGroup(GroupName);

	Group.mainDiv = GroupInterface.GroupDiv;
	Group.onlineDiv = GroupInterface.Online;
	Group.offlineDiv = GroupInterface.Offline;
	Group.title = GroupInterface.Title;
	Group.name = GroupName;

	Group.online = new UserListObj(Group.onlineDiv);
	Group.online.show();
	Group.online.hideSort();

	Group.offline = new UserListObj(Group.offlineDiv);
	Group.offline.show();
	Group.offline.hideSort();

	Group.display = "block";
	Group.show = function (){
		if(this.display == "none")
		{
			this.display = "block";
			this.online.showList();
			this.offline.showList();
			this.title.onclick = function() { Group.hide() };
		}
	}
	Group.hide = function() {
		if(this.display == "block")
		{
			this.display = "none";
			this.online.hideList();
			this.offline.hideList();
			this.title.onclick = function() { Group.show() };
		}
	}

	Group.remove = function(){
		this.mainDiv.parentNode.removeChild(this.mainDiv);
	};

	Group.title.onclick = function() { Group.hide() }

	this.groups.push(Group);
	this.listDiv.appendChild(GroupInterface.GroupDiv);
}

function INTERFACE_CreateGroup(GroupName)
{
	var GroupDiv;
	var GroupTitle;
	var GroupOnline, GroupOffline;

	GroupDiv = UTILS_CreateElement("div",null,"GroupDiv");

	if(GroupName != "default")
	{
		GroupTitle = UTILS_CreateElement("label",null,null,GroupName);
	}
	else
	{
		GroupTitle = UTILS_CreateElement("label",null,null,UTILS_GetText("contact_default_group"));
	}

	GroupOnline = UTILS_CreateElement("div",null,"OnlineGroup");
	GroupOffline = UTILS_CreateElement("div",null,"OfflineGroup");

	GroupDiv.appendChild(GroupTitle);
	GroupDiv.appendChild(GroupOnline);
	GroupDiv.appendChild(GroupOffline);

	return {GroupDiv:GroupDiv, Online:GroupOnline, Offline:GroupOffline, Title:GroupTitle };
}

function INTERFACE_RemoveContactGroup(GroupName)
{
	var GroupPos = this.findGroup(GroupName);
	var Group = this.getGroup(GroupName);

	//Remove group from interface
	Group.remove();

	this.groups.splice(GroupPos,1);

}

function INTERFACE_FindContactGroup(GroupName)
{
	var i=0;

	while((i<this.groups.length) && (GroupName != this.groups[i].name))
	{
		i++;
	}
	
	if(i<this.groups.length)
	{
		return i;
	}
	else
	{
		return null;
	}
}

function INTERFACE_GetContactGroup(GroupName)
{
	var GroupPos = this.findGroup(GroupName);
	return this.groups[GroupPos];
}


function INTERFACE_FindContactUserGroup(UserName)
{
	var i=0;
	var GroupTmp;

	while(i<this.groups.length)
	{
		GroupTmp = this.groups[i];
		if(GroupTmp.online.findUser(UserName) != null)
		{
			return GroupTmp;
		}

		if(GroupTmp.offline.findUser(UserName) != null)
		{
			return GroupTmp;
		}
		i++;
	}
	return null;
}


function INTERFACE_AddContactUser(GroupName, UserName, Status, Rating, Type)
{
	var Group = this.getGroup(GroupName);

	if(Group == null)
	{
		this.addGroup(GroupName);
		Group = this.getGroup(GroupName);
	}

	if(Status != "offline")
	{
		Group.online.addUser(UserName, Status, Rating, Type);
	}
	else
	{
		Group.offline.addUser(UserName, Status, Rating, Type);
	}
}

function INTERFACE_RemoveContactUser(UserName)
{
	var Group = this.findUserGroup(UserName);

	// User not founded
	if(Group == null)
	{
		return null;
	}

	if(Group.online.findUser(UserName) != null)
	{
		Group.online.removeUser(UserName);
	}
	else 
	{
		Group.offline.removeUser(UserName);
	}

	return UserName;
}

function INTERFACE_UpdateContactUser(UserName, Status, Rating, Type)
{
	var Group = this.findUserGroup(UserName);

	// Group not founded
	if(Group == null)
	{
		return null;
	}

	// if user is in online list
	if(Group.online.findUser(UserName) != null)
	{
		// if this online user status turn to offline... 
		if(Status == "offline")
		{	
			//Remove from online list and insert in offline;
			this.removeUser(UserName);
			this.addUser(Group.name, UserName, Status, Rating, Type);
		}
		// update user state
		else 
		{
			Group.online.updateUser(UserName, Status, Rating, Type);
		}
	}
	// if user is in offline list
	else 
	{
		if(Status != "offline")
		{
			//Remove from offline list and insert in online;
			this.removeUser(UserName);
			this.addUser(Group.name, UserName, Status, Rating, Type);
		}
		else
		{
			Group.offline.updateUser(UserName, Status, Rating, Type);
		}
	}
	return UserName;	
}



function INTERFACE_ShowContactList()
{
	var ParentTmp;

	// The code above is used in specific case of this interface.
	// Contact list should be in "Contact" div.
//	ParentTmp = document.getElementById("Contact");
	ParentTmp = document.getElementById("UserLists");
	if(this.div.parentNode != ParentTmp)
	{
		ParentTmp.appendChild(this.div);
	}

	this.div.style.display = "block";
}

function INTERFACE_HideContactList()
{
	this.div.style.display = "none";
}

function INTERFACE_ShowContactLoading()
{
	this.loadingDiv.style.display = "block";
}

function INTERFACE_HideContactLoading()
{
	this.loadingDiv.style.display = "none";
}


/*******************************************
 ******* FUNCTIONS - CONTACT ONLINE OBJECT
 * ***************************************/

// Contact Online Object
function ContactOnlineObj()
{
	var ContactOnline = INTERFACE_CreateOnlineContent();
	this.div = ContactOnline.Div;
	this.loadingDiv = ContactOnline.LoadingDiv;

	this.userList = new UserListObj(this.div);
	this.userList.show();
	this.userList.setSortUserFunction(CONTACT_OnlineSortUserByNick);
	this.userList.setSortRatingFunction(CONTACT_OnlineSortUserByRating);

	this.show = INTERFACE_ShowOnlineList;
	this.hide = INTERFACE_HideOnlineList;

	this.showLoading = INTERFACE_ShowContactLoading;
	this.hideLoading = INTERFACE_HideContactLoading;
}


function INTERFACE_ShowOnlineList()
{
	var ParentTmp;

	// The code above is used in specific case of this interface.
	// Contact list should be in "Contact" div.
//	ParentTmp = document.getElementById("Contact");
	ParentTmp = document.getElementById("UserLists");
	if(this.div.parentNode != ParentTmp)
	{
		ParentTmp.appendChild(this.div);
	}

	this.div.style.display = "block";
}

function INTERFACE_HideOnlineList()
{
	this.div.style.display = "none";
}



/**
* Set type of user in interface
*
* @public
*/
/*
function INTERFACE_SetUserType(Username, NewType)
{
	var User = document.getElementById("contact-"+Username);
	var List, Node, i;


	// Updating user's type
	if (User)
	{
		User.className = User.className.replace(/.*_/, NewType+"_");
	}

	// Updating in room lists
	for (i=0; i<MainData.RoomList.length; i++)
	{
		if (MainData.FindUserInRoom(MainData.RoomList[i].Name, Username) != null)
		{
			// Search user node in room user list
			Node = document.getElementById(MainData.RoomList[i].Name+"_"+Username);

			if (Node)
			{
				Node.className = Node.className.replace(/.*_/, NewType+"_");
			}
		}
	}

	return true;
}
*/

/**
* Show or hide contact groups
*
* @private
*/
function INTERFACE_ChangeGroupVisibility(Obj, Id)
{
	var Node = document.getElementById(Id);
	
	if (!Node)
	{
		return false;
	}
	// Changing node visibilty
	if (Node.style.display == "none")
	{
		// Display should be "list-item", "table" don't work in IE6
		Node.style.display = "list-item";
		Obj.innerHTML = Obj.innerHTML.replace("+", "-");
	}
	else
	{
		Node.style.display = "none";
		Obj.innerHTML = Obj.innerHTML.replace("-", "+");
	}
	return true;
}


/**
* Create contact list
*
* @private
*/
function INTERFACE_CreateContact()
{
	var ContactDiv, ContactTitle;
	var ContactTitleOnline, ContactTitleContacts;
	var ContactTitleOnlineSpan, ContactTitleContactsSpan;
	var Lists;

	// Main div
	ContactDiv = UTILS_CreateElement("div", "Contact");

	// Contact change bar
	ContactTitle = UTILS_CreateElement("ul", "ContactTitle");
	ContactTitleContacts = UTILS_CreateElement("li");

	ContactTitleOnline = UTILS_CreateElement("li", null, "contact_selec");

	ContactTitleContactsSpan = UTILS_CreateElement("span", null, 'bold', UTILS_GetText("contact_contacts"));
	ContactTitleOnlineSpan = UTILS_CreateElement("span", null, 'bold', UTILS_GetText("contact_online"));

	ContactTitleContacts.onclick = function(){
		ContactTitleContacts.className = "contact_selec";
		ContactTitleOnline.className = "";

		MainData.Contact.show();
		MainData.ContactOnline.hide();
	};
	ContactTitleOnline.onclick = function(){
		ContactTitleContacts.className = "";
		ContactTitleOnline.className = "contact_selec";

		MainData.Contact.hide();
		MainData.ContactOnline.show();
	};

	Lists = UTILS_CreateElement("div","UserLists");

	// Creating DOM tree
	ContactTitleOnline.appendChild(ContactTitleOnlineSpan);
	ContactTitleContacts.appendChild(ContactTitleContactsSpan);
	ContactTitle.appendChild(ContactTitleOnline);
	ContactTitle.appendChild(ContactTitleContacts);

	ContactDiv.appendChild(ContactTitle);
	ContactDiv.appendChild(Lists);
//	ContactDiv.appendChild(INTERFACE_CreateContactContent());

	return ContactDiv;
}

function INTERFACE_CreateContactContent()
{
	var ContactDiv, ContactsDiv, ContactTitle, ContactInside, ContactOnlineDiv, ContactOfflineDiv, ContactOnline, ContactOffline;
	var OrderNick, OrderRating, OrderRatingOpt, Search;
	var Hr;
	var SearchP, SearchS;
	var ListDiv;
	var LoadingDiv;

	//Contact content
	ContactInside = UTILS_CreateElement("div", "ContactInside");
	ListDiv = UTILS_CreateElement("div", "ListDiv");

	// Search user
//	Search = UTILS_CreateElement("a", null, null, UTILS_GetText("menu_search_user"));
	SearchP = UTILS_CreateElement("p",null,"contact_search_user_p");
	SearchS = UTILS_CreateElement("span","contact_search_user", null, UTILS_GetText("menu_search_user"));
	UTILS_AddListener(SearchP, "click", function() { WINDOW_SearchUser(); }, "false");
	SearchP.appendChild(SearchS);
	Hr = UTILS_CreateElement("hr");

	LoadingDiv = INTERFACE_CreateLoadingBox("contact_loading",UTILS_GetText("contact_loading"));

	// Creating DOM tree
	ContactInside.appendChild(ListDiv);
	ContactInside.appendChild(Hr);
	ContactInside.appendChild(SearchP);
	ContactInside.appendChild(LoadingDiv);

	//HIDE CONTACT DIV
	ContactInside.style.display = "none";

	return { MainDiv:ContactInside, ListDiv:ListDiv, LoadingDiv:LoadingDiv};
}


/**
* Create contact online list
*
* @private
*/
function INTERFACE_CreateOnlineContent()
{
	var ContactDiv, ContactsDiv, ContactTitle, ContactInside, ContactOnlineDiv, ContactOfflineDiv, ContactOnline, ContactOffline;
	var ContactsOnline, ContactsOffline;
	var OnlineTable, OnlineTbody;
	var OfflineTable, OfflineTbody;
	var OrderNick, OrderRating, OrderRatingOpt, Search;
	var Hr;
	var SearchP, SearchS;
	var LoadingDiv;

	//Contact content
	ContactInside = UTILS_CreateElement("div", "ContactOnline");


	// Search user
	SearchP = UTILS_CreateElement("p",null,"contact_search_user_p");
	SearchS = UTILS_CreateElement("span","contact_search_user", null, UTILS_GetText("menu_search_user"));
	UTILS_AddListener(SearchP, "click", function() { WINDOW_SearchUser(); }, "false");
	SearchP.appendChild(SearchS);
	Hr = UTILS_CreateElement("hr");

	LoadingDiv = INTERFACE_CreateLoadingBox("contact_online_loading",UTILS_GetText("contact_online_loading"));
	
	// Creating DOM tree
	ContactInside.appendChild(Hr);
	ContactInside.appendChild(SearchP);
	ContactInside.appendChild(LoadingDiv);

	//HIDE CONTACT DIV
	//ContactInside.style.display = "none";

	return { Div:ContactInside, LoadingDiv:LoadingDiv};
}

