function UTILS_IsSquareMorphB9(Col, Line)
{
	if(9==Line)
	{
		if(2 == Col)
			return true;
		else if(3== Col)
			return true;
		else if(5 == Col)
			return true;
		else if(7 == Col)
			return true;
		else if(8 == Col)
			return true;
	}
	else if(4 == Line)
	{
		if(2 <= Col && 8>= Col)
			return true;
	}

	return false;
}
function UTILS_IsSquareMorphA8(Col, Line)
{
	if(1 == Col)
	{
		if(8 == Line)
			return true;
		else if( 7 == Line)
			return true;
		else if( 5 == Line)
			return true;
		else if( 4 == Line)
			return true;
		else if( 2 == Line)
			return true;
		else if( 1 == Line)
			return true;
	}
	return false;
}
function UTILS_IsSquareMorphB8(Col, Line)
{
	if(8 == Line || 1 == Line)
	{
		if(2<=Col&&4>=Col)
			return true;
		if(6<=Col&&8>=Col)
			return true;
	}
	else if(7 == Line || 2 == Line) 
	{
		if(3 == Col || 5 == Col || 7 == Col)
			return true;
	}
	else if(6 == Line || 3 == Line)
	{
		if(2 == Col || 4 == Col|| 6 == Col || 8 == Col)
			return true;
	}
	return false;
}
function UTILS_IsSquareMorphI8(Col, Line)
{
	if(9 == Col)
	{
		if(8 == Line)
			return true;
		else if( 7 == Line)
			return true;
		else if( 5 == Line)
			return true;
		else if( 4 == Line)
			return true;
		else if( 2 == Line)
			return true;
		else if( 1 == Line)
			return true;
	}
	return false;
}
function UTILS_IsSquareMorphB7(Col, Line)
{
	if(7 == Line || 2 == Line)
	{
		if(2 == Col || 8 == Col)
			return true;
	}
	else if(6 == Line || 3 == Line)
	{
		if(3 == Col || 5 == Col || 7 == Col)
			return true;
	}
	return false;
}
function UTILS_IsSquareMorphB5(Col, Line)
{
	if(5 == Line)
	{
		if(2 <= Col && 8>= Col)
			return true;
	}
	else if( 0 == Line)
	{
		if(2 == Col || 3 == Col || 5 == Col || 7 == Col || 8 == Col)
			return true;
	}
	return false;

}
/**
* get square morph by its column and line
* column, range from [1,9],
* line, range from [0,9]
**/
function UTILS_GetSquareMorph(Col, Line)
{
	var MorphName;
	var BSqB9 = UTILS_IsSquareMorphB9(Col,Line);
	var BSqA8 = UTILS_IsSquareMorphA8(Col,Line);
	var BSqB8 = UTILS_IsSquareMorphB8(Col,Line);
	var BSqI8 = UTILS_IsSquareMorphI8(Col,Line);
	var BSqB7 = UTILS_IsSquareMorphB7(Col,Line);
	var BSqB5 = UTILS_IsSquareMorphB5(Col,Line);
	
// only 1 morph, total 12
	if(1==Col&&9==Line)
	{
		MorphName="sqa9";
	}
	else if(9==Col&&9==Line)
	{
		MorphName="sqi9";
	}
	else if(4==Col&&9==Line)
	{
		MorphName="sqd9";
	}
	else if(6==Col&&9==Line)
	{
		MorphName="sqf9";
	}
	else if(4==Col&&7==Line)
	{
		MorphName="sqd7";
	}
	else if(6==Col&&7==Line)
	{
		MorphName="sqf7";
	}
	else if(4==Col&&2==Line)
	{
		MorphName="sqd2";
	}
	else if(6==Col&&2==Line)
	{
		MorphName="sqf2";
	}
	else if(1==Col&&0==Line)
	{
		MorphName="sqa0";
	}
	else if(4==Col&&0==Line)
	{
		MorphName="sqd0";
	}
	else if(6==Col&&0==Line)
	{
		MorphName="sqf0";
	}
	else if(9==Col&&0==Line)
	{
		MorphName="sqi0";
	}
//more than 1 morph
// 2 morphes, total 3
	else if(5==Col&&(1==Line||8==Line))
	{
		MorphName="sqe8";
	}
	else if(1==Col&&(3==Line||6==Line))
	{
		MorphName="sqa6";
	}	
	else if(9==Col&&(3==Line||6==Line))
	{
		MorphName="sqi6";
	}
//more than 2 morphs

	else if(BSqA8==true)
	{
		MorphName="sqa8";
	}
	else if(BSqB5==true)
	{
		MorphName="sqb5";
	}
	else if(BSqB7==true)
	{
		MorphName="sqb7";
	}
	else if(BSqB8==true)
	{
		MorphName="sqb8";
	}
	else if(BSqB9==true)
	{
		MorphName="sqb9";
	}
	else if(BSqI8==true)
	{
		MorphName="sqi8";
	}
	else
	{
		MorphName="white";
	}
	return MorphName;

}
