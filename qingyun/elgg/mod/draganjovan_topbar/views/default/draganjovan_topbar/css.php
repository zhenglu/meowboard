/*************
draganjovan_topbar
************/


#draganjovan_topbar ul,#draganjovan_topbar li {
	font-size:14px; 
	font-family:Arial, Helvetica, sans-serif;
	line-height:21px;
	text-align:left;
	margin:0;
	padding::0;
}

#draganjovan_topbar {
	list-style:none;
        width:100%;
	margin:0px auto 0px auto;
	height:43px;


	/* Rounded Corners */
	


	/* Background color and gradients */
	
	background: #014464;
	background: -moz-linear-gradient(top, #0272a7, #013953);
	background: -webkit-gradient(linear, 0% 0%, 0% 100%, from(#0272a7), to(#013953));
	
	/* Borders */
	
	border: 1px solid #002232;

	-moz-box-shadow:inset 0px 0px 1px #edf9ff;
	-webkit-box-shadow:inset 0px 0px 1px #edf9ff;
	box-shadow:inset 0px 0px 1px #edf9ff;
}

#draganjovan_topbar li {
	float:left;
	display:block;
	text-align:center;
	position:relative;
	padding: 4px 7px 4px 7px;
	margin-right:30px;
	margin-top:7px;
	border:none;
}
#draganjovan_topbar span{
	float:left;
	display:block;
	text-align:center;
	position:relative;
	padding: 4px 7px 4px 7px;
	margin-top:7px;
	border:none;
}
#draganjovan_topbar .draganjovan_topbar_left {
	float:left;
	margin-left:-10;
}
#draganjovan_topbar span a {
	font-family:Arial, Helvetica, sans-serif;
	font-size:14px; 
	color: #EEEEEE;
	display:inline;
	outline:0;
	text-decoration:none;
	text-shadow: 1px 1px 1px #000;
}
#draganjovan_topbar li:hover {
	border: 1px solid #777777;
	padding: 4px 6px 4px 6px;
	
	/* Background color and gradients */
	
	background: #F4F4F4;
	background: -moz-linear-gradient(top, #F4F4F4, #EEEEEE);
	background: -webkit-gradient(linear, 0% 0%, 0% 100%, from(#F4F4F4), to(#EEEEEE));
	
	/* Rounded corners */
	
	-moz-border-radius: 5px 5px 0px 0px;
	-webkit-border-radius: 5px 5px 0px 0px;
	border-radius: 5px 5px 0px 0px;
}

#draganjovan_topbar li a {
	font-family:Arial, Helvetica, sans-serif;
	font-size:14px; 
	color: #EEEEEE;
	display:block;
	outline:0;
	text-decoration:none;
	text-shadow: 1px 1px 1px #000;
}

#draganjovan_topbar li:hover a {
	color:#161616;
	text-shadow: 1px 1px 1px #ffffff;
}
#draganjovan_topbar li .drop {
	padding-right:21px;
	background:url("<?php echo $vars['url']; ?>mod/draganjovan_topbar/graphics/drop.png") no-repeat right 8px;
}
#draganjovan_topbar li:hover .drop {
	background:url("<?php echo $vars['url']; ?>mod/draganjovan_topbar/graphics/drop.png") no-repeat right 7px;
}

