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
* Handle users status
*/


/**
* Change User Status 
*/
function CONTACT_ChangeStatus(NewStatus, DontSend)
{
	var i, XML, Status, StatusItem;
	var Select;
		
	// Change user status for contacts
	XML = MESSAGE_ChangeStatus(NewStatus);
	
	// Change user status for rooms
	for (i=0 ; i<MainData.RoomList.length ; i++)
	{
		XML += MESSAGE_ChangeStatus(NewStatus, MainData.RoomList[i].MsgTo);
	}
	
	// Change status in select menu
	Select = document.getElementById("UserStatusSelect");

	// If new status is playing, create new item in select box, select it and disabled select box
	if (NewStatus == "playing")
	{
		// Playing
		StatusItem = UTILS_CreateElement("option", 'status_playing_op', MainData.UserType+"_playing", "("+UTILS_Capitalize(UTILS_GetText("status_playing"))+")");
		StatusItem.value = "playing";
		StatusItem.selected = true;
		Select.appendChild(StatusItem);
	
		Select.disabled = true;
	}
	// If current status is playing, remove playing option from select box, enable select box and 
	// select avaiable status(Index 0)
	else if (MainData.Status == "playing")
	{
		Select.disabled = false;
		StatusItem = document.getElementById('status_playing_op');
		Select.removeChild(StatusItem);
		Select.selectedIdex = 0;
	}
	
	// Update your status in structure
	MainData.Status = NewStatus;

	// Send to jabber or return the message
	if (DontSend == null)
	{
		CONNECTION_SendJabber(XML);
		return null;
	}
	else
	{
		return XML;
	}
}

/**
* Change status of 'Username' in structure and interface
*/
function CONTACT_SetUserStatus(Username, NewStatus)
{
	var Rating, Type;
	var UserPos;

	// Update new user status in data struct
	MainData.SetUserStatus(Username, NewStatus)

	// Update user status in interface
	if(MainData.Contact != null)
	{
		// Find user in data struct 
		UserPos = MainData.FindUser(Username);

		if(UserPos != null)
		{
			// Get user type
			Type = MainData.UserList[UserPos].Type;

			// Get user rating
			switch(MainData.CurrentRating)
			{
				case "blitz":
					Rating = MainData.UserList[UserPos].Rating.Blitz;
					break;
				case "lightning":
					Rating = MainData.UserList[UserPos].Rating.Lightning;
					break;
				case "standard":
					Rating = MainData.UserList[UserPos].Rating.Standard;
					break;
			}
			// Update user status in contact list
			MainData.Contact.updateUser(Username, NewStatus, Rating, Type);
		}

	}
	return "";
}

/**
 * Start away counter;
 */
function CONTACT_StartAwayCounter()
{
	MainData.AwayCounter = 300;

	MainData.AwayTimeout = setInterval("CONTACT_SetAwayStatus()", 1000);

	document.body.setAttribute("onmousedown","CONTACT_ResetAwayStatus()");
	document.body.setAttribute("onkeypress","CONTACT_ResetAwayStatus()");
}

/**
 * Countdown away counter, and set away status if away counter less than zero
 */ 
function CONTACT_SetAwayStatus()
{
	var Select = document.getElementById("UserStatusSelect");

	MainData.AwayCounter = MainData.AwayCounter - 1;

	if(MainData.AwayCounter == 0)
	{
		if((MainData.Status != "playing")&&(MainData.Status != "unavailable"))
		{
			CONTACT_ChangeStatus("away");
			
			// Select away status 
			Select.selectedIndex = 3;
		}
	}
}

/**
 * Reset away counter and set status to available
 */
function CONTACT_ResetAwayStatus()
{
	var Select = document.getElementById("UserStatusSelect");

	// Away counter reset to 5 minutes
	MainData.AwayCounter = 300;

	if(MainData.Status == "away")
	{
		CONTACT_ChangeStatus("available");
			
		// Select available status 
		Select.selectedIndex = 0;
	}

}

/**
 * Stop away counter
 */
function CONTACT_StopAwayStatus()
{
	clearInterval(MainData.AwayTimeout);

	document.body.removeAttribute("onmousedown");
	document.body.removeAttribute("onkeypress");
}
