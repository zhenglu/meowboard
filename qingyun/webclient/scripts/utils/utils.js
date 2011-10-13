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
*/


/**
* Utils for webclient
*/


/**********************************
 * FUNCTIONS - XML SEARCH
 ************************************/

/**
* Identify client web browser
*/

function UTILS_IdentifyBrowser()
{
	var BrowserValue;
	var BrowserName=navigator.appName;


	// Firefox, Mozilla, Opera, etc.
	if (BrowserName.match("Netscape"))
	{
		/*
		// Code from:
		// http://www.javascriptkit.com/javatutors/navigator.shtml
		if ((/Firefox[\/\s](\d+\.\d+)/.test(navigator.userAgent)) ||
		   (/Iceweasel[\/\s](\d+\.\d+)/.test(navigator.userAgent)))
		{
			//test for Firefox/x.x or Firefox x.x (ignoring remaining digits);
			// capture x.x portion and store as a number
			var ffversion=new Number(RegExp.$1)
			if (ffversion>=3)
			{
				BrowserValue = 2;
			}
			else
			{
				BrowserValue = 1;
			}
		}
		// Quick fix to detect epiphany with gecko 1.9;
		else if (/Epiphany[\/\s](\d+\.\d+)/.test(navigator.userAgent)||
		        (/Galeon[\/\s](\d+\.\d+)/.test(navigator.userAgent)))
		*/ 
		if (
		   (/Firefox[\/\s](\d+\.\d+)/.test(navigator.userAgent)) ||
		   (/Iceweasel[\/\s](\d+\.\d+)/.test(navigator.userAgent))||
		   (/Epiphany[\/\s](\d+\.\d+)/.test(navigator.userAgent)) ||
		   (/Galeon[\/\s](\d+\.\d+)/.test(navigator.userAgent))
		   )
		{
			var geckoVersion;
			var UserAgent = navigator.userAgent.split(" ");
			var i=0;

			while((!UserAgent[i].match("rv:")) && ( i < UserAgent.length))
			{
				i++;
			}

			if(i != UserAgent.length)
			{
				geckoVersion = UserAgent[i].split(":")[1];
				if (geckoVersion.match("1.9"))
				{
					BrowserValue = 2;
				}
				else
				{
					BrowserValue = 1;
				}
				//alert(UserAgent[i] +"\n"+i+"\n"+geckoVersion+"\n"+BrowserValue);
			}
			else
			{
				alert("User agent without Gecko version.");
			}
		}
		else
		{
			BrowserValue = 1;
		}
			
	}
	// Internet Explorer
	else if (BrowserName.match("Microsoft Internet Explorer"))
	{
		/*
		var IEv = navigator.userAgent.split(";")[1].replace(/ /g,"");
		
		if (IEv == "MSIE6.0")
			BrowserValue = 0;
		else if (IEv == "MSIE7.0")
			BrowserValue = 3;
		else
			BrowserValue = 3;
		*/
		BrowserValue = 0;
	}
	// Other
	else
	{
		alert("Seu navegador pode não funcionar corretamente nesse site");
		BrowserValue = -1;
	}

	return BrowserValue;
}

/**
* Open a XML file and return XML DOM Tree
*/
function UTILS_OpenXMLFile(Url)
{	
	var XML, Parser;

	// Code for IE
	if (window.ActiveXObject)
	{
		XML = new ActiveXObject("Microsoft.XMLDOM");
	}
	// Code for Mozilla, Firefox, Opera, etc.
	else if (document.implementation && document.implementation.createDocument)
	{
		XML = document.implementation.createDocument("","",null);
	}
	else
	{
		alert('Seu navegador nao suporta XML DOM.');
	}

	XML.async = false;
	XML.load(Url);

	return(XML);
}


/**
* Return content of param
*/
function UTILS_GetTag(XML, TagName)
{
	var Node = XML.getElementsByTagName(TagName);

	// If dont find any tag
	if (Node == null)
	{
		return null;
	}
	// Return only first match
	else
	{
		Node = Node[0];
	}

	return UTILS_GetNodeText(Node);
}


/**
* Get Text for internacionalization
*/
function UTILS_GetText(TagName)
{
	return UTILS_GetTag(MainData.GetText, TagName);
}


