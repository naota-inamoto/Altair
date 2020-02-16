/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#include "stdafx.h"

#ifdef	_WINDOWS
#include <mbstring.h>
#endif

#include <gdm/gdm.h>
#include <gdm/string.h>
#include <gdm/file.h>
#include <gdm/error.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

AGmString::AGmString(const char *s) {
	_Copy(s);
}
AGmString::AGmString(const AGmString *s) {
	_Copy((const char*)s->area);
}
AGmString::AGmString(long size) {
	area = new unsigned char [areaSize = size];
	area[strLen = 0] = 0;
}
AGmString::~AGmString() {
	delete [] area;
}

const char* AGmString::Str() const {
	return (const char*)area;
}
long AGmString::Length() const {
	return strLen;
}
long AGmString::MaxLength() const {
	return areaSize;
}
unsigned char& AGmString::Component(long i) const {
	return area[i];
}

#ifdef	_WINDOWS
int AGmString::Compare(const char *s) const {
	return _mbscmp(area, (unsigned char*)s);
}
int AGmString::Compare(const AGmString *s) const {
	return _mbscmp(area, s->area);
}
int AGmString::CompareI(const char *s) const {
	return _mbsicmp(area, (unsigned char*)s);
}
int AGmString::CompareI(const AGmString *s) const {
	return _mbsicmp(area, s->area);
}
#endif
#ifdef	_LINUX
int AGmString::Compare(const char *s) const {
	return strcmp((const char*)area, (const char*)s);
}
int AGmString::Compare(const AGmString *s) const {
	return strcmp((const char*)area, (const char*)s->area);
}
int AGmString::CompareI(const char *s) const {
	return strcasecmp((const char*)area, (const char*)s);
}
int AGmString::CompareI(const AGmString *s) const {
	return strcasecmp((const char*)area, (const char*)s->area);
}
#endif

int AGmString::Expand(long size) {
	unsigned char *area2 = new unsigned char [areaSize + size];
	memcpy(area2, area, strLen+1);
	delete [] area;
	area = area2;
	areaSize += size;
	return 0;
}
int AGmString::Copy(const char *s) {
	delete [] area;
	return _Copy(s);
}
int AGmString::Copy(const AGmString *s) {
	delete [] area;
	return _Copy((const char*)s->area);
}
int AGmString::_Copy(const char *s) {
	strLen = (long)strlen(s);
	areaSize = strLen + 1;
	area = new unsigned char [areaSize];
	art_strcpy((char*)area, areaSize, s);
	return 0;
}
int AGmString::Append(const char *s) {
	int strLen2 = (long)(strLen + strlen(s));
	if (strLen2 < areaSize) {
		art_strcpy((char*)area + strLen, areaSize - strLen, s);
	} else {
		if (areaSize * 2 < strLen2 + 1) {
			areaSize = strLen2 + 1;
		} else {
			areaSize *= 2;
		}
		unsigned char *area2 = new unsigned char [areaSize];
		memcpy(area2, area, strLen);
		art_strcpy((char*)area2 + strLen, areaSize - strLen, s);
		delete [] area;
		area = area2;
	}
	strLen = strLen2;
	return 0;
}
int AGmString::Append(const AGmString *s) {
	return Append((const char*)s->area);
}
int AGmString::Append(char c) {
	unsigned char *buf;
	if (strLen + 1 < areaSize) {
		buf = area + strLen;
		*buf++ = c;
		*buf = 0;
	} else {
		areaSize *= 2;
		unsigned char *area2 = new unsigned char [areaSize];
		art_strcpy((char*)area2, areaSize, (char*)area);
		buf = area2 + strLen;
		*buf++ = c;
		*buf = 0;
		delete [] area;
		area = area2;
	}
	strLen += 1;
	return 0;
}
int AGmString::Append(AGmNode* n) {
	if (!IsString(n)) return 1;
	AGmString *s = n->a.s.v;
	if (s == NULL) return 1;
	return Append(s->Str());

}
int AGmString::AddChar(long ch) {
#ifdef	_WINDOWS
	if (0xff00 & ch) {
		Append((char)(ch >> 8));
		return Append((char)ch);
	} else {
		return Append((char)ch);
	}
#endif
#ifdef	_LINUX
	toUTF8(this, ch);
	return 0;
#endif
}
int AGmString::IsEmpty() {
	long i;
	for (i = 0; i < strLen; i++) {
		unsigned char c = area[i];
		if (c != ' ' && c != '\t' && c != '\r' && c != '\n') return 0;
	}
	return 1;
}
unsigned char AGmString::LastChar() {
	unsigned char c = 0;
	long i;
	for (i = 0; i < strLen; i++) {
#ifdef	_WINDOWS
		c = area[i];
		if (isFirstChar(c)) i++;
#endif
#ifdef	_LINUX
		c = area[i];
		int i2;
		if (isUTF8(&area[i], i2)) i += i2-1;
#endif
	}
	return c;
}

