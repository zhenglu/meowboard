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
* Create elements of the left side
* of screen
*/
function INTERFACE_CreateLeft()
{
	var Left, UserBox, Rooms, ContactList;

	Left = UTILS_CreateElement("div", "Left");
	UserBox = INTERFACE_CreateUserBox();
	ContactList = INTERFACE_CreateContact();
	Rooms = INTERFACE_CreateRooms();

	Left.appendChild(UserBox);
	Left.appendChild(ContactList);
	Left.appendChild(Rooms);

	return Left;
}

/**
* Create user box (left side of the screen)
*/
function INTERFACE_CreateUserBox()
{
	var UserDiv, UserImg, UserInf, Name, Status, StatusItem;
	var Profile, ProfileP;
	var UserType;

	UserType = MainData.Type;

	if (!UserType)
	{
		UserType = "user";
	}
 
	UserDiv = UTILS_CreateElement("div", "User");
	UserImg = UTILS_CreateElement("img","UserImg");
	UserImg.title = MainData.Username;

	if (MainData.Photo == null)
	{
		UserImg.src = "images/no_photo.png";
	}
	else
	{
		UserImg.src = MainData.Photo;
	}

	UserInf = UTILS_CreateElement("div", "UserInf");
	if (MainData.Username.length > 10)
	{
		Name = UTILS_CreateElement("h2", null, null, UTILS_ShortString(MainData.Username,10));
		Name.onmouseover = function () { INTERFACE_ShowUserFullName(this, MainData.Username); }
		Name.onmouseout = function () { INTERFACE_CloseUserFullName(); }
	}
	else
	{
		Name = UTILS_CreateElement("h2", null, null, MainData.Username);
	}
	Status = UTILS_CreateElement("select", "UserStatusSelect");

	// Available
	StatusItem = UTILS_CreateElement("option", null, UserType+"_available", "("+UTILS_Capitalize(UTILS_GetText("status_available"))+")");
	StatusItem.value = "available";
	Status.appendChild(StatusItem);

	// Unavailable
	StatusItem = UTILS_CreateElement("option", null, UserType+"_unavailable", "("+UTILS_Capitalize(UTILS_GetText("status_unavailable"))+")");
	StatusItem.value = "unavailable";
	Status.appendChild(StatusItem);

	// Busy
	StatusItem = UTILS_CreateElement("option", null, UserType+"_busy", "("+UTILS_Capitalize(UTILS_GetText("status_busy"))+")");
	StatusItem.value = "busy";
	Status.appendChild(StatusItem);

	// Away
	StatusItem = UTILS_CreateElement("option", null, UserType+"_away", "("+UTILS_Capitalize(UTILS_GetText("status_away"))+")");
	StatusItem.value = "away";
	Status.appendChild(StatusItem);

	Status.onchange = function () { CONTACT_ChangeStatus(this.value) };

	ProfileP = UTILS_CreateElement("p");
	Profile = UTILS_CreateElement("span",null,null,UTILS_GetText("contact_change_profile"));
	ProfileP.appendChild(Profile);
	UTILS_AddListener(Profile,"click", function() { PROFILE_StartProfile(MainData.Username); }, "false");

	UserInf.appendChild(Name);
	UserInf.appendChild(Status);
	UserInf.appendChild(ProfileP);
	UserDiv.appendChild(UserImg);
	UserDiv.appendChild(UserInf);

	return UserDiv;
}


/**
* Change user's image
*/
function INTERFACE_SetUserImage(Img)
{
	var UserImg = document.getElementById("UserImg");

	if ((UserImg != null) && (Img != "images/no_photo.png"))
	{
		UserImg.src = IMAGE_ImageDecode(Img);
	}
}
