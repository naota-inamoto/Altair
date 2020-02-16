/*
 * Copyright (c) 1999-2005 The OpenSSL Project.	All rights reserved.
 * Copyright (C) 1995-2020 Naota Inamoto, All rights reserved.
 *
 * This file uses the modified partial source codes of
 * 'pk7_mime.c', 'pk7_smime.c' and	'pk7_doit.c' of OpenSSL
 * in order to handle mime/multipart and SMIME signature.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying readme.txt file
 */

/* pk7_mime.c */
/* pk7_smime.c */
/* Written by Dr Stephen N Henson (shenson@bigfoot.com) for the OpenSSL
 * project 2011.
 */
/* ====================================================================
 * Copyright (c) 1999 The OpenSSL Project.	All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in
 *	  the documentation and/or other materials provided with the
 *	  distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *	  software must display the following acknowledgment:
 *	  "This product includes software developed by the OpenSSL Project
 *	  for use in the OpenSSL Toolkit. (http://www.OpenSSL.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *	  endorse or promote products derived from this software without
 *	  prior written permission. For written permission, please contact
 *	  licensing@OpenSSL.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *	  nor may "OpenSSL" appear in their names without prior written
 *	  permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *	  acknowledgment:
 *	  "This product includes software developed by the OpenSSL Project
 *	  for use in the OpenSSL Toolkit (http://www.OpenSSL.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.	IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */

/* crypto/pkcs7/pk7_doit.c */
/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 *	
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.  The following conditions
 * apply to all code found in this distribution, be it the RC4, RSA,
 * lhash, DES, etc., code; not just the SSL code.  The SSL documentation
 * included with this distribution is covered by the same copyright terms
 * except that the holder is Tim Hudson (tjh@cryptsoft.com).
 *	
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 * If this package is used in a product, Eric Young should be given attribution
 * as the author of the parts of the library used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 *	
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *	  notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *	  must display the following acknowledgement:
 *	  "This product includes cryptographic software written by
 *	   Eric Young (eay@cryptsoft.com)"
 *	  The word 'cryptographic' can be left out if the rouines from the library
 *	  being used are not cryptographic related :-).
 * 4. If you include any Windows specific code (or a derivative thereof) from 
 *	  the apps directory (application code) you must include an acknowledgement:
 *	  "This product includes software written by Tim Hudson (tjh@cryptsoft.com)"
 *	
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.	IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *	
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 */

#include "stdafx.h"

#include <gdm/gdm.h>
#include <gdm/string.h>

#include <net/multipart.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#ifdef	_AL_OPENSSL

/* 'pk7_mime.c' */

#define MAX_SMLEN2	(MAX_SMLEN * 4)
long num_read_char;

