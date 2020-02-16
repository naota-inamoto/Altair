/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#include "stdafx.h"

#include <gdm/gdm.h>
#include <gdm/pict.h>
#include <gdm/string.h>
#include <interp/interp.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#ifdef	_LINUX
#include <stdio.h>
#include <dlfcn.h>
#endif

#define TYPE_VOID	0
#define TYPE_CHAR	1
#define TYPE_SHORT	2
#define TYPE_INT32	3
#define TYPE_INT64	4
#define TYPE_FLOAT	5
#define TYPE_DOUBLE 6
#define TYPE_PTR	7

extern "C" {
	typedef void (*voidProc)(...);
	typedef char (*charProc)(...);
	typedef short (*shortProc)(...);
#ifdef	_WINDOWS
	typedef long (*int32Proc)(...);
	typedef int (*int64Proc)(...);
#endif
#ifdef	_LINUX
	typedef int (*int32Proc)(...);
	typedef long (*int64Proc)(...);
#endif
	typedef float (*floatProc)(...);
	typedef double (*doubleProc)(...);
	typedef char* (*ptrProc)(...);
}

// DLL
int AIpProcess::EvalUseDll() {
	AGmNode *cmd, *arg1, *arg2, *arg3;
	GetArg(cmd, arg1, arg2, arg3);
	if (!IsString(cmd)) {
		Unref(cmd, arg1, arg2, arg3);
		return interp_error("UseDll()");
	}
	const char *s = cmd->a.s.v->Str();
	Unref(val); val = NULL;
	if (strcmp(s, "Call") == 0) {
#ifdef	_WINDOWS
		if (IsInteger(arg1) && arg1->a.i.v != 0) {
			FARPROC func = (FARPROC)arg1->a.i.v;

			int ret_type = TYPE_VOID;
			if (IsString(arg2)) {
				const char *s = arg2->a.s.v->Str();
				if (strcmp(s, "char") == 0)
					ret_type = TYPE_CHAR;
				else if (strcmp(s, "short") == 0)
					ret_type = TYPE_SHORT;
				else if (strcmp(s, "int") == 0)
					ret_type = TYPE_INT32;
				else if (strcmp(s, "long") == 0)
					ret_type = TYPE_INT64;
				else if (strcmp(s, "float") == 0)
					ret_type = TYPE_FLOAT;
				else if (strcmp(s, "double") == 0)
					ret_type = TYPE_DOUBLE;
				else if (strcmp(s, "ptr") == 0)
					ret_type = TYPE_PTR;
			}

			unsigned char char_val = 0;
			short short_val = 0;
			int int32_val = 0;
#if	_WINDOWS && _64BIT
			long long int64_val = 0;
#else
			long int64_val = 0;
#endif
			float float_val = 0;
			double double_val = 0;
			void* ptr_val = NULL;

#if	_WINDOWS && _64BIT
			long long a[32];
#else
			long a[32];
#endif
			void* out_float_ptr[32]; long out_float_ptr_idx = 0;

			int err = 0;

#if	_WINDOWS &&_64BIT
			long long *ptr = a;
#else
			long *ptr = a;
#endif
			float *float_ptr; double *double_ptr;

			AGmNode *l;
			for (l = arg3; l && IsList(l); l = l->T) {
				if (l->H == NULL || !IsString(l->HH)
						|| l->HT == NULL || !IsList(l->HT) || l->HTH == NULL) {
					err = 1; break;
				}
				const char *s = l->HH->a.s.v->Str();
				AGmNode *n = l->HTH;
				int type = TYPE_INT64;

				if (strcmp(s, "char") == 0) {
					if (IsInteger(n)) {
						type = TYPE_CHAR; char_val = (unsigned char)(n->a.i.v & 0xff);
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "short") == 0) {
					if (IsInteger(n)) {
						type = TYPE_SHORT; short_val = (short)(n->a.i.v & 0xffff);
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "int") == 0) {
					if (IsInteger(n)) {
						type = TYPE_INT32; int32_val = (int)n->a.i.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "long") == 0) {
					if (IsInteger(n)) {
						type = TYPE_INT64; int64_val = n->a.i.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "float") == 0) {
					if (IsReal(n)) {
						type = TYPE_FLOAT; float_val = (float)n->a.r.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "double") == 0) {
					if (IsReal(n)) {
						type = TYPE_DOUBLE; double_val = n->a.r.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "ptr") == 0) {
					if (IsInteger(n)) {
						type = TYPE_PTR; ptr_val = (void*)n->a.i.v;
					} else if (IsString(n)) {
						type = TYPE_PTR; ptr_val = (void*)n->a.s.v->Str();
					} else if (IsBinary(n)) {
						type = TYPE_PTR; ptr_val = n->a.bin.v->Buffer();
					} else if (IsList(n) && IsBinary(n->H) && IsInteger(n->TH)) {
						type = TYPE_PTR; ptr_val =
							n->H->a.bin.v->Buffer() + n->TH->a.i.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "char*") == 0) {
					if (IsInteger(n)) {
						n->a.i.v = 0;
						type = TYPE_PTR; ptr_val = &n->a.i.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "short*") == 0) {
					if (IsInteger(n)) {
						n->a.i.v = 0;
						type = TYPE_PTR; ptr_val = &n->a.i.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "int*") == 0) {
					if (IsInteger(n)) {
						n->a.i.v = 0;
						type = TYPE_PTR; ptr_val = &n->a.i.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "long*") == 0) {
					if (IsInteger(n)) {
						n->a.i.v = 0;
						type = TYPE_PTR; ptr_val = &n->a.i.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "float*") == 0) {
					if (IsReal(n)) {
						n->a.r.v = 0;
						type = TYPE_PTR; ptr_val = &n->a.r.v;
						out_float_ptr[out_float_ptr_idx++] = ptr_val;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "double*") == 0) {
					if (IsReal(n)) {
						n->a.r.v = 0;
						type = TYPE_PTR; ptr_val = &n->a.r.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "ptr*") == 0) {
					if (IsInteger(n)) {
						n->a.i.v = 0;
						type = TYPE_PTR; ptr_val = &n->a.i.v;
					} else {
						err = 1;
					}
				} else {
					err = 1; break;
				}

				switch (type) {
				case TYPE_CHAR:
					short_val = char_val;
				case TYPE_SHORT:
					int32_val = short_val;
				case TYPE_INT32:
					int64_val = int32_val;
				case TYPE_INT64:
					*ptr++ = int64_val;
					break;
#if	_WINDOWS && _64BIT
				case TYPE_FLOAT:
					float_ptr = (float*)ptr;
					*float_ptr++ = float_val;
					ptr = (long long*)float_ptr;
					break;
				case TYPE_DOUBLE:
					double_ptr = (double*)ptr;
					*double_ptr++ = double_val;
					ptr = (long long*)double_ptr;
					break;
				case TYPE_PTR:
					*ptr++ = (long long)ptr_val;
					break;
#else
				case TYPE_FLOAT:
					float_ptr = (float*)ptr;
					*float_ptr++ = float_val;
					ptr = (long*)float_ptr;
					break;
				case TYPE_DOUBLE:
					double_ptr = (double*)ptr;
					*double_ptr++ = double_val;
					ptr = (long*)double_ptr;
					break;
				case TYPE_PTR:
					*ptr++ = (long)ptr_val;
					break;
#endif
				}
			}
			if (err) {
				Unref(cmd, arg1, arg2, arg3);
				return interp_error("UseDll() illegal arg list");
			}

			voidProc voidFunc;
			charProc charFunc;
			shortProc shortFunc;
			int32Proc int32Func;
			int64Proc int64Func;
			floatProc floatFunc;
			doubleProc doubleFunc;
			ptrProc ptrFunc;

#ifdef	_64BIT
#else
			void *save_sp;
			_asm {
				push eax;
				push ebx;
				push ecx;
				push edx;
				push esi;
				push edi;
				mov save_sp, esp;
			}
#endif

			switch (ret_type) {
			case TYPE_VOID:
				voidFunc = (voidProc)func;
				(*voidFunc)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
					a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15],
					a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
					a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31]);
				break;
			case TYPE_CHAR:
				charFunc = (charProc)func;
				char_val = (*charFunc)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
					a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15],
					a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
					a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31]);
				val = Ref(Integer((long)char_val));
				break;
			case TYPE_SHORT:
				shortFunc = (shortProc)func;
				short_val = (*shortFunc)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
					a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15],
					a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
					a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31]);
				val = Ref(Integer((long)short_val));
				break;
			case TYPE_INT32:
				int32Func = (int32Proc)func;
				int32_val = (*int32Func)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
					a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15],
					a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
					a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31]);
				val = Ref(Integer(int32_val));
				break;
			case TYPE_INT64:
				int64Func = (int64Proc)func;
				int64_val = (*int64Func)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
					a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15],
					a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
					a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31]);
				val = Ref(Integer(int64_val));
				break;
			case TYPE_FLOAT:
				floatFunc = (floatProc)func;
				float_val = (*floatFunc)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
					a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15],
					a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
					a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31]);
				val = Ref(Real((double)float_val));
				break;
			case TYPE_DOUBLE:
				doubleFunc = (doubleProc)func;
				double_val = (*doubleFunc)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
					a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15],
					a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
					a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31]);
				val = Ref(Real(double_val));
				break;
			case TYPE_PTR:
				ptrFunc = (ptrProc)func;
				ptr_val = (*ptrFunc)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
					a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15],
					a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
					a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31]);
