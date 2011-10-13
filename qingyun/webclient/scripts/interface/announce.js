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
* Create elements to announce a match 
*
* @return									Div; Array
* @author									Danilo Kiyoshi Simizu Yorinori
*/
function INTERFACE_AnnounceWindow()
{
	var Div;

	var Layer1Div;
	var L1LeftDiv;
	var ColorLabel, ColorOptW,BrW, ColorOptWImg, ColorOptB, ColorOptBImg,BrB, AutoColorOpt, AutoColorLabel, RandomColorOptImg, BrR;
	var L1RightDiv;
	var CatLabel, CatSelect, CatOptLi, CatOptBl, CatOptSt;
	var Br1;

	var Layer2Div;
	var L2LeftDiv;
	var TimeLabel, TimeSelect, TimeOpt, TimeLabelMin,TimeBr;
	var L2RightDiv;
	var IncLabel, IncSelect, IncOpt, IncLabelSeg,IncBr;
	var Br2;

	var Layer3Div;
	var L3RightDiv, L3LeftDiv;
	var RatingCheckbox, RatingLabel;
	var PrivateCheckbox, PrivateLabel;
	var AutoFlagCheckbox, AutoFlagLabel;
	var Br3, Br4, Br5, Br6;
	var IntervalCheckbox, IntervalLabel, FromLabel, FromInput, ToLabel, ToInput;
	var FromDiv, ToDiv;
	var FormatLabel1,FormatLabel2;

	var ButtonsDiv;
	var Cancel, Announce;
	
	var Buttons = new Array();

	var Type, Color, Rated;
	var i; 

	// Main Div
	Div = UTILS_CreateElement('div', 'AnnounceDiv');
	
	// Layer1 Elements
	Layer1Div = UTILS_CreateElement('div', 'Layer1Div');

	// Layer 1 Left Elements
	L1LeftDiv = UTILS_CreateElement('div', 'L1LeftDiv','leftDiv');

	ColorLabel = UTILS_CreateElement('p',null,null,UTILS_GetText('announce_pieces'));

	try
	//Fix radio button for IE
	{
		ColorOptW = document.createElement('<input class="radio" type="radio" name="color" value="colorW"  />');
	}
	catch(err)
	{ //FF
		ColorOptW =	UTILS_CreateElement('input',null);
		ColorOptW.type = "radio";
		ColorOptW.name = "color";
		ColorOptW.value = "colorW";
	}
	

	ColorOptWImg = UTILS_CreateElement('img',null,'color');
	ColorOptWImg.src = "images/invite_white_pawn.png";
	BrW = UTILS_CreateElement('br');

	try
	//Fix radio button for IE
	{
		ColorOptB = document.createElement('<input class="radio" type="radio" name="color" value="colorB" />');
	}
	catch(err)
	{ //FF
		ColorOptB = UTILS_CreateElement('input',null,'radio');
		ColorOptB.type = "radio";
		ColorOptB.name = "color";
		ColorOptB.value = "colorB";
	}

	ColorOptBImg = UTILS_CreateElement('img',null,'color');
	ColorOptBImg.src = "images/invite_black_pawn.png";
	BrB = UTILS_CreateElement('br');
	
	try
	//Fix radio button for IE
	{
		AutoColorOpt = document.createElement('<input class="radio" type="radio" name="color" value="auto" />');
	}
	catch(err)
	{ //FF
		AutoColorOpt = UTILS_CreateElement('input',null,'radio');
		AutoColorOpt.type = "radio";
		AutoColorOpt.name = "color";
		AutoColorOpt.value = "auto";
	}

	RandomColorOptImg = UTILS_CreateElement('img',null,'color');
	RandomColorOptImg.src = "images/random.png";
	BrR = UTILS_CreateElement('br');
	
	// Firefox fix
	AutoColorOpt.checked = true;
	//defaultChecked is used to fix IE radio checked
	AutoColorOpt.setAttribute("defaultChecked", "true");
	
	//*End Layer 1 Left Elements*
	
	// Layer 1 Right Elements
	L1RightDiv = UTILS_CreateElement('div', 'L1RightDiv');

	CatLabel = UTILS_CreateElement('p', null, null, UTILS_GetText('challenge_category'));
	CatOptLi = UTILS_CreateElement('option', null, null, UTILS_GetText("game_type_lightning"));
	CatOptLi.value = "lightning";
	CatOptBl = UTILS_CreateElement('option', null, null, UTILS_GetText("game_type_blitz"));
	CatOptBl.value = "blitz";
	CatOptSt = UTILS_CreateElement('option', null, null, UTILS_GetText("game_type_standard"));
	CatOptSt.value = "standard";
	CatSelect =	UTILS_CreateElement('select',null,'drop_select');

	CatSelect.appendChild(CatOptLi);
	CatSelect.appendChild(CatOptBl);
	CatSelect.appendChild(CatOptSt);

	CatSelect.onchange = function () 
	{
		Type = CatSelect.options.selectedIndex;
		i=0;
		
		// Remove todos os filhos
		while (TimeSelect.firstChild)
		{
			TimeSelect.removeChild (TimeSelect.firstChild);
		}
		
		// Lightning = 0 
		if (Type == 0)
		{
			for (i=1; i <= 2; i++)
			{
				TimeOpt = UTILS_CreateElement('option',null,null,i);
				TimeOpt.value = i;
				
				TimeSelect.appendChild(TimeOpt);
			}	
		}

		// Blitz = 1
		else if (Type == 1)
		{
			for (i=3; i <= 10; i++)
			{
				TimeOpt = UTILS_CreateElement('option',null,null,i);
				TimeOpt.value = i;
				
				TimeSelect.appendChild(TimeOpt);
			}	
		}

		// Standard = 2
		else if (Type == 2)
		{
			for (i=11; i <=30; i++)
			{
				TimeOpt = UTILS_CreateElement('option',null,null,i);
				TimeOpt.value = i;
				
				TimeSelect.appendChild(TimeOpt);
			}	
			TimeOpt = UTILS_CreateElement('option',null,null,"40");
			TimeOpt.value = 40;
			TimeSelect.appendChild(TimeOpt);
			TimeOpt = UTILS_CreateElement('option',null,null,"60");
			TimeOpt.value = 60;
			TimeSelect.appendChild(TimeOpt);
			TimeOpt = UTILS_CreateElement('option',null,null,"120");
			TimeOpt.value = 120;
			TimeSelect.appendChild(TimeOpt);
			TimeOpt = UTILS_CreateElement('option',null,null,"180");
			TimeOpt.value = 180;
			TimeOpt = UTILS_CreateElement('option',null,null,UTILS_GetText("challenge_notime"));
			TimeOpt.value = 190;
			TimeSelect.appendChild(TimeOpt);
		}
	}

	//* End Layer1 Right Elements*
	
	Br1= UTILS_CreateElement('br');

	//* End Layer1*

	// Layer 2 Elements
	Layer2Div = UTILS_CreateElement('div','Layer2Div');

	// Layer 2 Right Elements
	L2RightDiv = UTILS_CreateElement('div','L2RightDiv');
	IncLabel =	UTILS_CreateElement('p', null, null, UTILS_GetText('challenge_inc_label'));
	IncSelect = UTILS_CreateElement('select', null, 'drop_select');
	IncLabelSeg =	UTILS_CreateElement('span', null, 'italic', UTILS_GetText('challenge_inc_label_seg'));
	IncBr = UTILS_CreateElement('br');
	
	for (i=0; i < 30; i++)
	{
		IncOpt = UTILS_CreateElement("option", null, null, i);
		IncOpt.value = i;

		IncSelect.appendChild(IncOpt);
	}
	
	//* End Layer2 Right Elements*
	
	// L2 Left Elements
	L2LeftDiv = UTILS_CreateElement('div','L2LeftDiv','leftDiv');

	TimeLabel =	UTILS_CreateElement('p',null,null,UTILS_GetText('challenge_time_label'));
	TimeLabelMin =	UTILS_CreateElement('span',null,'italic',UTILS_GetText('challenge_time_label_min'));
	TimeBr = UTILS_CreateElement('br');
	TimeSelect = UTILS_CreateElement('select',null,'drop_select');
	
	for (i=11; i <= 30; i++)
	{
		TimeOpt = document.createElement("option");
		TimeOpt.innerHTML = i;
		TimeOpt.value = i;

		TimeSelect.appendChild(TimeOpt);
	}

	CatSelect.options.selectedIndex = 2;

	//* End Layer2 Left Elements*
	Br2= UTILS_CreateElement('br');
	// End Layer 2
	
	// Layer 3
	Layer3Div = UTILS_CreateElement('div','Layer3Div');
	L3LeftDiv = UTILS_CreateElement('div','L3LeftDiv','leftDiv');

	// Private
	try
	{
		PrivateCheckbox = document.createElement("<input class='rating_radio' type='checkbox' name='private' disabled='disabled' />");
	}
	catch(err)
	{
		PrivateCheckbox =	UTILS_CreateElement('input', null, 'rating_radio');
		PrivateCheckbox.type = "checkbox";
		PrivateCheckbox.name = "private";
		PrivateCheckbox.disabled = true;
	}
	PrivateLabel = UTILS_CreateElement('span',null,'cx',UTILS_GetText('challenge_private'));
	
	try
	{
		RatingCheckbox = document.createElement("<input class='rating_radio' checked='checked'  type='checkbox' name='rating'/>")
	}
	catch(err)
	{
		RatingCheckbox = UTILS_CreateElement('input',null,'rating_radio');
		RatingCheckbox.type = "checkbox";
		RatingCheckbox.name = "rating";
		RatingCheckbox.checked = true;
	}
	RatingLabel = UTILS_CreateElement('span',null,'cx',UTILS_GetText('challenge_rating'));

	// Auto-flag
	AutoFlagCheckbox = UTILS_CreateElement('input',null,'rating_radio');
	AutoFlagCheckbox.type = "checkbox";
	AutoFlagCheckbox.name = "autoflag";
	AutoFlagCheckbox.disabled = true;
	AutoFlagLabel = UTILS_CreateElement('span',null,'cx',UTILS_GetText('challenge_auto_flag'));

	Br3 = UTILS_CreateElement('br');
	Br4 = UTILS_CreateElement('br');
	
	L3RightDiv = UTILS_CreateElement('div','L3RightDiv');
	try
	{
		IntervalCheckbox = document.createElement("<input class='rating_radio' type='checkbox' name='interval' />");
	}
	catch(err)
	{
		IntervalCheckbox =	UTILS_CreateElement('input', null, 'rating_radio');
		IntervalCheckbox.type = "checkbox";
		IntervalCheckbox.name = "interval";
	}

	// Date Form
	FromDiv = UTILS_CreateElement('div','FromDiv', 'leftDiv');
	ToDiv = UTILS_CreateElement('div','ToDiv');
	IntervalLabel = UTILS_CreateElement('span',null,'cx',UTILS_GetText('announce_rating_interval'));

	FromLabel	= UTILS_CreateElement('span',null,'bold',UTILS_GetText('oldgame_from'));
	try
	{
		FromInput = document.createElement("<input class='disabled' type='text' maxLength='4' size='5' disabled='disabled' />");
	}
	catch(err)
	{
		FromInput	= UTILS_CreateElement('input',null,'disabled');
		FromInput.size = "5";
		FromInput.type = "text";
		FromInput.maxLength = "4";
		FromInput.disabled = true;
	}
	FormatLabel1 = UTILS_CreateElement('span',null,'format',UTILS_GetText('announce_min_rating_example'));

	ToLabel	= UTILS_CreateElement('span',null,'bold',UTILS_GetText('oldgame_to'));
	try
	{
		ToInput = document.createElement("<input class='disabled' type='text' maxLength='4' size='5' disabled='disabled' />");
	}
	catch(err)
	{
		ToInput	= UTILS_CreateElement('input',null,'disabled');
		ToInput.size = "5";
		ToInput.type = "text";
		ToInput.maxLength = "4";
		ToInput.disabled = true;
	}
	FormatLabel2 = UTILS_CreateElement('span',null,'format',UTILS_GetText('announce_max_rating_example'));

	Br5 = UTILS_CreateElement("br");
	Br6 = UTILS_CreateElement("br");

	IntervalCheckbox.onclick = function() {
		if (this.checked == true)
		{
			FromInput.disabled = false;
			FromInput.className = "enabled";
			ToInput.disabled = false;
			ToInput.className = "enabled";
		}
		else
		{
			FromInput.disabled = true;
			FromInput.className = "disabled";
			FromInput.value = "";
			ToInput.disabled = true;
			ToInput.className = "disabled";
			ToInput.value = "";
		}
	}
	
	//*End Layer 3 Elements*

	// Bottom Elements

	ButtonsDiv = UTILS_CreateElement('div', 'ButtonsDiv');

	Announce = UTILS_CreateElement('input', null, 'button');
	Announce.value = UTILS_GetText("challenge_announce_match");
	Announce.type = "button";
	Announce.onclick = function () {
		// Checking the color
		if (ColorOptW.checked)
		{
			Color = "white";
		}
		else if (ColorOptB.checked)
		{
			Color = "black";
		}
		else
		{
			Color = "";
		}

		// Rated or unrated?
		if (RatingCheckbox.checked)
		{
			Rated = "true";
		}
		else
		{
			Rated = "false";
		}

		// Create and send the chellenge message
		ANNOUNCE_SendAnnounce(MainData.Username, Color, TimeSelect.value, IncSelect.value, CatSelect.value, Rated, FromInput.value, ToInput.value);
	}

	Cancel = UTILS_CreateElement('input',null,'button');
	Cancel.value = UTILS_GetText('window_cancel');
	Cancel.type = "button";

	// Appending childs

	// Layer1
	// Left
	L1LeftDiv.appendChild(ColorLabel);
	L1LeftDiv.appendChild(ColorOptW);
	L1LeftDiv.appendChild(ColorOptWImg);
	L1LeftDiv.appendChild(BrW);
	L1LeftDiv.appendChild(ColorOptB);
	L1LeftDiv.appendChild(ColorOptBImg);
	L1LeftDiv.appendChild(BrB);
	L1LeftDiv.appendChild(AutoColorOpt);
	L1LeftDiv.appendChild(RandomColorOptImg);
	L1LeftDiv.appendChild(BrR);
//	L1LeftDiv.appendChild(AutoColorLabel);
	
	// Right
	L1RightDiv.appendChild(CatLabel);
	L1RightDiv.appendChild(CatSelect);

	Layer1Div.appendChild(L1LeftDiv);
	Layer1Div.appendChild(L1RightDiv);

	// Layer2
	// Left
	L2LeftDiv.appendChild(TimeLabel);
	L2LeftDiv.appendChild(TimeSelect);
	L2LeftDiv.appendChild(TimeLabelMin);
	L2LeftDiv.appendChild(TimeBr);

	// Right
	L2RightDiv.appendChild(IncLabel);
	L2RightDiv.appendChild(IncSelect);
	L2RightDiv.appendChild(IncLabelSeg);
	L2RightDiv.appendChild(IncBr);
	
	Layer2Div.appendChild(L2LeftDiv);
	Layer2Div.appendChild(L2RightDiv);

	// Layer3
	L3LeftDiv.appendChild(RatingCheckbox);
	L3LeftDiv.appendChild(RatingLabel);
	L3LeftDiv.appendChild(Br4);
	L3LeftDiv.appendChild(PrivateCheckbox);
	L3LeftDiv.appendChild(PrivateLabel);

	// Disabled
//	Layer3Div.appendChild(AutoFlagCheckbox);
//	Layer3Div.appendChild(AutoFlagLabel);

	L3RightDiv.appendChild(IntervalCheckbox);
	L3RightDiv.appendChild(IntervalLabel);

	FromDiv.appendChild(FromLabel);
	FromDiv.appendChild(FromInput);
	FromDiv.appendChild(Br5);
	FromDiv.appendChild(FormatLabel1);

	ToDiv.appendChild(ToLabel);
	ToDiv.appendChild(ToInput);
	ToDiv.appendChild(Br6);
	ToDiv.appendChild(FormatLabel2);

	L3RightDiv.appendChild(FromDiv);
	L3RightDiv.appendChild(ToDiv);

	Layer3Div.appendChild(L3LeftDiv);
	Layer3Div.appendChild(L3RightDiv);

	ButtonsDiv.appendChild(Announce);
	Buttons.push(Announce);
	ButtonsDiv.appendChild(Cancel);
	Buttons.push(Cancel);

	// Mount Main Div
	Div.appendChild(Layer1Div);
	Div.appendChild(Br1);
	Div.appendChild(Layer2Div);
	Div.appendChild(Br2);
	Div.appendChild(Layer3Div);
	Div.appendChild(Br3);
	Div.appendChild(ButtonsDiv);

	return {Div:Div, Buttons:Buttons}
}
