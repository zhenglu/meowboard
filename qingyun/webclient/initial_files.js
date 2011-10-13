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
* Append initial files
*/

// Create NoCache object
var NoCache = new Object();

NoCache.DateTime = new Date();
NoCache.TimeStamp = "";
NoCache.TimeStamp += NoCache.DateTime.getMonth();
NoCache.TimeStamp += "/"+NoCache.DateTime.getDate();
NoCache.TimeStamp += "/"+NoCache.DateTime.getFullYear();
NoCache.TimeStamp += "-"+NoCache.DateTime.getHours();
NoCache.TimeStamp += ":"+NoCache.DateTime.getMinutes();
NoCache.TimeStamp += ":"+NoCache.DateTime.getSeconds();

/* 
 * @brief	Create script or link element and append file in head element
 *
 * @param	FileType	File's type
 * @param	Addr	File's address
 * @return	void
 * @author	Danilo Yorinori
 */
function INITIAL_AppendFiles(FileType,Addr)
{
	var File;
	var Head = document.getElementsByTagName("head")[0];

	switch(FileType)
	{
		case "scripts":
			File = document.createElement("script");
			File.src = Addr+"?"+NoCache.TimeStamp;
			File.type = "text/javascript";
			Head.appendChild(File);
			break;

		case "css":
			File = document.createElement("link");
			File.href = Addr+"?"+NoCache.TimeStamp;
			File.type = "text/css";
			File.rel = "stylesheet";
			Head.appendChild(File);
			break;

		case "favicon":
			File = document.createElement("link");
			File.href = Addr;
			File.rel = "shortcut icon";
			Head.appendChild(File);
			break;
	}
}

/* 
 * @brief	Create list of scripts to be load
 *
 * @return	void
 * @author	Rubens Suguimoto
 */
function INITIAL_LoadScripts()
{
	var ScriptList = new Array();

	var ProgressText = document.createElement("p");
	var LoadingText = document.createElement("p");
	ProgressText.setAttribute("id","ProgressText");
	LoadingText.setAttribute("id","LoadingText");
	LoadingText.innerHTML = "Loading...";

	// Create Loading progress text
	document.body.appendChild(LoadingText);
	document.body.appendChild(ProgressText);

	// Append favicon
	INITIAL_AppendFiles("favicon","images/favicon.ico");

	// Append css files
	/*
	INITIAL_AppendFiles("css","css/Main.css");
	INITIAL_AppendFiles("css","css/Login.css");
	INITIAL_AppendFiles("css","css/Load.css");
	*/
	// Append script files
	ScriptList.push("scripts/utils/utils.js");
	ScriptList.push("scripts/data/data.js");
	ScriptList.push("scripts/data/consts.js");
	ScriptList.push("scripts/login/login.js");
	ScriptList.push("scripts/load/load.js");
	ScriptList.push("scripts/connection/connection.js");
	ScriptList.push("scripts/xmpp_messages/message.js");
	ScriptList.push("scripts/parser/parser.js");
	ScriptList.push("scripts/parser/parser_presence.js");
	ScriptList.push("scripts/interface/login.js");
	ScriptList.push("scripts/interface/load.js");
	ScriptList.push("scripts/start.js");

	INITIAL_AppendScript(ScriptList, ScriptList.length);
}

/* 
 * @brief	Select last script in script list and load it.
 * 
 * Select the last script file name in script list that will be loaded. When
 * script has already loaded, remove last script from script list and call
 * this function again.
 *
 * @param	ScriptList	List of script url
 * @param	NumFiles	Total number of files to be load
 * @return	void
 * @author	Rubens Suguimoto
 */
function INITIAL_AppendScript(ScriptList, NumFiles)
{
	var Script;
	var Head = document.getElementsByTagName("head")[0];
	var ProgressText, LoadingText;
	var ScriptFile
	var Perc;

	if(ScriptList.length > 0)
	{
		// Select and remove last script from script list
		ScriptFile = ScriptList.pop();

		// Load remaing files script
		Script = document.createElement("script");
		Script.src = ScriptFile+"?"+NoCache.TimeStamp;
		Script.type = "text/javascript";
		Head.appendChild(Script);

		Script.onload = function(){
			INITIAL_AppendScript(ScriptList, NumFiles);
			Perc = (NumFiles-ScriptList.length)/NumFiles;
			INITIAL_SetProgressText(Math.round(Perc*100)+"% - "+ScriptFile);
		}

		Script.onreadystatechange = function(){
			if(Script.readyState == "loaded")
			{
				INITIAL_AppendScript(ScriptList, NumFiles);
				Perc = (NumFiles-ScriptList.length)/NumFiles;
				INITIAL_SetProgressText(Math.round(Perc*100)+"% - "+ScriptFile);
			}
		}

/*
		Script.onerror = function(){
			alert("Error while loading scripts\nFile: "+ScriptFile);
		}

		Script.onabort = function(){
			alert("Abort loading scripts\nFile: "+ScriptFile);
		}
*/
	}
	else // All initial scripts has already loaded
	{
		// Remove loading texts and start login page
		INITIAL_RemoveTexts();
		START_StartPage();
	}
}

/* 
 * @brief	Change progress text
 *
 * @param	Text	Text string
 * @return	void
 * @author	Rubens Suguimoto
 */
function INITIAL_SetProgressText(Text)
{
	var ProgressText = document.getElementById("ProgressText");

	if(ProgressText != null)
	{
		ProgressText.innerHTML = Text;
	}
}

/* 
 * @brief	Remove loading and progress text;
 *
 * @return	void
 * @author	Rubens Suguimoto
 */
function INITIAL_RemoveTexts()
{
	var ProgressText = document.getElementById("ProgressText");
	var LoadingText = document.getElementById("LoadingText");
	document.body.removeChild(ProgressText);
	document.body.removeChild(LoadingText);
}