int AGmString::StartsWith(const char* s, int len) {
	return (strLen >= len && strncmp((const char*)area, s, len) == 0);
}
int AGmString::StartsWith(AGmString* s) {
	return StartsWith(s->Str(), s->Length());
}
int AGmString::EndsWith(const char* s, int len) {
	return (strLen >= len && strncmp((const char*)area + (strLen - len), s, len) == 0);
}
int AGmString::EndsWith(AGmString* s) {
	return EndsWith(s->Str(), s->Length());
}

void AGmString::Trim() {
	long i, j;
	for (i = 0, j = -1; i < strLen; i++, j = -1) {
		char s[8];
		s[0] = area[i]; s[1] = 0;
#ifdef	_WINDOWS
		if (isFirstChar(*s)) {
			s[1] = area[++i]; s[2] = 0;
		}
		if (strcmp(s, " ") || strcmp(s, "　")) continue;
		for (j = i; i < strLen; i++) {
			s[0] = area[i]; s[1] = 0;
			if (isFirstChar(*s)) {
				s[1] = area[++i]; s[2] = 0;
			}
			if (strcmp(s, " ") || strcmp(s, "　")) break;
		}
#endif
#ifdef	_LINUX
		int i2;
		if (isUTF8(&area[i], i2)) {
			art_strcpy(s, 8, (const char*)&area[i]);
			i += i2-1;
		}
		if (strcmp(s, " ") || strcmp(s, "　")) continue;
		for (j = i; i < strLen; i++) {
			s[0] = area[i]; s[1] = 0;
			if (isUTF8(&area[i], i2)) {
				art_strcpy(s, 8, (const char*)&area[i]);
				i += i2-1;
			}
			if (strcmp(s, " ") || strcmp(s, "　")) break;
		}
#endif
		if (i == strLen) break;
	}
	if (j == -1) return;
	area[strLen = j] = 0;
}

long AGmString::Search(unsigned char ch, long idx) const {
	long i;
	for (i = idx; i < strLen; i++) {
		unsigned char c1 = area[i];
		if (c1 == ch) {
			return i;
		}
#ifdef	_WINDOWS
		if (isFirstChar(c1))
			i++;
#endif
#ifdef	_LINUX
		int i2;
		if (isUTF8(&area[i], i2))
			i += i2-1;
#endif
	}
	return -1;
}

// distingish capital letters and small letters
long AGmString::Search(const char* s, long idx) const {
	long m = (long)strlen(s);
	long n = strLen - m;
	long i, j;
	for (i = idx; i <= n; i++) {
		for (j = 0; j < m; j++) {
			char c1 = area[i+j];
			char c2 = s[j];
			if (c1 != c2)	
				break;
		}
		if (j == m)
			return i;
#ifdef	_WINDOWS
		if (isFirstChar(area[i]))
			i++;
#endif
#ifdef	_LINUX
		int i2;
		if (isUTF8(&area[i], i2))
			i += i2-1;
#endif
	}
	return -1;
}
long AGmString::Search(const AGmString* s, long idx) const {
	return Search((const char*)s->area, idx);
}

// not distingish capital letters and small letters
long AGmString::SearchI(const char* s, long idx) const {
	long m = (long)strlen(s);
	long n = strLen - m;
	long i, j;
	for (i = idx; i <= n; i++) {
		for (j = 0; j < m; j++) {
			char c1 = area[i+j];
			char c2 = s[j];
			if (islower(c1))
				c1 += 'A' - 'a';
			if (islower(c2))
				c2 += 'A' - 'a';
			if (c1 != c2)	
				break;
		}
		if (j == m)
			return i;
#ifdef	_WINDOWS
		if (isFirstChar(area[i]))
			i++;
#endif
#ifdef	_LINUX
		int i2;
		if (isUTF8(&area[i], i2))
			i += i2-1;
#endif
	}
	return -1;
}
long AGmString::SearchI(const AGmString* s, long idx) const {
	return SearchI((const char*)s->area, idx);
}