/**
* Get param name for any browser
*/
function UTILS_GetNodeText(Node)
{
	if (!Node)
		return null;

	// Internet Explorer
	if (Node.text)
	{
		return Node.text;
	}
	// Mozilla, firefox, galeon
	else
	{
		return Node.textContent;
	}
}


/**********************************
 * FUNCTIONS - ELEMENT MANIPULATION
 ************************************/

function UTILS_CreateElement(Element, Id, ClassName, Inner)
{
	try
	{
		var Node = document.createElement(Element);
	}
	catch(e)
	{
		return null;
	}

	if (Id != null)
		Node.id = Id;

	if (ClassName != null)
		Node.className = ClassName;

	if (Inner != null)
		Node.innerHTML = Inner;

	return Node;
}


/**********************************
 * FUNCTIONS - COOKIE MANIPULATION
 ************************************/

/**
* Create cookies
*/
function UTILS_CreateCookie(CookieName, CookieValue, Days)
{
	var Expires, Data;

	if (Days)
	{
		Data = new Date();
		Data.setTime(Data.getTime()+(Days*24*60*60*1000));
		Expires = "; expires="+Data.toGMTString();
	}
	else 
		Expires = "";

	document.cookie = CookieName+"="+CookieValue+Expires;
}

/**
* Read cookies
*/
function UTILS_ReadCookie(CookieName)
{
	var Cookies = document.cookie.split("; ");

	for (var i=0; i<Cookies.length; i++)
	{
		if (Cookies[i].search(CookieName) != -1)
			return Cookies[i].replace(CookieName+"=","");
	}
	return "";
}

/**
* Erase cookies
*/
function UTILS_DeleteCookie(CookieName)
{
	UTILS_CreateCookie(CookieName,"",-1);
}

/**********************************
 * FUNCTIONS - VALIDATION
 ************************************/

/**
* Validate username
*/
function UTILS_ValidateUsername(Username)
{
	if (Username.match(/[^0-9a-z-_.]{1,}/))
	{
		return false;
	}
	else
	{
		return true;
	}
}

/**
* Capitalize a word
*/
function UTILS_Capitalize(Word)
{
	if (Word)
		return Word.charAt(0).toUpperCase() + Word.slice(1);
	else
		return "";
}

/**
* Put a <br /> tag at Obj.innerHTML if it pass the Width limit
*
* @param Obj
* 	Cell table's object
* @param Width
* 	max Base object 
* @return String
* @author Danilo Kiyoshi Simizu Yorinori
*
*/
function UTILS_BreakString(Obj, Width)
{
	var Text = Obj.innerHTML;
	var ObjWidth, TrWidth;
	var Broke = false;
	var Old;
	var i;

	// IE
	if (MainData.Browser == 0)
	{
		TrWidth = Width.Offset;
	}
	else // Other browsers
	{
		TrWidth = Width.Client;
	}

	if (Obj.clientWidth > TrWidth) {
		Obj.innerHMTL = "";

		for (i=0; i<=Text.length; i++)
		{
			if (Broke)
			{
				Old =Obj.innerHTML;
				Obj.innerHTML = Obj.innerHTML + Text.slice(i-1,i); 
			}
			else
			{
				Old = Obj.innerHTML;
				Obj.innerHTML = Text.slice(0,i);
			}

			if (Obj.clientWidth > TrWidth)
			{
				Obj.innerHTML = Old +"<br />" + Text.slice(i-1,i); 
				Broke = true;
			}
		}
	}
}

/**
* Short a word
*/
function UTILS_ShortString(Word, NumChars)
{
	var ShortWord;
	var NumChs;

	if(NumChars != null)
	{
		NumChs = NumChars;
	}
	else
	{
		NumChs = 5
	}

	if(Word.length > NumChs)
	{
		ShortWord = Word.slice(0,NumChs);
		ShortWord = ShortWord + "...";
	}
	else
	{
		ShortWord = Word;
	}
	return ShortWord;
}

/**********************************
 * FUNCTIONS - EVENT LISTENERS
 ************************************/