STACK_OF(MIME_HEADER) *openssl_mime_parse_hdr(BIO *bio) {
	char *p, *q, c, *ntmp, linebuf[MAX_SMLEN2];
	MIME_HEADER *mhdr = NULL;
	STACK_OF(MIME_HEADER) *headers;
	int len, state, save_state = 0;

	num_read_char = 0;

	headers = sk_MIME_HEADER_new(openssl_mime_hdr_cmp);
	while ((len = BIO_gets(bio, linebuf, MAX_SMLEN2)) > 0) {
		num_read_char += len;
		/* If whitespace at line start then continuation line */
		if (mhdr && isspace((unsigned char)linebuf[0])) state = MIME_NAME;
		else state = MIME_START;
		ntmp = NULL;
		/* Go through all characters */
		for (p = linebuf, q = linebuf; (c = *p) != 0 && (c!='\r') && (c!='\n'); p++) {
			/* State machine to handle MIME headers
			 * if this looks horrible that's because it *is*
			 */
			switch (state) {
			case MIME_START:
				if (c == ':') {
					state = MIME_TYPE;
					*p = 0;
					ntmp = openssl_strip_ends(q);
					q = p + 1;
				}
				break;
			case MIME_TYPE:
				if (c == ';') {
					*p = 0;
					mhdr = openssl_mime_hdr_new(ntmp, openssl_strip_ends(q));
					sk_MIME_HEADER_push(headers, mhdr);
					ntmp = NULL;
					q = p + 1;
					state = MIME_NAME;
				} else if (c == '(') {
					save_state = state;
					state = MIME_COMMENT;
				}
				break;
			case MIME_COMMENT:
				if (c == ')') {
					state = save_state;
				}
				break;
			case MIME_NAME:
				if (c == '=') {
					state = MIME_VALUE;
					*p = 0;
					ntmp = openssl_strip_ends(q);
					q = p + 1;
				}
				break;
			case MIME_VALUE:
				if (c == ';') {
					state = MIME_NAME;
					*p = 0;
					openssl_mime_hdr_addparam(mhdr, ntmp, openssl_strip_ends(q));
					ntmp = NULL;
					q = p + 1;
				} else if (c == '"') {
					state = MIME_QUOTE;
				} else if (c == '(') {
					save_state = state;
					state = MIME_COMMENT;
				}
				break;
			case MIME_QUOTE:
				if (c == '"') {
					state = MIME_VALUE;
				}
				break;
			}
		}
		if( state == MIME_TYPE) {
			mhdr = openssl_mime_hdr_new(ntmp, openssl_strip_ends(q));
			sk_MIME_HEADER_push(headers, mhdr);
		} else if (state == MIME_VALUE)
			openssl_mime_hdr_addparam(mhdr, ntmp, openssl_strip_ends(q));
		if (p == linebuf) break;	/* Blank line means end of headers */
	}
	return headers;
}

MIME_HEADER *openssl_mime_hdr_new(char *name, char *value) {
	MIME_HEADER *mhdr;
	char *tmpname, *tmpval /*, *p*/;
	/*int c;*/
	if (name) {
		if ((tmpname = BUF_strdup(name)) == 0) return NULL;
		/*for (p = tmpname ; *p; p++) {
			c = *p;
			if (isupper(c)) {
				c = tolower(c);
				*p = c;
			}
		}*/
	} else tmpname = NULL;
	if (value) {
		if ((tmpval = BUF_strdup(value)) == 0) return NULL;
		/*for (p = tmpval ; *p; p++) {
			c = *p;
			if (isupper(c)) {
				c = tolower(c);
				*p = c;
			}
		}*/
	} else tmpval = NULL;
	mhdr = (MIME_HEADER *) OPENSSL_malloc(sizeof(MIME_HEADER));
	if (!mhdr) return NULL;
	mhdr->name = tmpname;
	mhdr->value = tmpval;
	if ((mhdr->params = sk_MIME_PARAM_new(openssl_mime_param_cmp)) == 0) return NULL;
	return mhdr;
}
		
int openssl_mime_hdr_addparam(MIME_HEADER *mhdr, char *name, char *value) {
	char *tmpname, *tmpval /*, *p*/;
	/*int c;*/
	MIME_PARAM *mparam;
	if (name) {
		tmpname = BUF_strdup(name);
		if (!tmpname) return 0;
		/*for (p = tmpname ; *p; p++) {
			c = *p;
			if (isupper(c)) {
				c = tolower(c);
				*p = c;
			}
		}*/
	} else tmpname = NULL;
	if (value) {
		tmpval = BUF_strdup(value);
		if (!tmpval) return 0;
	} else tmpval = NULL;
	mparam = (MIME_PARAM *) OPENSSL_malloc(sizeof(MIME_PARAM));
	if (!mparam) return 0;
	mparam->param_name = tmpname;
	mparam->param_value = tmpval;
	sk_MIME_PARAM_push(mhdr->params, mparam);
	return 1;
}

int openssl_mime_hdr_cmp(const MIME_HEADER * const *a, const MIME_HEADER * const *b) {
	return(strcmp((*a)->name, (*b)->name));
}

int openssl_mime_param_cmp(const MIME_PARAM * const *a, const MIME_PARAM * const *b) {
	return(strcmp((*a)->param_name, (*b)->param_name));
}