.dropdown_1column, 
.dropdown_2columns, 
.dropdown_3columns, 
.dropdown_4columns,
.dropdown_5columns {
	margin:4px auto;
	float:left;
	position:absolute;
	left:-999em; /* Hides the drop down */
	text-align:left;
	padding:10px 5px 10px 5px;
	border:1px solid #777777;
	border-top:none;
	z-index: 1000;
	
	/* Gradient background */
	background:#F4F4F4;
	background: -moz-linear-gradient(top, #EEEEEE, #BBBBBB);
	background: -webkit-gradient(linear, 0% 0%, 0% 100%, from(#EEEEEE), to(#BBBBBB));

	/* Rounded Corners */
	-moz-border-radius: 0px 5px 5px 5px;
	-webkit-border-radius: 0px 5px 5px 5px;
	border-radius: 0px 5px 5px 5px;
}

.dropdown_1column {width: 140px;}
.dropdown_2columns {width: 280px;}
.dropdown_3columns {width: 420px;}
.dropdown_4columns {width: 560px;}
.dropdown_5columns {width: 700px;}

#draganjovan_topbar li:hover .dropdown_1column, 
#draganjovan_topbar li:hover .dropdown_2columns, 
#draganjovan_topbar li:hover .dropdown_3columns,
#draganjovan_topbar li:hover .dropdown_4columns,
#draganjovan_topbar li:hover .dropdown_5columns {
	left:-1px;
	top:auto;
}

.col_1,
.col_2,
.col_3,
.col_4,
.col_5 {
	display:inline;
	float: left;
	position: relative;
	margin-left: 5px;
	margin-right: 5px;
}
.col_1 {width:130px;}
.col_2 {width:270px;}
.col_3 {width:410px;}
.col_4 {width:550px;}
.col_5 {width:690px;}

#draganjovan_topbar .draganjovan_topbar_right {
	float:right;
	margin-right:0px;
}
#draganjovan_topbar li .align_right {
	/* Rounded Corners */
	-moz-border-radius: 5px 0px 5px 5px;
    -webkit-border-radius: 5px 0px 5px 5px;
    border-radius: 5px 0px 5px 5px;
}

#draganjovan_topbar li:hover .align_right {
	left:auto;
	right:-1px;
	top:auto;
}

#draganjovan_topbar p, #draganjovan_topbar h2, #draganjovan_topbar h3, #draganjovan_topbar ul li {
	font-family:Arial, Helvetica, sans-serif;
	line-height:21px;
	font-size:12px;
	text-align:left;
	text-shadow: 1px 1px 1px #FFFFFF;
}
#draganjovan_topbar h2 {
	font-size:21px;
	font-weight:400;
	letter-spacing:-1px;
	margin:7px 0 14px 0;
	padding-bottom:14px;
	border-bottom:1px solid #666666;
}
#draganjovan_topbar h3 {
	font-size:14px;
	margin:7px 0 14px 0;
	padding-bottom:7px;
	border-bottom:1px solid #888888;
}
#draganjovan_topbar p {
	line-height:18px;
	margin:0 0 10px 0;
}

#draganjovan_topbar li:hover div a {
	font-size:12px;
	color:#015b86;
}
#draganjovan_topbar li:hover div a:hover {
	color:#029feb;
}


.strong {
	font-weight:bold;
}
.italic {
	font-style:italic;
}

.imgshadow { /* Better style on light background */
	background:#FFFFFF;
	padding:4px;
	border:1px solid #777777;
	margin-top:5px;
	-moz-box-shadow:0px 0px 5px #666666;
	-webkit-box-shadow:0px 0px 5px #666666;
	box-shadow:0px 0px 5px #666666;
}
.img_left { /* Image sticks to the left */
	width:auto;
	float:left;
	margin:5px 15px 5px 5px;
}

#draganjovan_topbar li .black_box {
	background-color:#333333;
	color: #eeeeee;
	text-shadow: 1px 1px 1px #000;
	padding:4px 6px 4px 6px;

	/* Rounded Corners */
	-moz-border-radius: 5px;
    -webkit-border-radius: 5px;
    border-radius: 5px;

	/* Shadow */
	-webkit-box-shadow:inset 0 0 3px #000000;
	-moz-box-shadow:inset 0 0 3px #000000;
	box-shadow:inset 0 0 3px #000000;
}

