/*====================================================================

				   ÉcÅ[ÉãåQ

											   S.Kurohashi 1999. 4. 1

	$Id: tools.c,v 1.7 2005/07/09 13:27:22 ryohei Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

//#define	LEAK_CHECK

unsigned int seed[NSEED][256];

#ifdef _WIN32
/*==================================================================*/
	 JUMAN_KNP_API int sjis_fprintf(FILE *output, const char *fmt, ...)
/*==================================================================*/
{
	va_list ap;
	char buffer[DATA_LEN];
	char *SJISbuffer;

	va_start(ap, fmt);
	art_vsprintf(buffer, DATA_LEN, fmt, ap);
	va_end(ap);

	SJISbuffer = (char *)toStringSJIS(buffer);
	fwrite(SJISbuffer, sizeof(char), strlen(SJISbuffer), output);
	free(SJISbuffer);

	return TRUE;
}
#endif 

/*==================================================================*/
		void *malloc_data(size_t size, const char *comment)
/*==================================================================*/
{
	void *p;
	if ((p = malloc(size)) == NULL) {
	fprintf(stderr, "Can't allocate memory for %s\n", comment);
	exit(-1);
	}
#ifdef	LEAK_CHECK
	TRACE3("%s: address = %x, size = %d\n", comment, p, size);
#endif
	return p;
}

/*==================================================================*/
	  void *realloc_data(void *ptr, size_t size, const char *comment)
/*==================================================================*/
{
	void *p;
	if ((p = realloc(ptr, size)) == NULL) {
	fprintf(stderr, "Can't allocate memory for %s\n", comment);
	exit(-1);
	}
	return p;
}

/*==================================================================*/
			   void init_hash()
/*==================================================================*/
{
	int i, j;

	srand((unsigned int)time(NULL));
	for (i = 0; i < NSEED; ++i) {
	for (j = 0; j < 256; ++j) {
		seed[i][j] = rand();
	}
	}
}

/*==================================================================*/
		   int hash(unsigned char *key, int keylen)
/*==================================================================*/
{
	int i = 0, j = 0, hash = 0;

	while (i < keylen) {
	hash ^= seed[j++][key[i++]];
	j &= NSEED-1;
	}
	return (hash & (TBLSIZE-1));
}

/*==================================================================*/
	 unsigned char *katakana2hiragana(unsigned char *cp)
/*==================================================================*/
{
	int i;
	unsigned char *hira;

	hira = (unsigned char*)art_strdup((const char*)cp);

	for (i = 0; i < (int)strlen((const char*)hira); i += 2) {
	if (*(hira+i) == 0xa5) {
		*(hira+i) = 0xa4;
	}
	}		
	return hira;	/* free ÇµÇƒÇ≠ÇæÇ≥Ç¢ */
}

#if 0
/*==================================================================*/
	 char *strdup_with_check(const char *s)
/*==================================================================*/
{
	if (s) {
	return art_strdup(s);
	}
	else {
	return NULL;
	}
}
#endif

/*====================================================================
							   END
====================================================================*/
