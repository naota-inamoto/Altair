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
#include <gdm/big_int.h>
#include <interp/interp.h>
#include <interp/prcsmgr.h>

#include <net/socket.h>
#include <net/crypt.h>
#include <net/multipart.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#if defined(_DEBUG) || defined(_LINUX_DEBUG)
//#define	_DEBUG_NDES
#endif

#if 0
#define LINE_BREAK_LF
#else
#define LINE_BREAK_CRLF
#endif

// encrypt/decrypt function
#define BLOCK_SIZE 16
#define BLOCK_BUFF (BLOCK_SIZE * 256)
extern char *crypt_key;
AGmNode* SimpleEncryptDecrypt(AGmNode* key, AGmNode *infile, AGmNode* outfile, int dec) {
	if (!AGm::IsString(infile)) return AGm::Integer(100);
	FILE *in = art_fopen(infile->a.s.v->Str(), "rb");
	if (in == NULL) return AGm::Integer(101);
	if (!AGm::IsString(outfile)) return AGm::Integer(200);
	FILE *out = art_fopen(outfile->a.s.v->Str(), "wb");
	if (out == NULL) { fclose(in); return AGm::Integer(201); }
	AGmBinary *k0 = NULL, *k = NULL;
	if (AGm::IsBinary(key)) {
		k0 = key->a.bin.v->Copy();
	} else if (AGm::IsString(key)) {
		k0 = new AGmBinary(key->a.s.v->Length());
		k0->Copy(0, key->a.s.v);
	} else if (key == NULL) {
		AGmString *s = new AGmString(crypt_key);
		k0 = new AGmBinary(s->Length());
		k0->Copy(0, s);
		delete s;
	} else {
		return AGm::Integer(300);
	}
	k = new AGmBinary(16);
	md5_context *ctx = _al_md5_init();
	_al_md5_update(ctx, k0->Length(), k0->Buffer());
	_al_md5_final(ctx, k->Buffer());
	delete k0;

	n_des_context *context = _al_ndes_init(BLOCK_SIZE, k->Length(), k->Buffer());

	int file_size = 0;
	if (dec == 0) {
		struct stat buff;
		AGmFile::GetStat(infile->a.s.v->Str(), &buff);
		file_size = buff.st_size;
		art_write(art_fileno(out), &file_size, 4);
	} else if (dec == 1) {
		art_read(art_fileno(in), &file_size, 4);
	}

	unsigned char in_buf[BLOCK_BUFF], out_buf[BLOCK_BUFF];
	int left, left2, read_size, sz, size;
	if (dec == 0) {
		for (left = file_size; left > 0; left -= read_size) {
			read_size = BLOCK_BUFF;
			if (read_size > left) read_size = left;

			for (sz = read_size; sz > 0;) {
				int cc = art_read(art_fileno(in), in_buf, sz);
				sz -= cc; if (cc < 0) sz = 0;
			}

			for (size = read_size; size % BLOCK_SIZE; size++) in_buf[size] = 0x20;
			_al_ndes_encrypt(context, size, in_buf, out_buf);

			for (sz = size; sz > 0;) {
				int cc = art_write(art_fileno(out), out_buf, sz);
				sz -= cc; if (cc < 0) sz = 0;
			}
		}
	} else if (dec == 1) {
		for (left = left2 = file_size; left % BLOCK_SIZE; left++) ;
		for (; left > 0; left -= read_size, left2 -= read_size) {
			read_size = BLOCK_BUFF;
			if (read_size > left) read_size = left;

			for (sz = read_size; sz > 0;) {
				int cc = art_read(art_fileno(in), in_buf, sz);
				sz -= cc; if (cc < 0) sz = 0;
			}

			_al_ndes_decrypt(context, read_size, in_buf, out_buf);

			sz = read_size;
			if (sz > left2) sz = left2; 
			for (; sz > 0;) {
				int cc = art_write(art_fileno(out), out_buf, sz);
				sz -= cc; if (cc < 0) sz = 0;
			}
		}
	}

	_al_ndes_final(context);
	delete k;
	fclose(in); fclose(out);
	return NULL;
}

