/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _ADV_SOCKET_H
#define _ADV_SOCKET_H

#ifdef	_WINDOWS
#include <winsock.h>
typedef int socklen_t;
#endif

#ifdef	_LINUX
#include <sys/socket.h>
#include <glib.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SOCKET	int
#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1
#define closesocket close
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#endif

#ifdef	_AL_OPENSSL
#undef	I
#include <openssl/ssl.h>
#include <openssl/asn1.h>
#include <openssl/err.h>
#endif

class ADvDataQueue;

class AL_EXT_CLASS ADvSocket {
public:
	ADvSocket(int kind);
	ADvSocket(int kind, SOCKET s);
	~ADvSocket();

	static void Close(ADvSocket*);

	static void Initialize();
	static void Finalize();

	static int CreateStreamSocket();
	static int CreateDgramSocket();
	static int Close(int socket_id);

	static int Accept(int socket_id);
	static int Accept(ADvSocket *socket);

	static int Recv(ADvSocket*, char *buff, long size);
	static int Recv(int socket_id,
		AGmBinary *bin, long index, long size,
		long mode = 0, long size2 = 0x7fffffff);
	static int Recv(ADvSocket*, AGmBinary *bin, long index, long size,
		long mode = 0, long size2 = 0x7fffffff);

	static int RecvFrom(int socket_id, char *buff, long size);
	static int RecvFrom(ADvSocket*, char *buff, long size);
	static int Send(int socket_id, char *buff, long size);
	static int Send(ADvSocket*, char *buff, long size);

	static ADvSocket *GetSocket(int socket_id);
	static ADvSocket *_GetSocket(int socket_id);
	static void Link(ADvSocket* socket);
	static void Unlink(ADvSocket* socket);

	static const char* GetErrorString();

	static int available;
	static const char *global_error;
#ifdef	_WINDOWS
	static CRITICAL_SECTION cs;
#endif
	static long socket_id_seq;
	static ADvSocket *root;
	static AGmNode *closed_children;
	static int exit_watch_timer_id;
	static int exit_watch_interval;
	static void GCmark();

	ADvSocket *next, *prev;
	int socket_id;

	int kind;
	int connected;
	SOCKET	s;
	const char *error;

#ifdef _AL_OPENSSL
	SSL_CTX *ctx;
	SSL 	*ssl;
	long	auth;
#endif

	ADvDataQueue	*accept_queue;
	ADvDataQueue	*recv_queue;
	ADvDataQueue	*send_queue;

#ifdef	_WINDOWS
	HWND		hWnd, hWnd2;
#endif
#ifdef	_LINUX
	long		hWnd, hWnd2;
#endif
#if _WINDOWS && _64BIT
	long long	msg, msg2;
#else
	long		msg, msg2;
#endif
	AGmBinary	*bin, *bin2;

	long		index;
	long		size;
	long		mode;
	long		size2;
	int 	recv_wait;
	int 	send_wait;

#ifdef	_WINDOWS
	HANDLE	wait_event;

	HANDLE	accept_thread;
	HANDLE	recv_thread;
	HANDLE	send_thread;
#endif
#ifdef	_LINUX
	int		wait_cond_flag;
	GCond	wait_cond;

	GThread* 	accept_thread;
	GThread* 	recv_thread;
	GThread* 	send_thread;
#endif

	SOCKADDR_IN *addr_port;
};

class ADvDataQueueItem {
public:
	ADvDataQueueItem();
	~ADvDataQueueItem();

	ADvDataQueueItem *next, *prev;
	long socket_id;
	AGmBinary *bin;
	AGmString *str;
};

class ADvDataQueue {
public:
	ADvDataQueue();
	~ADvDataQueue();

	ADvDataQueueItem *Alloc();
	ADvDataQueueItem *Alloc(AGmBinary *bin, long index, long size);
	ADvDataQueueItem *Alloc(AGmString *str);
	void Free(ADvDataQueueItem*);

	void Put(ADvDataQueueItem*);
	ADvDataQueueItem *Get();

	ADvDataQueueItem *root;
	ADvDataQueueItem *free_ls;
	ADvDataQueueItem *buffer;

#ifdef	_WINDOWS
	CRITICAL_SECTION cs;
#endif
#ifdef	_LINUX
	GMutex mutex;
#endif
};

/* The IP header */
typedef struct {
	unsigned int h_len:4;		   // length of the header
	unsigned int version:4; 	   // Version of IP
	unsigned char tos;			   // Type of service
	unsigned short total_len;	   // total length of the packet
	unsigned short ident;		   // unique identifier
	unsigned short frag_and_flags; // flags
	unsigned char  ttl; 
	unsigned char proto;		   // protocol (TCP, UDP etc)
	unsigned short checksum;	   // IP checksum

	unsigned int sourceIP;
	unsigned int destIP;
} IpHeader;

//
// ICMP header
//
typedef struct {
  unsigned char i_type;
  unsigned char i_code; /* type sub code */
  unsigned short i_cksum;
  unsigned short i_id;
  unsigned short i_seq;
  /* This is not the std header, but we reserve space for time */
  unsigned long timestamp;
} IcmpHeader;

#endif	/* _ADV_SOCKET_H */

