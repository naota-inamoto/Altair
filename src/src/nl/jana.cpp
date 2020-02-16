/*
 * Copyright (C) 1995-2019 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#include "stdafx.h"

#include <gdm/gdm.h>
#include <gdm/string.h>
#include <gdm/file.h>
#include <interp/interp.h>

#include <knp/knp.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

int AIpProcess::EvalJpAna() {
	AGmNode *cmd, *arg1, *arg2, *arg3, *arg4;
	GetArg(cmd, arg1, arg2, arg3, arg4);
	if (!IsString(cmd)) {
		Unref(cmd, arg1, arg2, arg3, arg4);
		return interp_error("JpAna()");
	}
	const char *s = cmd->a.s.v->Str();
	Unref(val); val = NULL;
#ifdef JUMAN_KNP
	char str[FILENAME_MAX+128];
	if (strcmp(s, "juman_init") == 0) {
		if (!IsString(arg1)) {
#ifdef	_WINDOWS
			val = Ref(String("juman_init: 2nd argument must be dictionary directory."));
#endif
#ifdef	_LINUX
			val = Ref(String("juman_init: 2nd argument must be rcfile directory."));
#endif
		} else {
			if (al_juman_dic) {
				juman_close();

				fclose(Jumanrc_Fileptr);
				Jumanrc_Fileptr = NULL;

				free(al_juman_dic);
				al_juman_dic = NULL;
			}

			Show_Opt1 = Op_BB;
			Show_Opt2 = Op_E2;
			Show_Opt_jumanrc = 0;
			Show_Opt_tag[0] = '\0';
			Show_Opt_debug = 0;

			art_sprintf_1(str, FILENAME_MAX+128, "%s/jumanrc", arg1->a.s.v->Str());
			Jumanrc_Fileptr = art_fopen(str, "r");
			if (Jumanrc_Fileptr == NULL) {
				art_sprintf_1(str, FILENAME_MAX+128, "juman_init: can't open '%s/jumanrc'.", arg1->a.s.v->Str());
				val = Ref(String(str));
			} else {
				al_juman_dic = art_strdup(arg1->a.s.v->Str());
				if (!juman_init_rc(Jumanrc_Fileptr)) { /* rcfileŠÖŒW‚Ì‰Šú‰» */
					val = Ref(String("juman_dic: fail to initialize jumanrc."));

					fclose(Jumanrc_Fileptr);
					Jumanrc_Fileptr = NULL;

					free(al_juman_dic);
					al_juman_dic = NULL;
				} else {
					juman_init_etc(); /* –¢’è‹`Œêˆ—C”ŽŒˆ—C“§‰ßˆ—“™‚Ì‰Šú‰» */
				}
			}
		}
	} else if (strcmp(s, "juman_close") == 0) {
			if (al_juman_dic) {
				juman_close();

				fclose(Jumanrc_Fileptr);
				Jumanrc_Fileptr = NULL;

				free(al_juman_dic);
				al_juman_dic = NULL;
			} else {
				val = Ref(String("juman_close: juman_init not executed."));
			}
	} else if (strcmp(s, "juman_ana") == 0) {
		if (!IsString(arg1)) {
			val = Ref(String("juman_ana: 2nd argument must be read filename."));
		} else if (!IsString(arg2)) {
			val = Ref(String("juman_ana: 3rd argument must be write filename."));
		} else {
			FILE *in = art_fopen(arg1->a.s.v->Str(), "r");
			if (in == NULL) {
				art_sprintf_1(str, FILENAME_MAX+128, "juman_ana: can't open readfile '%s'.", arg1->a.s.v->Str());
				val = Ref(String(str));
			} else {
				FILE *out = art_fopen(arg2->a.s.v->Str(), "w");
				if (out == NULL) {
					art_sprintf_1(str, FILENAME_MAX+128, "juman_ana: can't open writefile '%s'.", arg2->a.s.v->Str());
					val = Ref(String(str));
					fclose(in);
				} else {
					StringBuff[LENMAX - 1] = '\n';
					while (fgets((char*)StringBuff, LENMAX, in) != NULL) {
						if (StringBuff[LENMAX - 1] != '\n') {
							StringBuff[LENMAX - 1] = '\0';
							ADvWindow::Print("Too long input string(");
							ADvWindow::Print((const char*)StringBuff);
							ADvWindow::Print(")");
							StringBuff[LENMAX - 1] = '\n';
							while (1) {
								char c;
								if ((c = (char)fgetc(in)) == EOF) break;
								if (c == '\n') break;
							}
							continue;
						}
#ifdef _WIN32
						char *eucstr = toStringEUC((const char*)StringBuff);
						art_strcpy((char*)StringBuff, LENMAX, eucstr);
						StringBuff[LENMAX - 1] = '\n';
						free(eucstr);
#endif			
						int length = (int)strlen((const char*)StringBuff);
						if (length == LENMAX-1 && StringBuff[length - 1] != '\n') {
							ADvWindow::Print("Too long input string(");
							ADvWindow::Print((const char*)StringBuff);
							ADvWindow::Print(")");
							continue;
						} else {
#ifdef _WIN32
							if (StringBuff[length - 2] == '\r' && StringBuff[length - 1] == '\n')
								StringBuff[length - 2] = '\0';
#endif
							if (StringBuff[length - 1] == '\n') StringBuff[length - 1] = '\0';
							else StringBuff[length] = '\0';
						}

						if (juman_sent() == 1) {
							print_homograph_path(out);
						}
						fprintf(out, "EOS\n");
						fflush(out);
					}
					fclose(out);
					fclose(in);
				}
			}
		}
	} else if (strcmp(s, "knp_init") == 0) {
		if (al_juman_dic == NULL) {
			val = Ref(String("knp_init: juman_init not executed."));
#ifdef	_WINDOWS
		} else if (!IsString(arg1)) {
			val = Ref(String("knp_init: 2nd argument must be dictionary directory."));
		} else if (!IsString(arg2)) {
			val = Ref(String("knp_init: 3rd argument must be rule directory."));
		} else {
			al_knp_dic = art_strdup(arg1->a.s.v->Str());
			al_knp_rule = art_strdup(arg2->a.s.v->Str());
			init_all();
		}
#endif
#ifdef	_LINUX
		} else if (!IsString(arg1)) {
			val = Ref(String("juman_init: 2nd argument must be rcfile directory."));
		} else {
			art_sprintf_1(str, FILENAME_MAX+128, "%s/knprc", arg1->a.s.v->Str());
			FILE *tmp = art_fopen(str, "r");
			if (tmp == NULL) {
				art_sprintf_1(str, FILENAME_MAX+128, "juman_init: can't open '%s/knprc'.", arg1->a.s.v->Str());
				val = Ref(String(str));
			} else {
				fclose(tmp);
				al_knp_dic = art_strdup(arg1->a.s.v->Str());
				Opt_knprc = str;
				init_all();
				Opt_knprc = NULL;
			}
		}