int AIpProcess::EvalCrypt() {
	AGmNode *cmd, *arg1, *arg2, *arg3;
	GetArg(cmd, arg1, arg2, arg3);
	if (!IsString(cmd) )	{
		Unref(cmd, arg1, arg2, arg3);
		return interp_error("Crypt()");
	}
	const char *s = cmd->a.s.v->Str();
	Unref(val); val = NULL;
	AGmNode *tail;
	if (strcmp(s, "generate_keypair") == 0) {
		if (IsString(arg1)) {
			const char *a = arg1->a.s.v->Str();
			if (strcmp(a, "N-RSA") == 0) {
#if	_WINDOWS && _64BIT
				long long p_hint, q_hint, s_hint;
#else
				long p_hint, q_hint, s_hint;
#endif
				if (Param(arg2, p_hint, tail) == 0 &&
						Param(tail, q_hint, tail) == 0 &&
						Param(tail, s_hint, tail) == 0 &&
						p_hint > 0 && q_hint > 0 && s_hint > 0) {

					AIpProcessMgr *pm = AIpProcessMgr::Instance();
					pm->AddLocalLoopProcess(this);
					AGmNode *sk, *pk, *mod;
					int ok = _al_nrsa_generate_keypair(
						(unsigned long)p_hint, (unsigned long)q_hint, (unsigned long)s_hint, sk, pk, mod);
					pm->RemoveLocalLoopProcess(this);
			
					if (ok) {
						// public key
						AGmNode *p_key = List(pk, mod);
						// secret key
						AGmNode *s_key = List(sk, mod);
						// keypair
						val = Ref(List(p_key, s_key));
					}
				}
			}
		}
	} else if (strcmp(s, "encrypt_init") == 0 || strcmp(s, "decrypt_init") == 0) {
		if (IsString(arg1) && IsInteger(arg3)) {
			const char *a = arg1->a.s.v->Str();
			long size_block = (long)arg3->a.i.v;
			if (strcmp(a, "N-RSA") == 0) {
				AGmNode *key, *mod; AGmNode *tail;
				if (Param(arg2, key, tail) == 0 && Param(tail, mod, tail) == 0) {
					n_rsa_context *context = _al_nrsa_init(size_block, key, mod);
					if (context) {
						val = Ref(List(Integer((long)0), PtrCell(context)));
					}
				}
			} else if (strcmp(a, "N-DES") == 0) {
				if (IsBinary(arg2)) {
					AGmBinary *bin = arg2->a.bin.v;
					n_des_context *context =
						_al_ndes_init(size_block, bin->Length(), bin->Buffer());
					val = Ref(List(Integer((long)1), PtrCell(context)));
				}
			}
		}
	} else if (strcmp(s, "encrypt_final") == 0 || strcmp(s, "decrypt_final") == 0) {
#if	_WINDOWS && _64BIT
		long long a;
#else
		long a;
#endif
		AGmNode *k, *tail;
		if (Param(arg1, a, tail) == 0 && Param(tail, k, tail) == 0) {
			if (a == 0) {
				n_rsa_context *context = (n_rsa_context*)k->a.p.ptr;
				_al_nrsa_final(context);
			} else if (a == 1) {
				n_des_context *context = (n_des_context*)k->a.p.ptr;
				_al_ndes_final(context);
			}
		}
	} else if (strcmp(s, "encrypt") == 0) {
#if	_WINDOWS && _64BIT
		long long a;
		long long src_idx, src_size;
		long long dst_idx;
#else
		long a;
		long src_idx, src_size;
		long dst_idx;
#endif
		AGmNode *k, *tail;
		AGmBinary *src_bin;
		AGmBinary *dst_bin;
		if (Param(arg1, a, tail) == 0 && Param(tail, k, tail) == 0 &&
				Param(arg2, src_bin, tail) == 0 &&
				Param(tail, src_idx, src_size, tail) == 0 &&
				src_idx >= 0 && src_idx + src_size <= src_bin->Length() &&
				Param(arg3, dst_bin, tail) == 0 &&
				Param(tail, dst_idx, tail) == 0 && dst_idx >= 0) {
			if (a == 0) {
				n_rsa_context *context = (n_rsa_context*)k->a.p.ptr;
				if (dst_bin->Length() - dst_idx >= src_size / context->size_block * context->size_mod) {
					_al_nrsa_encrypt(context,
						(long)src_size, &src_bin->Buffer()[src_idx], &dst_bin->Buffer()[dst_idx]);
				}
			} else if (a == 1 && dst_bin->Length() - dst_idx >= src_size) {
				n_des_context *context = (n_des_context*)k->a.p.ptr;
				_al_ndes_encrypt(context,
					(long)src_size, &src_bin->Buffer()[src_idx], &dst_bin->Buffer()[dst_idx]);
			}
		}
	} else if (strcmp(s, "decrypt") == 0) {
#if	_WINDOWS && _64BIT
		long long a;
		long long src_idx, src_size;
		long long dst_idx;
#else
		long a;
		long src_idx, src_size;
		long dst_idx;
#endif
		AGmNode *k, *tail;
		AGmBinary *src_bin;
		AGmBinary *dst_bin;
		if (Param(arg1, a, tail) == 0 && Param(tail, k, tail) == 0 &&
				Param(arg2, src_bin, tail) == 0 &&
				Param(tail, src_idx, src_size, tail) == 0 &&
				src_idx >= 0 && src_idx + src_size <= src_bin->Length() &&
				Param(arg3, dst_bin, tail) == 0 &&
				Param(tail, dst_idx, tail) == 0 && dst_idx >= 0) {
			if (a == 0) {
				n_rsa_context *context = (n_rsa_context*)k->a.p.ptr;
				if (dst_bin->Length() - dst_idx >= src_size / context->size_mod * context->size_block) {
					_al_nrsa_decrypt(context,
						(long)src_size, &src_bin->Buffer()[src_idx], &dst_bin->Buffer()[dst_idx]);
				}
			} else if (a == 1 && dst_bin->Length() - dst_idx >= src_size) {
				n_des_context *context = (n_des_context*)k->a.p.ptr;
				_al_ndes_decrypt(context,
					(long)src_size, &src_bin->Buffer()[src_idx], &dst_bin->Buffer()[dst_idx]);
			}
		}
	} else if (strcmp(s, "simple_encrypt") == 0) {
		val = Ref(SimpleEncryptDecrypt(arg1, arg2, arg3, 0));
	} else if (strcmp(s, "simple_decrypt") == 0) {
		val = Ref(SimpleEncryptDecrypt(arg1, arg2, arg3, 1));

	} else if (strcmp(s, "digest_init") == 0) {
		if (IsString(arg1)) {
			const char *a = arg1->a.s.v->Str();
			if (strcmp(a, "MD5") == 0) {
					md5_context *context = _al_md5_init();
					if (context) {
						val = Ref(List(Integer((long)0), PtrCell(context)));
					}
			} else if (strcmp(a, "SHA-1") == 0) {
					sha1_context *context = _al_sha1_init();
					if (context) {
						val = Ref(List(Integer((long)1), PtrCell(context)));
					}
			}
		}
	} else if (strcmp(s, "digest_update") == 0) {
#if	_WINDOWS && _64BIT
		long long a;
		long long idx, size;
#else
		long a;
		long idx, size;
#endif
		AGmNode *k, *tail;
		AGmBinary *bin;
		if (Param(arg1, a, tail) == 0 && Param(tail, k, tail) == 0 &&
				Param(arg2, bin, tail) == 0 &&
				Param(tail, idx, size, tail) == 0 &&
				idx >= 0 && idx + size <= bin->Length()) {
			if (a == 0) { // MD5
				md5_context *context = (md5_context*)k->a.p.ptr;
				_al_md5_update(context, (long)size, bin->Buffer()+idx);
			} else if (a == 1) { // SHA-1
				sha1_context *context = (sha1_context*)k->a.p.ptr;
				_al_sha1_update(context, (long)size, bin->Buffer()+idx);
			}
		}
	} else if (strcmp(s, "digest_final") == 0) {
#if	_WINDOWS && _64BIT
		long long a;
#else
		long a;
#endif
		AGmNode *k, *tail;
		if (Param(arg1, a, tail) == 0 && Param(tail, k, tail) == 0) {
			if (a == 0) { // MD5
				md5_context *context = (md5_context*)k->a.p.ptr;
				AGmBinary *digest = new AGmBinary(16);
				_al_md5_final(context, digest->Buffer());
				val = Ref(Binary(digest));
			} else if (a == 1) { // SHA-1
				sha1_context *context = (sha1_context*)k->a.p.ptr;
				AGmBinary *digest = new AGmBinary(20);
				_al_sha1_final(context, digest->Buffer());
				val = Ref(Binary(digest));
			}
		}

#ifdef	_AL_OPENSSL
	} else if (strcmp(s, "smime_encrypt") == 0
			|| strcmp(s, "smime_decrypt") == 0
			|| strcmp(s, "smime_sign") == 0
			|| strcmp(s, "smime_verify") == 0) {
		const char *in_file, *out_file;
		AGmBinary *in_bin, *out_bin;
		AGmNode *tail;
#if	_WINDOWS && _64BIT
		long long index, size;
#else
		long index, size;
#endif
		BIO *in = NULL, *out = NULL, *indata = NULL;
		int ret_is_bin = 0;
		char *cipher_str = NULL, *cert_file = NULL, *key_file = NULL, *caCert_file = NULL;
		const EVP_CIPHER *cipher = NULL;
		X509 *cert = NULL;
		EVP_PKEY *key = NULL;
		STACK_OF(X509) *certs = NULL;
		X509_STORE *store = NULL;
		PKCS7 *p7 = NULL;
		char *pass = NULL;
		STACK_OF(X509) *signers = NULL;
		int flags = 0;
		int ok = 0;
#ifdef	_WINDOWS
		::EnterCriticalSection(&ADvSocket::cs);
#endif
		for (;;) {
			if (IsString(arg1)) {
				in_file = arg1->a.s.v->Str();
				in = BIO_new_file(in_file, "rb");
			} else if (IsBinary(arg1)) {
				in_bin = arg1->a.bin.v;
				char *buf = (char*)OPENSSL_malloc(in_bin->Length() + 1);
				if (buf == NULL) break;
				memcpy(buf, in_bin->Buffer(), in_bin->Length());
				buf[in_bin->Length()] = 0;
				in = BIO_new_mem_buf(buf, in_bin->Length());
			} else if (Param(arg1, in_bin, tail) == 0
					&& Param(tail, index, tail) == 0 && 0 <= index
					&& Param(tail, size, tail) == 0
					&& 0 <= size && index + size <= in_bin->Length()) {
				char *buf = (char*)OPENSSL_malloc(size + 1);
				if (buf == NULL) break;
				memcpy(buf, in_bin->Buffer() + index, size);
				buf[size] = 0;
				in = BIO_new_mem_buf(buf, (long)size);
			} else {
				break;
			}
			if (in == NULL) break;
			if (arg2 == NULL) {
				char *buf = (char*)OPENSSL_malloc(1024);
				if (buf == NULL) break;
				out = BIO_new_mem_buf(buf, 1024);
				if (out == NULL) { OPENSSL_free(buf); break; }
				BUF_MEM *b = (BUF_MEM*)out->ptr;
				b->length = 0;
				ret_is_bin = 1;
			} else {
				if (IsString(arg2)) {
					out_file = arg2->a.s.v->Str();
					out = BIO_new_file(out_file, "wb");
				} else {
					break;
				}
			}
			if (out == NULL) break;
			out->flags &= ~BIO_FLAGS_MEM_RDONLY;
			if (Param(arg3, cipher_str, tail)
					|| Param(tail, cert_file, tail)
					|| Param(tail, key_file, tail)
					|| Param(tail, caCert_file, tail)) break;
			if (cert_file) {
				BIO *cert_in = BIO_new_file(cert_file, "rb");
				if (cert_in == NULL) break;
				cert = PEM_read_bio_X509_AUX(cert_in, NULL, NULL, NULL);
				BIO_free(cert_in);
				if (cert == NULL) break;
			}
			if (key_file) {
				BIO *key_in = BIO_new_file(key_file, "rb");
				if (key_in == NULL) break;
				key = PEM_read_bio_PrivateKey(key_in, NULL, NULL, pass);
				BIO_free(key_in);
				if (key == NULL) break;
			}

			if (strcmp(s, "smime_encrypt") == 0) {
				flags |= PKCS7_BINARY;
				if (cipher_str == NULL) break;
				if (strcmp(cipher_str, "des3") == 0) {
					cipher = EVP_des_ede3_cbc();
				} else if (strcmp(cipher_str, "des") == 0) {
					cipher = EVP_des_cbc();
				} else if (strcmp(cipher_str, "rc2-40") == 0) {
					cipher = EVP_rc2_40_cbc();
				} else if (strcmp(cipher_str, "rc2-64") == 0) {
					cipher = EVP_rc2_64_cbc();
				} else if (strcmp(cipher_str, "rc2-128") == 0) {
					cipher = EVP_rc2_cbc();
				} else {
					break;
				}
				if (cert == NULL) break;
				certs = sk_X509_new_null();
				if (certs == NULL) break;
				sk_X509_push(certs, cert);
				p7 = PKCS7_encrypt(certs, in, cipher, flags);
				if (p7 == NULL) break;
				SMIME_write_PKCS7(out, p7, in, flags);
				ok = 1;

			} else if (strcmp(s, "smime_decrypt") == 0) {
				flags |= PKCS7_BINARY;
				if (cert == NULL || key == NULL) break;
				p7 = openssl_SMIME_read_PKCS7(in, &indata);
				if (!p7) break;
				if (!PKCS7_decrypt(p7, key, cert, out, flags)) break;
				ok = 1;

			} else if (strcmp(s, "smime_sign") == 0) {
				flags |= PKCS7_BINARY;
				flags |= PKCS7_DETACHED;
				flags |= PKCS7_NOCERTS;
				if (cert == NULL || key == NULL) break;
				STACK_OF(X509) *certs = sk_X509_new_null();
				if (certs == NULL) break;
				sk_X509_push(certs, cert);
				p7 = PKCS7_sign(cert, key, certs, in, flags);
				if (p7 == NULL) break;
				BIO_reset(in);
				SMIME_write_PKCS7(out, p7, in, flags);
				ok = 1;

			} else if (strcmp(s, "smime_verify") == 0) {
				flags |= PKCS7_BINARY;
				certs = sk_X509_new_null();
				if (certs == NULL) break;
				sk_X509_push(certs, cert);
				if (caCert_file == NULL) break;
				store = X509_STORE_new();
				if (store == NULL) break;
				X509_LOOKUP *lookup = X509_STORE_add_lookup(store, X509_LOOKUP_file());
				if (lookup == NULL) break;
				ERR_clear_error();
				if (!X509_LOOKUP_load_file(lookup, caCert_file, X509_FILETYPE_PEM)) break;

				p7 = openssl_SMIME_read_PKCS7(in, &indata);
				if (p7 == NULL) break;

				if (!openssl_PKCS7_verify(p7, certs, store, indata, NULL, flags)) break;

				unsigned char* md_dat; unsigned int md_len;
				getMessageDigest(&md_dat, &md_len);
				BIO_write(out, md_dat, md_len);

				ok = 1;
			}
			break;
		}

		if (ok) {
			if (ret_is_bin) {
				BUF_MEM *b = (BUF_MEM*)out->ptr;
				out_bin = new AGmBinary((long)b->length);
				memcpy(out_bin->Buffer(), b->data, b->length);
				val = Ref(Binary(out_bin));
			} else {
				val = Ref(Integer(1));
			}
		}

		if (certs) {
			sk_X509_pop_free(certs, X509_free);
		} else {
			if (cert) X509_free(cert);
		}
		if (signers) {
			sk_X509_free(signers);
		}
		if (key) EVP_PKEY_free(key);
		if (store) X509_STORE_free(store);
		if (indata) BIO_free(indata);
		if (p7) PKCS7_free(p7);

		if (in) BIO_free(in);
		if (out) BIO_free(out);
#ifdef	_WINDOWS
		::LeaveCriticalSection(&ADvSocket::cs);
#endif

	} else if (strcmp(s, "mime_headers_read") == 0) {
		const char *in_file;
		AGmBinary *in_bin;
		AGmNode *tail;
#if	_WINDOWS && _64BIT
		long long index, size;
#else
		long index, size;
#endif
		BIO *in = NULL;
		STACK_OF(MIME_HEADER) *headers = NULL;
#ifdef	_WINDOWS
		::EnterCriticalSection(&ADvSocket::cs);
#endif
		for (;;) {
			if (IsString(arg1)) {
				in_file = arg1->a.s.v->Str();
				in = BIO_new_file(in_file, "rb");
			} else if (IsBinary(arg1)) {
				in_bin = arg1->a.bin.v;
				char *buf = (char*)OPENSSL_malloc(in_bin->Length() + 1);
				if (buf == NULL) break;
				memcpy(buf, in_bin->Buffer(), in_bin->Length());
				buf[in_bin->Length()] = 0;
				in = BIO_new_mem_buf(buf, in_bin->Length());
			} else if (Param(arg1, in_bin, tail) == 0
					&& Param(tail, index, tail) == 0 && 0 <=index
					&& Param(tail, size, tail) == 0
					&& 0 <= size && index + size <= in_bin->Length()) {
				char *buf = (char*)OPENSSL_malloc(size + 1);
				if (buf == NULL) break;
				memcpy(buf, in_bin->Buffer() + index, size);
				buf[size] = 0;
				in = BIO_new_mem_buf(buf, (long)size);
			} else {
				break;
			}
			if (in == NULL) break;
			headers = openssl_mime_parse_hdr(in);
			if (headers == NULL) break;
			AGmNode *ret_val = Cons(NULL);
			int i, n = sk_MIME_HEADER_num(headers);
			for (i = 0; i < n; i++) {
				MIME_HEADER* hdr = sk_MIME_HEADER_value(headers, i);
				AGmNode *node = String(hdr->value ? hdr->value : "");
				CreateArc(ret_val, node, String(hdr->name));
				if (hdr->params) {
					int j, m = sk_MIME_PARAM_num(hdr->params);
					for (j = 0; j < m; j++) {
						MIME_PARAM *prm = sk_MIME_PARAM_value(hdr->params, j);
						CreateArc(node, (prm->param_value ? String(prm->param_value) : Cons(NULL)), String(prm->param_name));
					}
				}
			}
			val = Ref(List(ret_val, Integer(num_read_char)));
			break;
		}
		if (headers) sk_MIME_HEADER_pop_free(headers, openssl_mime_hdr_free);
		if (in) BIO_free(in);
#ifdef	_WINDOWS
		::LeaveCriticalSection(&ADvSocket::cs);
#endif

	} else if (strcmp(s, "mime_multipart_read") == 0) {
		const char *in_file;
		AGmBinary *in_bin, *out_bin = NULL;
		AGmNode *tail;
#if	_WINDOWS && _64BIT
		long long index, size;
#else
		long index, size;
#endif
		BIO *in = NULL;
		STACK_OF(BIO) *parts = NULL;
#ifdef	_WINDOWS
		::EnterCriticalSection(&ADvSocket::cs);
#endif
		for (;;) {
			if (IsString(arg1)) {
				in_file = arg1->a.s.v->Str();
				in = BIO_new_file(in_file, "rb");
			} else if (IsBinary(arg1)) {
				in_bin = arg1->a.bin.v;
				char *buf = (char*)OPENSSL_malloc(in_bin->Length() + 1);
				if (buf == NULL) break;
				memcpy(buf, in_bin->Buffer(), in_bin->Length());
				buf[in_bin->Length()] = 0;
				in = BIO_new_mem_buf(buf, in_bin->Length());
			} else if (Param(arg1, in_bin, tail) == 0
					&& Param(tail, index, tail) == 0 && 0 <= index
					&& Param(tail, size, tail) == 0
					&& 0 <= size && index + size <= in_bin->Length()) {
				char *buf = (char*)OPENSSL_malloc(size + 1);
				if (buf == NULL) break;
				memcpy(buf, in_bin->Buffer() + index, size);
				buf[size] = 0;
				in = BIO_new_mem_buf(buf, (long)size);
			} else {
				break;
			}
			if (in == NULL) break;
			if (arg2 == NULL) break;
			AGmNode *node = dst_node(arg2, "Content-Type", 1);
			if (node == NULL) break;
			node = dst_node(node, "boundary", 1);
			if (!IsString(node)) break;
			char *boundary = (char*)node->a.s.v->Str();
			int ret = openssl_multi_split(in, boundary, &parts);
			if (ret == 0) break;
			val = Ref(Cons(arg2));
			int i, n = sk_BIO_num(parts);
			for (i = 0; i < n; i++) {
				BIO* body = sk_BIO_value(parts, i);
				char buf[1024];
				int num;
				for (;;) {
					num = BIO_read(body, buf, 1024);
					if (num < 0) break;
					if (out_bin == NULL) {
						out_bin = new AGmBinary(num);
						memcpy(out_bin->Buffer(), buf, num);
					} else {
						int size = out_bin->Length();
						out_bin->Expand(num);
						memcpy(out_bin->Buffer() + size, buf, num);
					}
					if (num < 1024) break;
				}
				if (out_bin) {
					CreateArc(val, Binary(out_bin), Integer((long)i));
					out_bin = NULL;
				}
			}
			break;
		}
		if (parts) sk_BIO_pop_free(parts, BIO_vfree);
		if (in) BIO_free(in);
#ifdef	_WINDOWS
		::LeaveCriticalSection(&ADvSocket::cs);
#endif

	} else if (strcmp(s, "mime_headers_write") == 0) {
		const char *out_file;
		AGmBinary *out_bin;
		BIO *out = NULL;
		int ret_is_bin = 0;
#ifdef	_WINDOWS
		::EnterCriticalSection(&ADvSocket::cs);
#endif
		for (;;) {
			if (arg1 == NULL) break;
			if (arg2 == NULL) {
				char *buf = (char*)OPENSSL_malloc(1024);
				if (buf == NULL) break;
				out = BIO_new_mem_buf(buf, 1024);
				if (out == NULL) { OPENSSL_free(buf); break; }
				BUF_MEM *b = (BUF_MEM*)out->ptr;
				b->length = 0;
				ret_is_bin = 1;
			} else {
				if (IsString(arg2)) {
					out_file = arg2->a.s.v->Str();
					out = BIO_new_file(out_file, "wb");
				} else {
					break;
				}
				ret_is_bin = 21;
			}
			if (out == NULL) break;
			out->flags &= ~BIO_FLAGS_MEM_RDONLY;
			if (ret_is_bin == 1) ret_is_bin = 2;
			if (ret_is_bin == 21) ret_is_bin = 22;
			AGmLink *l, *l2; AGmNode *attr, *node, *attr2, *node2;
			for (l = next_outlink(arg1, NULL); l; l = next_outlink(arg1, l)) {
				attr = l->arc->attr;
				node = l->arc->dst;
				if (!IsString(attr) || !IsString(node)) continue;
				BIO_printf(out, "%s: %s", attr->a.s.v->Str(), node->a.s.v->Str());
				for (l2 = next_outlink(node, NULL); l2; l2 = next_outlink(node, l2)) {
					attr2 = l2->arc->attr;
					node2 = l2->arc->dst;
					if (!IsString(attr2) || !IsString(node2)) continue;
					if (IsString(node2)) {
						BIO_printf(out, "; %s=\"%s\"", attr2->a.s.v->Str(), node2->a.s.v->Str());
					} else {
						BIO_printf(out, "; %s", attr2->a.s.v->Str());
					}
				}
#ifdef	LINE_BREAK_LF
				BIO_printf(out, "\n");
			}
			BIO_printf(out, "\n");
#endif
#ifdef	LINE_BREAK_CRLF
				BIO_printf(out, "\r\n");
			}
			BIO_printf(out, "\r\n");
#endif
			BIO_flush(out);
			break;
		}
		if (ret_is_bin == 2) {
			BUF_MEM *b = (BUF_MEM*)out->ptr;
			out_bin = new AGmBinary((long)b->length);
			memcpy(out_bin->Buffer(), b->data, (long)b->length);
			val = Ref(Binary(out_bin));
		} else if (ret_is_bin == 22) {
			val = Ref(Integer(1));
		}
		if (out) BIO_free(out);
#ifdef	_WINDOWS
		::LeaveCriticalSection(&ADvSocket::cs);
#endif

	} else if (strcmp(s, "remove_header") == 0) {
		if (IsBinary(arg1) && IsString(arg2)) {
			int size1 = arg1->a.bin.v->Length();
			unsigned char *buf1 = arg1->a.bin.v->Buffer();
			int size2 = arg2->a.s.v->Length();
			unsigned char *buf2 = (unsigned char *)arg2->a.s.v->Str();
			unsigned char *buf3 = new unsigned char[size1];
			int i, j, k;
			for (i = 0, k = 0; i < size1;) {
				for (j = 0; j < size2 && i + j < size1; j++) {
					if (buf1[i+j] != buf2[j]) break;
				}
				if (j == size2 && i + j < size1 && buf1[i+j] == ':') {
					for (;;) {
						unsigned char c;
						c =  buf1[i++];
						if (c == '\n' || i == size1) break;
					}
				} else {
					for (;;) {
						unsigned char c;
						c = buf3[k++] = buf1[i++];
						if (c == '\n' || i == size1) break;
					}
				}
			}
			AGmBinary *bin = new AGmBinary(k);
			unsigned char *buf = bin->Buffer();
			memcpy(buf, buf3, k);
			delete [] buf3;
			val = Ref(Binary(bin));
		}
	} else if (strcmp(s, "add_header") == 0) {
		if (IsBinary(arg1) && IsString(arg2)) {
			int size1 = arg1->a.bin.v->Length();
			int size2 = arg2->a.s.v->Length();
			AGmBinary *bin = new AGmBinary(size1 + size2);
			unsigned char *buf = bin->Buffer();
			memcpy(buf, arg1->a.bin.v->Buffer(), size1 - 2);
			memcpy(buf + size1 - 2, arg2->a.s.v->Str(), size2);
			memcpy(buf + size1 + size2 - 2, "\r\n", 2);
			val = Ref(Binary(bin));
		}
	} else if (strcmp(s, "get_header") == 0) {
		if (IsBinary(arg1) && IsString(arg2)) {
			int size1 = arg1->a.bin.v->Length();
			unsigned char *buf1 = arg1->a.bin.v->Buffer();
			int size2 = arg2->a.s.v->Length();
			unsigned char *buf2 = (unsigned char *)arg2->a.s.v->Str();
			int i, j;
			for (i = 0; i < size1;) {
				for (j = 0; j < size2 && i + j < size1; j++) {
					if (buf1[i+j] != buf2[j]) break;
				}
				if (j == size2 && i + j < size1 && buf1[i+j] == ':') {
					AGmString *str = new AGmString(16);
					for (;;) {
						unsigned char c;
						c = buf1[++i + j];
						if (c == '\n' || i == size1) break;
						str->Append(c);
					}
					val = Ref(String(str));
					break;
				} else {
					for (;;) {
						unsigned char c;
						c =  buf1[i++];
						if (c == '\n' || i == size1) break;
					}
				}
			}
		}

	} else if (strcmp(s, "mime_multipart_write") == 0) {
		const char *out_file;
		AGmBinary *out_bin;
		BIO *out = NULL;
		int ret_is_bin = 0;
#ifdef	_WINDOWS
		::EnterCriticalSection(&ADvSocket::cs);
#endif
		for (;;) {
			if (arg1 == NULL || !IsList(arg1)) break;
			if (arg2 == NULL) {
				char *buf = (char*)OPENSSL_malloc(1024);
				if (buf == NULL) break;
				out = BIO_new_mem_buf(buf, 1024);
				if (out == NULL) { OPENSSL_free(buf); break; }
				BUF_MEM *b = (BUF_MEM*)out->ptr;
				b->length = 0;
				ret_is_bin = 1;
			} else {
				if (IsString(arg2)) {
					out_file = arg2->a.s.v->Str();
					out = BIO_new_file(out_file, "wb");
				} else {
					break;
				}
				ret_is_bin = 21;
			}
			if (out == NULL) break;
			out->flags &= ~BIO_FLAGS_MEM_RDONLY;
			AGmNode *node = arg1->a.l.h;
			if (node == NULL) break;
			node = dst_node(node, "Content-Type", 1);
			if (node == NULL) break;
			node = dst_node(node, "boundary", 1);
			if (!IsString(node)) break;
			if (ret_is_bin == 1) ret_is_bin = 2;
			if (ret_is_bin == 21) ret_is_bin = 22;
			char *boundary = (char*)node->a.s.v->Str();
			AGmLink *l;
			for (l = next_outlink(arg1, NULL); l; l = next_outlink(arg1, l)) {
				node = l->arc->dst;
				if (!IsBinary(node)) continue;
				AGmBinary *bin = node->a.bin.v;
#ifdef	LINE_BREAK_LF
				BIO_printf(out, "--%s\n", boundary);
				BIO_write(out, bin->Buffer(), bin->Length());
				BIO_printf(out, "\n");
			}
			BIO_printf(out, "--%s--\n\n", boundary);
#endif
#ifdef	LINE_BREAK_CRLF
				BIO_printf(out, "--%s\r\n", boundary);
				BIO_write(out, bin->Buffer(), bin->Length());
				BIO_printf(out, "\r\n");
			}
			BIO_printf(out, "--%s--\r\n\r\n", boundary);
#endif
			BIO_flush(out);
			break;
		}
		if (ret_is_bin == 2) {
			BUF_MEM *b = (BUF_MEM*)out->ptr;
			out_bin = new AGmBinary((long)b->length);
			memcpy(out_bin->Buffer(), b->data, b->length);
			val = Ref(Binary(out_bin));
		} else if (ret_is_bin == 22) {
			val = Ref(Integer(1));
		}
		if (out) BIO_free(out);
