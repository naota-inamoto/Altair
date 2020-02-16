/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AGM_GDM_H
#define _AGM_GDM_H

// macro for duble linked list
#define AGM_INSERT(e,l) \
	{ e->prev = l->prev; e->next = l; \
	  l->prev->next = e; l->prev = e; }
#define AGM_REMOVE(e) \
	{ e->prev->next = e->next; \
	  e->next->prev = e->prev; }

class AGmString;
class AGmMatrix;
class AGmFile;
class AGmBinary;
class ADvWindow;
class ADvBitmap;
class ADvOcx;
class COleVariant;

class DBConnection;

#define AGM_NODE_LIST		0
#define AGM_NODE_STRING 	1
#define AGM_NODE_REAL		2
#define AGM_NODE_INTEGER	3
#define AGM_NODE_BIGINT 	4
#define AGM_NODE_GFP		5
#define AGM_NODE_ARRAY		6
#define AGM_NODE_MATRIX 	7
#define AGM_NODE_BINARY 	8
#define AGM_NODE_FILE		9
#define AGM_NODE_DST_ITR	10
#define AGM_NODE_SRC_ITR	11
#define AGM_NODE_OBJECT 	12
#define AGM_NODE_PTR		13
#define AGM_NODE_WINDOW 	14
#define AGM_NODE_BITMAP 	15
#define AGM_NODE_OCX		16
#define AGM_NODE_VARIANT	17
#define AGM_NODE_SQL_CONN	18
#define AGM_NODE_SQL_STAT	19
#define AGM_NODE_EMPTY		20

#define AGM_NODE_TYPE_BEGIN 	AGM_NODE_LIST
#define AGM_NODE_TYPE_END		AGM_NODE_EMPTY

#define AGM_BODY_OF_NODE	100
#define AGM_DMY_OUT_LINK	101
#define AGM_DMY_IN_LINK 	102
#define AGM_OUT_LINK		103
#define AGM_IN_LINK 		104
#define AGM_ARC 			105

#define AGM_ARC_ASSOC		0x0001

#define AGM_FLAG_DESTROY	0x01
#define AGM_FLAG_MARK		0x02
#define AGM_FLAG_SELF		0x04
#define AGM_FLAG_FREE_CELL	0x08
#define AGM_FLAG_USER_MARK	0x10
#define AGM_FLAG_REF		0x20
#define AGM_FLAG_CYCLIC 	0x40

#ifdef	_LINUX_ORACLE
class OCIStmt;
#endif

typedef struct	_AGmNode {
	char					type, flag;
#if	_WINDOWS && _64BIT
	long long				count;
#else
	long					count;
#endif
	struct	_AGmNodeBody*	body;
	union {
		struct	{
#if	_WINDOWS && _64BIT
			long long	v;
#else
			long	v;
#endif
			struct	_AGmNode*	t;
		} i;
		struct	{
			double	v;
		} r;
		struct	{
			struct	_AGmNode*	h;
			struct	_AGmNode*	t;
		} l;
		struct	{
			struct	_AGmNode**	v;
#if	_WINDOWS && _64BIT
			long long	size;
#else
			long	size;
#endif
		} a;
		struct	{
			struct	_AGmNode*	class_name;
			struct	_AGmNode*	class_id;
		} o;
		struct	{
			struct	_AGmNode*	node;
			struct	_AGmLink*	link;
		} itr;
		struct	{
			AGmString*	v;
			struct	_AGmNode*	t;
		} s;
		struct	{
			AGmMatrix*	v;
			struct	_AGmNode*	t;
		} m;
		struct	{
			AGmFile*	v;
			struct	_AGmNode*	t;
		} f;
		struct	{
			ADvWindow*	v;
			struct	_AGmNode*	t;
		} w;
		struct {
			ADvBitmap* v;
			struct	_AGmNode*	t;
		} b;
		struct {
#if	_WINDOWS && _64BIT
			long long	sign;
#else
			long	sign;
#endif
			AGmBinary*	bin;
		} bi;
		struct {
#if	_WINDOWS && _64BIT
			unsigned long long	v;
			unsigned long long	mod;
#else
			unsigned long	v;
			unsigned long	mod;
#endif
		} gp;
		struct	{
			void*	ptr;
			struct	_AGmNode*	t;
		} p;
		struct	{
			AGmBinary*	v;
			struct	_AGmNode*	t;
		} bin;
		struct	{
			ADvOcx* v;
		} ocx;
		struct	{
			COleVariant *v;
			int byref;
		} var;
		struct {
			DBConnection* impl;
		} conn;
		struct {
			DBConnection* impl;
		} stat;
	} a;
} AGmNode;

