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
#include <gdm/file.h>
#include <gdm/pict.h>
#include <dev/advcapp.h>
#include <interp/interp.h>

#include <sql/sql.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#ifdef	_LINUX
#include <sys/wait.h>

#ifdef	_LINUX_POSTGRES
static const char *col_type_sql = 
"SELECT\
  pg_type.typname,\
  case\
	when pg_type.typname='int2' 	 then  5\
	when pg_type.typname='int4' 	 then  10\
	when pg_type.typname='numeric'	 then (pg_attribute.atttypmod-4) / 65536\
	when pg_type.typname='decimal'	 then (pg_attribute.atttypmod-4) / 65536\
	when pg_type.typname='float4'	 then  7\
	when pg_type.typname='float8'	 then  15\
	when pg_type.typname='bpchar'	 then  pg_attribute.atttypmod-4\
	when pg_type.typname='varchar'	 then  pg_attribute.atttypmod-4\
	when pg_type.typname='date' 	 then 10\
	when pg_type.typname='time' 	 then  8\
	when pg_type.typname='timestamp' then 19\
	when pg_type.typname='bool' 	 then  1\
	end,\
  case\
	when pg_type.typname='numeric'	 then ((pg_attribute.atttypmod-4) % 65536)\
	when pg_type.typname='decimal'	 then ((pg_attribute.atttypmod-4) % 65536)\
	end,\
  pg_attribute.attnotnull\
  FROM	pg_class, pg_type, pg_attribute WHERE\
  pg_attribute.attrelid  = pg_class.oid AND\
  pg_type.oid		  = pg_attribute.atttypid AND\
  pg_class.oid		  = pg_attribute.attrelid AND\
  pg_attribute.attnum >= 0	AND\
  pg_class.relkind	  = 'r' AND\
  pg_class.relname	  NOT LIKE	'pg_%' AND\
  pg_class.relname	  =";
#endif	// _LINUX_POSTGRES

#ifdef	_LINUX_MYSQL
#define TIME_FUZZY_DATE    1
#define TIME_DATETIME_ONLY 2
enum enum_mysql_timestamp_type
str_to_datetime(const char *str, uint length, MYSQL_TIME *l_time,
	uint flags, int *was_cut);
#endif	// _LINUX_MYSQL

#endif	// _LINUX

#ifdef _WINDOWS
static HENV henv = NULL;
#endif // _WINDOWS

#ifdef _LINUX_ORACLE
static OCIEnv *envhp = 0; 
#endif // _LINUX_ORACLE

DBConnection::DBConnection() {
	db_body = NULL;
	table_name = NULL;
	al_sql = NULL;
	al_rs = NULL;
	al_count = 0;

	db_type = NULL;
	connected = 0;


#ifdef	_WINDOWS
	hdbc = NULL;
	hstmt = NULL;
#endif

#ifdef	_LINUX
	begin = 0;

#ifdef _LINUX_ORACLE
	srvhp = 0;	
	svchp = 0;	
	errhp = 0;	
	authp = 0;
	stmthp = 0;
#endif // _LINUX_ORACLE

#ifdef	_LINUX_POSTGRES
	pq_conn = NULL;
	pq_sql = NULL;
	pq_count = 0;
	pq_res = NULL;
#endif // _LINUX_POSTGRES

#ifdef	_LINUX_MYSQL
	my_conn = NULL;
	my_stat = NULL;
	my_res = NULL;
#endif // _LINUX_MYSQL

#endif
}

DBConnection::~DBConnection() {
	AGm::Unref(db_body, al_sql, al_rs);
	if (table_name) delete table_name;
#ifdef	_WINDOWS
	if (hstmt) {
		::SQLFreeStmt(hstmt, SQL_DROP);
	}
	if (hdbc) {
		::SQLDisconnect(hdbc);
		::SQLFreeConnect(hdbc);
	}
#endif

#ifdef _LINUX_ORACLE
	if (connected) {
		OCISessionEnd(svchp, errhp, authp, (ub4)0);
	}
	if (authp) OCIHandleFree((dvoid*)authp, (ub4)OCI_HTYPE_SESSION);
	if (errhp) OCIHandleFree((dvoid*)errhp, (ub4)OCI_HTYPE_ERROR);
	if (svchp) OCIHandleFree((dvoid*)svchp, (ub4)OCI_HTYPE_SVCCTX);
	if (srvhp) OCIHandleFree((dvoid*)srvhp, (ub4)OCI_HTYPE_SERVER);
#endif // _LINUX_ORACLE

#ifdef	_LINUX_POSTGRES
	if (pq_res) {
		PQclear(pq_res);
	}
	if (connected) {
		PQfinish(pq_conn);
	}
	AGm::Unref(pq_sql);
#endif // _LINUX_POSTGRES

#ifdef	_LINUX_MYSQL
	if (connected) {
		if (my_res) {
			mysql_free_result(my_res);
		}
		if (my_stat) {
			mysql_stmt_close(my_stat);
		}
		if (my_conn) {
			mysql_close(my_conn);
		}
	}
#endif // _LINUX_MYSQL
}

void DBConnection::GCmark() {
	AGm::_GCmark(db_body);
	AGm::_GCmark(al_sql);
	AGm::_GCmark(al_rs);
#ifdef	_LINUX_POSTGRES
	AGm::_GCmark(pq_sql);
#endif	// _LINUX_POSTGRES
}

static int sql_init = 0;
int ADvSQLInitialize() {
	if (sql_init) return 0;

#ifdef _WINDOWS
	if (henv != NULL) return 0;
	short rcode = ::SQLAllocEnv(&henv);
	if (rcode != SQL_SUCCESS) {
		henv = NULL; return 1;
	}
#endif	// _WINDOWS

#ifdef	_LINUX_ORACLE
	if (envhp) return 0;
	if (OCIInitialize((ub4)OCI_DEFAULT, (dvoid*)0,	
			(dvoid* (*)(dvoid*, size_t))0,
			(dvoid* (*)(dvoid*, dvoid*, size_t))0,	
			(void (*)(dvoid*, dvoid*))0)) {
		envhp = 0;
		return 1;
	}
	if (OCIEnvInit((OCIEnv**)&envhp, (ub4)OCI_DEFAULT,
			(size_t)0, (dvoid**)0)) {
		envhp = 0;
		return 1;
	}
#endif	// _LINUX_ORACLE

	sql_init = 1;
	return 0;
}
void ADvSQLFinalize() {
	if (sql_init == 0) return;

#ifdef _WINDOWS
	if (henv == NULL) return;
	::SQLFreeEnv(&henv);
	henv = NULL;
#endif // _WINDOWS

#ifdef _LINUX_ORACLE
	if (envhp) {
		OCIHandleFree((dvoid*)envhp, (ub4)OCI_HTYPE_ENV);
		envhp = 0;
	}
#endif // _LINUX_ORACLE

	sql_init = 0;
}

static char err_msg[512];

#ifdef _WINDOWS
static char state[256] = "";
void get_sql_error(HDBC hdbc, HSTMT hstmt) {
	char msg[256]; long code; short num_char;
	msg[0] = 0;
	::SQLError(henv, hdbc, hstmt,
		(unsigned char*)state, &code,
		(unsigned char*)msg, 255, (short *)&num_char);
	if (num_char >= 1 && msg[num_char - 1] == '\n') {
		msg[num_char - 1] = 0;
	}
	art_sprintf_1(err_msg, 512, "%s", msg);
}
#endif // _WINDOWS

#ifdef _LINUX_ORACLE
void get_sql_error(OCIError* errhp) {
	char msg[256]; sb4 code;
	msg[0] = 0;
	OCIErrorGet((dvoid*)errhp, (ub4)1, (text*)NULL, &code,
		(text*)msg, (ub4)255, (ub4)OCI_HTYPE_ERROR);
	sprintf(err_msg, "%s", msg);
}
#endif // _LINUX_ORACLE

#ifdef _LINUX_POSTGRES
void get_sql_error(PGconn* conn) {
	sprintf(err_msg, "%s", PQerrorMessage(conn));
}
void get_sql_error(PGresult* res) {
	sprintf(err_msg, "%s", PQresultErrorMessage(res));
}
#endif // _LINUX_POSTGRES