#ifdef	_WINDOWS
		::LeaveCriticalSection(&ADvSocket::cs);
#endif

	} else if (strcmp(s, "base64_encode") == 0
			|| strcmp(s, "base64_decode") == 0) {
		const char *in_file, *out_file;
		AGmBinary *in_bin, *out_bin;
		AGmNode *tail;
#if	_WINDOWS && _64BIT
		long long index, size = 0;
#else
		long index, size = 0;
#endif
		BIO *in = NULL, *out = NULL, *in_b64 = NULL, *out_b64 = NULL;
		int ret_is_bin = 0;
		int num = -1, num2 = -1;
#ifdef	_WINDOWS
		::EnterCriticalSection(&ADvSocket::cs);
#endif
		for (;;) {
			if (IsString(arg1)) {
				in_file = arg1->a.s.v->Str();
				in = BIO_new_file(in_file, "rb");
			} else if (IsBinary(arg1)) {
				in_bin = arg1->a.bin.v;
				char *buf = (char*)OPENSSL_malloc(in_bin->Length() + 1);
				if (buf == NULL) break;
				memcpy(buf, in_bin->Buffer(), in_bin->Length());
				buf[in_bin->Length()] = 0;
				in = BIO_new_mem_buf(buf, in_bin->Length());
			} else if (Param(arg1, in_bin, tail) == 0
					&& Param(tail, index, tail) == 0 && 0 <=index
					&& Param(tail, size, tail) == 0
					&& 0 <= size && index + size <= in_bin->Length()) {
				char *buf = (char*)OPENSSL_malloc(size + 1);
				if (buf == NULL) break;
				memcpy(buf, in_bin->Buffer() + index, size);
				buf[size] = 0;
				in = BIO_new_mem_buf(buf, (long)size);
			} else {
				break;
			}
			if (in == NULL) break;
			if (arg2 == NULL) {
				char *buf = (char*)OPENSSL_malloc(1024);
				if (buf == NULL) break;
				out = BIO_new_mem_buf(buf, 1024);
				if (out == NULL) { OPENSSL_free(buf); break; }
				BUF_MEM *b = (BUF_MEM*)out->ptr;
				b->length = 0;
				ret_is_bin = 1;
			} else {
				if (IsString(arg2)) {
					out_file = arg2->a.s.v->Str();
					out = BIO_new_file(out_file, "wb");
				} else {
					break;
				}
			}
			if (out == NULL) break;
			out->flags &= ~BIO_FLAGS_MEM_RDONLY;
			if (strcmp(s, "base64_encode") == 0) {
				out_b64 = BIO_new(BIO_f_base64());
				if (out_b64 == NULL) break;
				out = BIO_push(out_b64, out);
			} else if (strcmp(s, "base64_decode") == 0) {
				in_b64 = BIO_new(BIO_f_base64());
				if (in_b64 == NULL) break;
				in = BIO_push(in_b64, in);
			}
			char buf[1024];
			num = num2 = 0;
			size = 0;
			for (;;) {
				num = BIO_read(in, buf, 1024);
				if (num < 0) break;
				size += num;
				int count = num;
				while (count > 0) {
					num2 = BIO_write(out, buf, count);
					if (num2 < 0) break;
					count -= num2;
				}
				if (num2 < 0) break;
				if (num < 1024) break;
			}
			if (num < 0 || num2 < 0) break;
			BIO_flush(in); BIO_flush(out);
			if (in_b64) in = BIO_pop(in);
			if (out_b64) out = BIO_pop(out);
			break;
		}

		if (num >= 0 && num2 >= 0) {
			if (ret_is_bin) {
				out_bin = new AGmBinary((long)size);
				BUF_MEM *b = (BUF_MEM*)out->ptr;
				memcpy(out_bin->Buffer(), b->data, size);
				val = Ref(Binary(out_bin));
			} else {
				val = Ref(Integer(1));
			}
		}

		if (in_b64) BIO_free(in_b64);
		if (out_b64) BIO_free(out_b64);
		if (in) BIO_free(in);
		if (out) BIO_free(out);
#ifdef	_WINDOWS
		::LeaveCriticalSection(&ADvSocket::cs);
#endif

	} else if (strcmp(s, "random") == 0) {
		if (IsInteger(arg1) && arg1->a.i.v > 0) {
			long n = (long)arg1->a.i.v;
			unsigned char *buf = new unsigned char[n + 1];
			RAND_pseudo_bytes(buf, n);
			int i;
			for (i = 0; i < n; i++) {
				char c = (char)(buf[i] % 36);
				if (c < 10) buf[i] = (unsigned char)('0' + c);
				else buf[i] = (unsigned char)('A' + (c - 10));
			}
			buf[n] = 0;
			val = Ref(String((char*)buf));
			delete [] buf;
		}
#endif
	} else {
		Unref(cmd, arg1, arg2, arg3);
		return interp_error("Crypt()");
	}
	Unref(cmd, arg1, arg2, arg3);
	return 0;
}

