/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AGM_J_CODE_H
#define _AGM_J_CODE_H

AGmString *JIS_to_SJIS(const char*);
AGmString *SJIS_to_JIS(const char*);
AGmString *EUC_to_SJIS(const char*);
AGmString *SJIS_to_EUC(const char*);

AGmString *SJIS_to_UTF8(const char*);
AGmString *EUC_to_UTF8(const char*);
AGmString *UTF8_to_SJIS(const char*);
AGmString *UTF8_to_EUC(const char*);

extern unsigned char utf8[8];
int isUTF8(unsigned char* s, int& i, long* val=NULL);
int isUTF8(AGmFile* f, int& i, long* val=NULL);
#ifdef	_LINUX
void toUTF8(AGmString* s, long value);
void toUTF8(AGmFile* f, long value);
#endif

char *zen_to_han(const char *str);
char *han_to_zen(const char *str);

#define CODE_NONE			0	// 判別不可  
#define CODE_NO_MATCH		1	// 該当なし  
#define CODE_UTF8			2	// UTF-8  
#define CODE_JIS			3	// JIS(7ビットコード)  
#define CODE_SJIS			4	// SJIS  
#define CODE_EUC			5	// EUC  
#define CODE_EUC_THAN_SJIS	6	// SJISよりもEUCの可能性が高い  
#define CODE_EUC_OR_SJIS	7	// SJISまたはEUCどちらの可能性もあり  
#define CODE_UTF8_OR_SJIS	8	// UTF-8またはSJIS  

int GetCharacterCodeType(const char*, int);
int CheckUnicode(AGmFile*);
int CheckJapanseCode(AGmFile*);

#endif	/* _AGM_J_CODE_H */
