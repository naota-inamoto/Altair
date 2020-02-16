/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AGM_FILE_H
#define _AGM_FILE_H

int isIdentHead(const unsigned char);
int isIdentTail(const unsigned char);
int isFirstChar(const unsigned char);
int isNoCaseEqual(const unsigned char, const unsigned char);

#define AGM_FILE_READ		0
#define AGM_FILE_WRITE		1
#define AGM_FILE_APPEND 	2
#define AGM_FILE_STR_READ	3
#define AGM_FILE_STR_WRITE	4
#define AGM_FILE_STR_APPEND 5
#define AGM_FILE_BIN_READ	6
#define AGM_FILE_BIN_WRITE	7
#define AGM_FILE_BIN_APPEND 8

#define AGM_FILE_STDIN		9
#define AGM_FILE_STDOUT 	10
#define AGM_FILE_STDERR 	11
#define AGM_FILE_PIPE		12

#define AGM_FILE_FD_IN		13
#define AGM_FILE_FD_OUT 	14

#define AGM_FILE_LOCK		15

class AGmString;

class AL_EXT_CLASS AGmFile : public AGm {
public:
	AGmFile();
	int Create(const char*, unsigned char type);
	int Create(AGmString*, unsigned char type);
	int Create(int fd, unsigned char type);
	~AGmFile();

	void GCmark();

	int WriteChar(const char);
	int WriteBigInteger(AGmNode*);
	int WriteInteger(AGmNode*);
#if	_WINDOWS && _64BIT
	int WriteInteger(long long);
	int WriteHexa(long long);
#else
	int WriteInteger(long);
	int WriteHexa(long);
#endif
	int WriteReal(double);
	int WriteString(const char*);
	int WriteString(const AGmString*);
	int WriteString(const AGmBinary*);
	int WriteQChar(char);
	int WriteQString(const char*, int b_esc=1);
	int WriteQString(const AGmString*, int b_esc=1);
	int WriteSingleQString(const char*, int b_esc=1);
	int WriteMatrix(const AGmMatrix*);
	int WriteCsv(AGmNode*);

	int WriteList(AGmNode*, int&, int limit=-1);
	int _WriteList(AGmNode*, int&, int limit=-1);

	int ReadSpace();
	AGmString* ReadSpace2();
	AGmString* ReadComment();
	int ReadChar(char&);
#if	_WINDOWS && _64BIT
	int ReadInteger(long long&);
	int _ReadInteger(long long&);
	int ReadHexa(long long&);
#else
	int ReadInteger(long&);
	int _ReadInteger(long&);
	int ReadHexa(long&);
#endif
	int ReadReal(double&);
	int ReadIdent(AGmString*&);
	int ReadJIdent(AGmString*&);
	int ReadXIdent(AGmString*&);
	int ReadString(AGmString*&);
	int ReadLine(AGmString*&);
	int ReadQChar(char&);
	int ReadQString(AGmString*&, int b_esc=1);
	int ReadSingleQString(AGmString*&);
	int _ReadSingleQString(AGmString*&);
	int ReadMatrix(AGmMatrix*&);

	int ReadChar(AGmNode*&);
	int ReadInteger(AGmNode*&);
	int ReadBigInteger(AGmNode*&);
	int ReadHexa(AGmNode*&);
	int ReadReal(AGmNode*&);
	int ReadIdent(AGmNode*&);
	int ReadJIdent(AGmNode*&);
	int ReadXIdent(AGmNode*&);
	int ReadString(AGmNode*&);
	int ReadLine(AGmNode*&);
	int ReadQChar(AGmNode*&);
	int ReadQString(AGmNode*&, int b_esc=1);
	int ReadSingleQString(AGmNode*&);
	int ReadMatrix(AGmNode*&);
	int ReadCsv(AGmNode*&);

	int ReadList(AGmNode*&);
	int _ReadList(AGmNode*&);

	int Search(const char*, AGmString* s=NULL);
	int Search(const char, AGmString* s=NULL);
	int SearchIdent(const char*, AGmString* s=NULL);

	int MatchIdent(const char*, int ignore_case=0);
	int MatchIdent(const AGmString *s, int ignore_case=0);
	int MatchString(const char*, int ignore_case=0);
	int _MatchString(const char*, int ignore_case=0);
	int MatchString(const AGmString *s, int ignore_case=0);
	int IsChar(char);
	int IsEnd();

	static unsigned char HexaToNum(char);