#draganjovan_topbar li ul {
	list-style:none;
	padding:0;
	margin:0 0 12px 0;
}
#draganjovan_topbar li ul li {
	font-size:12px;
	line-height:24px;
	position:relative;
	text-shadow: 1px 1px 1px #ffffff;
	padding:0;
	margin:0;
	float:none;
	text-align:left;
	width:130px;
}
#draganjovan_topbar li ul li:hover {
	background:none;
	border:none;
	padding:0;
	margin:0;
}

#draganjovan_topbar li .greybox li {
	background:#F4F4F4;
	border:1px solid #bbbbbb;
	margin:0px 0px 4px 0px;
	padding:4px 6px 4px 6px;
	width:116px;

	/* Rounded Corners */
	-moz-border-radius: 5px;
    -webkit-border-radius: 5px;
    -khtml-border-radius: 5px;
    border-radius: 5px;
}
#draganjovan_topbar li .greybox li:hover {
	background:#ffffff;
	border:1px solid #aaaaaa;
	padding:4px 6px 4px 6px;
	margin:0px 0px 4px 0px;
}
.draganjovan_topbar_left .user_mini_avatar{
	border: 1px solid #EEEEEE;
	padding:1px;
}
.draganjovan_topbar_left a.privatemessages {
	background:transparent url(<?php echo $vars['url']; ?>mod/draganjovan_topbar/graphics/toolbar_messages_icon.gif) no-repeat left 2px;
	padding:0 0 1px 16px;
	margin:0 15px 0 5px;
	cursor:pointer;
}
.draganjovan_topbar_left a.privatemessages:hover {
	text-decoration: none;
	background:transparent url(<?php echo $vars['url']; ?>mod/draganjovan_topbar/graphics/toolbar_messages_icon.gif) no-repeat left -36px;
}
.draganjovan_topbar_left a.privatemessages_new {
	background:transparent url(<?php echo $vars['url']; ?>mod/draganjovan_topbar/graphics/toolbar_messages_icon.gif) no-repeat left -17px;
	padding:0 0 0 18px;
	margin:0 15px 0 5px;
	color:white;
}
/* IE6 */
* html .draganjovan_topbar_left a.privatemessages_new { background-position: left -18px; }
/* IE7 */
*+html .draganjovan_topbar_left a.privatemessages_new { background-position: left -18px; }

.draganjovan_topbar_left a.privatemessages_new:hover {
	text-decoration: none;
}
.searchform {
	margin: 10px 7px 0 5px;
}
.searchform input {

}
.searchform .searchfield {
	float:left;
        margin:0px;
	display:inline;
	position:relative;
        margin-right:4px;
	background: #fff;
	padding: 2px 6px 2px 8px;
	width: 180px;
	border: solid 1px #bcbbbb;
	outline: none;
	-webkit-border-radius: 6px;
	-moz-border-radius: 6px;
	border-radius: 6px;

	-moz-box-shadow: inset 0 1px 2px rgba(0,0,0,.2);
	-webkit-box-shadow: inset 0 1px 2px rgba(0,0,0,.2);
	box-shadow: inset 0 1px 2px rgba(0,0,0,.2);
}
.searchform .searchbutton {;
	float:left;
        margin:0px;
	display:inline;
	position:relative;
        height:24px;
	color: #fff;
	border: solid 1px #494949;
	font-size: 11px;
	text-shadow: 0 1px 1px rgba(0,0,0,.6);

	-webkit-border-radius: 4px;
	-moz-border-radius: 4px;
	border-radius: 4px;

	background: #5f5f5f;
	background: -webkit-gradient(linear, left top, left bottom, from(#9e9e9e), to(#454545));
	background: -moz-linear-gradient(top,  #9e9e9e,  #454545);
	filter:  progid:DXImageTransform.Microsoft.gradient(startColorstr='#9e9e9e', endColorstr='#454545'); /* ie7 */
	-ms-filter:  progid:DXImageTransform.Microsoft.gradient(startColorstr='#9e9e9e', endColorstr='#454545'); /* ie8 */
}
.imgborder img{
    border: 2px solid #FFFFFF;
    padding: 5px;

}