static AGmNode *get_sql_result(AGmNode *arg1, AGmNode *arg2, AGmNode *arg3) {
	AGmNode *val = AGm::Cons(NULL);

#ifdef	_WINDOWS
	short rcode;
	HSTMT hstmt = arg1->a.stat.impl->hstmt;;
#endif	// _WINDOWS

#ifdef	_LINUX
	DBConnection* impl = arg1->a.stat.impl;
#endif	// _LINUX

#ifdef	_WINDOWS
	long to_read = -1;
	if (AGm::IsInteger(arg3)) {
		to_read = (long)arg3->a.i.v;
	}
	int s_len = 512; char *sv = new char[s_len];
	for (rcode = ::SQLFetch(hstmt);
			rcode == SQL_SUCCESS;
			rcode = ::SQLFetch(hstmt)) {
		AGmLink *l; AGmNode *v; AGmNode *res;
		AGm::CreateArc(val, res = AGm::Cons(NULL));
		long count = 1;
		long iv; double rv;
#if	_64BIT
		long long len;
#else
		long len;
#endif
		for (l = AGm::next_outlink(arg2, NULL);
				l;
				l = AGm::next_outlink(arg2, l)) {
			v = l->arc->dst;
			if (!AGm::IsString(v)) {
				delete [] sv;
				art_strcpy(err_msg, 512, "result: illegal type");
				AGm::RefUnref(val); val = AGm::String(err_msg);
				return val;
			}
			const char *str = v->a.s.v->Str();
			AGmNode *count_n = AGm::Ref(AGm::Integer(count));
			if (strcmp(str, "integer") == 0) {
				rcode = ::SQLGetData(hstmt, (unsigned short)count, SQL_C_SLONG,
					&iv, (unsigned short)0, &len);
				if (rcode == SQL_SUCCESS && len > 0)
					AGm::CreateArc(res, AGm::Integer(iv), count_n);
			} else if (strcmp(str, "real") == 0) {
				rcode = ::SQLGetData(hstmt, (unsigned short)count, SQL_C_DOUBLE,
					&rv, (unsigned short)0, &len);
				if (rcode == SQL_SUCCESS && len > 0)
					AGm::CreateArc(res, AGm::Real(rv), count_n);
			} else if (strcmp(str, "string") == 0) {
				AGmString *str = NULL;
				for (;;) {
					rcode = ::SQLGetData(hstmt, (unsigned short)count, SQL_C_CHAR,
						sv, s_len -1, &len);
					if (rcode == SQL_SUCCESS || rcode == SQL_SUCCESS_WITH_INFO) {
						if (rcode == SQL_SUCCESS_WITH_INFO) len = s_len;
						if (str == NULL && len > 0) {
							str = new AGmString(sv);
						} else if (len > 0) {
							str->Append(sv);
						}
					}
					if (rcode != SQL_SUCCESS_WITH_INFO) break;
				}
				if (rcode == SQL_SUCCESS && str) {
					AGm::CreateArc(res, AGm::String(str), count_n);
				} else {
					if (str) delete str;
				}
			} else if (strcmp(str, "date") == 0) {
				struct tagTIMESTAMP_STRUCT tv;
				rcode = ::SQLGetData(hstmt, (unsigned short)count, SQL_C_TIMESTAMP,
					&tv, sizeof(tv), &len);
				if (rcode == SQL_SUCCESS && len > 0) {
					AGmNode *date = AGm::List(
						(long)tv.year, (long)tv.month, (long)tv.day,
						(long)tv.hour, (long)tv.minute, (long)tv.second);
					AGm::CreateArc(res, date, count_n);
				}
			} else if (strcmp(str, "binary") == 0) {
				AGmBinary *bin = NULL; long index = 0;
				for (;;) {
					rcode = ::SQLGetData(hstmt, (unsigned short)count, SQL_C_BINARY,
						sv, s_len, &len);
					if (rcode == SQL_SUCCESS_WITH_INFO) len = s_len;
					if (rcode == SQL_SUCCESS || rcode == SQL_SUCCESS_WITH_INFO) {
						if (bin == NULL && len > 0) {
							bin = new AGmBinary((long)len);
						} else if (len > 0) {
							bin->Expand((long)len);
						}
						if (len > 0) {
							unsigned char *buf = bin->Buffer() + index;
							int i;
							for (i = 0; i < len; i++) buf[i] = sv[i];
							index += (long)len;
						}
					}
					if (rcode == SQL_SUCCESS && bin) break;
				}
				if (rcode == SQL_SUCCESS && bin) {
					AGm::CreateArc(res, AGm::Binary(bin), count_n);
				} else {
					if (bin) delete bin;
				}
			}
			AGm::Unref(count_n);
			count++;
		}
		if (to_read > 0) {
			if (--to_read == 0) break;
		}
	}
	delete [] sv;
#endif	// _WINDOWS

#ifdef	_LINUX

#ifdef	_LINUX_ORACLE
	if (strcmp(impl->db_type, "oracle") == 0) {
		long to_read = -1;
		if (AGm::IsInteger(arg3)) {
			to_read = arg3->a.i.v;
		}
		AGmNode *lens = AGm::Ref(AGm::Cons(NULL)), *len;
		AGmNode *bins = AGm::Ref(AGm::Cons(NULL));
		AGmNode *indps = AGm::Ref(AGm::Cons(NULL)), *indp;
		int err = 0;
		long count = 1;
		AGmLink *l, *l2, *l3; AGmNode *v;
		OCIDefine *defnp;
		for (l = AGm::next_outlink(arg2, NULL);
				l;
				l = AGm::next_outlink(arg2, l)) {
			defnp = 0;
			v = l->arc->dst;
			if (!AGm::IsString(v)) {
				strcpy(err_msg, "result: illegal type");
				AGm::RefUnref(val); val = AGm::String(err_msg);
				AGm::Unref(lens, bins, indps);
				return val;
			}
			const char *str = v->a.s.v->Str();
			if (strcmp(str, "integer") == 0) {
				AGm::CreateArc(lens, len = AGm::Integer(0));
				AGm::CreateArc(indps, indp = AGm::Integer(-1));
				err = OCIDefineByPos(impl->stmthp, &defnp, impl->errhp, count,
					(dvoid*)&len->a.i.v, (sword)sizeof(len->a.i.v), SQLT_INT,
					(dvoid*)&indp->a.i.v, (ub2*)0, (ub2*)0, OCI_DEFAULT);
			} else if (strcmp(str, "real") == 0) {
				AGm::CreateArc(lens, len = AGm::Real(0.0));
				AGm::CreateArc(indps, indp = AGm::Integer(-1));
				err = OCIDefineByPos(impl->stmthp, &defnp, impl->errhp, count,
					(dvoid*)&len->a.r.v, (sword)sizeof(len->a.r.v), SQLT_FLT,
					(dvoid*)&indp->a.i.v, (ub2*)0, (ub2*)0, OCI_DEFAULT);
			} else if (strcmp(str, "string") == 0) {
				OCIParam *paramp; ub2 size;
				err = OCIParamGet(impl->stmthp, OCI_HTYPE_STMT, impl->errhp,
					(dvoid**)&paramp, count);
				if (err == 0) {
					err = OCIAttrGet((dvoid*)paramp, (ub4)OCI_DTYPE_PARAM,
						(dvoid*)&size, (ub4*)0, (ub4)OCI_ATTR_DATA_SIZE, impl->errhp);
					if (err == 0) {
						AGm::CreateArc(lens, len = AGm::String(new AGmString((long)size)));
						AGm::CreateArc(indps, indp = AGm::Integer(-1));
						err = OCIDefineByPos(impl->stmthp, &defnp, impl->errhp, count,
							(dvoid*)len->a.s.v->Str(), (sword)size, SQLT_STR,
							(dvoid*)&indp->a.i.v, (ub2*)0, (ub2*)0, OCI_DEFAULT);
					}
				}
			} else if (strcmp(str, "date") == 0) {
				ub1 d[7];
				AGm::CreateArc(lens, len = AGm::Binary(new AGmBinary(sizeof(d))));
				AGm::CreateArc(indps, indp = AGm::Integer(-1));
				err = OCIDefineByPos(impl->stmthp, &defnp, impl->errhp, count,
					(dvoid*)len->a.bin.v->Buffer(), (sword)sizeof(d), SQLT_DAT,
					(dvoid*)&indp->a.i.v, (ub2*)0, (ub2*)0, OCI_DEFAULT);
			} else if (strcmp(str, "binary") == 0) {
				OCILobLocator *lob;
				if (OCIDescriptorAlloc((dvoid*)envhp, (dvoid**)&lob,	
						(ub4)OCI_DTYPE_LOB, (size_t)0, (dvoid**)0)) {
					AGm::Unref(lens, bins, indps);
					strcpy(err_msg, "result: fail to alloc lob locator");
					AGm::RefUnref(val); val = AGm::String(err_msg);
					return val;
				}
				AGm::CreateArc(lens, len = AGm::PtrCell(lob));
				AGm::CreateArc(bins, len);
				AGm::CreateArc(indps, indp = AGm::Integer(-1));
				err = OCIDefineByPos(impl->stmthp, &defnp, impl->errhp, count,
					(dvoid*)&len->a.p.ptr, (sword)-1, SQLT_BLOB,
					(dvoid*)&indp->a.i.v, (ub2*)0, (ub2*)0, OCI_DEFAULT);
			} else {
				AGm::Unref(lens, bins, indps);
				strcpy(err_msg, "result: illegal type");
				AGm::RefUnref(val); val = AGm::String(err_msg);
				return val;
			}
			if (err == OCI_ERROR) {
				AGm::Unref(lens, bins, indps);
				get_sql_error(impl->errhp);
				AGm::RefUnref(val); val = AGm::String(err_msg);
				return val;
			}
			count++;
		}
		int status;
		for (;;) {
			status = OCIStmtFetch(impl->stmthp, impl->errhp, (ub4)1,
				(ub4)OCI_FETCH_NEXT, (ub4)OCI_DEFAULT);
			if (status != OCI_SUCCESS) break;
			AGmNode *res;
			AGm::CreateArc(val, res = AGm::Cons(NULL));
			long count = 1;
			long iv; double rv;
			for (l = AGm::next_outlink(arg2, NULL),
				 l2 = AGm::next_outlink(lens, NULL),
				 l3 = AGm::next_outlink(indps, NULL);
				 l;
				 l = AGm::next_outlink(arg2, l),
				 l2 = AGm::next_outlink(lens, l2),
				 l3 = AGm::next_outlink(indps, l3)) {
				v = l->arc->dst;
				len = l2->arc->dst;
				indp = l3->arc->dst;
				const char *str = v->a.s.v->Str();
				AGmNode *count_n = AGm::Ref(AGm::Integer(count));
				if (strcmp(str, "integer") == 0) {
					if (indp->a.i.v != -1) {
						AGm::CreateArc(res, AGm::Integer(len->a.i.v), count_n);
					}
				} else if (strcmp(str, "real") == 0) {
					if (indp->a.i.v != -1) {
						AGm::CreateArc(res, AGm::Real(len->a.r.v), count_n);
					}
				} else if (strcmp(str, "string") == 0) {
					if (indp->a.i.v != -1) {
						AGm::CreateArc(res, AGm::String(len->a.s.v->Str()), count_n);
					}
				} else if (strcmp(str, "date") == 0) {
					if (indp->a.i.v != -1) {
						ub1 *d = (ub1*)len->a.bin.v->Buffer();
						AGmNode *date = AGm::List(
							(long)(d[0] * 100 + d[1] -10100), (long)d[2], (long)d[3],
							(long)(d[4] -1), (long)(d[5] -1), (long)(d[6] -1));
						AGm::CreateArc(res, date, count_n);
					}
				} else if (strcmp(str, "binary") == 0) {
					if (indp->a.i.v != -1) {
						OCILobLocator *lobl = (OCILobLocator*)len->a.p.ptr;
						ub4 loblen; ub4 amtp = loblen;
						OCILobGetLength(impl->svchp, impl->errhp, lobl, &loblen);
						AGmBinary *bin = new AGmBinary((long)loblen);
						unsigned char *buf = bin->Buffer();
						status = OCILobRead(impl->svchp, impl->errhp, lobl,
							&amtp, 1, (dvoid*)buf,
							loblen, (dvoid *)0, 
							(sb4 (*)(dvoid*, dvoid*, ub4, ub1))0,
							(ub2)0, (ub1)SQLCS_IMPLICIT);
						if (status) break;
						AGm::CreateArc(res, AGm::Binary(bin), count_n);
					}
				}
				AGm::Unref(count_n);
				count++;
			}
			if (status == OCI_ERROR) break;
			if (to_read > 0) {
				if (--to_read == 0) break;
			}
		}
		if (status == OCI_ERROR) {
			AGm::Unref(lens, indps);
			for (l = AGm::next_outlink(bins, NULL); l; l = AGm::next_outlink(bins, l)) {
				len = l->arc->dst;
				OCILobLocator *lob = (OCILobLocator*)len->a.p.ptr;
				OCIDescriptorFree((dvoid *)lob, (ub4)OCI_DTYPE_LOB);
			}
			AGm::Unref(bins);
			get_sql_error(impl->errhp);
			AGm::RefUnref(val); val = AGm::String(err_msg);
			return val;
		}
		AGm::Unref(lens, indps);
		for (l = AGm::next_outlink(bins, NULL); l; l = AGm::next_outlink(bins, l)) {
			len = l->arc->dst;
			OCILobLocator *lob = (OCILobLocator*)len->a.p.ptr;
			OCIDescriptorFree((dvoid *)lob, (ub4)OCI_DTYPE_LOB);
		}
		AGm::Unref(bins);
	}
#endif	// _LINUX_ORACLE

#ifdef	_LINUX_POSTGRES
	if (strcmp(impl->db_type, "postgresql") == 0) {
		if (impl->pq_res == NULL) {
			strcpy(err_msg, "result: resultSet is null");
			AGm::RefUnref(val); val = AGm::String(err_msg);
			return val;
		}
		if (PQresultStatus(impl->pq_res) != PGRES_TUPLES_OK) {
			strcpy(err_msg, "result: not resultSet");
			AGm::RefUnref(val); val = AGm::String(err_msg);
			return val;
		}
		long to_read = -1;
		if (AGm::IsInteger(arg3)) {
			to_read = arg3->a.i.v;
		}

		AGmLink *l; AGmNode *v;
		char *buf;
		while (impl->pq_tuple < PQntuples(impl->pq_res)) {
			AGmNode *res;
			AGm::CreateArc(val, res = AGm::Cons(NULL));
			long count = 1;
			for (l = AGm::next_outlink(arg2, NULL);
					l && count <= PQnfields(impl->pq_res);
					l = AGm::next_outlink(arg2, l)) {
				v = l->arc->dst;
				if (PQgetisnull(impl->pq_res, impl->pq_tuple, count - 1) == 0) {
					buf = PQgetvalue(impl->pq_res, impl->pq_tuple, count - 1);
					const char *str = v->a.s.v->Str();
					AGmNode *count_n = AGm::Ref(AGm::Integer(count));
					if (strcmp(str, "integer") == 0) {
						long iv = atoi(buf);
						AGm::CreateArc(res, AGm::Integer(iv), count_n);
					} else if (strcmp(str, "real") == 0) {
						double rv = atof(buf);
						AGm::CreateArc(res, AGm::Real(rv), count_n);
					} else if (strcmp(str, "string") == 0) {
						if (*buf) AGm::CreateArc(res, AGm::String(buf), count_n);
					} else if (strcmp(str, "date") == 0) {
						long year, month, day, hour, minute, second, st;
						sscanf(buf, "%ld-%ld-%ld %ld:%ld:%ld+%ld",
							&year, &month, &day, &hour, &minute, &second, &st);
						AGmNode *date = AGm::List(year, month, day, hour, minute, second);
						AGm::CreateArc(res, date, count_n);
					} else if (strcmp(str, "binary") == 0) {
						int length = PQgetlength(impl->pq_res, impl->pq_tuple, count - 1) + 1;
						unsigned char *buf2 = new unsigned char[length];
						decode_escape_sequence(buf2, (unsigned char*)buf, &length);
						AGmBinary *bin = new AGmBinary((long)length);
						unsigned char *buf3 = bin->Buffer();
						memcpy(buf3, buf2, length);
						delete [] buf2;
						AGm::CreateArc(res, AGm::Binary(bin), count_n);
					}
					AGm::Unref(count_n);
				}
				count++;
			}
			impl->pq_tuple++;
			if (to_read > 0) {
				if (--to_read == 0) break;
			}
		}
	}
#endif	// _LINUX_POSTGRES

#ifdef	_LINUX_MYSQL
	if (strcmp(impl->db_type, "mysql") == 0) {
		if (impl->my_res == NULL) {
			AGm::RefUnref(val);
			sprintf(err_msg, "resultSet is null: %s", mysql_error(impl->my_conn));
			val = AGm::String(err_msg);
			return val;
		}
		long to_read = -1;
		if (AGm::IsInteger(arg3)) {
			to_read = arg3->a.i.v;
		}
		MYSQL_ROW row;
		for (row = mysql_fetch_row(impl->my_res);
				row;
				row = mysql_fetch_row(impl->my_res)) {
			AGmLink *l; AGmNode *v; AGmNode *res;
			AGm::CreateArc(val, res = AGm::Cons(NULL));
			long count = 1;
			for (l = AGm::next_outlink(arg2, NULL);
					l;
					l = AGm::next_outlink(arg2, l)) {
				v = l->arc->dst;
				if (!AGm::IsString(v)) {
					AGm::RefUnref(val); val = AGm::String("result: illegal type");
					return val;
				}
				const char *str = v->a.s.v->Str();
				AGmNode *count_n = AGm::Ref(AGm::Integer(count));
				if (strcmp(str, "integer") == 0) {
					if (row[count-1]) {
						long iv = atol(row[count-1]);
						AGm::CreateArc(res, AGm::Integer(iv), count_n);
					}
				} else if (strcmp(str, "real") == 0) {
					if (row[count-1]) {
						double rv = atof(row[count-1]);
						AGm::CreateArc(res, AGm::Real(rv), count_n);
					}
				} else if (strcmp(str, "string") == 0) {
					if (row[count-1]) {
						AGm::CreateArc(res, AGm::String(row[count-1]), count_n);
					}
				} else if (strcmp(str, "date") == 0) {
					if (row[count-1]) {
						MYSQL_TIME tv; int was_cut;
						str_to_datetime(row[count-1], strlen(row[count-1]), &tv,
							TIME_DATETIME_ONLY, &was_cut);
						AGmNode *date = AGm::List(
							(long)tv.year, (long)tv.month, (long)tv.day,
							(long)tv.hour, (long)tv.minute, (long)tv.second);
						AGm::CreateArc(res, date, count_n);
					}
				} else if (strcmp(str, "binary") == 0) {
					if (row[count-1]) {
						unsigned long *lengths = mysql_fetch_lengths(impl->my_res);
						AGmBinary *bin = new AGmBinary(lengths[count-1]);
						memcpy(bin->Buffer(), row[count-1], lengths[count-1]);
						AGm::CreateArc(res, AGm::Binary(bin), count_n);
					}
				}
				AGm::Unref(count_n);
				count++;
			}
			if (to_read > 0) {
				if (--to_read == 0) break;
			}
		}
	}
#endif	// _LINUX_MYSQL

#endif	// _LINUX
	return val;
}

