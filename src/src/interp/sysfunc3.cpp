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
#include <gdm/vmkey.h>
#include <gdm/debug.h>
#include <gdm/error.h>
#include <gdm/pict.h>
#include <interp/interp.h>
#include <interp/prcsmgr.h>
#include <dev/advcapp.h>
#include <dev/advcvw.h>
#include <dev/event.h>
#include <dev/advwnd.h>
#include <dev/advcedit.h>
#include <dev/shell.h>
#include <dev/bitmap.h>
#include <dev/olectrl.h>
#include <dev/mainfrm.h>
#include <cui/class.h>
#include <cui/member.h>
#include <cui/body.h>
#include <cui/cui.h>
#include <altair/strres.h>

#include <gdm/j_code.h>
#include <gdm/big_int.h>

#ifdef	_LINUX
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

#include <juman/juman.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

extern "C" AL_EXT_API int open_url(const char* url) {
	int ok;
#ifdef	_WINDOWS
	HINSTANCE h = ::ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
#ifdef	_64BIT
	if ((long long)h > 32) {
#else
	if ((long)h > 32) {
#endif
		ok = 1;
	} else {
		ok = 0;
	}
#endif
#ifdef	_LINUX
	char* argv[3];
#ifdef	_MAC
	argv[0] = AL_STR("open");
#else
	argv[0] = AL_STR("xdg-open");
#endif
	argv[1] = (char*)url;
	argv[2] = NULL;
	int pid;
	pid = fork();
	if (pid == 0) {
		execvp(argv[0], argv);
	}
#endif
	return ok;
}

AGmNode *split_str(AGmString *s, int d) {
	AGmNode *val = AGm::Cons(NULL);
	AGmString *ss = NULL;
	char c;
	int n = s->Length();
	int i;
	for (i = 0; i < n; i++) {
		c = s->Component(i);
		if (ss == NULL)
			ss = new AGmString(16);
		if (c == d) {
			AGm::CreateArc(val, AGm::String(ss));
			ss = new AGmString(16);
			continue;
		}
#ifdef	_WINDOWS
		if (isFirstChar(c)) {
			ss->Append(c);
			ss->Append(s->Component(++i));
#endif
#ifdef	_LINUX
		int i2;
		if (isUTF8((unsigned char*)s->Str()+i, i2)) {
			ss->Append((const char*)utf8);
			i += i2-1;
#endif
		} else {
			ss->Append(c);
		}
	}
	if (ss) {
		AGm::CreateArc(val, AGm::String(ss));
	}
	return val;
}

extern struct timeb startup_time;

time_t get_time() {
	time_t ltime;
	time(&ltime);
	return ltime;
}
void get_localtime(time_t ltime, long& year, long &month, long &day,
		long& hour, long& minute, long& second, long& wday) {
	struct tm *ntime = art_localtime(&ltime);
	year = ntime->tm_year + 1900;
	month = ntime->tm_mon + 1;
	day = ntime->tm_mday;
	hour = ntime->tm_hour;
	minute = ntime->tm_min;
	second = ntime->tm_sec;
	wday = ntime->tm_wday + 1;
}
time_t get_mktime(long year, long month, long day,
		long hour, long minute, long second) {
	struct tm ntime;
	ntime.tm_year = year - 1900;
	ntime.tm_mon = month - 1;
	ntime.tm_mday = day;
	ntime.tm_hour = hour;
	ntime.tm_min = minute;
	ntime.tm_sec = second;
	return mktime(&ntime); 
}
void get_gmtime(time_t ltime, long& year, long &month, long &day,
		long& hour, long& minute, long& second, long& wday) {
	struct tm *ntime = art_gmtime(&ltime);
	year = ntime->tm_year + 1900;
	month = ntime->tm_mon + 1;
	day = ntime->tm_mday;
	hour = ntime->tm_hour;
	minute = ntime->tm_min;
	second = ntime->tm_sec;
	wday = ntime->tm_wday + 1;
}

// file manipulation functions
int AIpProcess::EvalFileOpen() {
	AGmNode *name, *type;
	GetArg(name, type);
	Unref(val); val = NULL;
	if (!IsString(name))	{
		Unref(name, type);
		return 0;
	}
	if (!IsString(type))	{
		Unref(name, type);
		return interp_error("FileOpen()");
	}
	AGmFile *f = new AGmFile();
	const char *s = type->a.s.v->Str();
	int err;
	if (strcmp(s, "sr") == 0) {
		err = f->Create(name->a.s.v, AGM_FILE_STR_READ);
	} else if (strcmp(s, "sw") == 0) {
		err = f->Create(name->a.s.v, AGM_FILE_STR_WRITE);
	} else if (strcmp(s, "sa") == 0) {
		err = f->Create(name->a.s.v, AGM_FILE_STR_APPEND);
	} else if (strcmp(s, "r") == 0) {
		err = f->Create(name->a.s.v, AGM_FILE_READ);
	} else if (strcmp(s, "w") == 0) {
		err = f->Create(name->a.s.v, AGM_FILE_WRITE);
	} else if (strcmp(s, "a") == 0) {
		err = f->Create(name->a.s.v, AGM_FILE_APPEND);
	} else if (strcmp(s, "rb") == 0) {
		err = f->Create(name->a.s.v, AGM_FILE_BIN_READ);
	} else if (strcmp(s, "wb") == 0) {
		err = f->Create(name->a.s.v, AGM_FILE_BIN_WRITE);
	} else if (strcmp(s, "ab") == 0) {
		err = f->Create(name->a.s.v, AGM_FILE_BIN_APPEND);
	} else if (strcmp(s, "stdin") == 0) {
		err = f->Create(name->a.s.v, AGM_FILE_STDIN);
	} else if (strcmp(s, "stdout") == 0) {
		err = f->Create(name->a.s.v, AGM_FILE_STDOUT);
	} else if (strcmp(s, "stderr") == 0) {
		err = f->Create(name->a.s.v, AGM_FILE_STDERR);
	} else if (strcmp(s, "pipe") == 0) {
		err = f->Create(name->a.s.v, AGM_FILE_PIPE);
	} else if (strcmp(s, "lock") == 0) {
		err = f->Create(name->a.s.v, AGM_FILE_LOCK);
	} else {
		Unref(name, type);
		return interp_error("FileOpen()");
	}
	if (err) {
		delete f;
		Unref(name, type);
		return 0;
	}
	val = Ref(File(f));
	Unref(name, type);
	return 0;
}

#ifdef	_WINDOWS
static DWORD fileReadFunc(AGmFile *file);
#endif
#ifdef	_LINUX
static gpointer fileReadFunc(gpointer file);
#endif

int AIpProcess::EvalFileRead() {
	AGmNode *file, *type;
	AGmBinary *bin;
#if	_WINDOWS && _64BIT
	long long idx, size;
	long long find_char;
#else
	long idx, size;
	long find_char;
#endif
	AGmNode *tail;
	char *find, *find_str;
	GetArg(file, type);
	if (IsFile(file) && file->a.f.v && IsString(type)) {
		AGmFile *f = file->a.f.v;
		if (f->Type() != AGM_FILE_PIPE) {
			const char *s = type->a.s.v->Str();
			AGmNode *n = NULL; int err = 1;
			if (strcmp(s, "char") == 0) {
				err = f->ReadChar(n);
			} else if (strcmp(s, "space") == 0) {
				err = 0;
				AGmString *s1 = f->ReadSpace2();
				if (s1 != NULL) {
					n = AGm::String(s1);
				}
			} else if (strcmp(s, "comment") == 0) {
				err = 0;
				AGmString *s1 = f->ReadComment();
				if (s1 != NULL) {
					n = AGm::String(s1);
				}
			} else if (strcmp(s, "integer") == 0) {
				err = f->ReadInteger(n);
			} else if (strcmp(s, "hex") == 0) {
				err = f->ReadHexa(n);
			} else if (strcmp(s, "real") == 0) {
				err = f->ReadReal(n);
			} else if (strcmp(s, "ident") == 0) {
				err = f->ReadIdent(n);
			} else if (strcmp(s, "jident") == 0) {
				err = f->ReadJIdent(n);
			} else if (strcmp(s, "xident") == 0) {
				err = f->ReadXIdent(n);
			} else if (strcmp(s, "string") == 0) {
				err = f->ReadString(n);
			} else if (strcmp(s, "line") == 0) {
				err = f->ReadLine(n);
			} else if (strcmp(s, "quote_char") == 0) {
				err = f->ReadQChar(n);
			} else if (strcmp(s, "quote_string") == 0) {
				err = f->ReadQString(n);
			} else if (strcmp(s, "single_quote_string") == 0) {
				err = f->ReadSingleQString(n);
			} else if (strcmp(s, "csv") == 0) {
				err = f->ReadCsv(n);
			} else if (strcmp(s, "matrix") == 0) {
				err = f->ReadMatrix(n);
			} else if (strcmp(s, "list") == 0) {
				err = f->ReadList(n);
			} else if (strcmp(s, "tree") == 0) {
				err = f->ReadTree(n);
			} else if (strcmp(s, "graph") == 0) {
				err = f->ReadGraph(n);
				if (err == 0) {
					AGm::MakeStrUnique(n);
					if (AUiClassShell::LinkClassObj(RootClass(), n)) {
						AGm::RefUnref(n); n = NULL;
					}
				}

			} else if (strcmp(s, "class") == 0) {
				AGmString *ss;
				if (f->ReadIdent(ss)) {
					ADvWindow::Print(AL_S117);
					err = 1;
				} else {
					AGmNode *root = AUiClassShell::NameToClass(RootClass(), ss);
					if (root == NULL) {
						char error[256];
						art_sprintf_1(error, 256, AL_S115, ss->Str());
						ADvWindow::Print(error);
						delete ss; err = 1;
					} else {
						delete ss;
						if (AUiProgramMgr::_LoadLibText(f, NULL, RootClass(), root, AIP_STEP_CLASS, &n)) {
							ADvWindow::Print(AL_S111);
							err = 1;
						} else {
							err = 0;
						}
					}
				}
			} else if (strcmp(s, "member") == 0) {
				AGmString *ss;
				if (f->ReadIdent(ss)) {
					ADvWindow::Print(AL_S117);
					err = 1;
				} else {
					AGmNode *root = AUiClassShell::NameToClass(RootClass(), ss);
					if (root == NULL) {
						char error[256];
						art_sprintf_1(error, 256, AL_S115, ss->Str());
						ADvWindow::Print(error);
						delete ss; err = 1;
					} else {
						delete ss;
						if (AUiProgramMgr::_LoadLibText(f, NULL, RootClass(), root, AIP_STEP_MEMBER, &n)) {
							ADvWindow::Print(AL_S111);
							err = 1;
						} else {
							err = 0;
						}
					}
				}
			} else if (strcmp(s, "body") == 0) {
				AGmString *ss;
				if (f->ReadIdent(ss)) {
					ADvWindow::Print(AL_S117);
					err = 1;
				} else {
					AGmNode *root = AUiClassShell::NameToClass(RootClass(), ss);
					if (root == NULL) {
						char error[256];
						art_sprintf_1(error, 256, AL_S115, ss->Str());
						ADvWindow::Print(error);
						delete ss; err = 1;
					} else {
						delete ss;
						if (AUiProgramMgr::_LoadLibText(f, NULL, RootClass(), root, AIP_STEP_BODY, &n)) {
							ADvWindow::Print(AL_S111);
							err = 1;
						} else {
							err = 0;
						}
					}
				}
			} else {
				Unref(file, type);
				return interp_error("FileRead()");
			}
			if (err) n = NULL;
			Unref(val); val = Ref(n);
		} else {
#ifdef	_WINDOWS
			if (f->thread == NULL) {
				f->root = RootClass();
				f->read_type = Ref(type);

				AGmNode *n = (AGmNode*)Alloc();
				n->type = AGM_NODE_EMPTY;
				n->flag = 0; n->count = 0; n->body = NULL;
				f->value = n;

				SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
				DWORD threadID;
				f->thread = ::CreateThread(
					&sa, 0,
					(LPTHREAD_START_ROUTINE)fileReadFunc, f,
					0, &threadID);
			} else {
				::SetEvent(f->contEvent);
			}
			::WaitForSingleObject(f->retEvent, INFINITE);
			::ResetEvent(f->retEvent);
			if (f->wait) {
				Unref(val); val = Ref(Integer(0x80000000));
			} else {
				Unref(f->read_type);

				Unref(val);
				if (f->value->type == AGM_NODE_EMPTY) {
					Free((AGmCell*)f->value);
					val = NULL;
				} else {
					val = Ref(f->value);				
				}
				f->value = NULL;

				::CloseHandle(f->thread);
				f->thread = NULL;
			}
#endif
#ifdef	_LINUX
			if (f->thread == NULL) {
				f->root = RootClass();
				f->read_type = Ref(type);

				AGmNode *n = (AGmNode*)Alloc();
				n->type = AGM_NODE_EMPTY;
				n->flag = 0; n->count = 0; n->body = NULL;
				f->value = n;

				f->thread = g_thread_new(AL_STR("pipe-child"), fileReadFunc, f);
			} else {
				g_mutex_lock(&f->mutex);
				f->contCondFlag = 1;
				g_cond_signal(&f->contCond);
				g_mutex_unlock(&f->mutex);
			}
			g_mutex_lock(&f->mutex);
			while (f->retCondFlag == 0) {
				g_cond_wait(&f->retCond, &f->mutex);
			}
			f->retCondFlag = 0;
			g_mutex_unlock(&f->mutex);
			if (f->wait) {
				Unref(val); val = Ref(Integer(0x80000000));
			} else {
				Unref(f->read_type);

				Unref(val);
				if (f->value->type == AGM_NODE_EMPTY) {
					Free((AGmCell*)f->value);
					val = NULL;
				} else {
					val = Ref(f->value);				
				}
				f->value = NULL;

				f->thread = NULL;
			}
#endif
		}
	} else if (IsFile(file) && file->a.f.v && type && IsList(type)
			&& Param(type, bin, tail) == 0
			&& Param(tail, idx, size, tail) == 0
			&& idx >= 0 && idx + size <= bin->Length()) {
		int err = file->a.f.v->ReadBinary(bin, (long)idx, (long)size);
		if (err == size) {
			Unref(val); val = NULL;
		} else {
			Unref(val); val = Ref(Integer((long)err));
		}
	} else if (IsFile(file) && file->a.f.v && type && IsList(type)
			&& Param(type, find, tail) == 0 && strcmp(find, "find") == 0
			&& Param(tail, find_str, tail) == 0) {
		int found = file->a.f.v->Search(find_str);
		if (found) {
			Unref(val); val = Ref(Integer(1));
		} else {
			Unref(val); val = NULL;
		}
	} else if (IsFile(file) && file->a.f.v && type && IsList(type)
			&& Param(type, find, tail) == 0 && strcmp(find, "find") == 0
			&& Param(tail, find_char, tail) == 0) {
		int found = file->a.f.v->Search((char)find_char);
		if (found) {
			Unref(val); val = Ref(Integer(1));
		} else {
			Unref(val); val = NULL;
		}

	} else {
		Unref(file, type);
		return interp_error("FileRead()");
	}
	Unref(file, type);
	return 0;
}

#ifdef	_WINDOWS
static DWORD fileReadFunc(AGmFile *f) {
#endif
#ifdef	_LINUX
static gpointer fileReadFunc(gpointer data) {
	AGmFile* f = (AGmFile*)data;
#endif
	const char *type = f->read_type->a.s.v->Str();
	AGmNode *n = f->value;

	char c;
#if	_WINDOWS && _64BIT
	long long i;
#else
	long i;
#endif
	double r;
	AGmString *s;
	AGmMatrix *m;
	
	if (strcmp(type, "char") == 0) {
		if (f->ReadChar(c) == 0) {
			n->type = AGM_NODE_INTEGER;
			n->a.i.v = (unsigned char)c; n->a.i.t = NULL;
		}
	} else if (strcmp(type, "space") == 0) {
		AGmString *s1 = f->ReadSpace2();
		if (s1 != NULL) {
			n->type = AGM_NODE_STRING;
			n->a.s.v = s1; n->a.s.t = NULL;
		}
	} else if (strcmp(type, "comment") == 0) {
		AGmString *s1 = f->ReadComment();
		if (s1 != NULL) {
			n->type = AGM_NODE_STRING;
			n->a.s.v = s1; n->a.s.t = NULL;
		}
	} else if (strcmp(type, "integer") == 0) {
		if (f->ReadInteger(i) == 0) {
			n->type = AGM_NODE_INTEGER;
			n->a.i.v = i; n->a.i.t = NULL;
		}
	} else if (strcmp(type, "hex") == 0) {
		if (f->ReadHexa(i) == 0) {
			n->type = AGM_NODE_INTEGER;
			n->a.i.v = i; n->a.i.t = NULL;
		}
	} else if (strcmp(type, "real") == 0) {
		if (f->ReadReal(r) == 0) {
			n->type = AGM_NODE_REAL;
			n->a.r.v = r;
		}
	} else if (strcmp(type, "ident") == 0) {
		if (f->ReadIdent(s) == 0) {
			n->type = AGM_NODE_STRING;
			n->a.s.v = s; n->a.s.t = NULL;
		}
	} else if (strcmp(type, "jident") == 0) {
		if (f->ReadJIdent(s) == 0) {
			n->type = AGM_NODE_STRING;
			n->a.s.v = s; n->a.s.t = NULL;
		}
	} else if (strcmp(type, "xident") == 0) {
		if (f->ReadXIdent(s) == 0) {
			n->type = AGM_NODE_STRING;
			n->a.s.v = s; n->a.s.t = NULL;
		}
	} else if (strcmp(type, "string") == 0) {
		if (f->ReadString(s) == 0) {
			n->type = AGM_NODE_STRING;
			n->a.s.v = s; n->a.s.t = NULL;
		}
	} else if (strcmp(type, "line") == 0) {
		if (f->ReadLine(s) == 0) {
			n->type = AGM_NODE_STRING;
			n->a.s.v = s; n->a.s.t = NULL;
		}
	} else if (strcmp(type, "quote_char") == 0) {
		if (f->ReadQChar(c) == 0) {
			n->type = AGM_NODE_INTEGER;
			n->a.i.v = (unsigned char)c; n->a.i.t = NULL;
		}
	} else if (strcmp(type, "quote_string") == 0) {
		if (f->ReadQString(s) == 0) {
			n->type = AGM_NODE_STRING;
			n->a.s.v = s; n->a.s.t = NULL;
		}
	} else if (strcmp(type, "single_quote_string") == 0) {
		if (f->ReadSingleQString(s) == 0) {
			n->type = AGM_NODE_STRING;
			n->a.s.v = s; n->a.s.t = NULL;
		}
	} else if (strcmp(type, "matrix") == 0) {
		if (f->ReadMatrix(m) == 0) {
			n->type = AGM_NODE_MATRIX;
			n->a.m.v = m; n->a.m.t = NULL;
		}
	}
	f->wait = 0;
#ifdef	_WINDOWS
	if (f->retEvent) ::SetEvent(f->retEvent);
	::ExitThread(0);
#endif
#ifdef	_LINUX
	g_mutex_lock(&f->mutex);
	f->retCondFlag = 1;
	g_cond_signal(&f->retCond);
	g_mutex_unlock(&f->mutex);
	f->thread = NULL;
	g_thread_exit(NULL);
#endif
	return 0;
}

int AIpProcess::EvalFileWrite() {
	AGmNode *file, *type, *data;
	AGmBinary *bin;
#if	_WINDOWS && _64BIT
	long long idx, size;
#else
	long idx, size;
#endif
	AGmNode *tail;
	GetArg(file, type, data);
	if (IsFile(file) && file->a.f.v && IsString(type)) {
		const char *s = type->a.s.v->Str();
		int err = 1;
		if (strcmp(s, "char") == 0) {
			if (IsInteger(data)) {
				err = file->a.f.v->WriteChar((const char)data->a.i.v);
			}
		} else if (strcmp(s, "integer") == 0) {
			if (IsInteger(data) || IsBigInt(data)) {
				err = file->a.f.v->WriteInteger(data);
			}
		} else if (strcmp(s, "hex") == 0) {
			if (IsInteger(data)) {
				err = file->a.f.v->WriteHexa(data->a.i.v);
			}
		} else if (strcmp(s, "hex2") == 0) {
			if (IsInteger(data)) {
				long i = (long)data->a.i.v;
				if (0 <= i && i < 0x100) {
					char ss[16];
					art_sprintf_1(ss, 16, "%.2lx", i);
					err = file->a.f.v->WriteString(ss);
				}
			}
		} else if (strcmp(s, "real") == 0) {
			if (IsReal(data)) {
				err = file->a.f.v->WriteReal(data->a.r.v);
			}
		} else if (strcmp(s, "string") == 0) {
			if (IsString(data)) {
				err = file->a.f.v->WriteString(data->a.s.v);
			}
		} else if (strcmp(s, "quote_char") == 0) {
			if (IsInteger(data)) {
				err = file->a.f.v->WriteQChar((char)data->a.i.v);
			}
		} else if (strcmp(s, "quote_string") == 0) {
			if (IsString(data)) {
				err = file->a.f.v->WriteQString(data->a.s.v);
			}
		} else if (strcmp(s, "csv") == 0) {
			if (data) {
				err = file->a.f.v->WriteCsv(data);
			}
		} else if (strcmp(s, "matrix") == 0) {
			if (IsMatrix(data)) {
				err = file->a.f.v->WriteMatrix(data->a.m.v);
			}
		} else if (strcmp(s, "list") == 0) {
			if (IsList(data)) {
				int count = 0;
				err = file->a.f.v->WriteList(data, count);
			}
		} else if (strcmp(s, "tree") == 0) {
			err = file->a.f.v->WriteTree(data);
		} else if (strcmp(s, "graph") == 0) {
			err = file->a.f.v->WriteGraph(data);
		} else if (strcmp(s, "class") == 0) {
			AGmFile *f = file->a.f.v;
			AGmNode *sub_class = AUiClassShell::NameToClass(RootClass(), data);
			AGmNode *base_class = src_node(sub_class, NmSubClass());
			if (sub_class == NULL || base_class == NULL ||
				f->WriteString(base_class->H->a.s.v) ||
				f->WriteString("\n") ||
				AUiProgramMgr::_MakeLibText(f, RootClass(), sub_class, AIP_STEP_CLASS)) {
				err = 1;
			} else {
				err = 0;
			}
		} else if (strcmp(s, "member") == 0) {
			AGmFile *f = file->a.f.v;
			AGmNode *sub_class = AUiClassShell::NameToClass(RootClass(), data);
			AGmNode *base_class = src_node(sub_class, NmSubClass());
			if (sub_class == NULL || base_class == NULL ||
				f->WriteString(base_class->H->a.s.v) ||
				f->WriteString("\n") ||
				AUiProgramMgr::_MakeLibText(f, RootClass(), sub_class, AIP_STEP_MEMBER)) {
				err = 1;
			} else {
				err = 0;
			}
		} else if (strcmp(s, "body") == 0) {
			AGmFile *f = file->a.f.v;
			AGmNode *sub_class = AUiClassShell::NameToClass(RootClass(), data);
			AGmNode *base_class = src_node(sub_class, NmSubClass());
			if (sub_class == NULL || base_class == NULL ||
				f->WriteString(base_class->H->a.s.v) ||
				f->WriteString("\n") ||
				AUiProgramMgr::_MakeLibText(f, RootClass(), sub_class, AIP_STEP_BODY)) {
				err = 1;
			} else {
				err = 0;
			}
		} else {
			err = 1;
		}
		if (err) {
		Unref(val); val = Ref(Integer((long)err));
		} else {
		Unref(val); val = NULL;
		}
	} else if (IsFile(file) && file->a.f.v && type && IsList(type)
			&& Param(type, bin, tail) == 0
			&& Param(tail, idx, size, tail) == 0
			&& idx >= 0 && idx + size <= bin->Length()) {
		int err = file->a.f.v->WriteBinary(bin, (long)idx, (long)size);
		if (err == size) {
			Unref(val); val = NULL;
		} else {
			Unref(val); val = Ref(Integer((long)err));
		}
	} else {
		Unref(file, type, data);
		return interp_error("FileWrite()");
	}
	Unref(file, type, data);
	return 0;
}

int AIpProcess::EvalFileManip() {
	AGmNode *type, *arg1, *arg2;
	GetArg(type, arg1, arg2);
	if (!IsString(type))	{
		Unref(type, arg1, arg2);
		return interp_error("FileManip()");
	}
	int err = 0, error_code;
	AGmString *s1;
	AGmNode *tail;
	AGmNode *n = NULL;
	const char *s = type->a.s.v->Str();
	if (strcmp(s, "unput") == 0) {
		if (!IsFile(arg1) || !IsInteger(arg2)) {
			err = 1;
		} else {
			arg1->a.f.v->Unput((int)arg2->a.i.v);
		}
	} else if (strcmp(s, "curr_pos") == 0) {
		if (!IsFile(arg1)) {
			err = 1;
		} else {
			n= Integer((long)arg1->a.f.v->CurrentPoint());
		}
	} else if (strcmp(s, "back") == 0) {
		if (!IsFile(arg1) || !IsInteger(arg2)) {
			err = 1;
		} else {
			arg1->a.f.v->Back((int)arg2->a.i.v);
		}
	} else if (strcmp(s, "line_no") == 0) {
		if (!IsFile(arg1)) {
			err = 1;
		} else {
			n = Integer(arg1->a.f.v->LineNumber());
		}
	} else if (strcmp(s, "flush") == 0) {
		if (!IsFile(arg1)) {
			err = 1;
		} else {
			arg1->a.f.v->Flush();
		}
	} else if (strcmp(s, "get_dir") == 0) {
		if (AGmFile::GetDir(s1) == 0)
			n = String(s1);
	} else if (strcmp(s, "find_file") == 0) {
		if (!IsString(arg1)) {
			err = 1;
		} else {
			const char* path = arg1->a.s.v->Str();
			if (IsString(arg2)) {
				AGmFile::FindFile(n, path, arg2->a.s.v->Str());
			} else {
				AGmFile::FindFile(n, path, NULL);
			}
		}
	} else if (strcmp(s, "does_exist") == 0) {
		if (!IsString(arg1)) {
			err = 1;
		} else {
			const char* path = arg1->a.s.v->Str();
			if (AGmFile::DoesExist(path)) {
				n = Integer(1);
			}
		}
	} else if (strcmp(s, "is_dir") == 0) {
		if (!IsString(arg1)) {
			err = 1;
		} else {
			const char* path = arg1->a.s.v->Str();
			if (AGmFile::IsDir(path)) {
				n = Integer(1);
			}
		}
	} else if (strcmp(s, "chg_dir") == 0) {
		if (!IsString(arg1)) {
			err = 1;
		} else {
			const char* path = arg1->a.s.v->Str();
			error_code = AGmFile::ChDir(path);
				n = Integer((long)error_code);
		}
	} else if (strcmp(s, "mk_dir") == 0) {
		if (!IsString(arg1)) {
			err = 1;
		} else {
			const char* path = arg1->a.s.v->Str();
			error_code = AGmFile::MkDir(path);
			if (error_code)
				n = Integer((long)error_code);
		}
	} else if (strcmp(s, "remove") == 0) {
		if (!IsString(arg1)) {
			err = 1;
		} else {
			const char* path = arg1->a.s.v->Str();
			error_code = AGmFile::Remove(path);
			if (error_code)
				n = Integer((long)error_code);
		}
	} else if (strcmp(s, "rename") == 0) {
		if (!IsString(arg1) || !IsString(arg2)) {
			err = 1;
		} else {
			const char* path = arg1->a.s.v->Str();
			const char* path2 = arg2->a.s.v->Str();
			error_code = AGmFile::Rename(path, path2);
				n = Integer((long)error_code);
		}
	} else if (strcmp(s, "copy") == 0) {
		if (!IsString(arg1) || !IsString(arg2)) {
			err = 1;
		} else {
			const char* path = arg1->a.s.v->Str();
			const char* path2 = arg2->a.s.v->Str();
			error_code = AGmFile::Copy(path, path2);
			if (error_code)
				n = Integer((long)error_code);
		}

	} else if (strcmp(s, "get_size") == 0) {
		if (!IsString(arg1)) {
			err = 1;
		} else {
			struct stat buff;
			const char* path = arg1->a.s.v->Str();
			if (AGmFile::GetStat(path, &buff) == 0) {
				n = Integer((long)buff.st_size);
			}
		}
	} else if (strcmp(s, "is_readonly") == 0) {
		if (!IsString(arg1)) {
			err = 1;
		} else {
			struct stat buff;
			const char* path = arg1->a.s.v->Str();
			if (AGmFile::GetStat(path, &buff) == 0) {
#ifdef _WINDOWS
				if ((buff.st_mode & S_IWRITE) == 0) {
					n = Integer(1);
				}
#endif
#ifdef _LINUX
				if ((buff.st_mode & S_IWUSR) == 0) {
					n = Integer(1);
				}
#endif
			}
		}
	} else if (strcmp(s, "set_readonly") == 0) {
		if (!IsString(arg1)) {
			err = 1;
		} else {
			struct stat buff;
			const char* path = arg1->a.s.v->Str();
			if (AGmFile::GetStat(path, &buff) == 0) {
#ifdef _WINDOWS
				int prev_mode = buff.st_mode & _S_IWRITE;
				int mode;
				if (arg2) {
					mode = _S_IWRITE;
				} else {
					mode = 0;
				}
#endif
#ifdef _LINUX
				int prev_mode = buff.st_mode;
				int mode;
				if (arg2) {
					mode = (prev_mode | (S_IWUSR|S_IWGRP|S_IWOTH));
				} else {
					mode = (prev_mode & ~(S_IWUSR|S_IWGRP|S_IWOTH));
				}
#endif
				if (prev_mode != mode) {
					if (AGmFile::ChMod(path, mode)) {
						n = Integer(1);
					}
				}
			}
		}
	} else if (strcmp(s, "get_datetime") == 0) {
		if (!IsString(arg1)) {
			err = 1;
		} else {
			struct stat buff;
			const char* path = arg1->a.s.v->Str();
			if (AGmFile::GetStat(path, &buff) == 0) {
				long year, month, day, hour, minute, second, wday;
				get_localtime(buff.st_mtime, year, month, day, hour, minute, second, wday);
				n = List(year, month, day, hour, minute, second);
			}
		}
	} else if (strcmp(s, "set_datetime") == 0) {
		if (!IsString(arg1)) {
			err = 1;
		} else {
#if	_WINDOWS && _64BIT
			long long yy1, mm1, dd1, h1, m1, sec1;
#else
			long yy1, mm1, dd1, h1, m1, sec1;
#endif
			if (Param(arg2, yy1, mm1, dd1, h1, m1, sec1, tail)) {
				err = 1;
			} else {
				struct utimbuf buff;
				const char* path = arg1->a.s.v->Str();
				long ltime = (long)get_mktime((long)yy1, (long)mm1, (long)dd1, (long)h1, (long)m1, (long)sec1);
				buff.actime = ltime;
				buff.modtime = ltime;
				if (AGmFile::SetTime(path, &buff))
					n = Integer(1);
			}
		}
	} else if (strcmp(s, "current_datetime") == 0) {
		long year, month, day, hour, minute, second, wday;
		get_localtime(get_time(), year, month, day, hour, minute, second, wday);
		n = List(year, month, day, hour, minute, second);

	} else if (strcmp(s, "get_dayofweek") == 0) {
#if	_WINDOWS && _64BIT
		long long lyy1, lmm1, ldd1, lh1, lm1, ls1;
		if (Param(arg1, lyy1, lmm1, ldd1, tail) || Param(tail, lh1, lm1, ls1, tail)) {
			err = 1;
		} else {
			long yy1 = (long)lyy1, mm1 = (long)lmm1, dd1 = (long)ldd1;
			long h1 = (long)lh1, m1 = (long)lm1, s1 = (long)ls1;
			long ltime = (long)get_mktime(yy1, mm1, dd1, h1, m1, s1);
			long wday;
			get_localtime(ltime, yy1, mm1, dd1, h1, m1, s1, wday);
			n = Integer(wday);
		}
#else
		long yy1, mm1, dd1, h1, m1, s1;
		if (Param(arg1, yy1, mm1, dd1, tail) || Param(tail, h1, m1, s1, tail)) {
			err = 1;
		} else {
			long ltime = (long)get_mktime(yy1, mm1, dd1, h1, m1, s1);
			long wday;
			get_localtime(ltime, yy1, mm1, dd1, h1, m1, s1, wday);
			n = Integer(wday);
		}
#endif
	} else if (strcmp(s, "unput_buffer_size") == 0) {
		if (IsFile(arg1)) {
			val = Ref(Integer(arg1->a.f.v->UnputBufferSize()));
		}

	} else if (strcmp(s, "tmp_dir") == 0) {
		char *str = AGmFile::GetTempDir();
		if (str) {
			n = String(str);
		}
	} else if (strcmp(s, "tmp_file") == 0) {
		if (IsString(arg1)) {
			AGmString *str = AGmFile::GetTempFile(arg1->a.s.v->Str());
			if (str) {
				n = String(str);
			}
		}

	} else if (strcmp(s, "get_code_type") == 0) {
		if (IsString(arg1) && IsInteger(arg2)) {
			int code = GetCharacterCodeType(arg1->a.s.v->Str(), (int)arg2->a.i.v);
			switch (code) {
			case CODE_NONE:
				n = String("NONE"); break;
			case CODE_JIS:
				n = String("JIS"); break;
			case CODE_SJIS:
				n = String("SJIS"); break;
			case CODE_EUC:
				n = String("EUC"); break;
			case CODE_EUC_THAN_SJIS:
				n = String("EUC_THAN_SJIS"); break;
			case CODE_EUC_OR_SJIS:
				n = String("EUC_OR_SJIS"); break;
			case CODE_UTF8_OR_SJIS:
				n = String("UTF8_OR_SJIS"); break;
			}
		}

	} else {
		err = 1;
	}
	if (err) {
		Unref(type, arg1, arg2);
		return interp_error("FileManip()");
	}
	Unref(val); val = Ref(n);
	Unref(type, arg1, arg2);
	return 0;
}

int AIpProcess::EvalFileMatchStr() {
	AGmNode *file, *str;
	GetArg(file, str);
	if (!IsFile(file) || !IsString(str)) {
		Unref(file, str);
		return interp_error("FileMatchStr()");
	}
	Unref(val); val = NULL;
	if (file->a.f.v->MatchString(str->a.s.v)) {
		val = Ref(Integer(1));
	}
	Unref(file, str);
	return 0;
}

// string manipulation functions
int AIpProcess::EvalStrLen() {
	AGmNode *s;
	GetArg(s);
	if (!IsString(s)) {
		Unref(s);
		return interp_error("StrLen()");
	}
	Unref(val);
	val = Ref(Integer(s->a.s.v->Length()));
	Unref(s);
	return 0;
}
int AIpProcess::EvalGetChar() {
	AGmNode *s, *pos;
	GetArg(s, pos);
	if (!IsString(s) || !IsInteger(pos)) {
		Unref(s, pos);
		return interp_error("GetChar()");
	}
	if (pos->a.i.v < 0 || s->a.s.v->Length() <= pos->a.i.v) {
		Unref(s, pos);
		return interp_error("GetChar() index out of range");
	}	
	Unref(val);
	val = Ref(Integer(s->a.s.v->Component((long)pos->a.i.v)));
	Unref(s, pos);
	return 0;
}
int AIpProcess::EvalSetChar() {
	AGmNode *s, *pos, *c;
	GetArg(s, pos, c);
	if (!IsString(s) || !IsInteger(pos) || !IsInteger(c)) {
		Unref(s, pos, c);
		return interp_error("SetChar()");
	}
	if (pos->a.i.v < 0 || s->a.s.v->Length() <= pos->a.i.v) {
		Unref(s, pos, c);
		return interp_error("SetChar() index out of range");
	}	
	s->a.s.v->Component((long)pos->a.i.v) = (unsigned char)c->a.i.v;
	Unref(s, pos, c);
	return 0;
}
int AIpProcess::EvalSubStr() {
	AGmNode *s, *from, *to;
	GetArg(s, from, to);
	if (!IsString(s) || !IsInteger(from) || !IsInteger(to)) {
		Unref(s, from, to);
		return interp_error("SubStr()");
	}
	if (from->a.i.v < 0 || s->a.s.v->Length() < to->a.i.v ||
			from->a.i.v > to->a.i.v) {
		Unref(s, from, to);
		return interp_error("SubStr() index out of range");
	}	
	AGmString *ss = s->a.s.v->SubString((long)from->a.i.v, (long)to->a.i.v);
	Unref(val);
	val = Ref(String(ss));
	Unref(s, from, to);
	return 0;
}
int AIpProcess::EvalMatchStr() {
	AGmNode *s, *pos, *str;
	GetArg(s, pos, str);
	if (!IsString(s) || !IsInteger(pos) || !IsString(str)) {
		Unref(s, pos, str);
		return interp_error("MatchStr()");
	}
	if (pos->a.i.v < 0) {
		Unref(s, pos, str);
		return interp_error("MatchStr() index out of range");
	}	
	Unref(val); val = NULL;
	AGmFile *f = new AGmFile();
	f->Create(s->a.s.v, AGM_FILE_STR_READ);
	int r = f->MatchString(str->a.s.v);
	if (r)
		val = Ref(Integer(1));
	delete f;
	Unref(s, pos, str);
	return 0;
}
int AIpProcess::EvalTailStr() {
	AGmNode *s, *pos;
	GetArg(s, pos);
	if (!IsString(s) || !IsInteger(pos)) {
		Unref(s, pos);
		return interp_error("TailStr()");
	}
	if (pos->a.i.v < 0 || s->a.s.v->Length() < pos->a.i.v) {
		Unref(s, pos);
		return interp_error("TailStr() index out of range");
	}	
	Unref(val);
	AGmString *ss = s->a.s.v->SubString((long)pos->a.i.v, s->a.s.v->Length());
	val = Ref(String(ss));
	Unref(s, pos);
	return 0;
}
int AIpProcess::EvalInsertChar() {
	AGmNode *s, *pos, *c;
	GetArg(s, pos, c);
	if (!IsString(s) || !IsInteger(pos) || !IsInteger(c)) {
		Unref(s, pos, c);
		return interp_error("InsertChar()");
	}
	if (pos->a.i.v < 0 || s->a.s.v->Length() < pos->a.i.v) {
		Unref(s, pos, c);
		return interp_error("InsertStr() index out of range");
	}
	s->a.s.v->InsertChar((long)pos->a.i.v, (char)c->a.i.v);	
	Unref(s, pos, c);
	return 0;
}
int AIpProcess::EvalDeleteChar() {
	AGmNode *s, *pos;
	GetArg(s, pos);
	if (!IsString(s) || !IsInteger(pos)) {
		Unref(s, pos);
		return interp_error("DeleteChar()");
	}
	if (pos->a.i.v < 0 || s->a.s.v->Length() <= pos->a.i.v) {
		Unref(s, pos);
		return interp_error("DeleteStr() index out of range");
	}
	s->a.s.v->DeleteChar((long)pos->a.i.v);	
	Unref(s, pos);
	return 0;
}
int AIpProcess::EvalSearchStr() {
	AGmNode *s, *pos, *str;
	GetArg(s, pos, str);
	if (!IsString(s) || !IsInteger(pos) || !IsString(str)) {
		Unref(s, pos, str);
		return interp_error("SearchStr()");
	}
	if (pos->a.i.v < 0 || s->a.s.v->Length() < pos->a.i.v) {
		Unref(s, pos, str);
		return interp_error("SearchStr() index out of range");
	}
	Unref(val);
	val = Ref(Integer(s->a.s.v->Search(str->a.s.v, (long)pos->a.i.v)));	
	Unref(s, pos, str);
	return 0;
}
int AIpProcess::EvalReplaceStr() {
	AGmNode *s, *from, *to, *str;
	GetArg(s, from, to, str);
	if (!IsString(s) || !IsInteger(from) || !IsInteger(to) || !IsString(str)) {
		Unref(s, from, to, str);
		return interp_error("ReplaceStr()");
	}
	if (from->a.i.v < 0 || s->a.s.v->Length() < to->a.i.v ||
			from->a.i.v > to->a.i.v) {
		Unref(s, from, to, str);
		return interp_error("ReplaceStr() index out of range");
	}
	s->a.s.v->ReplaceStr((long)from->a.i.v, (long)to->a.i.v, str->a.s.v);	
	Unref(s, from, to, str);
	return 0;
}
int AIpProcess::EvalAppendStr() {
	AGmNode *s, *str;
	GetArg(s, str);
	if (IsString(s) && IsString(str)) {
		s->a.s.v->Append(str->a.s.v);
	} else if (IsString(s) && IsInteger(str)) {
		s->a.s.v->Append((char)str->a.i.v);
	} else if (IsString(s) && str == NULL) {
	} else {
		Unref(s, str);
		return interp_error("AppendStr()");
	}
	Unref(val); val = Ref(s);
	Unref(s, str);
	return 0;
}
int AIpProcess::EvalIsFirstChar() {
	AGmNode *c;
	GetArg(c);
	if (!IsInteger(c)) {
		Unref(c);
		return interp_error("FirstChar()");
	}
	Unref(val); val = NULL;
	if (isFirstChar((unsigned char)c->a.i.v)) {
		val = Ref(Integer(1));
	}
	Unref(c);
	return 0;
}
int AIpProcess::EvalIsNormalChar() {
	AGmNode *c;
	GetArg(c);
	if (!IsInteger(c)) {
		Unref(c);
		return interp_error("NormalChar()");
	}
	char cc = (char)c->a.i.v;
	Unref(val); val = NULL;
	if (' ' <= cc && cc <= '~') {
		val = Ref(Integer(1));
	}
	Unref(c);
	return 0;
}
int AIpProcess::EvalStrMisc() {
	AGmNode *cmd, *arg1, *arg2;
	GetArg(cmd, arg1, arg2);
	if (!IsString(cmd)) {
		Unref(cmd, arg1, arg2);
		return interp_error("StrMisc()");
	}
	const char *s = cmd->a.s.v->Str();
	Unref(val); val = NULL;
	if (strcmp(s, "starts_with") == 0) {
		if (!IsString(arg1) || !IsString(arg2)) {
			Unref(cmd, arg1, arg2);
			return interp_error("StrMisc() starts_with");
		}
		if (arg1->a.s.v->StartsWith(arg2->a.s.v)) {
			val = Ref(Integer(1));
		}
	} else if (strcmp(s, "ends_with") == 0) {
		if (!IsString(arg1) || !IsString(arg2)) {
			Unref(cmd, arg1, arg2);
			return interp_error("StrMisc() ends_with");
		}
		if (arg1->a.s.v->EndsWith(arg2->a.s.v)) {
			val = Ref(Integer(1));
		}
	} else if (strcmp(s, "is_empty") == 0) {
		if (!IsString(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("StrMisc() is_empty");
		}
		if (arg1->a.s.v->IsEmpty()) {
			val = Ref(Integer(1));
		}
	} else if (strcmp(s, "trim") == 0) {
		if (!IsString(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("StrMisc() trim");
		}
		AGmString *s = new AGmString(arg1->a.s.v);
		s->Trim();
		val = Ref(String(s));

	} else if (strcmp(s, "xml_encode") == 0) {
		if (!IsString(arg1) || !IsInteger(arg2)) {
			Unref(cmd, arg1, arg2);
			return interp_error("StrMisc() xml_encode");
		}
		val = Ref(String(arg1->a.s.v->XmlEncode((int)arg2->a.i.v)));
	} else if (strcmp(s, "xml_decode") == 0) {
		if (!IsString(arg1) || !IsInteger(arg2)) {
			Unref(cmd, arg1, arg2);
			return interp_error("StrMisc() xml_decode");
		}
		val = Ref(String(arg1->a.s.v->XmlDecode((int)arg2->a.i.v)));

	} else if (strcmp(s, "strcmp") == 0) {
		if (!IsString(arg1) || !IsString(arg2)) {
			Unref(cmd, arg1, arg2);
			return interp_error("StrMisc() strcmp");
		}
		val = Ref(Integer(arg1->a.s.v->Compare(arg2->a.s.v->Str())));
	} else if (strcmp(s, "strcmpi") == 0) {
		if (!IsString(arg1) || !IsString(arg2)) {
			Unref(cmd, arg1, arg2);
			return interp_error("StrMisc() strcmpi");
		}
		val = Ref(Integer(arg1->a.s.v->CompareI(arg2->a.s.v->Str())));
	} else if (strcmp(s, "to_upper") == 0) {
		if (!IsString(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("StrMisc() to_upper");
		}
		AGmString *s = new AGmString(arg1->a.s.v);
		char c;
		int n = s->Length();
		for (int i = 0; i < n; i++) {
			c = s->Component(i);
			if ('a' <= c  && c <= 'z') {
				s->Component(i) = (unsigned char)(c - 'a' + 'A');
				continue;
			}
#ifdef	_WINDOWS
			if (isFirstChar(c)) i++;
#endif
#ifdef	_LINUX
			int i2;
			if (isUTF8((unsigned char*)s->Str()+i, i2)) i += i2-1;
#endif
		}
		val = Ref(String(s));
		
	} else if (strcmp(s, "to_lower") == 0) {
		if (!IsString(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("StrMisc() to_lower");
		}
		AGmString *s = new AGmString(arg1->a.s.v);
		char c;
		int n = s->Length();
		for (int i = 0; i < n; i++) {
			c = s->Component(i);
			if ('A' <= c  && c <= 'Z') {
				s->Component(i) = (unsigned char)(c - 'A' + 'a');
				continue;
			}
#ifdef	_WINDOWS
			if (isFirstChar(c)) i++;
#endif
#ifdef	_LINUX
			int i2;
			if (isUTF8((unsigned char*)s->Str()+i, i2)) i += i2-1;
#endif
		}
		val = Ref(String(s));
		
	} else if (strcmp(s, "is_ident") == 0) {
		if (!IsString(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("StrMisc() is_ident");
		}
		AGmString *s = arg1->a.s.v;
		char c;
		int i, n = s->Length();
		for (i = 0; i < n; i++) {
			c = s->Component(i);
			if ('A' <= c  && c <= 'Z') {
				continue;
			}
			if ('a' <= c  && c <= 'z') {
				continue;
			}
			if (c == '_') {
				continue;
			}
			if (i > 0 && '0' <= c  && c <= '9') {
				continue;
			}
#ifdef	_WINDOWS
			if (isFirstChar(c)) {
				i++;
				continue;
			}
#endif
#ifdef	_LINUX
			int i2;
			if (isUTF8((unsigned char*)s->Str()+i, i2)) {
				i += i2-1;
				continue;
			}
#endif
			break;
		}
		if (i < n) {
		} else {
			val = Ref(Integer(1));
		}
		
	} else if (strcmp(s, "file_ext") == 0) {
		if (!IsString(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("StrMisc() file_ext");
		}
		val = Ref(String(arg1->a.s.v->FileExt()));

	} else if (strcmp(s, "dir_of_path") == 0) {
		if (!IsString(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("StrMisc() dir_of_path");
		}
		const char* delim = "/";
		if (IsString(arg2) && arg2->a.s.v->Length() > 0) delim = arg2->a.s.v->Str();
		val = Ref(String(arg1->a.s.v->DirOfPath(delim)));

	} else if (strcmp(s, "name_of_path") == 0) {
		if (!IsString(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("StrMisc() name_of_path");
		}
		const char* delim = "/";
		if (IsString(arg2) && arg2->a.s.v->Length() > 0) delim = arg2->a.s.v->Str();
		val = Ref(String(arg1->a.s.v->NameOfPath(delim)));

	} else if (strcmp(s, "split") == 0) {
		if (!IsString(arg1) || !IsInteger(arg2)) {
			Unref(cmd, arg1, arg2);
			return interp_error("StrMisc() split");
		}
		val = Ref(split_str(arg1->a.s.v, (int)arg2->a.i.v));

	} else if (strcmp(s, "chg_delimiter") == 0) {
		if (!IsString(arg1) || !arg2 || !IsList(arg2) || !arg2->T || !IsList(arg2->T)
				|| !IsInteger(arg2->H) || !IsInteger(arg2->TH)) {
			Unref(cmd, arg1, arg2);
			return interp_error("StrMisc() chg_delimiter");
		}
		AGmString *s = new AGmString(arg1->a.s.v);
		s->ChangeFileDelimiter((char)arg2->HI, (char)arg2->THI);
		val = Ref(String(s));

	} else if (strcmp(s, "is_alpha") == 0) {
		if (IsInteger(arg1)) {
			unsigned char c = (unsigned char)arg1->a.i.v;
			if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
				val = Ref(Integer(1));
		}
	} else if (strcmp(s, "is_digit") == 0) {
		if (IsInteger(arg1)) {
			unsigned char c = (unsigned char)arg1->a.i.v;
			if ('0' <= c && c <= '9')
				val = Ref(Integer(1));
		}

	} else if (strcmp(s, "u_to_d") == 0) {
		if (IsString(arg1)) {
			char *str = ADvCEdit::toCEditString(arg1->a.s.v->Str());
			val = Ref(String(str));
			delete [] str;
		}
	} else if (strcmp(s, "d_to_u") == 0) {
		if (IsString(arg1)) {
			char *str = ADvCEdit::fromCEditString(arg1->a.s.v->Str());
			val = Ref(String(str));
			delete [] str;
		}

	} else if (strcmp(s, "jis_to_sjis") == 0) {
		if (IsString(arg1)) {
			AGmString *s = JIS_to_SJIS(arg1->a.s.v->Str());
			if (s) val = Ref(String(s));
		}
	} else if (strcmp(s, "sjis_to_jis") == 0) {
		if (IsString(arg1)) {
			AGmString *s = SJIS_to_JIS(arg1->a.s.v->Str());
			if (s) val = Ref(String(s));
		}
	} else if (strcmp(s, "euc_to_sjis") == 0) {
		if (IsString(arg1)) {
			AGmString *s = EUC_to_SJIS(arg1->a.s.v->Str());
			if (s) val = Ref(String(s));
		}
	} else if (strcmp(s, "sjis_to_euc") == 0) {
		if (IsString(arg1)) {
			AGmString *s = SJIS_to_EUC(arg1->a.s.v->Str());
			if (s) val = Ref(String(s));
		}
	} else if (strcmp(s, "sjis_to_utf8") == 0) {
		if (IsString(arg1)) {
			AGmString *s = SJIS_to_UTF8(arg1->a.s.v->Str());
			if (s) val = Ref(String(s));
		}
	} else if (strcmp(s, "utf8_to_sjis") == 0) {
		if (IsString(arg1)) {
			AGmString *s = UTF8_to_SJIS(arg1->a.s.v->Str());
			if (s) val = Ref(String(s));
		}
	} else if (strcmp(s, "euc_to_utf8") == 0) {
		if (IsString(arg1)) {
			AGmString *s = EUC_to_UTF8(arg1->a.s.v->Str());
			if (s) val = Ref(String(s));
		}
	} else if (strcmp(s, "utf8_to_euc") == 0) {
		if (IsString(arg1)) {
			AGmString *s = UTF8_to_EUC(arg1->a.s.v->Str());
			if (s) val = Ref(String(s));
		}

	} else if (strcmp(s, "zen_to_han") == 0) {
		if (IsString(arg1)) {
			char* str = zen_to_han(arg1->a.s.v->Str());
			val = Ref(String(str));
			delete [] str;
		}
	} else if (strcmp(s, "han_to_zen") == 0) {
		if (IsString(arg1)) {
			char* str = han_to_zen(arg1->a.s.v->Str());
			val = Ref(String(str));
			delete [] str;
		}

	} else if (strcmp(s, "to_quote_string") == 0) {
		if (IsString(arg1)) {
			const char *str = arg1->a.s.v->Str();
			AGmString *ss = new AGmString(64);
			long n = (long)strlen(str);
			for (long i = 0; i < n; i++) {
				char c = str[i];
#ifdef	_WINDOWS
				if (isFirstChar(c)) {
					ss->Append(c);	
					c = str[++i];
					ss->Append(c);
#endif
#ifdef	_LINUX
				int i2;
				if (isUTF8((unsigned char*)&str[i], i2)) {
					ss->Append((const char*)utf8);
					i += i2-1;
#endif
				} else if (c == '\\') {
					ss->Append("\\\\");
				} else if (c == '\t') {
					ss->Append("\\t");
				} else if (c == '\r') {
					ss->Append("\\r");	
				} else if (c == '\n') {
					ss->Append("\\n");	
				} else if (c == '\'') {
					ss->Append("\\'");	
				} else if (c == '\"') {
					ss->Append("\\\""); 
				} else {
					ss->Append(c);
				}
			}
			val = Ref(String(ss));
		}

	} else if (strcmp(s, "to_single_quote_string") == 0) {
		if (IsString(arg1)) {
			AGmString *ss = new AGmString(64);
			ss->WriteSingleQString(arg1->a.s.v->Str());
			val = Ref(String(ss));
		}

	} else if (strcmp(s, "to_csv_string") == 0) {
		if (IsString(arg1)) {
			const char *s = arg1->a.s.v->Str();
			int convert = 0;
			AGmString *str = new AGmString(64);
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
				} else if (c == '\"') {
					str->Append(c);
					str->Append(c);
					convert = 1;
				} else {
					str->Append(c);
				}
			}
			if (convert) {
				str->InsertChar(0, '\"');
				str->Append('\"');
			}
			val = Ref(String(str));
		}

	} else if (strcmp(s, "param_to_string") == 0) {
		if (IsString(arg1)) {
			const char *str = arg1->a.s.v->Str();
			AGmString *ss = new AGmString(64);
			long n = (long)strlen(str);
			for (long i = 0; i < n; i++) {
				char c = str[i], c2; int j, m;
				if (c == '+') {
					ss->Append(' ');	
				} else if (c == '%') {
					c = 0;
					c2 = str[i+1];
					m = 1;
					for (j = 0; j < m; j++) {
						c2 = str[++i];
						if ('0' <= c2 && c2 <= '9') {
							c = (char)((c2 - '0') << 4);
						} else if ('a' <= c2 && c2 <= 'f') {
							c = (char)((c2 - 'a' + 10) << 4);
						} else if ('A' <= c2 && c2 <= 'F') {
							c = (char)((c2 - 'A' + 10) << 4);
						}
						c2 = str[++i];
						if ('0' <= c2 && c2 <= '9') {
							c = (char)(c + (c2 - '0'));
						} else if ('a' <= c2 && c2 <= 'f') {
							c = (char)(c + (c2 - 'a' + 10));
						} else if ('A' <= c2 && c2 <= 'F') {
							c = (char)(c + (c2 - 'A' + 10));
						}
						ss->Append(c);	
					}
				} else {
					ss->Append(c);	
				}
			}
			val = Ref(String(ss));
		}

	} else if (strcmp(s, "make_str_uniq") == 0) {
		MakeStrUnique(arg1);
		val = Ref(arg1);

	} else {
		Unref(cmd, arg1, arg2);
		return interp_error("StrMisc() format_numeric");
	}
	Unref(cmd, arg1, arg2);
	return 0;
}

// misc functions
int AIpProcess::EvalIsType() {
	AGmNode *data, *type;
	GetArg(data, type);
	if (!IsString(type)) {
		Unref(data, type);
		return interp_error("IsType()");
	}
	Unref(val); val = NULL;
	const char *s = type->a.s.v->Str();
	if (data) {
		switch (data->type) {
		case AGM_NODE_INTEGER:
			if (strcmp(s, "integer") == 0) val = Ref(Integer(1));
			break;
		case AGM_NODE_REAL:
			if (strcmp(s, "real") == 0) val = Ref(Integer(1));
			break;
		case AGM_NODE_STRING:
			if (strcmp(s, "string") == 0 && data->a.s.v) val = Ref(Integer(1));
			break;
		case AGM_NODE_BINARY:
			if (strcmp(s, "binary") == 0 && data->a.bin.v) val = Ref(Integer(1));
			break;
		case AGM_NODE_BIGINT:
			if (strcmp(s, "bigint") == 0 && data->a.bin.v) val = Ref(Integer(1));
			break;
		case AGM_NODE_GFP:
			if (strcmp(s, "gfp") == 0) val = Ref(Integer(1));
			break;
		case AGM_NODE_MATRIX:
			if (strcmp(s, "matrix") == 0) val = Ref(Integer(1));
			break;
		case AGM_NODE_BITMAP:
			if (strcmp(s, "bitmap") == 0) val = Ref(Integer(1));
			break;
		case AGM_NODE_OBJECT:
			if (strcmp(s, "object") == 0) val = Ref(Integer(1));
			break;
		case AGM_NODE_PTR:
			if (strcmp(s, "pointer") == 0) val = Ref(Integer(1));
			break;
		case AGM_NODE_ARRAY:
			if (strcmp(s, "array") == 0) val = Ref(Integer(1));
			break;
		case AGM_NODE_LIST:
			if (strcmp(s, "list") == 0) val = Ref(Integer(1));
			break;
		case AGM_NODE_FILE:
			if (strcmp(s, "file") == 0 && data->a.f.v) val = Ref(Integer(1));
			break;
		case AGM_NODE_WINDOW:
			if (strcmp(s, "window") == 0 && data->a.w.v) val = Ref(Integer(1));
			break;
		case AGM_NODE_OCX:
			if (strcmp(s, "ocx") == 0) val = Ref(Integer(1));
			break;
		case AGM_NODE_VARIANT:
			if (strcmp(s, "variant") == 0) val = Ref(Integer(1));
			break;
		case AGM_NODE_SQL_CONN:
			if (strcmp(s, "connection") == 0) val = Ref(Integer(1));
			break;
		case AGM_NODE_SQL_STAT:
			if (strcmp(s, "statement") == 0) val = Ref(Integer(1));
			break;
		default:
			break;
		}
	} else {
		if (strcmp(s, "list") == 0) val = Ref(Integer(1));
	}
	Unref(data, type);
	return 0;
}
int AIpProcess::EvalAddrEq() {
	AGmNode *arg1, *arg2;
	GetArg(arg1, arg2);
	if (arg1 == arg2) {
		Unref(val); val = Ref(Integer(1));
	} else {
		Unref(val); val = NULL;
	}
	Unref(arg1, arg2);
	return 0;
}
int AIpProcess::EvalGetId() {
	Unref(val); val = Ref(Integer(GetUniqueID()));
	return 0;
}
int AIpProcess::EvalReleaseId() {
	AGmNode *id;
	GetArg(id);
	if (!IsInteger(id)) {
		Unref(id);
		return interp_error("ReleaseId()");
	}
	ReleaseUniqueID(id->a.i.v);
	Unref(id);
	return 0;
}

int AIpProcess::EvalIsClass() {
	AGmNode *obj, *class_id;
	GetArg(obj, class_id);
	if (!IsObject(obj)) {
		Unref(obj, class_id);
		return interp_error("IsClass()");
	}
	if (obj->a.o.class_id == class_id) {
		Unref(val); val = Ref(Integer(1));
	} else {
		Unref(val); val = NULL;
	}
	Unref(obj, class_id);
	return 0;
}
int AIpProcess::EvalIsDerived() {
	AGmNode *obj, *class_id;
	GetArg(obj, class_id);
	if (!IsObject(obj)) {
		Unref(obj, class_id);
		return interp_error("IsClass()");
	}

	if (_EvalIsDerived(obj->a.o.class_id, class_id)) {
		Unref(val); val = Ref(Integer(1));
	} else {
		Unref(val); val = NULL;
	}
	Unref(obj, class_id);
	return 0;
}
int AIpProcess::_EvalIsDerived(AGmNode *obj_cl, AGmNode *class_id) {
	if (obj_cl == class_id) return 1;
	AGmLink* l;
	for (l = next_outlink(class_id, NULL, NmSubClass());
			l; l = next_outlink(class_id, l, NmSubClass())) {
		if (_EvalIsDerived(obj_cl, l->arc->dst)) return 1;
	}
	return 0;
}

int AIpProcess::EvalSetToClipboard() {
	AGmNode *obj;
	GetArg(obj);
	SetToClipboard(obj);
	Unref(obj);
	return 0;
}
int AIpProcess::EvalGetFromClipboard() {
	AGmNode *obj = GetFromClipboard();
	Unref(val); val = Ref(obj);
	return 0;
}

int AIpProcess::EvalGC() {
	AGmNode *cmd;
	GetArg(cmd);
	Unref(val); val = NULL;
	if (cmd == NULL) {
#if	_WINDOWS && _64BIT
		long long num_new_free, num_free, num_use;
#else
		long num_new_free, num_free, num_use;
#endif
		GC(&num_new_free, &num_free, &num_use);
		val = Ref(List(num_new_free, num_free, num_use));
	} else if (IsString(cmd)) {
		const char *s = cmd->a.s.v->Str();
		if (strcmp(s, "ref_count") == 0) {
#if	_WINDOWS && _64BIT
			long long ref_count_zero, ref_count_minus, num_new_free;
#else
			long ref_count_zero, ref_count_minus, num_new_free;
#endif
			GCtestRefCount(&ref_count_zero, &ref_count_minus, &num_new_free);
			val = Ref(List(ref_count_zero, ref_count_minus, num_new_free));
		} else if (strcmp(s, "cyclic") == 0) {
#if	_WINDOWS && _64BIT
			long long not_cyclic, cyclic, num_new_free;
#else
			long not_cyclic, cyclic, num_new_free;
#endif
			GCtestCyclic(&not_cyclic, &cyclic, &num_new_free);
			val = Ref(List(not_cyclic, cyclic, num_new_free));
		} else if (strcmp(s, "gc_cyclic_only") == 0) {
#if	_WINDOWS && _64BIT
			long long num_new_free, num_free, num_use;
#else
			long num_new_free, num_free, num_use;
#endif
			GCcyclicOnly(&num_new_free, &num_free, &num_use);
			val = Ref(List(num_new_free, num_free, num_use));
		}
	}
	Unref(cmd);
	return 0;
}

#ifdef	_WINDOWS
static DWORD stdout_func(AGmNode* opt) {
	AGmFile *f = opt->a.f.v;
	char buf[512]; unsigned long num;
	BOOL r; DWORD err;
	for (;;) {
		r = ::ReadFile(f->rStdoutPipe, buf, 511, &num, NULL);
		err = GetLastError();
		if (r != FALSE && num != 0) {
			char c = 0;
			buf[num] = 0;
			for (unsigned long i = 0; i < num; i++) {
				c = buf[i];
				if (isFirstChar(c)) {
					i++; continue;
				}
				if (c == '\r') {
					for (unsigned long j = i; j < num; j++) {
						buf[j] = buf[j+1];
					}
					num--;
				}
			}
			::EnterCriticalSection(&f->cs);
			if (f->rbuf1 == NULL) f->rbuf1 = new AGmString(512);
			f->rbuf1->Append(buf);
			::LeaveCriticalSection(&f->cs);
			if (c == EOF) break;
		}
		if (r == FALSE && err == ERROR_BROKEN_PIPE) break;
	}

	::CloseHandle(f->rStdoutPipe);
	f->rStdoutPipe = NULL;

	::EnterCriticalSection(&f->cs);
	if (f->rbuf1 == NULL) f->rbuf1 = new AGmString(512);
	f->rbuf1->Append(EOF);
	::LeaveCriticalSection(&f->cs);

	f->stdoutThread = NULL;
	::ExitThread(0);
}
#endif	// _WINDOWS

#ifdef	_LINUX
static gpointer stdout_func(gpointer data) {
	AGmFile* f = (AGmFile*)data;
	char buf[513]; int num;
	for (;;) {
		num = read(f->stdout_pipe[0], buf, 512);
		if (num <= 0) break;
		int c;
		buf[num] = 0;
		for (unsigned long i = 0; i < num; i++) {
			c = buf[i];
			int i2;
			if (isUTF8((unsigned char*)&buf[i], i2)) {
				i += i2-1; continue;
			}
			if (c == '\r') {
				for (unsigned long j = i; j < num; j++) {
					buf[j] = buf[j+1];
				}
				num--;
			}
		}
		g_mutex_lock(&f->mutex);
		if (f->rbuf1 == NULL) f->rbuf1 = new AGmString(512);
		f->rbuf1->Append(buf);
		g_mutex_unlock(&f->mutex);
	}

	close(f->stdout_pipe[0]); f->stdout_pipe[0] = -1;

	g_mutex_lock(&f->mutex);
	if (f->rbuf1 == NULL) f->rbuf1 = new AGmString(512);
	f->rbuf1->Append(EOF);
	g_mutex_unlock(&f->mutex);

	f->stdoutThread = NULL;
	g_thread_exit(NULL);
	return NULL;
}
#endif

int AIpProcess::EvalExec() {
	AGmNode *cmd, *dir, *show_wnd, *opt;
	GetArg(cmd, dir, show_wnd, opt);
	if (!IsString(cmd) || !IsString(dir)) {
		Unref(cmd, dir, show_wnd, opt);
		return interp_error("Exec()");
	}
#ifdef	_WINDOWS
	BOOL pipe = FALSE;
	STARTUPINFO sInfo;
	sInfo.cb = sizeof(STARTUPINFO);
	sInfo.lpReserved = NULL;
	sInfo.lpReserved2 = NULL;
	sInfo.cbReserved2 = 0;
	sInfo.lpDesktop = NULL;
	sInfo.lpTitle = NULL;
	sInfo.dwFlags = STARTF_USESHOWWINDOW;
	sInfo.wShowWindow = (unsigned short)(show_wnd ? SW_SHOW : SW_HIDE);
	if (IsFile(opt) && opt->a.f.v->Type() == AGM_FILE_PIPE) {
		pipe = TRUE;
		sInfo.dwFlags |= STARTF_USESTDHANDLES;
		sInfo.hStdInput = opt->a.f.v->rStdinPipe;
		sInfo.hStdOutput = sInfo.hStdError = opt->a.f.v->wStdoutPipe;
	}
	PROCESS_INFORMATION pInfo;
	BOOL ok = ::CreateProcess(
		NULL, /* application name */
		(LPSTR)cmd->a.s.v->Str(), /* command line string */
		NULL, /* process attribute (default security) */
		NULL, /* thread attribute (default security) */
		TRUE, /* inherit */
		0, /* creation flag */
		NULL, /* environment */
		dir->a.s.v->Str(), /* current directory name */
		&sInfo,
		&pInfo);
	if (ok) {
		if (IsInteger(opt) && opt->a.i.v == 1) {
			::WaitForSingleObject(pInfo.hProcess, INFINITE);
			DWORD exitCode = 0;
			GetExitCodeProcess(pInfo.hProcess, &exitCode);
			Unref(val); val = Ref(Integer(exitCode));
		} else {
			Unref(val); val = Ref(Integer(1));
		}
	} else {
		Unref(val); val = NULL; // fail to exec
	}
	if (ok)
		::CloseHandle(pInfo.hProcess);
	if (ok && pipe) {
		AGmFile *f = opt->a.f.v;
		SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
		DWORD rThreadID;
		f->stdoutThread = ::CreateThread(
			&sa, /* security attribute */
			0, /* stack size */
			(LPTHREAD_START_ROUTINE)stdout_func,
			opt /* argument */,
			0, /* creation flag */
			&rThreadID);
		::CloseHandle(f->wStdoutPipe); f->wStdoutPipe = NULL;
		::CloseHandle(f->rStdinPipe); f->rStdinPipe = NULL;
	}
#endif
#ifdef	_LINUX
	Unref(val); val = NULL;
	if (IsInteger(opt) && opt->a.i.v == 1) {
		int pid = fork();
		if (pid == 0) {
			// child
			char str[4096];
			char *argv[256]; char *cp = str;
			chdir(dir->a.s.v->Str());
			AGmFile *f = new AGmFile();
			f->Create(cmd->a.s.v, AGM_FILE_STR_READ);
			int i;
			for (i = 0; i < 256; i++) {
				AGmString *s;
				if (f->ReadQString(s) && f->ReadString(s)) break;
				argv[i] = cp; strcpy(cp, s->Str());
				cp += s->Length(); *cp++ = 0;
				delete s;
			}
			argv[i] = NULL;
			delete f;
			execvp(argv[0], argv);
		} else {
			// parent
			if (pid > 0) {
				int exit_code;
				waitpid(pid, &exit_code, 0);
				val = Ref(Integer(exit_code));
			}
		}
	}
	if (IsFile(opt) && opt->a.f.v->Type() == AGM_FILE_PIPE) {
		AGmFile *f = opt->a.f.v;
		int pid = fork();
		if (pid == 0) {
			// child
			close(0);
			dup(f->stdin_pipe[0]);
			close(1);
			dup(f->stdout_pipe[1]);
			close(2);
			dup(f->stdout_pipe[1]);
			close(f->stdin_pipe[0]); f->stdin_pipe[0] = -1;
			close(f->stdin_pipe[1]); f->stdin_pipe[1] = -1;
			close(f->stdout_pipe[0]); f->stdout_pipe[0] = -1;
			close(f->stdout_pipe[1]); f->stdout_pipe[1] = -1;

			char str[4096];
			chdir(dir->a.s.v->Str());
			char *argv[256]; char *cp = str;
			AGmFile *f = new AGmFile();
			f->Create(cmd->a.s.v, AGM_FILE_STR_READ);
			int i;
			for (i = 0; i < 256; i++) {
				AGmString *s;
				if (f->ReadQString(s) && f->ReadString(s)) break;
				argv[i] = cp; strcpy(cp, s->Str());
				cp += s->Length(); *cp++ = 0;
				delete s;
			}
			argv[i] = NULL;
			delete f;
			execvp(argv[0], argv);
		} else {
			// parent
			close(f->stdin_pipe[0]); f->stdin_pipe[0] = -1;
			close(f->stdout_pipe[1]); f->stdout_pipe[1] = -1;
			f->stdoutThread = g_thread_new(AL_STR("pipe-parent"), stdout_func, f);
			f->pid = pid;
			Unref(val); val = Ref(Integer(pid));
		}
	}
#endif
	Unref(cmd, dir, show_wnd, opt);
	return 0;
}

int AIpProcess::EvalLibrary() {
	AGmNode *cmd, *arg;
	GetArg(cmd, arg);
	if (!IsString(cmd) || !IsString(arg)) {
		Unref(cmd, arg);
		return interp_error("Library()");
	}
	Unref(val); val = Ref(Integer(1));
	const char *s = cmd->a.s.v->Str();
	if (strcmp(s, "load") == 0) {
		AGmFile *f = new AGmFile();
		if (f->Create(arg->a.s.v->Str(), AGM_FILE_READ) == 0 &&
				AUiProgramMgr::_LoadLib(f, NULL, RootClass(), NULL) == 0) {
			Unref(val); val = NULL;
		}
		delete f;
	} else if (strcmp(s, "can_unload") == 0) {
		AGmNode *_class = AUiClassShell::NameToClass(RootClass(), arg);
		if (_class == NULL || Refered(_class, AUiClassShell::ReferedFromOutside)) {
			Unref(val); val = NULL;
		}
	} else if (strcmp(s, "unload") == 0) {
		AGmNode *_class = AUiClassShell::NameToClass(RootClass(), arg);
		if (_class && !Refered(_class, AUiClassShell::ReferedFromOutside)) {
			AGmLink *l = next_inlink(_class, NULL, NmSubClass());
			ASSERT(l);
			DeleteArc(l->arc);
			AUiVtblUpdate = 0;
			Unref(val); val = NULL;
		}
	} else {
		Unref(cmd, arg);
		return interp_error("Library()");
	}
	Unref(cmd, arg);
	return 0;
}

int AIpProcess::EvalWndMessage() {
	AGmNode *wnd, *cmd, *arg1, *arg2;
	GetArg(wnd, cmd, arg1, arg2);
	if (!IsString(cmd)) {
		Unref(wnd, cmd, arg1, arg2);
		return interp_error("WndMessage()");
	}
	const char *s = cmd->a.s.v->Str();
	Unref(val); val = NULL;
	if (strcmp(s, "size") == 0) {
		if (!IsWindow(wnd)) {
			Unref(wnd, cmd, arg1, arg2);
			return interp_error("WndMessage() size");
		}
		ADvWindow *aw = wnd->a.w.v;
		CWnd *w = NULL;
		if (aw->View()) {
			w = aw->View();
		}
#ifdef	_WINDOWS
		if (aw->OleCtrl()) {
			w = aw->OleCtrl();
		}
		if (w) {
			CRect r;
			w->GetClientRect(&r);
			w->PostMessage(WM_SIZE, SIZE_RESTORED,
				MAKELPARAM((WORD)r.Width(), (WORD)r.Height()) );
		}
#endif
#ifdef	_LINUX
	// TODO_LINUX WM_SIZE
#endif

	} else if (strcmp(s, "register_msg") == 0) {
		if (!IsString(arg1)) {
			Unref(wnd, cmd, arg1, arg2);
			return interp_error("WndMessage() register_msg");
		}
#ifdef	_WINDOWS
		val = Ref(Integer((::RegisterWindowMessage(arg1->a.s.v->Str()))));
#endif
#ifdef	_LINUX
		ADvCMainFrame *frw = (ADvCMainFrame*)ADvGetApp()->m_pMainWnd;
		AGmNode *n = frw->_registered_msg;
		long value = 0;
		const char *str = arg1->a.s.v->Str();
		int i, len = strlen(str);
		for (i = 0; i < len; i++) {
			value = value * 50537 + str[i];
		}
		AGmNode *v = Integer(value);
		while (dst_node(n, v)) {
			RefUnref(v);
			v = Integer(++value);
		}
		CreateArc(n, String(arg1->a.s.v->Str()), v);
		val = Ref(Integer(value));
#endif
	} else if (strcmp(s, "unregister_msg") == 0) {
		if (IsInteger(arg1)) {
#ifdef	_LINUX
			ADvCMainFrame *frw = (ADvCMainFrame*)ADvGetApp()->m_pMainWnd;
			set_dst_node(frw->_registered_msg, arg1, (AGmNode*)NULL);
#endif
		}

	} else if (strcmp(s, "msg_callback") == 0) {
		if ( !IsInteger(arg1) || !( arg2 == NULL ||
				(IsList(arg2)
				 && arg2->T && IsList(arg2->T)
				 && arg2->TT && IsList(arg2->TT)) ) ) {
			Unref(wnd, cmd, arg1, arg2);
			return interp_error("WndMessage() msg_callback");
		}
		ADvCWinApp *app = ADvGetApp();
		if (app)
			((ADvCMainFrame*)app->m_pMainWnd)->SetMsgCallback((long)arg1->a.i.v, arg2);

	} else if (strcmp(s, "hwnd") == 0) {
#ifdef	_WINDOWS
		HWND hwnd = 0;
		if (IsWindow(wnd)) {
			ADvWindow *aw = wnd->a.w.v;
			if (aw->View()) {
				hwnd = aw->View()->m_hWnd;
			}
			if (aw->OleCtrl()) {
				hwnd = aw->OleCtrl()->m_hWnd;
			}
			if (aw->Frame()) {
				hwnd = aw->Frame()->m_hWnd;
			}
		} else {
			CWnd *pWnd = AfxGetApp()->m_pMainWnd;
			if (pWnd)
				hwnd = pWnd->m_hWnd;
		}
		Unref(val); val = Ref(Integer((long)hwnd));
#endif
#ifdef	_LINUX
		void *hwnd = NULL;
		if (IsWindow(wnd)) {
			ADvWindow *aw = wnd->a.w.v;
			if (aw->View()) {
				hwnd = aw->View()->m_pWidget;
			}
			if (aw->Frame()) {
				hwnd = aw->Frame()->m_pToplevelWidget;
			}
		} else {
			hwnd = ((ADvCMainFrame*)ADvGetApp()->m_pMainWnd)->m_pToplevelWidget;
		}
		val = Ref(Integer((long)hwnd));
#endif

	} else if (strcmp(s, "isHWND") == 0) {
#ifdef	_WINDOWS
		if (IsInteger(arg1)) {
			if (::IsWindow((HWND)arg1->a.i.v)) {
				val = Ref(Integer(1));
			}
		}
#endif

	} else if (strcmp(s, "send_msg") == 0) {
		if (!IsInteger(wnd) || !IsInteger(arg1)) {
			Unref(wnd, cmd, arg1, arg2);
			return interp_error("WndMessage() send_msg");
		}
#ifdef	_WINDOWS
		WPARAM wParam = 0; LPARAM lParam = 0;
		if (arg2 && IsList(arg2) && arg2->T
				&& IsInteger(arg2->H) && IsInteger(arg2->TH)) {
			wParam = (WPARAM)arg2->HI; lParam = (LPARAM)arg2->THI;
		}
		val = Ref(Integer(::SendMessage((HWND)wnd->a.i.v, (UINT)arg1->a.i.v, wParam, lParam)));
#endif
#ifdef	_LINUX
		ADvCMainFrame *frw = (ADvCMainFrame*)ADvGetApp()->m_pMainWnd;
		long wParam = 0; long lParam = 0;
		if (arg2 && IsList(arg2) && arg2->T
				&& IsInteger(arg2->H) && IsInteger(arg2->TH)) {
			wParam = arg2->HI; lParam = arg2->THI;
		}
		ADvMessage *msg = new ADvMessage(
			wnd->a.i.v, arg1->a.i.v, wParam, lParam);
		val = frw->ProcessMsg(msg);
		delete msg;
#endif
	} else if (strcmp(s, "post_msg") == 0) {
		if (!IsInteger(wnd) || !IsInteger(arg1)) {
			Unref(wnd, cmd, arg1, arg2);
			return interp_error("WndMessage() post_msg");
		}
#ifdef	_WINDOWS
		WPARAM wParam = 0; LPARAM lParam = 0;
		if (arg2 && IsList(arg2) && arg2->T
				&& IsInteger(arg2->H) && IsInteger(arg2->TH)) {
			wParam = (WPARAM)arg2->HI; lParam = (LPARAM)arg2->THI;
		}
		if (::PostMessage((HWND)wnd->a.i.v, (UINT)arg1->a.i.v, wParam, lParam)) {
		} else {
			val = Ref(Integer(1));
		}
#endif
#ifdef	_LINUX
		long wParam = 0; long lParam = 0;
		if (arg2 && IsList(arg2) && arg2->T
				&& IsInteger(arg2->H) && IsInteger(arg2->TH)) {
			wParam = arg2->HI; lParam = arg2->THI;
		}
		ADvPostMessage(wnd->a.i.v, arg1->a.i.v, wParam, lParam);
#endif

	} else if (strcmp(s, "reply") == 0) {
#ifdef	_WINDOWS
		if (IsInteger(arg1) && ::InSendMessage()) {
			if (::ReplyMessage((LRESULT)arg1->a.i.v)) {
				val = Ref(Integer(1));
			}
		}
#endif
#ifdef	_LINUX
		val = Ref(Integer(1));
#endif

	} else if (strcmp(s, "wParam") == 0) {
		ADvCWinApp *app = ADvGetApp();
		if (app)
			val = Ref( ((ADvCMainFrame*)app->m_pMainWnd)->wParam() );
	} else if (strcmp(s, "lParam") == 0) {
		ADvCWinApp *app = ADvGetApp();
		if (app)
			val = Ref( ((ADvCMainFrame*)app->m_pMainWnd)->lParam() );

	} else if (strcmp(s, "do_events") == 0) {
		AIpProcessMgr *pm = AIpProcessMgr::Instance();
		ASSERT(pm);
		pm->AddLocalLoopProcess(this);
		ADvDoEvents();
		pm->RemoveLocalLoopProcess(this);

	} else if (strcmp(s, "create_map") == 0) {
#ifdef	_WINDOWS
		if (!IsString(arg1) || !IsString(arg2)) {
			Unref(wnd, cmd, arg1, arg2);
			return interp_error("WndMessage() create_map");
		}
		int size = arg2->a.s.v->Length() + 1;
		SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
		HANDLE hmap = ::CreateFileMapping(
			(HANDLE)0xffffffff, &sa, PAGE_READWRITE,
			0, size, arg1->a.s.v->Str());
		if (hmap) {
			val = Ref(Integer((long)hmap));
			char *cp = (char*)::MapViewOfFile(hmap, FILE_MAP_WRITE, 0, 0, 0);
			if (cp) {
				memcpy(cp, arg2->a.s.v->Str(), size);
				UnmapViewOfFile(cp);
				val = Ref(Integer((long)hmap));
			} else {
				::CloseHandle(hmap); hmap = NULL;
			}
		}
#endif
#ifdef	_LINUX
		if (!IsString(arg2)) {
			Unref(wnd, cmd, arg1, arg2);
			return interp_error("WndMessage() create_map");
		}
		int size = arg2->a.s.v->Length() + 1;
		int id = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
		if (id != -1) {
			char *cp = (char*)shmat(id, 0, 0);
			if ((long)cp != -1) {
				memcpy(cp, arg2->a.s.v->Str(), size);
				int r = shmdt(cp);
				if (r == -1) {
					char ss[256];
					sprintf(ss, "create_map: shmdt error\n");
					ADvWindow::Print(ss);	
				}
				val = Ref(Integer((long)id));
			} else {
				int r = shmctl(id, IPC_RMID, 0);
				if (r == -1) {
					char ss[256];
					sprintf(ss, "create_map: shmctrl error\n");
					ADvWindow::Print(ss);	
				}
			}
		}
#endif
	} else if (strcmp(s, "get_map") == 0) {
#ifdef	_WINDOWS
		if (!IsString(arg1)) {
			Unref(wnd, cmd, arg1, arg2);
			return interp_error("WndMessage() get_map");
		}
		HANDLE hmap = ::OpenFileMapping(
			FILE_MAP_READ, FALSE, arg1->a.s.v->Str());
		if (hmap) {
			char *cp = (char*)::MapViewOfFile(hmap, FILE_MAP_READ, 0, 0, 0);
			if (cp) {
				val = Ref(String(cp));
				UnmapViewOfFile(cp);
			}
			::CloseHandle(hmap);
		}
#endif
#ifdef	_LINUX
		if (!IsInteger(arg1)) {
			Unref(wnd, cmd, arg1, arg2);
			return interp_error("WndMessage() get_map");
		}
		int id = arg1->a.i.v;
		char *cp = (char*)shmat(id, 0, 0);
		if ((long)cp != -1) {
			val = Ref(String(cp));
			int r = shmdt(cp);
			if (r == -1) {
				char ss[256];
				sprintf(ss, "get_map: shmdt error\n");
				ADvWindow::Print(ss);	
			}
		}
#endif
	} else if (strcmp(s, "close_map") == 0) {
		if (!IsInteger(arg1)) {
			Unref(wnd, cmd, arg1, arg2);
			return interp_error("WndMessage() close_map");
		}
#ifdef	_WINDOWS
		HANDLE hmap = (HGLOBAL)arg1->a.i.v;
		::CloseHandle(hmap);
#endif
#ifdef	_LINUX
		int id = arg1->a.i.v;
		int r = shmctl(id, IPC_RMID, 0);
		if (r == -1) {
			Unref(wnd, cmd, arg1, arg2);
			return interp_error("WndMessage() close_map");
		}
#endif

	} else if (strcmp(s, "get_wnd_text") == 0) {
		if (!IsInteger(wnd)) {
			Unref(wnd, cmd, arg1, arg2);
			return interp_error("WndMessage() get_wnd_text");
		}
		val = Ref(Cons(NULL));
#ifdef	_WINDOWS
		char buff[1024];
		for (HWND hWnd = ::GetTopWindow((HWND)wnd->a.i.v);
				hWnd;
				hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT)) {
			int result = ::GetWindowText(hWnd, buff, 1024);
			if (result == 0) buff[0] = 0;
			CreateArc(val, Integer((long)hWnd), String(buff));
		}
#endif
#ifdef	_LINUX
	// TODO_LINUX get_wnd_text
#endif

	} else {
		Unref(wnd, cmd, arg1, arg2);
		return interp_error("WndMessage()");
	}
	Unref(wnd, cmd, arg1, arg2);
	return 0;
}

int AIpProcess::EvalScript() {
	AGmNode *cmd, *script, *arg;
	GetArg(cmd, script, arg);
	if (!IsString(cmd) || !IsString(script)) {
		Unref(cmd, script, arg);
		return interp_error("Script()");
	}
	const char *s = cmd->a.s.v->Str();
	Unref(val); val = NULL;
	if (strcmp(s, "trans") == 0) {
		int check = (IsInteger(arg) && arg->a.i.v == 1);
		char line[512];
		AGmNode *cn = NULL;
		AGmString *s1 = script->a.s.v, *s2 = NULL; AGmFile *f = NULL;
		AUiMemberShell *ms = NULL; AUiBodyShell *bs = NULL;
		for (;;) {
			cn = dst_node2(script, NmO());
			if (cn) break;
			cn = Cons(String("AltairScript"));
			f = new AGmFile();
			if (f->Create(s1, AGM_FILE_STR_READ)) {
				val = Ref(Integer(1)); break;
			}
			ms = new AUiMemberShell(
				(ADvWindow*)NULL, RootClass(), cn);
			f->ReadSpace();
			if (f->ReadLine(s2)) {
				val = Ref(Integer(2)); break;
			}
			if (check) {
				art_sprintf_1(line, 512, "%s\n", s2->Str());
				ADvWindow::Print(line);
			}
			if (ms->InsertLine(s2->Str())) {
				val = Ref(Integer(3)); break;
			}
			delete s2;
			ms->Reset();
			f->ReadSpace();
			if (f->ReadLine(s2)) {
				val = Ref(Integer(4)); break;
			}
			if (check) {
				art_sprintf_1(line, 512, "%s\n", s2->Str());
				ADvWindow::Print(line);
			}
			if (s2->Compare("body")) {
				val = Ref(Integer(5)); break;
			}
			delete s2;
			bs = new AUiBodyShell(
				(ADvWindow*)NULL, RootClass(), ms->Class(), ms->Member());
			int err = 1;
			for (bs->Reset();;) {
				f->ReadSpace();
				if (f->ReadLine(s2)) {
					val = Ref(Integer(6)); break;
				}
				if (check) {
					art_sprintf_1(line, 512, "%s\n", s2->Str());
					ADvWindow::Print(line);
				}
				if (bs->InsertLine(s2->Str()) == 0) {
					delete s2; continue;
				}
				if (s2->Compare("end_body") != 0) {
					val = Ref(Integer(7)); break;
				}
				err = 0; break;
			}
			if (err) break;
			AGmArc *a = CreateArc(script, cn, NmO());
			a->incl_assoc |= AGM_ARC_ASSOC;
		}
		if (s2) delete s2;
		if (ms) delete ms;
		if (bs) delete bs;
		if (f) delete f;
		RefUnref(cn);
	} else if (strcmp(s, "eval") == 0) {
		EvalScript(script, arg);
	} else if (strcmp(s, "eval2") == 0) {
		for (;;) {
			AGmNode *cn = dst_node2(script, NmO());
			if (cn == NULL) break;
			AGmNode *func = dst_node(cn);
			ASSERT(func);

			Push(func_stack,
				List(this_ptr, prog,
				arg_base, loc_base, ctrl_stack, func));
			this_ptr = NULL;
			prog = func->TTH;
			arg_base = arg;
			loc_base = ctrl_stack = NULL;
			Push(this_ptr_ls, this_ptr);
			Push(arg_base_ls, arg_base);
			Push(loc_base_ls, loc_base);
			Push(ctrl_stack_ls, ctrl_stack);
			break;
		}
	} else {
		Unref(cmd, script, arg);
		return interp_error("Script()");
	}
	Unref(cmd, script, arg);
	return 0;
}
void AIpProcess::EvalScript(AGmNode* script, AGmNode* arg) {
	for (;;) {
		AGmNode *cn = dst_node2(script, NmO());
		if (cn == NULL) break;
		AGmNode *func = dst_node(cn);
		ASSERT(func);

		AGmNode *a = NULL;
		if (IsList(arg)) a = arg;
		AGmNode *arg_dcl = func->TTTH;
		AGmNode *l, *vars = NULL;
		for (l = arg_dcl; l; l = l->TT) {
			vars = Cons(List(l->H, (a?a->H:NULL), l->TH), vars);
			if (a) a = a->T; if (!IsList(a)) a = NULL;
		}
#if 1
		AGmNode *prev_func_stack = func_stack;
#endif
		Push(func_stack,
			List(this_ptr, prog,
			arg_base, loc_base, ctrl_stack, func));
		this_ptr = NULL;
		prog = func->TTH;
		arg_base = vars;
		loc_base = ctrl_stack = NULL;
		Push(this_ptr_ls, this_ptr);
		Push(arg_base_ls, arg_base);
		Push(loc_base_ls, loc_base);
		Push(ctrl_stack_ls, ctrl_stack);
#if 1
		while (func_stack && func_stack != prev_func_stack) {
			Execute();
		}
#endif
		break;
	}
}

static char *time_format(char *buf, int buf_len, long value, int num_digit) {
	char *cp = buf + buf_len;
	*--cp = 0;
	int i;
	for (i = 0; i < num_digit; i++) {
		*--cp = (char)('0' + (value % 10));
		value /= 10;
	}
	return cp;
}

static int init_rand = 0;

int AIpProcess::EvalMisc() {
	AGmNode *cmd, *arg1, *arg2;
	GetArg(cmd, arg1, arg2);
	if (!IsString(cmd)) {
		Unref(cmd, arg1, arg2);
		return interp_error("Misc()");
	}
	Unref(val); val = NULL;
	const char *s = cmd->a.s.v->Str();
	if (strcmp(s, "binary") == 0) {
		if (IsInteger(arg1) && arg1->a.i.v >= 0) {
			val = Ref(Binary(new AGmBinary((long)arg1->a.i.v)));
		}
	} else if (strcmp(s, "binary_size") == 0) {
		if (IsBinary(arg1)) {
			val = Ref(Integer(arg1->a.bin.v->Length()));
		}
	} else if (strcmp(s, "extend_binary") == 0) {
		if (IsBinary(arg1) && IsInteger(arg2) && arg2->a.i.v >= 0) {
			arg1->a.bin.v->Expand((long)arg2->a.i.v);
			val = Ref(arg1);
		}
	} else if (strcmp(s, "binary_copy") == 0) {
		long err = 1;
		AGmBinary *dst_b, *src_b; AGmNode *tail;
#if	_WINDOWS && _64BIT
		long long dst_idx, src_idx, src_size;
#else
		long dst_idx, src_idx, src_size;
#endif
		if (Param(arg1, dst_b, tail) == 0
				&& Param(tail, dst_idx, tail) == 0
				&& Param(arg2, src_b, tail) == 0
				&& Param(tail, src_idx, src_size, tail) == 0) {
			if (src_size >= 0
					&& dst_idx >= 0
					&& dst_idx + src_size <= dst_b->Length()
					&& src_idx >= 0
					&& src_idx + src_size <= src_b->Length()) {
				err = 0;
				dst_b->Copy((long)dst_idx, src_b, (long)src_idx, (long)src_size);
			}
		} else if (Param(arg1, dst_b, tail) == 0
				&& Param(tail, dst_idx, tail) == 0
				&& IsString(arg2)) {
			if (dst_idx >= 0
					&& dst_idx + arg2->a.s.v->Length() <= dst_b->Length()) {
				err = 0;
				dst_b->Copy((long)dst_idx, arg2->a.s.v);
			}
		}
		if (err) {
			val = Ref(Integer(err));
		}
	} else if (strcmp(s, "binary_to_string") == 0) {
		long err = 1;
		AGmBinary *src_b; AGmNode *tail;
#if	_WINDOWS && _64BIT
		long long src_idx, src_size;
#else
		long src_idx, src_size;
#endif
		if (Param(arg1, src_b, tail) == 0
				&& Param(tail, src_idx, src_size, tail) == 0) {
			if (src_size >= 0
					&& src_idx >= 0
					&& src_idx + src_size <= src_b->Length()) {
				err = 0;
				val = Ref(String(src_b->toString((long)src_idx, (long)src_size)));
			}
		}
		if (err) {
			val = Ref(Integer(err));
		}
	} else if (strcmp(s, "binary_search_str") == 0) {
		AGmBinary *src_b; AGmNode *tail;
#if	_WINDOWS && _64BIT
		long long src_idx;
#else
		long src_idx;
#endif
		if (Param(arg1, src_b, tail) == 0
				&& Param(tail, src_idx, tail) == 0) {
			if (src_idx < 0) src_idx = 0;
			if (IsString(arg2)) {
				val = Ref(Integer(src_b->Search((long)src_idx, arg2->a.s.v->Str())));
			} else if (IsInteger(arg2)) {
				val = Ref(Integer(src_b->Search((unsigned char)arg2->a.i.v, (long)src_idx)));
			}
		}
	} else if (strcmp(s, "get_byte") == 0) {
		AGmBinary *b; AGmNode *tail;
#if	_WINDOWS && _64BIT
		long long idx;
#else
		long idx;
#endif
		if (Param(arg1, b, tail) == 0
				&& Param(tail, idx, tail) == 0) {
			if (idx >= 0 && idx < b->Length()) {
				val = Ref(Integer((long)b->Component((long)idx)));
			}
		}
	} else if (strcmp(s, "put_byte") == 0) {
		long err = 1;
		AGmBinary *b; AGmNode *tail;
#if	_WINDOWS && _64BIT
		long long idx;
#else
		long idx;
#endif
		if (Param(arg1, b, tail) == 0
				&& Param(tail, idx, tail) == 0
				&& IsInteger(arg2)) {
			if (idx >= 0 && idx < b->Length()) {
				err = 0;
				b->Component((long)idx) = (unsigned char)arg2->a.i.v;
			}
		}
		if (err) {
			val = Ref(Integer(err));
		}

	} else if (strcmp(s, "integer_size") == 0) {
		AGmNode *n = BigInteger::ToBigInt(arg1);
		if (n) {
			val = Ref(Integer(n->a.bi.sign ? n->a.bi.bin->Length() : 0));
			RefUnref(n);
		}

	} else if (strcmp(s, "gcd") == 0) {
		val = Ref(BigInteger::Gcd(arg1, arg2));

	} else if (strcmp(s, "random") == 0) {
		if (init_rand == 0) {
			init_rand = 1;
			srand((unsigned int)time(NULL));
		}
		val = Ref(Integer((long)rand()));

	} else if (strcmp(s, "from_clipboard") == 0) {
#ifdef	_WINDOWS
		CWnd *pWnd = AfxGetApp()->m_pMainWnd;
		if (pWnd && pWnd->m_hWnd) {
			if (::OpenClipboard(pWnd->m_hWnd)) {
				HGLOBAL hData = ::GetClipboardData(CF_TEXT);
				if (hData) {
					char *str1 = (char*)::GlobalLock(hData);
					char *str2 = ADvCEdit::fromCEditString(str1);
					val = Ref(String(str2));
					delete [] str2;
					::GlobalUnlock(hData);
				}
				::CloseClipboard();
			}
		}
#endif
#ifdef	_LINUX
		GtkClipboard *clipboard = gtk_clipboard_get_for_display(gdk_display_get_default(), GDK_SELECTION_CLIPBOARD);
		if (clipboard && gtk_clipboard_wait_is_text_available(clipboard)) {
			gchar *c = gtk_clipboard_wait_for_text(clipboard);
			if (c) {
				gsize br, bw; GError *err = NULL;
				gchar *c2 = g_locale_from_utf8(c, -1, &br, &bw, &err);
				g_free(c);
				if (err == NULL) {
					val = Ref(String(c2));
					g_free(c2);
				} else {
					g_error_free(err);
				}
			}
		}
#endif
	} else if (strcmp(s, "to_clipboard") == 0) {
		if (!IsString(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("Misc() to_clipboard");
		}
#ifdef	_WINDOWS
		char *str = ADvCEdit::toCEditString(arg1->a.s.v->Str());
		int err = 0;
		CWnd *pWnd = AfxGetApp()->m_pMainWnd;
		if (pWnd && pWnd->m_hWnd) {
			if (::OpenClipboard(pWnd->m_hWnd)) {
				if (::EmptyClipboard()) {
					int len = (int)strlen(str) + 1;
					HGLOBAL hData = ::GlobalAlloc(GMEM_MOVEABLE, len);
					if (hData) {
						char *cp = (char*)::GlobalLock(hData);
						art_strcpy(cp, len, str);
						::SetClipboardData(CF_TEXT, hData);
						::GlobalUnlock(hData);
					} else {
						err = 3;
					}
				} else {
					err = 2;
				}
				::CloseClipboard();
			} else {
				err = 1;
			}
		}
		delete [] str;
		if (err)
			val = Ref(Integer((long)err));
#endif
#ifdef	_LINUX
		GtkClipboard *clipboard = gtk_clipboard_get_for_display(gdk_display_get_default(), GDK_SELECTION_CLIPBOARD);
		if (clipboard) {
			gsize br, bw; GError *err = NULL;
			gchar *c = g_locale_to_utf8(arg1->a.s.v->Str(), -1, &br, &bw, &err);
			if (err == NULL) {
				gtk_clipboard_set_text(clipboard, c, bw);
				g_free(c);
			} else {
				g_error_free(err);
			}
		}
#endif

	} else if (strcmp(s, "get_env") == 0) {
		if (IsString(arg1)) {
			char *str = art_getenv(arg1->a.s.v->Str());
			if (str)
				val = Ref(String(str));
		}
	} else if (strcmp(s, "set_env") == 0) {
		if (IsString(arg1) && IsString(arg2)) {
#ifdef _WINDOWS
			AGmString *str = new AGmString(arg1->a.s.v->Str());
			str->Append("=");
			str->Append(arg2->a.s.v->Str());
			if (art_putenv((char*)str->Str()) == 0)
				val = Ref(Integer(1));
			delete str;
#endif
#ifdef _LINUX
			if (setenv(arg1->a.s.v->Str(), arg2->a.s.v->Str(), 1) == 0)
				val = Ref(Integer(1));
#endif
		}
	} else if (strcmp(s, "search_env") == 0) {
		if (IsString(arg1) && IsString(arg2)) {
			char *str = art_getenv(arg2->a.s.v->Str());
			if (str) {
				AGmString *s = new AGmString(str);
				char path[MAX_PATH];
#ifdef _WINDOWS
				AGmNode *ls = Ref(split_str(s, ';')); AGmLink *l;
				for (l = next_outlink(ls, NULL); l; l = next_outlink(ls, l)) {
					art_sprintf_2(path, MAX_PATH, "%s\\%s", l->arc->dst->a.s.v->Str(), arg1->a.s.v->Str());
#endif
#ifdef _LINUX
				AGmNode *ls = Ref(split_str(s, ':')); AGmLink *l;
				for (l = next_outlink(ls, NULL); l; l = next_outlink(ls, l)) {
					sprintf(path, "%s/%s", l->arc->dst->a.s.v->Str(), arg1->a.s.v->Str());
#endif
					if (AGmFile::DoesExist(path)) {
						val = Ref(String(path));
						break;
					}
				}
				delete s;
			}
		}
	} else if (strcmp(s, "get_cmd_line") == 0) {
		if (ADvCmdLine)
			val = Ref(ADvCmdLine);
	} else if (strcmp(s, "exit") == 0) {
		if (IsInteger(arg1)) {
			ADvExitCode = (int)arg1->a.i.v;
			AUiProgramMgr::Exit();
		} else {
			Unref(cmd, arg1, arg2);
			return interp_error("Misc() exit");
		}

	} else if (strcmp(s, "sleep") == 0) {
		if (IsInteger(arg1)) {
#ifdef	_WINDOWS
			::Sleep((DWORD)arg1->a.i.v);
#endif
#ifdef	_LINUX
			usleep(arg1->a.i.v * 1000);
#endif
		}

	} else if (strcmp(s, "obj_to_addr") == 0) {
		if (arg1) {
			val = Ref(Integer((long)arg1));
		}
	} else if (strcmp(s, "addr_to_obj") == 0) {
		if (!IsInteger(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("Misc() addr_to_obj");
		}
		val = Ref((AGmNode*)arg1->a.i.v);

	} else if (strcmp(s, "sort") == 0) {
		AGmNode *root = NULL;
		if (arg1
				&& arg2 && IsList(arg2) && IsInteger(arg2->H)
				&& arg2->T && IsList(arg2->T) && IsInteger(arg2->TH)) {
			AGmLink *l;
			for (l = next_outlink(arg1, NULL);
					l; l = next_outlink(arg1, l)) {
				QuickSort(root, l->arc->dst, (int)arg2->H->a.i.v, l->arc->attr);
			}
			val = Ref(Cons(NULL));
			QuickSortResult(val, root, (int)arg2->TH->a.i.v);
		}
		Unref(root);

	} else if (strcmp(s, "para_lock") == 0) {
		if (arg1) {
			ParaLock(1);
		} else {
			ParaLock(0);
		}

	} else if (strcmp(s, "get_time") == 0) {
		val = Ref(Integer((long)get_time()));
	} else if (strcmp(s, "get_localtime") == 0) {
		if (IsInteger(arg1)) {
			long yy1, mm1, dd1, h1, m1, s1, wday;
			get_localtime(arg1->a.i.v, yy1, mm1, dd1, h1, m1, s1, wday);
			val = Ref(List(yy1, mm1, dd1, h1, m1, s1));
		}
	} else if (strcmp(s, "get_gmtime") == 0) {
		if (IsInteger(arg1)) {
			long yy1, mm1, dd1, h1, m1, s1, wday;
			get_gmtime(arg1->a.i.v, yy1, mm1, dd1, h1, m1, s1, wday);
			val = Ref(List(yy1, mm1, dd1, h1, m1, s1));
		}
	} else if (strcmp(s, "get_mktime") == 0) {
#if	_WINDOWS && _64BIT
		long long yy1, mm1, dd1, h1, m1, s1; AGmNode *tail;
#else
		long yy1, mm1, dd1, h1, m1, s1; AGmNode *tail;
#endif
		if (Param(arg1, yy1, mm1, dd1, tail) == 0 && Param(tail, h1, m1, s1, tail) == 0) {
			val = Ref(Integer((long)get_mktime((long)yy1, (long)mm1, (long)dd1, (long)h1, (long)m1, (long)s1)));
		}
		if (Param(arg1, yy1, mm1, dd1, tail) == 0 && Param(tail, h1, m1, s1, tail) == 0) {
			val = Ref(Integer((long)get_mktime((long)yy1, (long)mm1, (long)dd1, (long)h1, (long)m1, (long)s1)));
		}
	} else if (strcmp(s, "format_time") == 0) {
#if	_WINDOWS && _64BIT
		long long yy1, mm1, dd1, h1, m1, s1; AGmNode *tail;
#else
		long yy1, mm1, dd1, h1, m1, s1; AGmNode *tail;
#endif
		if (Param(arg1, yy1, mm1, dd1, tail) == 0 && Param(tail, h1, m1, s1, tail) == 0
				&& IsString(arg2)) {
			AGmFile *f = new AGmFile();
			f->Create(arg2->a.s.v, AGM_FILE_STR_READ);
			AGmString *text = new AGmString(16), *s;
			char buf[64], *cp;
			for (;;) {
				if (f->IsEnd()) {
					val = Ref(String(text));
					break;
				} else if (f->MatchString("yyyy")) {
					cp = time_format(buf, 64, (long)yy1, 4);
					text->Append(cp);
				} else if (f->MatchString("yy")) {
					cp = time_format(buf, 64, (long)yy1, 2);
					text->Append(cp);
				} else if (f->MatchString("MM")) {
					cp = time_format(buf, 64, (long)mm1, 2);
					text->Append(cp);
				} else if (f->MatchString("dd")) {
					cp = time_format(buf, 64, (long)dd1, 2);
					text->Append(cp);
				} else if (f->MatchString("HH")) {
					cp = time_format(buf, 64, (long)h1, 2);
					text->Append(cp);
				} else if (f->MatchString("mm")) {
					cp = time_format(buf, 64, (long)m1, 2);
					text->Append(cp);
				} else if (f->MatchString("ss")) {
					cp = time_format(buf, 64, (long)s1, 2);
					text->Append(cp);
				} else if (f->ReadSingleQString(s) == 0) {
					text->Append(s->Str());
					delete s;
				} else {
					delete text;	
					break;
				}
			}
			delete f;
		}

	} else if (strcmp(s, "time_stamp") == 0) {
		struct timeb buf;
		ftime(&buf);
		val = Ref(Integer((long)(buf.time - startup_time.time) * 1000 + (long)(buf.millitm - startup_time.millitm)));

	} else if (strcmp(s, "dump_trace") == 0) {
		if (IsInteger(arg1)) {
			AGmDumpTrace = (int)arg1->a.i.v; 
		}
	} else if (strcmp(s, "cell_heap") == 0) {
		if (IsInteger(arg1)) {
			if (MaxHeapNum((long)arg1->a.i.v)) val = Ref(Integer(1));
		} else {
			long max = MaxHeapNum();
			long curr = CurrHeapNum();
			long size = HeapBlockSize();
			char s[256];
			art_sprintf_4(s, 256, "max = %ld (%.3f MB), current = %ld (%.3f MB)",
				max, ((float)(max*size))/(1024*1024), curr, ((float)(curr*size))/(1024*1024));
			val = Ref(String(s));
		}
	} else if (strcmp(s, "stack_trace") == 0) {
		if (IsList(arg1) && arg2 == NULL) {
			StackTrace(arg1);
		}
		if (IsList(arg1) && IsFile(arg2)) {
			StackTrace(arg1, arg2->a.f.v);
		}
	} else if (strcmp(s, "error_source") == 0) {
		if (arg1 && IsList(arg1) && arg2 == NULL) {
			ErrorSource(arg1->H, arg1->TH);
		}
		if (arg1 && IsList(arg1) && arg1->T && IsFile(arg2)) {
			ErrorSource(arg1->H, arg1->TH, arg2->a.f.v);
		}

	} else if (strcmp(s, "platform") == 0) {
#ifdef _WINDOWS
		val = Ref(String("windows"));
#endif
#ifdef _LINUX
#ifdef _MAC
		val = Ref(String("mac"));
#else
		val = Ref(String("linux"));
#endif
#endif

	} else if (strcmp(s, "output") == 0) {
		if (arg1 && IsString(arg1)) {
			if (AGmOutputFile) delete AGmOutputFile;
			AGmOutputFile = new AGmFile();
			if (AGmOutputFile->Create(arg1->a.s.v->Str(), AGM_FILE_WRITE) == 0) {
				val = Ref(Integer(1));
			} else {
				delete AGmOutputFile; AGmOutputFile = NULL;
			}
		}
		if (arg1 == NULL) {
			if (AGmOutputFile) {
				delete AGmOutputFile; AGmOutputFile = NULL;
				val = Ref(Integer(1));
			}
		}
		if (arg2 && IsString(arg2)) {
			const char *s = arg2->a.s.v->Str();
			if (strcmp(s, "no_gui") == 0) {
				AGmOutputGui = 0;
			} else if (strcmp(s, "gui") == 0) {
				AGmOutputGui = 1;
			}
		}
	} else if (strcmp(s, "output_max_size") == 0) {
		if (IsInteger(arg1)) {
			if (arg1->a.i.v >= 1000) AGmOutputFileMaxSize = (int)arg1->a.i.v;
		} else {
			val = Ref(Integer(AGmOutputFileMaxSize));
		}

	} else if (strcmp(s, "version") == 0) {
		val = Ref(String("0.43"));

	} else if (strcmp(s, "getpid") == 0) {
#ifdef	_LINUX
		val = Ref(Integer((long)getpid()));
#endif
	} else if (strcmp(s, "waitpid") == 0) {
#ifdef	_LINUX
		if (IsInteger(arg1)) {
			val = Ref(Integer((long)waitpid((pid_t)arg1->a.i.v, NULL, 0)));
		}
#endif
	} else if (strcmp(s, "does_exit") == 0) {
#ifdef	_LINUX
		if (IsInteger(arg1)) {
			char str[MAX_PATH], c;
			if (AGmFile::DoesProcessExit(arg1->a.i.v, str, &c)) {
				val = Ref(Integer(1));
			}
		}
#endif

	} else {
		Unref(cmd, arg1, arg2);
		return interp_error("Misc()");
	}
	Unref(cmd, arg1, arg2);
	return 0;
}

void QuickSort(AGmNode*& node, AGmNode* value, int level, AGmNode* attr) {
	AGmNode *key;
	if (level <= 0) {
		key = value;
	} else {
		AGmNode *l; int i = level - 1;
		for (l = value; i && l->T; l = l->T, i--) ;
		key = l->H;
	}
	if (node == NULL) {
		node = AGm::Ref(AGm::List(key, value, NULL, NULL, attr));
		return;
	}
	int r = AGm::Compare(key, node->H);
	if (r < 0) {
		QuickSort(node->TTH, value, level, attr);
	} else {
		QuickSort(node->TTTH, value, level, attr);
	}
}
void QuickSortResult(AGmNode *result, AGmNode* node, int rev) {
	if (node == NULL) return;
	if (rev != 1)
		QuickSortResult(result, node->TTH, rev);
	else
		QuickSortResult(result, node->TTTH, rev);
	AGm::CreateArc(result, node->TH, node->TTTTH);
	if (rev != 1)
		QuickSortResult(result, node->TTTH, rev);
	else
		QuickSortResult(result, node->TTH, rev);
}
