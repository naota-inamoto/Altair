/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#include "stdafx.h"

#include <gdm/gdm.h>
#include <gdm/string.h>
#include <gdm/matrix.h>
#include <gdm/file.h>
#include <gdm/debug.h>
#include <gdm/error.h>
#include <gdm/pict.h>
#include <dev/bitmap.h>
#include <dev/ocx.h>
#include <gdm/big_int.h>

#ifdef	_WINDOWS
#include <direct.h>
#endif

#ifdef	_LINUX
#include <dirent.h>
#include <sys/wait.h>
#endif

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#define UNPUT_BUFFER_SIZE	16384
#define PRE_READ_CC 512

int isIdentHead(const unsigned char c) {
	if ('a' <= c  && c <= 'z')
		return 1;
	if ('A' <= c  && c <= 'Z')
		return 1;
	if (c == '_')
		return 1;
//#ifdef	AL_LANG_ENGLISH
//	unsigned char c1 = c;
//	if (0xc0 <= c1 && c1 <= 0xfc)
//		return 1;
//#endif
	return 0;
}
int isIdentTail(const unsigned char c) {
	if ('a' <= c  && c <= 'z')
		return 1;
	if ('A' <= c  && c <= 'Z')
		return 1;
	if ('0' <= c  && c <= '9')
		return 1;
	if (c == '_')
		return 1;
//#ifdef	AL_LANG_ENGLISH
//	unsigned char c1 = c;
//	if (0xc0 <= c1 && c1 <= 0xfc)
//		return 1;
//#endif
	return 0;
}
int isFirstChar(const unsigned char c) {
//#ifdef	AL_LANG_JAPANESE
//	if (0x81 <= c && c <= 0x9f)
//		return 1;
//	if (0xe0 <= c && c <= 0xfc)
//		return 1;
//#endif
	if (c == 0xff) return 0;
#ifdef	_WINDOWS
	if (0xa1 <= c && c <= 0xdf) return 0; // hankakukana
#endif
	return ((c & 0x80) != 0);
}

#define TO_UPPER(c) (c + 'A' - 'a')
#define TO_LOWER(c) (c + 'a' - 'A')

int isNoCaseEqual(const unsigned char c1, const unsigned char c2) {
	if ('a' <= c1  && c1 <= 'z' && TO_UPPER(c1) == c2) return 1;
	if ('A' <= c1  && c1 <= 'Z' && TO_LOWER(c1) == c2) return 1;
	if (c1 == c2) return 1;
	return 0;
}

AGmFile::AGmFile() {
	type = 0xff;
	
	auto_expand_unput_buffer = 0;
	offset = 0;
	unput_buffer_size = UNPUT_BUFFER_SIZE;
	redirect_pos = -1;
	debug_input_unput = 0;

	filename = NULL;
	_doc = _delim = NULL;

#ifdef	_WINDOWS
	rStdinPipe = NULL;
	wStdinPipe = NULL;
	rStdoutPipe = NULL;
	wStdoutPipe = NULL;

	thread = NULL;
	stdoutThread = NULL;

	retEvent = NULL;
	contEvent = NULL;

#endif
#ifdef	_LINUX
	stdin_pipe[0] = -1;
	stdin_pipe[1] = -1;
	stdout_pipe[0] = -1;
	stdout_pipe[1] = -1;

	thread = NULL;
	stdoutThread = NULL;

	g_cond_init(&retCond);
	g_cond_init(&contCond);

	retCondFlag = 0;
	contCondFlag = 0;

	g_mutex_init(&mutex);

#endif
	value = NULL;

	rbuf1 = NULL;
	rbuf2 = NULL;
}

int AGmFile::Create(const char *name, unsigned char type) {
	ok = 0;
	this->type = type;
	switch (type) {
	case AGM_FILE_READ:
		if ((fp = art_fopen(name, "r")) == NULL)
			return 1;
		filename = new AGmString(name);
		BeginUnputBuffer();
		break;
	case AGM_FILE_WRITE:
		if ((fp = art_fopen(name, "w")) == NULL)
			return 1;
		filename = new AGmString(name);
		break;
	case AGM_FILE_APPEND:
		if ((fp = art_fopen(name, "a")) == NULL)
			return 1;
		filename = new AGmString(name);
		break;
	default:
		return 1;
	}
	ok = 1;
	return 0;
}

int AGmFile::Create(AGmString *name, unsigned char type) {
#ifdef	_WINDOWS
	SECURITY_ATTRIBUTES sa;
#endif
	int fd;

	ok = 0;
	this->type = type;
	switch (type) {

	case AGM_FILE_READ:
		if ((fp = art_fopen(name->Str(), "r")) == NULL)
			return 1;
		filename = new AGmString(name);
		BeginUnputBuffer(); 
		break;
	case AGM_FILE_WRITE:
		if ((fp = art_fopen(name->Str(), "w")) == NULL)
			return 1;
		filename = new AGmString(name);
		break;
	case AGM_FILE_APPEND:
		if ((fp = art_fopen(name->Str(), "a")) == NULL)
			return 1;
		filename = new AGmString(name);
		break;
	case AGM_FILE_STR_READ:
		str = name;
		str_ptr = (char*)name->Str();
		num_eof = 0;
		nl_count = 0;
		nl_token = 0;
		break;
	case AGM_FILE_STR_WRITE:
		str = name;
		str->Copy("");
		break;
	case AGM_FILE_STR_APPEND:
		str = name;
		break;
	case AGM_FILE_BIN_READ:
		if ((fp = art_fopen(name->Str(), "rb")) == NULL)
			return 1;
		filename = new AGmString(name);
		BeginUnputBuffer();
		break;
	case AGM_FILE_BIN_WRITE:
		if ((fp = art_fopen(name->Str(), "wb")) == NULL)
			return 1;
		filename = new AGmString(name);
		break;
	case AGM_FILE_BIN_APPEND:
		if ((fp = art_fopen(name->Str(), "ab")) == NULL)
			return 1;
		filename = new AGmString(name);
		break;
	case AGM_FILE_STDIN:
		fp = stdin;
		BeginUnputBuffer(); 
		break;
	case AGM_FILE_STDOUT:
		fp = stdout;
		break;
	case AGM_FILE_STDERR:
		fp = stderr;
		break;
	case AGM_FILE_PIPE:
#ifdef	_WINDOWS
		retEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		contEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		if (retEvent == NULL || contEvent == NULL)
			return 1;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;
		if (CreatePipe(&rStdinPipe, &wStdinPipe, &sa, 0) != TRUE) {
			rStdinPipe = NULL;
			wStdinPipe = NULL;
			return 1;
		}
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;
		if (CreatePipe(&rStdoutPipe, &wStdoutPipe, &sa, 0) != TRUE) {
			rStdoutPipe = NULL;
			wStdoutPipe = NULL;
			return 1;
		}
		BeginUnputBuffer();
		::InitializeCriticalSection(&cs);
#endif
#ifdef	_LINUX
		if (pipe(stdin_pipe) == -1) {
			stdin_pipe[0] = stdin_pipe[1] = -1;
			return 1;
		}
		if (pipe(stdout_pipe) == -1) {
			stdout_pipe[0] = stdout_pipe[1] = -1;
			return 1;
		}

		BeginUnputBuffer();
		g_mutex_init(&mutex);
#endif
		break;
	case AGM_FILE_LOCK:
		str = name;
		fd = art_open(str->Str(), O_WRONLY | O_CREAT | O_EXCL);
		if (fd >= 0) {
			fp = art_fdopen(fd, "w");
			if (fp != NULL) {
				break;
			}
			art_close(fd);
		}
		return 1;
	default:
		return 1;
	}
	ok = 1;
	return 0;
}

int AGmFile::Create(int fd, unsigned char type) {
	ok = 0;
	this->type = type;
	switch (type) {
	case AGM_FILE_FD_IN:
		this->fd = fd;
		BeginUnputBuffer(); 
		break;
	case AGM_FILE_FD_OUT:
		this->fd = fd;
		break;
	default:
		return 1;
	}
	ok = 1;
	return 0;
}

AGmFile::~AGmFile() {
	if (ok) {
		switch (type) {

		case AGM_FILE_READ:
		case AGM_FILE_BIN_READ:
			fclose(fp);
			EndUnputBuffer();
			break;
		case AGM_FILE_WRITE:
		case AGM_FILE_APPEND:
		case AGM_FILE_BIN_WRITE:
		case AGM_FILE_BIN_APPEND:
			fclose(fp);
			break;
		case AGM_FILE_STDIN:
		case AGM_FILE_FD_IN:
			EndUnputBuffer();
			break;
		case AGM_FILE_STDOUT:
		case AGM_FILE_STDERR:
		case AGM_FILE_FD_OUT:
			break;
		case AGM_FILE_PIPE:
#ifdef	_WINDOWS
			if (stdoutThread) {
				::TerminateThread(stdoutThread, (DWORD)-1);
				::CloseHandle(stdoutThread);
			}
			EndUnputBuffer();
			::DeleteCriticalSection(&cs);
			if (wStdinPipe) ::CloseHandle(wStdinPipe);
			if (rStdinPipe) ::CloseHandle(rStdinPipe);
			if (wStdoutPipe) ::CloseHandle(wStdoutPipe);
			if (rStdoutPipe) ::CloseHandle(rStdoutPipe);
#endif
#ifdef	_LINUX
			if (stdoutThread) {
				//g_thread_unref(stdoutThread);
				stdoutThread = NULL;
			}
			EndUnputBuffer();
			g_mutex_clear(&mutex);
			if (stdin_pipe[0] != -1) close(stdin_pipe[0]);
			if (stdin_pipe[1] != -1) close(stdin_pipe[1]);
			if (stdout_pipe[0] != -1) close(stdout_pipe[0]);
			if (stdout_pipe[1] != -1) close(stdout_pipe[1]);
#endif
			break;
		case AGM_FILE_LOCK:
			fclose(fp);
			AGmFile::Remove(str->Str());
			break;
		default:
			break;
		}
	}

	if (filename) delete filename;
	Unref(_doc, _delim);

#ifdef	_WINDOWS
	if (retEvent) ::CloseHandle(retEvent);
	if (contEvent) ::CloseHandle(contEvent);
#endif
#ifdef	_LINUX
	g_cond_clear(&retCond);
	g_cond_clear(&contCond);
#endif

	if (rbuf1) delete rbuf1;
	if (rbuf2) delete rbuf2;
}

void AGmFile::GCmark() {
#ifdef	_WINDOWS
	_GCmark(value);
#endif
	_GCmark(_doc); _GCmark(_delim);
}

AGmString* AGmFile::GetDir() {
	if (filename == NULL) return NULL;
	return filename->DirOfPath("/");
}

int AGmFile::HereDoc(AGmNode *doc, AGmNode *delim) {
	if (_doc || _delim) return 1;
	_doc = Ref(doc); _delim = Ref(delim);
	return 0;
}