/**
* Add a Element event listener
* SRC = http://snipplr.com/view/561/add-event-listener/
* Cross-browser implementation of Element.addEventListener()
*/
function UTILS_AddListener(Element, Type, Expression, Bubbling)
{
	Bubbling = Bubbling || false;

	if (window.addEventListener) // Standard
	{
		Element.addEventListener(Type, Expression, Bubbling);
		return true;
	} 
	else if(window.attachEvent) // IE
	{
		Element.attachEvent('on' + Type, Expression);
		Element.cancelBubble = !(Bubbling);
		return true;
	} 
	else
	{ 
		return false;
	}
}

/**
* Remove an event listener
*/
function UTILS_RemoveListener(Element, Type, Expression, Bubbling)
{
	Bubbling = Bubbling || false;

	if (window.addEventListener) // Standard
	{
		Element.removeEventListener(Type, Expression, Bubbling);
		return true;
	} 
	else if(window.attachEvent) // IE
	{
		Element.detachEvent('on' + Type, Expression);
		Element.cancelBubble = !(Bubbling); // ??? TODO -> precisa tirar isso?
		return true;
	} 
	else
	{ 
		return false;
	}
}


/**********************************
 * FUNCTIONS - CROSS BROWSER EVENT
 ************************************/

function UTILS_ReturnEvent(event)
{
	if(MainData.Browser == 0) // IE
	{
		return window.event;
	}
	else //FIREFOX
	{
		return event;
	}
}

function UTILS_ReturnKeyCode(event)
{
	var KeyNum;

	if(MainData.Browser == 0) // IE
	{
		KeyNum = window.event.keyCode;
	}
	else // Netscape/Firefox/Opera
	{
		KeyNum = event.which;
	}
	return KeyNum;
}

/**********************************
 * FUNCTIONS - TIME CONVERSION
 ************************************/

/**
* Return time in format (XXhXX) from a given timestamp
* If timestamp is null, return current time
*/
function UTILS_GetTime(Timestamp)
{
	var Offset, Time, Hour, Min, Now, NewTime;


	Now = new Date();

	if (Timestamp)
	{
		Offset = Now.getTimezoneOffset()/60;
		Time = Timestamp.split("T")[1];
		Hour = (Time.split(":")[0] - Offset + 24) % 24;
		Min = Time.split(":")[1];

		NewTime = "("+Hour+"h"+Min+")";
	}
	else
	{
		NewTime = "("+Now.getHours()+"h";

		// Insert zero before minutes < 10
		if (Now.getMinutes() < 10)
		{
			NewTime += "0"+Now.getMinutes();
		}
		else
		{
			NewTime += Now.getMinutes();
		}
		NewTime += ")";
	}
	return NewTime;
}

/**
* Return the max_timestamp from rating to format(dd-mm-yyyy)
*
* @return	String
* @see		PROFILE_HandleRatings()
* @author	Danilo Yorinori
*/
function UTILS_ConvertTimeStamp(TimeStamp)
{
	var DateTime = TimeStamp.split("T")[0];
	var Year = DateTime.split("-")[0];
	var Month = DateTime.split("-")[1];
	var Day = DateTime.split("-")[2];

	return Day+"/"+Month+"/"+Year;

}

/**
* Return the date-time string to search old games 
* input format  (dd/mm/yyyy)
* output format (yyyy-mm-ddTnn:nn:nnZ)
*
* @param	TimeStamp
* 				Date string
* @param	Type
* 				begin or end
* @return	String
* @see		INTERFACE_SetSearchButton(Node)
* @author	Danilo Yorinori
*/
function UTILS_ConvertSearchDate(TimeStamp, Type)
{
	var Day, Month, Year;
	var SDate = "";

	if (TimeStamp == "") {
		return "";
	}
	else if (TimeStamp.match(/^\d{2}\/\d{2}\/\d{4}/g)==null) {
		return null;
	}

	Day = TimeStamp.split("/")[0];
	Month = TimeStamp.split("/")[1];
	Year = TimeStamp.split("/")[2];

	SDate += Year+"-"+Month+"-"+Day+"T";
	if (Type == "begin")
	{
		SDate +="00:00:00Z";
	}
	else if (Type == "end")
	{
		SDate +="23:59:59Z";
	}
	return SDate;
}

/************************************
 * FUNCTIONS - OBJECT OFFSETS       *
 ************************************/
