/*====================================================================

				   Database

											   S.Kurohashi 1999. 4. 1

	$Id: db.c,v 1.12 2002/12/06 02:27:12 kawahara Exp $
====================================================================*/
#include "stdafx.h"

#include "knp/knp.h"

extern int hash_put(HASH_FILE *hashdb, char *key, char *value, int mode);

#ifdef GDBM

/* DB open for reading */
DBM_FILE db_read_open(char *filename)
{
	DBM_FILE db;

	if (!(db = gdbm_open(filename, DBM_BLOCK_SIZE, GDBM_READER, 0444, 0))) {
#ifdef KNP_DEBUG
		fprintf(stderr, "db_read_open: %s: %s\n", filename, (char *)strerror(errno));
#endif
	return NULL;
	}
	return db;
}

/* DB open for creating */
DBM_FILE db_write_open(char *filename)
{
	DBM_FILE db;

	if (!(db = gdbm_open(filename, 1024, GDBM_NEWDB, 0644, 0))) {
#ifdef KNP_DEBUG
		fprintf(stderr, "db_write_open: %s: %s\n", filename, (char *)strerror(errno));
#endif
		exit(1);
	}
	return db;
}

/* DB list */
void db_list(DBM_FILE db)
{
	datum content, key, nextkey;

	key = gdbm_firstkey(db);
	while (key.dptr) {
	content = gdbm_fetch(db, key);
	*(key.dptr+key.dsize) = '\0';
	*(content.dptr+content.dsize) = '\0';
	fprintf(stdout, "%s %s\n", key.dptr, content.dptr);
	free(content.dptr);
	nextkey = gdbm_nextkey(db, key);
	free(key.dptr);
	key = nextkey;
	}
}

/* DB get */
char *db_get(DBM_FILE db, char *buf)
{
	datum content, key;
	char *rbuf;

	key.dptr = buf;
	key.dsize = strlen(buf);
	content = gdbm_fetch(db, key);
	if (content.dptr) {
	rbuf = (char *)malloc_data(content.dsize+1, "db_get");
	strncpy(rbuf, content.dptr, content.dsize);
	*(rbuf+content.dsize) = '\0';
	free(content.dptr);
	return rbuf;
	}
	return NULL;
}

/* DB put */
int db_put(DBM_FILE db, char *buf, char *value, char *Separator, int mode)
{
	datum content, key;
	char *buffer = NULL;
	int valuesize, i, storeflag;

	valuesize = strlen(value);

	key.dptr = buf;
	key.dsize = strlen(buf);
	content.dptr = value;
	content.dsize = valuesize;

	if (mode == DBM_APPEND || mode == DBM_AND || mode == DBM_OR) {
	storeflag = gdbm_store(db, key, content, DBM_INSERT);

	/* key existence */
	if (storeflag == 1) {
		/* get the content which already exists */
		content = gdbm_fetch(db, key);

		if (mode == DBM_APPEND) {
		if (Separator)
			buffer = (char *)malloc_data(content.dsize+valuesize+strlen(Separator)+1, "db_put");
		else
			buffer = (char *)malloc_data(content.dsize+valuesize+1, "db_put");
		strncpy(buffer, content.dptr, content.dsize);
		buffer[content.dsize] = '\0';
		free(content.dptr);
		if (Separator)
			strcat(buffer, Separator);
		strcat(buffer, value);

		content.dptr = buffer;
		content.dsize = strlen(content.dptr);
		}
		else if (mode == DBM_AND) {
		for (i = 0; i < content.dsize; i++)
			if (*((char *)(content.dptr)+i) == '0')
			value[i] = '0';
		free(content.dptr);
		content.dptr = value;
		content.dsize = valuesize;
		}
		else if (mode == DBM_OR) {
		for (i = 0; i < content.dsize; i++)
			if (*((char *)(content.dptr)+i) == '1')
			value[i] = '1';
		free(content.dptr);
		content.dptr = value;
		content.dsize = valuesize;
		}
		storeflag = gdbm_store(db, key, content, DBM_REPLACE);
		if (mode == DBM_APPEND)
		free(buffer);
	}
	else if (storeflag < 0) {
		fprintf(stderr, "db_put : Cannot store key.\n");
		exit(4);
	}
	}
	else if (mode == DBM_OVERRIDE) {
	storeflag = gdbm_store(db, key, content, DBM_REPLACE);
	}
	else {
	fprintf(stderr, "db_put : Invalid mode (%d)\n", mode);
	exit(1);
	}
	return 0;
}

#else

/* BerkeleyDB 3 */
#ifdef DB3