AGmString *AGmString::SubString(long from, long to) {
	if (from < 0) from = 0;
	if (to > strLen) to = strLen;
	if (from > to) return NULL;
	AGmString *s = new AGmString(16);
	for (long i = from; i < to; i++) {
		char c = (char)area[i];
#ifdef	_WINDOWS
		if (isFirstChar(c)) {
			if (i + 1 < to) {
				s->Append(c);
				c = (char)area[++i];
				s->Append(c);
			} else {
				++i;
			}
		} else {
			s->Append(c);
		}
#endif
#ifdef	_LINUX
		int i2;
		if (isUTF8(&area[i], i2)) {
			if (i + i2-1 < to) {
				s->Append((const char*)utf8);
				i += i2-1;
			} else {
				i += i2-1;
			}
		} else {
			s->Append(c);
		}
#endif
	}
	return s;
}

AGmString* AGmString::FileExt() {
	long i = 0, j = 0;
	for (;;) {
		j = Search(".", i+1);
		if (j < 0) break;
		i = j;
	}
	if (i == 0) return new AGmString(".");
	return SubString(i, Length());
}
AGmString* AGmString::DirOfPath(const char* delim) {
	int len = (int)strlen(delim);
	long i, j;
	for (i = 0; (j = Search(delim, i+len)) > 0; i = j) ;
	if (i == 0) return new AGmString(".");
	AGmString *dir = SubString(0, i);
	return dir;
}
AGmString* AGmString::NameOfPath(const char* delim) {
	int len = (int)strlen(delim);
	long i, j;
	for (i = 0; (j = Search(delim, i+len)) > 0; i = j) ;
	if (i == 0) return new AGmString(Str());
	AGmString *name = SubString(i+len, Length());
	return name;
}

int AGmString::InsertChar(long pos, char c) {
	Append((char)0);
	for (long i = strLen;	i > pos; i--)	
		area[i] = area[i-1];
	area[pos] = c;
	return 0;
}
int AGmString::DeleteChar(long pos) {
	for (long i = pos; i <= strLen; i++)
		area[i] = area[i+1];
	strLen--;
	return 0;
}
int AGmString::ReplaceStr(long from, long to, const char *s) {
	long i;
	for (i = 0; i <= strLen - to; i++)	
		area[from + i] = area[to + i];
	strLen -= (to - from);
	long n = (long)strlen(s);
	if (strLen + n >= areaSize) {
		areaSize *= 2;
		unsigned char *area2 = new unsigned char [areaSize];
		art_strcpy((char*)area2, areaSize, (char*)area);
		delete [] area;
		area = area2;
	}
	for (i = strLen - 1; i >= from; i--) {
		area[i + n] = area[i];
	}
	for (i = 0; i < n; i++) {
		area[from + i] = s[i];
	}
	strLen += n;
	area[strLen] = 0;
	return 0;
}
int AGmString::ReplaceStr(long from, long to, AGmString *s) {
	return ReplaceStr(from, to, (char*)s->area);
}

int AGmString::CreateString(AGmString*& s, const char *ss) {
	if (s) delete s;
	s = new AGmString(ss);
	return 0;
}

void AGmString::DeleteString(AGmString*& s) {
	if (s == NULL) return;
	delete s; s = NULL;
} 