/*
* Return object offsets (top and left)
*/
function UTILS_GetOffset(Obj)
{
	var Curleft, Curtop;

	if (Obj.offsetParent) 
	{
		Curleft = Obj.offsetLeft;
		Curtop = Obj.offsetTop;
		Obj = Obj.offsetParent;
		while (Obj)
		{
			Curleft += Obj.offsetLeft;
			Curtop += Obj.offsetTop;
			Obj = Obj.offsetParent;
		}
		return {X:Curleft, Y:Curtop};
	}
	else
	{
		return {X:0, Y:0};
	}
}

/**
* Get the first parent div in DOM tree
*/
function UTILS_GetParentDiv(Obj)
{
	do
	{
		if (Obj.tagName == "DIV")
			return Obj;
		Obj = Obj.parentNode;
	}
	while(Obj);
	return null;
}


////HORIZONTAL INDEX CONVERT
//If CharNum is char return respective number
//If CharNum is number return respective char
/*1 =a, ... 9 = i*/
function UTILS_HorizontalIndex(CharNum)
{
	var Row = new Array();
	var i=1;

	Row[1] = 'a';
	Row[2] = 'b';
	Row[3] = 'c';
	Row[4] = 'd';
	Row[5] = 'e';
	Row[6] = 'f';
	Row[7] = 'g';
	Row[8] = 'h';
	Row[9] = 'i';

	if(isNaN(CharNum)) //Char
	{
		while(i <= 9)
		{
			if(Row[i] == CharNum)
			{
				return i;
			}
			i++;
		}
		return null;
	}
	else //number
	{
		if(( CharNum > 0) && (CharNum <= 9) )
		{
			return Row[CharNum];
		}
		else
		{
			return null;
		}
	}
}


/**
* Convert a string board to a array (10x9) board
*
* @return Array x Array (10x9) of char
* @private
*/
function UTILS_String2Board(BoardString)
//BoardString is a array of char that contains chess board
{
        var Lin1, Lin2, Lin3, Lin4, Lin5, Lin6, Lin7, Lin8, Lin9, Lin10;
        var tmpArray;

        BoardString = BoardString.replace(/1/g,"-");
        BoardString = BoardString.replace(/2/g,"--");
        BoardString = BoardString.replace(/3/g,"---");
        BoardString = BoardString.replace(/4/g,"----");
        BoardString = BoardString.replace(/5/g,"-----");
        BoardString = BoardString.replace(/6/g,"------");
        BoardString = BoardString.replace(/7/g,"-------");
        BoardString = BoardString.replace(/8/g,"--------");
        BoardString = BoardString.replace(/9/g,"---------");

/* FIXME: 9- means 9 slashes*/

        tmpArray = BoardString.split("/",10);

        Lin1 = tmpArray[0];
        Lin2 = tmpArray[1];
        Lin3 = tmpArray[2];
        Lin4 = tmpArray[3];
        Lin5 = tmpArray[4];
        Lin6 = tmpArray[5];
        Lin7 = tmpArray[6];
        Lin8 = tmpArray[7];
        Lin9 = tmpArray[8];
        Lin10 = tmpArray[9];

        var Board = new Array(Lin1, Lin2, Lin3, Lin4, Lin5, Lin6, Lin7, Lin8, Lin9, Lin10);

        return Board;
}


/**
* Return apropriate game type text according to game type string 
*
* @param game type's string
* @return String
* @see OLDGAME_HandleSearchOldGame
* @author zhuzhenglu
*/
function UTILS_GetGameCategory(Type)
{
	if ((Type == "") || (Type == null) || (Type == undefined))
		return "";
	else if (Type == "standard"||Type == "Standard")
		return UTILS_GetText("game_type_standard");
	else if (Type == "blitz"||Type == "Blitz")
		return UTILS_GetText("game_type_blitz");
	else if (Type == "lightning"||Type == "Lightning")
		return UTILS_GetText("game_type_lightning");
	else if (Type == "untimed"||Type == "Untimed")
		return UTILS_GetText("game_type_untimed");
	else
		return null;
}


/************************************
 * FUNCTIONS - CONVERT STRING       *
 ************************************/

