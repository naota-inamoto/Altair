/*
==============================================================================
	iotool.c
		1990/12/14/Fri	Yutaka MYOKI(Nagao Lab., KUEE)
		1990/12/25/Tue	Last Modified
==============================================================================
*/

#include "stdafx.h"

//#ifdef HAVE_CONFIG_H
#include <juman/config.h>
//#endif

#include	<juman/juman.h>

/*
------------------------------------------------------------------------------
	GLOBAL:
	definition of global variable
------------------------------------------------------------------------------
*/

JUMAN_KNP_DATA FILE *Jumanrc_Fileptr = NULL;
extern char 	Jumangram_Dirname[FILENAME_MAX];  /*k.n*/
extern int		LineNoForError, LineNo;

int 	Cha_errno = 0;
FILE	*Cha_stderr = NULL;

/*
------------------------------------------------------------------------------
	FUNCTION:
	<my_fopen>: do "fopen"/<filename> and error processing
------------------------------------------------------------------------------
*/

FILE *my_fopen(const char *filename, const char *mode)
{
	 FILE	*fp;

	 if ((fp = art_fopen(filename, mode)) == NULL)
	  error(OpenError, "can't open", filename, ".", EOA);

	 return fp;
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<pathfopen>: do "fopen"/<filename_path> ( <path> + <filename> )
------------------------------------------------------------------------------
*/

FILE *pathfopen(const char *filename, const char *mode, const char *path, char *filename_path)
{
	 FILE	*fp;

	 art_strcpy(filename_path, FILENAME_MAX, path);
	 art_strcat(filename_path, FILENAME_MAX, filename);

	 return (fp = art_fopen(filename_path, mode));
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<my_pathfopen>: do <pathfopen> and error processing
------------------------------------------------------------------------------
*/

FILE *my_pathfopen(const char *filename, const char *mode, const char *path, char *filename_path)
{
	 FILE	*fp;
		
	 if ((fp = pathfopen(filename, mode, path, filename_path)) == NULL)
	  error(OpenError, "Can't open", filename_path, EOA);

	 return fp;
}
	  
/*
------------------------------------------------------------------------------
	FUNCTION:
	<my_feof>: if <fp> points to "EOF" return <TRUE> else return <FALSE>
------------------------------------------------------------------------------
*/

int my_feof(FILE *fp)
{
	 int	c;

	 if ((c = fgetc(fp)) == EOF) {
	  return TRUE;
	 } else {
	  ungetc(c, fp);
	  return FALSE;
	 }
}

/*
------------------------------------------------------------------------------
	PROCEDURE:
	<append_postfix>: append <affix>  to <filename>
	<change_postfix>: change <affix1> of <filename> to <affix2>
------------------------------------------------------------------------------
*/

void append_postfix(char *filename, char *affix)
{
	 if ((strcmp(&filename[strlen(filename) - strlen(affix)], affix)) &&
	 (endchar(filename) != '.'))
	  art_strcat(filename, FILENAME_MAX, affix);
}

void change_postfix(char *filename, char *affix1, char *affix2)
{
	 if (!strcmp(&filename[strlen(filename) - strlen(affix1)], affix1))
	  filename[strlen(filename) - strlen(affix1)] = '\0';
	 art_strcat(filename, FILENAME_MAX, affix2);
}

/*	
------------------------------------------------------------------------------
	PROCEDURE:
	<getpath>: get <cur_path> and <juman_path>
------------------------------------------------------------------------------
*/

void getpath(char *cur_path, char *juman_path)
{	  
#ifdef _WIN32
	 GetCurrentDirectory(FILENAME_MAX,cur_path);
#else
	 getcwd(cur_path, FILENAME_MAX);
#endif
	 art_strcpy(juman_path, FILENAME_MAX, Jumangram_Dirname);

#ifdef _WIN32
	 if ((endchar(cur_path)) != '\\') art_strcat(cur_path, FILENAME_MAX, "\\");
	 if ((endchar(juman_path)) != '\\') art_strcat(juman_path, FILENAME_MAX, "\\");
#else
	 if ((endchar(cur_path)) != '/') strcat(cur_path, "/");
	 if ((endchar(juman_path)) != '/') strcat(juman_path, "/");
#endif
}

/*
------------------------------------------------------------------------------
	PROCEDURE:
	<my_alloc>: do "malloc" (library function) and error processing
------------------------------------------------------------------------------
*/

void *my_alloc(int n)
{
	 void *p;

	 if ((p = (void *)malloc(n)) == NULL)
	  error(AllocateError, "Not enough memory. Can't allocate.", EOA);

	 return p;
}

/*
------------------------------------------------------------------------------
	PROCEDURE:
	<my_realloc>: do "realloc" (library function) and error processing
------------------------------------------------------------------------------
*/

void *my_realloc(void *ptr, int n)
{
	 void *p;

	 if ((p = (void *)realloc(ptr, n)) == NULL)
	  error(AllocateError, "Not enough memory. Can't allocate.", EOA);

	 return p;
}

/*
------------------------------------------------------------------------------
	PROCEDURE
	<my_exit>: print error-number on "stderr", and "exit"
------------------------------------------------------------------------------
*/

void my_exit(int exit_code)
{
	 fprintf(stderr, "exit(%d)\n", exit_code);
	 exit(exit_code);
}

/*
------------------------------------------------------------------------------
	PROCEDURE
	<error>: print error-message(s) on "stderr", and "exit"
------------------------------------------------------------------------------
*/

void error(int err_no, const char *msg, ...)
{
	 char *str;
	 va_list ap;

	 if (Cha_stderr == NULL)
	 Cha_stderr = stderr;

	 fputs( (Cha_stderr != stderr)? "600 " : "\n", Cha_stderr );
	 fprintf(Cha_stderr, "%s: %s ", ProgName, msg);
	 va_start(ap, msg);

	 while ((str = va_arg(ap, char *)) != EOA)
	 fprintf(Cha_stderr, "%s ", str);
	 fputc('\n', Cha_stderr);	

	 va_end(ap);

	 my_exit(err_no);
}

/*
------------------------------------------------------------------------------
	PROCEDURE
	<warning>: print warning-message(s) on "stderr"
------------------------------------------------------------------------------
*/

void warning(int /*errno*/, char *msg, ...)
{
	 char *str;
	 va_list ap;

	 fprintf(stderr, "\n%s: %s ", ProgName, msg);
	 va_start(ap, msg);

	 while ((str = va_arg(ap, char *)) != EOA)
	  fprintf(stderr, "%s ", str);
	 fputc('\n', stderr);	

	 va_end(ap);
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<lower>: if <char:c> is a large character, lower <c>
------------------------------------------------------------------------------
*/

char lower(char c)
{
	 if (isupper(c))
	  return (char)tolower(c);
	 else
	  return c;
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<upper>: if <char:c> is a small character, upper <c>
------------------------------------------------------------------------------
*/

char upper(char c)
{
	 if (islower(c))
	  return (char)toupper(c);
	 else
	  return c;
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<my_strlen>: return length of the string which is pointed to by <s>.
				 if <s> == NULL return 0
------------------------------------------------------------------------------
*/

int my_strlen(U_CHAR *s)
{
	 int	n = 0;

	 if (s != NULL) 
	  while (s[n])	n++;

	 return n;
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<my_strcmp>: 
------------------------------------------------------------------------------
*/

int my_strcmp(U_CHAR *s1, U_CHAR *s2)
{
	 if (s1 == NULL && s2 == NULL) return 0;

	 if (s1 == NULL) return -1;
	 if (s2 == NULL) return  1;

	 return strcmp((const char*)s1, (const char*)s2);
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<compare_top_str>: if <s1> = <s2...> or <s2> = <s1...> return TRUE
------------------------------------------------------------------------------
*/

int compare_top_str(U_CHAR *s1, U_CHAR *s2)
{
	 int	i = 0;

	 while (s1[i] && s2[i]) {
	  if (s1[i] != s2[i]) return FALSE;
	  i++;
	 }
	 return TRUE;
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<compare_top_str1>: if <s1> = <s2...> return TRUE
------------------------------------------------------------------------------
*/

int compare_top_str1(U_CHAR *s1, U_CHAR *s2)
{
	 int	l1, l2;

	 l1 = (int)strlen((const char*)s1);
	 l2 = (int)strlen((const char*)s2);

	 if (l1 > l2) return FALSE;

	 while (l1--)
	  if (s1[l1] != s2[l1]) return FALSE;

	 return TRUE;
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<compare_top_str2>: if <s1...> = <s2> return TRUE
------------------------------------------------------------------------------
*/

int compare_top_str2(U_CHAR *s1, U_CHAR *s2)
{
	 int	l1, l2;

	 l1 = (int)strlen((const char*)s1);
	 l2 = (int)strlen((const char*)s2);

	 if (l1 < l2) return FALSE;

	 while (l2--)
	  if (s1[l2] != s2[l2]) return FALSE;

	 return TRUE;
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<compare_end_str>: if <s1> = <...s2> or <s2> = <...s1> return TRUE
------------------------------------------------------------------------------
*/

int compare_end_str(U_CHAR *s1, U_CHAR *s2)
{
	 int	l1, l2;

	 l1 = (int)strlen((const char*)s1);
	 l2 = (int)strlen((const char*)s2);

	 if (l1 >= l2) {
	  if (strcmp((const char*)(s1 + l1 - l2), (const char*)s2) == 0)
		   return TRUE;
	  else
		   return FALSE;
	 } else {
	  if (strcmp((const char*)(s2 + l2 - l1), (const char*)s1) == 0)
		   return TRUE;
	  else
		   return FALSE;
	 }
}

/*
------------------------------------------------------------------------------
	PROCEDURE:
	<ls>: print <char *:p> file-information on <FILE *:fp>
------------------------------------------------------------------------------
*/

void ls(FILE *fp, char *p, char *f)
{
	 char	path[FILENAME_MAX];
	 STAT	stbuf;
		
	 art_strcpy(path, FILENAME_MAX, p);
	 art_strcat(path, FILENAME_MAX, f);
	 stat(path, &stbuf);

	 fprintf(fp, "%8lld bytes: %s\n", stbuf.st_size, path);
}

/*
------------------------------------------------------------------------------
	PROCEDURE:
	<print_current_time>: print current local-time on <FILE *:fp>
------------------------------------------------------------------------------
*/

void print_current_time(FILE *fp)
{
	 time_t t;
	 struct tm	*tp;

	 time(&t); tp = art_localtime(&t);
	 fprintf(fp, (const char*)art_asctime(tp));
}

/*
------------------------------------------------------------------------------
	PROCEDURE:
	<print_execute_time>: print two kinds of execution time on <FILE *:fp>
------------------------------------------------------------------------------
*/

void print_execute_time(FILE *fp, int dt, float dp)
{
	 dp = (float)(dp/1000000.0);
	 fprintf(fp, "execution time: %8.3fs\n", (float)dt);
	 fprintf(fp, "processor time: %8.3fs\n", dp);
}

#ifndef JUMAN_KNP
/*
------------------------------------------------------------------------------
		PROCEDURE:
		<set_jumanrc_fileptr>: set Jumanrc_Fileptr

	WIN32 用に juman.ini を見に行くように変更
	RC_DEFAULTがない場合にexitするかどうかをflagで制御するように変更  (2002/11/08)
------------------------------------------------------------------------------
*/
void set_jumanrc_fileptr(char *option_rcfile, int look_rcdefault_p, int exit_rc_notfound_p)
{
	/*
	  rcfileをさがす順

	  <makeint, makemat>
		$HOME/.jumanrc
	→ rcfileがなくてもよい

	  <juman server, standalone>	
		-r オプション
	$HOME/.jumanrc				  _WIN32 の場合は探す必要はない (Changed by Taku Kudoh)
		c:\(winnt|windows)\juman.ini  _WIN32 の場合juman.ini を探す (Changed by Taku Kudoh)
	RC_DEFAULT (Makefile)			
	→ rcfileがなければエラー

	  <juman client>
		-r オプション
	$HOME/.jumanrc
	→ rcfileがなくてもよい
	*/

	char filename[FILENAME_MAX];
#ifdef _LINUX
	char *user_home_ptr;
#endif

	if (option_rcfile) {
	if ((Jumanrc_Fileptr = art_fopen(option_rcfile, "r")) == NULL) {
		fprintf(stderr, "not found <%s>.\n", option_rcfile);
		exit(0);
	}
	} else {
#ifdef _WIN32
	GetPrivateProfileString("juman","dicfile","C:\\juman\\dic",filename,sizeof(filename),"juman.ini");
	if ((endchar(filename)) != '\\') art_strcat(filename, FILENAME_MAX, "\\");
	art_strcat(filename, FILENAME_MAX, "jumanrc");
#else
	if((user_home_ptr = (char *)getenv("HOME")) == NULL)
		/* error(ConfigError, "please set <environment variable> HOME.", EOA); */
		filename[0] = '\0';
	else
		sprintf(filename, "%s/.jumanrc" , user_home_ptr);
#endif
	if (filename[0] == '\0' || (Jumanrc_Fileptr = art_fopen(filename, "r")) == NULL) {
		if (look_rcdefault_p) {
#ifdef RC_DEFAULT
		if ((Jumanrc_Fileptr = art_fopen(RC_DEFAULT ,"r")) == NULL) {
			if (exit_rc_notfound_p) {
			fprintf(stderr, 
				"not found <.jumanrc> and <RC_DEFAULT> file.\n");
			exit(0);
			}
			else {
			Jumanrc_Fileptr = NULL;
			}
		}
#else
		fprintf(stderr,
			"not found <.jumanrc> file in your home directory.\n");
		exit(0);
#endif			
		} else {
		Jumanrc_Fileptr = NULL;
		}
	}
	 }
}
#endif	// JUMAN_KNP

/*
==============================================================================
		Oct. 1996		A.Kitauchi <akira-k@is.aist-nara.ac.jp>
==============================================================================
*/

#define CHA_FILENAME_MAX 1024

static char progpath[CHA_FILENAME_MAX] = "juman";
static char filepath[CHA_FILENAME_MAX];

/*
------------------------------------------------------------------------------
		PROCEDURE
		<cha_exit>: print error messages on stderr and exit
------------------------------------------------------------------------------
*/

void cha_exit(int status, const char *format,
		char *a, char *b, char *c, char *d, char *e, char *f, char *g, char *h) {
	if (Cha_errno)
	  return;

	if (Cha_stderr != stderr)
	  fputs("500 ", Cha_stderr);

	//if (progpath)
	  fprintf(Cha_stderr, "%s: ", progpath);
	fprintf(Cha_stderr, format, a, b, c, d, e, f, g, h);

	if (status >= 0) {
	fputc('\n', Cha_stderr);
	if (Cha_stderr == stderr)
	  exit(status);
	Cha_errno = 1;
	}
}

void cha_exit_file(int status, const char *format,
		char *a, char *b, char *c, char *d, char *e, char *f, char *g, char *h) 
{
	if (Cha_errno)
	  return;

	if (Cha_stderr != stderr)
	  fputs("500 ", Cha_stderr);

	//if (progpath)
	  fprintf(Cha_stderr, "%s: ", progpath);

	if (LineNo == 0)
	  ; /* do nothing */
	else if (LineNo == LineNoForError)
	  fprintf(Cha_stderr, "%s:%d: ", filepath, LineNo);
	else
	  fprintf(Cha_stderr, "%s:%d-%d: ", filepath, LineNoForError, LineNo);

	fprintf(Cha_stderr, format, a, b, c, d, e, f, g, h);

	if (status >= 0) {
	fputc('\n', Cha_stderr);	
	if (Cha_stderr == stderr)
	  exit(status);
	Cha_errno = 1;
	}
}

void cha_perror(char *s) {
	cha_exit(-1, "");
	perror(s);
}

void cha_exit_perror(char *s) {
	cha_perror(s);
	exit(1);
}