typedef struct	_AGmNodeBody {
	char				type, flag;
#if	_WINDOWS && _64BIT
	long long			extension;
	long long			nid;
#else
	long				extension;
	long				nid;
#endif
	struct	_AGmLink*	in;
	struct	_AGmLink*	out;
} AGmNodeBody;

typedef struct	_AGmLink {
	char				type, flag;
	long				count;
	struct	_AGmArc*	arc;
	struct	_AGmLink*	next;
	struct	_AGmLink*	prev;
} AGmLink;

typedef struct	_AGmArc {
	char				type, flag;
	long				incl_assoc;
	struct	_AGmNode*	src;
	struct	_AGmNode*	dst;
	struct	_AGmNode*	attr;
} AGmArc;

typedef union	_AGmCell {
	struct	_AGmNode		node;
	struct	_AGmNodeBody	node_body;
	struct	_AGmLink		link;
	struct	_AGmArc 		arc;
} AGmCell;

#define AGM_MEM_BLOCK_SIZE	3600

class AL_EXT_CLASS AGm {
public:
#if	_WINDOWS && _64BIT
	static	AGmNode*	Integer(long long, AGmNode *t=NULL);
#else
	static	AGmNode*	Integer(long, AGmNode *t=NULL);
#endif
	static	AGmNode*	Real(double);
	static	AGmNode*	Cons(AGmNode *h, AGmNode *t=NULL);
#if	_WINDOWS && _64BIT
	static	AGmNode*	Array(long long size);
#else
	static	AGmNode*	Array(long size);
#endif
	static	AGmNode*	Object(AGmNode *name, AGmNode *id=NULL);
	static	AGmNode*	DstItr(AGmNode*);
	static	AGmNode*	SrcItr(AGmNode*);
	static	AGmNode*	String(const char*, AGmNode *t=NULL);
	static	AGmNode*	String(AGmString*, AGmNode *t=NULL);
	static	AGmNode*	Matrix(AGmMatrix*, AGmNode *t=NULL);
	static	AGmNode*	File(AGmFile*, AGmNode *t=NULL);
	static	AGmNode*	Window(ADvWindow*, AGmNode *t=NULL);
	static	AGmNode*	Bitmap(ADvBitmap*, AGmNode *t=NULL);
	static	AGmNode*	PtrCell(void *ptr, AGmNode *t=NULL);
	static	AGmNode*	Binary(AGmBinary*, AGmNode *t=NULL);
	static	AGmNode*	BigInt(long, AGmBinary*);
#if	_WINDOWS && _64BIT
	static	AGmNode*	GFp(long long, long long);
#else
	static	AGmNode*	GFp(long, long);
#endif
	static	AGmNode*	Ocx(ADvOcx*);
	static	AGmNode*	Variant(COleVariant*, int byref=0);
	static	int IsInteger(const AGmNode*);
	static	int IsReal(const AGmNode*);
	static	int IsList(const AGmNode*);
	static	int IsArray(const AGmNode*);
	static	int IsObject(const AGmNode*);
	static	int IsDstItr(const AGmNode*);
	static	int IsSrcItr(const AGmNode*);
	static	int IsString(const AGmNode*);
	static	int IsMatrix(const AGmNode*);
	static	int IsFile(const AGmNode*);
	static	int IsWindow(const AGmNode*);
	static	int IsBitmap(const AGmNode*);
	static	int IsPtrCell(const AGmNode*);
	static	int IsBinary(const AGmNode*);
	static	int IsBigInt(const AGmNode*);
	static	int IsGFp(const AGmNode*);
	static	int IsNumber(const AGmNode*);
	static	int IsOcx(const AGmNode*);
	static	int IsVariant(const AGmNode*);
	static	int IsNum(const AGmNode*, double&);
	static	int IsStringList(const AGmNode*);