/**
* Convert a chat string to a format that can't be interpretated
*
* @param 	str is string
* @return 	string with '<' , '>', '&' and '"' replaced
* @author	Rubens
*/
function UTILS_ConvertChatString(Str)
{
	var StrTmp;

	StrTmp = Str.replace(/&/g,"&amp;");
	StrTmp = StrTmp.replace(/</g,"&lt;");
	StrTmp = StrTmp.replace(/>/g,"&gt;");
	StrTmp = StrTmp.replace(/"/g,"&quot;");

	return StrTmp;
}

/**
* Convert default lang to jabber lang
*/
function UTILS_JabberLang(DefaultLang)
{
	if (DefaultLang)
		return DefaultLang.substr(0, 2)+"-"+DefaultLang.substr(3, 5).toLowerCase();
	else
		return "";
}

/*
* Convert time in seconds to days, hours, minuts.
*
* @param 	time number
* @return 	time formated in string
* @author	Rubens
*/
function UTILS_ConvertTime(Seconds)
{
	var Day, Month, Year;
	var Sec, Min, Hour;
	var TimeFormat = "";

	Sec = Seconds % 60;
	Min = Math.floor(Seconds / 60) % 60;
	Hour  = Math.floor(Seconds / 3600) % 24;

	Day = Math.floor(Seconds / (3600*24)) % 30;
	Month = Math.floor(Seconds / (3600*24*30)) % 12;
	Year = Math.floor(Seconds / (3600*24*30*12));

	// Concat Years
	if(Year != 0)
	{
		TimeFormat += Year + UTILS_GetText("profile_year");
	}
	// Concat Months
	if(Month != 0)
	{
		TimeFormat += " "+Month + UTILS_GetText("profile_month");
	}
	// Concat Days
	if(Day != 0)
	{
		TimeFormat += " "+Day + UTILS_GetText("profile_day");
	}


	// Concat hour
	if(Hour != 0)
	{
		TimeFormat +=" ";
		if(Hour < 10)
		{
			TimeFormat += "0";
		}
		TimeFormat += Hour+":";
	}
	else
	{
		TimeFormat += " 00:" ;
	}


	// Concat minutes
	if(Min != 0)
	{
		if(Min < 10)
		{
			TimeFormat += "0";
		}
		TimeFormat += Min+":";
	}
	else
	{
		TimeFormat += "00:" ;
	}

	
	// Concat seconds
	if(Sec != 0)
	{
		if(Sec < 10)
		{
			TimeFormat += "0";
		}
		TimeFormat += Sec;
	}
	else
	{
		TimeFormat += "00" ;
	}

	return TimeFormat;
}

/*
* Disable selection text inside a element
*
* @param 	Element	HTML elements
* @return 	False	Aways return false to disable seletion
* @author	Rubens Suguimoto
*/

/* Code from:
*http://ajaxcookbook.org/disable-text-selection/
*/
function UTILS_DisableSelection(Element)
{
	// IE disable selection method
	Element.onselectstart = function() {
		return false;
	};

	Element.unselectable = "on";
	Element.style.MozUserSelect = "none";
	Element.style.cursor = "default";
	
	return false;
}
/************************************
 * FUNCTIONS - SORT FUNCTIONS       *
 ************************************/

/**
* Use to sort Userlist into ascendent order
* If x < y return -1
*    x > y return  1
*    x = y return  0
*
* @return integer	
* @author Danilo Yorinori
*/
function UTILS_SortByUsernameAsc(a, b) 
{
	var x = a.Username.toLowerCase();
	var y = b.Username.toLowerCase();
	return ((x < y) ? -1 : ((x > y) ? 1 : 0));
}

/**
* Use to sort Userlist into descendent order
* If x < y return  1
*    x > y return -1
*    x = y return  0
*
* @return integer	
* @author Danilo Yorinori
*/
function UTILS_SortByUsernameDsc(a, b) 
{
	var x = a.Username.toLowerCase();
	var y = b.Username.toLowerCase();
	return ((x < y) ? 1 : ((x > y) ? -1 : 0));
}

/**
* Use to sort Userlist into descendent order by Rating
* If x < y return  1
*    x > y return -1
*    x = y return  0
*
* @return integer	
* @author Danilo Yorinori
*/
function UTILS_SortByRatingDsc(a, b) 
{
	var Type = UTILS_Capitalize(MainData.CurrentRating);
	var x;
	var y;

	if (Type == "Lightning")
	{
		if (a.Rating.Lightning != undefined)
		{
			x = parseInt(parseFloat(a.Rating.Lightning));
		}
		else
		{
			x = 0;
		}
		if (b.Rating.Lightning != undefined)
		{
			y = parseInt(parseFloat(b.Rating.Lightning));
		}
		else
		{
			y = 0;
		}
	}
	else if (Type == "Blitz")
	{
		if (a.Rating.Blitz != undefined)
		{
			x = parseInt(parseFloat(a.Rating.Blitz));
		}
		else
		{
			x = 0;
		}
		if (b.Rating.Blitz != undefined)
		{
			y = parseInt(parseFloat(b.Rating.Blitz));
		}
		else
		{
			y = 0;
		}
	}
	if (Type == "Standard")
	{
		if (a.Rating.Standard != undefined)
		{
			x = parseInt(parseFloat(a.Rating.Standard));
		}
		else
		{
			x = 0;
		}
		if (b.Rating.Standard != undefined)
		{
			y = parseInt(parseFloat(b.Rating.Standard));
		}
		else
		{
			y = 0;
		}
	}
	return ((x < y) ? 1 : ((x > y) ? -1 : 0));
}

/**
* Use to sort Rooms's userlist into descendent order by Rating
* If x < y return  1
*    x > y return -1
*    x = y return  0
*
* @return integer	
* @author Danilo Yorinori
*/
function UTILS_SortRoomByRatingDsc(a, b) 
{
	var Type = UTILS_Capitalize(MainData.RoomCurrentRating);
	var x;
	var y;

	if (Type == "Lightning")
	{
		if (a.Rating.Lightning != undefined)
		{
			x = parseInt(parseFloat(a.Rating.Lightning));
		}
		else
		{
			x = 0;
		}
		if (b.Rating.Lightning != undefined)
		{
			y = parseInt(parseFloat(b.Rating.Lightning));
		}
		else
		{
			y = 0;
		}
	}
	else if (Type == "Blitz")
	{
		if (a.Rating.Blitz != undefined)
		{
			x = parseInt(parseFloat(a.Rating.Blitz));
		}
		else
		{
			x = 0;
		}
		if (b.Rating.Blitz != undefined)
		{
			y = parseInt(parseFloat(b.Rating.Blitz));
		}
		else
		{
			y = 0;
		}
	}
	if (Type == "Standard")
	{
		if (a.Rating.Standard != undefined)
		{
			x = parseInt(parseFloat(a.Rating.Standard));
		}
		else
		{
			x = 0;
		}
		if (b.Rating.Standard != undefined)
		{
			y = parseInt(parseFloat(b.Rating.Standard));
		}
		else
		{
			y = 0;
		}
	}
	if (Type == "Untimed")
	{
		if (a.Rating.Untimed != undefined)
		{
			x = parseInt(parseFloat(a.Rating.Untimed));
		}
		else
		{
			x = 0;
		}
		if (b.Rating.Untimed != undefined)
		{
			y = parseInt(parseFloat(b.Rating.Untimed));
		}
		else
		{
			y = 0;
		}
	}

	return ((x < y) ? 1 : ((x > y) ? -1 : 0));
}

/************************************
 * FUNCTIONS - BROWSER LANGUAGE     *
 ************************************/
/*
Code from:
http://www.criarweb.com/faq/conseguir_idioma_navegador_cliente.html
*/

/* Detect browser language
 *
 * @params void
 * @return Language string (i.e.: pt-BR, en-US, etc.)
 */
function UTILS_GetLanguage()
{
	var Lang
	
	Lang = navigator.browserLanguage;
	if (navigator.userAgent.indexOf("Opera")!=-1)
	{
		Lang=navigator.language;
	}
	else if (navigator.appName == "Netscape")
	{
		Lang=navigator.language;
	}
	else
	{
		Lang=navigator.browserLanguage;
	}

	switch(Lang)
	{
		case "pt-BR":
			return "pt_BR";

		case "en-US":
			return "en_US";

		case "zh-CN":
			return "zh_CN";

		default:
			return "zh_CN";
	}
}