// -------- Prime Number Generator
AGmPNumMgr* AGmPNumMgr::inst = NULL;
struct p_num_block* AGmPNumMgr::start_block = NULL;
struct p_num_block* AGmPNumMgr::end_block = NULL;
int AGmPNumMgr::p_num_index = 0;
unsigned long AGmPNumMgr::k = 1;

AGmPNumMgr* AGmPNumMgr::GetInstance() {
	if (inst) return inst;
	inst = new AGmPNumMgr();
	return inst;
}
void AGmPNumMgr::Finalize() {
	if (inst) delete inst;
	inst = NULL;
}
unsigned long AGmPNumMgr::get_p_num_greater_than(unsigned long num) {
	struct p_num_block* block = start_block;
	while (block) {
		if (block != end_block) {
			for (int i = 0; i < P_NUM_BLOCK_SIZE; i++) {
				if (block->p_num[i] > num) return block->p_num[i];
			}
			block = block->next;
		} else {
			for (int i = 0; i < p_num_index; i++) {
				if (block->p_num[i] > num)	return block->p_num[i];
			}
			block = block->next;
		}
	}

	for (;; k++) {
		unsigned long p_num;

		p_num = 6 * k + 1;
		if (__is_p_num(p_num)) {
			add_p_num_to_list(p_num);
			if (p_num > num) return p_num;
		}

		p_num = 6 * k + 5;
		if (__is_p_num(p_num)) {
			add_p_num_to_list(p_num);
			if (p_num > num) return p_num;
		}
	}
}
int AGmPNumMgr::is_p_num(unsigned long p_num) {
	unsigned long limit = (unsigned long)ceil(sqrt((float)p_num));
	get_p_num_greater_than(limit);

	struct p_num_block* block;

	block = start_block;
	while (block) {
		if (block != end_block) {
			for (int i = 0; i < P_NUM_BLOCK_SIZE; i++) {
				if (p_num == block->p_num[i]) return 1;
			}
			block = block->next;
		} else {
			for (int i = 0; i < p_num_index; i++) {
				if (p_num == block->p_num[i]) return 1;
			}
			block = block->next;
		}
	}

	block = start_block;
	while (block) {
		if (block != end_block) {
			for (int i = 0; i < P_NUM_BLOCK_SIZE; i++) {
				if ((p_num % block->p_num[i]) == 0) return 0;
			}
			block = block->next;
		} else {
			for (int i = 0; i < p_num_index; i++) {
				if ((p_num % block->p_num[i]) == 0) return 0;
			}
			block = block->next;
		}
	}
	return 1;
}

