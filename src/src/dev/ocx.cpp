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
#include <gdm/vmkey.h>
#include <gdm/pict.h>
#include <interp/interp.h>
#include <dev/advcapp.h>
#include <dev/advcvw.h>
#include <dev/advwnd.h>
#include <dev/bitmap.h>
#include <dev/ocx.h>
#include <dev/olectrl.h>
#include <altair/strres.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

int AIpProcess::EvalOcx() {
	AGmNode *cmd, *arg1, *arg2, *arg3;
	GetArg(cmd, arg1, arg2, arg3);
#ifdef	_WINDOWS
	USES_CONVERSION;
#endif
	if (!IsString(cmd)) {
		Unref(cmd, arg1, arg2, arg3);
		return interp_error("OCX()");
	}
#ifdef	_WINDOWS
	const char *s = cmd->a.s.v->Str();
	if (strcmp(s, "is_created") == 0) {
		if (!IsOcx(arg1)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() created");
		}
		ADvOcx *ocx = arg1->a.ocx.v;
		Unref(val); val = NULL;
		if (ocx->Ctrl()) val = Ref(Integer(1));

	} else if (strcmp(s, "place") == 0) {
		double x, y, w, h;
		if (!IsOcx(arg1) ||
				!IsNum(arg2->H, x) ||
				!IsNum(arg2->TH, y) ||
				!IsNum(arg2->TTH, w) ||
				!IsNum(arg2->TTTH, h)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() place");
		}
		ADvOcx *ocx = arg1->a.ocx.v;
		Unref(val); val = NULL;
		int err = ocx->Place((int)x, (int)y, (int)w, (int)h);
		if (err) val = Ref(Integer((long)err));

	} else if (strcmp(s, "object") == 0) {
		if (!IsOcx(arg1)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() object");
		}
		ADvOcx *ocx = arg1->a.ocx.v;
		IDispatch *pdisp = ocx->GetDispatch();
		if (pdisp == NULL) {
			Unref(val); val = NULL;
			Unref(cmd, arg1, arg2, arg3);
			return 0;
		}
		Unref(val);
		VARIANT v;
		VariantInit(&v);
		V_VT(&v) = VT_DISPATCH;
		V_DISPATCH(&v) = pdisp;
		val = Ref(Variant(new COleVariant(v)));
		pdisp->Release();

	} else if (strcmp(s, "Picture") == 0) {
		if (!IsString(arg1) || !IsString(arg2)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() Picture");
		}
		const char *dir = arg1->a.s.v->Str();
		const char *name = arg2->a.s.v->Str();
		COleVariant *var = ADvOcx::LoadPicture(dir, name);
		Unref(val);
		val = Ref(Variant(var));

	} else if (strcmp(s, "Font") == 0) {
		if (!IsString(arg1) || !IsString(arg2)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() Font");
		}
		const char *name = arg1->a.s.v->Str();
#if	_WINDOWS && _64BIT
		long long size = arg2->a.i.v;
		long long bold, italic, underline, strike; AGmNode *tail;
#else
		long size = arg2->a.i.v;
		long bold, italic, underline, strike; AGmNode *tail;
#endif
		bold = italic = underline = strike = 0;
		if (arg3) {
			if (Param(arg3, bold, italic, underline, strike, tail)) {
				Unref(cmd, arg1, arg2, arg3);
				return interp_error("OCX() Font");
			}
		}
		COleVariant *var = ADvOcx::CreateFont(name, (int)size,
			(int)bold, (int)italic, (int)underline, (int)strike);
		Unref(val);
		val = Ref(Variant(var));
	
	} else if (strcmp(s, "pre_create") == 0) {
		if (!IsString(arg1) || !IsString(arg3)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() pre_create");
		}
		const char *progID = arg1->a.s.v->Str();
		int visible = (arg2 ? 1 : 0);
		const char *SubStgID = arg3->a.s.v->Str();
		ADvOcx *ocx = new ADvOcx(progID, visible, SubStgID);
		Unref(val);
		val = Ref(Ocx(ocx));

	} else if (strcmp(s, "create") == 0) {
		if (!IsOcx(arg1) || !IsWindow(arg2)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() create");
		}
		ADvOcx *ocx = arg1->a.ocx.v;
		ADvWindow *w = arg2->a.w.v;
		Unref(val); val = NULL;
		int err = ocx->Create(w);
		if (err) val = Ref(Integer((long)err));

	} else if (strcmp(s, "destroy") == 0) {
		if (!IsOcx(arg1)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() destroy");
		}
		ADvOcx *ocx = arg1->a.ocx.v;
		ADvOcxWnd *ctrl = ocx->Ctrl();
		if (ctrl) delete ctrl;
		ocx->FileName(NULL);

	} else if (strcmp(s, "progID") == 0) {
		if (!IsOcx(arg1)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() progID");
		}
		ADvOcx *ocx = arg1->a.ocx.v;
		Unref(val);
		val = Ref(String(ocx->ProgID()->Str()));

	} else if (strcmp(s, "subStgID") == 0) {
		if (!IsOcx(arg1)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() subStgID");
		}
		ADvOcx *ocx = arg1->a.ocx.v;
		Unref(val);
		val = Ref(String(ocx->SubStgID()->Str()));

	} else if (strcmp(s, "visible") == 0) {
		if (!IsOcx(arg1) || !IsInteger(arg2)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() visible");
		}
		ADvOcx *ocx = arg1->a.ocx.v;
		Unref(val); val = NULL;
		ocx->Visible((int)arg2->HI);
	
	} else if (strcmp(s, "runtime_invisible") == 0) {
		if (!IsOcx(arg1)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() runtime_invisible");
		}
		ADvOcx *ocx = arg1->a.ocx.v;
		Unref(val); val = NULL;
		if (ocx->RuntimeInvisible())
			val = Ref(Integer(1));

	} else if (strcmp(s, "mode") == 0) {
		if (!IsOcx(arg1)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() mode");
		}
		ADvOcx *ocx = arg1->a.ocx.v;
		ocx->Mode(arg2 ? 1 : 0);

	} else if (strcmp(s, "prop_editor") == 0) {
		if (!IsOcx(arg1)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() prop_editor");
		}
		ADvOcx *ocx = arg1->a.ocx.v;
		ocx->PropertyEditor();
	
	} else if (strcmp(s, "get_filename") == 0) {
		if (!IsOcx(arg1)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() get_filename");
		}
		ADvOcx *ocx = arg1->a.ocx.v;
		Unref(val); val = NULL;
		if (ocx->FileName())
			val = Ref(String(ocx->FileName()));

	} else if (strcmp(s, "set_filename") == 0) {
		if (!IsOcx(arg1) || !IsString(arg2)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() set_filename");
		}
		ADvOcx *ocx = arg1->a.ocx.v;
		ocx->FileName(arg2->a.s.v->Str());

	} else if (strcmp(s, "save") == 0) {
		if (!IsOcx(arg1) || !arg1->a.ocx.v->FileName()) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() save");
		}
		ADvOcx *ocx = arg1->a.ocx.v;
		const char *filename = ocx->FileName();
		Unref(val); val = NULL;
		IStorage *pStg;
		unsigned long mode = STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_DIRECT;
		HRESULT hr = StgOpenStorage(T2OLE(filename), NULL, mode, NULL, 0, &pStg);
		if (FAILED(hr)) {
			mode = STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_DIRECT;
			hr = StgCreateDocfile(T2OLE(filename), mode, 0, &pStg);
		}
		if (FAILED(hr)) {
			val = Ref(Integer(10));
		} else {
			int err = ocx->Save(pStg);
			pStg->Release();
			if (err) val = Ref(Integer((long)err));
		}

	} else if (strcmp(s, "load") == 0) {
		if (!IsOcx(arg1) || !arg1->a.ocx.v->FileName()) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() load");
		}
		ADvOcx *ocx = arg1->a.ocx.v;
		const char *filename = ocx->FileName();
		Unref(val); val = NULL;
		IStorage* pStg;
		unsigned long mode = STGM_READ|STGM_SHARE_DENY_WRITE|STGM_DIRECT;
		HRESULT hr = StgOpenStorage(T2OLE(filename), NULL, mode, NULL, 0, &pStg);
		if (FAILED(hr)) {
			val = Ref(Integer(10));
		} else {
			int err = ocx->Load(pStg);
			pStg->Release();
			if (err) val = Ref(Integer((long)err));
		}

	} else if (strcmp(s, "callback") == 0) {
		if (!IsOcx(arg1) || !IsString(arg2) ||
				!arg3 || !IsList(arg3) || !arg3->H ||
				!arg3->T || !IsList(arg3->T) || !arg3->TH ||
				!arg3->TT || !IsList(arg3->TT) || !arg3->TTH) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() callback");
		}
		ADvOcx *ocx = arg1->a.ocx.v;
		const char *event_name = arg2->a.s.v->Str();
		AGmNode *info = arg3;

		if (!ocx || !ocx->Parent()) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() callback");
		}

		AGmNode *nID = Ref(Integer((long)ocx->nID()));
		AGmNode *tbl;
		if (ocx->Parent()->View()) {
			tbl = dst_node(
				ocx->Parent()->View()->CallbackTbl(), nID);
			if (tbl == NULL) {
				CreateArc(ocx->Parent()->View()->CallbackTbl(),
					(tbl = Cons(NULL)), nID);
			}
		} else if (ocx->Parent()->OleCtrl()) {
			tbl = dst_node(
				ocx->Parent()->OleCtrl()->CallbackTbl(), nID);
			if (tbl == NULL) {
				CreateArc(ocx->Parent()->OleCtrl()->CallbackTbl(),
					(tbl = Cons(NULL)), nID);
			}
		} else {
			Unref(nID);
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() callback");
		}
		Unref(nID);

		DISPID memid = 0; int ok = 0;
		ITypeInfo *pClassInfo;
		for (;;) {
			pClassInfo = ocx->GetClassInfo();
			if (pClassInfo == NULL) break;
			LPTYPEATTR pClassAttr;
			HRESULT hr = pClassInfo->GetTypeAttr(&pClassAttr);
			if (FAILED(hr)) break;
			for (int i = 0; i < pClassAttr->cImplTypes; i++) {
				HREFTYPE hRefType;
				LPTYPEINFO pTypeInfo;
				hr = pClassInfo->GetRefTypeOfImplType(i, &hRefType);
				if (FAILED(hr)) continue;
				hr = pClassInfo->GetRefTypeInfo(hRefType, &pTypeInfo);
				if (FAILED(hr)) continue;
				LPTYPEATTR pTypeAttr;
				hr = pTypeInfo->GetTypeAttr(&pTypeAttr);
				if (FAILED(hr)) {
					pTypeInfo->Release();
					continue;
				}
				LPFUNCDESC pFuncDesc;
				for (long n = 0; n < pTypeAttr->cFuncs; n++) {
					hr = pTypeInfo->GetFuncDesc(n, &pFuncDesc);
					if (FAILED(hr)) continue;
					BSTR bstr; unsigned int cNames;
					hr = pTypeInfo->GetNames(pFuncDesc->memid, &bstr, 1, &cNames);
					if (FAILED(hr) || cNames < 1) {
						pTypeInfo->ReleaseFuncDesc(pFuncDesc);
						continue;
					}
					if (strcmp(OLE2T(bstr), event_name) == 0) {
						memid = pFuncDesc->memid;
						ok = 1;
					}
					SysFreeString(bstr);
					pTypeInfo->ReleaseFuncDesc(pFuncDesc);
					if (ok) break;
				}
				pTypeInfo->ReleaseTypeAttr(pTypeAttr);
				pTypeInfo->Release();
				if (ok) break;
			}
			pClassInfo->ReleaseTypeAttr(pClassAttr);
			break;
		}
		if (pClassInfo) pClassInfo->Release();
		if (ok == 0) {
			Unref(val); val = Ref(Integer(1));
		} else {
			set_dst_node(tbl, Integer((long)memid), info);
			Unref(val); val = NULL;
		}

	} else if (strcmp(s, "unset_callback") == 0) {
		if (!IsOcx(arg1)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() unset_callback");
		}
		ADvOcx *ocx = arg1->a.ocx.v;
		AGmNode *nID = Ref(Integer((long)ocx->nID()));
		if (ocx->Parent()) {
			if (ocx->Parent()->View()) {
				set_dst_node(
					ocx->Parent()->View()->CallbackTbl(), nID, NULL);
			} else if (ocx->Parent()->OleCtrl()) {
				set_dst_node(
					ocx->Parent()->OleCtrl()->CallbackTbl(), nID, NULL);
			} else {
				Unref(nID);
				Unref(cmd, arg1, arg2, arg3);
				return interp_error("OCX() unset_callback");
			}
		}
		Unref(nID);

	} else if (strcmp(s, "ocx_list") == 0) {
		Unref(val);
		val = Ref(ADvOcx::ProgIDList());

	} else if (strcmp(s, "wnd") == 0) {
		if (!IsOcx(arg1)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() wnd");
		}
		Unref(val); val = NULL;
		ADvOcx *ocx; ADvWindow *aw;
		if ((ocx = arg1->a.ocx.v) != NULL && (aw = ocx->Parent()) != NULL) {
			void *value = NULL;
			if (aw->View()) {
				GetValue(aw->View(), AGM_VM_KEY_WINDOW_NODE, value);
			} else if (aw->OleCtrl()) {
				GetValue(aw->OleCtrl(), AGM_VM_KEY_WINDOW_NODE, value);
			}
			val = Ref((AGmNode*)value);
		}

	} else if (strcmp(s, "z_order") == 0) {
		if (!IsOcx(arg1)) {
			Unref(cmd, arg1, arg2, arg3);
			return interp_error("OCX() z_order");
		}
		ADvOcx *ocx = arg1->a.ocx.v;
		if (ocx && ocx->Ctrl()) {
			if (arg2) {
				ocx->Ctrl()->SetWindowPos(&CWnd::wndTop, 0, 0, 1, 1,
					SWP_NOMOVE|SWP_NOSIZE);
			} else {
				ocx->Ctrl()->SetWindowPos(&CWnd::wndBottom, 0, 0, 1, 1,
					SWP_NOMOVE|SWP_NOSIZE);
			}
		}

	} else {
		Unref(cmd, arg1, arg2, arg3);
		return interp_error("OCX()");
	}