	static	AGmNode*	Ref(AGmNode*);
	static	int 	Unref(AGmNode*);
	static	void	Unref(AGmNode *n1, AGmNode *n2);
	static	void	Unref(AGmNode *n1, AGmNode *n2, AGmNode *n3);
	static	void	Unref(AGmNode *n1, AGmNode *n2, AGmNode *n3, AGmNode *n4);
	static	void	Unref(AGmNode *n1, AGmNode *n2, AGmNode *n3, AGmNode *n4,
			AGmNode *n5, AGmNode *n6=NULL, AGmNode *n7=NULL, AGmNode *n8=NULL);
	static	void	RefUnref(AGmNode *n1);
	static	void	RefUnref(AGmNode *n1, AGmNode *n2,
				AGmNode *n3=NULL, AGmNode *n4=NULL, AGmNode *n5=NULL, AGmNode *n6=NULL);
	static	void	ReleaseNode(AGmNode*);

	static	AGmArc* CreateArc(AGmNode *src, AGmNode *dst,
							AGmNode *s_attr=NULL, AGmLink *out=NULL, AGmLink *in=NULL,
							AGmLink **new_out=NULL, AGmLink **new_in=NULL);
	static	AGmArc* ReplaceArcSrc(AGmArc*, AGmNode*, AGmLink *src_link=NULL);
	static	AGmArc* ReplaceArcDst(AGmArc*, AGmNode*, AGmLink *dst_link=NULL);
	static	int 	DeleteArc(AGmArc*, int unref_dst = 1);
protected:
	static	int 	DeleteNode(AGmNode*);
	static	void	DeleteNodeSelf(AGmNode*); // for File I/O error
public:
	static	void	DeleteLoopRef(AGmNode*);
	static	int 	Refered(AGmNode*, int(*)(AGmLink*));	// whether refered form outside
	static	int 	Refer(AGmNode*, int(*)(AGmLink*));	// whether refer to outside
	static	void	MarkNode(AGmNode*, int flag);
	static	void	UnmarkNode(AGmNode*, int flag);
	static	void	AttachNid(AGmNode*);
	static	void	DetachNid(AGmNode*);
	static	AGmNode* GraphCopy(AGmNode*, int us=0);
protected:
	static	void	_GraphCopyNode(AGmNode*, int us);
	static	void	_GraphCopyArc(AGmNode*);
public:
	static	int 	Height(AGmNode*, AGmNode*);
protected:
	static	int 	HeightMark(AGmNode*, AGmNode*, AGmNode*);
	static	void	HeightUnmark(AGmNode*, AGmNode*, AGmNode*);
protected:
	static	long		nid;
	static	AGmNode**	ctbl;
	static	void	_DeleteLoopRef(AGmNode*);
	static	int 	_Refered(AGmNode*, int(*)(AGmLink*));
	static	int 	_Refer(AGmNode*, int(*)(AGmLink*));
public:
	static	int 	ReferedFromOutside(AGmLink*);
	static	int 	ReferToOutside(AGmLink*);
public:
	static	int 	Push(AGmNode*&, AGmNode*);
	static	void	Pop(AGmNode*&);
	static	void	Set(AGmNode*&, AGmNode*);
	static	void	Ins(AGmNode**, AGmNode*);
	static	AGmNode* Nconc(AGmNode*, AGmNode*);
	static	AGmNode* Append(AGmNode*, AGmNode*);
	static	AGmNode* Reverse(AGmNode*, AGmNode *y=NULL);
	static	int Length(AGmNode*);
	static	int Member(AGmNode*, AGmNode*);

