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
* Load images, scripts and css used in the interface
* from server
*/

/**
* Show load screen to user, and begin to load scripts
*/
function LOAD_StartLoad()
{
	// Remove login screen
	LOGIN_EndLogin();

	// Show load screen to user
	//INTERFACE_StartLoad();
	MainData.Load = new LoadObj();

	// Loading css files
	//INTERFACE_SetLoadPhrase(UTILS_GetText("login_load_css"), 2);
	LOAD_LoadFiles();
}

/**
* Remove load box from screen
*/
function LOAD_EndLoad()
{
	//INTERFACE_EndLoad();
	MainData.Load.remove();
	delete(MainData.Load);
}

/**
* Load scripts files while interface is loading
*/
function LOAD_LoadFiles()
{
	var Files = new Array();
	var NumFiles;
	
	// Images Files to be loaded
	/*
	Files.push("images/logochessd.png");
	Files.push("images/pieces/bbishop.png");
	Files.push("images/pieces/bknight.png");
	Files.push("images/pieces/bqueen.png");
	Files.push("images/pieces/bking.png");
	Files.push("images/pieces/bpawn.png");
	Files.push("images/pieces/bbrook.png");
	Files.push("images/pieces/wbishop.png");
	Files.push("images/pieces/wknight.png");
	Files.push("images/pieces/wqueen.png");
	Files.push("images/pieces/wking.png");
	Files.push("images/pieces/wpawn.png");
	Files.push("images/pieces/wbrook.png");
	Files.push("images/board/square_black.png");
	Files.push("images/board/square_white.png");
	Files.push("images/board/square_select.png");
	*/

	// CSS Files to be loaded
	Files.push("css/Top.css");
	Files.push("css/Left.css");
	Files.push("css/Contacts.css");
	Files.push("css/Rooms.css");
	Files.push("css/Window.css");
	Files.push("css/TopMenus.css");
	Files.push("css/Challenge.css");
	Files.push("css/Board.css");
	Files.push("css/Game.css");
	Files.push("css/Chat.css");
	Files.push("css/Profile.css");
	Files.push("css/Oldgame.css");
	Files.push("css/Welcome.css");
	Files.push("css/User.css");
	Files.push("css/Admin.css");
	Files.push("css/ChallengeMenu.css");
	Files.push("css/Announce.css");
	Files.push("css/Help.css");

	if(MainData.Browser == 0) //IE
	{
		Files.push("css/IEFix.css");
	}

	// Scripts Files to be loaded
	Files.push("scripts/parser/parser_iq.js");
	Files.push("scripts/parser/parser_chat.js");
	Files.push("scripts/admin/admin.js");
	Files.push("scripts/contact/contact.js");
	Files.push("scripts/chat/chat.js");
	Files.push("scripts/challenge/challenge.js");
	Files.push("scripts/challenge/adjourn.js");
	Files.push("scripts/challenge/announce.js");
	Files.push("scripts/contact/status.js");
	Files.push("scripts/contact/invite.js");
	Files.push("scripts/contact/info.js");
	Files.push("scripts/contact/search.js");
	Files.push("scripts/interface/interface.js");
	Files.push("scripts/interface/top.js");
	Files.push("scripts/interface/left.js");
	Files.push("scripts/interface/room.js");
	Files.push("scripts/interface/contact.js");
	Files.push("scripts/interface/chat.js");
	Files.push("scripts/interface/window.js");
	Files.push("scripts/interface/challengemenu.js");
	Files.push("scripts/interface/challenge.js");
	Files.push("scripts/interface/oldgame.js");
	Files.push("scripts/interface/welcome.js");
	Files.push("scripts/interface/user.js");
	Files.push("scripts/interface/admin.js");
	Files.push("scripts/interface/announce.js");
	Files.push("scripts/interface/profile.js");
	Files.push("scripts/interface/board.js");
	Files.push("scripts/interface/game.js");
	Files.push("scripts/profile/profile.js");
	Files.push("scripts/room/room.js");
	Files.push("scripts/window/window.js");
	Files.push("scripts/utils/dragpiece.js");
	Files.push("scripts/utils/square.js");
	Files.push("scripts/utils/images.js");
	Files.push("scripts/utils/dragwindow.js");
	Files.push("scripts/game/oldgame.js");
	Files.push("scripts/game/game.js");
	Files.push("scripts/interface/help.js");

	NumFiles = Files.length;
	LOAD_AppendFiles(Files, NumFiles);
}