// =============================================
int AGmFile::WriteChar(const char c) {
	char buff[2];
	buff[0] = c;
	buff[1] = 0;
	return WriteString(buff);
}
#if	_WINDOWS && _64BIT
int AGmFile::WriteInteger(long long v) {
#else
int AGmFile::WriteInteger(long v) {
#endif
	char buff[50];
	art_sprintf_1(buff, 50, "%ld", v);
	return WriteString(buff);
}
int AGmFile::WriteBigInteger(AGmNode* v) {
	if (IsBigInt(v)) {
		AGmString *ss = BigInteger::ToString((long)v->a.bi.sign, v->a.bi.bin);
		int err = WriteString(ss->Str());
		delete ss;
		return err;
	} else {
		return -1;
	}
}
int AGmFile::WriteInteger(AGmNode* v) {
	if (IsInteger(v)) {
		return WriteInteger(v->a.i.v);
	} else if (IsBigInt(v)) {
		return WriteBigInteger(v);
	} else {
		return -1;
	}
}
#if	_WINDOWS && _64BIT
int AGmFile::WriteHexa(long long v) {
#else
int AGmFile::WriteHexa(long v) {
#endif
	char buff[50];
	art_sprintf_1(buff, 50, "%lx", v);
	return WriteString(buff);
}
int AGmFile::WriteReal(double v) {
	char buff[50];
	int i, count = art_sprintf_1(buff, 50, "%.16g", v);
	for (i = 0; i < count; i++) {
		if (buff[i] == '.') break;
	}
	if (i == count) {
		// append ".", when "e" does not exist.
		for (i = 0; i < count; i++) {
			if (buff[i] == 'e') break;
		}
		if (i == count) {
			// append ".", when "e" does not exist at last.
			art_sprintf_0(&buff[count], 50 - count, ".");
		} else {
			// insert "." before "e".
			for (int j = count; i <= j; j--) {
				buff[j+1] = buff[j];
			}
			buff[i] = '.';
		}
	}
	return WriteString(buff);
}
int AGmFile::WriteString(const char *s) {
	long n = (long)strlen(s);
	for (long i = 0; i < n; i++) {
		Output(s[i]);
	}
	return 0;
}
int AGmFile::WriteString(const AGmString *s) {
	return WriteString(s->Str());
}
int AGmFile::WriteString(const AGmBinary *bin) {
	long n = bin->Length();
	unsigned char *s = bin->Buffer();
	for (long i = 0; i < n; i++) {
		Output(s[i]);
	}
	return 0;
}
int AGmFile::WriteQChar(char c) {
	Output('\'');
	if (c == '\\') {
		Output('\\');
		Output('\\');
	} else if (c == '\t') {
		Output('\\');
		Output('t');
	} else if (c == '\r') {
		Output('\\');
		Output('r');
	} else if (c == '\n') {
		Output('\\');
		Output('n');
	} else if (c == '\'') {
		Output('\\');
		Output('\'');
	} else if (c == '\"') {
		Output('\\');
		Output('\"');
	} else {
		Output(c);
	}
	Output('\'');
	return 0;
}
int AGmFile::WriteQString(const char *s, int b_esc) {
	Output('\"');
	long n = (long)strlen(s);
	for (long i = 0; i < n; i++) {
		char c = s[i];
#ifdef	_WINDOWS
		if (isFirstChar(c)) {
			Output(c);
			c = s[++i];
			Output(c);
#endif
#ifdef	_LINUX
		int i2;
		if (isUTF8((unsigned char*)&s[i], i2)) {
			int j;
			for (j = 0; j < i2; j++) {
				Output(utf8[j]);
			}
			i += i2-1;
#endif
		} else if (b_esc) {
			if (c == '\\') {
				Output('\\');
				Output('\\');
			} else if (c == '\t') {
				Output('\\');
				Output('t');
			} else if (c == '\r') {
				Output('\\');
				Output('r');
			} else if (c == '\n') {
				Output('\\');
				Output('n');
			} else if (c == '\"') {
				Output('\\');
				Output('\"');
			} else {
				Output(c);
			}
		} else {
			Output(c);
		}
	}
	Output('\"');
	return 0;
}
int AGmFile::WriteQString(const AGmString *s, int b_esc) {
	return WriteQString(s->Str(), b_esc);
}
int AGmFile::WriteSingleQString(const char* s, int b_esc) {
	//Output('\'');
	long n = (long)strlen(s);
	for (long i = 0; i < n; i++) {
		char c = s[i];
#ifdef	_WINDOWS
		if (isFirstChar(c)) {
			Output(c);	
			c = s[++i];
			Output(c);
#endif
#ifdef	_LINUX
		int i2;
		if (isUTF8((unsigned char*)&s[i], i2)) {
			int j;
			for (j = 0; j < i2; j++) {
				Output(utf8[j]);
			}
			i += i2-1;
#endif
		} else if (c == '\'') {
			Output('\'');	
			Output('\'');
		} else if (b_esc) {
			if (c == '\\') {
				Output('\\');
				Output('\\');
			} else if (c == '\t') {
				Output('\\');
				Output('t');
			} else if (c == '\r') {
				Output('\\');
				Output('r');
			} else if (c == '\n') {
				Output('\\');
				Output('n');
			} else {
				Output(c);
			}
		} else {
			Output(c);
		}
	}
	//Output('\'');
	return 0;
}
int AGmFile::WriteMatrix(const AGmMatrix *m) {
	long r = m->NumRow(), c = m->NumColumn();
	WriteInteger(r);
	WriteString(" ");
	WriteInteger(c);
	for (long i = 0; i < r; i++)
	for (long j = 0; j < c; j++) {
		WriteString(" ");
		WriteReal(m->Component(i, j));
	}
	return 0;
}
int AGmFile::WriteCsv(AGmNode *n) {
	AGmLink *l; AGmNode *n2; int first = 1;
	for (l = next_outlink(n, NULL); l; l = next_outlink(n, l)) {
		n2 = l->arc->dst;
		if (!IsString(n2)) return 1; // error
		const char *s = n2->a.s.v->Str();
		int convert = 0;
		AGmString *str = new AGmString(16);
		long n = (long)strlen(s);
		for (long i = 0; i < n; i++) {
			char c = s[i];
#ifdef	_WINDOWS
			if (isFirstChar(c)) {
				str->Append(c);
				c = s[++i];
				str->Append(c);
#endif
#ifdef	_LINUX
			int i2;
			if (isUTF8((unsigned char*)&s[i], i2)) {
				str->Append((const char*)utf8);
				i += i2-1;
#endif
			} else if (c == ',') {
				str->Append(c);
				convert = 1;
			} else if (c == '\n') {
				str->Append(c);
				convert = 1;
			} else if (c == '\"') {
				str->Append(c);
				str->Append(c);
				convert = 1;
			} else {
				str->Append(c);
			}
		}
		if (first) {
			first = 0;
		} else {
			Output(',');
		}
		if (convert) {
			Output('\"');
		}
		WriteString(str->Str());
		delete str;
		if (convert) {
			Output('\"');
		}
	}
	Output('\n');
	return 0;
}
int AGmFile::WriteList(AGmNode *n, int& count, int limit) {
	if (n == NULL) {
		if (limit > 0 && ++count > limit) return 0;
		WriteString("null");
		return 0;
	}
	switch (n->type) {
	case AGM_NODE_INTEGER:
		if (limit > 0 && ++count > limit) return 0;
		WriteInteger(n->a.i.v);
		break;
	case AGM_NODE_REAL:
		if (limit > 0 && ++count > limit) return 0;
		WriteReal(n->a.r.v);
		break;
	case AGM_NODE_STRING:
		if (limit > 0 && ++count > limit) return 0;
		WriteQString(n->a.s.v);
		break;
	case AGM_NODE_BIGINT:
		if (limit > 0 && ++count > limit) return 0;
		WriteBigInteger(n);
		break;
	case AGM_NODE_LIST:
		if (limit > 0 && ++count > limit) return 0;
		WriteString("(");
		WriteList(n->a.l.h, count, limit);
		_WriteList(n->a.l.t, count, limit);
		WriteString(")");
		break;
	case AGM_NODE_GFP:
		if (limit > 0 && ++count > limit) return 0;
		WriteString("finit_set");
		break;
	case AGM_NODE_ARRAY:
		if (limit > 0 && ++count > limit) return 0;
		WriteString("array");
		break;
	case AGM_NODE_OBJECT:
		if (limit > 0 && ++count > limit) return 0;
		WriteString("object");
		break;
	case AGM_NODE_DST_ITR:
		if (limit > 0 && ++count > limit) return 0;
		WriteString("dst_itr");
		break;
	case AGM_NODE_SRC_ITR:
		if (limit > 0 && ++count > limit) return 0;
		WriteString("src_itr");
		break;
	case AGM_NODE_MATRIX:
		if (limit > 0 && ++count > limit) return 0;
		WriteString("matrix");
		break;
	case AGM_NODE_FILE:
		if (limit > 0 && ++count > limit) return 0;
		WriteString("file");
		break;
	case AGM_NODE_WINDOW:
		if (limit > 0 && ++count > limit) return 0;
		WriteString("window");
		break;
	case AGM_NODE_BITMAP:
		if (limit > 0 && ++count > limit) return 0;
		WriteString("bitmap");
		break;
	case AGM_NODE_PTR:
		if (limit > 0 && ++count > limit) return 0;
		WriteString("pointer");
		break;
	case AGM_NODE_BINARY:
		if (limit > 0 && ++count > limit) return 0;
		WriteString("binary");
		break;
	case AGM_NODE_OCX:
		if (limit > 0 && ++count > limit) return 0;
		WriteString("ocx");
		break;
	case AGM_NODE_VARIANT:
		if (limit > 0 && ++count > limit) return 0;
		WriteString("variant");
		break;
	case AGM_NODE_SQL_CONN:
		if (limit > 0 && ++count > limit) return 0;
		WriteString("connection");
		break;
	case AGM_NODE_SQL_STAT:
		if (limit > 0 && ++count > limit) return 0;
		WriteString("statement");
		break;
	default:
		if (limit > 0 && ++count > limit) return 0;
		WriteString("unknown");
		break;
	}
	return 0;
}
int AGmFile::_WriteList(AGmNode *n, int& count, int limit) {
	if (n == NULL) return 0;
	if (limit > 0 && ++count > limit) return 0;
	WriteString(", ");
	switch (n->type) {
	case AGM_NODE_LIST:
		WriteList(n->a.l.h, count, limit);
		_WriteList(n->a.l.t, count, limit);
		break;
	default:
		WriteString("other");
		break;
	}
	return 0;
}

int AGmFile::ReadSpace() {
	int ret = 0;
	for (;;) {
		char c = Input();
		if (c == ' ' || c == '\t') {
			ret++; continue;
		}
		if (nl_token == 0 && c == '\n') {
			ret++; continue;
		}
		Unput(); break;
	}
	return ret;
}
AGmString* AGmFile::ReadSpace2() {
	int ret = 0;
	AGmString *s = new AGmString(16);
	for (;;) {
		char c = Input();
		if (c == ' ' || c == '\t') {
			s->Append(c); ret++; continue;
		}
		if (nl_token == 0 && c == '\n') {
			s->Append(c); ret++; continue;
		}
		Unput(); break;
	}
	if (ret == 0) {
		delete s; s = NULL;
	}
	return s;
}
AGmString* AGmFile::ReadComment() {
#if	_WINDOWS && _64BIT
	long long pos = CurrentPoint();
#else
	long pos = CurrentPoint();
#endif
	char c;
	AGmString *str = new AGmString(512);
	for (;;) {
		c = Input();
		if (c == ' ' || c == '\t' || c == '\n') {
			str->Append(c); continue;
		}
		break;
	}
	if (c != '/') { delete str; Back(pos); return NULL; }
	str->Append(c);
	c = Input();
	if (c == '*') {
		str->Append(c);
		for (;;) {
			c = Input();
#ifdef	_WINDOWS
			if (isFirstChar(c)) {
				str->Append(c); str->Append(Input()); continue;
			} else if (c == EOF) {
				delete str; Back(pos); return NULL;
			} else if (c == '*') {
				str->Append(c);
				c = Input();
				if (c == '/') {
					str->Append(c); return str;
				}
				Unput(); continue;
			}
#endif
#ifdef	_LINUX
			int i2;
			Unput();
			if (isUTF8(this, i2)) {
				str->Append((const char*)utf8); continue;
			} else if (c == EOF) {
				delete str; Back(pos); return NULL;
			} else if (c == '*') {
				c = Input();
				str->Append(c);
				c = Input();
				if (c == '/') {
					str->Append(c); return str;
				}
				Unput(); continue;
			}
			c = Input();
#endif
			str->Append(c);
		}
	} else if (c == '/') {
		str->Append(c);
		for (;;) {
			c = Input();
#ifdef	_WINDOWS
			if (isFirstChar(c)) {
				str->Append(c); str->Append(Input());
			} else if (c == EOF) {
				return str;
			} else if (c == '\n') {
				str->Append(c); return str;
			} else {
				str->Append(c);
			}
#endif
#ifdef	_LINUX
			int i2;
			Unput();
			if (isUTF8(this, i2)) {
				str->Append((const char*)utf8);
			} else if (c == EOF) {
				c = Input();
				return str;
			} else if (c == '\n') {
				c = Input();
				str->Append(c); return str;
			} else {
				c = Input();
				str->Append(c);
			}
#endif
		}
	} else {
		delete str; Back(pos); return NULL;
	}
}
int AGmFile::ReadChar(char& c) {
	c = Input();
	if (c == EOF) {
		Unput(); return 1;
	}
	return 0;
}
#if	_WINDOWS && _64BIT
int AGmFile::ReadInteger(long long& v) {
#else
int AGmFile::ReadInteger(long& v) {
#endif
	ReadSpace();
	return _ReadInteger(v);
}
#if	_WINDOWS && _64BIT
int AGmFile::_ReadInteger(long long& v) {
#else
int AGmFile::_ReadInteger(long& v) {
#endif
	v = 0;
	int sign = 0;
	char c = Input();
	if (c == '-') {
		sign = 1; c = Input();
	}
	if (!isdigit(c)) {
		Unput(); if (sign) Unput(); return 1;
	}
	v = c - '0';
	for (;;) {
		c = Input();
		if (isdigit(c)) {
			v = v * 10 + c - '0'; continue;
		}
		Unput();
		break;
	}
	if (sign) v = -v;
	return 0;
}
#if	_WINDOWS && _64BIT
int AGmFile::ReadHexa(long long& v) {
#else
int AGmFile::ReadHexa(long& v) {
#endif
	ReadSpace();
	char c = Input();
	if (!isxdigit(c)) {
		Unput(); return 1;
	}
	v = HexaToNum(c);
	for (;;) {
		c = Input();
		if (isxdigit(c)) {
			v = (v << 4) | HexaToNum(c); continue;
		}
		Unput();
		break;
	}
	return 0;
}
int AGmFile::ReadReal(double& v) {
	v = 0;
#if	_WINDOWS && _64BIT
	long long pos = CurrentPoint();
#else
	long pos = CurrentPoint();
#endif
	char ss[512]; int idx = 0;
	for (;;) {
		ReadSpace();
		int dot = 0;
		short count1, count2 = 0;
		char c = Input();
		if (c == '-') {
			ss[idx++] = c;
			c = Input();
		}
		for (v = 0, count1 = 0; isdigit(c); c = Input()) {
			ss[idx++] = c;
			count1++;
		}
		if (c == '.') { // real number must inclue "."
			ss[idx++] = c;
			dot = 1;
			c = Input();
			for (count2 = 0; isdigit(c); c = Input()) {
				ss[idx++] = c; count2++;
			}
		}
		if (dot == 0 || (count1 == 0 && count2 == 0)) {
			Back(pos); return 1;
		}
		pos = 1;
		if (c == 'e') {
			ss[idx++] = c;
			c = Input();
			if (c == '+') {
				ss[idx++] = c;
				c = Input();
			} else if (c == '-') {
				ss[idx++] = c;
				c = Input();
			}
			for (; isdigit(c); c = Input()) {
				ss[idx++] = c;
			}
		}
		Unput();
		break;
	}
	ss[idx] = 0;
	art_sscanf_1(ss, "%lg", &v);
	return 0;
}
int AGmFile::ReadIdent(AGmString*& s) {
	ReadSpace();
	s = new AGmString(16);
	for (;;) {
		char c = Input();
		if (isIdentHead(c)) {
			s->Append(c);
			break;
		}
#ifdef	_WINDOWS
		if (isFirstChar(c)) {
			s->Append(c);
			s->Append(Input());
			break;
		}
#endif
#ifdef	_LINUX
		int i2;
		Unput();
		if (isUTF8(this, i2)) {
			s->Append((const char*)utf8);
			break;
		}
		c = Input();
#endif
		Unput();
		delete s;
		return 1;
	}
	for (;;) {
		char c = Input();
		if (isIdentTail(c)) {
			s->Append(c);
			continue;
		}
#ifdef	_WINDOWS
		if (isFirstChar(c)) {
			s->Append(c);
			s->Append(Input());
			continue;
		}
		Unput();
#endif
#ifdef	_LINUX
		int i2;
		Unput();
		if (isUTF8(this, i2)) {
			s->Append((const char*)utf8);
			continue;
		}
#endif
		break;
	}
	return 0;
}
int AGmFile::ReadJIdent(AGmString*& s) {
	ReadSpace();
	s = new AGmString(16);
	for (;;) {
		char c = Input();
		if (isIdentHead(c)) {
			s->Append(c);
			break;
		}
#ifdef	_WINDOWS
		if (isFirstChar(c)) {
			s->Append(c);
			s->Append(Input());
			break;
		}
		Unput();
#endif
#ifdef	_LINUX
		int i2;
		Unput();
		if (isUTF8(this, i2)) {
			s->Append((const char*)utf8);
			break;
		}
#endif
		delete s;
		return 1;
	}
	for (;;) {
		char c = Input();
		if (isIdentTail(c)) {
			s->Append(c);
			continue;
		}
#ifdef	_WINDOWS
		if (isFirstChar(c)) {
			s->Append(c);
			s->Append(Input());
			continue;
		}
#endif
#ifdef	_LINUX
		int i2;
		Unput();
		if (isUTF8(this, i2)) {
			s->Append((const char*)utf8);
			continue;
		}
		c = Input();
#endif
		if (c == '.' || c == '$') {
			s->Append(c);
			continue;
		}
		Unput();
		break;
	}
	return 0;
}
int AGmFile::ReadXIdent(AGmString*& s) {
	ReadSpace();
	s = new AGmString(16);
	for (;;) {
		char c = Input();
		if (isIdentTail(c)) {
			s->Append(c);
			break;
		}
#ifdef	_WINDOWS
		if (isFirstChar(c)) {
			s->Append(c);
			s->Append(Input());
			break;
		}
		Unput();
#endif
#ifdef	_LINUX
		int i2;
		Unput();
		if (isUTF8(this, i2)) {
			s->Append((const char*)utf8);
			break;
		}
#endif
		delete s;
		return 1;
	}
	for (;;) {
		char c = Input();
		if (isIdentTail(c)) {
			s->Append(c);
			continue;
		}
#ifdef	_WINDOWS
		if (isFirstChar(c)) {
			s->Append(c);
			s->Append(Input());
			continue;
		}
#endif
#ifdef	_LINUX
		int i2;
		Unput();
		if (isUTF8(this, i2)) {
			s->Append((const char*)utf8);
			continue;
		}
		c = Input();
#endif
		if (c == '.' || c == '-' || c == ':') {
			s->Append(c);
			continue;
		}
		Unput();
		break;
	}
	return 0;
}
int AGmFile::ReadString(AGmString*& s) {
	ReadSpace();
	char c = Input();
	if (c == EOF) {
		s = NULL; return 1;
	}
	s = new AGmString(16);
	for (;;) {
#ifdef	_WINDOWS
		if (isFirstChar(c)) {
			s->Append(c);
			s->Append(Input());
		} else {
			s->Append(c);
		}
#endif
#ifdef	_LINUX
		int i2;
		Unput();
		if (isUTF8(this, i2)) {
			s->Append((const char*)utf8);
		} else {
			c = Input();
			s->Append(c);
		}
#endif
		c = Input();
		if (c == EOF || c == ' ' || c == '\t' || c == '\n') break;
	}
	Unput();
	return 0;
}
int AGmFile::ReadLine(AGmString*& s) {
	int count = 0;
	s = new AGmString(64);
	char c;
	for (;;) {
		c = Input();
		if (c == ' ' || c == '\t') {
			s->Append(c); count++; continue;
		} else if (c == '\n') {
			return 0;
		}
		break;
	}
	if (c == EOF) {
		if (count > 0) return 0;
		delete s; s = NULL; return 1;
	}
	Unput();
	for (;;) {
		c = Input();
		if (c == EOF) {
			return 0;
		} else if (c == '\n') {
			return 0;
		} else if (Type() != AGM_FILE_BIN_READ) {
#ifdef	_WINDOWS
			if (isFirstChar(c)) {
				s->Append(c); s->Append(Input());
			} else {
				s->Append(c);
			}
#endif
#ifdef	_LINUX
			int i2;
			Unput();
			if (isUTF8(this, i2)) {
				s->Append((const char*)utf8);
			} else {
				c = Input();
				s->Append(c);
			}
#endif
		} else {
			s->Append(c);
		}
	}
}
int AGmFile::ReadQChar(char& c) {
	ReadSpace();
	long pos = 0;
	if (!IsChar('\'')) return 1;
	pos++;
	c = Input(); pos++;
	if (c == '\\') {
		c = Input(); pos++;
		if (c == 't') c = '\t';
		if (c == 'r') c = '\r';
		if (c == 'n') c = '\n';
	}
	if (!IsChar('\'')) {
		Unput(pos); return 1;
	}
	return 0;
}
int AGmFile::ReadQString(AGmString*& s, int b_esc) {
	ReadSpace();
#if	_WINDOWS && _64BIT
	long long pos = CurrentPoint();
#else
	long pos = CurrentPoint();
#endif
	char c = Input();
	if (c == EOF || c != '\"') {
		s = NULL; Back(pos); return 1;
	}
	c = Input();
	s = new AGmString(16);
	for (;;) {
		if (c == EOF || c == '\"') break;
#ifdef	_WINDOWS
		if (isFirstChar(c)) {
			s->Append(c);
			c = Input();
			s->Append(c);
		} else if (b_esc) {
			if (c == '\\') {
				c = Input();
				if (c == 't') {
					s->Append('\t');
				} else if (c == 'r') {
					s->Append('\r');
				} else if (c == 'n') {
					s->Append('\n');
				} else {
					s->Append(c);
				}
			} else {
				s->Append(c);
			}
		} else {
			s->Append(c);
		}
#endif
#ifdef	_LINUX
		int i2;
		Unput();
		if (isUTF8(this, i2)) {
			s->Append((const char*)utf8);
		} else if (b_esc) {
			c = Input();
			if (c == '\\') {
				c = Input();
				if (c == 't') {
					s->Append('\t');
				} else if (c == 'r') {
					s->Append('\r');
				} else if (c == 'n') {
					s->Append('\n');
				} else {
					s->Append(c);
				}
			} else {
				s->Append(c);
			}
		} else {
			c = Input();
			s->Append(c);
		}
#endif
		c = Input();
	}
	if (c == EOF) {
		delete s; s = NULL; Back(pos); return 1;
	}
	return 0;
}
int AGmFile::ReadSingleQString(AGmString*& s) {
	ReadSpace();
	return _ReadSingleQString(s);
}
int AGmFile::_ReadSingleQString(AGmString*& s) {
#if	_WINDOWS && _64BIT
	long long pos = CurrentPoint();
#else
	long pos = CurrentPoint();
#endif
	char c = Input();
	if (c == EOF || c != '\'') {
		s = NULL; Back(pos); return 1;
	}
	s = new AGmString(16);
	c = Input();
	for (;;) {
		if (c == EOF) break;
		if (c == '\'') {
			c = Input();
			// is escape
			if (c == '\'') {
				s->Append('\'');
				s->Append('\'');
				c = Input();
				continue;
			}
			// last
			Unput(2);
			c = Input();
			break;
		}
#ifdef	_WINDOWS
		if (isFirstChar(c)) {
			s->Append(c);
			c = Input();
			if (c == EOF) break;
			s->Append(c);
		} else {
			s->Append(c);
		}
#endif
#ifdef	_LINUX
		int i2;
		Unput();
		if (isUTF8(this, i2)) {
			s->Append((const char*)utf8);
			c = Input();
			if (c == EOF) break;
			s->Append(c);
		} else {
			c = Input();
			s->Append(c);
		}
#endif
		c = Input();
	}
	if (c == EOF) {
		delete s; s = NULL; Back(pos); return 1;
	}
	return 0;
}
int AGmFile::ReadMatrix(AGmMatrix *&m) {
#if	_WINDOWS && _64BIT
	long long r, c;
#else
	long r, c;
#endif
	if (ReadInteger(r)) return 1;
	if (ReadInteger(c)) return 1;
	m = new AGmMatrix((long)r, (long)c);
	for (long i = 0; i < r; i++)
	for (long j = 0; j < c; j++) {
		if (ReadReal(m->Component(i, j))) return 1;
	}
	return 0;
}

int AGmFile::ReadChar(AGmNode*& n) {
	long i; char c;
	ReadChar(c);
	if (c == EOF) {
		i = -1;
	} else {
		i = (unsigned char)c;
	}
	n = Integer(i);
	return 0;
}
int AGmFile::ReadInteger(AGmNode*& n) {
	AGmNode *n2 = NULL;
	if (ReadBigInteger(n2)) {
		return 1;
	}
	n = BigInteger::ToInteger(n2);
	if (n) {
		RefUnref(n2);
	} else {
		n = n2;
	}
	return 0;
}
int AGmFile::ReadBigInteger(AGmNode*& n) {
#if	_WINDOWS && _64BIT
	long long pos = CurrentPoint();
#else
	long pos = CurrentPoint();
#endif
	ReadSpace();
	AGmString *str = new AGmString(16);
	char c = Input();
	if (c == '-') {
		str->Append(c);
	} else {
		Unput();
	}
	int len = 0;
	for (;;) {
		c = Input();
		if ('0' <= c && c <= '9') {
			str->Append(c);
			len++;
		} else {
			Unput();
			break;
		}
	}
	if (len == 0) {
		Back(pos);
		delete str; return 1;
	}
	n = BigInteger::FromString(str->Str());
	delete str; return 0;
}
int AGmFile::ReadHexa(AGmNode*& n) {
#if	_WINDOWS && _64BIT
	long long i;
#else
	long i;
#endif
	if (ReadHexa(i)) {
		n = NULL; return 1;
	}
	n = Integer(i);
	return 0;
}
int AGmFile::ReadReal(AGmNode*& n) {
	double r;
	if (ReadReal(r)) {
		n = NULL; return 1;
	}
	n = Real(r);
	return 0;
}
int AGmFile::ReadIdent(AGmNode*& n) {
	AGmString *s;
	if (ReadIdent(s)) {
		n = NULL; return 1;
	}
	n = String(s);
	return 0;
}
int AGmFile::ReadJIdent(AGmNode*& n) {
	AGmString *s;
	if (ReadJIdent(s)) {
		n = NULL; return 1;
	}
	n = String(s);
	return 0;
}
int AGmFile::ReadXIdent(AGmNode*& n) {
	AGmString *s;
	if (ReadXIdent(s)) {
		n = NULL; return 1;
	}
	n = String(s);
	return 0;
}
int AGmFile::ReadString(AGmNode*& n) {
	AGmString *s;
	if (ReadString(s)) {
		n = NULL; return 1;
	}
	n = String(s);
	return 0;
}
int AGmFile::ReadLine(AGmNode*& n) {
	AGmString *s;
	if (ReadLine(s)) {
		n = NULL; return 1;
	}
	n = String(s);
	return 0;
}
int AGmFile::ReadQChar(AGmNode*& n) {
	char c;
	if (ReadQChar(c)) {
		n = NULL; return 1;
	}
	n = Integer((unsigned char)c);
	return 0;
}
int AGmFile::ReadQString(AGmNode*& n, int b_esc) {
	AGmString *s;
	if (ReadQString(s, b_esc)) {
		n = NULL; return 1;
	}
	n = String(s);
	return 0;
}
int AGmFile::ReadSingleQString(AGmNode*& n) {
	AGmString *s;
	ReadSpace();
	if (_ReadSingleQString(s)) {
		n = NULL; return 1;
	}
	n = String(s);
	return 0;
}
int AGmFile::ReadMatrix(AGmNode*& n) {
	AGmMatrix *m;
	if (ReadMatrix(m)) {
		n = NULL; return 1;
	}
	n = Matrix(m);
	return 0;
}
int AGmFile::ReadCsv(AGmNode*& n) {
	long col_no = 1;
	char c = Input();
	if (c == EOF) { n = Integer(1); Unput(); return 0; }
	n = Cons(NULL);
	for (;;) {
		AGmString *s = new AGmString(16);
		if (c != ',') {
			if (c != '\"') {
				for (;;) {
					if (c == ',' || c == '\n' || c == EOF) break;
#ifdef	_WINDOWS
					if (isFirstChar(c)) {
						s->Append(c);
						c = Input();
						if (c == EOF) break;
						s->Append(c);
					} else {
						s->Append(c);
					}
#endif
#ifdef	_LINUX
					int i2;
					Unput();
					if (isUTF8(this, i2)) {
						s->Append((const char*)utf8);
						c = Input();
						if (c == EOF) break;
						s->Append(c);
					} else {
						c = Input();
						s->Append(c);
					}
#endif
					c = Input();
				}
			} else {
				for (;;) {
					c = Input();
					if (c == EOF) break;
					if (c == '\"') {
						c = Input();
						// is escape
						if (c == '\"') {
							s->Append('\"');
							continue;
						}
						// last
						break;
					}
#ifdef	_WINDOWS
					if (isFirstChar(c)) {
						s->Append(c);
						c = Input();
						if (c == EOF) break;
						s->Append(c);
					} else {
						s->Append(c);
					}
#endif
#ifdef	_LINUX
					int i2;
					Unput();
					if (isUTF8(this, i2)) {
						s->Append((const char*)utf8);
						c = Input();
						if (c == EOF) break;
						s->Append(c);
					} else {
						c = Input();
						s->Append(c);
					}
#endif
				}
			}
		}
		CreateArc(n, String(s), Integer(col_no++));
		if (c == ',') { c = Input(); continue; }
		if (c == '\n' || c == EOF) break;
	}
	if (c == EOF) {
		RefUnref(n); n = NULL; return 1;
	}
	return 0;
}
int AGmFile::ReadList(AGmNode *&n) {
#if	_WINDOWS && _64BIT
	long long p = CurrentPoint();
#else
	long p = CurrentPoint();
#endif
	if (MatchString("()") || MatchString("null")) {
		n = NULL; return 0;
	} else if (ReadReal(n) == 0) {
		return 0;
	} else if (ReadInteger(n) == 0) {
		return 0;
	} else if (ReadQString(n) == 0) {
		return 0;
	}
	AGmNode *h = NULL, *t = NULL;
	if (MatchString("(") == 0) {
		return 1;
	}
	if (ReadList(h) || _ReadList(t) || MatchString(")") == 0) {
		RefUnref(h, t); Back(p); return 1;
	}
	n = Cons(h, t);
	return 0;
}
int AGmFile::_ReadList(AGmNode *&n) {
	if (MatchString(",") == 0) {
		n = NULL; return 0;
	}
	AGmNode *h = NULL, *t = NULL;
	if (ReadList(h) || _ReadList(t)) {
		RefUnref(h, t); return 1;
	}
	n = Cons(h, t);
	return 0;
}

int AGmFile::Search(const char* s, AGmString* str) {
#if	_WINDOWS && _64BIT
	long long pos = CurrentPoint();
#else
	long pos = CurrentPoint();
#endif
	int pos2 = (int)pos;
	char *cp, c2, c, c3;
	for (;;) {
		cp = (char*)s;
		c2 = *cp++;
		c3 = 0;
		for (;;) {
			c = Input();
			if (c3 == 0) c3 = c;
			if (c == EOF) { Back(pos); return 0; }
			if (c == c2) break;
		}
		for (;;) {
			c2 = *cp++;
			if (c2 == 0) break;
			c = Input();
			if (c == EOF) { Back(pos); return 0; }
			if (c != c2) break;
		}
		if (c2 == 0) break;
		if (str) str->Append(c3);
		pos2++;
		Back(pos2);
	}
	return 1;
}
int AGmFile::Search(const char ch, AGmString* str) {
#if	_WINDOWS && _64BIT
	long long pos = CurrentPoint();
#else
	long pos = CurrentPoint();
#endif
	for (;;) {
		char c = Input();
		if (c == EOF) { Back(pos); return 0; }
		if (c == ch) break;
		if (str) str->Append(c);
	}
	return 1;
}
int AGmFile::SearchIdent(const char* s, AGmString* str) {
#if	_WINDOWS && _64BIT
	long long pos = CurrentPoint();
#else
	long pos = CurrentPoint();
#endif
	char *cp, c, c2, c3;
	for (;;) {
		cp = (char*)s;
		c2 = *cp++;
		int flag = 0;
		for (;;) {
			c = Input();
			if (c == EOF) { Back(pos); return 0; }
			if (flag == 0 && c == c2) break;
			if (str) str->Append(c);
			flag = isIdentTail(c);
		}
#if	_WINDOWS && _64BIT
		long long pos2 = CurrentPoint();
#else
		long pos2 = CurrentPoint();
#endif
		c3 = c2;
		for (;;) {
			c2 = *cp++;
			if (c2 == 0) break;
			c = Input();
			if (c == EOF) { Back(pos); return 0; }
			if (c != c2) break;
		}
		if (c2 == 0) {
			c = Input();
			if (!isIdentHead(c)) {
				Unput(); break;
			}
		}
		if (str) str->Append(c3);
		pos2++;
		Back(pos2);
	}
	return 1;
}

int AGmFile::MatchIdent(const char *s, int ignore_case) {
	ReadSpace();
	long pos;
	for (pos = 0; *s; s++) {
		char c = Input();
		pos ++;
		if (ignore_case == 0) {
			if (c == EOF || c != *s) {
				Unput(pos); return 0;
			}
		} else {
			if (c == EOF || !isNoCaseEqual(c, *s)) {
				Unput(pos); return 0;
			}
		}
	}
	char c = Input();
	pos ++;
	if (isIdentHead(c)) {
		Unput(pos); return 0;
	}
#ifdef	_WINDOWS
	if (isFirstChar(c)) {
		Unput(pos); return 0;
	}
	Unput();
#endif
#ifdef	_LINUX
	int i2;
	Unput();
	if (isUTF8(this, i2)) {
		Unput(pos-1); return 0;
	}
#endif
	return 1;
}
int AGmFile::MatchIdent(const AGmString *s, int ignore_case) {
	return MatchIdent(s->Str(), ignore_case);
}
int AGmFile::MatchString(const char *s, int ignore_case) {
#if	_WINDOWS && _64BIT
	long long pos = CurrentPoint();
#else
	long pos = CurrentPoint();
#endif
	ReadSpace();
	int flag = _MatchString(s, ignore_case);
	if (flag == 0) Back(pos);
	return flag;
}
int AGmFile::_MatchString(const char *s, int ignore_case) {
	for (long pos = 0; *s; s++) {
		if (*s == 0) break;
		char c = Input();
		pos ++;
		if (ignore_case == 0) {
			if (c == EOF || c != *s) {
				Unput(pos); return 0;
			}
		} else {
			if (c == EOF || !isNoCaseEqual(c, *s)) {
				Unput(pos); return 0;
			}
		}
	}
	return 1;
}
int AGmFile::MatchString(const AGmString *s, int ignore_case) {
	return MatchString(s->Str(), ignore_case);
}
int AGmFile::IsEnd() {
	ReadSpace();
	char c = Input();
	Unput();
	return (c == EOF);
}

long AGmFile::GetChar() {
	char c1 = _Input();
	if (c1 == EOF) return EOF;
	unsigned char c11 = (unsigned char)c1;
#ifdef	_WINDOWS
	if (!isFirstChar(c1)) {
		return c11;
	}
	char c2 = _Input();
	if (c2 == EOF) return EOF;
	unsigned char c22 = (unsigned char)c2;
	return (c11 << 8) + c22;
#endif
#ifdef	_LINUX
	int i2;
	long val;
	Unput();
	if (!isUTF8(this, i2, &val)) {
		_Input();
		return c11;
	} else {
		return val;
	}
#endif
}

#if	_WINDOWS && _64BIT
long long AGmFile::CurrentPoint() {
#else
long AGmFile::CurrentPoint() {
#endif
	if (type == AGM_FILE_STR_READ) {
		return (str_ptr - str->Str());
	}
	return point;
}
#if _WINDOWS && _64BIT
void AGmFile::Back(long long back_point) {
#else
void AGmFile::Back(long back_point) {
#endif
	if (type == AGM_FILE_STR_READ) {
		char *buf = str_ptr;
		str_ptr = (char*)(str->Str() + back_point);
		while ((unsigned long)str_ptr < (unsigned long)buf) {
			if (*(--buf) == '\n') nl_count--;
		}
		num_eof = 0;
		return;
	}
	if (back_point < offset) {
		char msg[256];
		art_sprintf_2(msg, 256, "AGmFile::Back(pos): truncate pos from %ld to offset %ld.", back_point, offset);
		ADvWindow::Print(msg);
		back_point = offset;
	}
#if	_WINDOWS && _64BIT
	long long p = point - back_point;
#else
	long p = point - back_point;
#endif
	if (auto_expand_unput_buffer == 0 && p < 0) p += unput_buffer_size;
	Unput((long)p);
}
long AGmFile::LineNumber() {
	return nl_count + 1;
}
void AGmFile::LineNumber(long lno) {
	nl_count = lno - 1;
}

char AGmFile::Input() {
	for (;;) {
		char c = _Input();
		if (c == '\r') continue;
		return c;
	}
}
char AGmFile::_Input() {
	char c;
	if (type == AGM_FILE_STR_READ) {
		if (*str_ptr) {
			c = *str_ptr++;
			if (c == '\n') nl_count++;
			return c;
		} else {
			num_eof++;
			return EOF;
		}
	}
	if (point != to) {
		c = unputBuffer[point];
if (debug_input_unput) {
char msg[256];
art_sprintf_4(msg, 256, "buffer: (point, to) = (%ld, %ld), c = %x, c = %c\n", point, to, c, c);
ADvWindow::Print(msg);
}
		point++;
		if (auto_expand_unput_buffer == 0 && point >= unput_buffer_size) point -= unput_buffer_size;
		if (c == '\n') nl_count++;
		return c;
	}
	int cc;
	switch (type) {
	case AGM_FILE_READ:
	case AGM_FILE_BIN_READ:
	case AGM_FILE_STDIN:
		if (auto_expand_unput_buffer) {
			if (unput_buffer_size - (to - offset) <= PRE_READ_CC) {
				ExpandUnputBuffer(UNPUT_BUFFER_SIZE);
			}
		}
		cc = art_read(art_fileno(fp), &unputBuffer[point], PRE_READ_CC);
		if (cc <= 0) {
			unputBuffer[point] = EOF;
			cc = 1;
		}
		to = point + cc;
		if (auto_expand_unput_buffer == 0 && to >= unput_buffer_size) {
			to -= unput_buffer_size;
			for (int i = 0; i <= to; i++)
				unputBuffer[i] = unputBuffer[i+unput_buffer_size];
		}
		c = unputBuffer[point];
if (debug_input_unput) {
char msg[256];
art_sprintf_4(msg, 256, "file:   (point, to) = (%ld, %ld), c = %x, c = %c\n", point, to, c, c);
ADvWindow::Print(msg);
}
		point++;
		if (auto_expand_unput_buffer == 0 && point >= unput_buffer_size) point -= unput_buffer_size;
		break;
	case AGM_FILE_FD_IN:
		cc = art_read(fd, &unputBuffer[point], PRE_READ_CC);
		if (cc <= 0) {
			unputBuffer[point] = EOF;
			cc = 1;
		}
		to = point + cc;
		if (auto_expand_unput_buffer == 0 && to >= unput_buffer_size) {
			to -= unput_buffer_size;
			for (int i = 0; i <= to; i++)
				unputBuffer[i] = unputBuffer[i+unput_buffer_size];
		}
		c = unputBuffer[point];
		point++;
		if (auto_expand_unput_buffer == 0 && point >= unput_buffer_size) point -= unput_buffer_size;
		break;
	case AGM_FILE_PIPE:
#ifdef	_WINDOWS
		for (;;) {
			::EnterCriticalSection(&cs);
			if (rbuf2) {
				if (r_index < rbuf2->Length()) {
					c = rbuf2->Str()[r_index++];
					::LeaveCriticalSection(&cs);
					break;
				} else {
					delete rbuf2; rbuf2 = NULL;
				}
			}
			if (rbuf1) {
				rbuf2 = rbuf1; rbuf1 = NULL; r_index = 0;
				::LeaveCriticalSection(&cs);
				continue;
			}
			::LeaveCriticalSection(&cs);
			wait = 1;
			::SetEvent(retEvent);
			::WaitForSingleObject(contEvent, INFINITE);
			::ResetEvent(contEvent);
		}
#endif
#ifdef	_LINUX
		for (;;) {
			g_mutex_lock(&mutex);
			if (rbuf2) {
				if (r_index < rbuf2->Length()) {
					c = rbuf2->Str()[r_index++];
					g_mutex_unlock(&mutex);
					break;
				} else {
					delete rbuf2; rbuf2 = NULL;
				}
			}
			if (rbuf1) {
				rbuf2 = rbuf1; rbuf1 = NULL; r_index = 0;
				g_mutex_unlock(&mutex);
				continue;
			}
			wait = 1;

			retCondFlag = 1;
			g_cond_signal(&retCond);
			g_mutex_unlock(&mutex);

			g_mutex_lock(&mutex);
			while (contCondFlag == 0) {
				g_cond_wait(&contCond, &mutex);
			}
			contCondFlag = 0;
			g_mutex_unlock(&mutex);
		}
#endif
		unputBuffer[point] = c;
		point++;
		if (auto_expand_unput_buffer == 0 && point >= unput_buffer_size) point -= unput_buffer_size;
		to = point;
		break;
	default:
		return EOF;
	}
	if (c == '\n') nl_count++;
	return c;
}

int AGmFile::Output(char c) {
	switch (type) {
	case AGM_FILE_WRITE:
	case AGM_FILE_APPEND:
	case AGM_FILE_BIN_WRITE:
	case AGM_FILE_BIN_APPEND:
	case AGM_FILE_STDOUT:
	case AGM_FILE_STDERR:
		if (fputc(c, fp) == EOF) throw new AlException(DISK_IS_FULL, AL_STR("disk full."));
		break;
	case AGM_FILE_FD_OUT:
		art_write(fd, &c, 1);
		break;
	case AGM_FILE_STR_WRITE:
	case AGM_FILE_STR_APPEND:
		str->Append(c);
		break;
	case AGM_FILE_PIPE:
#ifdef	_WINDOWS
		unsigned long num;
		num = ::WriteFile(wStdinPipe, &c, 1, &num, NULL);
		ASSERT(num == 1);
#endif
#ifdef	_LINUX
		write(stdin_pipe[1], &c, 1);
#endif
		break;
	default:
		return 1;
	}
	if (_doc && _delim && c == '\n') {
		AGmNode *doc = _doc; _doc = NULL;
		AGmNode *delim = _delim; _delim = NULL;
		WriteString(doc->a.s.v);
		WriteString(delim->a.s.v);
		WriteString("\n");
		Unref(doc, delim);
	}
	return 0;
}

void AGmFile::Unput(long p) {
	if (type == AGM_FILE_STR_READ) {
		if (num_eof >= p) { num_eof -= p; return; }
		char *buf = str_ptr;
		str_ptr -= (p - num_eof);
		while ((unsigned long)str_ptr < (unsigned long)buf) {
			if (*(--buf) == '\n') nl_count--;
		}
		num_eof = 0;
		return;
	}
	long i, n = point - p;
	if (auto_expand_unput_buffer == 0 && n < 0) n += unput_buffer_size;

	for (i = 0; i < p; i++) {
		long pp = n + i;
		if (auto_expand_unput_buffer == 0 && pp > unput_buffer_size) pp -= unput_buffer_size;
		char c = unputBuffer[pp];
		if (c == '\n') nl_count--;
	}
	point = n;
	if (redirect_pos >= 0 && point < redirect_pos) {
		for (long i = redirect_pos + redirect_size; i < to; i++) {
			unputBuffer[i - redirect_size] = unputBuffer[i];
		}
		to -= redirect_size; redirect_pos = -1;
if (debug_input_unput) {
char msg[256];
art_sprintf_0(msg, 256, "redirect clear.\n");
ADvWindow::Print(msg);
}
	}

if (debug_input_unput) {
char msg[256];
art_sprintf_3(msg, 256, "unput:  (point, to) = (%ld, %ld), p = %ld\n", point, to, p);
ADvWindow::Print(msg);
}
}

int AGmFile::IsChar(char c) {
	if (Input() == c) return 1;
	Unput();
	return 0;
}

void AGmFile::Flush() {
	switch (type) {
	case AGM_FILE_WRITE:
	case AGM_FILE_APPEND:
	case AGM_FILE_BIN_WRITE:
	case AGM_FILE_BIN_APPEND:
	case AGM_FILE_STDOUT:
	case AGM_FILE_STDERR:
		fflush(fp);
		break;
	case AGM_FILE_FD_OUT:
#ifdef	_LINUX
		fsync(fd);
#endif
		break;
	case AGM_FILE_PIPE:
#ifdef	_LINUX
		fsync(stdin_pipe[1]);
#endif
		break;
	default:
		break;
	}
}

int AGmFile::AddChar(long ch) {
#ifdef	_WINDOWS
	if (0xff00 & ch) {
		Output((char)(ch >> 8));
		return Output((char)ch);
	} else {
		return Output((char)ch);
	}
#endif
#ifdef	_LINUX
	toUTF8(this, ch);
	return (int)ch;
#endif
}

void AGmFile::AutoExpandUnputBuffer(int flag) {
	if (type == AGM_FILE_STR_READ) return;
	auto_expand_unput_buffer = flag;
}
void AGmFile::Cut() {
	if (auto_expand_unput_buffer) {
		for (long i = 0; i + point < offset + unput_buffer_size; i++) {
			unputBuffer[i + offset] = unputBuffer[i + point];
		}
		unputBuffer += offset;
		offset += (point - offset);
		unputBuffer -= offset;
	}
}
long AGmFile::UnputBufferSize() {
	return unput_buffer_size;
}

void AGmFile::NewlineToken(int flag) {
	nl_token = flag;
}
int AGmFile::NewlineToken() {
	return nl_token;
}

int AGmFile::Redirect(const char *s) {
	if (auto_expand_unput_buffer == 0) return 1;
	if (redirect_pos == point) return 0;
	redirect_pos = point;
if (debug_input_unput) {
char msg[256];
art_sprintf_3(msg, 256, "redirect:  (point, to) = (%ld, %ld), s = %s\n", point, to, s);
ADvWindow::Print(msg);
}
	redirect_size = (long)strlen(s);
	long size = to - point, i;
	if (auto_expand_unput_buffer == 0 && size < 0) size += unput_buffer_size;
	if (unput_buffer_size - (to - offset) <= redirect_size) {
		ExpandUnputBuffer(UNPUT_BUFFER_SIZE);
	}
	for (i = size-1; i >= 0; i--) {
		unputBuffer[point + i + redirect_size] = unputBuffer[point + i];
	}
	to += redirect_size;
	long pp = point;
	for (i = 0; i < redirect_size; i++) {
		unputBuffer[pp++] = s[i];
	}
	return 0;
}
void AGmFile::DebugInputUnput(int flag) {
	debug_input_unput = flag;
}

unsigned char AGmFile::HexaToNum(char c) {
	if (islower(c)) return (unsigned char)(c - 'a' + 10);
	if (isupper(c)) return (unsigned char)(c - 'A' + 10);
	return (unsigned char)(c - '0');
}

int AGmFile::BeginUnputBuffer() {
	unputBuffer = new char [unput_buffer_size + PRE_READ_CC];
	offset = point = to = 0;
	nl_count = 0;
	nl_token = 0;
	return 0;
}
void AGmFile::EndUnputBuffer() {
	unputBuffer += offset;
	delete [] unputBuffer;
}
void AGmFile::ExpandUnputBuffer(int expand_size) {
	ASSERT(point <= to);
	long size = unput_buffer_size + expand_size;
	char *buf = new char[size + PRE_READ_CC];
	unputBuffer += offset;
	memcpy(buf, unputBuffer, unput_buffer_size);
	unput_buffer_size = size;
	delete [] unputBuffer; unputBuffer = buf - offset;
}

int AGmFile::CreateStrReadFile(AGmFile*&f, AGmString*& s, const char *ss) {
	s = new AGmString(ss);
	f = new AGmFile();
	return f->Create(s, AGM_FILE_STR_READ);
}
void AGmFile::DeleteStrReadFile(AGmFile *f, AGmString *s,
			AGmNode *s1, AGmNode *s2, AGmNode *s3, AGmNode *s4,
			AGmNode *s5, AGmNode *s6, AGmNode *s7, AGmNode *s8) {
	Unref(s1, s2, s3, s4, s5, s6, s7, s8);
	delete f;
	AGmString::DeleteString(s);
}

// ======================== File I/O ============================
// ---- tree data
#define TREE_INDENT(i,d) { for (i = 0; i<d; i++) WriteString("\t"); }
int AGmFile::WriteTree(AGmNode *n) {
	int count = 0;
	WriteList(n, count);
	WriteTree(1, n);
	return 0;
}
int AGmFile::WriteTree(int depth, AGmNode *n) {
	int i, first = 1, count = 0;
	AGmLink *l;
	for (l = next_outlink(n, NULL); l; l = next_outlink(n, l)) {
		if (first) {
			first = 0;
			WriteString(" {\n");
		} else {
		}
		AGmNode *a2 = l->arc->attr;
		AGmNode *n2 = l->arc->dst;
		TREE_INDENT(i, depth)
		WriteList(a2, count);
		WriteString("\t");
		WriteList(n2, count);
		WriteTree(depth + 1, n2);
	}
	if (first) {
		WriteString("\n");
	} else {
		TREE_INDENT(i, depth - 1)
		WriteString("}\n");
	}
	return 0;
}
int AGmFile::ReadTree(AGmNode *&n) {
	int err = ReadList(n);
	if (err) return err;
	if (MatchString("{")) {
		AGmNode *a2, *n2;
		for (;;) {
			if (MatchString("}")) break;
			err = ReadList(a2);
			if (err) { RefUnref(n); return err; }
			err = ReadTree(n2);
			if (err) { RefUnref(n, a2); return err; }
			CreateArc(n, n2, a2);
		}
	}
	return 0;
}
int AGmFile::WriteTree(AGmNode *n, const char *path) {
	AGmFile *f = new AGmFile();
	int flag;
	flag = f->Create(path, AGM_FILE_WRITE);
	if (flag) { delete f; return flag; }
	flag = f->WriteTree(n);
	delete f;
	return flag;
}
int AGmFile::WriteTree(AGmNode *n, AGmString *path) {
	return WriteTree(n, path->Str());
}
int AGmFile::ReadTree(AGmNode*& n, const char *path) {
	AGmFile *f = new AGmFile();
	int flag;
	flag = f->Create(path, AGM_FILE_READ);
	if (flag) { delete f; return flag; }
	flag = f->ReadTree(n);
	delete f;
	return flag;
}
int AGmFile::ReadTree(AGmNode*& n, AGmString *path) {
	return ReadTree(n, path->Str());
}
// ---- graph data
int AGmFile::WriteGraph(AGmNode *n) {
	nid = 0;
	MarkNode(n, AGM_FLAG_SELF);
	AttachNid(n);

	WriteString("B ");
	WriteInteger(nid);
	WriteString("\n");	

	nid = 0;
	WriteNode(n);
	nid = 0;
	WriteArc(n);

	WriteString("E\n");

	DetachNid(n);
	UnmarkNode(n, AGM_FLAG_SELF);
	return 0;
}

int AGmFile::WriteNode(AGmNode *n) {
#ifdef	_WINDOWS
	USES_CONVERSION;
#endif
	while (n) {
		if (n->body->nid != nid+1) break;
		nid++;

		WriteString("N ");
		WriteInteger(n->body->nid);

		AGmNode *t = NULL;
		switch (n->type) {
		case AGM_NODE_INTEGER:
		case AGM_NODE_BIGINT:
			WriteString(" I ");
			WriteInteger(n);
			WriteString("\n");
			t = n->a.i.t;
			break;
		case AGM_NODE_GFP:
			WriteString(" g ");
			WriteInteger(n->a.gp.v);
			WriteString(" ");
			WriteInteger(n->a.gp.mod);
			t = NULL;
			break;
		case AGM_NODE_REAL:
			WriteString(" R ");
			WriteReal(n->a.r.v);
			WriteString("\n");
			t = NULL;
			break;
		case AGM_NODE_LIST:
			WriteString(" L\n");
			WriteNode(n->a.l.h);
			t = n->a.l.t;
			break;
		case AGM_NODE_ARRAY:
			long i, size;
			WriteString(" A\n");
			size = (long)n->a.a.size;
			for (i = 0; i < size; i++)
				WriteNode(n->a.a.v[i]);
			t = NULL;
			break;
		case AGM_NODE_OBJECT:
			WriteString(" O ");
			WriteString(n->a.o.class_name->a.s.v);
			WriteString("\n");
			t = NULL;
			break;
		case AGM_NODE_DST_ITR:
			WriteString(" d\n");
			t = NULL;
			break;
		case AGM_NODE_SRC_ITR:
			WriteString(" s\n");
			t = NULL;
			break;
		case AGM_NODE_STRING:
			WriteString(" S ");
			WriteQString(n->a.s.v);
			WriteString("\n");
			t = n->a.s.t;
			break;
		case AGM_NODE_MATRIX:
			WriteString(" M ");
			WriteMatrix(n->a.m.v);
			WriteString("\n");
			t = n->a.m.t;
			break;
		case AGM_NODE_FILE:
			WriteString(" F\n");
			t = n->a.f.t;
			break;
		case AGM_NODE_WINDOW:
			WriteString(" W\n");
			t = n->a.w.t;
			break;
		case AGM_NODE_BITMAP:
			WriteString(" b ");
			ADvBitmap *b;
			b = n->a.b.v;
			if (b) b->Write(this);
			WriteString("\n");
			t = n->a.b.t;
			break;
		case AGM_NODE_PTR:
			WriteString(" P\n");
			t = NULL;
			break;
		case AGM_NODE_BINARY:
			WriteString(" B");
			AGmBinary *bin;
			bin = n->a.bin.v;
			if (bin) bin->Write(this);
			WriteString("\n");
			t = n->a.bin.t;
			break;
		case AGM_NODE_OCX:
			ADvOcx *ocx;
			ocx = n->a.ocx.v;
			WriteString(" X ");
			WriteString(ocx->ProgID()->Str());
			WriteString(" ");
			WriteInteger(ocx->Visible());
			WriteString(" ");
			WriteString(ocx->SubStgID()->Str());
			WriteString("\n");
#ifdef	_WINDOWS
			if (ocx->FileName() && ocx->Ctrl()) {
				const char *filename = ocx->FileName();
				IStorage *pStg;
				unsigned long mode = STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_DIRECT;
				HRESULT hr = StgOpenStorage(T2OLE(filename), NULL, mode, NULL, 0, &pStg);
				if (FAILED(hr)) {
					mode = STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_DIRECT;
					hr = StgCreateDocfile(T2OLE(filename), mode, 0, &pStg);
				}
				if (SUCCEEDED(hr)) {
					ocx->Save(pStg);
					pStg->Release();
				}
			}
#endif
			t = NULL;
			break;
		case AGM_NODE_VARIANT:
			WriteString(" V\n");
			t = NULL;
			break;
		case AGM_NODE_SQL_CONN:
			WriteString(" n\n");
			t = NULL;
			break;
		case AGM_NODE_SQL_STAT:
			WriteString(" t\n");
			t = NULL;
			break;
		default:	ASSERT(0);
		}
		AGmLink *l;
		for (l = next_outlink(n, NULL);
				l; l = next_outlink(n, l)) {
			if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) {
				WriteNode(l->arc->attr);
				WriteNode(l->arc->dst);
			} else {
				if ((l->arc->dst->flag & AGM_FLAG_SELF) != 0) {
					WriteNode(l->arc->attr);
				}
			}
		}
		n = t;
	}
	return 0;
}

int AGmFile::WriteArc(AGmNode *n) {
	while (n) {
		if (n->body->nid != nid+1) break;
		nid++;

		AGmNode *t = NULL;
		switch (n->type) {
		case AGM_NODE_INTEGER:
		case AGM_NODE_STRING:
		case AGM_NODE_MATRIX:
		case AGM_NODE_FILE:
		case AGM_NODE_WINDOW:
		case AGM_NODE_BITMAP:
		case AGM_NODE_PTR:
		case AGM_NODE_BINARY:
			if (n->a.i.t) {
				WriteString("t ");
				WriteInteger(n->body->nid);
				WriteString(" ");
				WriteInteger(n->a.i.t->body->nid);
				WriteString("\n");
			}
			t = n->a.i.t;
			break;
		case AGM_NODE_REAL:
		case AGM_NODE_OBJECT:
		case AGM_NODE_DST_ITR:
		case AGM_NODE_SRC_ITR:
		case AGM_NODE_OCX:
		case AGM_NODE_VARIANT:
		case AGM_NODE_SQL_CONN:
		case AGM_NODE_SQL_STAT:
			t = NULL;
			break;
		case AGM_NODE_LIST:
			if (n->a.l.h) {
				WriteString("h ");
				WriteInteger(n->body->nid);
				WriteString(" ");
				WriteInteger(n->a.l.h->body->nid);
				WriteString("\n");
			}
			if (n->a.l.t) {
				WriteString("t ");
				WriteInteger(n->body->nid);
				WriteString(" ");
				WriteInteger(n->a.l.t->body->nid);
				WriteString("\n");
			}
			WriteArc(n->a.l.h);
			t = n->a.l.t;
			break;
		case AGM_NODE_ARRAY:
			long i, size;
			size = (long)n->a.a.size;
			for (i = 0; i < size; i++) {
				if (n->a.a.v[i]) {
					WriteString("e ");
					WriteInteger(n->body->nid);
					WriteString(" ");
					WriteInteger(i);
					WriteString(" ");
					WriteInteger(n->a.a.v[i]->body->nid);
					WriteString("\n");
				}
			}
			for (i = 0; i < size; i++)
				WriteArc(n->a.a.v[i]);
			t = NULL;
			break;
		default:	ASSERT(0);
		}
		AGmLink *l;
		for (l = next_outlink(n, NULL);
				l; l = next_outlink(n, l)) {
			AGmNode *n1 = l->arc->dst;
			if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0 || n1->body->nid) {
				AGmNode *attr = l->arc->attr;
				WriteString("a ");
				WriteInteger(n->body->nid);
				WriteString(" ");
				WriteInteger(n1->body->nid);
				WriteString(" ");
				WriteInteger(attr ? attr->body->nid : 0);
				WriteString(" ");
				WriteInteger(l->arc->incl_assoc);
				WriteString("\n");			
			}
			if ((l->arc->incl_assoc & AGM_ARC_ASSOC) == 0) {
				WriteArc(l->arc->attr);
				WriteArc(l->arc->dst);
			} else {
				if ((l->arc->dst->flag & AGM_FLAG_SELF) != 0) {
					WriteArc(l->arc->attr);
				}
			}
		}
		n = t;
	}
	return 0;
}

#define N_TBL(n)	tbl[(n)/16000][(n)%16000]

int AGmFile::ReadGraph(AGmNode *&n) {
	char c;
#if _WINDOWS && _64BIT
	long long n_node, nid;
#else
	long n_node, nid;
#endif

	ReadSpace();
	c = Input();
	if (c == EOF) { Unput(); return 1; }
	if (c != 'B') return 1;
	Input();
	if (_ReadInteger(n_node))
		return 1;

	// allocate node table
#if _WINDOWS && _64BIT
	long long n_node2 = n_node / 16000 + 1;
#else
	long n_node2 = n_node / 16000 + 1;
#endif
	AGmNode ***tbl = new AGmNode**[n_node2];
	long i, j;
	for (i = 0; i < n_node2; i++) {
		tbl[i] = new AGmNode*[16000];
		for (j = 0; j < 16000; j++) {
			tbl[i][j] = NULL;
		}
	}

	Input();
	c = Input();
	if (c == EOF) {
		Unput(); goto error;
	}

	for (;;) {
		if (c != 'N')
			break;
		Input();
		if (_ReadInteger(nid))
			goto error;
		Input();
		c = Input();
		if (c == EOF) {
			Unput(); goto error;
		}

		if (c == 'I') {
			AGmNode* n;
			Input();
			if (ReadInteger(n))
				goto error;
			N_TBL(nid-1) = n;

		} else if (c == 'R') {
			double v;
			if (ReadReal(v))
				goto error;
			N_TBL(nid-1) = Real(v);

		} else if (c == 'L') {
			N_TBL(nid-1) = Cons(NULL);

		} else if (c == 'A') {
#if	_WINDOWS && _64BIT
			long long size;
#else
			long size;
#endif
			Input();
			if (_ReadInteger(size))
				goto error;
			N_TBL(nid-1) = Array(size);

		} else if (c == 'O') {
			AGmString *cl_name;
			if (ReadIdent(cl_name))
				goto error;
			N_TBL(nid-1) = Object(String(cl_name));

		} else if (c == 'd') {
			N_TBL(nid-1) = DstItr(NULL);
		} else if (c == 's') {
			N_TBL(nid-1) = SrcItr(NULL);

		} else if (c == 'S') {
			AGmString *str;
			if (ReadQString(str))
				goto error;
			AGmNode *ss = UniqueStr(str->Str());
			if (ss) {
				N_TBL(nid-1) = ss; delete str;
			} else {
				N_TBL(nid-1) = String(str);
			}

		} else if (c == 'M') {
			AGmMatrix *mat;
			if (ReadMatrix(mat))
				goto error;
			N_TBL(nid-1) = Matrix(mat);

		} else if (c == 'F') {
			N_TBL(nid-1) = File(NULL);

		} else if (c == 'W') {
			N_TBL(nid-1) = Window(NULL);
		} else if (c == 'b') {
			ADvBitmap *b = ADvBitmap::Read(this);
			N_TBL(nid-1) = Bitmap(b);
		} else if (c == 'P') {
			N_TBL(nid-1) = PtrCell(NULL);
		} else if (c == 'B') {
			AGmBinary *b = AGmBinary::Read(this);
			N_TBL(nid-1) = Binary(b);

		} else if (c == 'X') {
			AGmString *progID, *subStgID;
#if	_WINDOWS && _64BIT
			long long visible;
#else
			long visible;
#endif
			if (ReadString(progID)) goto error;
			if (ReadInteger(visible) || ReadString(subStgID)) {
				delete progID;
				goto error;
			}
			ADvOcx* ocx = new ADvOcx(
				progID->Str(), (int)visible, subStgID->Str());
			ocx->Mode(0);
			delete progID; delete subStgID;
			N_TBL(nid-1) = Ocx(ocx);
		} else if (c == 'V') {
			N_TBL(nid-1) = Variant(NULL);

		} else if (c == 'n') {
			AGmNode *nn = Integer(0);
			nn->type = AGM_NODE_SQL_CONN;
			nn->a.conn.impl = NULL;
			N_TBL(nid-1) = nn;
		} else if (c == 't') {
			AGmNode *nn = Integer(0);
			nn->type = AGM_NODE_SQL_STAT;
			nn->a.stat.impl = NULL;
			N_TBL(nid-1) = nn;

		} else if (c == 'g') {
#if	_WINDOWS && _64BIT
			long long v, mod;
#else
			long v, mod;
#endif
			if (ReadInteger(v))
				goto error;
			if (ReadInteger(mod))
				goto error;
			N_TBL(nid-1) = GFp(v, mod);

		} else {
			goto error;
		}
		c = Input();
		c = Input();
		if (c == EOF) { 
			Unput(); goto error;	
		}
	}

	for (;;) {
		if (c == 'h') {
#if	_WINDOWS && _64BIT
			long long pid, cid;
#else
			long pid, cid;
#endif
			Input();
			if (_ReadInteger(pid))
				goto error;
			Input();
			if (_ReadInteger(cid))
				goto error;
			Set(N_TBL(pid-1)->a.l.h, N_TBL(cid-1));

		} else if (c == 't') {
#if	_WINDOWS && _64BIT
			long long pid, cid;
#else
			long pid, cid;
#endif
			Input();
			if (_ReadInteger(pid))
				goto error;
			Input();
			if (_ReadInteger(cid))
				goto error;
			Set(N_TBL(pid-1)->a.s.t, N_TBL(cid-1));

		} else if (c == 'e') {
#if	_WINDOWS && _64BIT
			long long aid, index, eid;
#else
			long aid, index, eid;
#endif
			Input();
			if (_ReadInteger(aid))
				goto error;
			Input();
			if (_ReadInteger(index))
				goto error;
			Input();
			if (_ReadInteger(eid))
				goto error;
			Set(N_TBL(aid-1)->a.a.v[index], N_TBL(eid-1));

		} else if (c == 'a') {
#if	_WINDOWS && _64BIT
			long long sid, did, aid, incl_assoc;
#else
			long sid, did, aid, incl_assoc;
#endif
			Input();
			if (_ReadInteger(sid))
				goto error;
			Input();
			if (_ReadInteger(did))
				goto error;
			Input();
			if (_ReadInteger(aid))
				goto error;
			Input();
			if (_ReadInteger(incl_assoc))
				goto error;
			AGmArc *a = CreateArc( N_TBL(sid-1), N_TBL(did-1), (aid ? N_TBL(aid-1) : NULL));
			a->incl_assoc = (unsigned short)incl_assoc;

		} else {
			break;
		}
		c = Input();
		c = Input();
		if (c == EOF) {
			Unput(); goto error;
		}
	}

	if (c != 'E')
		goto error;

	n = N_TBL(0);
	for (i = 0; i < n_node2; i++) delete [] tbl[i];
	delete [] tbl;
	return 0;
error:
	for (i = 0; i < n_node; i++) DeleteNodeSelf(N_TBL(i));
	for (i = 0; i < n_node2; i++) delete [] tbl[i];
	delete [] tbl;
	return 1;
}

int AGmFile::WriteGraph(AGmNode *n, const char *path) {
	AGmFile *f = new AGmFile();
	int flag;
	flag = f->Create(path, AGM_FILE_WRITE);
	if (flag) { delete f; return flag; }
	flag = f->WriteGraph(n);
	delete f;
	return flag;
}
int AGmFile::WriteGraph(AGmNode *n, AGmString *path) {
	return WriteGraph(n, path->Str());
}
int AGmFile::ReadGraph(AGmNode*& n, const char *path) {
	AGmFile *f = new AGmFile();
	int flag;
	flag = f->Create(path, AGM_FILE_READ);
	if (flag) { delete f; return flag; }
	flag = f->ReadGraph(n);
	delete f;
	return flag;
}
int AGmFile::ReadGraph(AGmNode*& n, AGmString *path) {
	return ReadGraph(n, path->Str());
}

// ======================== File manipulation ============================

int AGmFile::GetDir(AGmString*& s) {
	char path[MAX_PATH];
	if (art_getcwd(path, MAX_PATH)) {
		s = new AGmString(path);
#ifdef	_WINDOWS
		s->ChangeFileDelimiter('\\', '/');
#endif
		return 0;
	} else {
		return 1;
	}
}
int AGmFile::FindFile(AGmNode*& n, const char* path, const char *name) {
#ifdef	_WINDOWS
	int cur_drv = _getdrive();
	char cur_path[MAX_PATH];
	_getdcwd(cur_drv, cur_path, MAX_PATH);	
	ChDir(path);

	n = Cons(NULL);
	struct _finddata_t info;
	char *name2 = "*.*";
	int len = 0;
	if (name) len = (int)strlen(name);
	intptr_t handle = _findfirst(name2, &info);
	if (handle == -1) return 0;
	if (len) {
		if (strncmp(info.name, name, len) == 0) 
			CreateArc(n, String((const char*)&info.name));
	} else {
		CreateArc(n, String((const char*)&info.name));
	}
	for (;;) {
		long result = _findnext(handle, &info); 	
		if (result == -1) break;
		if (len) {
			if (strncmp(info.name, name, len) == 0) 
				CreateArc(n, String((const char*)&info.name));
		} else {
			CreateArc(n, String((const char*)&info.name));
		}
	}
	_findclose(handle);

	_chdrive(cur_drv);
	_chdir(cur_path);
#endif
#ifdef	_LINUX
	n = Cons(NULL);
	DIR *dir = opendir(path);
	if (dir == NULL) return 0;
	int len = 0;
	if (name) len = strlen(name);
	for (;;) {
		struct dirent *info = readdir(dir);
		if (info == NULL) break;
		if (len) {
			if (strncmp(info->d_name, name, len) == 0)	
				CreateArc(n, String((const char*)info->d_name));
		} else {
			CreateArc(n, String((const char*)info->d_name));
		}
	}
	closedir(dir);
#endif
	return 0;
}
int AGmFile::DoesExist(const char* path) {
	struct stat sb;
	int result = stat(path, &sb);
	return (result != -1);
}
int AGmFile::IsDir(const char* path) {
	struct stat sb;
	int result = stat(path, &sb);
	if (result == -1) return 0;
	return ((sb.st_mode & S_IFDIR) != 0);
}
int AGmFile::ChDir(const char* path) {
	return art_chdir(path);
}
int AGmFile::MkDir(const char* path) {
#ifdef	_WINDOWS
	return art_mkdir(path);
#endif
#ifdef	_LINUX
	return mkdir(path, 0755);
#endif
}
int AGmFile::Remove(const char* path) {
	if (IsDir(path)) {
		return art_rmdir(path);
	} else {
		return remove(path);
	}
}
int AGmFile::Rename(const char* old_path, const char* new_path) {
	return rename(old_path, new_path);
}
int AGmFile::Copy(const char* src, const char* dst) {
	FILE *in = art_fopen(src, "rb");
	if (in == NULL) return 1;
	FILE *out = art_fopen(dst, "wb");
	if (out == NULL) { fclose(in); return 1; }
	unsigned char buf[1024]; int cc, cc2;
	for (;;) {
		cc = art_read(art_fileno(in), buf, 1024);
		if (cc <= 0) break;
		cc2 = art_write(art_fileno(out), buf, cc);
		if (cc2 < cc) {
			fclose(in); fclose(out);
			return 10;
		}
	}
	fclose(in); fclose(out);
	return 0;
}

int AGmFile::GetStat(const char* path, struct stat *buf) {
	return stat(path, buf);
}
int AGmFile::SetTime(const char* path, struct utimbuf *buf) {
	return utime(path, buf);
}
int AGmFile::ChMod(const char* path, int mode) {
	return art_chmod(path, mode);
}

char* AGmFile::GetTempDir() {
#ifdef	_WINDOWS
	return art_getenv("TEMP");
#endif
#ifdef	_LINUX
	return AL_STR("/tmp");	
#endif
}
AGmString* AGmFile::GetTempFile(const char* dir) {
#ifdef _WINDOWS
	char buf[MAX_PATH];
	GetTempFileName(dir, "al", 0, buf);
	return new AGmString(buf);
#endif
#ifdef _LINUX
	return new AGmString(art_tempnam(dir, "al"));
#endif
}

int AGmFile::ReadBinary(AGmBinary *b, long i, long size) {
	switch (type) {
	case AGM_FILE_BIN_READ:
		unsigned char* buff;
		buff = b->Buffer();
		buff = &buff[i];
		int read_c, total_c, cc;
		read_c = size;
		total_c = 0;
		while (read_c > 0) {
			cc = art_read(art_fileno(fp), &buff[total_c], read_c);
			if (cc <= 0) return total_c; // read bytes (0..size-1)
			total_c += cc; read_c -= cc;
		}
		break;
	default:
		return -1; // error
	}
	return size; // read bytes == size
}
int AGmFile::WriteBinary(AGmBinary *b, long i, long size) {
	switch (type) {
	case AGM_FILE_BIN_WRITE:
	case AGM_FILE_BIN_APPEND:
		unsigned char* buff;
		buff = b->Buffer();
		buff = &buff[i];
		int write_c, total_c, cc;
		write_c = size;
		total_c = 0;
		while (write_c > 0) {
			cc = art_write(art_fileno(fp), &buff[total_c], write_c);
			if (cc <= 0) return total_c; // write bytes (0..size-1)
			total_c += cc; write_c -= cc;
		}
		break;
	default:
		return -1; // error
	}
	return size; // write bytes == size
}

#ifdef _LINUX
void AGmFile::CheckChildProcessExit() {
	AGmNode *n = dst_node2(RootGlobal(), NmPipe());
	if (n == NULL) return;
	AGmLink *l;
	for (l = next_outlink(n, NULL);
			l; l = next_outlink(n, l)) {
		AGmNode *dst = l->arc->dst;
		AGmFile *f = dst->a.f.v;

		char str[MAX_PATH], c;
		if (AGmFile::DoesProcessExit(f->pid, str, &c) == 0) continue;
		waitpid(f->pid, NULL, 0);

		c = 0xff;
		write(f->stdout_pipe[1], &c, 1);
		l = l->prev;
		DeleteArc(l->next->arc);
	}
}

//#define	_DEBUG_PROCESS_EXIT
int AGmFile::DoesProcessExit(int pid, char *str, char *c) {
	char cmd[256];

	AGmString* path = GetTempFile("/tmp");
#ifdef	_DEBUG_PROCESS_EXIT
	printf("tmppath = %s\n", path->Str());
#endif
	art_sprintf_1(cmd, 256, "ps -o pid,stat > %s", path->Str());
	system(cmd);

	AGmFile* f = new AGmFile();
	if (f->Create(path->Str(),  AGM_FILE_READ)) {
		delete f, delete path; return 1;
	}

	/* ignore first line. */
	AGmString* line;
	if (f->ReadLine(line)) {
#ifdef	_DEBUG_PROCESS_EXIT
	printf("header line read failed.\n");
#endif
		goto ret1;
	} else {
#ifdef	_DEBUG_PROCESS_EXIT
	printf("header line = %s\n", line->Str());
#endif
		delete line;
	}

	for (;;) {
		long c_pid; AGmString* stat;

		if (f->ReadLine(line)) goto ret1;
#ifdef	_DEBUG_PROCESS_EXIT
	printf("line = %s\n", line->Str());
#endif

		AGmFile* f2 = new AGmFile();
		if (f2->Create(line, AGM_FILE_STR_READ)) {
			delete f2; delete line; goto ret1;
		} else {
		}

		/* pid */
		if (f2->ReadInteger(c_pid)) {
			delete f2; delete line;
			goto ret1;
		} else {
		}
#ifdef	_DEBUG_PROCESS_EXIT
	printf("pid = %d, c_pid = %d\n", pid, c_pid);
#endif
		if (c_pid != pid) {
			delete f2; delete line;
			continue;
		} else {
		}
#ifdef	_DEBUG_PROCESS_EXIT
	printf("process found.\n");
#endif

		/* status */
		if (f2->ReadString(stat)) {
			delete f2; delete line;
			goto ret1;
		} else {
		}
#ifdef	_DEBUG_PROCESS_EXIT
	printf("status = %s\n", stat->Str());
#endif

		/* chech status */
		if (stat->StartsWith("Z", 1)) {
			delete f2; delete line; delete stat;
			goto ret1;
		} else {
			delete f2; delete line; delete stat;
			art_sprintf_1(cmd, 256, "rm -f %s", path->Str());
			system(cmd);
			delete f; delete path;
			return 0;
		}
	}
ret1:
	art_sprintf_1(cmd, 256, "rm -f %s", path->Str());
	system(cmd);
	delete f; delete path;
	return 1;
}
#endif
