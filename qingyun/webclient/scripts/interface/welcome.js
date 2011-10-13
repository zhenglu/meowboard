/*
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

function INTERFACE_CreateWelcome()
{
	var WelcomeDiv, WelcomeCenterDiv;
	var WelcomeImageDiv, WelcomeInstructionDiv;
	var WelcomeClose;
	var InstructionList;
	var InstructionItem;
	var Title;
	var Text1, Text2;

	var XMLInstruction;
	var i;

	WelcomeDiv = UTILS_CreateElement("div","Welcome");
	WelcomeCenterDiv = UTILS_CreateElement("div","WelcomeCenter");
	WelcomeImageDiv = UTILS_CreateElement("div","WelcomeImage");
	WelcomeInstructionDiv = UTILS_CreateElement("div","WelcomeInstruction");

	WelcomeClose= UTILS_CreateElement("div","WelcomeClose",null,"X");

	InstructionList = UTILS_CreateElement("ul");

	Title = UTILS_CreateElement("h1",null,null,UTILS_GetText("welcome_title"));

	Text1 = UTILS_CreateElement("p",null,"center", UTILS_GetText("welcome_text1"));
	Text2 = UTILS_CreateElement("p",null,"center", UTILS_GetText("welcome_text2"));

	WelcomeClose.onclick = function(){ INTERFACE_RemoveWelcome();};

	XMLInstruction = MainData.GetText.getElementsByTagName("welcome_item");

	for(i=0; i<XMLInstruction.length ; i++)
	{
		InstructionItem = UTILS_CreateElement("li",null,null,UTILS_GetNodeText(XMLInstruction[i]));

		InstructionList.appendChild(InstructionItem);
	}

	WelcomeInstructionDiv.appendChild(InstructionList);

	WelcomeCenterDiv.appendChild(WelcomeImageDiv);
	WelcomeCenterDiv.appendChild(WelcomeInstructionDiv);

	WelcomeDiv.appendChild(Title);
	WelcomeDiv.appendChild(Text1);
	WelcomeDiv.appendChild(WelcomeCenterDiv);
	WelcomeDiv.appendChild(Text2);
	WelcomeDiv.appendChild(WelcomeClose);

	return WelcomeDiv;
}

function INTERFACE_RemoveWelcome()
{
	var WelcomeDiv = document.getElementById("Welcome");

	if(WelcomeDiv != null)
	{
		WelcomeDiv.parentNode.removeChild(WelcomeDiv);
	}

	return "";
}
