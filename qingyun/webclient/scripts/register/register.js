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
* Register window functions
*/


var HttpRequest;

//Necessary to persist the RID and SID
var RegisterData;


/*
* Register Object with Host, SID and RID
* @parameter	void
* @return	void
* @author	Fabiano
*/
function REGISTER_RegisterData(){
/*FIXME: change back 	this.Host = "localhost"; */
/**
	FIXME: Should keep this host the same as that in the ejabberd.cfg 
			or the host in the ejabberd.cfg should keep the same as this one.
*/
	this.Host = REGISTER_GetHost();
	this.SID = "";
	this.RID = parseInt(Math.random()*1000000000);
}

/*
* Create XMPP Bosh message
* @parameter	Msg is XMPP message
* @return	XMPP Bosh
* @author	Fabiano
*/
function REGISTER_MakeXMPP(Msg){

	var XMPP = "<body rid='"+RegisterData.RID+"' sid='"+RegisterData.SID+"' xmlns='http://jabber.org/protocol/httpbind'>"+Msg+"</body>";

	RegisterData.RID++;

	return XMPP;
}

/*
* Validate fields and send message to open a stream for new register
* @parameter	void
* @return	void
* @author	Fabiano
*/
function REGISTER_Post()
{

	var User, Mail, Pwd, ConfPwd;
	var Valid;
	var Msg;

	Pwd = document.getElementById("pwd").value;
	ConfPwd = document.getElementById("confpwd").value;
	Mail = document.getElementById("mail").value;
	User = document.getElementById("username").value;
	Valid = REGISTER_DateValidate(User, Mail, Pwd, ConfPwd);

	
	if(Valid > 0)
	{
		alert(REGISTER_GetError(Valid));
		return false;
	}


	RegisterData = new REGISTER_RegisterData();

   	Msg = "<body hold='1' rid='"+RegisterData.RID+"' to='"+RegisterData.Host+"' ver='1.6' wait='10' xml:lang='en' xmlns='http://jabber.org/protocol/httpbind'/>";
	REGISTER_SendData(Msg);
}


/*
* Get error in register user 
* @parameter	var err int error code
* @return 	string error description
* @author	Fabiano
*/

function REGISTER_GetError(err)
{
	var XML = UTILS_OpenXMLFile(REGISTER_GetLanguage(window.location.href));
	switch(err)
	{
		case 1: 
			return UTILS_GetTag(XML, "register_invalid_user_name");
		case 2:
			return UTILS_GetTag(XML, "register_invalid_mail");
		case 3:
			return UTILS_GetTag(XML, "register_invalid_password");
		case 4:
			return UTILS_GetTag(XML, "register_invalid_user_name_len");
	}
}

/*
* Validate date before send
* @parameter	User - user name 
* @parameter	Mail - user mail
* @parameter	Pwd - user password
* @parameter	ConfPW - user password confirmation
* @return	0 is sucess or error code
* @author	Fabiano
*/

function REGISTER_DateValidate(User, Mail, Pwd, ConfPW)
{
	//RE = regular expression	
	var REMail = /^[A-Za-z0-9_\-\.]+@[A-Za-z0-9_\-\.]{2,}\.[A-Za-z0-9]{2,}(\.[A-Za-z0-9])?/;
	//var REUsername = /^\w{1,}$/
	var REUsername = /^[a-z0-9-._]+$/

	if(!REUsername.test(User))
	{
		return 1;
	}

	if (User.length > 14)
	{
		return 4;
	}

	if(Mail.length > 0)
	{
		if(!REMail.test(Mail))
		{
			return  2;
		}
	}

	if(!(Pwd == ConfPW && Pwd.length > 3))
	{
		return 3;
	}

	return 0;
}
 


/*
* Send data to register in jabber
* @params	Msg is XMPP message that contais register instructions
* @author	Fabiano
*/

function REGISTER_SendData(Msg)
{

// Create XMLHttpRequest
	if (window.XMLHttpRequest)
	{
		// Mozilla, Opera, Galeon
		HttpRequest = new XMLHttpRequest();

		if (HttpRequest.overrideMimeType)
		{
			HttpRequest.overrideMimeType("text/xml");
		}
	}
	else if (window.ActiveXObject)
	{
		// Internet Explorer
		try
		{
			HttpRequest = new ActiveXObject("Msxml2.XMLHTTP");
		}
		catch(e)
		{
			HttpRequest = new ActiveXObject("Microsoft.XMLHTTP");
		}
	}

	// Avoid browser caching
	DT = Math.floor(Math.random()*10000);

	HttpRequest.open('POST', 'http://'+RegisterData.Host+'/jabber?id='+DT , true);
	HttpRequest.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");

	// ReceiveXml is used to register direct in Jabber, but now
	// this feature is not implemented
	HttpRequest.onreadystatechange = REGISTER_ReceiveXml;

	// Send request to server
	HttpRequest.send(Msg)

}