#endif
	Unref(cmd, arg1, arg2, arg3);
	return 0;
}

int AIpProcess::EvalOcxInvoke() {
#ifdef	_WINDOWS
	const char *method = prog->a.i.t->H->a.s.v->Str();
	long num_arg = (long)prog->a.i.t->T->a.i.v;
	BYTE param[33];
	COleVariant* args[32];
	char err[256];
	if (num_arg > 32) {
		art_sprintf_1(err, 256, AL_O2, method);
		return interp_error(err);
	}
	int i;
	for (i = 0; i < num_arg; i++) {
		if (!IsVariant(val) || val->a.var.byref == 0) {
			param[i] = VT_VARIANT;
			args[i] = ADvOcx::ToVariant(val);
		} else {
			COleVariant *var = val->a.var.v;
			switch (V_VT(var)) {
			case VT_I2:
				param[i] = VT_MFCBYREF|VT_I2;
				args[i] = (COleVariant*)&V_I2(var);
				break;
			case VT_I4:
				param[i] = VT_MFCBYREF|VT_I4;
				args[i] = (COleVariant*)&V_I4(var);
				break;
			case VT_R4:
				param[i] = VT_MFCBYREF|VT_R4;
				args[i] = (COleVariant*)&V_R4(var);
				break;
			case VT_R8:
				param[i] = VT_MFCBYREF|VT_R8;
				args[i] = (COleVariant*)&V_R8(var);
				break;
			case VT_BSTR:
				param[i] = VT_MFCBYREF|VT_BSTR;
				args[i] = (COleVariant*)&V_BSTR(var);
				break;
			case VT_BOOL:
				param[i] = VT_MFCBYREF|VT_BOOL;
				args[i] = (COleVariant*)&V_BOOL(var);
				break;
			case VT_DISPATCH:
				param[i] = VT_MFCBYREF|VT_DISPATCH;
				args[i] = (COleVariant*)&V_DISPATCH(var);
				break;
			case VT_UNKNOWN:
				param[i] = VT_MFCBYREF|VT_UNKNOWN;
				args[i] = (COleVariant*)&V_UNKNOWN(var);
				break;
			case VT_EMPTY:
				param[i] = VT_MFCBYREF|VT_VARIANT;
				args[i] = (COleVariant*)var;
				break;
			default:
				param[i] = VT_VARIANT;
				args[i] = ADvOcx::ToVariant(val);
				break;
			}
		}
		ASSERT(val_stack);
		Set(val, val_stack->H);
		Pop(val_stack);
	}
	param[i] = 0;	

	if (!IsVariant(val) || val->a.var.v == NULL ||
			V_VT(val->a.var.v) != VT_DISPATCH) {
		art_sprintf_1(err, 256, AL_O3, method);
		for (i = 0; i < num_arg; i++)
			delete args[i];
		return interp_error(err);
	}

	IDispatch *pdisp = V_DISPATCH(val->a.var.v);
	if (pdisp == NULL) {
		art_sprintf_1(err, 256, AL_O10, method);
		for (i = 0; i < num_arg; i++)
			delete args[i];
		return interp_error(err);
	}
	DISPID dispid;
	if (ADvOcx::GetIDsOfNames(pdisp, method, &dispid)) {
		art_sprintf_1(err, 256, AL_O4, method);
		for (i = 0; i < num_arg; i++)
			delete args[i];
		return interp_error(err);
	}

	try {
		COleDispatchDriver drv;
		drv.AttachDispatch(pdisp, FALSE);

		COleVariant var;
		drv.InvokeHelperV(dispid, DISPATCH_METHOD|DISPATCH_PROPERTYGET,
			VT_VARIANT, &var, param, (va_list)args);
		Unref(val);
		val = Ref(ADvOcx::FromVariant(var));
		for (i = 0; i < num_arg; i++)
			if (param[i] == VT_VARIANT)
				delete args[i];
		prog = prog->a.i.t->TT;
		return 0;

	} catch (COleException* e) {
		e->Delete();
		art_sprintf_1(err, 256, AL_O5, method);
		for (i = 0; i < num_arg; i++)
			if (param[i] == VT_VARIANT)
				delete args[i];
		return interp_error(err);

	} catch (COleDispatchException* e) {
		e->Delete();
		art_sprintf_1(err, 256, AL_O6, method);
		for (i = 0; i < num_arg; i++)
			if (param[i] == VT_VARIANT)
				delete args[i];
		return interp_error(err);

	} catch (CMemoryException* e) {
		e->Delete();
		art_sprintf_1(err, 256, AL_O7, method);
		for (i = 0; i < num_arg; i++)
			if (param[i] == VT_VARIANT)
				delete args[i];
		return interp_error(err);
	}
#endif
#ifdef	_LINUX
	return 0;
#endif
}