#if	_WINDOWS && _64BIT
				val = Ref(Integer((long long)ptr_val));
#else
				val = Ref(Integer((long)ptr_val));
#endif
				break;
			}

#ifdef	_64BIT
#else
			_asm {
				mov esp, save_sp;
				pop edi;
				pop esi;
				pop edx;
				pop ecx;
				pop ebx;
				pop eax;
			}
#endif
			for (int i = 0; i < out_float_ptr_idx; i++) {
				float_val = *((float*)out_float_ptr[i]);
				double_ptr = (double*)out_float_ptr[i];
				*double_ptr = float_val;
			}
		}
#endif
#ifdef	_LINUX
		if (IsInteger(arg1) && arg1->a.i.v != 0) {
			void* (*func)(...);
			func = (void*(*)(...))arg1->a.i.v;

			int ret_type = TYPE_VOID;
			if (IsString(arg2)) {
				const char *s = arg2->a.s.v->Str();
				if (strcmp(s, "char") == 0)
					ret_type = TYPE_CHAR;
				else if (strcmp(s, "short") == 0)
					ret_type = TYPE_SHORT;
				else if (strcmp(s, "int") == 0)
					ret_type = TYPE_INT32;
				else if (strcmp(s, "long") == 0)
					ret_type = TYPE_INT64;
				else if (strcmp(s, "float") == 0)
					ret_type = TYPE_FLOAT;
				else if (strcmp(s, "double") == 0)
					ret_type = TYPE_DOUBLE;
				else if (strcmp(s, "ptr") == 0)
					ret_type = TYPE_PTR;
			}

			unsigned char char_val; 
			short short_val;
			int int32_val;
			long int64_val;
			float float_val;
			double double_val;
			void* ptr_val;

			long a[32];
			void* out_float_ptr[32]; long out_float_ptr_idx = 0;

			int err = 0;

			long *ptr = a;
			float *float_ptr; double *double_ptr;

			AGmNode *l;
			for (l = arg3; l && IsList(l); l = l->T) {
				if (l->H == NULL || !IsString(l->HH)
						|| l->HT == NULL || !IsList(l->HT) || l->HTH == NULL) {
					err = 1; break;
				}
				const char *s = l->HH->a.s.v->Str();
				AGmNode *n = l->HTH;
				int type;

				if (strcmp(s, "char") == 0) {
					if (IsInteger(n)) {
						type = TYPE_CHAR; char_val = (unsigned char)(n->a.i.v & 0xff);
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "short") == 0) {
					if (IsInteger(n)) {
						type = TYPE_SHORT; short_val = (short)(n->a.i.v & 0xffff);
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "int") == 0) {
					if (IsInteger(n)) {
						type = TYPE_INT32; int32_val = n->a.i.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "long") == 0) {
					if (IsInteger(n)) {
						type = TYPE_INT64; int64_val = n->a.i.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "float") == 0) {
					if (IsReal(n)) {
						type = TYPE_FLOAT; float_val = (float)n->a.r.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "double") == 0) {
					if (IsReal(n)) {
						type = TYPE_DOUBLE; double_val = n->a.r.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "ptr") == 0) {
					if (IsInteger(n)) {
						type = TYPE_PTR; ptr_val = (void*)n->a.i.v;
					} else if (IsString(n)) {
						type = TYPE_PTR; ptr_val = (void*)n->a.s.v->Str();
					} else if (IsBinary(n)) {
						type = TYPE_PTR; ptr_val = n->a.bin.v->Buffer();
					} else if (IsList(n) && IsBinary(n->H) && IsInteger(n->TH)) {
						type = TYPE_PTR; ptr_val =
							n->H->a.bin.v->Buffer() + n->TH->a.i.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "char*") == 0) {
					if (IsInteger(n)) {
						n->a.i.v = 0;
						type = TYPE_PTR; ptr_val = &n->a.i.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "short*") == 0) {
					if (IsInteger(n)) {
						n->a.i.v = 0;
						type = TYPE_PTR; ptr_val = &n->a.i.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "int*") == 0) {
					if (IsInteger(n)) {
						n->a.i.v = 0;
						type = TYPE_PTR; ptr_val = &n->a.i.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "long*") == 0) {
					if (IsInteger(n)) {
						n->a.i.v = 0;
						type = TYPE_PTR; ptr_val = &n->a.i.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "float*") == 0) {
					if (IsReal(n)) {
						n->a.r.v = 0;
						type = TYPE_PTR; ptr_val = &n->a.r.v;
						out_float_ptr[out_float_ptr_idx++] = ptr_val;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "double*") == 0) {
					if (IsReal(n)) {
						n->a.r.v = 0;
						type = TYPE_PTR; ptr_val = &n->a.r.v;
					} else {
						err = 1; break;
					}
				} else if (strcmp(s, "ptr*") == 0) {
					if (IsInteger(n)) {
						n->a.i.v = 0;
						type = TYPE_PTR; ptr_val = &n->a.i.v;
					} else {
						err = 1;
					}
				} else {
					err = 1; break;
				}

				switch (type) {
				case TYPE_CHAR:
					short_val = char_val;
				case TYPE_SHORT:
					int32_val = short_val;
				case TYPE_INT32:
					int64_val = int32_val;
				case TYPE_INT64:
					*ptr++ = int64_val;
					break;
				case TYPE_FLOAT:
					float_ptr = (float*)ptr;
					*float_ptr++ = float_val;
					ptr = (long*)float_ptr;
					break;
				case TYPE_DOUBLE:
					double_ptr = (double*)ptr;
					*double_ptr++ = double_val;
					ptr = (long*)double_ptr;
					break;
				case TYPE_PTR:
#if	_WINDOWS && _64BIT
					*ptr++ = (long long)ptr_val;
#else
					*ptr++ = (long)ptr_val;
#endif
					break;
				}
			}
			if (err) {
				Unref(cmd, arg1, arg2, arg3);
				return interp_error("UseDll() illegal arg list");
			}

			voidProc voidFunc;
			charProc charFunc;
			shortProc shortFunc;
			int32Proc int32Func;
			int64Proc int64Func;
			floatProc floatFunc;
			doubleProc doubleFunc;
			ptrProc ptrFunc;

			/*
			void *save_sp;
			_asm {
				push eax;
				push ebx;
				push ecx;
				push edx;
				push esi;
				push edi;
				mov save_sp, esp;
			}
			*/

			switch (ret_type) {
			case TYPE_VOID:
				voidFunc = (voidProc)func;
				(*voidFunc)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
					a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15],
					a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
					a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31]);
				break;
			case TYPE_CHAR:
				charFunc = (charProc)func;
				char_val = (*charFunc)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
					a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15],
					a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
					a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31]);
				val = Ref(Integer((long)char_val));
				break;
			case TYPE_SHORT:
				shortFunc = (shortProc)func;
				short_val = (*shortFunc)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
					a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15],
					a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
					a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31]);
				val = Ref(Integer((long)short_val));
				break;
			case TYPE_INT32:
				int32Func = (int32Proc)func;
				int32_val = (*int32Func)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
					a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15],
					a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
					a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31]);
				val = Ref(Integer(int32_val));
				break;
			case TYPE_INT64:
				int64Func = (int64Proc)func;
				int64_val = (*int64Func)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
					a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15],
					a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
					a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31]);
				val = Ref(Integer(int64_val));
				break;
			case TYPE_FLOAT:
				floatFunc = (floatProc)func;
				float_val = (*floatFunc)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
					a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15],
					a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
					a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31]);
				val = Ref(Real((double)float_val));
				break;
			case TYPE_DOUBLE:
				doubleFunc = (doubleProc)func;
				double_val = (*doubleFunc)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
					a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15],
					a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
					a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31]);
				val = Ref(Real(double_val));
				break;
			case TYPE_PTR:
				ptrFunc = (ptrProc)func;
				ptr_val = (*ptrFunc)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
					a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15],
					a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
					a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31]);