AGmPNumMgr::AGmPNumMgr() {
	add_p_num_to_list(2);
	add_p_num_to_list(3);
	add_p_num_to_list(5);
	k = 1;
}
AGmPNumMgr::~AGmPNumMgr() {
	clear_p_num_list();
}
void AGmPNumMgr::add_p_num_to_list(unsigned long p_num) {
	if (start_block == NULL) {
		start_block = end_block = new struct p_num_block;
		end_block->next = NULL;
		p_num_index = 0;
	}
	if (p_num_index >= P_NUM_BLOCK_SIZE) {
		end_block->next = new struct p_num_block;
		end_block = end_block->next;
		end_block->next = NULL;
		p_num_index = 0;
	}
	end_block->p_num[p_num_index++] = p_num;
}
void AGmPNumMgr::clear_p_num_list() {
	struct p_num_block* block = start_block, *next;
	while (block) {
		next = block->next;
		delete block;
		block = next;
	}
	start_block = end_block = NULL;
}
int AGmPNumMgr::__is_p_num(unsigned long p_num) {
	unsigned long limit = (unsigned long)ceil(sqrt((float)p_num));
	struct p_num_block* block = start_block;
	while (block) {
		if (block != end_block) {
			for (int i = 0; i < P_NUM_BLOCK_SIZE; i++) {
				unsigned long check = block->p_num[i];
				if (check > limit) return 1;
				if ((p_num % check) == 0) return 0;
			}
			block = block->next;
		} else {
			for (int i = 0; i < p_num_index; i++) {
				unsigned long check = block->p_num[i];
				if (check > limit) return 1;
				if ((p_num % check) == 0) return 0;
			}
			block = block->next;
		}
	}
	return 1;
}

// -------- N-RSA
// p: prime number
// q: prime number
// m = p * q
// k = lcm(p-1, q-1)
// sk: sk < k && gcd(sk, k) == 1 (secret key)
// pk: (sk * pk) mod k == 1 	 (public key)

int _al_nrsa_generate_keypair(unsigned long p_hint, unsigned long q_hint, unsigned long s_hint,
		AGmNode*& sk, AGmNode*& pk, AGmNode*& mod) {

	AGmPNumMgr *mgr = AGmPNumMgr::GetInstance();

	// -------- find first prime number p
	unsigned long p = mgr->get_p_num_greater_than(p_hint);

	// -------- find second prime number q
	unsigned long q = mgr->get_p_num_greater_than(q_hint);

	// -------- mod = p * q
	AGmNode *p1 = AGm::Ref(AGm::Integer(p));
	AGmNode *q1 = AGm::Ref(AGm::Integer(q));
	mod = BigInteger::Mul(p1, q1);
	AGm::Unref(p1, q1);

	// -------- k = lcm(p-1, q-1)
	p1 = AGm::Ref(AGm::Integer(p - 1));
	q1 = AGm::Ref(AGm::Integer(q - 1));
	AGmNode *gcd = AGm::Ref(BigInteger::Gcd(p1, q1));
	AGmNode *tmp = AGm::Ref(BigInteger::Mul(p1, q1));
	AGmNode *k = AGm::Ref(BigInteger::Div(tmp, gcd));
	AGm::Unref(p1, q1, gcd, tmp);

	// -------- find sk
	sk = AGm::Integer(s_hint);
	for (;;) {
		tmp = BigInteger::Add(sk, AGm::one);
		AGm::RefUnref(sk); sk = tmp;
		// sk < k
		if (BigInteger::Compare(sk, k) >= 0) {
			AGm::RefUnref(mod, sk); AGm::Unref(k); return 0;
		}
		AGmNode *m = AGm::Ref(BigInteger::Gcd(sk, k));
		// gcd(sk, k) = 1
		if (AGm::IsInteger(m) && m->a.i.v == 1) {
			AGm::Unref(m); break;
		}
		AGm::Unref(m);
	}

	// -------- find pk
	// first candidate of pk is sk
	// candidate -> tmp
	tmp = AGm::GraphCopy(sk);
	for (;;) {
		// tmp -> candidate
		pk = tmp;
		// (tmp * sk) % k -> tmp
		AGmNode *tmp2 = AGm::Ref(BigInteger::Mul(tmp, sk));
		tmp = BigInteger::Mod(tmp2, k);
		AGm::Unref(tmp2);
		// check (tmp == 1)
		if (!AGm::IsInteger(tmp) || tmp->a.i.v != 1) {
			AGm::RefUnref(pk); continue;
		}
		// candidate -> pk
		AGm::RefUnref(tmp);
		break;
	}

	AGm::Unref(k);

	return 1;
}

// -------- N-RSA (encrypt/decrypt)
n_rsa_context *_al_nrsa_init(long size_block, AGmNode *key, AGmNode *mod) {

	key = AGm::Ref(BigInteger::ToBigInt(key));
	mod = AGm::Ref(BigInteger::ToBigInt(mod));

	// size_mod > size_block
	if (size_block >= mod->a.bi.bin->Length()) {
		AGm::Unref(key, mod);
		return NULL;
	}

	n_rsa_context *context = new n_rsa_context;

	// block size
	context->size_block = size_block;

	// key
	context->size_key = key->a.bi.bin->Length();
	context->buf_key = new unsigned char[context->size_key];
	memcpy(context->buf_key, key->a.bi.bin->Buffer(), context->size_key);
	context->size_mod = mod->a.bi.bin->Length();
	context->buf_mod = new unsigned char[context->size_mod];
	memcpy(context->buf_mod, mod->a.bi.bin->Buffer(), context->size_mod);

	// tmp data
	long size_mod2 = context->size_mod * 2;
	context->buf_tmp = new unsigned char[size_mod2];
	context->buf_aa = new unsigned char[size_mod2];

	AGm::Unref(key, mod);

	return context;
}

void _al_nrsa_encrypt(
		n_rsa_context *context,
		long size_data, unsigned char *buf_data, unsigned char *buf_data2) {
		// size of buf_data = size_block * n
		// size of buf_data2 = n * size_mod
	while (size_data > 0) {
		_al_nrsa_kernel(context, context->size_block, buf_data, context->buf_aa);
		_al_bi_copy(context->size_mod, context->buf_aa, buf_data2);
		buf_data += context->size_block;
		buf_data2 += context->size_mod;
		size_data -= context->size_block;
	}
}

void _al_nrsa_decrypt(
		n_rsa_context *context,
		long size_data, unsigned char *buf_data, unsigned char *buf_data2) {
		// size of buf_data = size_mod * n
		// size of buf_data2 = n * size_block
	while (size_data > 0) {
		_al_nrsa_kernel(context, context->size_mod, buf_data, context->buf_aa);
		_al_bi_copy(context->size_block,
			&context->buf_aa[context->size_mod - context->size_block], buf_data2);
		buf_data += context->size_mod;
		buf_data2 += context->size_block;
		size_data -= context->size_mod;
	}
}