/*
 * Load all files when log in jabber
 */
function LOAD_AppendFiles(Files, NumFiles)
{
	var FileType;
	var File;
	var Head = document.getElementsByTagName("head")[0];

	if(Files.length > 0)
	{
		// Get File type to create correct tag
		FileType = Files[0].split("/")[0];
		
		//Show file to be load
		MainData.Load.setLabel(Files[0])
			
		switch(FileType)
		{
			case "scripts":
				File = UTILS_CreateElement("script");
				File.src = Files[0]+"?"+NoCache.TimeStamp;
				File.type = "text/javascript";
				Head.appendChild(File);
				break;

			case "css":
				File = UTILS_CreateElement("link");
				File.href = Files[0]+"?"+NoCache.TimeStamp;
				File.type = "text/css";
				File.rel = "stylesheet";
				Head.appendChild(File);
				
				//Quick fix -> CSS doesn't trigger onload event
				//in FF2/FF3
				LOAD_NextFile(Files, NumFiles);
				break;

			case "images":
				File = UTILS_CreateElement("img");
				File.src = Files[0];
				break;
		}
	
		// http://cain.supersized.org/archives/2-Dynamic-loading-of-external-JavaScript-.js-files.html	
		// IE script onload doesn't work. To resolve this problem
		// we used onreadystatechange event to know when script
		// was loaded and ready to use.
		// This event work with CSS files too.
		if(MainData.Browser == 0) //IE
		{
			
			File.onreadystatechange = function(){
				if(File.readyState == "loaded" )
				{
					LOAD_NextFile(Files, NumFiles);
				}
			};
		}
		else // FF2 / FF3
		{
			File.onload = function(){LOAD_NextFile(Files, NumFiles)};
		}

		File.onerror = function(){LOAD_NextFile(Files, NumFiles)};
		File.onabort = function(){LOAD_NextFile(Files, NumFiles)};
	}
	// All files has been loaded
	else 
	{
		LOAD_EndFile(Files,NumFiles);
	}

}

function LOAD_NextFile(Files, NumFiles)
{
	var Num = (1/ NumFiles)*300;
	// Fill the loading bar progress
	MainData.Load.LoadBar.add(Num)

	// Remove first file from list and load next file
	Files.splice(0,1);
	LOAD_AppendFiles(Files, NumFiles);
}

function LOAD_EndFile(Files, NumFiles)
{
	var Num = (1/ NumFiles)*300;
	// Complete load bar
	MainData.Load.LoadBar.add(Num)

	// Start Webclient chess environment
	setTimeout("START_Webclient()", 1500);
}

/*
 * @brief Reload script, css files 
 *
 */
function LOAD_ReloadFiles()
{
	var Head = document.getElementsByTagName("head")[0];

	var CssFiles = Head.getElementsByTagName("link");
	var ScriptFiles = Head.getElementsByTagName("script");

	var File;

	while (CssFiles.length > 0)
	{
		Head.removeChild(CssFiles[0]);
	}
	
	while (ScriptFiles.length > 0)
	{
		Head.removeChild(ScriptFiles[0]);
	}

	File = UTILS_CreateElement("script");
	File.src = "initial_files.js?"+NoCache.TimeStamp;
	File.type = "text/javascript";
	Head.appendChild(File);

	File = UTILS_CreateElement("link");
	File.rel = "stylesheet";
	File.type = "text/css";
	File.href = "css/Main.css";
	Head.appendChild(File);

	File = UTILS_CreateElement("link");
	File.rel = "stylesheet";
	File.type = "text/css";
	File.href = "css/Login.css";
	Head.appendChild(File);

	File = UTILS_CreateElement("link");
	File.rel = "stylesheet";
	File.type = "text/css";
	File.href = "css/Load.css";
	Head.appendChild(File);

}

function LOAD_IECssFile()
{
	var Head = document.getElementsByTagName("head")[0];
	var File;

	File = UTILS_CreateElement("link");
	File.href = "css/LoadIE.css?"+NoCache.TimeStamp;
	File.type = "text/css";
	File.rel = "stylesheet";
	Head.appendChild(File);
}
