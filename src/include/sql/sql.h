/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _ADV_SQL_H
#define _ADV_SQL_H

AL_EXT_API int ADvSQLInitialize();
AL_EXT_API void ADvSQLFinalize();

#ifdef _LINUX_ORACLE
#include <oci.h>
#endif

#ifdef _LINUX_POSTGRES
#include <libpq-fe.h>
#endif

#ifdef _LINUX_MYSQL
#define	DBUG_OFF
#include <mysql.h>
#endif

class DBConnection { 
public:
	DBConnection();
	~DBConnection();

	void GCmark();

	AGmNode* db_body;
	AGmString* table_name;
	AGmNode* al_sql;
	AGmNode* al_rs;
	int al_count;

	char *db_type;
	int connected;

#ifdef	_WINDOWS
	HDBC hdbc;
	HSTMT hstmt;
#endif

#ifdef	_LINUX
	int begin;

#ifdef	_LINUX_ORACLE
	OCIServer	*srvhp; 
	OCISvcCtx	*svchp; 
	OCIError	*errhp; 
	OCISession	*authp;
	OCIStmt* stmthp;
#endif

#ifdef	_LINUX_POSTGRES
	PGconn		*pq_conn;
	PGresult	*pq_res;
	AGmNode 	*pq_sql;
	int 		pq_count;
	int 		pq_tuple;
#endif

#ifdef	_LINUX_MYSQL
	MYSQL		*my_conn;
	MYSQL_STMT	*my_stat;
	MYSQL_RES	*my_res;
#endif

#endif
};

class SQL : public AGm {
public:
	static int Initialize();
	static void Finalize();
	static AGmNode* connect(AGmNode*, AGmNode*);
	static AGmNode* statement(AGmNode*);
	static AGmNode* prepare(AGmNode*, AGmNode*);
	static AGmNode* execute(AGmNode*, AGmNode*, AGmNode*);
	static AGmNode* result(AGmNode*, AGmNode*, AGmNode*);
	static AGmNode* commit(AGmNode*);
	static AGmNode* rollback(AGmNode*);
	static AGmNode* col_names(AGmNode*);
	static AGmNode* col_types(AGmNode*);

protected:

#ifdef	_WINDOWS
	static short rcode;
	static HDBC hdbc;
	static HSTMT hstmt;
#endif	// _WINDOWS

#ifdef	_LINUX
	static DBConnection* impl;
#endif	// _LINUX
};

void encode_escape_sequence(unsigned char *dst, unsigned char *src, int length);
void decode_escape_sequence(unsigned char *dst, unsigned char *src, int *length);

#endif	/* _ADV_SQL_H */