void _al_nrsa_kernel(
		n_rsa_context *context,
		long size_a, unsigned char *buf_a, unsigned char *buf_b) {
	// tmp = a;
	long size_tmp = size_a;
	_al_bi_copy(size_a, buf_a, context->buf_tmp);
	// b = 1;
	long size_b = 1;
	buf_b[0] = 1;
	// for (bit = msb_of_key; bit >= 0; bit--)	{
	int i, j; unsigned long c; long size_c;
	for (i = context->size_key - 1; i >= 0; i--) {
		c = context->buf_key[i];
		for (j = 0; j < 8; j++) {
			//	if (key[bit]) {
			if ((c & 0x01) != 0) {
				//		b *= tmp;
				size_c = size_b + size_tmp;
				_al_bi_mul(size_b, buf_b, size_tmp, context->buf_tmp, &size_b, buf_b);
				_al_bi_copy(size_b, &buf_b[size_c - size_b], buf_b);
				//		b %= mod;
				if (size_b >= context->size_mod) {
					_al_bi_div(size_b, buf_b, context->size_mod, context->buf_mod,
						NULL, NULL, &size_b, buf_b);
					_al_bi_copy(size_b, &buf_b[context->size_mod - size_b], buf_b);
					ASSERT(size_b > 0);
				}
			//	}
			}
			c >>= 1;
			//	tmp *= tmp;
			size_c = size_tmp * 2;
			_al_bi_mul(size_tmp, context->buf_tmp, size_tmp, context->buf_tmp, &size_tmp, context->buf_tmp);	
			_al_bi_copy(size_tmp, &context->buf_tmp[size_c - size_tmp], context->buf_tmp);
			//	tmp %= mod;
			if (size_tmp >= context->size_mod) {
				_al_bi_div(size_tmp, context->buf_tmp, context->size_mod, context->buf_mod,
					NULL, NULL, &size_tmp, context->buf_tmp);
				_al_bi_copy(size_tmp, &context->buf_tmp[context->size_mod - size_tmp], context->buf_tmp);
				ASSERT(size_tmp > 0);
			}
	// }
		}
	}
	_al_bi_copy(size_b, buf_b, context->buf_tmp);
	_al_bi_copy(size_b, context->buf_tmp, &buf_b[context->size_mod - size_b]);
	for  (i = 0; i < context->size_mod - size_b; i++) buf_b[i] = 0;
}
void _al_nrsa_final(n_rsa_context *context) {
	delete [] context->buf_key;
	delete [] context->buf_mod;
	delete [] context->buf_tmp;
	delete [] context->buf_aa;
	
	delete context;
}

// -------- N-DES
n_des_context *_al_ndes_init(
		long size_block,
		long size_key, unsigned char *buf_key) {
	n_des_context *context = new n_des_context;

	// data
	context->size_block = size_block;

	// num repeat
	context->n_repeat = 16;

	// tmp data
	long size_block8 = size_block * 8;
	context->size_block8 = size_block8;
	long exp_block = _al_ndes_get_exp(size_block8);

	context->buf_a = new unsigned char[size_block8];
	context->buf_b = new unsigned char[size_block8];

	// key
	long size_key2 = size_key * 2;
	long size_key2_8 = size_key2 * 8;
	long exp_key = _al_ndes_get_exp(size_key2_8);
	long size_xor8 = (size_key2_8 > size_block8 ? size_block8 : size_key2_8);
	context->size_xor8 = size_xor8;

	unsigned char *tmp_buf_key_a = new unsigned char[size_key2];
	unsigned char *tmp_buf_key_b = new unsigned char[size_key2];

	long size8 = (size_block8 > size_key2_8 ? size_block8 : size_key2_8);
	unsigned char *tmp_buf_key_a8 = new unsigned char[size8];
	unsigned char *tmp_buf_key_b8 = new unsigned char[size8];
	unsigned char *tmp_buf_key_c8 = new unsigned char[size8];

	context->key_transpose = new unsigned char*[context->n_repeat];
	context->key_xor = new unsigned char*[context->n_repeat];
	_al_ndes_first_key(size_key, buf_key, tmp_buf_key_a);

	int i, j, tmp_size_key2_8, k, tmp_size_block8;
	unsigned long c;
	unsigned char *buf;
	for (i = 0; i < context->n_repeat; i++) {
		// ======== transpose key
		_al_bi_copy(size_key2, tmp_buf_key_a, tmp_buf_key_b);
		for (j = 0; j < size_key2_8; j++) {
			tmp_buf_key_a8[j] = (unsigned char)(size_key2_8 -1 - j);
		}
		// make transpose
		tmp_size_key2_8 = size_key2_8;
		for (j = 0; j < size_key2_8; j++) {
			c = _al_ndes_rotateL(exp_key, size_key2, tmp_buf_key_b);
			c %= tmp_size_key2_8;
			tmp_buf_key_b8[j] = tmp_buf_key_a8[c];
			for (k = c; k < tmp_size_key2_8 - 1; k++) {
				tmp_buf_key_a8[k] = tmp_buf_key_a8[k+1];
			}
			tmp_size_key2_8--;
		}
		// byte -> byte[8]
		buf = tmp_buf_key_a8;
		for (j = 0; j < size_key2; j++) {
			c = tmp_buf_key_a[j];
			for (k = 0; k < 8; k++) {
				if ((c & 0x80) == 0) {
					*buf++ = 0x00;
				} else {
					*buf++ = 0xff;
				}
				c <<= 1;
			}
		}
		// transpose
		for (j = 0; j < size_key2_8; j++) {
			tmp_buf_key_c8[j] = tmp_buf_key_a8[tmp_buf_key_b8[j]];
		}
		// byte[8] -> byte
		buf = tmp_buf_key_c8;
		for (j = 0; j < size_key2; j++) {
			c = 0;
			for (k = 0; k < 8; k++) {
				c <<= 1;
				if (*buf++) {
					c |= 0x01;
				}
			}
			tmp_buf_key_b[j] = (unsigned char)c;
		}
		buf = tmp_buf_key_a;
		tmp_buf_key_a = tmp_buf_key_b;
		tmp_buf_key_b = buf;

		// ======== xor key
		for (j = 0; j < size_key; j++) {
			tmp_buf_key_a[j] ^= tmp_buf_key_b[j];
		}

		// ======== transpose of data
		context->key_transpose[i] = new unsigned char[size_block8];
		_al_bi_copy(size_key2, tmp_buf_key_a, tmp_buf_key_b);
		for (j = 0; j < size_block8; j++) {
			tmp_buf_key_c8[j] = (unsigned char)(size_block8 -1 - j);
		}
		tmp_size_block8 = size_block8;
		for (j = 0; j < size_block8; j++) {
			c = _al_ndes_rotateL(exp_block, size_key2, tmp_buf_key_b);
			c %= tmp_size_block8;
			context->key_transpose[i][j] = tmp_buf_key_c8[c];
			for (k = c; k < tmp_size_block8 - 1; k++) {
				tmp_buf_key_c8[k] = tmp_buf_key_c8[k+1];
			}
			tmp_size_block8--;
		}
#ifdef	_DEBUG_NDES
	{
		TRACE1("transpose %d = \n", (long)i);
		k = 0;
		for (j = 0; j < size_block8; j++) {
			TRACE1(" %d", context->key_transpose[i][j]);
			if (++k % 16 == 0) TRACE0("\n");
		}
		TRACE0("\n");
	}
#endif

		// xor of data
		context->key_xor[i] = new unsigned char[size_xor8];
		for (j = 0; j < size_xor8; j++) {
			context->key_xor[i][j] = tmp_buf_key_a8[j];
		}
#ifdef	_DEBUG_NDES
	{
		TRACE1("xor %d = \n", (long)i);
		k = 0;
		for (j = 0; j < size_block8; j++) {
			TRACE1(" %d", context->key_xor[i][j]);
			if (++k % 16 == 0) TRACE0("\n");
		}
		TRACE0("\n");
	}
#endif
	}

	delete [] tmp_buf_key_a;
	delete [] tmp_buf_key_b;
	delete [] tmp_buf_key_a8;
	delete [] tmp_buf_key_b8;
	delete [] tmp_buf_key_c8;

	return context;
}

void _al_ndes_first_key(long size_key, unsigned char *buf_key, unsigned char *buf_key_a) {
	int i, j;
	unsigned char c; long a;
	for (i = 0; i < size_key; i++) {
		c = buf_key[i]; a = 0;
		for (j = 0; j < 8; j++) {
			a <<= 2;
			if ((c & 0x80) == 0) {
				a |= 0x01;
			} else {
				a |= 0x02;
			}
			c <<= 1;
		}
		*buf_key_a++ = (unsigned char)(a >> 8);
		*buf_key_a++ = (unsigned char)a;
	}
}
long _al_ndes_get_exp(long size) {
	long exp = 0;
	long c = 1;
	while (c < size) {
		exp++;
		c <<= 1;
	}
	return exp;
	// c  exp
	// 1  0
	// 2  1
	// 4  2
	// 8  3
}
long _al_ndes_rotateL(long exp, long size, unsigned char *buf) {
	int i;
	long c = 0, a;
	for (i = size - 1; i >= 0; i--) {
		a = buf[i];
		a <<= exp;
		a |= c;
		buf[i] = (unsigned char)a;
		c = (a >> 8);
	}
	buf[size - 1] |= c;
	return c;
}