int AIpProcess::EvalOcxSetProp() {
#ifdef	_WINDOWS
	const char *method = prog->a.i.t->H->a.s.v->Str();
	long num_arg = (long)prog->a.i.t->T->a.i.v;
	BYTE param[34];
	COleVariant* args[33];
	char err[256];
	if (num_arg > 33) {
		art_sprintf_1(err, 256, AL_O2, method);
		return interp_error(err);
	}
	int i;
	for (i = 0; i < num_arg; i++) {
		if (!IsVariant(val) || val->a.var.byref == 0) {
			param[i] = VT_VARIANT;
			args[i] = ADvOcx::ToVariant(val);
		} else {
			COleVariant *var = val->a.var.v;
			switch (V_VT(var)) {
			case VT_I2:
				param[i] = VT_MFCBYREF|VT_I2;
				args[i] = (COleVariant*)&V_I2(var);
				break;
			case VT_I4:
				param[i] = VT_MFCBYREF|VT_I4;
				args[i] = (COleVariant*)&V_I4(var);
				break;
			case VT_R4:
				param[i] = VT_MFCBYREF|VT_R4;
				args[i] = (COleVariant*)&V_R4(var);
				break;
			case VT_R8:
				param[i] = VT_MFCBYREF|VT_R8;
				args[i] = (COleVariant*)&V_R8(var);
				break;
			case VT_BSTR:
				param[i] = VT_MFCBYREF|VT_BSTR;
				args[i] = (COleVariant*)&V_BSTR(var);
				break;
			case VT_BOOL:
				param[i] = VT_MFCBYREF|VT_BOOL;
				args[i] = (COleVariant*)&V_BOOL(var);
				break;
			case VT_DISPATCH:
				param[i] = VT_MFCBYREF|VT_DISPATCH;
				args[i] = (COleVariant*)&V_DISPATCH(var);
				break;
			case VT_UNKNOWN:
				param[i] = VT_MFCBYREF|VT_UNKNOWN;
				args[i] = (COleVariant*)&V_UNKNOWN(var);
				break;
			case VT_EMPTY:
				param[i] = VT_MFCBYREF|VT_VARIANT;
				args[i] = (COleVariant*)var;
				break;
			default:
				param[i] = VT_VARIANT;
				args[i] = ADvOcx::ToVariant(val);
				break;
			}
		}
		ASSERT(val_stack);
		Set(val, val_stack->H);
		Pop(val_stack);
	}
	param[i] = 0;	

	if (!IsVariant(val) || val->a.var.v == NULL ||
			V_VT(val->a.var.v) != VT_DISPATCH) {
		art_sprintf_1(err, 256, AL_O3, method);
		for (i = 0; i < num_arg; i++)
			delete args[i];
		return interp_error(err);
	}

	IDispatch *pdisp = V_DISPATCH(val->a.var.v);
	if (pdisp == NULL) {
		art_sprintf_1(err, 256, AL_O10, method);
		for (i = 0; i < num_arg; i++)
			delete args[i];
		return interp_error(err);
	}
	DISPID dispid;
	if (ADvOcx::GetIDsOfNames(pdisp, method, &dispid)) {
		art_sprintf_1(err, 256, AL_O4, method);
		for (i = 0; i < num_arg; i++)
			delete args[i];
		return interp_error(err);
	}

	try {
		COleDispatchDriver drv;
		drv.AttachDispatch(pdisp, FALSE);

		drv.InvokeHelperV(dispid, DISPATCH_PROPERTYPUT,
			VT_EMPTY, NULL, param, (va_list)args);
		for (i = 0; i < num_arg; i++)
			if (param[i] == VT_VARIANT)
				delete args[i];
		prog = prog->a.i.t->TT;
		return 0;

	} catch (COleException* e) {
		e->Delete();
		art_sprintf_1(err, 256, AL_O5, method);
		for (i = 0; i < num_arg; i++)
			if (param[i] == VT_VARIANT)
				delete args[i];
		return interp_error(err);

	} catch (COleDispatchException* e) {
		e->Delete();
		art_sprintf_1(err, 256, AL_O6, method);
		for (i = 0; i < num_arg; i++)
			if (param[i] == VT_VARIANT)
				delete args[i];
		return interp_error(err);

	} catch (CMemoryException* e) {
		e->Delete();
		art_sprintf_1(err, 256, AL_O7, method);
		for (i = 0; i < num_arg; i++)
			if (param[i] == VT_VARIANT)
				delete args[i];
		return interp_error(err);
	}
#endif
#ifdef	_LINUX
	return 0;
#endif
}