int AIpProcess::EvalSQL() {
	AGmNode *cmd, *arg1, *arg2, *arg3, *arg4;
	GetArg(cmd, arg1, arg2, arg3, arg4);
	if (!IsString(cmd)) {
		Unref(cmd, arg1, arg2, arg3, arg4);
		return interp_error("EvalSQL()");
	}
	// ADvDoEvents();
	const char* s = cmd->a.s.v->Str();
	Unref(val); val = NULL;

	if (strcmp(s, "connect") == 0) {
		val = Ref(SQL::connect(arg1, arg2));
	} else if (strcmp(s, "statement") == 0) {
		val = Ref(SQL::statement(arg1));
	} else if (strcmp(s, "prepare") == 0) {
		val = Ref(SQL::prepare(arg1, arg2));
	} else if (strcmp(s, "execute") == 0) {
		val = Ref(SQL::execute(arg1, arg2, arg3));
	} else if (strcmp(s, "result") == 0) {
		val = Ref(SQL::result(arg1, arg2, arg3));
	} else if (strcmp(s, "commit") == 0) {
		val = Ref(SQL::commit(arg1));
	} else if (strcmp(s, "rollback") == 0) {
		val = Ref(SQL::rollback(arg1));
	} else if (strcmp(s, "col_names") == 0) {
		val = Ref(SQL::col_names(arg1));
	} else if (strcmp(s, "col_types") == 0) {
		val = Ref(SQL::col_types(arg1));
	} else if (strcmp(s, "finalize") == 0) {
		SQL::Finalize();
	} else {
		Unref(cmd, arg1, arg2, arg3, arg4);
		return interp_error("EvalSQL()");
	}
	Unref(cmd, arg1, arg2, arg3, arg4);
	return 0;
}

#ifdef	_WINDOWS
short SQL::rcode;
HDBC SQL::hdbc;
HSTMT SQL::hstmt;
#endif	// _WINDOWS
#ifdef	_LINUX
DBConnection* SQL::impl;
#endif	// _LINUX

