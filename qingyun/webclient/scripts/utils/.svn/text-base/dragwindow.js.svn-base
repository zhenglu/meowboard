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
* Drag and Drog div Element used in window
*/

/**
* Start drag window when mouse is clicked over a element, and drop when release button.
*
* @param        ev is event
* @param	Obj is element object
* @return       boolean
* @author       Rubens
*/
function UTILS_StartDragWindow(ev, Obj)
{
	var MousePos, OldPos;
	var OffsetLeft, OffsetTop;

	Obj.style.position = "absolute";

	// Get mouse position when click on window title
	MousePos = UTILS_GetMouseCoords(ev);
	MousePos.x -= 19;

	// Get mouse offset in window
	OffsetTop = MousePos.y - Obj.offsetTop;
	OffsetLeft = MousePos.x - Obj.offsetLeft;

	// Drag window
	document.body.onmousemove = function(ev){
		var NewMousePos;
		NewMousePos = UTILS_GetMouseCoords(ev);
		NewMousePos.x -= 19;

		Obj.style.top = (NewMousePos.y - OffsetTop)+"px";
		Obj.style.left = (NewMousePos.x - OffsetLeft)+"px";

		return false;
	};

	// Stop drag
	document.body.onmouseup = function (ev){	
		var NewMousePos;

		// Getting mouse coord
		NewMousePos = UTILS_GetMouseCoords(ev);
		NewMousePos.x -= 19;

		// Set object in the new position
		Obj.style.top = (NewMousePos.y - OffsetTop)+"px";
		Obj.style.left = (NewMousePos.x - OffsetLeft)+"px";

		// Remove listener
		document.body.onmousemove = null;
		document.body.onmouseup = null;

		return false;
	}

	return false;
}
