#ifndef KB_FR_H
#define KB_FR_H

#ifdef KB_MAP
#error Keyboard map already loaded
#endif
#define KB_MAP

const char KEYBOARD_MAP[] = 
{
	0,	//(0)
	0,	//ESC(1)
	'&','é','"','\'','(','-','è','_','ç','à',')','=','\b',	//(2-E)
	'\t','a','z','e','r','t','y','u','i','o','p','^','$','\n',	//(F-1C)
	0,	//LCTRL(1D)
	'q','s','d','f','g','h','j','k','l','m','ù',//(1E-28)
	0,	//SQUARE(29)
	0,	//LSHIFT(2A)
	'*', //(2B)
	'w','x','c','v','b','n',',',';',':','!',	//(2C-35)
	//0x35 is also NUM_SLASH
	0,	//RSHIFT(36)
	'*',//NUM_MULT(37)
	0,	//LALT(38)
	' ',//(39)
	0,	//CAPS(3A)
	0,	//F1(3B)
	0,	//F2(3C)
	0,	//F3(3D)
	0,	//F4(3E)
	0,	//F5(3F)
	0,	//F6(40)
	0,	//F7(41)
	0,	//F8(42)
	0,	//F9(43)
	0,	//F10(44)
	0,	//VERR_NUM(45)
	0,	//SCROLL_LOCK(46)
	0,	//NUM_7(47)
	0,	//UP or NUM_8(48)
	0,	//PAGE_UP or NUM_9(49)
	'-',//NUM_MINUS(4A)
	0,	//LEFT or NUM_4(4B)
	0,	//NUM_5(4C)
	0,	//RIGHT or NUM_6(4D)
	'+',//NUM_ADD(4E)
	0,	//END or NUM_1(4F)
	0,	//DOWN or NUM_2(50)
	0,	//PAGE_DOWN or NUM_3(51)
	'0',//NUM_0(52)
	'.',//NUM_POINT or DEL(53)
};

#endif
