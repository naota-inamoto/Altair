
ALTAIR_HOME=/home/inamoto/altair
TARGET=altair
ALCORE_LIB=alcore1.so
JUMAN_KNP_LIB=jumanknp1.so

OPENSSL_HOME=/usr/local/ssl
OPENSSL_LIB=libssl.so libcrypto.so

#ORACLE_HOME=/home/inamoto/altair/oracle-8.1.7
#ORACLE_LIB=libclntsh.so
ORACLE_HOME=
ORACLE_LIB=

POSTGRES_HOME=/usr/local/pgsql
POSTGRES_LIB=libpq.so

MYSQL_HOME=/usr/local/mysql
MYSQL_LIB=libmysqlclient.so

INC_DIR=\
	-I$(ALTAIR_HOME)/include\
	-I$(OPENSSL_HOME)/include\
	-I$(ORACLE_HOME)/rdbms/demo\
	-I$(ORACLE_HOME)/rdbms/public\
	-I$(POSTGRES_HOME)/include\
	-I$(MYSQL_HOME)/include\
	-I/usr/lib64/glib-2.0/include\
	-I/usr/include/glib-2.0\
	-I/usr/include/gtk-3.0\
	-I/usr/include/atk-1.0\
	-I/usr/include/pango-1.0\
	-I/usr/include/cairo\
	-I/usr/include/gdk-pixbuf-2.0

# CPPFLAGS=-D_LINUX_DEBUG -D_LINUX -D_LINUX_ORACLE -D_LINUX_POSTGRES -D_LINUX_MYSQL -D_AL_OPENSSL -DJUMAN_KNP -DAL_LANG_ENGLISH $(INC_DIR) -O0 -g
# CPPFLAGS=-DNDEBUG           -D_LINUX -D_LINUX_ORACLE -D_LINUX_POSTGRES -D_LINUX_MYSQL -D_AL_OPENSSL -DJUMAN_KNP -DAL_LANG_ENGLISH $(INC_DIR) -O0

# CPPFLAGS=-D_LINUX_DEBUG -D_LINUX -D_LINUX_POSTGRES -D_LINUX_MYSQL -D_AL_OPENSSL -DJUMAN_KNP -DAL_LANG_ENGLISH $(INC_DIR) -O0 -g
# CPPFLAGS=-DNDEBUG           -D_LINUX -D_LINUX_POSTGRES -D_LINUX_MYSQL -D_AL_OPENSSL -DJUMAN_KNP -DAL_LANG_ENGLISH $(INC_DIR) -O0

# CPPFLAGS=-D_LINUX_DEBUG -D_LINUX -D_LINUX_POSTGRES -D_LINUX_MYSQL -D_AL_OPENSSL -DJUMAN_KNP -DAL_LANG_JAPANESE $(INC_DIR) -O0 -g
# CPPFLAGS=-DNDEBUG           -D_LINUX -D_LINUX_POSTGRES -# D_LINUX_MYSQL -D_AL_OPENSSL -DJUMAN_KNP -DAL_LANG_JAPANESE $(INC_DIR) -O0

# CPPFLAGS=-D_LINUX_DEBUG -D_LINUX -D_LINUX_POSTGRES -D_LINUX_MYSQL -D_AL_OPENSSL -DJUMAN_KNP -DAL_LANG_ENGLISH -D_64BIT $(INC_DIR) -O0 -fPIC -g
# CPPFLAGS=-DNDEBUG  -D_LINUX -D_LINUX_POSTGRES -D_LINUX_MYSQL -D_AL_OPENSSL -DJUMAN_KNP -DAL_LANG_ENGLISH -D_64BIT $(INC_DIR) -O0 -fPIC

# CPPFLAGS=-D_LINUX_DEBUG -D_LINUX -D_LINUX_POSTGRES -D_LINUX_MYSQL -D_AL_OPENSSL -DJUMAN_KNP -DAL_LANG_JAPANESE -D_64BIT $(INC_DIR) -O0 -fPIC -g
CPPFLAGS=-DNDEBUG  -D_LINUX -D_LINUX_POSTGRES -D_LINUX_MYSQL -D_AL_OPENSSL -DJUMAN_KNP -DAL_LANG_JAPANESE -D_64BIT $(INC_DIR) -O0 -fPIC

# LDFLAGS= -lgtk-3 -lgdk-3 -fPIC -g
LDFLAGS= -lgtk-3 -lgdk-3 -fPIC

ALTAIR_SRC=\
	altair.cpp \
	altairdoc.cpp \
	altairview.cpp
ALCORE_SRC=\
	gdm.cpp \
	pict.cpp \
	string.cpp \
	matrix.cpp \
	debug.cpp \
	error.cpp \
	copy.cpp \
	gc.cpp \
	j_code.cpp \
	big_int.cpp \
	big_int_ia-rhel6.nasm \
	crypt.cpp \
	crypt_key.cpp \
	multipart.cpp \
	prof.cpp \
	gp_intp.cpp \
	interp1.cpp \
	interp2.cpp \
	parallel.cpp \
	prcsmgr.cpp \
	prog_if.cpp \
	sysfunc1.cpp \
	sysfunc2.cpp \
	sysfunc3.cpp \
	file.cpp \
	dllcall.cpp \
	advwnd.cpp \
	bitmap.cpp \
	shell.cpp \
	advcapp.cpp \
	advcvw.cpp \
	advcedit.cpp \
	mainfrm.cpp \
	ocx.cpp \
	olectrl.cpp \
	cui.cpp \
	project.cpp \
	class.cpp \
	member.cpp \
	body.cpp \
	body_in.cpp \
	body_out.cpp \
	sql.cpp \
	socket.cpp \
	xml.cpp \
	typedic.cpp \
	xslt.cpp \
	solid.cpp \
	jana.cpp \
	almath.cpp \
	parser_generator.cpp \
	pro_lisp.cpp \
	prolog.cpp \
	symbolic_manip.cpp \
	game_frwk.cpp
