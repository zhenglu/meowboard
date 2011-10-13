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

function LoadObj() 
{
	var Load = INTERFACE_StartLoad();

	// Attributes
	this.LoadDiv = Load.LoadDiv;
	this.LoadLabel = Load.LoadLabel;
	this.BarBorder = Load.BarBorder;
	this.LoadBar = new LoadingBar();

	// Methods
	this.show = INTERFACE_ShowLoad;
	this.hide = INTERFACE_HideLoading;
	this.remove = INTERFACE_RemoveLoad;
	this.setLabel = INTERFACE_SetLabel;

	// Show LoadDiv
	this.LoadBar.show(this.BarBorder);
	this.show();
}

/**
* Shows load screen to user
*/
function INTERFACE_StartLoad()
{
	var LoadDiv, LoadHeader, WaitLabel, LoadingLabel;
	var LoadList, Item, Img, i;
	var BarBorder;

	// Creating elements
	LoadDiv = UTILS_CreateElement("div", "LoadDiv");
	LoadHeader = UTILS_CreateElement("h1", null, null, UTILS_GetText("general_name"));
	WaitLabel = UTILS_CreateElement("h3", null, null, UTILS_GetText("login_load_wait"));
	LoadingLabel = UTILS_CreateElement("p", "LoadingLabel", null);
	
	BarBorder = UTILS_CreateElement("div","BarBorder");

	/*
	LoadList = UTILS_CreateElement("ul", "LoadList");
	// Creating balls
	for (i=1; i<=5; i++)
	{
		Item = UTILS_CreateElement("li", "LoadingBall"+i, "grey_ball");
		LoadList.appendChild(Item);
	}
	*/
	// Creating tree
	LoadDiv.appendChild(LoadHeader);
	LoadDiv.appendChild(WaitLabel);
	LoadDiv.appendChild(LoadingLabel);
	//LoadDiv.appendChild(LoadList);

	LoadDiv.appendChild(BarBorder);

	// Setting first message in the loading box
	//INTERFACE_SetLoadPhrase(UTILS_GetText("login_load_start"), 1);

	return {LoadDiv:LoadDiv, LoadLabel:LoadingLabel, BarBorder:BarBorder}
}



/** 
* Show load screen
*/ 
function INTERFACE_ShowLoad()
{
	if(this.LoadDiv.parentNode != document.body)
	{
		document.body.appendChild(this.LoadDiv);
	}
	this.LoadDiv.style.display = "block";
}

/** 
* Hide load screen
*/ 
function INTERFACE_HideLoading()
{
	this.LoadDiv.style.display = "none";
}

/** 
* Remove load screen
*/ 
function INTERFACE_RemoveLoad()
{
	this.LoadDiv.parentNode.removeChild(this.LoadDiv);
}

/** 
* Show wich files will be load
*/ 
function INTERFACE_SetLabel(Str)
{
	this.LoadLabel.innerHTML = Str;
}

/**
* Show phrase in the load box and paint next ball
*/
/*
function INTERFACE_SetLoadPhrase(Phrase, Num)
{
	var Node = document.getElementById('LoadingLabel');
	var Ball = document.getElementById('LoadingBall'+Num);

	// Setting phrease
	if (Node)
	{
		Node.innerHTML = Phrase;
		
		// Changing ball color
		if (Ball)
		{
			Ball.className = "green_ball";
		}
		return Phrase;
	}
	else
	{
		return null;
	}
}
*/
/************************
 * Loading Bar Object
 ************************/

function LoadingBar()
{
	// Attributes
	this.MaxValue = 300;
	this.CurrentValue = 0;
	this.ProgressBar = INTERFACE_CreateBar("Bar");

	//methods
	this.add = INTERFACE_AddValue;
	this.sub = INTERFACE_SubValue;
	this.show = INTERFACE_ShowLoadingBar;
	this.hide = INTERFACE_HideLoadingBar;
	this.remove = INTERFACE_RemoveLoadingBar;
}

function INTERFACE_CreateBar(Id)
{
	var Bar = document.createElement("div");

	Bar.id = Id;

	/*
	Bar.style.height = "15px";
	Bar.style.width = "0px";
	Bar.style.backgroundColor = "#000";
	Bar.style.position = "relative";
	*/

	return Bar;
}

function INTERFACE_AddValue(Num)
{
	if((this.CurrentValue + Num) > this.MaxValue)
	{
		this.CurrentValue = this.MaxValue;
	}
	else
	{
		this.CurrentValue += Num;
	}

	this.ProgressBar.style.width = this.CurrentValue+"px";
}

function INTERFACE_SubValue(Num)
{
	if((this.CurrentValue - Num) < 0)
	{
		this.CurrentValue = 0;
	}
	else
	{
		this.CurrentValue -= Num;
	}

	this.ProgressBar.style.width = this.CurrentValue+"px";
}

function INTERFACE_ShowLoadingBar(Element)
{
	if(Element != null)
	{
		Element.appendChild(this.ProgressBar);
	}
	else
	{
		document.body.appendChild(this.ProgressBar);
	}

	this.ProgressBar.style.display = "block";
}

function INTERFACE_HideLoadingBar(Element)
{
	this.ProgressBar.style.display = "none";
}

function INTERFACE_RemoveLoadingBar()
{
	this.ProgressBar.parendNode.removeChild(this.ProgressBar);
}