	static int CreateStrReadFile(AGmFile*&, AGmString*&, const char*);
	static void DeleteStrReadFile(AGmFile*, AGmString *s=NULL,
		AGmNode *s1=NULL, AGmNode *s2=NULL, AGmNode *s3=NULL, AGmNode *s4=NULL,
		AGmNode *s5=NULL, AGmNode *s6=NULL, AGmNode *s7=NULL, AGmNode *s8=NULL);

	int WriteGraph(AGmNode*);
	int ReadGraph(AGmNode*&);
	static int WriteGraph(AGmNode*, const char*);
	static int WriteGraph(AGmNode*, AGmString*);
	static int ReadGraph(AGmNode*&, const char*);
	static int ReadGraph(AGmNode*&, AGmString*);

	int WriteTree(AGmNode*);
	int ReadTree(AGmNode*&);
	static int WriteTree(AGmNode*, const char*);
	static int WriteTree(AGmNode*, AGmString*);
	static int ReadTree(AGmNode*&, const char*);
	static int ReadTree(AGmNode*&, AGmString*);

	int ReadBinary(AGmBinary *b, long i, long size);
	int WriteBinary(AGmBinary *b, long i, long size);

	long GetChar();				     // regex (read multi byte one character)
#if	_WINDOWS && _64BIT
	void Back(long long back_point); // back track execution
	long long CurrentPoint();		 // remenber back track position
#else
	long CurrentPoint();		     // remenber back track position
	void Back(long back_point);      // back track execution
#endif
	long LineNumber();			     // file current line number
	void LineNumber(long);

	char Input();
	char _Input();
	void Unput(long p=1);

	int Output(char);
	void Flush();

	int AddChar(long);

	void AutoExpandUnputBuffer(int);
	void Cut();
	long UnputBufferSize();

	void NewlineToken(int);
	int NewlineToken();

	int Redirect(const char*);
	void DebugInputUnput(int);

	AGmString *GetDir();

	int HereDoc(AGmNode*, AGmNode*);

protected:
	int WriteNode(AGmNode*);
	int WriteArc(AGmNode*);
	int WriteTree(int, AGmNode*);

public:
	static int GetDir(AGmString*&);
	static int FindFile(AGmNode*&, const char* path, const char *name);
	static int DoesExist(const char* path); // return 1 if exists, otherwise 0.
	static int IsDir(const char* path); // return 1 if directory, otherwise 0.
	static int ChDir(const char* path);
	static int MkDir(const char* path);
	static int Remove(const char* path);
	static int Rename(const char* old_path, const char* new_path);
	static int Copy(const char* src, const char* dst);

	static int GetStat(const char* path, struct stat *buf);
	static int SetTime(const char* path, struct utimbuf *buf);
	static int ChMod(const char* path, int mode);

	static char* GetTempDir();
	static AGmString* GetTempFile(const char* dir);

	unsigned char Type() { return type; }
	AGmString *Filename() { return filename; }

protected:
	unsigned char type, ok;
	FILE *fp; int fd;
	AGmString *str; char* str_ptr; int num_eof;

	int 	BeginUnputBuffer();
	void	EndUnputBuffer();
	void	ExpandUnputBuffer(int);
	char	*unputBuffer;
	long	point, to, unput_buffer_size, offset;
	int 	auto_expand_unput_buffer;
	long	redirect_size;

	int 	nl_count;
	int 	nl_token;

	long	redirect_pos;
	int 	debug_input_unput;

	AGmString	*filename;

	AGmNode *_doc;
	AGmNode *_delim;

public:
	AGmString	*rbuf1;
	AGmString	*rbuf2;
	long		r_index;

	AGmNode 	*read_type;
	AGmNode 	*root;
	AGmNode 	*value;

	short	wait;
#ifdef	_WINDOWS
	HANDLE	rStdinPipe;
	HANDLE	wStdinPipe;
	HANDLE	rStdoutPipe;
	HANDLE	wStdoutPipe;

	HANDLE	thread;
	HANDLE	stdoutThread;

	HANDLE	retEvent;
	HANDLE	contEvent;

	CRITICAL_SECTION	cs;

#endif
#ifdef	_LINUX
	int		pid;
	int 	stdin_pipe[2];	// parent -> child
	int 	stdout_pipe[2]; // child -> parent

	GThread*	thread;
	GThread*	stdoutThread;

	GCond		retCond;
	GCond		contCond;
	int			retCondFlag;
	int			contCondFlag;

	GMutex		mutex;

	static	void CheckChildProcessExit();
	static	int DoesProcessExit(int pid, char *str, char *c);
#endif
};

#endif	/* _AGM_FILE_H */