int AIpProcess::EvalOcxSetMethod() {
	AGmNode *wnd, *method, *obj, *func;
	GetArg(wnd, method, obj, func);
	if (!IsWindow(wnd) || !wnd->a.w.v->OleCtrl() ||
			!IsString(method) || !obj || !func) {
		Unref(wnd, method, obj, func);
		return interp_error("OcxSetMethod()");
	}
#ifdef	_WINDOWS
	if (wnd->a.w.v->OleCtrl()->SetMethod(method, obj, func)) {
		Unref(wnd, method, obj, func);
		return interp_error("OcxSetMethod()");
	}
#endif
	Unref(wnd, method, obj, func);
	return 0;
}
int AIpProcess::EvalOcxInvokeArg() {
	AGmNode *name, *value, *tail;
	GetArg(name, value, tail);
	if (!IsString(name)) {
		Unref(name, value, tail);
		return interp_error("OcxInvokeArg()");
	}
	Unref(val); val = Ref(Cons( List(UniqueStr(name), value, NULL), tail));
	Unref(name, value, tail);
	return 0;
}
int AIpProcess::EvalOcxFireEvent() {
	AGmNode *wnd, *evnt, *params;
	GetArg(wnd, evnt, params);
	if (!IsWindow(wnd) || !IsString(evnt) || !wnd->a.w.v->OleCtrl()) {
		Unref(wnd, evnt, params);
		return interp_error("OcxFireEvent()");
	}
#ifdef	_WINDOWS
	wnd->a.w.v->OleCtrl()->CustomEvent(evnt, params);
#endif
	Unref(wnd, evnt, params);
	return 0;
}
int AIpProcess::EvalRootClass() {
	Unref(val);
	val = Ref(RootClass());
	return 0;
}

#ifdef	_WINDOWS
static AGmNode* vt_to_type(int vt) {
	switch (vt) {
	case VT_I2:
	case VT_UI2:
		return AGm::String("short");
	case VT_I4:
	case VT_UI4:
	case VT_INT:
	case VT_UINT:
		return AGm::String("long");
	case VT_R4:
		return AGm::String("float");
	case VT_R8:
		return AGm::String("double");
	case VT_DATE:
		return AGm::String("date");
	case VT_BSTR:
		return AGm::String("string");
	case VT_DISPATCH:
		return AGm::String("dispatch");
	case VT_BOOL:
		return AGm::String("boolean");
	case VT_VARIANT:
		return AGm::String("variant");
	case VT_VOID:
		return AGm::String("void");
	case VT_HRESULT:
		return AGm::String("HRESULT");
	case VT_PTR:
		return AGm::String("pointer");
	case VT_USERDEFINED:
		return AGm::String("user-defined");
	default:
		return NULL;
	}
}
#endif

