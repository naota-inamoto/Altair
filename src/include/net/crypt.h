/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AGM_CRYPT_H
#define _AGM_CRYPT_H

// -------- N-RSA

#define P_NUM_BLOCK_SIZE 6553600	

struct p_num_block {
	struct p_num_block* next;
	unsigned long p_num[P_NUM_BLOCK_SIZE];
};

// -------- Prime Number Generator
class AL_EXT_CLASS AGmPNumMgr {
private:
	AGmPNumMgr();
	~AGmPNumMgr();

public:
	static AGmPNumMgr* GetInstance();
	static void Finalize();

	int is_p_num(unsigned long p_num);
	unsigned long get_p_num_greater_than(unsigned long num);

private:
	void add_p_num_to_list(unsigned long p_num);
	void clear_p_num_list();
	int __is_p_num(unsigned long p_num);

	static AGmPNumMgr* inst;
	static struct p_num_block* start_block;
	static struct p_num_block* end_block;
	static int p_num_index;
	static unsigned long k;
};

int _al_nrsa_generate_keypair(
		unsigned long p_hint,
		unsigned long q_hint,
		unsigned long s_hint,
		AGmNode*& sk,
		AGmNode*& pk,
		AGmNode*& mod);

// -------- N-RSA
// p: prime number
// q: prime number
// m = p * q
// k = lcm(p-1, q-1)
// sk: sk < k && gcd(sk, k) == 1
// pk: (sk * pk) mod k == 1

// encrypt/decrypt
typedef struct {
	// block size ( < size_mod )
	long size_block;
	// key
	long size_key;
	unsigned char *buf_key;
	long size_mod;
	unsigned char *buf_mod;
	// tmp data
	unsigned char *buf_tmp; // size_mod * 2
	unsigned char *buf_aa;	// size_mod * 2
} n_rsa_context;

n_rsa_context *_al_nrsa_init(
		long size_block,
		AGmNode *key,
		AGmNode *mod);
		// size_mod > size_block

void _al_nrsa_encrypt(
		n_rsa_context *context,
		long size_data, unsigned char *buf_data, unsigned char *buf_data2);
		// size of buf_data = size_block * n
		// size of buf_data2 = n * size_mod
void _al_nrsa_decrypt(
		n_rsa_context *context,
		long size_data, unsigned char *buf_data, unsigned char *buf_data2);
		// size of buf_data = size_mod * n
		// size of buf_data2 = n * size_block

void _al_nrsa_kernel(
		n_rsa_context *context,
		long size_a, unsigned char *buf_a, unsigned char *buf_b);

void _al_nrsa_final(n_rsa_context *context);

// -------- N-DES
typedef struct {
	// block size
	long size_block;
	// num repeat
	long n_repeat;
	// tmp data
	long size_block8;	  // size_block * 8
	unsigned char *buf_a; // size_block8
	unsigned char *buf_b; // size_block8
	// key
	long size_xor8; // min(size_block8, size_key2_8)
	unsigned char **key_transpose; // size_block8
	unsigned char **key_xor;	   // size_xor8
} n_des_context;

n_des_context *_al_ndes_init(
		long size_block,
		long size_key, unsigned char *buf_key);
void _al_ndes_first_key(long size_key, unsigned char *buf_key, unsigned char *buf_key_a);
long _al_ndes_get_exp(long size);
long _al_ndes_rotateL(long exp, long size, unsigned char *buf);

void _al_ndes_encrypt(n_des_context *context,
		long size_data, unsigned char *buf_data, unsigned char *buf_data2);
void _al_ndes_decrypt(n_des_context *context,
		long size_data, unsigned char *buf_data, unsigned char *buf_data2);

void _al_ndes_final(n_des_context *context);

// -------- MD5
typedef struct {
	unsigned long i[2]; 		// number of bits handled mod 2^64
	unsigned long buf[4];		// scratch buffer
	unsigned char in[64];		// input buffer
} md5_context;

md5_context *_al_md5_init();
void _al_md5_update(md5_context *context, long size, unsigned char *buf);
void _al_md5_final(md5_context *context, unsigned char *buf_digest);

void _al_md5_transform(unsigned long *buf, unsigned long *in);

// -------- SHA-1
typedef struct {
	unsigned long total;	// total length log
	unsigned long filled;	// bytes filled in buf
	unsigned char buf[64];	// 512bit Mi buffer
	unsigned long h[5]; 	// state across blocks
} sha1_context;

sha1_context *_al_sha1_init();
void _al_sha1_update(sha1_context *context, long size, unsigned char *buf);
void _al_sha1_final(sha1_context *context, unsigned char *buf_digest);

void _al_sha1_compute_block(sha1_context *context);

#endif	/* _AGM_CRYPT_H */