/**
* Check and parse the XMPP register response
* @public
* @return 	none
* @author	Fabiano
*/
function REGISTER_ReceiveXml()
{
	var XmlDoc;

	if (HttpRequest.readyState == 4)
	{
		if (HttpRequest.status == 200)
		{
			XmlDoc = HttpRequest.responseXML;
			REGISTER_ProcessMessage(XmlDoc);
		}
	}
}

/**
* Process the XMPP register response
* @public
* @return 	none
* @author	Fabiano
*/
function REGISTER_ProcessMessage(XmlDoc){

	var Msg, Uname, Passwd, TestBody, TestIq, RID, SID;
		
	TestIq = XmlDoc.getElementsByTagName("iq");
	TestBody = XmlDoc.getElementsByTagName("body");

	if(TestBody[0].getAttribute("condition"))
	{
		return;
	}


	// Open a stream to jabber
	if(TestIq.length == 0)
	{
		RegisterData.SID = XmlDoc.getElementsByTagName("body")[0].getAttribute("sid");
		Msg = "<iq type='get' id='reg1' to='"+RegisterData.Host+"'><query xmlns='jabber:iq:register'/></iq>";
		Msg = REGISTER_MakeXMPP(Msg);
		REGISTER_SendData(Msg);
	}
	// Send register data to jabber
	else if(TestIq[0].getAttribute("id") == "reg1")
	{
			Uname = "<username>"+document.getElementById("username").value+"</username>";
			Passwd = "<password>"+document.getElementById("pwd").value+"</password>";
			Msg = "<iq type='set' id='reg2'><query xmlns='jabber:iq:register'>"
			Msg += Uname + Passwd + "</query></iq>";
			Msg = REGISTER_MakeXMPP(Msg);
			REGISTER_SendData(Msg);
	}
	// Receive final response from server. Check if sucess or errors
	else if(TestIq[0].getAttribute("id") == "reg2")
	{
		if(XmlDoc.getElementsByTagName("error").length > 0)
		{
			REGISTER_ParseError(XmlDoc.getElementsByTagName("error")[0].getAttribute("code"));
		}
		else
		{
			REGISTER_SucessMessage();
			window.location=(".");
		}
	}
}

/**
* Show a alert box to sucess register
* @public
* @return 	none
* @author	Fabiano
*/
function REGISTER_SucessMessage(){

	var XML = UTILS_OpenXMLFile(REGISTER_GetLanguage(window.location.href));

	alert(UTILS_GetTag(XML, "register_sucess_message"));


}

/**
* Show a alert box to error in register
* @public
* @return 	none
* @author	Fabiano
*/
function REGISTER_ParseError(code){

	var XML = UTILS_OpenXMLFile(REGISTER_GetLanguage(window.location.href));

	switch(code)
	{
		case "409":
			alert(UTILS_GetTag(XML, "register_error_409"));
			break;
	}

}

/*
* get language parameter
* @parameter	string page url
* @return	string language xml file
* @author	Fabiano
*/
function REGISTER_GetLanguage(URL)
{
	var QString = URL.split('?');

	if(QString[1])
	{
		QString = URL.split('=');
		if(QString[1])
		{
			return "./scripts/lang/"+QString[1];
		}
	}

	return "./scripts/lang/zh_CN.xml"
}

/*
* Get host from configuration file
* @parameter	void
* @return	Host string
* @author	Fabiano
*/
function REGISTER_GetHost(){
	
	return this.Host = window.location.href.split("/")[2].split(":")[0];
//	var CONF = UTILS_OpenXMLFile("scripts/data/conf.xml");
//	return UTILS_GetTag(CONF, "host");

}