int AIpProcess::EvalOleMisc() {
	AGmNode *cmd, *arg1, *arg2;
	GetArg(cmd, arg1, arg2);
	if (!IsString(cmd)) {
		Unref(cmd, arg1, arg2);
		return interp_error("OleMisc()");
	}
	Unref(val); val = NULL;
#ifdef	_WINDOWS
	const char *s = cmd->a.s.v->Str();
	if (strcmp(s, "create_object") == 0) {
		if (!IsString(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("OleMisc() create_object");
		}
		const char *progid = arg1->a.s.v->Str();
		COleException excep;
		COleDispatchDriver drv;
		if (drv.CreateDispatch(progid, &excep)) {
			VARIANT v;
			VariantInit(&v);
			V_VT(&v) = VT_DISPATCH;
			V_DISPATCH(&v) = drv.DetachDispatch();
			val = Ref(ADvOcx::FromVariant(v));
		}

	} else if (strcmp(s, "get_object") == 0) {
		USES_CONVERSION;
		if (!IsString(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("OleMisc() get_object");
		}
		const char *path = arg1->a.s.v->Str();
		IStorage *pStg = NULL;
		HRESULT hr = StgOpenStorage(
			T2OLE(path), NULL, STGM_PRIORITY, NULL, 0, &pStg);
		if (hr == NOERROR) {
			CLSID clsid;
			hr = ReadClassStg(pStg, &clsid);
			pStg->Release();
			if (hr == NOERROR) {
				IBindCtx *pbc;
				hr = CreateBindCtx(0, &pbc);
				if (hr == NOERROR) {
					ULONG cEaten;
					IMoniker *pmk;
					hr = MkParseDisplayName(pbc, T2OLE(path), &cEaten, &pmk);
					pbc->Release();
					if (hr == NOERROR) {
						IDispatch *pdisp;
						hr = BindMoniker(pmk, 0, IID_IDispatch, (void**)&pdisp);
						pmk->Release();
						if (hr == NOERROR) {
							VARIANT v;
							VariantInit(&v);
							V_VT(&v) = VT_DISPATCH;
							V_DISPATCH(&v) = pdisp;
							val = Ref(ADvOcx::FromVariant(v));
						}
					}
				}
			}
		}

	} else if (strcmp(s, "short") == 0) {
		if (!IsInteger(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("OleMisc() short");
		}
		VARIANT v;
		VariantInit(&v);
		V_VT(&v) = VT_I2;
		V_I2(&v) = (short)arg1->a.i.v;
		Unref(val);
		val = Ref(Variant(new COleVariant(v), (arg2 ? 1 : 0)));

	} else if (strcmp(s, "long") == 0) {
		if (!IsInteger(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("OleMisc() long");
		}
		VARIANT v;
		VariantInit(&v);
		V_VT(&v) = VT_I4;
		V_I4(&v) =(long) arg1->a.i.v;
		Unref(val);
		val = Ref(Variant(new COleVariant(v), (arg2 ? 1 : 0)));

	} else if (strcmp(s, "float") == 0) {
		if (!IsReal(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("OleMisc() float");
		}
		VARIANT v;
		VariantInit(&v);
		V_VT(&v) = VT_R4;
		V_R4(&v) = (float)arg1->a.r.v;
		Unref(val);
		val = Ref(Variant(new COleVariant(v), (arg2 ? 1 : 0)));

	} else if (strcmp(s, "double") == 0) {
		if (!IsReal(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("OleMisc() double");
		}
		VARIANT v;
		VariantInit(&v);
		V_VT(&v) = VT_R8;
		V_R8(&v) = arg1->a.r.v;
		Unref(val);
		val = Ref(Variant(new COleVariant(v), (arg2 ? 1 : 0)));

	} else if (strcmp(s, "string") == 0) {
		if (!IsString(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("OleMisc() string");
		}
		VARIANT v;
		VariantInit(&v);
		V_VT(&v) = VT_BSTR;
		CString s = arg1->a.s.v->Str();
		V_BSTR(&v) = s.AllocSysString();
		Unref(val);
		val = Ref(Variant(new COleVariant(v), (arg2 ? 1 : 0)));

	} else if (strcmp(s, "boolean") == 0) {
		VARIANT v;
		VariantInit(&v);
		V_VT(&v) = VT_BOOL;
		V_BOOL(&v) = (short)(arg1 ? -1 : 0);
		Unref(val);
		val = Ref(Variant(new COleVariant(v), (arg2 ? 1 : 0)));

	} else if (strcmp(s, "dispatch") == 0) {
		VARIANT v;
		VariantInit(&v);
		V_VT(&v) = VT_DISPATCH;
		V_DISPATCH(&v) = NULL;
		Unref(val);
		val = Ref(Variant(new COleVariant(v), (arg2 ? 1 : 0)));

	} else if (strcmp(s, "unknown") == 0) {
		VARIANT v;
		VariantInit(&v);
		V_VT(&v) = VT_UNKNOWN;
		V_UNKNOWN(&v) = NULL;
		Unref(val);
		val = Ref(Variant(new COleVariant(v), (arg2 ? 1 : 0)));

	} else if (strcmp(s, "variant") == 0) {
		VARIANT v;
		VariantInit(&v);
		V_VT(&v) = VT_EMPTY;
		Unref(val);
		val = Ref(Variant(new COleVariant(v), (arg2 ? 1 : 0)));

	} else if (strcmp(s, "convert") == 0) {
		if (!IsVariant(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("OleMisc() convert");
		}
		COleVariant *var = arg1->a.var.v;
		Unref(val);
		val = Ref(ADvOcx::FromVariant(*var));

	} else if (strcmp(s, "api") == 0) {
		if (!IsVariant(arg1)) {
			Unref(cmd, arg1, arg2);
			return interp_error("OleMisc() api");
		}
		if (V_VT(arg1->a.var.v) != VT_DISPATCH) {
			Unref(cmd, arg1, arg2);
			return interp_error("not OLE Object.");
		}
		IDispatch *pdisp = V_DISPATCH(arg1->a.var.v);
		ITypeInfo *pTypeInfo;
		HRESULT r = pdisp->GetTypeInfo(0, 0, &pTypeInfo);
		if (r != S_OK) {
			Unref(cmd, arg1, arg2);
			return interp_error("fail to get TypeInfo.");
		}
		int num, i;

		AGmNode *n2, *n3, *funcs;
		val = Ref(Cons(NULL));
		
		LPTYPEATTR pTypeAttr;
		r = pTypeInfo->GetTypeAttr(&pTypeAttr);
		if (r != S_OK) {
			Unref(cmd, arg1, arg2);
			pTypeInfo->Release();
			return interp_error("fail to get property TypeAttr.");
		}

		// property
		CreateArc(val, n2 = Cons(NULL), String("property"));

		num = pTypeAttr->cVars;
		i = 0;
		for (i = 0; i < num; i++) {
			LPVARDESC pVarDesc;
			r = pTypeInfo->GetVarDesc(i, &pVarDesc);
			if (r != S_OK) {
				continue;
			}
			BSTR *pbstr = new BSTR[1]; unsigned int cNames;
			r = pTypeInfo->GetNames(pVarDesc->memid, pbstr, 1, &cNames);
			if (r != S_OK || cNames < 1) {
				delete [] pbstr;
				pTypeInfo->ReleaseVarDesc(pVarDesc);
				continue;
			}
			char s[100];
			art_wcstombs(s, 100, pbstr[0], 100);
			unsigned int k;
			for (k = 0; k < cNames; k++) {
				SysFreeString(pbstr[k]);
			}
			delete [] pbstr;

			// add property
			CreateArc(n2, Cons(vt_to_type(pVarDesc->elemdescVar.tdesc.vt)), String(s));

			pTypeInfo->ReleaseVarDesc(pVarDesc);
		}

		// method
		CreateArc(val, funcs = n2 = Cons(NULL), String("method"));

		num = pTypeAttr->cFuncs;
		i = 0;
		for (i = 0; i < num; i++) {
			LPFUNCDESC pFuncDesc;
			r = pTypeInfo->GetFuncDesc(i, &pFuncDesc);
			if (r != S_OK) {
				continue;
			}

			switch (pFuncDesc->invkind) {
			case INVOKE_FUNC:
			case INVOKE_PROPERTYPUT:
			case INVOKE_PROPERTYGET:
				break;
			default:
				pTypeInfo->ReleaseFuncDesc(pFuncDesc);
				continue;
			}

			BSTR *pbstr = new BSTR[pFuncDesc->cParams+1]; unsigned int cNames;
			r = pTypeInfo->GetNames(pFuncDesc->memid, pbstr, pFuncDesc->cParams+1, &cNames);
			if (r != S_OK || cNames < 1) {
				delete [] pbstr;
				pTypeInfo->ReleaseFuncDesc(pFuncDesc);
				continue;
			}
			char s[100];
			art_wcstombs(s, 100, pbstr[0], 100);
			unsigned int k;
			for (k = 0; k < cNames; k++) {
				SysFreeString(pbstr[k]);
			}
			delete [] pbstr;

			// add method
			CreateArc(n2, n3 = Cons(vt_to_type(pFuncDesc->elemdescFunc.tdesc.vt)), String(s));

			int np;
			for (np = 0; np < pFuncDesc->cParams ; np++) {
				// add method argument
				CreateArc(n3, Cons(vt_to_type(pFuncDesc->lprgelemdescParam[np].tdesc.vt)));
			}

			pTypeInfo->ReleaseFuncDesc(pFuncDesc);
		}

		pTypeInfo->ReleaseTypeAttr(pTypeAttr);
		pTypeInfo->Release();

		// event
		CreateArc(val, n2 = Cons(NULL), String("event"));

		LPPROVIDECLASSINFO pPCI;
		r = pdisp->QueryInterface(IID_IProvideClassInfo, (void**)&pPCI);
		if (r != S_OK) {
			Unref(cmd, arg1, arg2);
			return 0;
		}
		LPTYPEINFO pClassInfo;
		r = pPCI->GetClassInfo(&pClassInfo);
		if (r != S_OK) {
			pPCI->Release();
			Unref(cmd, arg1, arg2);
			return 0;
		}
		LPTYPEATTR pClassAttr;
		r = pClassInfo->GetTypeAttr(&pClassAttr);
		if (r != S_OK) {
			pClassInfo->Release();
			pPCI->Release();
			Unref(cmd, arg1, arg2);
			return 0;
		}
		num = pClassAttr->cImplTypes;
		for (i = 0; i < num; i++) {
			HREFTYPE hRefType;
			LPTYPEINFO pEventInfo;
			r = pClassInfo->GetRefTypeOfImplType(i, &hRefType);
			if (r != S_OK) {
				continue;
			}
			r = pClassInfo->GetRefTypeInfo(hRefType, &pEventInfo);
			if (r != S_OK) {
				continue;
			}
			LPTYPEATTR pEventAttr;
			r = pEventInfo->GetTypeAttr(&pEventAttr);
			if (r != S_OK) {
				pEventInfo->Release();
				continue;
			}
			LPFUNCDESC pFuncDesc;
			int np;
			for (np = 0; np < pEventAttr->cFuncs; np++) {
				r = pEventInfo->GetFuncDesc(np, &pFuncDesc);
				if (r != S_OK) {
					continue;
				}
				BSTR *pbstr = new BSTR[pFuncDesc->cParams+1]; unsigned int cNames;
				r = pEventInfo->GetNames(pFuncDesc->memid, pbstr, pFuncDesc->cParams+1, &cNames);
				if (r != S_OK || cNames < 0) {
					pEventInfo->ReleaseFuncDesc(pFuncDesc);
					continue;
				}
				char s[100];
				art_wcstombs(s, 100, pbstr[0], 100);
				for (unsigned long k = 0; k < cNames; k++)
					SysFreeString(pbstr[k]);
				delete [] pbstr;

				// skip if same name method exist
				if (dst_node(funcs, s)) {
					pEventInfo->ReleaseFuncDesc(pFuncDesc);
					continue;
				}

				// add event
				CreateArc(n2, n3 = Cons(NULL), String(s));

				int np;
				for (np = 0; np < pFuncDesc->cParams ; np++) {
					// add event argument
					CreateArc(n3, Cons(vt_to_type(pFuncDesc->lprgelemdescParam[np].tdesc.vt)));
				}

				pEventInfo->ReleaseFuncDesc(pFuncDesc);
			}
			pEventInfo->ReleaseTypeAttr(pEventAttr);
			pEventInfo->Release();
		}
		pClassInfo->ReleaseTypeAttr(pClassAttr);
		pClassInfo->Release();
		pPCI->Release();

	} else {
		Unref(cmd, arg1, arg2);
		return interp_error("OleMisc()");
	}
#endif
	Unref(cmd, arg1, arg2);
	return 0;
}

ADvOcx::ADvOcx(const char *progID, int visible, const char *subStgID) {
	_progID = new AGmString(progID);
	_visible = visible;
	_subStgID = new AGmString(subStgID);
	_left = 10; _top = 10; _right = 20; _bottom = 20;
	_aw = NULL;
	_nID = (long)GetUniqueID();
	_ctrl = NULL;
	_fileName = NULL;
	_mode = 1;
}
ADvOcx::ADvOcx(ADvOcx &ocx) {
	_progID = new AGmString(ocx._progID);
	_visible = ocx._visible;
	_subStgID = new AGmString(ocx._subStgID);
	_left = 10; _top = 10; _right = 20; _bottom = 20;
	_aw = NULL;
	_nID = (long)GetUniqueID();
	_ctrl = NULL;
	_fileName = NULL;
	_mode = 1;
}
ADvOcx::~ADvOcx() {
	ReleaseUniqueID(_nID);
	if (_ctrl)
		delete _ctrl;
	delete _progID;
	delete _subStgID;
	if (_fileName)
		delete _fileName;
}

#ifdef	_WINDOWS
ADvOcxWnd::ADvOcxWnd(ADvOcx *ocx) {
	_ocx = ocx;
}
ADvOcxWnd::~ADvOcxWnd() {
	_ocx->Ctrl(NULL);
	_ocx->Parent(NULL);
}

BOOL ADvOcxWnd::PreTranslateMessage(MSG* msg) {
	if (_ocx->Mode()) {
		return CWnd::PreTranslateMessage(msg);
	} else {
		UINT message = msg->message;
		WPARAM w = msg->wParam;
		LPARAM l = msg->lParam;
		switch (message) {
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_MOUSEMOVE:
			POINT p;
			p.x = LOWORD(l); p.y = HIWORD(l);
			MapWindowPoints(GetParent(), &p, 1);
			LPARAM l2;
			l2 = MAKELPARAM(p.x, p.y);
			GetParent()->SendMessage(message, w, l2);
			return TRUE;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			GetParent()->SendMessage(message, w, l);
			return TRUE;
		default:
			return CWnd::PreTranslateMessage(msg);
		}
	}
}
BOOL ADvOcxWnd::PreCreateWindow(CREATESTRUCT&cs) {
	cs.style |= WS_CLIPSIBLINGS;
	return CWnd::PreCreateWindow(cs);
}
BOOL ADvOcxWnd::OnCmdMsg(UINT nID, int nCode,
	void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo) {
	if (ADvGetApp() == NULL && m_hWnd) {
		CWnd *w = GetParentFrame();
		if (w)
			return w->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}
	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

int ADvOcx::Create(ADvWindow *aw) {
	if (_ctrl) return 1;
	CWnd *parent;
	if (aw->View()) {
		parent = aw->View();
	} else if (aw->OleCtrl()) {
		parent = aw->OleCtrl();
	} else {
		return 2;
	}
	_right = max(_right, _left+1);
	_bottom = max(_bottom, _top+1);
	_ctrl = new ADvOcxWnd(this);
	_aw = aw;
	CRect rect(_left, _top, _right, _bottom);
	BOOL r = _ctrl->CreateControl(_progID->Str(), NULL,
		(_visible ? WS_VISIBLE : 0), rect, parent, _nID);
	if (r == FALSE) {
		delete _ctrl;
		_ctrl = NULL;
		_aw = NULL;
		return 3;
	}
	_ctrl->BringWindowToTop();
	AGmNode *idx = Ref(Integer(_nID));
	if (aw->View()) {
		set_dst_node(_aw->View()->CallbackTbl(), idx, NULL);
	} else if (aw->OleCtrl()) {
		set_dst_node(_aw->OleCtrl()->CallbackTbl(), idx, NULL);
	}
	Unref(idx);
	return 0;
}

const char *ADvOcx::FileName() const {
	if (_fileName == NULL)
		return NULL;
	return _fileName->Str();
}
void ADvOcx::FileName(const char *name) {
	if (_fileName)
		delete _fileName;
	_fileName = NULL;
	if (name == NULL)
		return;
	_fileName = new AGmString(name);
}

int ADvOcx::Place(int left, int top, int right, int bottom) {
	if (right - left < 1 || bottom - top < 1) return 1;
	if (left == _left && top == _top &&
			right == _right && bottom == _bottom) {
		return 0;
	}
	_left = left; _top = top; _right = right; _bottom = bottom;
	if (_ctrl && _ctrl->m_hWnd) {
		_ctrl->MoveWindow(left, top, right - left, bottom - top);
	} else {
		return 2;
	}
	return 0;
}
void ADvOcx::Visible(int visible) {
	_visible = visible;
	if (_ctrl && _ctrl->m_hWnd) {
		if (visible) {
			_ctrl->ShowWindow(SW_SHOW);
		} else {
			_ctrl->ShowWindow(SW_HIDE);
		}
	}
}

IUnknown *ADvOcx::GetUnknown() {
	if (_ctrl == NULL) return NULL;
	return _ctrl->GetControlUnknown();
}
IDispatch *ADvOcx::GetDispatch() {
	IUnknown *punk = GetUnknown();
	if (punk == NULL) return NULL;
	IDispatch *pdisp;
	HRESULT hr = punk->QueryInterface(IID_IDispatch, (void**)&pdisp);
	if (FAILED(hr)) return NULL;
	return pdisp;
}
ITypeInfo *ADvOcx::GetClassInfo() {
	IUnknown *punk = GetUnknown();
	if (punk == NULL) return NULL;
	IProvideClassInfo *pPCI;
	HRESULT hr = punk->QueryInterface(IID_IProvideClassInfo, (void**)&pPCI);
	if (FAILED(hr)) return NULL;
	ITypeInfo *pClassInfo;
	hr = pPCI->GetClassInfo(&pClassInfo);
	pPCI->Release();
	if (FAILED(hr)) return NULL;
	return pClassInfo;
}

BOOL ADvOcx::RuntimeInvisible() {
	IUnknown *punk = GetUnknown();
	if (punk == NULL) return FALSE;
	IOleObject *obj;
	HRESULT hr = punk->QueryInterface(IID_IOleObject, (void**)&obj);
	if (FAILED(hr)) return FALSE;
	DWORD dw;
	obj->GetMiscStatus(DVASPECT_CONTENT, &dw);
	BOOL r = ((dw & OLEMISC_INVISIBLEATRUNTIME) != 0);
	obj->Release();
	return r;
}
void ADvOcx::PropertyEditor() {
	IUnknown *punk = GetUnknown();
	if (punk == NULL) return;
	IOleObject *obj;
	HRESULT hr = punk->QueryInterface(IID_IOleObject, (void**)&obj);
	if (FAILED(hr)) return;
	MSG msg;
	COleControlSite *csite = _ctrl->GetControlSite();
	IOleClientSite *site = &csite->m_xOleClientSite;
	HWND parent = 0;
	if (_aw->View()) {
		parent = _aw->View()->m_hWnd;
	} else if (_aw->OleCtrl()) {
		parent = _aw->OleCtrl()->m_hWnd;
	}
	if (parent) {
		CRect rect;
		obj->DoVerb(OLEIVERB_PROPERTIES, &msg, site, 0, parent, rect);	
	}
	obj->Release();
}

int ADvOcx::Save(IStorage *pStg) {
	USES_CONVERSION;
	if (_ctrl == NULL) return 1;
	IStorage *pSubStg;
	HRESULT hr = pStg->OpenStorage(T2OLE(_subStgID->Str()),NULL,
		STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_DIRECT,
		NULL, 0 , &pSubStg);
	if (FAILED(hr)) {
		hr = pStg->CreateStorage(T2OLE(_subStgID->Str()) ,
			STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_DIRECT,
			0, 0 , &pSubStg);
		if (FAILED(hr)) {
			return 2;
		}
	}
	IUnknown *punk = GetUnknown();
	if (punk == NULL) {
		pSubStg->Release();
		return 3;
	}
	IPersistStorage *pPersistStg;
	hr = punk->QueryInterface(IID_IPersistStorage, (void **)&pPersistStg);
	if (FAILED(hr)) {
		pSubStg->Release();
		return 4;
	}
	hr = pPersistStg->Save(pSubStg, FALSE);
	if (FAILED(hr)) {
		pPersistStg->Release();
		pSubStg->Release();
		return 5;
	}
	hr = pPersistStg->SaveCompleted(NULL);
	if (FAILED(hr)) {
		pPersistStg->Release();
		pSubStg->Release();
		return 6;
	}
	pSubStg->Commit(STGC_OVERWRITE);
	pPersistStg->Release();
	pSubStg->Release();
	return 0;
}
int ADvOcx::Load(IStorage *pStg) {
	USES_CONVERSION;
	if (_ctrl == NULL) return 1;
	IStorage *pSubStg;
	HRESULT hr = pStg->OpenStorage(T2OLE(_subStgID->Str()), NULL,
		STGM_READWRITE|STGM_SHARE_DENY_WRITE|STGM_DIRECT,
		NULL, 0 , &pSubStg);
	if (FAILED(hr)) {
		hr = pStg->OpenStorage(T2OLE(_subStgID->Str()), NULL,
			STGM_READ|STGM_SHARE_DENY_WRITE|STGM_DIRECT,
			NULL, 0 , &pSubStg);
		if (FAILED(hr)) {
			return 2;
		}
	}
	IUnknown *punk = GetUnknown();
	if (punk == NULL) {
		pSubStg->Release();
		return 3;
	}
	IPersistStorage *pPersistStg;
	hr = punk->QueryInterface(IID_IPersistStorage, (void **)&pPersistStg);
	if (FAILED(hr)) {
		pSubStg->Release();
		return 4;
	}
	hr = pPersistStg->Load(pSubStg);
	if (FAILED(hr)) {
		pPersistStg->Release();
		pSubStg->Release();
		return 5;
	}
	pPersistStg->Release();
	pSubStg->Release();
	return 0;
}

int ADvOcx::GetIDsOfNames(IDispatch *pdisp, const char *name, DISPID *id) {
	USES_CONVERSION;
	OLECHAR *name2 = T2OLE(name);
	if (FAILED(pdisp->GetIDsOfNames(
			IID_NULL, &name2, 1, LOCALE_SYSTEM_DEFAULT, id))) {
		return 1;
	}
	return 0;

}

COleVariant *ADvOcx::ToVariant(AGmNode *n) {
	COleVariant *var;
	VARIANT v;
	VariantInit(&v);
	if (n == NULL) {
		V_VT(&v) = VT_ERROR;
		v.scode = DISP_E_PARAMNOTFOUND;
		var = new COleVariant(v);	
		return var;
	}
	switch (n->type) {
	case AGM_NODE_INTEGER:
		var = new COleVariant((short)n->a.i.v);
		break;
	case AGM_NODE_REAL:
		var = new COleVariant(n->a.r.v);
		break;
	case AGM_NODE_STRING:
		var = new COleVariant(n->a.s.v->Str());
		break;
	case AGM_NODE_VARIANT:
		var = new COleVariant(n->a.var.v);
		break;
	default:
		IAGmNode *pNode;
		pNode = new IAGmNode;
		pNode->SetNode(n);
		V_VT(&v) = VT_UNKNOWN;
		V_UNKNOWN(&v) = pNode;
		var = new COleVariant(v);	
		break;
	}
	return var;
}

AGmNode *ADvOcx::FromVariant(const VARIANT& v) {
	USES_CONVERSION;
	AGmNode *n;
	switch (V_VT(&v)) {
	case VT_UI1:
		n = Integer(V_UI1(&v));
		break;
	case VT_I2:
		n = Integer(V_I2(&v));
		break;
	case VT_I4:
		n = Integer(V_I4(&v));
		break;
	case VT_R4:
		n = Real(V_R4(&v));
		break;
	case VT_R8:
		n = Real(V_R8(&v));
		break;
	case VT_BOOL:
		if (V_BOOL(&v)) {
			n = Integer(1);
		} else {
			n = NULL;
		}
		break;
	case VT_BSTR:
		n = String(OLE2T(V_BSTR(&v)));
		break;
	case VT_UI1|VT_BYREF:
		n = Integer(*V_UI1REF(&v));
		break;
	case VT_I2|VT_BYREF:
		n = Integer(*V_I2REF(&v));
		break;
	case VT_I4|VT_BYREF:
		n = Integer(*V_I4REF(&v));
		break;
	case VT_R4|VT_BYREF:
		n = Real(*V_R4REF(&v));
		break;
	case VT_R8|VT_BYREF:
		n = Real(*V_R8REF(&v));
		break;
	case VT_BOOL|VT_BYREF:
		if (*V_BOOLREF(&v)) {
			n = Integer(1);
		} else {
			n = NULL;
		}
		break;
	case VT_BSTR|VT_BYREF:
		n = String(OLE2T(*V_BSTRREF(&v)));
		break;
	case VT_UNKNOWN: {
			IUnknown *punk = V_UNKNOWN(&v);
			IAGmNode *pNode;
			HRESULT hr = punk->QueryInterface(IID_IAGmNode, (void**)&pNode);
			if (FAILED(hr)) {
				n = Variant(new COleVariant(v));
				punk->Release();
				break;
			}
			punk->Release();
			pNode->GetNode(&n);
			break;
		}
	case VT_DISPATCH:
		n = Variant(new COleVariant(v));
		break;
	case VT_VARIANT|VT_BYREF:
		VARIANT *var;
		var = V_VARIANTREF(&v);
		n = FromVariant(*var);
		break;
	default:
		n = Variant(new COleVariant(v));
		break;
	}
	return n;
}

#undef T
#include <atlimage.h>

COleVariant *ADvOcx::LoadPicture(const char *dir, const char *name) {
	PICTDESC pdesc;
	pdesc.cbSizeofstruct = sizeof(PICTDESC);
	pdesc.picType = (unsigned int)PICTYPE_UNINITIALIZED;

	HBITMAP hbmp = NULL;
	ADvBitmap *bitmap = new ADvBitmap;
	bitmap->SetFileName(name);
	if (bitmap->Load(dir) == 0) {
		pdesc.picType = PICTYPE_BITMAP;
		pdesc.bmp.hbitmap = bitmap->Buffer()->Detach();
		pdesc.bmp.hpal = NULL;
	}
	delete bitmap;

	if (pdesc.picType == PICTYPE_UNINITIALIZED)
		return new COleVariant();
	IPicture* pPict;
	HRESULT hr = OleCreatePictureIndirect(
		&pdesc, IID_IPicture, TRUE, (void**)&pPict);
	if (FAILED(hr)) {
		if (pdesc.picType == PICTYPE_BITMAP)
			DeleteObject(hbmp);
		return new COleVariant();
	}
	IPictureDisp *pdisp;
	hr = pPict->QueryInterface(IID_IPictureDisp, (void**)&pdisp);
	if (FAILED(hr)) {
		pPict->Release();
		return new COleVariant();
	}
	VARIANT v;
	VariantInit(&v);
	V_VT(&v) = VT_DISPATCH;
	V_DISPATCH(&v) = pdisp;
	COleVariant *var = new COleVariant(v);
	pdisp->Release();
	pPict->Release();
	return var;
}

COleVariant *ADvOcx::CreateFont(const char *name, int size,
		int bold, int italic, int underline, int strike) {
	USES_CONVERSION;
	FONTDESC fdesc;
	fdesc.cbSizeofstruct = sizeof(FONTDESC);
	fdesc.lpstrName = T2OLE(name);
	CY cy;
	cy.Hi = 0; cy.Lo = size * 10000;
	fdesc.cySize = cy;
	fdesc.sWeight = (short)(bold ? FW_BOLD : FW_NORMAL);
	fdesc.sCharset = DEFAULT_CHARSET;
	fdesc.fItalic = (italic != 0);
	fdesc.fUnderline = (underline != 0);
	fdesc.fStrikethrough = (strike != 0);
	IFont* pFont;
	HRESULT hr = OleCreateFontIndirect(
		&fdesc, IID_IFont, (void**)&pFont);
	if (FAILED(hr)) return new COleVariant();
	IFontDisp *pdisp;
	hr = pFont->QueryInterface(IID_IPictureDisp, (void**)&pdisp);
	if (FAILED(hr)) {
		pFont->Release();
		return new COleVariant();
	}
	VARIANT v;
	VariantInit(&v);
	V_VT(&v) = VT_DISPATCH;
	V_DISPATCH(&v) = pdisp;
	COleVariant *var = new COleVariant(v);
	pdisp->Release();
	pFont->Release();
	return var;
}

AGmNode *ADvOcx::ProgIDList() {
	HKEY	hkCLSID, hkClass, hkProgID, hkTemp;
	TCHAR	clsid[512], ProgID[512];
	AGmNode *n = Cons(NULL);

	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, "CLSID", 0,
			KEY_READ, &hkCLSID) != ERROR_SUCCESS)
		return n;
	TCHAR	 buff[256] ;
	DWORD	idx, size, type;
	FILETIME filetime;

	for (idx = 0, size = sizeof(clsid);; idx++, size = sizeof(clsid)) {
		if (RegEnumKeyEx(hkCLSID, idx, clsid, &size,
				NULL, NULL, NULL, &filetime) == ERROR_NO_MORE_ITEMS)
			break;
		if (RegOpenKeyEx(hkCLSID, clsid, 0, KEY_READ, &hkClass)
				!= ERROR_SUCCESS)
			continue;
		if (RegOpenKeyEx(hkClass, "ProgID", 0, KEY_READ, &hkProgID)
				!= ERROR_SUCCESS) {
			RegCloseKey(hkClass);
			continue;
		}
		type = REG_SZ;
		size = sizeof(ProgID);
		if (RegQueryValueEx(hkProgID, "", NULL, &type, (unsigned char*)ProgID, &size)
				!= ERROR_SUCCESS ) {
			RegCloseKey(hkProgID);
			RegCloseKey(hkClass);
			continue;
		}
		RegCloseKey(hkProgID);

		// OLE control
		wsprintf(buff, "CLSID\\%s\\Control", clsid) ;
		if (RegOpenKeyEx(HKEY_CLASSES_ROOT, buff, 0, KEY_READ, &hkTemp)
				!= ERROR_SUCCESS ) {
			RegCloseKey(hkProgID);
			RegCloseKey(hkClass);
			continue;
		}
		RegCloseKey(hkTemp);
		RegCloseKey(hkProgID);
		RegCloseKey(hkClass);
		CreateArc(n, String(ProgID));
	}
	RegCloseKey(hkCLSID);
	return n;
}
#endif