#if	_WINDOWS && _64BIT
				val = Ref(Integer((long long)ptr_val));
#else
				val = Ref(Integer((long)ptr_val));
#endif
				break;
			}

			/*
			_asm {
				mov esp, save_sp;
				pop edi;
				pop esi;
				pop edx;
				pop ecx;
				pop ebx;
				pop eax;
			}
			*/

			for (int i = 0; i < out_float_ptr_idx; i++) {
				float_val = *((float*)out_float_ptr[i]);
				double_ptr = (double*)out_float_ptr[i];
				*double_ptr = float_val;
			}
		}
#endif
	} else if (strcmp(s, "GetProcAddress") == 0) {
#ifdef	_WINDOWS
		if (IsInteger(arg1) && arg1->a.i.v != 0 && IsString(arg2)) {
			FARPROC func;
			func = ::GetProcAddress((HMODULE)arg1->a.i.v, arg2->a.s.v->Str());
			if (func) {
#if	_WINDOWS && _64BIT
				val = Ref(Integer((long long)func));
#else
				val = Ref(Integer((long)func));
#endif
			}
		}
#endif
#ifdef	_LINUX
		if (IsInteger(arg1) && arg1->a.i.v != 0 && IsString(arg2)) {
			void* (*func)(...);
			func = (void* (*)(...))dlsym((void*)arg1->a.i.v, arg2->a.s.v->Str());
			char *error = dlerror();
			if (func && error == NULL)
				val = Ref(Integer((long)func));
		}
#endif
	} else if (strcmp(s, "LoadLibrary") == 0) {
#ifdef	_WINDOWS
		if (IsString(arg1)) {
			HMODULE hModule = ::LoadLibrary(arg1->a.s.v->Str());
			if (hModule)
#if	_64BIT
				val = Ref(Integer((long long)hModule));
#else
				val = Ref(Integer((long)hModule));
#endif
		}
#endif
#ifdef	_LINUX
		if (IsString(arg1)) {
			void* handle = dlopen(arg1->a.s.v->Str(), RTLD_LAZY);
			if (handle) {
				val = Ref(Integer((long)handle));
#ifdef	_LINUX_DEBUG
			} else {
				printf("dlerror = %s\n", dlerror());
#endif
			}
		}
#endif
	} else if (strcmp(s, "FreeLibrary") == 0) {
#ifdef	_WINDOWS
		if (IsInteger(arg1) && arg1->a.i.v != 0) {
			if (::FreeLibrary((HMODULE)arg1->a.i.v))
				val = Ref(Integer(1)); // success
		}
#endif
#ifdef	_LINUX
		if (IsInteger(arg1) && arg1->a.i.v != 0) {
			if (dlclose((void*)arg1->a.i.v) == 0)
				val = Ref(Integer(1)); // success
		}
#endif
	} else {
		Unref(cmd, arg1, arg2, arg3);
		return interp_error("UseDll()");
	}
	Unref(cmd, arg1, arg2, arg3);
	return 0;
}