char *openssl_strip_ends(char *name) {
	return openssl_strip_end(openssl_strip_start(name));
}
char *openssl_strip_start(char *name) {
	char *p, c;
	/* Look for first non white space or quote */
	for (p = name; (c = *p) != 0 ;p++) {
		if (c == '"') {
			/* Next char is start of string if non null */
			if(p[1]) return p + 1;
			/* Else null string */
			return NULL;
		}
		if (!isspace((unsigned char)c)) return p;
	}
	return NULL;
}
char *openssl_strip_end(char *name) {
	char *p, c;
	if (!name) return NULL;
	/* Look for first non white space or quote */
	for (p = name + strlen(name) - 1; p >= name ;p--) {
		c = *p;
		if (c == '"') {
			if(p - 1 == name) return NULL;
			*p = 0;
			return name;
		}
		if (isspace((unsigned char)c)) *p = 0;	
		else return name;
	}
	return NULL;
}

void openssl_mime_hdr_free(MIME_HEADER *hdr) {
	if (hdr->name) OPENSSL_free(hdr->name);
	if (hdr->value) OPENSSL_free(hdr->value);
	if (hdr->params) sk_MIME_PARAM_pop_free(hdr->params, openssl_mime_param_free);
	OPENSSL_free(hdr);
}

void openssl_mime_param_free(MIME_PARAM *param) {
	if (param->param_name) OPENSSL_free(param->param_name);
	if (param->param_value) OPENSSL_free(param->param_value);
	OPENSSL_free(param);
}

int openssl_multi_split(BIO *bio, char *bound, STACK_OF(BIO) **ret) {
	char linebuf[MAX_SMLEN2];
	int len, blen;
	BIO *bpart = NULL;
	STACK_OF(BIO) *parts;
	char state, part, first;

	char crlf[16];
	int crlf_index = 0;

	blen = (int)strlen(bound);
	part = 0;
	state = 0;
	first = 1;
	parts = sk_BIO_new_null();
	*ret = parts;

	while ((len = BIO_gets(bio, linebuf, MAX_SMLEN2)) > 0) {
		state = (char)openssl_mime_bound_check(linebuf, len, bound, blen);
		if (state == 1) {
			first = 1;
			part++;
			crlf_index = 0;
		} else if (state == 2) {
			sk_BIO_push(parts, bpart);
			return 1;
		} else if (part) {
			if (first) {
				first = 0;
				if (bpart) sk_BIO_push(parts, bpart);
				bpart = BIO_new(BIO_s_mem());	
			} else {
				while (crlf_index > 0) {
					BIO_write(bpart, &crlf[--crlf_index], 1);
				}
				/* BIO_write(bpart, "\r\n", 2); */
			}
			/* Strip CR+LF from linebuf */
			/* while (openssl_iscrlf(linebuf[len - 1])) len--; */
			while (openssl_iscrlf(linebuf[len - 1])) {
				if (crlf_index < 32) crlf[crlf_index++] = linebuf[len - 1];
				len--;
			}
			BIO_write(bpart, linebuf, len);
		}
	}
	return 0;
}
int openssl_iscrlf(char c) {
	if(c == '\r' || c == '\n') return 1;
	return 0;
}
int openssl_mime_bound_check(char *line, int linelen, char *bound, int blen) {
	if (linelen == -1) linelen = (int)strlen(line);
	if (blen == -1) blen = (int)strlen(bound);
	/* Quickly eliminate if line length too short */
	if (blen + 2 > linelen) return 0;
	/* Check for part boundary */
	if (!strncmp(line, "--", 2) && !strncmp(line + 2, bound, blen)) {
		if (!strncmp(line + blen + 2, "--", 2)) return 2;
		else return 1;
	}
	return 0;
}

