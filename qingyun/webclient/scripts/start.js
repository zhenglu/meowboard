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

/*
* Start MainData and show login page
*/

/**
* Global object that stores all data needed
* by interface
*/
var MainData;

/*
* @brief	Start main data and show login page
* 
* Start page, initialize MainData and show page
*
* @author 	Rubens Suguimoto
*/
function START_StartPage()
{
	var Lang;
	var ConfTmp;

	// What language show?
	// Find lang in cookie
	Lang = UTILS_ReadCookie("lang");
	// if language is not found in cookie
	if (Lang == "")
	{
		// Get from browser language
		//Lang = UTILS_GetLanguage();
		
		// Get default lang from configuration file
		ConfTmp = UTILS_OpenXMLFile("scripts/data/conf.xml?"+NoCache.TimeStamp);
		Lang = UTILS_GetTag(ConfTmp, "default-lang");
	}

	// Read xml config files and starting data structure
	MainData = new DATA("scripts/data/conf.xml?"+NoCache.TimeStamp, "scripts/lang/"+Lang+".xml?"+NoCache.TimeStamp);
	MainData.Lang = Lang;
	
	INTERFACE_StartLogin(Lang);
}

/*
* @brief	Set new language and re-show login page
*
* Set new language in main data and show login page with new language selected
*
* @param 	Lang	Language in ISO 639 and ISO 3166 format standard.
* @author 	Rubens Suguimoto
*/
function START_ChangeLanguage(Lang)
{
	// Close login div
	INTERFACE_EndLogin();

	// Reload MainData with configurations and new language selected
//	MainData = new DATA("scripts/data/conf.xml", "scripts/lang/"+Lang+".xml");
	MainData = new DATA("scripts/data/conf.xml?"+NoCache.TimeStamp, "scripts/lang/"+Lang+".xml?"+NoCache.TimeStamp);
	// Create cookie for new language
	UTILS_CreateCookie("lang", Lang, MainData.CookieValidity);

	// Set language
	MainData.Lang = Lang;
	
	// Show new login div with language selected
	INTERFACE_StartLogin(Lang);
}



/**
* Clear Login window and start interface 
*
* @return none
* @public
*/
function START_Webclient()
{
	var All = INTERFACE_CreateInterface();
	var XMPP = "";

	MainData.ConnectionStatus = 0;

	XMPP += MESSAGE_UserList();
	XMPP += MESSAGE_Presence(MainData.RatingComponent+"."+MainData.Host);
	XMPP += MESSAGE_GetProfile(MainData.Username, MainData.Const.IQ_ID_GetMyProfile);
	XMPP += MESSAGE_Presence();
	XMPP += MESSAGE_Presence("general@"+MainData.ConferenceComponent+"."+MainData.Host+"/"+MainData.Username);

	CONNECTION_SendJabber(XMPP);

	// Close load image
	LOAD_EndLoad();

	// Open XadrezLivre game environment
	INTERFACE_ShowInterface(All);
	
	// Create contact object and set values
	CONTACT_StartContact();
	CONTACT_LoadUserContactList();	

	// Create challenge menu object
	CHALLENGE_StartChallenge();

	// Search for some game that player is playing
	GAME_SearchCurrentGame();

	// Set away counter
	CONTACT_StartAwayCounter();
}
/*
*	@brief Stop interface and reload files
*
*	@author Danilo Yorinori
*/
function START_Restart()
{
	INTERFACE_StopInterface();

	CONTACT_StopAwayStatus();

	delete MainData;

	// Get new timestamp
	NoCache.TimeStamp = "";
	NoCache.TimeStamp += NoCache.DateTime.getMonth();
	NoCache.TimeStamp += "/"+NoCache.DateTime.getDate();
	NoCache.TimeStamp += "/"+NoCache.DateTime.getFullYear();
	NoCache.TimeStamp += "-"+NoCache.DateTime.getHours();
	NoCache.TimeStamp += ":"+NoCache.DateTime.getMinutes();
	NoCache.TimeStamp += ":"+NoCache.DateTime.getSeconds();

	// Reload Scripts
	LOAD_ReloadFiles();

	//START_StartPage();
	INITIAL_LoadScripts();

	// Verify browser and if IE then append related css file
	if(MainData.Browser == 0)
	{
		LOAD_IECssFile();
	}
}
