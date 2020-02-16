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

#include <xml/xml.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

static int type_check(AGmString *value, char *type) {
	int i, len = value->Length(), cc;
	char c, *buf = (char*)value->Str();
	if (strcmp(type, "Integer") == 0) {
		for (i = 0; i < len; i++) {
			c = *buf++;
			if (c < '0' || c > '9') return 1;
		}
		if (*buf) return 2;

	} else if (strcmp(type, "NatuarlNumber") == 0) {
		for (i = 0; i < len; i++) {
			c = *buf++;
			if (i == 0 && c == '0') return 1;
			if (c < '0' || c > '9') return 1;
		}
		if (*buf) return 2;

	} else if (strncmp(type, "PositiveInteger", 15) == 0) {
		long l = 0;
		for (i = 0; i < len; i++) {
			c = *buf++;
			if (i == 0 && c == '0') return 1;
			if (c < '0' || c > '9') return 1;
			l = l * 10 + c - '0';
		}
		if (*buf) return 2;
		int max_value = -1;
		type += 15;
		while (*type == ' ') type++;
		if (*type && *type == '<' && *(type+1) && *(type+1) == '=') {
			type += 2;
			cc = art_sscanf_1(type, "%d", &max_value);
			if (cc > 0 && l > max_value) return 3;
		}

	} else if (strcmp(type, "Real") == 0) {
		int r1 = 1, r2 = 0;
		c = *buf++;
		if (c == '-') c = *buf++;
		while ('0' <= c && c <= '9') {
			r1 = 1;
			c = *buf++;
		}
		if (c == '.') {
			c = *buf++;
			while ('0' <= c && c <= '9') {
				r2 = 1;
				c = *buf++;
			}
		}
		if (r1 == 0 && r2 == 0) return 1;
		if (c == 'e' || c == 'E') {
			c = *buf++;
			if (c == '+' || c == '-') c = *buf++;
			if (c < '0' || c > '9') return 2;
			c = *buf++;
			while ('0' <= c && c <= '9') {
				c = *buf++;
			}
		}
		if (c) return 3;
	}
	return 0;
}

static int num_digit_check(AGmString *value, int min, int max) {
	int len = value->Length();
	if (min >= 0 && len < min) return 1;
	if (max >= 0 && len > max) return 1;
	return 0;
}

static int rep_check(AGmString *value, char *rep) {
	char *buf = (char*)value->Str();
	while (*rep) {
		char type = *rep;
		int count = 1, cc, i;
		rep++;
		if (*rep == '(') {
			rep++;
			cc = art_sscanf_1(rep, "%d", &count);
			rep += cc;
			if (*rep == ')') rep++;
		}
		int cont = 1;
		for (i = 0; cont && *buf && i < count; i++) {
			char c = *buf++;
			switch (type) {
			case '9':
				if (c < '0' || c > '9') {
					if (i == 0) return 1;
					--buf; cont = 0;
				}
				break;
			case 'V':
				if (c !=  '.') return 1;
				break;
			case 'X':
				break;
			}
		}
	}
	if (*buf) return 3;
	return 0;
}

int type_check(AGmString *value, AGmNode* type_info) {
	char *type = NULL, *rep = NULL; int min = -1, max = -1;
	for (;;) {
		AGmNode *n = type_info;
		if (n && AGm::IsList(n)) {
			if (AGm::IsString(n->H)) type = (char*)n->H->a.s.v->Str();
			n = n->T;
		} else {
			break;
		}
		if (n && AGm::IsList(n)) {
			if (AGm::IsInteger(n->H)) min = (int)n->H->a.i.v;
			n = n->T;
		} else {
			break;
		}
		if (n && AGm::IsList(n)) {
			if (AGm::IsInteger(n->H)) max = (int)n->H->a.i.v;
			n = n->T;
		} else {
			break;
		}
		if (n && AGm::IsList(n)) {
			if (AGm::IsString(n->H)) rep = (char*)n->H->a.s.v->Str();
			n = n->T;
		} else {
			break;
		}
		break;
	}
	if (type && type_check(value, type)) return 1;
	if (num_digit_check(value, min, max)) return 2;
	if (rep && *rep && rep_check(value, rep)) return 3;
	return 0;
}

int dic_check(AGmString *value, AGmNode* dic_info) {
	if (dic_info == NULL) return 0;
	AGmNode *l;
	for (l = dic_info; l; l = l->T) {
		AGmNode *n = l->H;
		if (!AGm::IsString(n)) continue;
		if (n->a.s.v->Compare(value->Str()) == 0) return 0;
	}
	return 1;
}