int SQL::Initialize() {
	return ADvSQLInitialize();
}
void SQL::Finalize() {
	ADvSQLFinalize();
}
AGmNode* SQL::connect(AGmNode* arg1, AGmNode* arg2) {
	if (Initialize() != 0) return String("fail to initialize.");
	if (!arg1 || !IsList(arg1)
			|| !arg1->T || !IsList(arg1->T)
			|| !arg1->TT || !IsList(arg1->TT)
			|| !arg1->TTT || !IsList(arg1->TTT)
			|| !arg1->TTTT || !IsList(arg1->TTTT)) {
		art_strcpy(err_msg, 512, "connect: argument is not string");
		return String(err_msg);
	}
#ifdef	_LINUX
	const char *host = (IsString(arg1->H) ? arg1->H->a.s.v->Str() : NULL);
	const char *port = (IsString(arg1->TH) ? arg1->TH->a.s.v->Str() : NULL);
#endif
	const char *db_name = (IsString(arg1->TTH) ? arg1->TTH->a.s.v->Str() : "");
	const char *user = (IsString(arg1->TTTH) ? arg1->TTTH->a.s.v->Str() : "");
	const char *passwd = (IsString(arg1->TTTTH) ? arg1->TTTTH->a.s.v->Str(): "");
	const char *db_type = (IsString(arg2) ? arg2->a.s.v->Str() : "");
#ifdef	_WINDOWS
	rcode = ::SQLAllocConnect(henv, &hdbc);
	if (rcode != SQL_SUCCESS) {
		art_strcpy(err_msg, 512, "connect: fail to SQLAllocConnect");
		return String(err_msg);
	}
	rcode = ::SQLConnect(hdbc,
		(unsigned char*)db_name, (short)strlen(db_name),
		(unsigned char*)user,	(short)strlen(user),
		(unsigned char*)passwd, (short)strlen(passwd));
	if (rcode != SQL_SUCCESS) {
		get_sql_error(hdbc, NULL);
		::SQLFreeConnect(hdbc);
		return String(err_msg);
	}
	// auto-commit false
	rcode = ::SQLSetConnectOption(hdbc, SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF);
	if (rcode != SQL_SUCCESS) {
		get_sql_error(hdbc, NULL);
		::SQLFreeConnect(hdbc);
		return String(err_msg);
	}
	AGmNode *n = Integer(0);
	n->type = AGM_NODE_SQL_CONN;
	n->a.conn.impl = new DBConnection();
	n->a.conn.impl->hdbc = hdbc;
	n->a.conn.impl->hstmt = NULL;
	n->a.conn.impl->connected = 1;
	n->a.conn.impl->db_type = (char*)db_type;
	return n;
#endif	// _WINDOWS

#ifdef	_LINUX
	impl = new DBConnection();

#ifdef	_LINUX_ORACLE
	if (strcmp(db_type, "oracle") == 0) {
		// alloc handles
		if (OCIHandleAlloc((dvoid*)envhp, (dvoid**)&impl->svchp,
				(ub4)OCI_HTYPE_SVCCTX, (size_t)0, (dvoid**)0)) {
			strcpy(err_msg, "connect: fail to alloc service context handle");
			delete impl;
			return String(err_msg);
		}
		if (OCIHandleAlloc((dvoid*)envhp, (dvoid**)&impl->errhp,
				(ub4)OCI_HTYPE_ERROR, (size_t)0, (dvoid**)0)) {
			strcpy(err_msg, "connect: fail to alloc error handle");
			delete impl;
			return String(err_msg);
		}
		if (OCIHandleAlloc((dvoid*)envhp, (dvoid**)&impl->srvhp,
				(ub4)OCI_HTYPE_SERVER, (size_t)0, (dvoid**)0)) {
			strcpy(err_msg, "connect: fail to alloc server handle");
			delete impl;
			return String(err_msg);
		}
		if (OCIHandleAlloc((dvoid *)envhp, (dvoid**)&impl->authp,
				(ub4)OCI_HTYPE_SESSION, (size_t)0, (dvoid**)0)) {
			strcpy(err_msg, "connect: fail to alloc session handle");
			delete impl;
			return String(err_msg);
		}
		// set server
		if (OCIServerAttach(impl->srvhp, impl->errhp, (text*)db_name,
				(sb4)strlen(db_name), (ub4)OCI_DEFAULT)) {
			strcpy(err_msg, "connect: fail to attach server to server handle");
			delete impl;
			return String(err_msg);
		}
		if (OCIAttrSet((dvoid*)impl->svchp, (ub4)OCI_HTYPE_SVCCTX,
				(dvoid *)impl->srvhp, (ub4)0, (ub4)OCI_ATTR_SERVER, impl->errhp)) {
			strcpy(err_msg, "connect: fail to set service context to server");
			delete impl;
			return String(err_msg);
		}
		// set username
		if (OCIAttrSet((dvoid*)impl->authp, (ub4)OCI_HTYPE_SESSION,
				(dvoid*)user, (ub4)strlen(user), (ub4)OCI_ATTR_USERNAME, impl->errhp)) {
			strcpy(err_msg, "connect: fail to set username to service context");
			delete impl;
			return String(err_msg);
		}
		// set password
		if (OCIAttrSet((dvoid*)impl->authp, (ub4)OCI_HTYPE_SESSION,
				(dvoid*)passwd, (ub4)strlen(passwd), (ub4)OCI_ATTR_PASSWORD, impl->errhp)) {
			strcpy(err_msg, "connect: fail to set password to service context");
			delete impl;
			return String(err_msg);
		}
		if (OCISessionBegin(impl->svchp, impl->errhp, impl->authp,
				(ub4)OCI_CRED_RDBMS, (ub4)OCI_DEFAULT)) {
			get_sql_error(impl->errhp);
			delete impl;
			return String(err_msg);
		}
		if (OCIAttrSet((dvoid *)impl->svchp, (ub4)OCI_HTYPE_SVCCTX,
				 (dvoid *)impl->authp, (ub4)0, (ub4)OCI_ATTR_SESSION, impl->errhp)) {
			strcpy(err_msg, "connect: fail to set session to service context");
			delete impl;
			return String(err_msg);
		}
		impl->connected = 1;
		impl->db_type = (char*)db_type;
	}
#endif	// _LINUX_ORACLE

#ifdef	_LINUX_POSTGRES
	if (strcmp(db_type, "postgresql") == 0) {
		impl->pq_conn = PQsetdbLogin(host, port, NULL, NULL, db_name, user, passwd);
		if (impl->pq_conn == NULL) {
			strcpy(err_msg, "connect: fail to connect");
			delete impl;
			return String(err_msg);
		}
		if (PQstatus(impl->pq_conn) == CONNECTION_BAD) {
			get_sql_error(impl->pq_conn);
			PQfinish(impl->pq_conn);
			delete impl;
			return String(err_msg);
		}
		impl->connected = 1;
		impl->db_type = (char*)db_type;
	}
#endif	// _LINUX_POSTGRES

#ifdef	_LINUX_MYSQL
		if (strcmp(db_type, "mysql") == 0) {
			impl->my_conn = mysql_init(NULL);
			if (impl->my_conn == NULL) {
				strcpy(err_msg, "connect: fail to initialize");
				delete impl;
				return String(err_msg);
			}
			unsigned int port_num = (port ? atoi(port) : 0);
			MYSQL *conn = mysql_real_connect(impl->my_conn,
				host, user, passwd, db_name, port_num, NULL, 0);
			if (conn == NULL) {
				sprintf(err_msg, "connect: fail to connect: %s", mysql_error(impl->my_conn));
				delete impl;
				return String(err_msg);
			}
			mysql_autocommit(impl->my_conn, 0);
			impl->connected = 1;
			impl->db_type = (char*)db_type;
		}
#endif	// _LINUX_MYSQL

		if (!impl->connected) {
			sprintf(err_msg, "unsupported db type: %s", db_type);
			delete impl;
			return String(err_msg);
		}
		AGmNode *n = Integer(0);
		n->type = AGM_NODE_SQL_CONN;
		n->a.conn.impl = impl;
		return n;
#endif	// _LINUX
}
AGmNode* SQL::statement(AGmNode* arg1) {
	if (arg1 == NULL || arg1->type != AGM_NODE_SQL_CONN) {
		art_strcpy(err_msg, 512, "statement: not connection");
		return String(err_msg);
	}

#ifdef	_WINDOWS
	if (arg1->a.conn.impl->hstmt) {
		::SQLFreeStmt(arg1->a.stat.impl->hstmt, SQL_DROP);
		arg1->a.stat.impl->hstmt = NULL;
	}
	hdbc = arg1->a.conn.impl->hdbc;
	rcode = ::SQLAllocStmt(hdbc, &hstmt);
	if (rcode != SQL_SUCCESS) {
		get_sql_error(hdbc, NULL);
		return String(err_msg);
	}
	AGmNode *n = Integer(0);
	n->type = AGM_NODE_SQL_STAT;
	n->a.stat.impl = arg1->a.conn.impl;
	n->a.stat.impl->hstmt = hstmt;
	CreateArc(n, arg1); // statement refer to connection
	return n;
#endif	// _WINDOWS

#ifdef	_LINUX
	impl = arg1->a.conn.impl;

#ifdef	_LINUX_ORACLE
	if (strcmp(impl->db_type, "oracle") == 0) {
		if (impl->stmthp) {
			OCIHandleFree((dvoid*)impl->stmthp, (ub4)OCI_HTYPE_STMT);
			impl->stmthp = 0;
		}
		if (OCIHandleAlloc((dvoid *)envhp, (dvoid **)&impl->stmthp,
			(ub4)OCI_HTYPE_STMT, (size_t)0, (dvoid**)0)) {
			impl->stmthp = 0;
			strcpy(err_msg, "statement: fail to alloc handle");
			return String(err_msg);
		}
	}
#endif	// _LINUX_ORACLE

#ifdef	_LINUX_MYSQL
	if (strcmp(impl->db_type, "mysql") == 0) {
		if (impl->my_stat) {
			mysql_stmt_close(impl->my_stat);
			impl->my_stat = NULL;
		}
		impl->my_stat = mysql_stmt_init(impl->my_conn);
		if (impl->my_stat == NULL) {
			sprintf(err_msg, "connect: fail to create statement: %s", mysql_error(impl->my_conn));
			return String(err_msg);
		}
	}
#endif	// _LINUX_MYSQL

	AGmNode *n = Integer(0);
	n->type = AGM_NODE_SQL_STAT;
	n->a.stat.impl = impl;
	CreateArc(n, arg1); // statement refer to connection
	return n;
#endif	// _LINUX
}
AGmNode* SQL::prepare(AGmNode* arg1, AGmNode* arg2) {
	if (arg1 && arg1->type == AGM_NODE_SQL_STAT && IsString(arg2)) {
		const char *sql = arg2->a.s.v->Str();

#ifdef	_WINDOWS
		hdbc = arg1->a.stat.impl->hdbc;
		hstmt = arg1->a.stat.impl->hstmt;
		rcode = ::SQLPrepare(hstmt, (unsigned char*)sql, SQL_NTS);
		if (rcode != SQL_SUCCESS) {
			get_sql_error(hdbc, hstmt);
			return String(err_msg);
		}
#endif	// _WINDOWS

#ifdef	_LINUX
		impl = arg1->a.stat.impl;

#ifdef	_LINUX_ORACLE
		if (strcmp(impl->db_type, "oracle") == 0) {
			// ? -> :bn<n>
			AGmFile *f = new AGmFile(); AGmString *sql_str = new AGmString(512);
			{
				f->Create(arg2->a.s.v, AGM_FILE_STR_READ);
				int count = 1;
				for (;;) {
					char c;
					if (f->ReadChar(c)) break;
					if (c == '?') {
						char tmp_str[16];
						sprintf(tmp_str, ":bn%d", count++);
						sql_str->Append(tmp_str);
						continue;
					}
					if (c == '\'') {
						f->Unput();
						AGmString *tmp_str;
						int err = f->_ReadSingleQString(tmp_str);
						sql_str->Append('\'');
						sql_str->Append(tmp_str->Str());
						sql_str->Append('\'');
						delete tmp_str;
						if (err) break;
						continue;
					}
					sql_str->Append(c);
					if (isIdentHead(c)) {
						if (f->ReadChar(c)) break;
						sql_str->Append(c);
					}
				}
				if (count > 1) {
					sql = sql_str->Str();
				} else {
					delete sql_str; delete f; f = NULL;
				}
			}

			if (OCIStmtPrepare(impl->stmthp, impl->errhp, (text*)sql, (ub4)strlen(sql),
				(ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT)) {
				get_sql_error(impl->errhp);
				return String(err_msg);
			}

			if (f) {
				delete sql_str; delete f;
			}
		}
#endif	 // _LINUX_ORACLE

#ifdef	_LINUX_POSTGRES
		if (strcmp(impl->db_type, "postgresql") == 0) {
			if (impl->begin == 0) {
				AGmFile *f = new AGmFile(); AGmString *type;
				f->Create(arg2->a.s.v, AGM_FILE_STR_READ);
				if (f->ReadString(type)) {
					delete f;
					strcpy(err_msg, "prepare: token expected");
					return String(err_msg);
				}
				if (type->CompareI("SELECT") != 0) {
					if (impl->begin == 0) {
						if (impl->pq_res) PQclear(impl->pq_res);
						impl->pq_res = PQexec(impl->pq_conn, "BEGIN");
						if (impl->pq_res == NULL) {
							get_sql_error(impl->pq_conn);
							delete type;
							delete f;
							return String(err_msg);
						}
						impl->begin = 1;
					}
				} else {
					AGmString *from, *name;
					for (;;) {
						if (f->ReadString(from)) break;
						if (from->CompareI("FROM") == 0) {
							if (f->ReadString(name) == 0) {
								if (impl->table_name) delete impl->table_name;
								impl->table_name = name;
							}
							delete from;
							break;
						}
						delete from;
					}
				}
				delete type;
				delete f;
			}

			Unref(impl->pq_sql); impl->pq_sql = Ref(Cons(NULL));
			AGmFile *f = new AGmFile();
			AGmString *sql_str = new AGmString(16);
			int count = 0;
			{
				f->Create(arg2->a.s.v, AGM_FILE_STR_READ);
				for (;;) {
					char c;
					if (f->ReadChar(c)) break;
					if (c == '?') {
						if (sql_str) {
							CreateArc(impl->pq_sql, String(sql_str));
							sql_str = NULL;
							count++;
						}
						continue;
					}
					if (c == '\'') {
						f->Unput();
						AGmString *tmp_str;
						sql_str->Append('\'');
						int err = f->_ReadSingleQString(tmp_str);
						if (err) break;
						if (sql_str == NULL) sql_str = new AGmString(16);
						sql_str->Append(tmp_str->Str());
						sql_str->Append('\'');
						delete tmp_str;
						continue;
					}
					if (sql_str == NULL) sql_str = new AGmString(16);
					sql_str->Append(c);
					if (isIdentHead(c)) {
						if (f->ReadChar(c)) break;
						sql_str->Append(c);
					}
				}
			}
			if (sql_str) {
				CreateArc(impl->pq_sql, String(sql_str));
				sql_str = NULL;
				impl->pq_count = count;
			}

			delete f;
		}
#endif	// _LINUX_POSTGRES

#ifdef	_LINUX_MYSQL
		if (strcmp(impl->db_type, "mysql") == 0) {
			if (impl->my_stat == NULL) {
				sprintf(err_msg, "statement is null.");
				return String(err_msg);
			}
			if (impl->begin == 0) {
				AGmFile *f = new AGmFile(); AGmString *type;
				f->Create(arg2->a.s.v, AGM_FILE_STR_READ);
				if (f->ReadString(type)) {
					delete f;
					strcpy(err_msg, "prepare: token expected");
					return String(err_msg);
				}
				if (type->CompareI("SELECT") != 0) {
					if (impl->my_res) mysql_free_result(impl->my_res);
					if (mysql_query(impl->my_conn, "BEGIN")) {
						sprintf(err_msg, "fail to execute: %s", mysql_error(impl->my_conn));
						delete type;
						delete f;
						return String(err_msg);
					}
					impl->begin = 1;
				}
			}
			if (mysql_stmt_prepare(impl->my_stat, sql, strlen(sql))) {
				sprintf(err_msg, AL_STR("fail to prepare: "), mysql_stmt_error(impl->my_stat));
				return String(err_msg);
			}
		}
#endif	 // _LINUX_MYSQL

#endif	// _LINUX
	}
	return NULL;
}
AGmNode* SQL::execute(AGmNode* arg1, AGmNode* arg2, AGmNode* arg3) {
	if (arg1 == NULL || arg1->type != AGM_NODE_SQL_STAT) {
		art_strcpy(err_msg, 512, "execute: not statment");
		return String(err_msg);
	}

#ifdef	_WINDOWS
	hdbc = arg1->a.stat.impl->hdbc;
	hstmt = arg1->a.stat.impl->hstmt;
#endif	// _WINDOWS

#ifdef	_LINUX
	impl = arg1->a.stat.impl;
#endif	// _LINUX

	if (IsString(arg2)) {
		const char *sql = arg2->a.s.v->Str();

#ifdef	_WINDOWS
		rcode = ::SQLExecDirect(hstmt, (unsigned char*)sql, SQL_NTS);
		if (rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO) {
			get_sql_error(hdbc, hstmt);
			return String(err_msg);
		}
#endif	// _WINDOWS

#ifdef	_LINUX

#ifdef	_LINUX_ORACLE
		if (strcmp(impl->db_type, "oracle") == 0) {
			if (OCIStmtPrepare(impl->stmthp, impl->errhp, (text*)sql, (ub4)strlen(sql),
				(ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT)) {
				get_sql_error(impl->errhp);
				return String(err_msg);
			}
			AGmFile *f = new AGmFile(); AGmString *type;
			f->Create(arg2->a.s.v, AGM_FILE_STR_READ);
			if (f->ReadString(type)) {
				delete f;
				strcpy(err_msg, "execute: token expected");
				return String(err_msg);
			}
			delete f;
			if (type->CompareI("SELECT") != 0) {
				delete type;
				if (OCIStmtExecute(impl->svchp, impl->stmthp, impl->errhp, (ub4)1, (ub4)0,
						(OCISnapshot*)0, (OCISnapshot*)0, (ub4)OCI_DEFAULT)) {
					get_sql_error(impl->errhp);
					return String(err_msg);
				}
			} else {
				delete type;
				if (OCIStmtExecute(impl->svchp, impl->stmthp, impl->errhp, (ub4)0, (ub4)0,
						(OCISnapshot*)0, (OCISnapshot*)0, (ub4)OCI_DEFAULT)) {
					get_sql_error(impl->errhp);
					return String(err_msg);
				}
			}
		}
#endif	// _LINUX_ORACLE

#ifdef	_LINUX_POSTGRES
		if (strcmp(impl->db_type, "postgresql") == 0) {
			if (impl->begin == 0) {
				AGmFile *f = new AGmFile(); AGmString *type;
				f->Create(arg2->a.s.v, AGM_FILE_STR_READ);
				if (f->ReadString(type)) {
					delete f;
					strcpy(err_msg, "execute: token expected");
					return String(err_msg);
				}
				if (type->CompareI("SELECT") != 0) {
					if (impl->pq_res) PQclear(impl->pq_res);
					impl->pq_res = PQexec(impl->pq_conn, "BEGIN");
					if (impl->pq_res == NULL) {
						get_sql_error(impl->pq_conn);
						delete type;
						delete f;
						return String(err_msg);
					}
					impl->begin = 1;
				} else {
					AGmString *from, *name;
					for (;;) {
						if (f->ReadString(from)) break;
						if (from->CompareI("FROM") == 0) {
							if (f->ReadString(name) == 0) {
								if (impl->table_name) delete impl->table_name;
								impl->table_name = name;
							}
							break;
						}
						delete from;
					}
				}
				delete type;
				delete f;
			}

			if (impl->pq_res) {
				PQclear(impl->pq_res); impl->pq_res = NULL;
			}
			impl->pq_tuple = 0;
			impl->pq_res = PQexec(impl->pq_conn, sql);
			if (impl->pq_res == NULL) {
				get_sql_error(impl->pq_conn);
				return String(err_msg);
			}
			switch (PQresultStatus(impl->pq_res)) {
			case PGRES_TUPLES_OK:  // select ok
			case PGRES_COMMAND_OK: // insert,update etc. ok
				break;
			case PGRES_EMPTY_QUERY:
			case PGRES_NONFATAL_ERROR:
			case PGRES_FATAL_ERROR:
			case PGRES_BAD_RESPONSE:
			default:
				get_sql_error(impl->pq_res);
				return String(err_msg);
			}
		}
#endif	// _LINUX_POSTGRES

#ifdef	_LINUX_MYSQL
		if (strcmp(impl->db_type, "mysql") == 0) {
			if (impl->my_res) {
				mysql_free_result(impl->my_res);
				impl->my_res = NULL;
			}
			AGmFile *f = new AGmFile(); AGmString *type;
			f->Create(arg2->a.s.v, AGM_FILE_STR_READ);
			if (f->ReadString(type)) {
				delete f;
				strcpy(err_msg, "execute: token expected");
				return String(err_msg);
			}
			if (impl->begin == 0) {
				if (type->CompareI("SELECT") != 0) {
					if (impl->my_res) mysql_free_result(impl->my_res);
					if (mysql_query(impl->my_conn, "BEGIN")) {
						sprintf(err_msg, "fail to execute: %s", mysql_error(impl->my_conn));
						delete type;
						delete f;
						return String(err_msg);
					}
					impl->begin = 1;
				}
			}
			if (mysql_query(impl->my_conn, sql) != 0) {
				sprintf(err_msg, "fail to execute: %s", mysql_error(impl->my_conn));
				return String(err_msg);
			}
			impl->my_res = mysql_use_result(impl->my_conn);
		}
#endif	// _LINUX_MYSQL

#endif	// _LINUX

	} else if (arg3) {
		AGmLink *l; AGmNode *v;

#ifdef	_WINDOWS
#ifdef	_64BIT
		long long *int_vec;
		long long *len;
#else
		long *int_vec;
		long *len;
#endif
		int count = 0;
		for (l = next_outlink(arg3, NULL);
				l;
				l = next_outlink(arg3, l)) {
			count++;
		}
#ifdef	_64BIT
		int_vec = new long long[count];
#else
		int_vec = new long[count];
#endif		rcode = SQL_SUCCESS;
		count = 1;
		for (l = next_outlink(arg3, NULL);
				l;
				l = next_outlink(arg3, l)) {
			v = l->arc->dst;
			switch (v->type) {
			case AGM_NODE_INTEGER:
				int_vec[count-1] = 0;
				len = &int_vec[count-1];
				rcode = ::SQLBindParameter(hstmt,
					(unsigned short)count, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
					0, 0,
					&v->a.i.v, (unsigned short)0, len);
				break;
			case AGM_NODE_REAL:
				int_vec[count-1] = 0;
				len = &int_vec[count-1];
				rcode = ::SQLBindParameter(hstmt,
					(unsigned short)count, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
					0, 0,
					&v->a.r.v, (unsigned short)0, len);
				break;
			case AGM_NODE_STRING:
				int_vec[count-1] = SQL_NTS;
				len = &int_vec[count-1];
				rcode = ::SQLBindParameter(hstmt,
					(unsigned short)count, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,
					strlen(v->a.s.v->Str()), 0,
					(void*)v->a.s.v->Str(), 0, len);
				break;
			case AGM_NODE_BINARY:
				int_vec[count-1] = v->a.bin.v->Length();
				len = &int_vec[count-1];
				if (strcmp(arg1->a.stat.impl->db_type, "oracle") == 0) {
					rcode = ::SQLBindParameter(hstmt,
						(unsigned short)count, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_LONGVARBINARY,
						v->a.bin.v->Length(), 0,
						v->a.bin.v->Buffer(), (unsigned short)0, len);
				} else if (strcmp(arg1->a.stat.impl->db_type, "postgresql") == 0
						|| strcmp(arg1->a.stat.impl->db_type, "mysql") == 0) {
					rcode = ::SQLBindParameter(hstmt,
						(unsigned short)count, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY,
						v->a.bin.v->Length(), 0,
						v->a.bin.v->Buffer(), (unsigned short)0, len);
				} else {
					char msg[256];
					art_sprintf_1(msg, 256, "execute: unkown db_type (%s)", arg1->a.stat.impl->db_type);
					delete [] int_vec;
					return String(msg);
				}
				break;
			case AGM_NODE_LIST:
				if (IsInteger(v->H) &&
						v->T && IsInteger(v->TH) &&
						v->TT && IsInteger(v->TTH) &&
						v->TTT && IsInteger(v->TTTH) &&
						v->TTTT && IsInteger(v->TTTTH) &&
						v->TTTTT && IsInteger(v->TTTTTH)) {
					struct tagTIMESTAMP_STRUCT tv;
					int_vec[count-1] = 0;
					len = &int_vec[count-1];
					tv.year   = (short)v->HI;
					tv.month  = (short)v->THI;
					tv.day	  = (short)v->TTHI;
					tv.hour   = (short)v->TTTHI;
					tv.minute = (short)v->TTTTHI;
					tv.second = (short)v->TTTTTHI;
					rcode = ::SQLBindParameter(hstmt,
						(unsigned short)count, SQL_PARAM_INPUT, SQL_C_TIMESTAMP, SQL_TIMESTAMP,
						sizeof(tv), 0,
						&tv, (unsigned short)0, len);
					break;
				}
			default:
				char msg[256];
				art_sprintf_2(msg, 256, "execute: illegal %d-th type value (%d)", count+1, v->type);
				art_strcat(err_msg, 512, msg);
				delete [] int_vec;
				return String(err_msg);
			}
			if (rcode != SQL_SUCCESS) {
				get_sql_error(hdbc, hstmt);
				delete [] int_vec;
				return String(err_msg);
			}
			count++;
		}

		rcode = ::SQLExecute(hstmt);
		if (rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO) {
			get_sql_error(hdbc, hstmt);
			delete [] int_vec;
			return String(err_msg);
		}
		delete [] int_vec;
#endif	// _WINDOWS

#ifdef	_LINUX

#ifdef	_LINUX_ORACLE
		if (strcmp(impl->db_type, "oracle") == 0) {
			int err;
			int count = 1;
			AGmNode *ds = Ref(Cons(NULL)), *dp;
			AGmNode *lobs = Ref(Cons(NULL)), *lobp;
			ub1 *d;
			OCILobLocator *lob;
			for (l = next_outlink(arg3, NULL);
					l;
					l = next_outlink(arg3, l)) {
				v = l->arc->dst;
				char bn[16];
				OCIBind *bndhp =0;
				ub4 loblen;
				switch (v->type) {
				case AGM_NODE_INTEGER:
					sprintf(bn, ":bn%d", count);
					err = OCIBindByName(impl->stmthp, &bndhp,
						impl->errhp, (text*)bn, (sb4)strlen(bn),
						(dvoid*)&v->a.i.v, (sb4)sizeof(v->a.i.v), SQLT_INT,
						(dvoid*)0, (ub2*)0, (ub2*)0,
						(ub4)0, (ub4 *)0, (ub4)OCI_DEFAULT);
					break;
				case AGM_NODE_REAL:
					sprintf(bn, ":bn%d", count);
					err = OCIBindByName(impl->stmthp, &bndhp,
						impl->errhp, (text*)bn, (sb4)strlen(bn),
						(dvoid*)&v->a.r.v, (sb4)sizeof(v->a.r.v), SQLT_FLT,
						(dvoid*)0, (ub2*)0, (ub2*)0,
						(ub4)0, (ub4 *)0, (ub4)OCI_DEFAULT);
					break;
				case AGM_NODE_STRING:
					sprintf(bn, ":bn%d", count);
					err = OCIBindByName(impl->stmthp, &bndhp,
						impl->errhp, (text*)bn, (sb4)strlen(bn),
						(dvoid*)v->a.s.v->Str(), (sb4)(v->a.s.v->Length() + 1), SQLT_STR,
						(dvoid*)0, (ub2*)0, (ub2*)0,
						(ub4)0, (ub4 *)0, (ub4)OCI_DEFAULT);
					break;
				case AGM_NODE_LIST:
					if (IsInteger(v->H) &&
							v->T && IsInteger(v->TH) &&
							v->TT && IsInteger(v->TTH) &&
							v->TTT && IsInteger(v->TTTH) &&
							v->TTTT && IsInteger(v->TTTTH) &&
							v->TTTTT && IsInteger(v->TTTTTH)) {
						d = new ub1[7];
						int yyyy = v->HI + 2;
						d[0] = v->HI / 100 + 100; // year
						d[1] = v->HI % 100 + 100; // year
						d[2] = v->THI; // month
						d[3] = v->TTHI; // day
						d[4] = v->TTTHI + 1; // hour
						d[5] = v->TTTTHI + 1; // min
						d[6] = v->TTTTTHI + 1; // sec
						sprintf(bn, ":bn%d", count);
						CreateArc(ds, dp = PtrCell(d));
						err = OCIBindByName(impl->stmthp, &bndhp,
							impl->errhp, (text*)bn, (sb4)strlen(bn),
							(dvoid*)dp->a.p.ptr, (sb4)sizeof(ub1[7]), SQLT_DAT,
							(dvoid*)0, (ub2*)0, (ub2*)0,
							(ub4)0, (ub4 *)0, (ub4)OCI_DEFAULT);
						break;
					}
				case AGM_NODE_BINARY:
					sprintf(bn, ":bn%d", count);
					err = OCIDescriptorAlloc((dvoid*)envhp, (dvoid**)&lob,	
						(ub4)OCI_DTYPE_LOB, (size_t)0, (dvoid**)0);
					if (err == 0) {
						err = OCILobCreateTemporary(impl->svchp, impl->errhp,
							lob, (ub2)0, SQLCS_IMPLICIT, OCI_TEMP_BLOB,
							OCI_ATTR_NOCACHE, OCI_DURATION_SESSION);
					}
					if (err == 0) {
						ub4 amt = v->a.bin.v->Length();
						ub4 offset = 1;
						err = OCILobWrite(impl->svchp, impl->errhp, lob, &amt, offset,
							(dvoid *)v->a.bin.v->Buffer(),
							(ub4)v->a.bin.v->Length(), OCI_ONE_PIECE, (dvoid *)0,
							(sb4 (*)(dvoid *, dvoid *, ub4 *, ub1 *))0,
							(ub2)0, (ub1)SQLCS_IMPLICIT);
					}
					if (err == 0) {
						err = OCILobGetLength(impl->svchp, impl->errhp, lob, &loblen);
					}
					if (err == 0) {
						CreateArc(lobs, List(lobp = PtrCell(lob), v));
						err = OCIBindByName(impl->stmthp, &bndhp,
							impl->errhp, (text*)bn, (sb4)strlen(bn),
							(dvoid*)&lobp->a.p.ptr, (sb4)loblen, SQLT_BLOB,
							(dvoid*)0, (ub2*)0, (ub2*)0,
							(ub4)0, (ub4 *)0, (ub4)OCI_DEFAULT);
					}
					break;
				default:
					char msg[256];
					art_sprintf_2(msg, 256, "execute: illegal %d-th type value (%d)", count+1, v->type);
					strcat(err_msg, msg);
					return String(err_msg);
				}
				if (err) {
					get_sql_error(impl->errhp);
					for (l = next_outlink(ds, NULL); l; l= next_outlink(ds, l)) {
						d = (ub1*)l->arc->dst->a.p.ptr;
						delete [] d;
					}
					for (l = next_outlink(lobs, NULL); l; l= next_outlink(lobs, l)) {
						lob = (OCILobLocator*)l->arc->dst->a.p.ptr;
						OCIDescriptorFree((dvoid *)lob, (ub4)OCI_DTYPE_LOB);
					}
					Unref(ds, lobs);
					return String(err_msg);
				}
				count++;
			}
			
			err = OCIStmtExecute(impl->svchp, impl->stmthp, impl->errhp, (ub4)1, (ub4)0,
					(OCISnapshot*)0, (OCISnapshot*)0, (ub4)OCI_DEFAULT);
			if (err) {
				get_sql_error(impl->errhp);
				for (l = next_outlink(ds, NULL); l; l= next_outlink(ds, l)) {
					d = (ub1*)l->arc->dst->a.p.ptr;
					delete [] d;
				}
				for (l = next_outlink(lobs, NULL); l; l= next_outlink(lobs, l)) {
					lob = (OCILobLocator*)l->arc->dst->a.p.ptr;
					OCIDescriptorFree((dvoid *)lob, (ub4)OCI_DTYPE_LOB);
				}
				Unref(ds, lobs);
				return String(err_msg);
			}
			for (l = next_outlink(ds, NULL); l; l= next_outlink(ds, l)) {
				d = (ub1*)l->arc->dst->a.p.ptr;
				delete [] d;
			}
			err = 0;
			for (l = next_outlink(lobs, NULL); l; l= next_outlink(lobs, l)) {
				lob = (OCILobLocator*)l->arc->dst->a.p.ptr;
				OCIDescriptorFree((dvoid *)lob, (ub4)OCI_DTYPE_LOB);
			}
			Unref(ds, lobs);
		}
#endif	// _LINUX_ORACLE

#ifdef	_LINUX_POSTGRES
		if (strcmp(impl->db_type, "postgresql") == 0) {
			if (impl->pq_sql == NULL) {
				strcpy(err_msg, "execute: statement not prepared");
				return String(err_msg);
			}
			AGmNode *itr = Ref(DstItr(impl->pq_sql));
			AGmNode *sql = itr_next(itr);
			AGmString *sql_str = new AGmString(16);
			char buf[256], *cp;
			int count = 1;
			for (l = next_outlink(arg3, NULL);
					l;
					l = next_outlink(arg3, l)) {
				if (sql) {
					sql_str->Append(sql->a.s.v->Str());
					sql = itr_next(itr);
				}
				v = l->arc->dst;
				switch (v->type) {
				case AGM_NODE_INTEGER:
					sprintf(buf, "%ld", v->a.i.v);
					sql_str->Append(buf);
					break;
				case AGM_NODE_REAL:
					sprintf(buf, "%.16g", v->a.r.v);
					sql_str->Append(buf);
					break;
				case AGM_NODE_STRING:
					sql_str->Append('\'');
					for (cp = (char*)v->a.s.v->Str(); *cp;) {
						char c = *cp++;
						sql_str->Append(c);
						if (c == '\'') sql_str->Append(c);
					}
					sql_str->Append('\'');
					break;
				case AGM_NODE_BINARY: {
					int length = v->a.bin.v->Length();
					unsigned char *buf = v->a.bin.v->Buffer();
					unsigned char *buf2 = new unsigned char[length*5+3];
					encode_escape_sequence(buf2, buf, length);
					sql_str->Append((char*)buf2);
					delete [] buf2;
					break;
				}
				case AGM_NODE_LIST:
					if (IsInteger(v->H) &&
							v->T && IsInteger(v->TH) &&
							v->TT && IsInteger(v->TTH) &&
							v->TTT && IsInteger(v->TTTH) &&
							v->TTTT && IsInteger(v->TTTTH) &&
							v->TTTTT && IsInteger(v->TTTTTH)) {
						int year   = v->HI;
						int month  = v->THI;
						int day    = v->TTHI;
						int hour   = v->TTTHI;
						int minute = v->TTTTHI;
						int second = v->TTTTTHI;
						sprintf(buf, "'%d-%d-%d %d:%d:%d'",
							year, month, day, hour, minute, second);
						sql_str->Append(buf);
						break;
					}
				default:
					art_sprintf_2(err_msg, 256, "execute: illegal %d-th type value (%d)", count+1, v->type);
					return String(err_msg);
				}
				count++;
			}
			if (sql) {
				sql_str->Append(sql->a.s.v->Str());
				sql = itr_next(itr);
			}
			Unref(itr);

			if (impl->pq_res) {
				PQclear(impl->pq_res); impl->pq_res = NULL;
			}
			impl->pq_tuple = 0;
			impl->pq_res = PQexec(impl->pq_conn, sql_str->Str());
			delete sql_str;
			if (impl->pq_res == NULL) {
				get_sql_error(impl->pq_conn);
				return String(err_msg);
			}

			switch (PQresultStatus(impl->pq_res)) {
			case PGRES_TUPLES_OK:  // select ok
			case PGRES_COMMAND_OK: // insert,update etc. ok
				break;
			case PGRES_EMPTY_QUERY:
			case PGRES_NONFATAL_ERROR:
			case PGRES_FATAL_ERROR:
			case PGRES_BAD_RESPONSE:
			default:
				get_sql_error(impl->pq_res);
				return String(err_msg);
			}
		}
#endif	// _LINUX_POSTGRES

#ifdef	_LINUX_MYSQL
		if (strcmp(impl->db_type, "mysql") == 0) {
			MYSQL_BIND *bind; unsigned long *length; char *is_null;
			int count = 0;
			AGmLink *l;
			for (l = next_outlink(arg3, NULL);
					l;
					l = next_outlink(arg3, l)) {
				count++;
			}
			bind = new MYSQL_BIND[count];
			length = new unsigned long[count];
			is_null = new char[count];
			count = 0;
			for (l = next_outlink(arg3, NULL);
					l;
					l = next_outlink(arg3, l)) {
				v = l->arc->dst;
				switch (v->type) {
				case AGM_NODE_INTEGER:
					bind[count].buffer_type = MYSQL_TYPE_LONG;
					bind[count].buffer = (char*)&v->a.i.v;
					bind[count].is_null = &is_null[count];
					is_null[count] = 0;
					break;
				case AGM_NODE_REAL:
					bind[count].buffer_type = MYSQL_TYPE_DOUBLE;
					bind[count].buffer = (char*)&v->a.r.v;
					bind[count].is_null = &is_null[count];
					is_null[count] = 0;
					break;
				case AGM_NODE_STRING:
					bind[count].buffer_type = MYSQL_TYPE_VAR_STRING;
					bind[count].buffer = (char*)v->a.s.v->Str();
					bind[count].buffer_length = v->a.s.v->Length();
					bind[count].length = &length[count];
					length[count] = v->a.s.v->Length();
					bind[count].is_null = &is_null[count];
					is_null[count] = 0;
					break;
				case AGM_NODE_BINARY:
					bind[count].buffer_type = MYSQL_TYPE_BLOB;
					bind[count].buffer = (char*)v->a.bin.v->Buffer();
					bind[count].buffer_length = v->a.bin.v->Length();
					bind[count].length = &length[count];
					length[count] = v->a.bin.v->Length();
					bind[count].is_null = &is_null[count];
					is_null[count] = 0;
					break;
				case AGM_NODE_LIST:
					if (IsInteger(v->H) &&
							v->T && IsInteger(v->TH) &&
							v->TT && IsInteger(v->TTH) &&
							v->TTT && IsInteger(v->TTTH) &&
							v->TTTT && IsInteger(v->TTTTH) &&
							v->TTTTT && IsInteger(v->TTTTTH)) {
						MYSQL_TIME tv;
						tv.year   = v->HI;
						tv.month  = v->THI;
						tv.day	  = v->TTHI;
						tv.hour   = v->TTTHI;
						tv.minute = v->TTTTHI;
						tv.second = v->TTTTTHI;
						bind[count].buffer_type = MYSQL_TYPE_TIMESTAMP;
						bind[count].buffer = (char*)&tv;
						bind[count].buffer_length = sizeof(tv);
						bind[count].length = &length[count];
						length[count] = sizeof(tv);
						bind[count].is_null = &is_null[count];
						is_null[count] = 0;
						break;
					}
				default:
					char msg[256];
					art_sprintf_2(msg, 256, "execute: illegal %d-th type value (%d)", count+1, v->type);
					strcat(err_msg, msg);
					delete [] bind; delete [] length; delete [] is_null;
					return String(err_msg);
				}
				count++;
			}
			if (mysql_stmt_bind_param(impl->my_stat, bind)) {
				sprintf(err_msg, "fail to bind param: %s", mysql_stmt_error(impl->my_stat));
				delete [] bind; delete [] length; delete [] is_null;
				return String(err_msg);
			}

			if (mysql_stmt_execute(impl->my_stat)) {
				sprintf(err_msg, "fail to execute: %s", mysql_stmt_error(impl->my_stat));
				delete bind; delete length; delete is_null;
				return String(err_msg);
			}
		}
#endif	// _LINUX_MYSQL

#endif	// _LINUX
	}
	return NULL;
}
AGmNode* SQL::result(AGmNode* arg1, AGmNode* arg2, AGmNode* arg3) {
	if (arg1 == NULL || arg1->type != AGM_NODE_SQL_STAT) {
		return String("result: not statment");
	}
	if (arg2 == NULL) {
		return String("result: types is null.");
	}
	return get_sql_result(arg1, arg2, arg3);
}
AGmNode* SQL::commit(AGmNode* arg1) {
	if (arg1 == NULL || arg1->type != AGM_NODE_SQL_CONN) {
		art_strcpy(err_msg, 512, "commit: not connection");
		return String(err_msg);
	}

#ifdef	_WINDOWS
	hdbc = arg1->a.stat.impl->hdbc;
	rcode = ::SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
	if (rcode != SQL_SUCCESS) {
		get_sql_error(hdbc, NULL);
		return String(err_msg);
	}
#endif	// _WINDOWS

#ifdef	_LINUX
	impl = arg1->a.conn.impl;

#ifdef	_LINUX_ORACLE
	if (strcmp(impl->db_type, "oracle") == 0) {
		if (OCITransCommit(impl->svchp, impl->errhp, (ub4)0)) {
			get_sql_error(impl->errhp);
			return String(err_msg);
		}
	}
#endif	// _LINUX_ORACLE

#ifdef	_LINUX_POSTGRES
	if (strcmp(impl->db_type, "postgresql") == 0) {
		if (impl->pq_res) PQclear(impl->pq_res);
		impl->pq_res = PQexec(impl->pq_conn, "COMMIT");
		if (impl->pq_res == NULL) {
			get_sql_error(impl->pq_conn);
			return String(err_msg);
		}
		if (PQresultStatus(impl->pq_res) != PGRES_COMMAND_OK) {
			get_sql_error(impl->pq_res);
			return String(err_msg);
		}
		impl->begin = 0;
	}
#endif	// _LINUX_POSTGRES

#ifdef	_LINUX_MYSQL
	if (strcmp(impl->db_type, "mysql") == 0) {
		if (mysql_commit(impl->my_conn)) {
			sprintf(err_msg, "fail to commit: %s", mysql_error(impl->my_conn));
			return String(err_msg);
		}
	}
#endif	// _LINUX_MYSQL

#endif	// _LINUX
	return NULL;
}
AGmNode* SQL::rollback(AGmNode* arg1) {
	if (arg1 == NULL || arg1->type != AGM_NODE_SQL_CONN) {
		art_strcpy(err_msg, 512, "rollback: not connection");
		return String(err_msg);
	}

#ifdef	_WINDOWS
	hdbc = arg1->a.stat.impl->hdbc;
	rcode = ::SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_ROLLBACK);
	if (rcode != SQL_SUCCESS) {
		get_sql_error(hdbc, NULL);
		return String(err_msg);
	}
#endif	// _WINDOWS

#ifdef	_LINUX
	impl = arg1->a.conn.impl;

#ifdef	_LINUX_ORACLE
	if (strcmp(impl->db_type, "oracle") == 0) {
		if (OCITransRollback(impl->svchp, impl->errhp, (ub4)0)) {
			get_sql_error(impl->errhp);
			return String(err_msg);
		}
	}
#endif	// _LINUX_ORACLE

#ifdef	_LINUX_POSTGRES
	if (strcmp(impl->db_type, "postgresql") == 0) {
		if (impl->pq_res) PQclear(impl->pq_res);
		impl->pq_res = PQexec(impl->pq_conn, "ROLLBACK");
		if (impl->pq_res == NULL) {
			get_sql_error(impl->pq_conn);
			return String(err_msg);
		}
		if (PQresultStatus(impl->pq_res) != PGRES_COMMAND_OK) {
			get_sql_error(impl->pq_res);
			return String(err_msg);
		}
		impl->begin = 0;
	}
#endif	// _LINUX_POSTGRES

#ifdef	_LINUX_MYSQL
	if (strcmp(impl->db_type, "mysql") == 0) {
		if (mysql_rollback(impl->my_conn)) {
			sprintf(err_msg, "fail to rollback: %s", mysql_error(impl->my_conn));
			return String(err_msg);
		}
	}
#endif	// _LINUX_MYSQL

#endif	// _LINUX
	return NULL;
}
AGmNode* SQL::col_names(AGmNode* arg1) {
	AGmNode *ret = NULL;
	if (arg1 && arg1->type == AGM_NODE_SQL_STAT) {
		long count = 1;
		ret = Cons(NULL);

#ifdef	_WINDOWS
		hdbc = arg1->a.stat.impl->hdbc;
		hstmt = arg1->a.stat.impl->hstmt;
		for (;;) {
			unsigned char name[128];  short nameLen;
			short dataType;
#ifdef	_64BIT
			unsigned long long columnSize;
#else
			unsigned long columnSize;
#endif
			short decimalDigits; short nullable;
			rcode = SQLDescribeCol(hstmt, (unsigned short)count,	
				name, (unsigned short)128, &nameLen,
				&dataType, &columnSize,
				&decimalDigits, &nullable);
			if (rcode != SQL_SUCCESS) break;
			CreateArc(ret, String(new AGmString((char*)name)), Integer(count++));
		}
#endif	// _WINDOWS

#ifdef	_LINUX
		impl = arg1->a.stat.impl;

#ifdef	_LINUX_ORACLE
		if (strcmp(impl->db_type, "oracle") == 0) {
			for (;;) {
				text *name; ub4 nameLen;
				int err;
				OCIParam *paramp;
				err = OCIParamGet(impl->stmthp, OCI_HTYPE_STMT, impl->errhp,
					(dvoid**)&paramp, count);
				if (err) break;
				err = OCIAttrGet((dvoid*)paramp, (ub4)OCI_DTYPE_PARAM,
					(dvoid*)&name, (ub4*)&nameLen, (ub4)OCI_ATTR_NAME, impl->errhp);
				if (err) break;
				CreateArc(ret, String(new AGmString((char*)name)), Integer(count++));
			}
		}
#endif	// _LINUX_ORACLE

#ifdef	_LINUX_POSTGRES
		if (strcmp(impl->db_type, "postgresql") == 0) {
			if (impl->pq_res == NULL) {
				strcpy(err_msg, "col_names: resultSet is null.");
				RefUnref(ret); return String(err_msg);
			}
			if (PQresultStatus(impl->pq_res) != PGRES_TUPLES_OK) {
				strcat(err_msg, "col_names: not resultSet.");
				RefUnref(ret); return String(err_msg);
			}
			long i;
			for (i = 0; i < PQnfields(impl->pq_res); i++) {
				char *name = PQfname(impl->pq_res, i);	
				CreateArc(ret, String(name), Integer(count++));
			}
		}
#endif	// _LINUX_POSTGRES

#ifdef	_LINUX_MYSQL
		if (strcmp(impl->db_type, "mysql") == 0) {
			if (impl->my_stat == NULL) {
				RefUnref(ret); return String("statement is null.");
			}
			if (impl->my_res == NULL) {
				RefUnref(ret); return String("resultSet is null.");
			}
			MYSQL_FIELD *field;
			for (field = mysql_fetch_field(impl->my_res);
					field;
					field = mysql_fetch_field(impl->my_res)) {
				CreateArc(ret, String(new AGmString(field->name)), Integer(count++));
			}
		}
#endif	// _LINUX_MYSQL

#endif	// _LINUX
		}
	return ret;
}
AGmNode* SQL::col_types(AGmNode* arg1) {
	AGmNode *ret = NULL;
	if (arg1 && arg1->type == AGM_NODE_SQL_STAT) {
		long count = 1;
		ret = Cons(NULL);

#ifdef	_WINDOWS
		hdbc = arg1->a.stat.impl->hdbc;
		hstmt = arg1->a.stat.impl->hstmt;
		for (;;) {
			unsigned char name[128];  short nameLen;
			short dataType;
#ifdef	_64BIT
			unsigned long long columnSize;
#else
			unsigned long columnSize;
#endif
			short decimalDigits; short nullable;
			rcode = SQLDescribeCol(hstmt, (unsigned short)count,	
				name, (unsigned short)128, &nameLen,
				&dataType, &columnSize,
				&decimalDigits, &nullable);
			if (rcode != SQL_SUCCESS) break;
			AGmNode *n;
			CreateArc(ret, n = Cons(NULL), Integer(count++));
			char *type;
			switch (dataType) {
			case SQL_DECIMAL:
			case SQL_NUMERIC:

			case SQL_BIT:
			case SQL_TINYINT:
			case SQL_SMALLINT:
			case SQL_INTEGER:
			case SQL_BIGINT:
				if (decimalDigits == 0) {
					type = "integer";
				} else {
					type = "decimal";
				}
				break;
			case SQL_REAL:
			case SQL_FLOAT:
			case SQL_DOUBLE:
				type = "real"; break;

			case SQL_CHAR:
			case SQL_WCHAR:
				type = "char"; break;
			case SQL_VARCHAR:
			case SQL_WVARCHAR:
			case SQL_LONGVARCHAR:
			case SQL_WLONGVARCHAR:
				type = "varchar"; break;

			case SQL_TYPE_DATE:
			case SQL_TYPE_TIME:
			case SQL_TYPE_TIMESTAMP:
				type = "date"; break;

			case SQL_BINARY:
			case SQL_VARBINARY:
			case SQL_LONGVARBINARY:
				type = "binary"; break;

			default:
				type = "Unknown"; break;
			}
			CreateArc(n, String(type), String("type"));
			CreateArc(n, Integer((long)columnSize), String("size"));
			CreateArc(n, Integer((long)decimalDigits), String("decimal"));
			if (nullable)
				CreateArc(n, Integer(1), String("nullable"));
		}
#endif	// _WINDOWS

#ifdef	_LINUX
		impl = arg1->a.stat.impl;

#ifdef	_LINUX_ORACLE
		if (strcmp(impl->db_type, "oracle") == 0) {
			for (;;) {
				text *name; ub4 nameLen;
				int err;
				OCIParam *paramp;
				err = OCIParamGet(impl->stmthp, OCI_HTYPE_STMT, impl->errhp,
					(dvoid**)&paramp, count);
				if (err) break;

				AGmNode *n1 = Ref(Cons(NULL));
				AGmNode *n_dty, *n_maxdsz, *n_scale, *n_precision;
				ub2 dty;
				ub2 maxdsz;
				sb1 scale;
				ub1 precision;
				ub1 isnull;
				if (err == 0) {
					err = OCIAttrGet((dvoid *)paramp, (ub4)OCI_DTYPE_PARAM,
						(dvoid*)&dty, (ub4*)0, (ub4)OCI_ATTR_DATA_TYPE, impl->errhp);
				}
				CreateArc(n1, Integer((long)dty), n_dty = String("dty"));
				if (err == 0) {
					err = OCIAttrGet((dvoid *)paramp, (ub4)OCI_DTYPE_PARAM,
						(dvoid*)&maxdsz, (ub4*)0, (ub4)OCI_ATTR_DATA_SIZE, impl->errhp);
				}
				CreateArc(n1, Integer((long)maxdsz), n_maxdsz = String("maxdsz"));
				if (err == 0) {
					err = OCIAttrGet((dvoid *)paramp, (ub4)OCI_DTYPE_PARAM,
						(dvoid *)&scale, (ub4*)0, (ub4)OCI_ATTR_SCALE, impl->errhp);
				}
				CreateArc(n1, Integer((long)scale), n_scale = String("scale"));
				if (err == 0) {
					err = OCIAttrGet((dvoid *)paramp, (ub4)OCI_DTYPE_PARAM,
						(dvoid *)&precision, (ub4*)0, (ub4)OCI_ATTR_PRECISION, impl->errhp);
				}
				CreateArc(n1, Integer((long)precision), n_precision = String("precision"));
				if (err == 0) {
					err = OCIAttrGet((dvoid *)paramp, (ub4)OCI_DTYPE_PARAM,
						(dvoid *)&isnull, (ub4*)0, (ub4)OCI_ATTR_IS_NULL, impl->errhp);
				}
				dty = (ub2)dst_node(n1, n_dty)->a.i.v;
				maxdsz = (ub2)dst_node(n1, n_maxdsz)->a.i.v;
				scale = (sb1)dst_node(n1, n_scale)->a.i.v;
				precision = (ub1)dst_node(n1, n_precision)->a.i.v;
				Unref(n1);
				if (err) {
					get_sql_error(impl->errhp);
					RefUnref(ret); return String(err_msg);
				}

				AGmNode *n;
				CreateArc(ret, n = Cons(NULL), Integer(count++));
				char *type; long size = precision; long decimal = 0;
				switch (dty) {
				case SQLT_NUM:
				case SQLT_PDN:
				case SQLT_INT:
				case SQLT_LNG:
				case SQLT_UIN:
					if (scale < 0) { type = "real"; break; }
					decimal = scale;
					if (decimal == 0) {
						type = "integer";
					} else {
						type = "decimal";
					}
					break;
				case SQLT_FLT:
					type = "real"; break;
				case SQLT_AVC:
				case SQLT_AFC:
					type = "char";	size = maxdsz; break;
				case SQLT_CHR:
				case SQLT_STR:
				case SQLT_VCS:
				case SQLT_LVC:
				case SQLT_CLOB:
					type = "varchar"; size = maxdsz; break;
				case SQLT_DAT:
					type = "date"; break;
				case SQLT_VBI:
				case SQLT_BIN:
				case SQLT_LBI:
				case SQLT_LVB:
				case SQLT_BLOB:
					type = "binary"; break;
				case SQLT_VNU:
				case SQLT_NON:
				case SQLT_RID:
				case SQLT_SLS:
				case SQLT_CUR:
				case SQLT_RDD:
				case SQLT_LAB:
				case SQLT_OSL:
				case SQLT_NTY:
				case SQLT_REF:
				case SQLT_BFILEE:
				case SQLT_CFILEE:
				case SQLT_RSET:
				case SQLT_NCO:
				case SQLT_VST:
				case SQLT_ODT:
				default:
					type = "Unknown"; break;
				}
				CreateArc(n, String(type), String("type"));
				CreateArc(n, Integer((long)size), String("size"));
				CreateArc(n, Integer((long)decimal), String("decimal"));
				if (isnull) CreateArc(n, Integer(1), String("nullable"));
			}
		}
#endif	 // _LINUX_ORACLE

#ifdef	_LINUX_POSTGRES
		if (strcmp(impl->db_type, "postgresql") == 0) {
			if (impl->table_name == NULL) {
				strcat(err_msg, "col_types: not found table name");
				RefUnref(ret); return String(err_msg);
			}
			char buf[4096];
			strcpy(buf, col_type_sql);
			strcat(buf, "'");
			strcat(buf, impl->table_name->Str());
			strcat(buf, "'");
			PGresult *res = PQexec(impl->pq_conn, buf);
			if (PQresultStatus(res) != PGRES_TUPLES_OK) {
				strcat(err_msg, "col_types: fail to get col info");
				PQclear(res);
				RefUnref(ret); return String(err_msg);
			}
			int i;
			for (i = 0; i < PQntuples(res); i++) {
				AGmNode *n;
				CreateArc(ret, n = Cons(NULL), Integer(count++));
				const char *type = PQgetvalue(res, i, 0);
				int size = 0, decimal = 0, isnull = 0;
				char *buf;
				if (!PQgetisnull(res, i, 1)) {
					buf = PQgetvalue(res, i, 1);
					size = atoi(buf);
				}
				if (!PQgetisnull(res, i, 2)) {
					buf = PQgetvalue(res, i, 2);
					decimal = atoi(buf);
				}
				if (!PQgetisnull(res, i, 3)) {
					buf = PQgetvalue(res, i, 3);
					if (strcmp(buf, "f") == 0) isnull = 1;
				}
				if (strcmp(type, "int2") == 0 || strcmp(type, "int4") == 0) {
					type = "integer";
				} else if (strcmp(type, "numeric") == 0 || strcmp(type, "decimal") == 0) {
					if (decimal == 0) {
						type = "integer";
					} else {
						type = "decimal";
					}
				} else if (strcmp(type, "float4") == 0 || strcmp(type, "float8") == 0) {
					type = "real";
				} else if (strcmp(type, "bpchar") == 0) {
					type = "char";
				} else if (strcmp(type, "varchar") == 0) {
					type = "varchar";
				} else if (strcmp(type, "date") == 0 || strcmp(type, "time") == 0
						|| strcmp(type, "datetime") == 0 || strcmp(type, "timestamp") == 0) {
					type = "date";
				} else if (strcmp(type, "bytea") == 0) {
					type = "binary";
				} else {
					type = "Unknown";
				}
				CreateArc(n, String(type), String("type"));
				CreateArc(n, Integer((long)size), String("size"));
				CreateArc(n, Integer((long)decimal), String("decimal"));
				if (isnull) CreateArc(n, Integer(1), String("nullable"));
			}
			PQclear(res);
		}
#endif	// _LINUX_POSTGRES

#ifdef	_LINUX_MYSQL
		if (strcmp(impl->db_type, "mysql") == 0) {
			if (impl->my_stat == NULL) {
				RefUnref(ret); return String("statement is null.");
			}
			if (impl->my_res == NULL) {
				RefUnref(ret); return String("resultSet is null.");
			}
			MYSQL_FIELD *field;
			for (field = mysql_fetch_field(impl->my_res);
					field;
					field = mysql_fetch_field(impl->my_res)) {
				AGmNode *n;
				CreateArc(ret, n = Cons(NULL), Integer(count++));
				const char *type;
				switch (field->type) {
				case FIELD_TYPE_DECIMAL:
					type = "decimal"; break;
				case FIELD_TYPE_TINY:
				case FIELD_TYPE_SHORT:
				case FIELD_TYPE_LONG:
				case FIELD_TYPE_LONGLONG:
				case FIELD_TYPE_INT24:
					type = "integer"; break;
				case FIELD_TYPE_FLOAT:
				case FIELD_TYPE_DOUBLE:
					type = "real"; break;
				case FIELD_TYPE_STRING:
					type = "char"; break;
				case FIELD_TYPE_VAR_STRING:
					type = "varchar"; break;
				case FIELD_TYPE_TIMESTAMP:
				case FIELD_TYPE_DATE:
				case FIELD_TYPE_TIME:
				case FIELD_TYPE_DATETIME:
				case FIELD_TYPE_YEAR:
				case FIELD_TYPE_NEWDATE:
					type = "date"; break;
				case FIELD_TYPE_TINY_BLOB:
				case FIELD_TYPE_MEDIUM_BLOB:
				case FIELD_TYPE_LONG_BLOB:
				case FIELD_TYPE_BLOB:
					type = "binary"; break;
				case FIELD_TYPE_NULL:
				case FIELD_TYPE_ENUM:
				case FIELD_TYPE_SET:
				default:
					type = "Unknown"; break;
				}
				CreateArc(n, String(type), String("type"));
				CreateArc(n, Integer(field->length), String("size"));
				CreateArc(n, Integer(field->decimals), String("decimal"));
				if ((field->flags & NOT_NULL_FLAG) == 0)
					CreateArc(n, Integer(1), String("nullable"));
			}
		}
#endif	// _LINUX_MYSQL

#endif	// _LINUX
	}
	return ret;
}

void encode_escape_sequence(unsigned char *dst, unsigned char *src, int length) {
	*dst++ = '\'';
	int i;
	for (i = 0; i < length; i++) {
		unsigned char c = *src++;
		if (0x20 <= c && c <= 0x7f) {
			if (c == '\'') {
				*dst++ = '\'';
				*dst++ = '\'';
			} else if (c == '\\') {
				*dst++ = '\\';
				*dst++ = '\\';
				*dst++ = '\\';
				*dst++ = '\\';
			} else {
				*dst++ = c;
			}
		} else {
			unsigned char b1 = (c & 0x07);
			c >>= 3;
			unsigned char b2 = (c & 0x07);
			c >>= 3;
			unsigned char b3 = (c & 0x07);
			*dst++ = '\\';
			if ((b1 | b2 | b3) == 0) *dst++ = '\\';
			*dst++ = '0' + b3;
			*dst++ = '0' + b2;
			*dst++ = '0' + b1;
		}
	}
	*dst++ = '\'';
	*dst++ = 0;
}
void decode_escape_sequence(unsigned char *dst, unsigned char *src, int *length) {
	*length = 0;
	for (;;) {
		unsigned char c = *src++;
		if (c == 0) break;
		if (c == '\\') {
			c = *src++;
			if (c != '\\') {
				c = c - '0';
				c = (c << 3) + *src++ - '0';
				c = (c << 3) + *src++ - '0';
			}
		}
		*dst++ = c; (*length)++;
	}
}

#ifdef	_LINUX_MYSQL

#undef	max
#undef	min

#include <my_global.h>
#include <m_ctype.h>

#define max(a,b)	((a) > (b) ? (a) : (b))
#define min(a,b)	((a) < (b) ? (a) : (b))

extern ulonglong log_10_int[20];
extern uchar days_in_month[];
#define YY_PART_YEAR	   70
#define MAX_DATE_PARTS 8
static uchar internal_format_positions[]= {0, 1, 2, 3, 4, 5, 6, (uchar) 255};

enum enum_mysql_timestamp_type
str_to_datetime(const char *str, uint length, MYSQL_TIME *l_time,
				uint flags, int *was_cut)
{
  uint field_length, year_length, digits, i, number_of_fields;
  uint date[MAX_DATE_PARTS], date_len[MAX_DATE_PARTS];
  uint add_hours= 0, start_loop;
  ulong not_zero_date, allow_space;
  bool is_internal_format;
  const char *pos, *last_field_pos;
  const char *end=str+length;
  const uchar *format_position;
  bool found_delimitier= 0, found_space= 0;
  uint frac_pos, frac_len;
  DBUG_ENTER("str_to_datetime");
  DBUG_PRINT("ENTER",("str: %.*s",length,str));

  LINT_INIT(field_length);
  LINT_INIT(year_length);
  LINT_INIT(last_field_pos);

  *was_cut= 0;

  /* Skip space at start */
  for (; str != end && my_isspace(&my_charset_latin1, *str) ; str++)
	;
  if (str == end || ! my_isdigit(&my_charset_latin1, *str))
  {
	*was_cut= 1;
	DBUG_RETURN(MYSQL_TIMESTAMP_NONE);
  }

  is_internal_format= 0;
  /* This has to be changed if want to activate different timestamp formats */
  format_position= internal_format_positions;

  /*
	Calculate number of digits in first part.
	If length= 8 or >= 14 then year is of format YYYY.
	(YYYY-MM-DD,  YYYYMMDD, YYYYYMMDDHHMMSS)
  */
  for (pos=str; pos != end && my_isdigit(&my_charset_latin1,*pos) ; pos++)
	;

  digits= (uint) (pos-str);
  start_loop= 0;								/* Start of scan loop */
  date_len[format_position[0]]= 0;				/* Length of year field */
  if (pos == end)
  {
	/* Found date in internal format (only numbers like YYYYMMDD) */
	year_length= (digits == 4 || digits == 8 || digits >= 14) ? 4 : 2;
	field_length=year_length-1;
	is_internal_format= 1;
	format_position= internal_format_positions;
  }
  else
  {
	if (format_position[0] >= 3)				/* If year is after HHMMDD */
	{
	  /*
		If year is not in first part then we have to determinate if we got
		a date field or a datetime field.
		We do this by checking if there is two numbers separated by
		space in the input.
	  */
	  while (pos < end && !my_isspace(&my_charset_latin1, *pos))
		pos++;
	  while (pos < end && !my_isdigit(&my_charset_latin1, *pos))
		pos++;
	  if (pos == end)
	  {
		if (flags & TIME_DATETIME_ONLY)
		{
		  *was_cut= 1;
		  DBUG_RETURN(MYSQL_TIMESTAMP_NONE);   /* Can't be a full datetime */
		}
		/* Date field.	Set hour, minutes and seconds to 0 */
		date[0]= date[1]= date[2]= date[3]= date[4]= 0;
		start_loop= 5;						   /* Start with first date part */
	  }
	}
  }

  /*
	Only allow space in the first "part" of the datetime field and:
	- after days, part seconds
	- before and after AM/PM (handled by code later)

	2003-03-03 20:00:20 AM
	20:00:20.000000 AM 03-03-2000
  */
  i= max((uint) format_position[0], (uint) format_position[1]);
  set_if_bigger(i, (uint) format_position[2]);
  allow_space= ((1 << i) | (1 << format_position[6]));
  allow_space&= (1 | 2 | 4 | 8);

  not_zero_date= 0;
  for (i = start_loop;
	   i < MAX_DATE_PARTS-1 && str != end &&
		 my_isdigit(&my_charset_latin1,*str);
	   i++)
  {
	const char *start= str;
	ulong tmp_value= (uint) (uchar) (*str++ - '0');
	while (str != end && my_isdigit(&my_charset_latin1,str[0]) &&
		   (!is_internal_format || field_length--))
	{
	  tmp_value=tmp_value*10 + (ulong) (uchar) (*str - '0');
	  str++;
	}
	date_len[i]= (uint) (str - start);
	if (tmp_value > 999999) 					/* Impossible date part */
	{
	  *was_cut= 1;
	  DBUG_RETURN(MYSQL_TIMESTAMP_NONE);
	}
	date[i]=tmp_value;
	not_zero_date|= tmp_value;

	/* Length-1 of next field */
	field_length= format_position[i+1] == 0 ? 3 : 1;

	if ((last_field_pos= str) == end)
	{
	  i++;										/* Register last found part */
	  break;
	}
	/* Allow a 'T' after day to allow CCYYMMDDT type of fields */
	if (i == format_position[2] && *str == 'T')
	{
	  str++;									/* ISO8601:  CCYYMMDDThhmmss */
	  continue;
	}
	if (i == format_position[5])				/* Seconds */
	{
	  if (*str == '.')							/* Followed by part seconds */
	  {
		str++;
		field_length= 5;						/* 5 digits after first (=6) */
	  }
	  continue;

	  /* No part seconds */
	  date[++i]= 0;
	}
	while (str != end &&
		   (my_ispunct(&my_charset_latin1,*str) ||
			my_isspace(&my_charset_latin1,*str)))
	{
	  if (my_isspace(&my_charset_latin1,*str))
	  {
		if (!(allow_space & (1 << i)))
		{
		  *was_cut= 1;
		  DBUG_RETURN(MYSQL_TIMESTAMP_NONE);
		}
		found_space= 1;
	  }
	  str++;
	  found_delimitier= 1;						/* Should be a 'normal' date */
	}
	/* Check if next position is AM/PM */
	if (i == format_position[6])				/* Seconds, time for AM/PM */
	{
	  i++;										/* Skip AM/PM part */
	  if (format_position[7] != 255)			/* If using AM/PM */
	  {
		if (str+2 <= end && (str[1] == 'M' || str[1] == 'm'))
		{
		  if (str[0] == 'p' || str[0] == 'P')
			add_hours= 12;
		  else if (str[0] != 'a' || str[0] != 'A')
			continue;							/* Not AM/PM */
		  str+= 2;								/* Skip AM/PM */
		  /* Skip space after AM/PM */
		  while (str != end && my_isspace(&my_charset_latin1,*str))
			str++;
		}
	  }
	}
	last_field_pos= str;
  }
  if (found_delimitier && !found_space && (flags & TIME_DATETIME_ONLY))
  {
	*was_cut= 1;
	DBUG_RETURN(MYSQL_TIMESTAMP_NONE);			/* Can't be a datetime */
  }

  str= last_field_pos;

  number_of_fields= i - start_loop;
  while (i < MAX_DATE_PARTS)
  {
	date_len[i]= 0;
	date[i++]= 0;
  }

  if (!is_internal_format)
  {
	year_length= date_len[(uint) format_position[0]];
	if (!year_length)							/* Year must be specified */
	{
	  *was_cut= 1;
	  DBUG_RETURN(MYSQL_TIMESTAMP_NONE);
	}

	l_time->year=				date[(uint) format_position[0]];
	l_time->month=				date[(uint) format_position[1]];
	l_time->day=				date[(uint) format_position[2]];
	l_time->hour=				date[(uint) format_position[3]];
	l_time->minute= 			date[(uint) format_position[4]];
	l_time->second= 			date[(uint) format_position[5]];

	frac_pos= (uint) format_position[6];
	frac_len= date_len[frac_pos];
	if (frac_len < 6)
	  date[frac_pos]*= (uint) log_10_int[6 - frac_len];
	l_time->second_part= date[frac_pos];

	if (format_position[7] != (uchar) 255)
	{
	  if (l_time->hour > 12)
	  {
		*was_cut= 1;
		goto err;
	  }
	  l_time->hour= l_time->hour%12 + add_hours;
	}
  }
  else
  {
	l_time->year=		date[0];
	l_time->month=		date[1];
	l_time->day=		date[2];
	l_time->hour=		date[3];
	l_time->minute= 	date[4];
	l_time->second= 	date[5];
	if (date_len[6] < 6)
	  date[6]*= (uint) log_10_int[6 - date_len[6]];
	l_time->second_part=date[6];
  }
  l_time->neg= 0;

  if (year_length == 2 && not_zero_date)
	l_time->year+= (l_time->year < YY_PART_YEAR ? 2000 : 1900);

  if (number_of_fields < 3 ||
	  l_time->year > 9999 || l_time->month > 12 ||
	  l_time->day > 31 || l_time->hour > 23 ||
	  l_time->minute > 59 || l_time->second > 59 ||
	  (!(flags & TIME_FUZZY_DATE) && (l_time->month == 0 || l_time->day == 0)))
  {
	/* Only give warning for a zero date if there is some garbage after */
	if (!not_zero_date) 						/* If zero date */
	{
	  for (; str != end ; str++)
	  {
		if (!my_isspace(&my_charset_latin1, *str))
		{
		  not_zero_date= 1; 					/* Give warning */
		  break;
		}
	  }
	}
	if (not_zero_date)
	  *was_cut= 1;
	goto err;
  }

  l_time->time_type= (number_of_fields <= 3 ?
					  MYSQL_TIMESTAMP_DATE : MYSQL_TIMESTAMP_DATETIME);

  for (; str != end ; str++)
  {
	if (!my_isspace(&my_charset_latin1,*str))
	{
	  *was_cut= 1;
	  break;
	}
  }

  DBUG_RETURN(l_time->time_type=
			  (number_of_fields <= 3 ? MYSQL_TIMESTAMP_DATE :
									   MYSQL_TIMESTAMP_DATETIME));

err:
  bzero((char*) l_time, sizeof(*l_time));
  DBUG_RETURN(MYSQL_TIMESTAMP_ERROR);
}
#endif	// _LINUX_MYSQL