MIME_HEADER *openssl_mime_hdr_find(STACK_OF(MIME_HEADER) *hdrs, char *name) {
	/*
	MIME_HEADER htmp;
	int idx;
	htmp.name = name;
	idx = sk_MIME_HEADER_find(hdrs, &htmp);
	if(idx < 0) return NULL;
	return sk_MIME_HEADER_value(hdrs, idx);
	*/
	AGmString *str = new AGmString(name);
	int i, n = sk_MIME_HEADER_num(hdrs);
	for (i = 0; i < n; i++) {
		MIME_HEADER* hdr = sk_MIME_HEADER_value(hdrs, i);
		if (str->CompareI(hdr->name) == 0) {
			delete str;
			return hdr;
		}
	}
	delete str;
	return NULL;
}

MIME_PARAM *openssl_mime_param_find(MIME_HEADER *hdr, char *name) {
	MIME_PARAM param;
	int idx;
	param.param_name = name;
	idx = sk_MIME_PARAM_find(hdr->params, &param);
	if(idx < 0) return NULL;
	return sk_MIME_PARAM_value(hdr->params, idx);
}

PKCS7 *openssl_B64_read_PKCS7(BIO *bio) {
	BIO *b64;
	PKCS7 *p7;
	if((b64 = BIO_new(BIO_f_base64())) == 0) {
		PKCS7err(PKCS7_F_B64_READ_PKCS7,ERR_R_MALLOC_FAILURE);
		return 0;
	}
	bio = BIO_push(b64, bio);
	if((p7 = d2i_PKCS7_bio(bio, NULL)) == 0)	
		PKCS7err(PKCS7_F_B64_READ_PKCS7,PKCS7_R_DECODE_ERROR);
	BIO_flush(bio);
	bio = BIO_pop(bio);
	BIO_free(b64);
	return p7;
}

PKCS7 *openssl_SMIME_read_PKCS7(BIO *bio, BIO **bcont)
{
	BIO *p7in;
	STACK_OF(MIME_HEADER) *headers = NULL;
	STACK_OF(BIO) *parts = NULL;
	MIME_HEADER *hdr;
	MIME_PARAM *prm;
	PKCS7 *p7;
	int ret;

	if(bcont) *bcont = NULL;

	if ((headers = openssl_mime_parse_hdr(bio)) == 0) {
		PKCS7err(PKCS7_F_SMIME_READ_PKCS7,PKCS7_R_MIME_PARSE_ERROR);
		return NULL;
	}

	if((hdr = openssl_mime_hdr_find(headers, AL_STR("content-type"))) == 0 || !hdr->value) {
		sk_MIME_HEADER_pop_free(headers, openssl_mime_hdr_free);
		PKCS7err(PKCS7_F_SMIME_READ_PKCS7, PKCS7_R_NO_CONTENT_TYPE);
		return NULL;
	}

	/* Handle multipart/signed */

	if(!strcmp(hdr->value, "multipart/signed")) {
		/* Split into two parts */
		prm = openssl_mime_param_find(hdr, AL_STR("boundary"));
		if(!prm || !prm->param_value) {
			sk_MIME_HEADER_pop_free(headers, openssl_mime_hdr_free);
			PKCS7err(PKCS7_F_SMIME_READ_PKCS7, PKCS7_R_NO_MULTIPART_BOUNDARY);
			return NULL;
		}
		ret = openssl_multi_split(bio, prm->param_value, &parts);
		sk_MIME_HEADER_pop_free(headers, openssl_mime_hdr_free);
		if(!ret || (sk_BIO_num(parts) != 2) ) {
			PKCS7err(PKCS7_F_SMIME_READ_PKCS7, PKCS7_R_NO_MULTIPART_BODY_FAILURE);
			sk_BIO_pop_free(parts, BIO_vfree);
			return NULL;
		}

		/* Parse the signature piece */
		p7in = sk_BIO_value(parts, 1);

		if ((headers = openssl_mime_parse_hdr(p7in)) == 0) {
			PKCS7err(PKCS7_F_SMIME_READ_PKCS7,PKCS7_R_MIME_SIG_PARSE_ERROR);
			sk_BIO_pop_free(parts, BIO_vfree);
			return NULL;
		}

		/* Get content type */

		if((hdr = openssl_mime_hdr_find(headers, AL_STR("content-type"))) == 0 ||
								 !hdr->value) {
			sk_MIME_HEADER_pop_free(headers, openssl_mime_hdr_free);
			PKCS7err(PKCS7_F_SMIME_READ_PKCS7, PKCS7_R_NO_SIG_CONTENT_TYPE);
			return NULL;
		}

		if(strcmp(hdr->value, "application/x-pkcs7-signature") &&
			strcmp(hdr->value, "application/pkcs7-signature")) {
			sk_MIME_HEADER_pop_free(headers, openssl_mime_hdr_free);
			PKCS7err(PKCS7_F_SMIME_READ_PKCS7,PKCS7_R_SIG_INVALID_MIME_TYPE);
			ERR_add_error_data(2, "type: ", hdr->value);
			sk_BIO_pop_free(parts, BIO_vfree);
			return NULL;
		}
		sk_MIME_HEADER_pop_free(headers, openssl_mime_hdr_free);
		/* Read in PKCS#7 */
		if((p7 = openssl_B64_read_PKCS7(p7in)) == 0) {
			PKCS7err(PKCS7_F_SMIME_READ_PKCS7,PKCS7_R_PKCS7_SIG_PARSE_ERROR);
			sk_BIO_pop_free(parts, BIO_vfree);
			return NULL;
		}

		if(bcont) {
			*bcont = sk_BIO_value(parts, 0);
			BIO_free(p7in);
			sk_BIO_free(parts);
		} else sk_BIO_pop_free(parts, BIO_vfree);
		return p7;
	}
		
	/* OK, if not multipart/signed try opaque signature */

	if (strcmp (hdr->value, "application/x-pkcs7-mime") &&
		strcmp (hdr->value, "application/pkcs7-mime")) {
		PKCS7err(PKCS7_F_SMIME_READ_PKCS7,PKCS7_R_INVALID_MIME_TYPE);
		ERR_add_error_data(2, "type: ", hdr->value);
		sk_MIME_HEADER_pop_free(headers, openssl_mime_hdr_free);
		return NULL;
	}

	sk_MIME_HEADER_pop_free(headers, openssl_mime_hdr_free);
	
	if((p7 = openssl_B64_read_PKCS7(bio)) == 0) {
		PKCS7err(PKCS7_F_SMIME_READ_PKCS7, PKCS7_R_PKCS7_PARSE_ERROR);
		return NULL;
	}
	return p7;
}