void _al_ndes_encrypt(n_des_context *context,
		long size_data, unsigned char *buf_data, unsigned char *buf_data2) {
		// size_data == size_block * n
	unsigned char *buf;
	int i, j;
	unsigned char c;

	while (size_data > 0) {
		buf = context->buf_a;
		for (i = 0; i < context->size_block; i++) {
			c = buf_data[i];
			for (j = 0; j < 8; j++) {
				if ((c & 0x80) == 0) {
					*buf++ = 0x00;
				} else {
					*buf++ = 0xff;
				}
				c <<= 1;
			}
		}
		for (i = 0; i < context->n_repeat; i++) {
			// transpose
			for (j = 0; j < context->size_block8; j++) {
				context->buf_b[j] = context->buf_a[context->key_transpose[i][j]];
			}
			// exclusive or
			for (j = 0; j < context->size_xor8; j++) {
				context->buf_b[j] ^= context->key_xor[i][j];
			}
			buf = context->buf_a;
			context->buf_a = context->buf_b;
			context->buf_b = buf;
		}
		buf = context->buf_a;
		for (i = 0; i < context->size_block; i++) {
			c = 0;
			for (j = 0; j < 8; j++) {
				c <<= 1;
				if (*buf++) {
					c |= 0x01;
				}
			}
			buf_data2[i] = c;
		}
		buf_data += context->size_block;
		buf_data2 += context->size_block;
		size_data -= context->size_block;
	}
}

void _al_ndes_decrypt(n_des_context *context,
		long size_data, unsigned char *buf_data, unsigned char *buf_data2) {
		// size_data == size_block * n
	unsigned char *buf;
	int i, j;
	unsigned char c;

	while (size_data > 0) {
		buf = context->buf_a;
		for (i = 0; i < context->size_block; i++) {
			c = buf_data[i];
			for (j = 0; j < 8; j++) {
				if ((c & 0x80) == 0) {
					*buf++ = 0x00;
				} else {
					*buf++ = 0xff;
				}
				c <<= 1;
			}
		}
		for (i = context->n_repeat - 1; i >= 0; i--) {
			// exclusive or
			for (j = 0; j < context->size_xor8; j++) {
				context->buf_a[j] ^= context->key_xor[i][j];
			}
			// transpose
			for (j = 0; j < context->size_block8; j++) {
				context->buf_b[context->key_transpose[i][j]] = context->buf_a[j];
			}
			buf = context->buf_a;
			context->buf_a = context->buf_b;
			context->buf_b = buf;
		}
		buf = context->buf_a;
		for (i = 0; i < context->size_block; i++) {
			c = 0;
			for (j = 0; j < 8; j++) {
				c <<= 1;
				if (*buf++) {
					c |= 0x01;
				}
			}
			buf_data2[i] = c;
		}
		buf_data += context->size_block;
		buf_data2 += context->size_block;
		size_data -= context->size_block;
	}
}

void _al_ndes_final(n_des_context *context) {
	delete [] context->buf_a;
	delete [] context->buf_b;

	int i;
	for (i = 0; i < context->n_repeat; i++) {
		delete [] context->key_transpose[i];
		delete [] context->key_xor[i];
	}
	delete [] context->key_transpose;
	delete [] context->key_xor;

	delete context;
}

