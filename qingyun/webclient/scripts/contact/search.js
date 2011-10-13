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
* Handle search user
*/


/**
* Handle search user
*
* @param XML XML received from jabber server
* @return
* @author	Danilo
*/
function CONTACT_HandleSearchUser(XML)
{
	var Result, Fields;
	var j;

	Result = new Array ();

	{
		// Search jid field in itens and get the value and insert it in array
		{
			Fields = XML.getElementsByTagName("user");
			for (j=0; j < Fields.length; j++)
			{
				{
					Result.push((Fields[j].getAttribute("jid").split("@")[0]));
				}
			}
		}
	}

	// Display the result
	WINDOW_SearchUserResult(Result);

	return "";
}