/* 'pk7_smime.c' */

#include <openssl/x509.h>
#include <openssl/x509v3.h>

int openssl_PKCS7_verify(PKCS7 *p7, STACK_OF(X509) *certs, X509_STORE *store,
					BIO *indata, BIO *out, int flags)
{
	STACK_OF(X509) *signers;
	X509 *signer;
	STACK_OF(PKCS7_SIGNER_INFO) *sinfos;
	PKCS7_SIGNER_INFO *si;
	X509_STORE_CTX cert_ctx;
	char buf[4096];
	int i, j=0, k, ret = 0;
	BIO *p7bio;
	BIO *tmpout;

	if(!p7) {
		PKCS7err(PKCS7_F_PKCS7_VERIFY,PKCS7_R_INVALID_NULL_POINTER);
		return 0;
	}

	if(!PKCS7_type_is_signed(p7)) {
		PKCS7err(PKCS7_F_PKCS7_VERIFY,PKCS7_R_WRONG_CONTENT_TYPE);
		return 0;
	}

	/* Check for no data and no content: no data to verify signature */
	if(PKCS7_get_detached(p7) && !indata) {
		PKCS7err(PKCS7_F_PKCS7_VERIFY,PKCS7_R_NO_CONTENT);
		return 0;
	}
#if 0
	/* NB: this test commented out because some versions of Netscape
	 * illegally include zero length content when signing data.
	 */

	/* Check for data and content: two sets of data */
	if(!PKCS7_get_detached(p7) && indata) {
				PKCS7err(PKCS7_F_PKCS7_VERIFY,PKCS7_R_CONTENT_AND_DATA_PRESENT);
		return 0;
	}
#endif

	sinfos = PKCS7_get_signer_info(p7);

	if(!sinfos || !sk_PKCS7_SIGNER_INFO_num(sinfos)) {
		PKCS7err(PKCS7_F_PKCS7_VERIFY,PKCS7_R_NO_SIGNATURES_ON_DATA);
		return 0;
	}


	signers = PKCS7_get0_signers(p7, certs, flags);

	if(!signers) return 0;

	/* Now verify the certificates */

	if (!(flags & PKCS7_NOVERIFY)) for (k = 0; k < sk_X509_num(signers); k++) {
		signer = sk_X509_value (signers, k);
		if (!(flags & PKCS7_NOCHAIN)) {
			X509_STORE_CTX_init(&cert_ctx, store, signer,
							p7->d.sign->cert);
			X509_STORE_CTX_set_purpose(&cert_ctx,
						X509_PURPOSE_SMIME_SIGN);
		} else X509_STORE_CTX_init (&cert_ctx, store, signer, NULL);
		i = X509_verify_cert(&cert_ctx);
		if (i <= 0) j = X509_STORE_CTX_get_error(&cert_ctx);
		X509_STORE_CTX_cleanup(&cert_ctx);
		if (i <= 0) {
			PKCS7err(PKCS7_F_PKCS7_VERIFY,PKCS7_R_CERTIFICATE_VERIFY_ERROR);
			ERR_add_error_data(2, "Verify error:",
					 X509_verify_cert_error_string(j));
			sk_X509_free(signers);
			return 0;
		}
		/* Check for revocation status here */
	}

	p7bio=PKCS7_dataInit(p7,indata);

	if(flags & PKCS7_TEXT) {
		if((tmpout = BIO_new(BIO_s_mem())) == 0) {
			PKCS7err(PKCS7_F_PKCS7_VERIFY,ERR_R_MALLOC_FAILURE);
			goto err;
		}
	} else tmpout = out;

	/* We now have to 'read' from p7bio to calculate digests etc. */
	for (;;)
	{
		i=BIO_read(p7bio,buf,sizeof(buf));
		if (i <= 0) break;
		if (tmpout) BIO_write(tmpout, buf, i);
	}

	if(flags & PKCS7_TEXT) {
		if(!SMIME_text(tmpout, out)) {
			PKCS7err(PKCS7_F_PKCS7_VERIFY,PKCS7_R_SMIME_TEXT_ERROR);
			BIO_free(tmpout);
			goto err;
		}
		BIO_free(tmpout);
	}

	/* Now Verify All Signatures */
	if (!(flags & PKCS7_NOSIGS))
		for (i=0; i<sk_PKCS7_SIGNER_INFO_num(sinfos); i++)
		{
		si=sk_PKCS7_SIGNER_INFO_value(sinfos,i);
		signer = sk_X509_value (signers, i);
		j=openssl_PKCS7_signatureVerify(p7bio,p7,si, signer);
		if (j <= 0) {
			PKCS7err(PKCS7_F_PKCS7_VERIFY,PKCS7_R_SIGNATURE_FAILURE);
			goto err;
		}
	}

	ret = 1;

	err:

	if(indata) BIO_pop(p7bio);
	BIO_free_all(p7bio);
	sk_X509_free(signers);

	return ret;
}