	static	AGmNode*	itr_next(AGmNode *itr);
	static	AGmNode*	itr_next(AGmNode *itr, AGmNode *attr);
	static	AGmNode*	itr_next2(AGmNode *itr, AGmNode *addr);
	static	AGmNode*	itr_prev(AGmNode *itr);
	static	AGmNode*	itr_prev(AGmNode *itr, AGmNode *attr);
	static	AGmNode*	itr_prev2(AGmNode *itr, AGmNode *addr);
	static	int itr_component(AGmNode *itr, AGmNode*&);
	static	int itr_insert(AGmNode *itr, AGmNode *obj, AGmNode *attr);
	static	int itr_append(AGmNode *itr, AGmNode *obj, AGmNode *attr);
	static	int itr_remove(AGmNode *itr);

	static	AGmLink*	find_outlink(AGmNode *src, AGmNode *dst);
	static	AGmLink*	find_outlink(AGmNode *src, AGmNode *dst, AGmNode* attr);
	static	AGmLink*	find_inlink(AGmNode *src, AGmNode *dst);
	static	AGmLink*	find_inlink(AGmNode *src, AGmNode *dst, AGmNode* attr);
	static	AGmLink*	next_outlink(AGmNode*, AGmLink*);
	static	AGmLink*	next_outlink(AGmNode*, AGmLink*, AGmNode*);
	static	AGmLink*	next_outlink2(AGmNode*, AGmLink*, AGmNode*);
	static	AGmLink*	next_outlink3(AGmNode*, AGmLink*, AGmNode*);
	static	AGmLink*	next_inlink(AGmNode*, AGmLink*);
	static	AGmLink*	next_inlink(AGmNode*, AGmLink*, AGmNode*);
	static	AGmLink*	next_inlink2(AGmNode*, AGmLink*, AGmNode*);
	static	AGmLink*	next_inlink3(AGmNode*, AGmLink*, AGmNode*);
	static	AGmLink*	prev_outlink(AGmNode*, AGmLink*);
	static	AGmLink*	prev_outlink(AGmNode*, AGmLink*, AGmNode*);
	static	AGmLink*	prev_outlink2(AGmNode*, AGmLink*, AGmNode*);
	static	AGmLink*	prev_outlink3(AGmNode*, AGmLink*, AGmNode*);
	static	AGmLink*	prev_inlink(AGmNode*, AGmLink*);
	static	AGmLink*	prev_inlink(AGmNode*, AGmLink*, AGmNode*);
	static	AGmLink*	prev_inlink2(AGmNode*, AGmLink*, AGmNode*);
	static	AGmLink*	prev_inlink3(AGmNode*, AGmLink*, AGmNode*);
#if	_WINDOWS && _64BIT
	static	AGmLink*	next_outlink(AGmNode*, AGmLink*, long long);
	static	AGmLink*	next_inlink(AGmNode*, AGmLink*, long long);
	static	AGmLink*	prev_outlink(AGmNode*, AGmLink*, long long);
	static	AGmLink*	prev_inlink(AGmNode*, AGmLink*, long long);
#else
	static	AGmLink*	next_outlink(AGmNode*, AGmLink*, long);
	static	AGmLink*	next_inlink(AGmNode*, AGmLink*, long);
	static	AGmLink*	prev_outlink(AGmNode*, AGmLink*, long);
	static	AGmLink*	prev_inlink(AGmNode*, AGmLink*, long);
#endif
	static	AGmLink*	next_outlink(AGmNode*, AGmLink*, char*);
	static	AGmLink*	next_inlink(AGmNode*, AGmLink*, char*);
	static	AGmLink*	prev_outlink(AGmNode*, AGmLink*, char*);
	static	AGmLink*	prev_inlink(AGmNode*, AGmLink*, char*);
	static	AGmNode*	dst_node(AGmNode*);
	static	AGmNode*	dst_node(AGmNode*, AGmNode*);
	static	AGmNode*	dst_node(AGmNode*, const char*, int ignore_case=0);
#if	_WINDOWS && _64BIT
	static	AGmNode*	dst_node(AGmNode*, long long);
#else
	static	AGmNode*	dst_node(AGmNode*, long);
#endif
	static	AGmNode*	dst_node2(AGmNode*, AGmNode*);
	static	AGmNode*	dst_node3(AGmNode*, AGmNode*);
	static	AGmNode*	src_node(AGmNode*);
	static	AGmNode*	src_node(AGmNode*, AGmNode*);
	static	AGmNode*	src_node(AGmNode*, const char*, int ignore_case=0);
#if	_WINDOWS && _64BIT
	static	AGmNode*	src_node(AGmNode*, long long);
#else
	static	AGmNode*	src_node(AGmNode*, long);
#endif
	static	AGmNode*	src_node2(AGmNode*, AGmNode*);
	static	AGmNode*	src_node3(AGmNode*, AGmNode*);
	static	AGmArc* 	set_dst_node(AGmNode*, AGmNode*);
	static	AGmArc* 	set_dst_node(AGmNode*, AGmNode*, AGmNode*);
	static	AGmArc* 	set_dst_node(AGmNode*, const char*, AGmNode*);
#if	_WINDOWS && _64BIT
	static	AGmArc*		set_dst_node(AGmNode*, long long, AGmNode*);
#else
	static	AGmArc*		set_dst_node(AGmNode*, long, AGmNode*);
#endif
	static	AGmArc* 	set_dst_node2(AGmNode*, AGmNode*, AGmNode*);
	static	int 		out_count(AGmNode*);
	static	int 		out_count(AGmNode*, AGmNode*);
	static	int 		in_count(AGmNode*);
	static	int 		in_count(AGmNode*, AGmNode*);

public:
	static int Compare(AGmNode*, AGmNode*);
	static long ListLength(AGmNode*);
	static AGmNode* ListCopy(AGmNode*, int us=0);
	static AGmNode* ShallowCopy(AGmNode*);

