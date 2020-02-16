/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AGM_STRING_H
#define _AGM_STRING_H

class AGmFile;

class AL_EXT_CLASS AGmString : AGm {
public:
	AGmString(const char *s);
	AGmString(const AGmString *s);
	AGmString(long size);
	virtual ~AGmString();

	const char* Str() const;
	long Length() const;
	long MaxLength() const;
	unsigned char& Component(long i) const;

	int Compare(const char *s) const;
	int Compare(const AGmString *s) const;
	int CompareI(const char *s) const;
	int CompareI(const AGmString *s) const;

	int Expand(long size);
	int Copy(const char *s);
	int Copy(const AGmString *s);
	int Append(const char *s);
	int Append(const AGmString* s);
	int Append(char);
	int Append(AGmNode*);
	int AddChar(long);
	int IsEmpty();
	unsigned char LastChar();

	int StartsWith(const char*, int);
	int StartsWith(AGmString*);
	int EndsWith(const char*, int);
	int EndsWith(AGmString*);
	void Trim();

	long Search(unsigned char ch, long idx=0) const;
	long Search(const char*, long idx=0) const;
	long Search(const AGmString* s, long idx=0) const;
	long SearchI(const char*, long idx=0) const;
	long SearchI(const AGmString* s, long idx=0) const;
	AGmString* SubString(long from, long to);
	AGmString* FileExt();
	AGmString* DirOfPath(const char*);
	AGmString* NameOfPath(const char*);

	int InsertChar(long pos, char c);
	int DeleteChar(long pos);
	int ReplaceStr(long from, long to, const char *s);
	int ReplaceStr(long from, long to, AGmString *s);

	static int CreateString(AGmString*&, const char*);
	static void DeleteString(AGmString*&);

#if	_WINDOWS && _64BIT
	int WriteInteger(long long);
#else
	int WriteInteger(long);
#endif
	int WriteInteger(AGmNode*);
	int WriteHexa(long);
	int WriteReal(double);
	int WriteString(const char*);
	int WriteString(const AGmString*);
	int WriteQChar(char);
	int WriteQString(const char*);
	int WriteQString(const AGmString*);
	int WriteSingleQString(const char*);
	int WriteSingleQString(const AGmString*);
	int WriteMatrix(const AGmMatrix*);
	int WriteList(AGmNode*, int&, int limit=-1);

	static	int str_cmp(const char*, const char*);
	static	int str_cmp(const char*, const char*, long);

	void ChangeFileDelimiter(const unsigned char from, const unsigned char to);

	AGmString* XmlEncode(int mode);
	AGmString* XmlDecode(int mode);

	static int IsBlank(const char*);
	static int IsMbStr(const char*);

protected:
	int _Copy(const char*);
	unsigned char*	area;
	long	areaSize, strLen;
};

class AGmBinary : AGm {
public:
	AGmBinary(long size);
	virtual ~AGmBinary();

	unsigned char* Buffer() const;
	long Length() const;
	unsigned char& Component(long i) const;

	AGmBinary* Copy();
	int Compare(AGmBinary *b);

	int Expand(long size);
	int Copy(long i, AGmBinary *src_b, long src_i, long src_size);

	int Copy(long i, AGmString *s);
	AGmString* toString(long index, long size);

	long Search(unsigned char ch, long idx=0) const;
	long Search(long idx, const char* s) const;

	int Write(AGmFile*);
	static AGmBinary* Read(AGmFile*);

protected:
	unsigned char*	buff;
	long	length;
};

#endif	/* _AGM_STRING_H */