#endif
	} else if (strcmp(s, "knp_close") == 0) {
		if (al_knp_dic) {
			close_all();

			free(al_knp_dic); al_knp_dic = NULL;
			if (al_knp_rule) {
				free(al_knp_rule); al_knp_rule = NULL;
			}
		} else {
			val = Ref(String("knp_close: knp_init not executed."));
		}
	} else if (strcmp(s, "knp_ana") == 0) {
		if (!IsString(arg1)) {
			val = Ref(String("knp_ana: 2nd argument must be read filename."));
		} else if (!IsString(arg2)) {
			val = Ref(String("knp_ana: 3rd argument must be write filename."));
		} else {
			Infp = art_fopen(arg1->a.s.v->Str(), "r");
			if (Infp == NULL) {
				art_sprintf_1(str, FILENAME_MAX+128, "knp_ana: can't open readfile '%s'.", arg1->a.s.v->Str());
				val = Ref(String(str));
			} else {
				Outfp = art_fopen(arg2->a.s.v->Str(), "w");
				if (Outfp == NULL) {
					art_sprintf_1(str, FILENAME_MAX+128, "knp_ana: can't open writefile '%s'.", arg2->a.s.v->Str());
					val = Ref(String(str));
					fclose(Infp);
				} else {
					knp_main();
					fclose(Outfp);
					fclose(Infp);
				}
			}
		}
	} else {
		Unref(cmd, arg1, arg2, arg3, arg4);
		return interp_error("JpAna()");
	}
	Unref(cmd, arg1, arg2, arg3, arg4);
	return 0;
#else
	Unref(cmd, arg1, arg2, arg3, arg4);
	return interp_error("JUMAN/KNP version required.");
#endif
}