	static AGmNode* Sort(AGmNode*, int desc);
	static AGmNode* MaxAttr(AGmNode*);
	static AGmNode* MinAttr(AGmNode*);

public:
	static AGmNode* List(AGmNode*);
	static AGmNode* List(AGmNode*, AGmNode*);
	static AGmNode* List(AGmNode*, AGmNode*, AGmNode*);
	static AGmNode* List(AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	static AGmNode* List(AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	static AGmNode* List(AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	static AGmNode* List(AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*);
	static AGmNode* List(AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*, AGmNode*);

#if	_WINDOWS && _64BIT
	static AGmNode* List(long long, AGmNode*t=NULL);
	static AGmNode* List(long long, long long, AGmNode*t=NULL);
	static AGmNode* List(long long, long long, long long, AGmNode*t=NULL);
	static AGmNode* List(long long, long long, long long, long long, AGmNode*t=NULL);
	static AGmNode* List(long long, long long, long long, long long, long long, AGmNode*t=NULL);
	static AGmNode* List(long long, long long, long long, long long, long long, long long, AGmNode*t=NULL);
#else
	static AGmNode* List(long, AGmNode*t=NULL);
	static AGmNode* List(long, long, AGmNode*t=NULL);
	static AGmNode* List(long, long, long, AGmNode*t=NULL);
	static AGmNode* List(long, long, long, long, AGmNode*t=NULL);
	static AGmNode* List(long, long, long, long, long, AGmNode*t=NULL);
	static AGmNode* List(long, long, long, long, long, long, AGmNode*t=NULL);
#endif

	static AGmNode* List(double, AGmNode*t=NULL);
	static AGmNode* List(double, double, AGmNode*t=NULL);
	static AGmNode* List(double, double, double, AGmNode*t=NULL);
	static AGmNode* List(double, double, double, double, AGmNode*t=NULL);

	static AGmNode* List(const char*, AGmNode*t=NULL);

#if	_WINDOWS && _64BIT
	static AGmNode* List(const char*, long long, AGmNode*t=NULL);
	static AGmNode* List(const char*, long long, long long, AGmNode*t=NULL);
	static AGmNode* List(const char*, long long, long long, long long, AGmNode*t=NULL);
#else
	static AGmNode* List(const char*, long, AGmNode*t=NULL);
	static AGmNode* List(const char*, long, long, AGmNode*t=NULL);
	static AGmNode* List(const char*, long, long, long, AGmNode*t=NULL);
#endif

	static AGmCell* Alloc();
	static void Free(AGmCell*);

public:
	static long MaxHeapNum();
	static long CurrHeapNum();
	static long HeapBlockSize();
	static int MaxHeapNum(long);

	static void RefMemorySystem();
	static void UnrefMemorySystem();
	static long MemorySize(AGmNode *);

private:
	static long max_heap_num;
	static long curr_heap_num;

	static AGmCell *_mem_block_chain;
	static AGmCell *_mem_block;
	static unsigned short _mem_index;
	static AGmCell *_mem_free;
	static long _allocated_max_id;
	static AGmNode* _free_id_list;

	static	void ReleaseMemory();
	static	void _MemorySize(AGmNode *, long&);
	static	long _mem_system_ref_count;
	
public:
#if	_WINDOWS && _64BIT
	static long long GetUniqueID();
	static int	ReleaseUniqueID(long long);
#else
	static long GetUniqueID();
	static int	ReleaseUniqueID(long);
#endif

public:
	static int Param(AGmNode* args,
					AGmNode*& arg1, AGmNode*& tail);

#if	_WINDOWS && _64BIT
	static int Param(AGmNode* args,
					long long& arg1, AGmNode*& tail);
	static int Param(AGmNode* args,
					long long& arg1, long long& arg2, AGmNode*& tail);
	static int Param(AGmNode* args,
					long long& arg1, long long& arg2, long long& arg3, AGmNode*& tail);
	static int Param(AGmNode* args,
					long long& arg1, long long& arg2, long long& arg3, long long& arg4, AGmNode*& tail);
	static int Param(AGmNode* args,
					long long& arg1, long long& arg2, long long& arg3, long long& arg4, long long& arg5, AGmNode*& tail);
	static int Param(AGmNode* args,
					long long& arg1, long long& arg2, long long& arg3, long long& arg4, long long& arg5, long long& arg6, AGmNode*& tail);
#else
	static int Param(AGmNode* args,
					long& arg1, AGmNode*& tail);
	static int Param(AGmNode* args,
					long& arg1, long& arg2, AGmNode*& tail);
	static int Param(AGmNode* args,
					long& arg1, long& arg2, long& arg3, AGmNode*& tail);
	static int Param(AGmNode* args,
					long& arg1, long& arg2, long& arg3, long& arg4, AGmNode*& tail);
	static int Param(AGmNode* args,
					long& arg1, long& arg2, long& arg3, long& arg4, long& arg5, AGmNode*& tail);
	static int Param(AGmNode* args,
					long& arg1, long& arg2, long& arg3, long& arg4, long& arg5, long& arg6, AGmNode*& tail);
#endif

	static int Param(AGmNode* args,
					double& arg1, AGmNode*& tail);
	static int Param(AGmNode* args,
					double& arg1, double& arg2, AGmNode*& tail);
	static int Param(AGmNode* args,
					double& arg1, double& arg2, double& arg3, AGmNode*& tail);
	static int Param(AGmNode* args,
					double& arg1, double& arg2, double& arg3, double& arg4, AGmNode*& tail);

	static int Param(AGmNode* args,
					char*& arg1, AGmNode*& tail);

	static int Param(AGmNode* args,
					AGmMatrix*& arg1, AGmNode*& tail);

	static int Param(AGmNode* args,
					AGmBinary*& arg1, AGmNode*& tail);

public:
	static AGmNode* UniqueStr(const char*);
	static AGmNode* UniqueStr(AGmString*);
	static AGmNode* UniqueStr(AGmNode*);
protected:
	static int		InitUniqueStrTbl();
	static AGmNode* SearchSortCell(AGmNode*& ptr, const char* key);
public:
	static void 	ReduceUniqueStrTbl();
	static void 	ReduceSortCell(AGmNode*& ptr);
	static void 	MakeStrUnique(AGmNode*);
protected:
	static void 	_MakeStrUnique(AGmNode*);
public:
	static	int SetValue(void* ptr, long key, void* value);
	static	int GetValue(void* ptr, long key, void*& value);
	static	int RemoveValue(void* ptr, long key);
private:
	static	AGmNode *_values;

public:
	static	AGmNode*	Root();
	static	AGmNode*	RootWindowNode();
	static	AGmNode*	RootGlobal();
	static	AGmNode*	DocTmpl();

public:
	static AGmNode* NmClass();
	static AGmNode* NmSubClass();
	static AGmNode* NmMember();
	static AGmNode* NmType();
	static AGmNode* NmVtbl();
	static AGmNode* NmThis();

	static AGmNode* NmWindow();
	static AGmNode* NmDocTmpl();
	static AGmNode* NmGlobal();
	static AGmNode* NmPen();
	static AGmNode* NmBrush();
	static AGmNode* NmFont();
	static AGmNode* NmPipe();
	static AGmNode* NmEvent();
	static AGmNode* NmO();
	static AGmNode* NmArg();
	static AGmNode* NmProf();

	static AGmNode* NmProLisp();
	static AGmNode* NmProlog();
	static AGmNode*	NmGame();

	static AGmNode *minus_one;
	static AGmNode *zero;
	static AGmNode *one;
	static AGmNode *two;
	static AGmNode *three;
	static AGmNode *four;
	static AGmNode *five;
	static AGmNode *six;
	static AGmNode *seven;
	static AGmNode *eight;
	static AGmNode *nine;
	static AGmNode *ten;

private:
	static int _init_u_str_tbl;
	static AGmNode *_u_str_tbl;
	static AGmNode *_class;
	static AGmNode *_subclass;
	static AGmNode *_member;
	static AGmNode *_type;
	static AGmNode *_vtbl;
	static AGmNode *_this;

	static AGmNode *_window;
	static AGmNode *_doc_tmpl;
	static AGmNode *_global;
	static AGmNode *_pen;
	static AGmNode *_brush;
	static AGmNode *_font;
	static AGmNode *_pipeStr;
	static AGmNode *_event;
	static AGmNode *_o;
	static AGmNode *_arg;
	static AGmNode *_prof;

	static AGmNode *_pro_lisp;
	static AGmNode *_prolog;
	static AGmNode *_game;

public:
	static void SetToClipboard(AGmNode*);
	static AGmNode* GetFromClipboard();
	static void ReleaseClipboard();
	static void SetToClipboard2(AGmNode*);
	static AGmNode* GetFromClipboard2();
	static void ReleaseClipboard2();

public:
#if	_WINDOWS && _64BIT
	static void GC(long long *num_new_free=NULL, long long *num_free=NULL, long long *num_use=NULL, int debug=0);
	static void GCcyclicOnly(long long *num_new_free=NULL, long long *num_free=NULL, long long *num_use=NULL, int debug=0);
	static void GCtestRefCount(long long *ref_count_zero, long long *ref_count_minus, long long *num_new_free,
		long long *num_free=NULL, long long *num_use=NULL, int debug=0);
	static void GCtestCyclic(long long *not_cyclic, long long *cyclic, long long *num_new_free,
		long long *num_free=NULL, long long *num_use=NULL, int debug=0);
	static void _GCmark(AGmNode*);
private:
	static void GCmark();
	static void _GCscan(int, long long *a1, long long  *a2, long long  *num_new_free,
		long long  *num_free, long long  *num_use, int debug);
	static void _Recycle(AGmNode*);
	static void _GCmark2(AGmNode*, int);
#else
	static void GC(long *num_new_free=NULL, long *num_free=NULL, long *num_use=NULL, int debug=0);
	static void GCcyclicOnly(long *num_new_free=NULL, long *num_free=NULL, long *num_use=NULL, int debug=0);
	static void GCtestRefCount(long *ref_count_zero, long *ref_count_minus, long *num_new_free,
		long *num_free=NULL, long *num_use=NULL, int debug=0);
	static void GCtestCyclic(long *not_cyclic, long *cyclic, long *num_new_free,
		long *num_free=NULL, long *num_use=NULL, int debug=0);
	static void _GCmark(AGmNode*);
private:
	static void GCmark();
	static void _GCscan(int, long *a1, long *a2, long *num_new_free,
		long *num_free, long *num_use, int debug);
	static void _Recycle(AGmNode*);
	static void _GCmark2(AGmNode*, int);
#endif
};

#define H		a.l.h
#define T		a.l.t
#define HH		a.l.h->a.l.h
#define HT		a.l.h->a.l.t
#define TH		a.l.t->a.l.h
#define TT		a.l.t->a.l.t
#define HHH 	a.l.h->a.l.h->a.l.h
#define HHT 	a.l.h->a.l.h->a.l.t
#define HTH 	a.l.h->a.l.t->a.l.h
#define HTT 	a.l.h->a.l.t->a.l.t
#define THH 	a.l.t->a.l.h->a.l.h
#define THT 	a.l.t->a.l.h->a.l.t
#define TTH 	a.l.t->a.l.t->a.l.h
#define TTT 	a.l.t->a.l.t->a.l.t
#define HHHH	a.l.h->a.l.h->a.l.h->a.l.h
#define HHHT	a.l.h->a.l.h->a.l.h->a.l.t
#define HHTH	a.l.h->a.l.h->a.l.t->a.l.h
#define HHTT	a.l.h->a.l.h->a.l.t->a.l.t
#define HTHH	a.l.h->a.l.t->a.l.h->a.l.h
#define HTHT	a.l.h->a.l.t->a.l.h->a.l.t
#define HTTH	a.l.h->a.l.t->a.l.t->a.l.h
#define HTTT	a.l.h->a.l.t->a.l.t->a.l.t
#define THHH	a.l.t->a.l.h->a.l.h->a.l.h
#define THHT	a.l.t->a.l.h->a.l.h->a.l.t
#define THTH	a.l.t->a.l.h->a.l.t->a.l.h
#define THTT	a.l.t->a.l.h->a.l.t->a.l.t
#define TTHH	a.l.t->a.l.t->a.l.h->a.l.h
#define TTHT	a.l.t->a.l.t->a.l.h->a.l.t
#define TTTH	a.l.t->a.l.t->a.l.t->a.l.h
#define TTTT	a.l.t->a.l.t->a.l.t->a.l.t
#define TTTHH	a.l.t->a.l.t->a.l.t->a.l.h->a.l.h
#define TTTHT	a.l.t->a.l.t->a.l.t->a.l.h->a.l.t
#define TTTTH	a.l.t->a.l.t->a.l.t->a.l.t->a.l.h
#define TTTTT	a.l.t->a.l.t->a.l.t->a.l.t->a.l.t
#define TTTTTH	a.l.t->a.l.t->a.l.t->a.l.t->a.l.t->a.l.h

#define I		a.i.v
#define HI		a.l.h->a.i.v
#define TI		a.l.t->a.i.v
#define HHI 	a.l.h->a.l.h->a.i.v
#define HTI 	a.l.h->a.l.t->a.i.v
#define THI 	a.l.t->a.l.h->a.i.v
#define TTI 	a.l.t->a.l.t->a.i.v
#define HHHI	a.l.h->a.l.h->a.l.h->a.i.v
#define HHTI	a.l.h->a.l.h->a.l.t->a.i.v
#define HTHI	a.l.h->a.l.t->a.l.h->a.i.v
#define HTTI	a.l.h->a.l.t->a.l.t->a.i.v
#define THHI	a.l.t->a.l.h->a.l.h->a.i.v
#define THTI	a.l.t->a.l.h->a.l.t->a.i.v
#define TTHI	a.l.t->a.l.t->a.l.h->a.i.v
#define TTTI	a.l.t->a.l.t->a.l.t->a.i.v
#define TTTHI	a.l.t->a.l.t->a.l.t->a.l.h->a.i.v
#define TTTTHI	a.l.t->a.l.t->a.l.t->a.l.t->a.l.h->a.i.v
#define TTTTTHI a.l.t->a.l.t->a.l.t->a.l.t->a.l.t->a.l.h->a.i.v

#define HR		a.l.h->a.r.v
#define THR 	a.l.t->a.l.h->a.r.v
#define TTHR	a.l.t->a.l.t->a.l.h->a.r.v
#define TTTHR	a.l.t->a.l.t->a.l.t->a.l.h->a.r.v
#define TTTTHR	a.l.t->a.l.t->a.l.t->a.l.t->a.l.h->a.r.v

#endif	/* _AGM_GDM_H */