/* DB open for reading */
DBM_FILE db_read_open(char *filename)
{
	DBM_FILE db;

	if ((errno = db_create(&db, NULL, 0))) {
#ifdef KNP_DEBUG
		fprintf(stderr, "db_read_open: %s: %s\n", filename, (char *)strerror(errno));
#endif
	return NULL;
	}

	/* Initialization */
	db->set_cachesize(db, 0, 1048576, 0);
	db->set_pagesize(db, 4096);

#ifdef DB41
	if ((errno = db->open(db, NULL, filename, NULL, DB_HASH, DB_RDONLY, 0444))) {
#else
	if ((errno = db->open(db, filename, NULL, DB_HASH, DB_RDONLY, 0444))) {
#endif

#ifdef KNP_DEBUG
		fprintf(stderr, "db_read_open: %s: %s\n", filename, (char *)strerror(errno));
#endif
	db->close(db, 0);
	return NULL;
	}
	return db;
}

/* DB open for creating */
DBM_FILE db_write_open(char *filename)
{
	DBM_FILE db;

	if ((errno = db_create(&db, NULL, 0))) {
#ifdef KNP_DEBUG
		fprintf(stderr, "db_write_open: %s: %s\n", filename, (char *)strerror(errno));
#endif
	exit(1);
	}

	/* Initialization */
	db->set_cachesize(db, 0, 1048576, 0);

#ifdef DB41
	if ((errno = db->open(db, NULL, filename, NULL, DB_HASH, DB_CREATE | DB_TRUNCATE, 0644))) {
#else
	if ((errno = db->open(db, filename, NULL, DB_HASH, DB_CREATE | DB_TRUNCATE, 0644))) {
#endif

#ifdef KNP_DEBUG
		fprintf(stderr, "db_write_open: %s: %s\n", filename, (char *)strerror(errno));
#endif
	db->close(db, 0);
		exit(1);
	}
	return db;
}

/* DB close */
void db_close(DBM_FILE db)
{
	if ((errno = db->close(db, 0))) {
	fprintf(stderr, "db->close : %s\n", (char *)strerror(errno));
		exit(1);
	}
}

/* DB list */
void db_list(DBM_FILE db)
{
	DBC *dbc;
	DBT content, key;

	if ((errno = db->cursor(db, NULL, &dbc, 0))) {
		fprintf(stderr, "db_list : %s\n", strerror(errno));
	exit(1);
	}

	/* Initialization */
	memset(&key, 0, sizeof(key));
	memset(&content, 0, sizeof(content));

	while ((errno = dbc->c_get(dbc, &key, &content, DB_NEXT)) == 0) {	
	fprintf(stdout, "%.*s %.*s\n", (int)key.size, (char *)key.data, 
		(int)content.size, (char *)content.data);
	}
	dbc->c_close(dbc);
}

/* DB get */
char *db_get(DBM_FILE db, char *buf)
{
	DBT content, key;
	char *rbuf;

	/* Initialization */
	memset(&key, 0, sizeof(DBT));
	memset(&content, 0, sizeof(DBT));

	content.flags = DB_DBT_MALLOC;

	key.data = buf;
	key.size = strlen(key.data);
	errno = db->get(db, NULL, &key, &content, 0);

	/* Not existence */
	if (errno == DB_NOTFOUND)
	return NULL;
	/* Other errors */
	else if (errno) {
		fprintf(stderr, "db_get(%s): %s (%d)\n", buf, (char *)strerror(errno), errno);
		exit(1);
	}

	rbuf = (char *)malloc_data(content.size+1, "db_get");
	strncpy(rbuf, content.data, content.size);
	*(rbuf+content.size) = '\0';
	free(content.data);
	return rbuf;
}

/* DB put */
int db_put(DBM_FILE db, char *buf, char *value, char *Separator, int mode)
{
	DBT content, key;
	char *buffer;
	int valuesize, i;

	/* Initialization */
	memset(&key, 0, sizeof(DBT));
	memset(&content, 0, sizeof(DBT));

	valuesize = strlen(value);

	key.data = buf;
	key.size = strlen(buf);
	content.data = value;
	content.size = valuesize;

	if (mode == DBM_APPEND || mode == DBM_AND || mode == DBM_OR) {
	errno = db->put(db, NULL, &key, &content, DB_NOOVERWRITE);

	/* key existence */
	if (errno == DB_KEYEXIST) {
		/* get the content which already exists */
		errno = db->get(db, NULL, &key, &content, 0);

		if (mode == DBM_APPEND) {
		if (Separator)
			buffer = (char *)malloc_data(content.size+valuesize+strlen(Separator)+1, "db_put");
		else
			buffer = (char *)malloc_data(content.size+valuesize+1, "db_put");
		strncpy(buffer, content.data, content.size);
		buffer[content.size] = '\0';
		if (Separator)
			strcat(buffer, Separator);
		strcat(buffer, value);

		content.data = buffer;
		content.size = strlen(content.data);
		}
		else if (mode == DBM_AND) {
		for (i = 0; i < content.size; i++)
			if (*((char *)(content.data)+i) == '0')
			value[i] = '0';
		content.data = value;
		content.size = valuesize;
		}
		else if (mode == DBM_OR) {
		for (i = 0; i < content.size; i++)
			if (*((char *)(content.data)+i) == '1')
			value[i] = '1';
		content.data = value;
		content.size = valuesize;
		}
		errno = db->put(db, NULL, &key, &content, 0);
		if (mode == DBM_APPEND)
		free(buffer);
	}
	else if (errno) {
		fprintf(stderr, "db_put : %s\n", (char *)strerror(errno));
		exit(4);
	}
	}
	else if (mode == DBM_OVERRIDE) {
	errno = db->put(db, NULL, &key, &content, 0);
	}
	else {
	fprintf(stderr, "db_put : Invalid mode (%d)\n", mode);
	exit(1);
	}
	return 0;
}

DB_ENV *dbenv;

/* dbenv setup */
void db_setup()
{
	int ret;

	if ((ret = db_env_create(&dbenv, 0))) {
	fprintf(stderr, "%s\n", db_strerror(ret));
	exit(1);
	}
	dbenv->set_errfile(dbenv, stderr);
	dbenv->set_errpfx(dbenv, "knp");

	/* if ((ret = dbenv->set_lk_detect(dbenv, DB_LOCK_OLDEST))) {
	dbenv->err(dbenv, ret, "environment set_lk_detect");
	dbenv->close(dbenv, 0);
	exit(1);
	} */

	if ((dbenv->open(dbenv, ".",	
			 DB_CREATE | DB_INIT_LOCK | DB_INIT_LOG | DB_INIT_MPOOL | DB_INIT_TXN, 0))) {
	dbenv->err(dbenv, ret, "envirnment open");
	dbenv->close(dbenv, 0);
	exit(1);
	}
}

/* dbenv teardown */
void db_teardown()
{
	dbenv->close(dbenv, 0);
}

#else

#ifdef INTERNAL_HASH

/* DB put */
int db_put(DBM_FILE db, char *buf, char *value, char *Separator, int mode)
{
	int n;

	if (mode == DBM_APPEND || mode == DBM_AND || mode == DBM_OR) {
	errno = hash_put(db, buf, value, HASH_NOOVERWRITE);

	/* key existence */
	if (errno == HASH_KEYEXIST) {
		int valuesize, contentsize, i;
		char *buffer = NULL, *content;

		valuesize = (int)strlen(value);

		/* get the content which already exists */
		content = hash_fetch(db, buf);
		contentsize = (int)strlen(content);

		if (mode == DBM_APPEND) {
		if (Separator)
			buffer = (char *)malloc_data(n = contentsize+valuesize+strlen(Separator)+1, "db_put");
		else
			buffer = (char *)malloc_data(n = contentsize+valuesize+1, "db_put");
		art_strncpy(buffer, n, content, contentsize);
		buffer[contentsize] = '\0';
		if (Separator)
			art_strcat(buffer, n, Separator);
		art_strcat(buffer, n, value);
		}
		else if (mode == DBM_AND) {
		for (i = 0; i < contentsize; i++)
			if (*((char *)(content)+i) == '0')
			value[i] = '0';
		buffer = art_strdup(value);
		}
		else if (mode == DBM_OR) {
		for (i = 0; i < contentsize; i++)
			if (*((char *)(content)+i) == '1')
			value[i] = '1';
		buffer = art_strdup(value);
		}
		free(content);
		errno = hash_put(db, buf, buffer, 0);
		free(buffer);
	}
	else if (errno) {
		fprintf(stderr, "db_put : error\n");
		exit(4);
	}
	}
	else if (mode == DBM_OVERRIDE) {
	errno = hash_put(db, buf, value, 0);
	}
	else {
	fprintf(stderr, "db_put : Invalid mode (%d)\n", mode);
	exit(1);
	}
	return 0;
}

#else
/* BerkeleyDB 2 */

/* DB open for reading */
DBM_FILE db_read_open(char *filename)
{
	DB_INFO dbinfo;
	DBM_FILE db;

	/* Initialization */
	memset(&dbinfo, 0, sizeof(dbinfo));
	dbinfo.db_cachesize = 1048576;

	if ((errno = db_open(filename, DB_HASH, DB_RDONLY, 0444, NULL, &dbinfo, &db))) {
#ifdef KNP_DEBUG
		fprintf(stderr, "db_read_open: %s: %s\n", filename, (char *)strerror(errno));
#endif
	return NULL;
	}
	return db;
}

/* DB open for creating */
DBM_FILE db_write_open(char *filename)
{
	DB_INFO dbinfo;
	DBM_FILE db;

	/* Initialization */
	memset(&dbinfo, 0, sizeof(dbinfo));
	dbinfo.db_cachesize = 1048576;

	if ((errno = db_open(filename, DB_HASH, DB_CREATE | DB_TRUNCATE, 0644, NULL, &dbinfo, &db))) {
#ifdef KNP_DEBUG
		fprintf(stderr, "db_write_open: %s: %s\n", filename, (char *)strerror(errno));
#endif
		exit(1);
	}
	return db;
}

/* DB close */
void db_close(DBM_FILE db)
{
	if ((errno = db->close(db, 0))) {
	fprintf(stderr, "db->close : %s\n", (char *)strerror(errno));
		exit(1);
	}
}

/* DB list */
void db_list(DBM_FILE db)
{
	DBC *dbc;
	DBT content, key;

	if ((errno = db->cursor(db, NULL, &dbc, 0))) {
		fprintf(stderr, "db_list : %s\n", strerror(errno));
	exit(1);
	}

	/* Initialization */
	memset(&key, 0, sizeof(key));
	memset(&content, 0, sizeof(content));

	while ((errno = dbc->c_get(dbc, &key, &content, DB_NEXT)) == 0) {	
	fprintf(stdout, "%.*s %.*s\n", (int)key.size, (char *)key.data, 
		(int)content.size, (char *)content.data);
	}
	dbc->c_close(dbc);
}

/* DB get */
char *db_get(DBM_FILE db, char *buf)
{
	DBT content, key;
	char *rbuf;

	/* Initialization */
	memset(&key, 0, sizeof(DBT));
	memset(&content, 0, sizeof(DBT));

	content.flags = DB_DBT_MALLOC;

	key.data = buf;
	key.size = strlen(key.data);
	errno = db->get(db, NULL, &key, &content, 0);

	/* Not existence */
	if (errno == DB_NOTFOUND)
	return NULL;
	/* Other errors */
	else if (errno) {
		fprintf(stderr, "db_get(%s): %s (%d)\n", buf, (char *)strerror(errno), errno);
		exit(1);
	}

	rbuf = (char *)malloc_data(content.size+1, "db_get");
	strncpy(rbuf, content.data, content.size);
	*(rbuf+content.size) = '\0';
	free(content.data);
	return rbuf;
}

/* DB put */
int db_put(DBM_FILE db, char *buf, char *value, char *Separator, int mode)
{
	DBT content, key;
	char *buffer;
	int valuesize, i;

	/* Initialization */
	memset(&key, 0, sizeof(DBT));
	memset(&content, 0, sizeof(DBT));

	valuesize = strlen(value);

	key.data = buf;
	key.size = strlen(buf);
	content.data = value;
	content.size = valuesize;

	if (mode == DBM_APPEND || mode == DBM_AND || mode == DBM_OR) {
	errno = db->put(db, NULL, &key, &content, DB_NOOVERWRITE);

	/* key existence */
	if (errno == DB_KEYEXIST) {
		/* get the content which already exists */
		errno = db->get(db, NULL, &key, &content, 0);

		if (mode == DBM_APPEND) {
		if (Separator)
			buffer = (char *)malloc_data(content.size+valuesize+strlen(Separator)+1, "db_put");
		else
			buffer = (char *)malloc_data(content.size+valuesize+1, "db_put");
		strncpy(buffer, content.data, content.size);
		buffer[content.size] = '\0';
		if (Separator)
			strcat(buffer, Separator);
		strcat(buffer, value);

		content.data = buffer;
		content.size = strlen(content.data);
		}
		else if (mode == DBM_AND) {
		for (i = 0; i < content.size; i++)
			if (*((char *)(content.data)+i) == '0')
			value[i] = '0';
		content.data = value;
		content.size = valuesize;
		}
		else if (mode == DBM_OR) {
		for (i = 0; i < content.size; i++)
			if (*((char *)(content.data)+i) == '1')
			value[i] = '1';
		content.data = value;
		content.size = valuesize;
		}
		errno = db->put(db, NULL, &key, &content, 0);
		if (mode == DBM_APPEND)
		free(buffer);
	}
	else if (errno) {
		fprintf(stderr, "db_put : %s\n", (char *)strerror(errno));
		exit(4);
	}
	}
	else if (mode == DBM_OVERRIDE) {
	errno = db->put(db, NULL, &key, &content, 0);
	}
	else {
	fprintf(stderr, "db_put : Invalid mode (%d)\n", mode);
	exit(1);
	}
	return 0;
}

#endif
#endif
#endif
