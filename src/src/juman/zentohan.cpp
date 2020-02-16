/*
==============================================================================
	zentohan.c
		1991/01/08/Tue	Yutaka MYOKI(Nagao Lab., KUEE)
		1991/01/08/Tue	Last Modified
==============================================================================
*/

#include "stdafx.h"

#include <juman/juman.h>

/*
------------------------------------------------------------------------------
	LOCAL:
	definition of macros
------------------------------------------------------------------------------
*/

#define 	U_CHAR		unsigned char
#define 	iskanji(x)	((x)&&0x80)

/*
------------------------------------------------------------------------------
	LOCAL:
	definition of global variables
------------------------------------------------------------------------------
*/

static	U_CHAR	hankaku_table[]=
"!\"#$%&()*+,-.'/0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^abcdefghijklmnopqrstuvwxyz{|}^~_ --" ;

static U_CHAR	zenkaku_table[]=
"Ih”“•ij–{C?Df^‚O‚P‚Q‚R‚S‚T‚U‚V‚W‚XFGƒ„H—‚`‚a‚b‚c‚d‚e‚f‚g‚h‚i‚j‚k‚l‚m‚n‚o‚p‚q‚r‚s‚t‚u‚v‚w‚x‚ymnO‚‚‚‚ƒ‚„‚…‚†‚‡‚ˆ‚‰‚Š‚‹‚Œ‚‚‚‚‚‘‚’‚“‚”‚•‚–‚—‚˜‚™‚šobpOPQ@]\201\134" ;

/*
------------------------------------------------------------------------------
	FUNCTION:
	<zentohan>: convert (zenkaku)str -> (hankaku)str_out
------------------------------------------------------------------------------
*/

JUMAN_KNP_API U_CHAR *zen_to_han(U_CHAR *str)  {
	 U_CHAR *str_buffer = (U_CHAR*)malloc(strlen((const char*)str) + 1);
	 U_CHAR *str_out = str_buffer;
	 while (*str) {
	  if (iskanji(*str) && iskanji(*(str+1))) {
		   int ptr;
		   for (ptr = 0; *(zenkaku_table + ptr); ptr += 2) {
			if (zenkaku_table[ptr]	 == *(str) &&
			zenkaku_table[ptr+1] == *(str+1)) {
			 *str_out++ = hankaku_table[ptr/2];
			 break;
			}
		   }
		   if (*(zenkaku_table + ptr) == 0) {
			*str_out++ = *(str	);
			*str_out++ = *(str+1);
		   }
		   str += 2;
	  } else {
		   *str_out++ = *str++;
	  }
	 }
	 *str_out = 0;
	 return str_buffer;
}

/*
------------------------------------------------------------------------------
	FUNCTION
	<hantozen>: convert (hankaku)str -> (zenkaku)str_out
------------------------------------------------------------------------------
*/

JUMAN_KNP_API U_CHAR *han_to_zen(U_CHAR *str) {
	 U_CHAR *str_buffer = (U_CHAR*)malloc(strlen((const char*)str) * 2 + 1);
	 U_CHAR *str_out = str_buffer;
	 while (*str) {
	  U_CHAR	*str_tmp;
	  if ((str_tmp = (U_CHAR *)strchr((const char*)hankaku_table, *str)) != NULL) {
		   int	ptr = (int)(str_tmp - hankaku_table);
		   *str_out++ = zenkaku_table[ptr<<1];
		   *str_out++ = zenkaku_table[(ptr<<1)+1];
	  } else {
		   *str_out++ = *str;
	  }
	  str++;
	 }
	 *str_out = 0;
	 return str_buffer;
}