/*
* Change fields label language
* @parameter	void
* @return	void
* @author	Fabiano
*/
function REGISTER_Labels()
{

	var XML = UTILS_OpenXMLFile(REGISTER_GetLanguage(window.location.href));
	var Inf = document.getElementById("register_inf");
	var Login = document.getElementById("register_login");
	var MaxChar = document.getElementById("register_max_char");
	var UserInfo = document.getElementById("register_user_inf");
	var Mail1 = document.getElementById("register_mail1");
	var Mail2 = document.getElementById("register_mail2");
	var MailInf = document.getElementById("register_mail_inf");
	var Pass = document.getElementById("register_passwd");
	var PassConf1 = document.getElementById("register_passwd_confirm1");
	var PassConf2 = document.getElementById("register_passwd_confirm2");
	var Title =  document.getElementById("register_title");
	var BtConfirm = document.getElementById("register");
	var BtCancel = document.getElementById("cancel");

	Inf.innerHTML = UTILS_GetTag(XML, "register_inf");
	Login.innerHTML = UTILS_GetTag(XML, "register_login");
	MaxChar.innerHTML = UTILS_GetTag(XML, "register_max_char");
	Login.appendChild(MaxChar);
	Login.innerHTML += ":";

	UserInfo.innerHTML = UTILS_GetTag(XML, "register_user_inf");

	Mail1.innerHTML = UTILS_GetTag(XML, "register_mail1");
	Mail2.innerHTML = UTILS_GetTag(XML, "register_mail2");
	Mail1.appendChild(Mail2);
	Mail1.innerHTML += ":";

	MailInf.innerHTML = UTILS_GetTag(XML, "register_mail_inf");

	Pass.innerHTML = UTILS_GetTag(XML, "register_passwd");
	PassConf1.innerHTML = UTILS_GetTag(XML, "register_passwd_confirm1");
	PassConf2.innerHTML = UTILS_GetTag(XML, "register_passwd_confirm2");
	PassConf1.appendChild(PassConf2);
	PassConf1.innerHTML +=":";

	Title.innerHTML = UTILS_GetTag(XML, "register_title");

	BtConfirm.value = UTILS_GetTag(XML, "register_bt_confirm");
	BtCancel.value = UTILS_GetTag(XML, "window_cancel");


}

/**
 * @brief	Align main div in middle
 * @author	Rubens
 */

function REGISTER_VerticalAlignMiddle()
{
	var MainDiv = document.getElementById("mainDiv");

	var WindowHeight = window.innerHeight;

	if(WindowHeight > 500)
	{
		MainDiv.style.top = ((WindowHeight / 2) - 200) + "px";
	}
}

/**************************************
***************** PHP *****************
**************************************/
/**
* Send register data to a PHP file (used if jabber don't support user register)
* @return none
* @public
* @author	Fabiano
*
* @deprecated
*/
function REGISTER_SendDataPHP(User, Mail, Pwd)
{

	var Post = "username="+User+"&mail="+Mail+"&pwd="+Pwd;
	// Create XMLHttpRequest
	if (window.XMLHttpRequest)
	{
		// Mozilla, Opera, Galeon
		HttpRequest = new XMLHttpRequest();
		if (HttpRequest.overrideMimeType)
		{
			HttpRequest.overrideMimeType("text/xml");
		}
	}
	else if (window.ActiveXObject)
	{
		// Internet Explorer
		try
		{
			HttpRequest = new ActiveXObject("Microsoft.XMLHTTP");
		}
		catch(e)
		{
			HttpRequest = new ActiveXObject("Microsoft.XMLHTTP");
		}
	}

	// Avoid browser caching
	DT = Math.floor(Math.random()*10000);

	HttpRequest.open('POST','php/register.php?jabber?id='+DT , true);
	HttpRequest.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");

	HttpRequest.onreadystatechange = REGISTER_ReceiveXmlPHP;

	// Send request to server
	HttpRequest.send(Post);
}

/**
* Receive response from PHP file
* @return none
* @public
* @author	Fabiano
* @deprecated
*/
function REGISTER_ReceiveXmlPHP()
{
	var XmlDoc;
	var Sql;
	var User;

	if (HttpRequest.readyState == 4)
	{
		if (HttpRequest.status == 200)
		{
			XmlDoc = HttpRequest.responseXML;
			Sql = XmlDoc.getElementsByTagName("sql_result")[0].childNodes[0].nodeValue;
			User = XmlDoc.getElementsByTagName("username")[0].childNodes[0].nodeValue;
			if(Sql != "ok")
			{
				alert(REGISTER_GetDatabaseError(Sql));
			}
			else
			{	
				window.location=(".");
			}
		}
	}
}

/**
* Show a alert with error
* @return none
* @public
* @author	Fabiano
* @deprecated
*/
function REGISTER_GetDatabaseError(Msg)
{
	var ErrMsg = /violates unique constraint/

	if(ErrMsg.test(Msg))
	{
		return UTILS_GetText("register_error_409");
	}
	return Msg;
}