JUMAN_SRC=\
	connect.cpp \
	getid.cpp \
	grammar.cpp \
	iotool.cpp \
	japanese.cpp \
	juman_lib.cpp \
	katuyou.cpp \
	lisp.cpp \
	pat.cpp \
	patfile.cpp
KNP_SRC=\
	bnst_compare.cpp \
	configfile.cpp \
	db.cpp \
	declare.cpp \
	dpnd_analysis.cpp \
	feature.cpp \
	hash.cpp \
	koou.cpp \
	lib_bgh.cpp \
	lib_dt.cpp \
	lib_print.cpp \
	lib_ps.cpp \
	lib_scase.cpp \
	lib_sm.cpp \
	main.cpp \
	para_analysis.cpp \
	para_dpnd.cpp \
	para_relation.cpp \
	para_revision.cpp \
	proper.cpp \
	quote.cpp \
	read_data.cpp \
	read_rule.cpp \
	regexp.cpp \
	thesaurus.cpp \
	tools.cpp \
	tree_conv.cpp

ALTAIR_OBJ=\
	altair.o \
	altairdoc.o \
	altairview.o
ALCORE_OBJ=\
	gdm.o \
	pict.o \
	string.o \
	matrix.o \
	debug.o \
	error.o \
	copy.o \
	gc.o \
	j_code.o \
	big_int.o \
	big_int_ia.o \
	crypt.o \
	crypt_key.o \
	multipart.o \
	prof.o \
	gp_intp.o \
	interp1.o \
	interp2.o \
	parallel.o \
	prcsmgr.o \
	prog_if.o \
	sysfunc1.o \
	sysfunc2.o \
	sysfunc3.o \
	file.o \
	dllcall.o \
	advwnd.o \
	bitmap.o \
	shell.o \
	advcapp.o \
	advcvw.o \
	advcedit.o \
	mainfrm.o \
	ocx.o \
	olectrl.o \
	cui.o \
	project.o \
	class.o \
	member.o \
	body.o \
	body_in.o \
	body_out.o \
	sql.o \
	socket.o \
	xml.o \
	typedic.o \
	xslt.o \
	solid.o \
	jana.o \
	almath.o \
	parser_generator.o \
	pro_lisp.o \
	prolog.o \
	gdb.o \
	symbolic_manip.o \
	game_frwk.o
JUMAN_OBJ=\
	connect.o \
	getid.o \
	grammar.o \
	iotool.o \
	japanese.o \
	juman_lib.o \
	katuyou.o \
	lisp.o \
	pat.o \
	patfile.o
KNP_OBJ=\
	bnst_compare.o \
	configfile.o \
	db.o \
	declare.o \
	dpnd_analysis.o \
	feature.o \
	hash.o \
	koou.o \
	lib_bgh.o \
	lib_dt.o \
	lib_print.o \
	lib_ps.o \
	lib_scase.o \
	lib_sm.o \
	main.o \
	para_analysis.o \
	para_dpnd.o \
	para_relation.o \
	para_revision.o \
	proper.o \
	quote.o \
	read_data.o \
	read_rule.o \
	regexp.o \
	thesaurus.o \
	tools.o \
	tree_conv.o

$(TARGET): $(ALTAIR_OBJ) $(ALCORE_LIB) $(JUMAN_KNP_LIB)
	g++ $(LDFLAGS) -o $(TARGET) $(ALTAIR_OBJ) $(ALCORE_LIB)\
		$(ORACLE_LIB) $(POSTGRES_LIB) $(MYSQL_LIB) $(OPENSSL_LIB)

$(ALCORE_LIB): $(ALCORE_OBJ) $(JUMAN_KNP_LIB)
	g++ $(LDFLAGS) -shared -o $(ALCORE_LIB) $(ALCORE_OBJ) $(JUMAN_KNP_LIB)

$(JUMAN_KNP_LIB): $(JUMAN_OBJ) $(KNP_OBJ)
	g++ $(LDFLAGS) -shared -o $(JUMAN_KNP_LIB) $(JUMAN_OBJ) $(KNP_OBJ)

.cpp.o:
	g++ $(CPPFLAGS) -c $<

big_int_ia.o:	big_int_ia.nasm
	nasm -f elf64 big_int_ia.nasm

depend:
	makedepend $(INC_DIR) $(ALTAIR_SRC) $(ALCORE_SRC) $(JUMAN_SRC) $(KNP_SRC)

clean:
	rm -f $(ALTAIR_OBJ) $(ALCORE_OBJ) $(JUMAN_OBJ) $(KNP_OBJ)

othello:
	g++ $(CPPFLAGS) -c othello1.cpp
	g++ -shared -o othello.so othello1.o

igo:
	g++ $(CPPFLAGS) -c igo1.cpp
	g++ -shared -o igo.so igo1.o

shogi:
	g++ $(CPPFLAGS) -c shogi1.cpp
	g++ -shared -o shogi.so shogi1.o

tp:
	g++ -o tp $(INC_DIR) tp.cpp $(LDFLAGS)

asm.s:
	g++ $(CPPFLAGS) -S asm.cpp

######## dependency ########