/* 'pk7_doit.c' */

static unsigned char md_dat[EVP_MAX_MD_SIZE];
static unsigned int md_len;

void getMessageDigest(unsigned char** dat, unsigned int *len) {
	*dat = md_dat;
	*len = md_len;
}

int openssl_PKCS7_signatureVerify(BIO *bio, PKCS7 *p7, PKCS7_SIGNER_INFO *si,
								X509 *x509)
	{
	ASN1_OCTET_STRING *os;
	EVP_MD_CTX mdc_tmp,*mdc;
	unsigned char *pp;
	int ret=0,i;
	int md_type;
	STACK_OF(X509_ATTRIBUTE) *sk;
	BIO *btmp;
	EVP_PKEY *pkey;

	EVP_MD_CTX_init(&mdc_tmp);

	if (!PKCS7_type_is_signed(p7) &&	
				!PKCS7_type_is_signedAndEnveloped(p7)) {
		PKCS7err(PKCS7_F_PKCS7_SIGNATUREVERIFY,
						PKCS7_R_WRONG_PKCS7_TYPE);
		goto err;
	}

	md_type=OBJ_obj2nid(si->digest_alg->algorithm);

	btmp=bio;
	for (;;)
		{
		if ((btmp == NULL) ||
			((btmp=BIO_find_type(btmp,BIO_TYPE_MD)) == NULL))
			{
			PKCS7err(PKCS7_F_PKCS7_SIGNATUREVERIFY,
					PKCS7_R_UNABLE_TO_FIND_MESSAGE_DIGEST);
			goto err;
			}
		BIO_get_md_ctx(btmp,&mdc);
		if (mdc == NULL)
			{
			PKCS7err(PKCS7_F_PKCS7_SIGNATUREVERIFY,
							ERR_R_INTERNAL_ERROR);
			goto err;
			}
		if (EVP_MD_CTX_type(mdc) == md_type)
			break;
		if (EVP_MD_pkey_type(EVP_MD_CTX_md(mdc)) == md_type)
			break;
		btmp=BIO_next(btmp);
		}

	/* mdc is the digest ctx that we want, unless there are attributes,
	 * in which case the digest is the signed attributes */
	EVP_MD_CTX_copy_ex(&mdc_tmp,mdc);

	sk=si->auth_attr;
	if ((sk != NULL) && (sk_X509_ATTRIBUTE_num(sk) != 0))
		{
		//unsigned char md_dat[EVP_MAX_MD_SIZE], *abuf = NULL;
		//unsigned int md_len, alen;
		ASN1_OCTET_STRING *message_digest;

		EVP_DigestFinal(&mdc_tmp,md_dat,&md_len);
		message_digest=PKCS7_digest_from_attributes(sk);
		if (!message_digest)
			{
			PKCS7err(PKCS7_F_PKCS7_SIGNATUREVERIFY,
					PKCS7_R_UNABLE_TO_FIND_MESSAGE_DIGEST);
			goto err;
			}
		if ((message_digest->length != (int)md_len) ||
			(memcmp(message_digest->data,md_dat,md_len)))
			{
#if 0
{
int ii;
for (ii=0; ii<message_digest->length; ii++)
	printf("%02X",message_digest->data[ii]); printf(" sent\n");
for (ii=0; ii<md_len; ii++)
	printf("%02X",md_dat[ii]); printf(" calc\n");
}
#endif
			PKCS7err(PKCS7_F_PKCS7_SIGNATUREVERIFY,
							PKCS7_R_DIGEST_FAILURE);
			ret= -1;
			goto err;
			}

		EVP_VerifyInit_ex(&mdc_tmp,EVP_get_digestbynid(md_type), NULL);

		i = ASN1_item_i2d((ASN1_VALUE *)sk, &pp,
						ASN1_ITEM_rptr(PKCS7_ATTR_VERIFY));
		EVP_VerifyUpdate(&mdc_tmp, pp, i);

		OPENSSL_free(pp);
		}

	os=si->enc_digest;
	pkey = X509_get_pubkey(x509);
	if (!pkey)
		{
		ret = -1;
		goto err;
		}
#ifndef NO_DSA
	if(pkey->type == EVP_PKEY_DSA) mdc_tmp.digest=EVP_dss1();
#endif

	i=EVP_VerifyFinal(&mdc_tmp,os->data,os->length, pkey);
	EVP_PKEY_free(pkey);
	if (i <= 0)
		{
		PKCS7err(PKCS7_F_PKCS7_SIGNATUREVERIFY,
						PKCS7_R_SIGNATURE_FAILURE);
		ret= -1;
		goto err;
		}
	else
		ret=1;
err:
	EVP_MD_CTX_cleanup(&mdc_tmp);
	return(ret);
}

#endif