// ======== MessageDigest
static unsigned char padding[64] = {
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// -------- MD5
md5_context *_al_md5_init() {
	md5_context *context = new md5_context;
	context->i[0] = context->i[1] = 0;
	context->buf[0] = 0x67452301;
	context->buf[1] = 0xefcdab89;
	context->buf[2] = 0x98badcfe;
	context->buf[3] = 0x10325476;
	return context;
}
void _al_md5_update(md5_context *context, long size, unsigned char*buf) {
	unsigned long in[16];

	int mdi = (context->i[0] >> 3) & 0x3f;

	if (context->i[0] + (size << 3) < context->i[0])
		context->i[1]++;
	context->i[0] += (size << 3);
	context->i[1] += (size >> 29);

	while (size--) {
		context->in[mdi++] = *buf++;

		if (mdi == 0x40) {
			for (int i = 0; i < 16; i++) {
				in[i] = (context->in[i*4+3] << 24) |
						(context->in[i*4+2] << 16) |
						(context->in[i*4+1] <<	8) |
						(context->in[i*4]		 );
			}
			_al_md5_transform(context->buf, in);
			mdi = 0;
		}
	}
}
void _al_md5_final(md5_context *context, unsigned char *buf_digest) {
	unsigned long in[16];

	in[14] = context->i[0];
	in[15] = context->i[1];

	int mdi = (context->i[0] >> 3) & 0x3f;

	int pad_len = (mdi < 56) ? (56 - mdi) : (120 - mdi);
	_al_md5_update(context, pad_len, padding);

	int i;
	for (i = 0; i < 14; i++) {
		in[i] = (context->in[i*4+3] << 24) |
				(context->in[i*4+2] << 16) |
				(context->in[i*4+1] <<	8) |
				(context->in[i*4]		 );
	}
	_al_md5_transform(context->buf, in);

	for (i = 0; i < 4; i++) {
		buf_digest[i*4]   = (unsigned char)(context->buf[i]);
		buf_digest[i*4+1] = (unsigned char)(context->buf[i] >> 8);
		buf_digest[i*4+2] = (unsigned char)(context->buf[i] >> 16);
		buf_digest[i*4+3] = (unsigned char)(context->buf[i] >> 24);
	}

	delete context;
}

void ff(unsigned long &a,
		unsigned long b, unsigned long c, unsigned long d,
		unsigned long x, unsigned long s, unsigned long ac) {
	a += ((b & c) | (~b & d)) + x + ac;
	a = (a << s) | (a >> (32-s));
	a += b;
}
void gg(unsigned long &a,
		unsigned long b, unsigned long c, unsigned long d,
		unsigned long x, unsigned long s, unsigned long ac) {
	a += ((b & d) | (c & ~d)) + x + ac;
	a = (a << s) | (a >> (32-s));
	a += b;
}
void hh(unsigned long &a,
		unsigned long b, unsigned long c, unsigned long d,
		unsigned long x, unsigned long s, unsigned long ac) {
	a += (b ^ c ^ d) + x + ac;
	a = (a << s) | (a >> (32-s));
	a += b;
}
void ii(unsigned long &a,
		unsigned long b, unsigned long c, unsigned long d,
		unsigned long x, unsigned long s, unsigned long ac) {
	a += (c ^ (b | ~d)) + x + ac;
	a = (a << s) | (a >> (32-s));
	a += b;
}

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

void _al_md5_transform(unsigned long *buf, unsigned long *in) {
	unsigned long a = buf[0], b = buf[1], c = buf[2], d = buf[3];

	ff(a, b, c, d, in[ 0], S11, 3614090360);
	ff(d, a, b, c, in[ 1], S12, 3905402710);
	ff(c, d, a, b, in[ 2], S13,  606105819);
	ff(b, c, d, a, in[ 3], S14, 3250441966);
	ff(a, b, c, d, in[ 4], S11, 4118548399);
	ff(d, a, b, c, in[ 5], S12, 1200080426);
	ff(c, d, a, b, in[ 6], S13, 2821735955);
	ff(b, c, d, a, in[ 7], S14, 4249261313);
	ff(a, b, c, d, in[ 8], S11, 1770035416);
	ff(d, a, b, c, in[ 9], S12, 2336552879);
	ff(c, d, a, b, in[10], S13, 4294925233);
	ff(b, c, d, a, in[11], S14, 2304563134);
	ff(a, b, c, d, in[12], S11, 1804603682);
	ff(d, a, b, c, in[13], S12, 4254626195);
	ff(c, d, a, b, in[14], S13, 2792965006);
	ff(b, c, d, a, in[15], S14, 1236535329);
	gg(a, b, c, d, in[ 1], S21, 4129170786);
	gg(d, a, b, c, in[ 6], S22, 3225465664);
	gg(c, d, a, b, in[11], S23,  643717713);
	gg(b, c, d, a, in[ 0], S24, 3921069994);
	gg(a, b, c, d, in[ 5], S21, 3593408605);
	gg(d, a, b, c, in[10], S22,   38016083);
	gg(c, d, a, b, in[15], S23, 3634488961);
	gg(b, c, d, a, in[ 4], S24, 3889429448);
	gg(a, b, c, d, in[ 9], S21,  568446438);
	gg(d, a, b, c, in[14], S22, 3275163606);
	gg(c, d, a, b, in[ 3], S23, 4107603335);
	gg(b, c, d, a, in[ 8], S24, 1163531501);
	gg(a, b, c, d, in[13], S21, 2850285829);
	gg(d, a, b, c, in[ 2], S22, 4243563512);
	gg(c, d, a, b, in[ 7], S23, 1735328473);
	gg(b, c, d, a, in[12], S24, 2368359562);
	hh(a, b, c, d, in[ 5], S31, 4294588738);
	hh(d, a, b, c, in[ 8], S32, 2272392833);
	hh(c, d, a, b, in[11], S33, 1839030562);
	hh(b, c, d, a, in[14], S34, 4259657740);
	hh(a, b, c, d, in[ 1], S31, 2763975236);
	hh(d, a, b, c, in[ 4], S32, 1272893353);
	hh(c, d, a, b, in[ 7], S33, 4139469664);
	hh(b, c, d, a, in[10], S34, 3200236656);
	hh(a, b, c, d, in[13], S31,  681279174);
	hh(d, a, b, c, in[ 0], S32, 3936430074);
	hh(c, d, a, b, in[ 3], S33, 3572445317);
	hh(b, c, d, a, in[ 6], S34,   76029189);
	hh(a, b, c, d, in[ 9], S31, 3654602809);
	hh(d, a, b, c, in[12], S32, 3873151461);
	hh(c, d, a, b, in[15], S33,  530742520);
	hh(b, c, d, a, in[ 2], S34, 3299628645);
	ii(a, b, c, d, in[ 0], S41, 4096336452);
	ii(d, a, b, c, in[ 7], S42, 1126891415);
	ii(c, d, a, b, in[14], S43, 2878612391);
	ii(b, c, d, a, in[ 5], S44, 4237533241);
	ii(a, b, c, d, in[12], S41, 1700485571);
	ii(d, a, b, c, in[ 3], S42, 2399980690);
	ii(c, d, a, b, in[10], S43, 4293915773);
	ii(b, c, d, a, in[ 1], S44, 2240044497);
	ii(a, b, c, d, in[ 8], S41, 1873313359);
	ii(d, a, b, c, in[15], S42, 4264355552);
	ii(c, d, a, b, in[ 6], S43, 2734768916);
	ii(b, c, d, a, in[13], S44, 1309151649);
	ii(a, b, c, d, in[ 4], S41, 4149444226);
	ii(d, a, b, c, in[11], S42, 3174756917);
	ii(c, d, a, b, in[ 2], S43,  718787259);
	ii(b, c, d, a, in[ 9], S44, 3951481745);

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}

// -------- SHA-1
sha1_context *_al_sha1_init() {
	sha1_context *context = new sha1_context;
	context->total = 0;
	context->filled = 0;
	context->h[0] = 0x67452301;
	context->h[1] = 0xefcdab89;
	context->h[2] = 0x98badcfe;
	context->h[3] = 0x10325476;
	context->h[4] = 0xc3d2e1f0;
	memset(context->buf, 0, 64);
	return context;
}
void _al_sha1_update(sha1_context *context, long size, unsigned char *buf) {
	context->total += size;
	while (size) {
		unsigned long c;
		for(c = context->filled; size && c < 64; c++, buf++,size--) {
			context->buf[c] = *buf;
		}
		context->filled = c;
		if (c == 64) {
			_al_sha1_compute_block(context);
			context->filled = 0;
		}
	}
}
void _al_sha1_final(sha1_context *context, unsigned char *buf_digest) {

	unsigned long lenL = (context->total & 0x1fffffff)<<3;
	unsigned long lenH = (context->total >> (32-3));

	_al_sha1_update(context, 1, padding);
	if (context->filled > 56) {
		_al_sha1_update(context, 64-context->filled, &padding[1]);
	}
	int i;
	for (i = context->filled; i < 56; i++) context->buf[i] = 0;
	context->buf[i++] = (unsigned char)((lenH >> 24) & 255);
	context->buf[i++] = (unsigned char)((lenH >> 16) & 255);
	context->buf[i++] = (unsigned char)((lenH >> 8 ) & 255);
	context->buf[i++] = (unsigned char)((lenH	   ) & 255);
	context->buf[i++] = (unsigned char)((lenL >> 24) & 255);
	context->buf[i++] = (unsigned char)((lenL >> 16) & 255);
	context->buf[i++] = (unsigned char)((lenL >> 8 ) & 255);
	context->buf[i++] = (unsigned char)((lenL	   ) & 255);

	_al_sha1_compute_block(context);

	for(i = 0; i < 5; i++) {
		*buf_digest++ = (unsigned char)((context->h[i] >> 24) & 255);
		*buf_digest++ = (unsigned char)((context->h[i] >> 16) & 255);
		*buf_digest++ = (unsigned char)((context->h[i] >>  8) & 255);
		*buf_digest++ = (unsigned char)((context->h[i]		) & 255);
	}
	delete context;
}

#define S1(x)	(((x)<<1)|((x)>>(32-1)))
#define S5(x)	(((x)<<5)|((x)>>(32-5)))
#define S30(x)	(((x)<<30)|((x)>>(32-30)))

void Ft(int s, unsigned long &t, unsigned long a, unsigned long &b,
		unsigned long c, unsigned long d, unsigned long e,
		unsigned long w[]) {
	switch ((int)s/20) {
	case 0: t = S5(a) + ((b & c) | (~b & d)) + e + w[s&15] + 0x5a827999; break;
	case 1: t = S5(a) + (b ^ c ^ d) + e + w[s&15] + 0x6ed9eba1; break;
	case 2: t = S5(a) + ((b & (c|d)) | (c & d)) + e + w[s&15] + 0x8F1bbcdc; break;
	case 3: t = S5(a) + (b ^ c ^ d) + e + w[s&15] + 0xca62c1d6; break;
	}
	b = S30(b);
}

void Ftx(int s, unsigned long &t, unsigned long a, unsigned long &b,
		 unsigned long c, unsigned long d, unsigned long e,
		 unsigned long w[]) {
	t = w[(s+13)&15] ^ w[(s+8)&15] ^ w[(s+2)&15] ^ w[s&15];
	w[s&15] = S1(t);
	Ft(s,t,a,b,c,d,e,w);
}

void _al_sha1_compute_block(sha1_context *context) {
	unsigned long a, b, c, d, e, f, w[16];

	for (int i = 0; i < 16; i++) {
		w[i] = (context->buf[i*4  ] << 24) +
			   (context->buf[i*4+1] << 16) +
			   (context->buf[i*4+2] <<	8) +
			   (context->buf[i*4+3]);
	}

	a = context->h[0];
	b = context->h[1];
	c = context->h[2];
	d = context->h[3];
	e = context->h[4];

	Ft( 0,f,a,b,c,d,e,w);
	Ft( 1,e,f,a,b,c,d,w);
	Ft( 2,d,e,f,a,b,c,w);
	Ft( 3,c,d,e,f,a,b,w);
	Ft( 4,b,c,d,e,f,a,w);
	Ft( 5,a,b,c,d,e,f,w);
	Ft( 6,f,a,b,c,d,e,w);
	Ft( 7,e,f,a,b,c,d,w);
	Ft( 8,d,e,f,a,b,c,w);
	Ft( 9,c,d,e,f,a,b,w);
	Ft(10,b,c,d,e,f,a,w);
	Ft(11,a,b,c,d,e,f,w);
	Ft(12,f,a,b,c,d,e,w);
	Ft(13,e,f,a,b,c,d,w);
	Ft(14,d,e,f,a,b,c,w);
	Ft(15,c,d,e,f,a,b,w);
	Ftx(16,b,c,d,e,f,a,w);
	Ftx(17,a,b,c,d,e,f,w);
	Ftx(18,f,a,b,c,d,e,w);
	Ftx(19,e,f,a,b,c,d,w);
	Ftx(20,d,e,f,a,b,c,w);
	Ftx(21,c,d,e,f,a,b,w);
	Ftx(22,b,c,d,e,f,a,w);
	Ftx(23,a,b,c,d,e,f,w);
	Ftx(24,f,a,b,c,d,e,w);
	Ftx(25,e,f,a,b,c,d,w);
	Ftx(26,d,e,f,a,b,c,w);
	Ftx(27,c,d,e,f,a,b,w);
	Ftx(28,b,c,d,e,f,a,w);
	Ftx(29,a,b,c,d,e,f,w);
	Ftx(30,f,a,b,c,d,e,w);
	Ftx(31,e,f,a,b,c,d,w);
	Ftx(32,d,e,f,a,b,c,w);
	Ftx(33,c,d,e,f,a,b,w);
	Ftx(34,b,c,d,e,f,a,w);
	Ftx(35,a,b,c,d,e,f,w);
	Ftx(36,f,a,b,c,d,e,w);
	Ftx(37,e,f,a,b,c,d,w);
	Ftx(38,d,e,f,a,b,c,w);
	Ftx(39,c,d,e,f,a,b,w);
	Ftx(40,b,c,d,e,f,a,w);
	Ftx(41,a,b,c,d,e,f,w);
	Ftx(42,f,a,b,c,d,e,w);
	Ftx(43,e,f,a,b,c,d,w);
	Ftx(44,d,e,f,a,b,c,w);
	Ftx(45,c,d,e,f,a,b,w);
	Ftx(46,b,c,d,e,f,a,w);
	Ftx(47,a,b,c,d,e,f,w);
	Ftx(48,f,a,b,c,d,e,w);
	Ftx(49,e,f,a,b,c,d,w);
	Ftx(50,d,e,f,a,b,c,w);
	Ftx(51,c,d,e,f,a,b,w);
	Ftx(52,b,c,d,e,f,a,w);
	Ftx(53,a,b,c,d,e,f,w);
	Ftx(54,f,a,b,c,d,e,w);
	Ftx(55,e,f,a,b,c,d,w);
	Ftx(56,d,e,f,a,b,c,w);
	Ftx(57,c,d,e,f,a,b,w);
	Ftx(58,b,c,d,e,f,a,w);
	Ftx(59,a,b,c,d,e,f,w);
	Ftx(60,f,a,b,c,d,e,w);
	Ftx(61,e,f,a,b,c,d,w);
	Ftx(62,d,e,f,a,b,c,w);
	Ftx(63,c,d,e,f,a,b,w);
	Ftx(64,b,c,d,e,f,a,w);
	Ftx(65,a,b,c,d,e,f,w);
	Ftx(66,f,a,b,c,d,e,w);
	Ftx(67,e,f,a,b,c,d,w);
	Ftx(68,d,e,f,a,b,c,w);
	Ftx(69,c,d,e,f,a,b,w);
	Ftx(70,b,c,d,e,f,a,w);
	Ftx(71,a,b,c,d,e,f,w);
	Ftx(72,f,a,b,c,d,e,w);
	Ftx(73,e,f,a,b,c,d,w);
	Ftx(74,d,e,f,a,b,c,w);
	Ftx(75,c,d,e,f,a,b,w);
	Ftx(76,b,c,d,e,f,a,w);
	Ftx(77,a,b,c,d,e,f,w);
	Ftx(78,f,a,b,c,d,e,w);
	Ftx(79,e,f,a,b,c,d,w);

	context->h[0] += e;
	context->h[1] += f;
	context->h[2] += a;
	context->h[3] += b;
	context->h[4] += c;
}