#if	_WINDOWS && _64BIT
int AGmString::WriteInteger(long long v) {
#else
int AGmString::WriteInteger(long v) {
#endif
	AGmFile *f = new AGmFile();
	f->Create(this, AGM_FILE_STR_APPEND);
	int flag = f->WriteInteger(v);
	delete f;
	return flag;
}
int AGmString::WriteInteger(AGmNode* v) {
	AGmFile *f = new AGmFile();
	f->Create(this, AGM_FILE_STR_APPEND);
	int flag = f->WriteInteger(v);
	delete f;
	return flag;
}
int AGmString::WriteHexa(long v) {
	AGmFile *f = new AGmFile();
	f->Create(this, AGM_FILE_STR_APPEND);
	int flag = f->WriteHexa(v);
	delete f;
	return flag;
}
int AGmString::WriteReal(double v) {
	AGmFile *f = new AGmFile();
	f->Create(this, AGM_FILE_STR_APPEND);
	int flag = f->WriteReal(v);
	delete f;
	return flag;
}
int AGmString::WriteString(const char *s) {
	AGmFile *f = new AGmFile();
	f->Create(this, AGM_FILE_STR_APPEND);
	int flag = f->WriteString(s);
	delete f;
	return flag;
}
int AGmString::WriteString(const AGmString *s) {
	AGmFile *f = new AGmFile();
	f->Create(this, AGM_FILE_STR_APPEND);
	int flag = f->WriteString(s);
	delete f;
	return flag;
}
int AGmString::WriteQChar(char c) {
	AGmFile *f = new AGmFile();
	f->Create(this, AGM_FILE_STR_APPEND);
	int flag = f->WriteQChar(c);
	delete f;
	return flag;
}
int AGmString::WriteQString(const char *s) {
	AGmFile *f = new AGmFile();
	f->Create(this, AGM_FILE_STR_APPEND);
	int flag = f->WriteQString(s);
	delete f;
	return flag;
}
int AGmString::WriteQString(const AGmString *s) {
	return WriteQString(s->Str());
}
int AGmString::WriteSingleQString(const char *s) {
	AGmFile *f = new AGmFile();
	f->Create(this, AGM_FILE_STR_APPEND);
	int flag = f->WriteSingleQString(s);
	delete f;
	return flag;
}
int AGmString::WriteSingleQString(const AGmString *s) {
	return WriteSingleQString(s->Str());
}
int AGmString::WriteMatrix(const AGmMatrix *m) {
	AGmFile *f = new AGmFile();
	f->Create(this, AGM_FILE_STR_APPEND);
	int flag = f->WriteMatrix(m);
	delete f;
	return flag;
}
int AGmString::WriteList(AGmNode* n, int& count, int limit) {
	AGmFile *f = new AGmFile();
	f->Create(this, AGM_FILE_STR_APPEND);
	int flag = f->WriteList(n, count, limit);
	delete f;
	return flag;
}

int AGmString::str_cmp(const char *s1, const char *s2) {
	if (s1 == NULL && s2 == NULL) return 0;
	if (s1 == NULL) return 1;
	if (s2 == NULL) return -1;
	return strcmp(s1, s2);
}
int AGmString::str_cmp(const char *s1, const char *s2, long n) {
	if (s1 == NULL && s2 == NULL) return 0;
	if (s1 == NULL) return 1;
	if (s2 == NULL) return -1;
	return strncmp(s1, s2, n);
}

void AGmString::ChangeFileDelimiter(
		const unsigned char from, const unsigned char to) {
	for (long i = 0; i <= strLen; i++) {
		unsigned char c = area[i];
#ifdef	_WINDOWS
		if (isFirstChar(c)) {
			i++;
			continue;
		}
#endif
#ifdef	_LINUX
		int i2;
		if (isUTF8(&area[i], i2)) {
			i += i2-1;
			continue;
		}
#endif
		if (c == from) area[i] = to;
	}
}

AGmString* AGmString::XmlEncode(int mode) {
	AGmString *s = new AGmString(areaSize);
	int i;
	for (i = 0; i < strLen; i++) {
		unsigned char c = area[i];
#ifdef	_WINDOWS
		if (isFirstChar(c)) {
			s->Append(c);
			c = area[++i];
			s->Append(c);
#endif
#ifdef	_LINUX
		int i2;
		if (isUTF8(&area[i], i2)) {
			s->Append((const char*)utf8);
			i += i2-1;
#endif
		} else if ((mode & 0x01) && c == '&') {
			s->Append("&amp;");
		} else if ((mode & 0x01) && c == '<') {
			s->Append("&lt;");
		} else if ((mode & 0x01) && c == '>') {
			s->Append("&gt;");
		} else if ((mode & 0x02) && c == '\"') {
			s->Append("&quot;");
		} else {
			s->Append(c);
		}
	}
	return s;
}
AGmString* AGmString::XmlDecode(int mode) {
	AGmString *s = new AGmString(areaSize);
	int i;
	for (i = 0; i < strLen; i++) {
		unsigned char c = area[i];
#ifdef	_WINDOWS
		if (isFirstChar(c)) {
			s->Append(c);
			c = area[++i];
			s->Append(c);
#endif
#ifdef	_LINUX
		int i2;
		if (isUTF8(&area[i], i2)) {
			s->Append((const char*)utf8);
			i += i2-1;
#endif
		} else if (c == '&') {
			if ((mode & 0x01) && strncmp((char*)&area[i+1], "amp;", 4) == 0) {
				i += 4; s->Append('&');
			} else if ((mode & 0x01) && strncmp((char*)&area[i+1], "lt;", 3) == 0) {
				i += 3; s->Append('<');
			} else if ((mode & 0x01) && strncmp((char*)&area[i+1], "gt;", 3) == 0) {
				i += 3; s->Append('>');
			} else if ((mode & 0x02) && strncmp((char*)&area[i+1], "quot;", 5) == 0) {
				i += 5; s->Append('\"');
			} else {
				s->Append(c);
			}
		} else {
			s->Append(c);
		}
	}
	return s;
}

int AGmString::IsBlank(const char* s) {
	while (*s) {
		char c = *s++;
		if (c != ' ' && c != '\t' && c != '\n') return 0;
	}
	return 1;
}
int AGmString::IsMbStr(const char* s) {
	while (*s) {
		char c = *s++;
		if (c & 0x80) return 1;
	}
	return 0;
}

AGmBinary::AGmBinary(long size) {
	buff = new unsigned char [length = size];
}
AGmBinary::~AGmBinary() {
	delete [] buff;
}

unsigned char* AGmBinary::Buffer() const {
	return buff;
}
long AGmBinary::Length() const {
	return length;
}
unsigned char& AGmBinary::Component(long i) const {
	return buff[i];
}

AGmBinary* AGmBinary::Copy() {
	AGmBinary *b2 = new AGmBinary(length);
	b2->Copy(0, this, 0, length);
	return b2;
}
int AGmBinary::Compare(AGmBinary* b) {
	if (length < b->Length()) return 1;
	if (length > b->Length()) return -1;
	unsigned char *buff2 = b->Buffer();
	for (int idx = 0; idx < length; idx++) {
		if (buff[idx] > buff2[idx]) return 1;
		if (buff[idx] < buff2[idx]) return -1;
	}
	return 0;
}

int AGmBinary::Expand(long size) {
	unsigned char *buff2 = new unsigned char [length + size];
	memcpy(buff2, buff, length);
	delete [] buff;
	buff = buff2;
	length += size;
	return 0;
}
int AGmBinary::Copy(long i, AGmBinary *src_b, long src_i, long src_size) {
	unsigned char *buff2 = src_b->Buffer();
	memcpy((char*)buff + i, (char*)buff2 + src_i, src_size);
	return 0;
}

int AGmBinary::Copy(long i, AGmString *s) {
	memcpy((char*)buff + i, s->Str(), s->Length());
	return 0;
}
AGmString* AGmBinary::toString(long i, long size) {
	char *buff2 = new char [size+1];
	memcpy((char*)buff2, (char*)buff + i, size);
	buff2[size] = 0;
	AGmString *s = new AGmString(buff2);
	delete [] buff2;
	return s;
}

long AGmBinary::Search(unsigned char ch, long idx) const {
	long i;
	for (i = idx; i < length; i++) {
		unsigned char c1 = buff[i];
		if (c1 == ch) {
			return i;
		}
	}
	return -1;
}
long AGmBinary::Search(long idx, const char* s) const {
	long m = (long)strlen(s);
	long n = length - m;
	long i, j;
	for (i = idx; i <= n; i++) {
		for (j = 0; j < m; j++) {
			char c1 = buff[i+j];
			char c2 = s[j];
			if (c1 != c2)	
				break;
		}
		if (j == m)
			return i;
	}
	return -1;
}
int AGmBinary::Write(AGmFile* f) {
	f->WriteInteger(length);
	long count = 0;
	for (long i = 0; i < length; i++) {
		if (count++ < 50) {
			f->WriteString(" ");
		} else {
			count = 0;
			f->WriteString("\n");
		}
		f->WriteInteger(buff[i]);
	}
	return 0;
}
AGmBinary* AGmBinary::Read(AGmFile* f) {
#if	_WINDOWS && _64BIT
	long long size;
#else
	long size;
#endif
	if (f->ReadInteger(size)) return NULL;
	AGmBinary *bin = new AGmBinary((long)size);
	for (long i = 0; i < size; i++) {
#if	_WINDOWS && _64BIT
		long long d;
#else
		long d;
#endif
		if (f->ReadInteger(d)) {
			delete bin;
			return NULL;
		}
		bin->buff[i] = (BYTE)d;
	}
	return bin;
}
