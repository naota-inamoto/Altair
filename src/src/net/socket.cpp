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
#include <interp/interp.h>

#include <net/socket.h>

static int init = 0;

int ADvSocket::available = 0;
const char* ADvSocket::global_error = NULL;
ADvSocket* ADvSocket::root = NULL;
long ADvSocket::socket_id_seq = 1;

#define SOCKET_NONE 	0
#define STREAM_SOCKET	1
#define DGRAM_SOCKET	2

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#ifdef	_WINDOWS
#include <fcntl.h>
CRITICAL_SECTION ADvSocket::cs;
#endif

#ifdef	_LINUX
#include <sys/wait.h>
#include <sys/poll.h>
#include <errno.h>
static void sigpipe_handler(int x) { }
GMutex sock_mutex;
#endif	// _LINUX

#ifdef	_AL_OPENSSL
AGmNode *from_asn1_time(ASN1_TIME *tm) {
	long y = 0, M = 0, d = 0, h = 0, m = 0, s = 0;
	if (tm->type == V_ASN1_UTCTIME) {
		char *v = (char *)tm->data;
		y = (v[0] - '0') * 10 + (v[1] - '0');
		if (y < 50) y += 100;
		y += 1900;
		M = (v[2]-'0') * 10 + (v[3] - '0');
		d = (v[4] - '0') * 10 + (v[5] - '0');
		h = (v[6] - '0') * 10 + (v[7] - '0');
		m = (v[8] - '0') * 10 + (v[9] - '0');
		if ((v[10] >= '0') && (v[10] <= '9') && (v[11] >= '0') && (v[11] <= '9'))
			s =  (v[10] - '0') * 10 + (v[11] - '0');
	} else if (tm->type == V_ASN1_GENERALIZEDTIME) {
		char *v = (char *)tm->data;
		y = (v[0] - '0') * 1000 + (v[1] - '0') * 100 + (v[2] - '0') * 10 + (v[3] - '0');
		M = (v[4] - '0') * 10 + (v[5] - '0');
		d = (v[6] - '0') * 10 + (v[7] - '0');
		h = (v[8] - '0') * 10 + (v[9] - '0');
		m =  (v[10] - '0') * 10 + (v[11] - '0');
		if ((v[12] >= '0') && (v[12] <= '9') && (v[13] >= '0') && (v[13] <= '9'))
			s = (v[12] -'0') * 10 + (v[13] - '0');
	}
	return AGm::List(AGm::Integer(y), AGm::Integer(M), AGm::Integer(d),
		AGm::Integer(h), AGm::Integer(m), AGm::Integer(s));
}
static int verify_depth = 1;
#endif

static AGmString* get_X_X_X_X_from_IP(long ip) {
	char buf[32];
	long v1 = (ip >> 24) & 0xff;
	long v2 = (ip >> 16) & 0xff;
	long v3 = (ip >> 8) & 0xff;
	long v4 = ip & 0xff;
	art_sprintf_4(buf, 32, "%ld.%ld.%ld.%ld", v1, v2, v3, v4);
	return new AGmString(buf);
}
static long get_IP_from_X_X_X_X(const char* addr) {
	AGmString *s = new AGmString(addr);
	AGmFile *f = new AGmFile();
	f->Create(s, AGM_FILE_STR_READ);
	long ip = 0;
#if	_WINDOWS && _64BIT
	long long v1, v2, v3, v4;
#else
	long v1, v2, v3, v4;
#endif
	for (;;) {
		if (f->_ReadInteger(v1)) break;
		if (f->_MatchString(".") == 0) break;
		if (f->_ReadInteger(v2)) break; 
		if (f->_MatchString(".") == 0) break;
		if (f->_ReadInteger(v3)) break; 
		if (f->_MatchString(".") == 0) break;
		if (f->_ReadInteger(v4)) break; 
		if (f->Input() != EOF) break;
		ip = (long)((v1 << 24) + (v2 << 16)  + (v3 << 8) + v4);
	}
	delete f; delete s;
	return ip;
}

// ADvSocket
ADvSocket::ADvSocket(int kind) {
#ifdef	_WINDOWS
	accept_thread = NULL;
	recv_thread = NULL;
	send_thread = NULL;

	recv_wait = 0;
	send_wait = 0;

	wait_event = NULL;
#endif
#ifdef	_LINUX
	accept_thread = NULL;
	recv_thread = NULL;
	send_thread = NULL;

	recv_wait = 0;
	send_wait = 0;

	g_cond_init(&wait_cond);
#endif

	accept_queue = NULL;
	recv_queue = NULL;
	send_queue = NULL;

	this->kind = kind;
	connected = 0;
	switch (kind) {
	case SOCKET_NONE:
		s = INVALID_SOCKET;
		socket_id = 0;
		error = NULL;
		return;
	case STREAM_SOCKET:
		s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		{
			int value = 1;
			setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
				(char*)&value, sizeof(value));
		}
		break;
	case DGRAM_SOCKET:
		s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		{
			int value = 1;
			setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
				(char*)&value, sizeof(value));
		}
		break;
	}
	if (s == INVALID_SOCKET) {
		error = GetErrorString();
	} else {
		error = NULL;
	}
#ifdef	_AL_OPENSSL
	ctx = NULL;
	ssl = NULL;
	auth = 0;
#endif
}
ADvSocket::ADvSocket(int kind, SOCKET s) {
#ifdef	_WINDOWS
	accept_thread = NULL;
	recv_thread = NULL;
	send_thread = NULL;

	recv_wait = 0;
	send_wait = 0;

	wait_event = NULL;
#endif
#ifdef	_LINUX
	accept_thread = NULL;
	recv_thread = NULL;
	send_thread = NULL;

	recv_wait = 0;
	send_wait = 0;

	g_cond_init(&wait_cond);
#endif

	accept_queue = NULL;
	recv_queue = NULL;
	send_queue = NULL;

	this->kind = kind;
	this->s = s;
	error = NULL;
#ifdef	_AL_OPENSSL
	ctx = NULL;
	ssl = NULL;
	auth = 0;
#endif
}
ADvSocket::~ADvSocket() {
}
void ADvSocket::Close(ADvSocket *sock) {
#ifdef	_WINDOWS
	if (sock->accept_thread) {
		::TerminateThread(sock->accept_thread, (DWORD)-1);
		::CloseHandle(sock->accept_thread);
	}
	if (sock->recv_thread) {
		::TerminateThread(sock->recv_thread, (DWORD)-1);
		::CloseHandle(sock->recv_thread);
	}
	if (sock->send_thread) {
		::TerminateThread(sock->send_thread, (DWORD)-1);
		::CloseHandle(sock->send_thread);
	}
	if (sock->wait_event) ::CloseHandle(sock->wait_event);
#endif
#ifdef	_LINUX
	if (sock->accept_thread) {
	}
	if (sock->recv_thread) {
	}
	if (sock->send_thread) {
	}
	g_cond_clear(&sock->wait_cond);

#endif
#ifdef	_AL_OPENSSL
	if (sock->ssl) {
#ifdef	_WINDOWS
		::EnterCriticalSection(&ADvSocket::cs);
#endif
		SSL_shutdown(sock->ssl);
		SSL_free(sock->ssl);
#ifdef	_WINDOWS
		::LeaveCriticalSection(&ADvSocket::cs);
#endif
	}
	if (sock->ctx) {
		SSL_CTX_free(sock->ctx);
	}
#endif
	// close socket after 3 sec
	AGm::CreateArc(closed_children, AGm::List(AGm::Integer(3), AGm::PtrCell(sock)), AGm::Integer((long)sock->socket_id));

	if (sock->accept_queue) delete sock->accept_queue;
	if (sock->recv_queue) delete sock->recv_queue;
	if (sock->send_queue) delete sock->send_queue;
}

AGmNode *ADvSocket::closed_children = NULL;
int ADvSocket::exit_watch_timer_id;
int ADvSocket::exit_watch_interval = 1000;

void ADvSocket::GCmark() {
	AGm::_GCmark(closed_children);
}
#ifdef	_WINDOWS
static void exitWatchFunc(HWND /*hwnd*/, UINT /*msg*/, UINT /*idEvent*/, DWORD /*current_time*/) {
	CWinApp *app = ADvGetApp();
	if (app && app->m_pMainWnd) {
		::PostMessage(app->m_pMainWnd->m_hWnd, WM_USER + 211, 0, 0);
	}
}
void exitTimerFunc() {
	AGmLink *l; AGmNode *dst; ADvSocket *socket;
#if	_WINDOWS && _64BIT
	long long *time;
#else
	long *time;
#endif
	for (l = AGm::next_outlink(ADvSocket::closed_children, NULL);
			l; l = AGm::next_outlink(ADvSocket::closed_children, l)) {
		dst = l->arc->dst;
		time = &dst->HI; socket = (ADvSocket*)dst->TH->a.p.ptr;
		if (--(*time) == 0) {
			switch (socket->kind) {
			case STREAM_SOCKET:
			case DGRAM_SOCKET:
				if (socket->s != INVALID_SOCKET) {
					shutdown(socket->s, 2);
					closesocket(socket->s);
				}
				break;
			default:
				break;
			}

			delete socket;
			
			l = l->prev;
			AGm::DeleteArc(l->next->arc);
		}
	}
}
#endif
#ifdef	_LINUX
static gint exit_watch_func(gpointer) {
	AGmLink *l; AGmNode *dst; long *time; ADvSocket *socket;
	for (l = AGm::next_outlink(ADvSocket::closed_children, NULL);
			l; l = AGm::next_outlink(ADvSocket::closed_children, l)) {
		dst = l->arc->dst;
		time = &dst->HI; socket = (ADvSocket*)dst->TH->a.p.ptr;
		if (--(*time) == 0) {

			shutdown(socket->s, 2);
			closesocket(socket->s);

			delete socket;
			
			l = l->prev;
			AGm::DeleteArc(l->next->arc);
		}
	}
	return TRUE;
}
#endif

void ADvSocket::Initialize() {
	if (init) return;
#ifdef	_WINDOWS
	WORD wVersionRequested = MAKEWORD(2, 0);
	WSADATA wsData;
	int rc = WSAStartup(wVersionRequested, &wsData);
	if (rc) {
		global_error = GetErrorString();
		return;
	}
	::InitializeCriticalSection(&cs);
#endif
#ifdef _LINUX
	g_mutex_init(&sock_mutex);
#endif
	root = new ADvSocket(SOCKET_NONE);
	root->prev = root->next = root;
	available = 1;
	init = 1;
#ifdef	_AL_OPENSSL
	SSL_library_init();
	SSL_load_error_strings();
	OpenSSL_add_ssl_algorithms();
#endif
#ifdef	_WINDOWS
	closed_children = AGm::Ref(AGm::Cons(NULL));
	exit_watch_timer_id = (int)AGm::GetUniqueID();
	::SetTimer((HWND)NULL, exit_watch_timer_id, exit_watch_interval, (TIMERPROC)exitWatchFunc);
#endif
#ifdef	_LINUX
	signal(SIGPIPE, sigpipe_handler);
	closed_children = AGm::Ref(AGm::Cons(NULL));
	exit_watch_timer_id = g_timeout_add(exit_watch_interval, exit_watch_func, (gpointer)NULL);
#endif
}
void ADvSocket::Finalize() {
	if (!available)
		return;

	if (init) {
		init = 0;
#ifdef	_WINDOWS
		::KillTimer((HWND)NULL, exit_watch_timer_id);
		::EnterCriticalSection(&cs);
#endif
#ifdef	_LINUX
		g_source_remove(exit_watch_timer_id);
		g_mutex_lock(&sock_mutex);
#endif
		for (ADvSocket *s = root->next; s != root; s = s->next) {
			ADvSocket *s1 = s;
			s = s->prev;
			s1->prev->next = s1->next;
			s1->next->prev = s1->prev;
			ADvSocket::Close(s1);
		}
		delete root;
#ifdef	_WINDOWS
		::LeaveCriticalSection(&cs);
		::DeleteCriticalSection(&cs);
		WSACleanup();
#endif
#ifdef	_LINUX
		g_mutex_unlock(&sock_mutex);
		g_mutex_clear(&sock_mutex);
#endif
		AGm::Unref(closed_children);
		closed_children = NULL;
#ifdef	_AL_OPENSSL
		ERR_free_strings();
		EVP_cleanup();
		CRYPTO_cleanup_all_ex_data();
#endif
	}
}

int ADvSocket::CreateStreamSocket() {
	ADvSocket *socket = new ADvSocket(STREAM_SOCKET);
	if (socket->error) {
		delete socket;
		return -1;
	}
	Link(socket);
	return socket->socket_id;
}
int ADvSocket::CreateDgramSocket() {
	ADvSocket *socket = new ADvSocket(DGRAM_SOCKET);
	if (socket->error) {
		delete socket;
		return -1;
	}
	Link(socket);
	return socket->socket_id;
}
int ADvSocket::Close(int socket_id) {
	ADvSocket *socket = GetSocket(socket_id);
	if (socket == NULL) return 0;
	Unlink(socket);
	ADvSocket::Close(socket);
	return 1;
}

int ADvSocket::Accept(int socket_id) {
	ADvSocket *socket = GetSocket(socket_id);
	if (socket == NULL) return -1;
	return Accept(socket);
}
int ADvSocket::Accept(ADvSocket *socket) {
	SOCKET s = accept(socket->s, NULL, NULL);
	if (s == INVALID_SOCKET) {
		socket->error = ADvSocket::GetErrorString();
		return -1;
	}
	ADvSocket *socket2 = new ADvSocket(STREAM_SOCKET, s);
	socket2->connected = 1;
	Link(socket2);
#ifdef	_AL_OPENSSL
	if (socket->ctx) {
#ifdef	_WINDOWS
		::EnterCriticalSection(&cs);
#endif
#ifdef	_LINUX
		g_mutex_lock(&sock_mutex);
#endif
		BIO *sbio = BIO_new_socket((int)socket2->s, BIO_NOCLOSE);
		socket2->ssl = SSL_new(socket->ctx);
		SSL_set_bio(socket2->ssl, sbio, sbio);
		if (SSL_accept(socket2->ssl) <= 0) {
			socket2->error = "SSL_accept error";
		}
#ifdef	_WINDOWS
		::LeaveCriticalSection(&cs);
#endif
#ifdef	_LINUX
		g_mutex_unlock(&sock_mutex);
#endif
	}
#endif
	return socket2->socket_id;
}

int ADvSocket::Recv(ADvSocket *sock, char *buff, long size) {
	if (size == 0) return 0;
	int rc;
#ifdef	_AL_OPENSSL
	if (sock->ssl) {
#ifdef	_WINDOWS
		::EnterCriticalSection(&cs);
#endif
#ifdef	_LINUX
		g_mutex_lock(&sock_mutex);
#endif
		rc = SSL_read(sock->ssl, buff, size);
		switch (SSL_get_error(sock->ssl, rc)) {
		case SSL_ERROR_NONE:
			break;
		case SSL_ERROR_WANT_WRITE:
		case SSL_ERROR_WANT_READ:
		case SSL_ERROR_WANT_X509_LOOKUP:
			break;
		case SSL_ERROR_SYSCALL:
		case SSL_ERROR_SSL:
			sock->error = "SSL_read error";
			return SOCKET_ERROR;
		case SSL_ERROR_ZERO_RETURN:
			break;
		}
#ifdef	_WINDOWS
		::LeaveCriticalSection(&cs);
#endif
#ifdef	_LINUX
		g_mutex_unlock(&sock_mutex);
#endif
	} else {
		rc = recv(sock->s, buff, size, 0);
	}
#else
	rc = recv(sock->s, buff, size, 0);
#endif
	if (rc == SOCKET_ERROR) {
		sock->error = ADvSocket::GetErrorString();
	}
	if (rc == 0) {
		sock->error = "zero byte read";
	}
	return rc;
}
int ADvSocket::Recv(int socket_id, AGmBinary *bin, long index, long size,
		long mode, long size2) {
	ADvSocket *socket = GetSocket(socket_id);
	if (socket == NULL)
		return SOCKET_ERROR;
	return Recv(socket, bin, index, size, mode, size2);
}
int ADvSocket::Recv(ADvSocket *sock, AGmBinary *bin, long index, long size,
		long mode, long size2) {
	if (sock->error) return SOCKET_ERROR;

	if (mode == 0) {
		char *buff = (char*)bin->Buffer() + index;
		return Recv(sock, buff, size);
	}

	char *buff = (char*)bin->Buffer() + index;
	long max = bin->Length();

	int mode2 = 0;
	long count = 0;

	while (count < size2) {
		if (index >= max) {
#ifdef	_WINDOWS
			::EnterCriticalSection(&cs);
			bin->Expand(max);
			::LeaveCriticalSection(&cs);
#endif
#ifdef	_LINUX
			g_mutex_lock(&sock_mutex);
			bin->Expand(max);
			g_mutex_unlock(&sock_mutex);
#endif
			buff = (char*)bin->Buffer() + index;
			max *= 2;
		}
		int rc;
#ifdef	_AL_OPENSSL
		if (sock->ssl) {
#ifdef	_WINDOWS
			::EnterCriticalSection(&cs);
#endif
#ifdef	_LINUX
			g_mutex_lock(&sock_mutex);
#endif
			rc = SSL_read(sock->ssl, buff, 1);
			switch (SSL_get_error(sock->ssl, rc)) {
			case SSL_ERROR_NONE:
				break;
			case SSL_ERROR_WANT_WRITE:
			case SSL_ERROR_WANT_READ:
			case SSL_ERROR_WANT_X509_LOOKUP:
				break;
			case SSL_ERROR_SYSCALL:
			case SSL_ERROR_SSL:
				sock->error = "SSL_read error";
				return SOCKET_ERROR;
			case SSL_ERROR_ZERO_RETURN:
				break;
			}
#ifdef	_WINDOWS
			::LeaveCriticalSection(&cs);
#endif
#ifdef	_LINUX
			g_mutex_unlock(&sock_mutex);
#endif
		} else {
			rc = recv(sock->s, buff, 1, 0);
		}
#else
		rc = recv(sock->s, buff, 1, 0);
#endif
		if (rc == SOCKET_ERROR) {
			sock->error = ADvSocket::GetErrorString();
			return rc;
		}
		if (rc == 0) {
			sock->error = "zero byte read";
			return SOCKET_ERROR;
		}
		switch (mode) {
		case 1:
			if (*buff == '\n') return count;
		case 2:
			switch (mode2) {
			case 0:
				if (*buff == '\r') {
					mode2 = 1;
				} else if (*buff == '\n') {
					mode2 = 2;
				} else {
					mode2 = 0;
				}
				break;
			case 1:
				if (*buff == '\n') {
					mode2 = 2;	
				} else {
					mode2 = 0;
				}
				break;
			case 2:
				if (*buff == '\r') {
					mode2 = 3;
				} else if (*buff == '\n') {
					return ++count;
				} else {
					mode2 = 0;
				}
				break;
			case 3:
				if (*buff == '\n') {
					return ++count;
				} else {
					mode2 = 0;
				}
				break;
			}
		}
		buff++; index++; count++;
	}
	return count;
}
int ADvSocket::RecvFrom(int socket_id, char *buff, long size) {
	ADvSocket *socket = GetSocket(socket_id);
	if (socket == NULL)
		return SOCKET_ERROR;
	return RecvFrom(socket, buff, size);
}
int ADvSocket::RecvFrom(ADvSocket *sock, char *buff, long size) {
	if (size < 0) return 0;
	SOCKADDR_IN addr;
	socklen_t addrsize = sizeof(addr);
	int rc = recvfrom(sock->s, buff, size, 0, (SOCKADDR*)&addr, &addrsize);
	return rc;
}

int ADvSocket::Send(int socket_id, char *buff, long size) {
	ADvSocket *socket = GetSocket(socket_id);
	if (socket == NULL)
		return SOCKET_ERROR;
	return Send(socket, buff, size);
}
int ADvSocket::Send(ADvSocket *sock, char *buff, long size) {
	if (sock->error) return SOCKET_ERROR;
	int rc, count = size;
	while (count > 0) {
#ifdef	_AL_OPENSSL
		if (sock->ssl) {
#ifdef	_WINDOWS
			::EnterCriticalSection(&cs);
#endif
#ifdef	_LINUX
			g_mutex_lock(&sock_mutex);
#endif
			rc = SSL_write(sock->ssl, buff, count);
			switch (SSL_get_error(sock->ssl, rc)) {
			case SSL_ERROR_NONE:
				break;
			case SSL_ERROR_WANT_WRITE:
			case SSL_ERROR_WANT_READ:
			case SSL_ERROR_WANT_X509_LOOKUP:
				break;
			case SSL_ERROR_SYSCALL:
			case SSL_ERROR_SSL:
				sock->error = "SSL_write error";
				return SOCKET_ERROR;
			case SSL_ERROR_ZERO_RETURN:
				break;
			}
#ifdef	_WINDOWS
			::LeaveCriticalSection(&cs);
#endif
#ifdef	_LINUX
			g_mutex_unlock(&sock_mutex);
#endif
		} else {
			rc = send(sock->s, buff, count, 0);
		}
#else
		rc = send(sock->s, buff, count, 0);
#endif
		if (rc == SOCKET_ERROR) {
			sock->error = ADvSocket::GetErrorString();
			return rc;
		}
		buff += rc; count -= rc;
	}
	return size;
}

ADvSocket *ADvSocket::GetSocket(int socket_id) {
#ifdef	_WINDOWS
	::EnterCriticalSection(&cs);
	ADvSocket *s = _GetSocket(socket_id);
	::LeaveCriticalSection(&cs);
#endif
#ifdef	_LINUX
	g_mutex_lock(&sock_mutex);
	ADvSocket *s = _GetSocket(socket_id);
	g_mutex_unlock(&sock_mutex);
#endif
	return s;
}
ADvSocket *ADvSocket::_GetSocket(int socket_id) {
	ADvSocket *s;
	for (s = root->next; s != root; s = s->next) {
		if (s->socket_id == socket_id) break;
	}
	if (s == root) return NULL;
	return s;
}
void ADvSocket::Link(ADvSocket* socket) {
#ifdef	_WINDOWS
	::EnterCriticalSection(&cs);
#endif
#ifdef	_LINUX
	g_mutex_lock(&sock_mutex);
#endif
	socket->prev = root->prev; socket->next = root;
	root->prev->next = socket; root->prev = socket;
	socket->socket_id = socket_id_seq;
	while (_GetSocket(socket_id_seq)) {
		if (++socket_id_seq > LONG_MAX)
			socket_id_seq = 1;
	}
#ifdef	_WINDOWS
	::LeaveCriticalSection(&cs);
#endif
#ifdef	_LINUX
	g_mutex_unlock(&sock_mutex);
#endif
}
void ADvSocket::Unlink(ADvSocket* socket) {
#ifdef	_WINDOWS
	::EnterCriticalSection(&cs);
#endif
#ifdef	_LINUX
	g_mutex_lock(&sock_mutex);
#endif
	socket->prev->next = socket->next;
	socket->next->prev = socket->prev;
#ifdef	_WINDOWS
	::LeaveCriticalSection(&cs);
#endif
#ifdef	_LINUX
	g_mutex_unlock(&sock_mutex);
#endif
}

const char* ADvSocket::GetErrorString() {
#ifdef	_WINDOWS
	int code = WSAGetLastError();
	switch (code) {
	case WSAECONNREFUSED:		return "WSAECONNREFUSED";
	case WSAEWOULDBLOCK:		return "WSAEWOULDBLOCK";
	case WSAEINTR:				return "WSAEINTR";
	case WSAEINPROGRESS:		return "WSAEINPROGRESS";
	case WSAENOTSOCK:			return "WSAENOTSOCK";
	case WSAEADDRINUSE: 		return "WSAEADDRINUSE";
	case WSAEADDRNOTAVAIL:		return "WSAEADDRNOTAVAIL";
	case WSAENETDOWN:			return "WSAENETDOWN";
	case WSAENETUNREACH:		return "WSAENETUNREACH";
	case WSAENETRESET:			return "WSAENETRESET";
	case WSAECONNABORTED:		return "WSAECONNABORTED";
	case WSAECONNRESET: 		return "WSAECONNRESET";
	case WSAENOBUFS:			return "WSAENOBUFS";
	case WSAENOTCONN:			return "WSAENOTCONN";
	case WSAESHUTDOWN:			return "WSAESHUTDOWN";
	case WSAETIMEDOUT:			return "WSAETIMEDOUT";
	case WSAEHOSTDOWN:			return "WSAEHOSTDOWN";
	case WSAEHOSTUNREACH:		return "WSAEHOSTUNREACH";
	case WSANOTINITIALISED: 	return "WSANOTINITIALISED";
	case WSAEMFILE: 			return "WSAEMFILE";
	case WSAEFAULT: 			return "WSAEFAULT";
	case WSAEINVAL: 			return "WSAEINVAL";
	case WSAENOPROTOOPT:		return "WSAENOPROTOOPT";
	default:	return "WS_UNKNOWNERROR";
	}
#endif
#ifdef	_LINUX
	// TODO_LINUX Socket Error
	return "error";
#endif
}

// ADvDataQueue
ADvDataQueue::ADvDataQueue() {
	root = new ADvDataQueueItem();
#ifdef	_WINDOWS
	::InitializeCriticalSection(&cs);
#endif
#ifdef	_LINUX
	g_mutex_init(&mutex);
#endif
}
ADvDataQueue::~ADvDataQueue() {
	ADvDataQueueItem *item;
	while ((item = Get()) != NULL) {
		Free(item);
	}
#ifdef	_WINDOWS
	::DeleteCriticalSection(&cs);
#endif
#ifdef	_LINUX
	g_mutex_clear(&mutex);
#endif
}

void ADvDataQueue::Put(ADvDataQueueItem *item) {
#ifdef	_WINDOWS
	::EnterCriticalSection(&cs);
#endif
#ifdef	_LINUX
	g_mutex_lock(&mutex);
#endif
	item->prev = root->prev; item->next = root;
	root->prev->next = item; root->prev = item;
#ifdef	_WINDOWS
	::LeaveCriticalSection(&cs);
#endif
#ifdef	_LINUX
	g_mutex_unlock(&mutex);
#endif
}
ADvDataQueueItem *ADvDataQueue::Get() {
#ifdef	_WINDOWS
	::EnterCriticalSection(&cs);
#endif
#ifdef	_LINUX
	g_mutex_lock(&mutex);
#endif
	ADvDataQueueItem *item = root->next;
	if (item == root) {
		item = NULL;
	} else {
		item->prev->next = item->next;
		item->next->prev = item->prev;
	}
#ifdef	_WINDOWS
	::LeaveCriticalSection(&cs);
#endif
#ifdef	_LINUX
	g_mutex_unlock(&mutex);
#endif
	return item;
}

ADvDataQueueItem::ADvDataQueueItem() {
	next = prev = this;
	bin = NULL;
	str = NULL;
}
ADvDataQueueItem::~ADvDataQueueItem() {
}
ADvDataQueueItem *ADvDataQueue::Alloc() {
	ADvDataQueueItem *item = new ADvDataQueueItem();
	return item;
}
ADvDataQueueItem *ADvDataQueue::Alloc(AGmBinary *bin, long index, long size) {
	ADvDataQueueItem *item = Alloc();
	item->bin = new AGmBinary(size);
	item->bin->Copy(0, bin, index, size);
	item->str = NULL;
	return item;
}
ADvDataQueueItem *ADvDataQueue::Alloc(AGmString *str) {
	ADvDataQueueItem *item = Alloc();
	item->bin = NULL;
	item->str = new AGmString(str);
	return item;
}
void ADvDataQueue::Free(ADvDataQueueItem *item) {
	delete item;
}

// ---------------------------------
// ----- begin thread function -----
// ---------------------------------
#ifdef	_WINDOWS
static DWORD acceptFunc(ADvSocket* socket) {
	HWND hWnd = (HWND)socket->hWnd;
	long msg = (long)socket->msg;
	long socket_id = socket->socket_id;
	int id = ADvSocket::Accept(socket);
	::CloseHandle(socket->accept_thread);
	socket->accept_thread = NULL;
	socket->recv_wait = 0;
	::PostMessage(hWnd, msg, (WPARAM)socket_id, (LPARAM)id);
#ifdef	_AL_OPENSSL
	ERR_remove_state(0);
#endif
	::ExitThread(0);
}
#endif
#ifdef	_LINUX
static gpointer acceptFunc(gpointer data) {
	ADvSocket* socket = (ADvSocket*)data;
	long hWnd = socket->hWnd;
	long msg = socket->msg;
	long socket_id = socket->socket_id;
	int id = ADvSocket::Accept(socket);
	socket->accept_thread = NULL;
	socket->recv_wait = 0;
	ADvPostMessage(hWnd, msg, socket_id, id);
#ifdef	_AL_OPENSSL
	ERR_remove_state(0);
#endif
	g_thread_exit(NULL);
	return NULL;
}
#endif

#ifdef	_WINDOWS
static DWORD recvFunc(ADvSocket* socket) {
	HWND hWnd = (HWND)socket->hWnd;
	long msg = (long)socket->msg;
	long socket_id = socket->socket_id;
	int rc = ADvSocket::Recv(socket,
		socket->bin, socket->index, socket->size, socket->mode, socket->size2);
	if (rc == SOCKET_ERROR) rc = -1;
	::CloseHandle(socket->recv_thread);
	socket->recv_thread = NULL;
	socket->recv_wait = 0;
	::PostMessage(hWnd, msg, (WPARAM)socket_id, (LPARAM)rc);
#ifdef	_AL_OPENSSL
	ERR_remove_state(0);
#endif
	::ExitThread(0);
	return 0;
}
#endif
#ifdef	_LINUX
static gpointer recvFunc(gpointer data) {
	ADvSocket* socket = (ADvSocket*)data;
	long hWnd = socket->hWnd;
	long msg = socket->msg;
	long socket_id = socket->socket_id;
	int rc = ADvSocket::Recv(socket,
		socket->bin, socket->index, socket->size, socket->mode, socket->size2);
	if (rc == SOCKET_ERROR) rc = -1;
	socket->recv_thread = NULL;
	socket->recv_wait = 0;
	ADvPostMessage(hWnd, msg, socket_id, rc);
#ifdef	_AL_OPENSSL
	ERR_remove_state(0);
#endif
	g_thread_exit(NULL);
	return NULL;
}
#endif

#ifdef	_WINDOWS
static DWORD recvFromFunc(ADvSocket* socket) {
	HWND hWnd = (HWND)socket->hWnd;
	long msg = (long)socket->msg;
	long socket_id = socket->socket_id;
	char *buff = (char*)socket->bin->Buffer() + socket->index;
	long size = socket->size;
	int rc = ADvSocket::RecvFrom(socket, buff, size);
	if (rc == SOCKET_ERROR) rc = -1;
	::CloseHandle(socket->recv_thread);
	socket->recv_thread = NULL;
	socket->recv_wait = 0;
	::PostMessage(hWnd, msg, (WPARAM)socket_id, (LPARAM)rc);
#ifdef	_AL_OPENSSL
	ERR_remove_state(0);
#endif
	::ExitThread(0);
	return 0;
}
#endif
#ifdef	_LINUX
static gpointer recvFromFunc(gpointer data) {
	ADvSocket* socket = (ADvSocket*)data;
	long hWnd = socket->hWnd;
	long msg = socket->msg;
	long socket_id = socket->socket_id;
	char *buff = (char*)socket->bin->Buffer() + socket->index;
	long size = socket->size;
	int rc = ADvSocket::RecvFrom(socket, buff, size);
	if (rc == SOCKET_ERROR) rc = -1;
	socket->recv_thread = NULL;
	socket->recv_wait = 0;
	ADvPostMessage(hWnd, msg, socket_id, rc);
#ifdef	_AL_OPENSSL
	ERR_remove_state(0);
#endif
	g_thread_exit(NULL);
	return NULL;
}
#endif

#ifdef	_WINDOWS
static DWORD sendFunc(ADvSocket* socket) {
	HWND hWnd = socket->hWnd2;
	long msg = (long)socket->msg2;
	long socket_id = socket->socket_id;
	AGmBinary *bin = socket->bin2;
	int rc = ADvSocket::Send(socket, (char*)bin->Buffer(), bin->Length());
	if (rc == SOCKET_ERROR) rc = -1;
	delete bin;
	::CloseHandle(socket->send_thread);
	socket->send_thread = NULL;
	socket->send_wait = 0;
	::PostMessage(hWnd, msg, (WPARAM)socket_id, (LPARAM)rc);
#ifdef	_AL_OPENSSL
	ERR_remove_state(0);
#endif
	::ExitThread(0);
	return 0;
}
#endif
#ifdef	_LINUX
static gpointer sendFunc(gpointer data) {
	ADvSocket* socket = (ADvSocket*)data;
	long hWnd = socket->hWnd2;
	long msg = socket->msg2;
	long socket_id = socket->socket_id;
	AGmBinary *bin = socket->bin2;
	int rc = ADvSocket::Send(socket, (char*)bin->Buffer(), bin->Length());
	if (rc == SOCKET_ERROR) rc = -1;
	delete bin;
	socket->send_thread = NULL;
	socket->send_wait = 0;
	ADvPostMessage(hWnd, msg, socket_id, rc);
#ifdef	_AL_OPENSSL
	ERR_remove_state(0);
#endif
	g_thread_exit(NULL);
	return NULL;
}
#endif

#ifdef	_WINDOWS
static DWORD queueAcceptFunc(ADvSocket* socket) {
	for (;;) {
		int id = ADvSocket::Accept(socket);
		if (id < 0) break;
		ADvDataQueueItem *item = socket->accept_queue->Alloc();
		item->socket_id = id;
		socket->accept_queue->Put(item);
		::PostMessage(socket->hWnd, (UINT)socket->msg, (WPARAM)socket->socket_id, 0);
	}
	socket->accept_thread = NULL;
#ifdef	_AL_OPENSSL
	ERR_remove_state(0);
#endif
	::ExitThread((DWORD)-2);
	return 0;
}
#endif
#ifdef	_LINUX
static gpointer queueAcceptFunc(gpointer data) {
	ADvSocket* socket = (ADvSocket*)data;
	for (;;) {
		int id = ADvSocket::Accept(socket);
		if (id < 0) break;
		ADvDataQueueItem *item = socket->accept_queue->Alloc();
		item->socket_id = id;
		socket->accept_queue->Put(item);
		ADvPostMessage(socket->hWnd, socket->msg, socket->socket_id, 0);
	}
	socket->accept_thread = NULL;
#ifdef	_AL_OPENSSL
	ERR_remove_state(0);
#endif
	g_thread_exit(NULL);
	return NULL;
}
#endif

#ifdef	_WINDOWS
static DWORD queueRecvFunc(ADvSocket* socket) {
	for (;;) {
		int rc; char c; char *buff; int count;
		rc = ADvSocket::Recv(socket, &c, 1);
		if (rc == SOCKET_ERROR) break;
		ADvDataQueueItem *item;
		if (c == 'b') {
			// binary
			count = 0;
			while (1) {
				rc = ADvSocket::Recv(socket, &c, 1);
				if (rc == SOCKET_ERROR) break;
				if (c == '\n') break;
				count *= 10; count += c - '0';
			}
			if (c != '\n') break;
			AGmBinary *bin = new AGmBinary(count);
			buff = (char*)bin->Buffer();
			while (count > 0) {
				rc = ADvSocket::Recv(socket, buff, count);
				if (rc == SOCKET_ERROR) break;
				count -= rc; buff += rc;
				if (count == 0) break;
			}
			if (count > 0) break;
			item = socket->recv_queue->Alloc();
			item->bin = bin;
		} else if (c == 't') {
			// text
			count = 0;
			while (1) {
				rc = ADvSocket::Recv(socket, &c, 1);
				if (rc == SOCKET_ERROR) break;
				if (c == '\n') break;
				count *= 10; count += c - '0';
			}
			if (c != '\n') break;
			AGmString *str = new AGmString(count + 1);
			char buf[1024];
			while (count > 0) {
				rc = ADvSocket::Recv(socket, buf, count);
				if (rc == SOCKET_ERROR) break;
				buf[rc] = 0;
				str->Append(buf);
				count -= rc;
				if (count == 0) break;
			}
			if (count > 0) break;
			item = socket->recv_queue->Alloc();
			item->str = str;
		} else {
			break;
		}
		socket->recv_queue->Put(item);
		::PostMessage(socket->hWnd, (UINT)socket->msg, (WPARAM)socket->socket_id, 0);
	}
	socket->error = "queue recv error";
	socket->recv_thread = NULL;
#ifdef	_AL_OPENSSL
	ERR_remove_state(0);
#endif
	::ExitThread(0);
	return 0;
}
#endif
#ifdef	_LINUX
static gpointer queueRecvFunc(gpointer data) {
	ADvSocket* socket = (ADvSocket*)data;
	for (;;) {
		int rc; char c; char *buff; int count;
		rc = ADvSocket::Recv(socket, &c, 1);
		if (rc == SOCKET_ERROR) break;
		ADvDataQueueItem *item;
		if (c == 'b') {
			// binary
			count = 0;
			while (1) {
				rc = ADvSocket::Recv(socket, &c, 1);
				if (rc == SOCKET_ERROR) break;
				if (c == '\n') break;
				count *= 10; count += c - '0';
			}
			if (c != '\n') break;
			AGmBinary *bin = new AGmBinary(count);
			buff = (char*)bin->Buffer();
			while (count > 0) {
				rc = ADvSocket::Recv(socket, buff, count);
				if (rc == SOCKET_ERROR) break;
				count -= rc; buff += rc;
				if (count == 0) break;
			}
			if (count > 0) break;
			item = socket->recv_queue->Alloc();
			item->bin = bin;
		} else if (c == 't') {
			// text
			count = 0;
			while (1) {
				rc = ADvSocket::Recv(socket, &c, 1);
				if (rc == SOCKET_ERROR) break;
				if (c == '\n') break;
				count *= 10; count += c - '0';
			}
			if (c != '\n') break;
			AGmString *str = new AGmString(count + 1);
			char buf[1024];
			while (count > 0) {
				rc = ADvSocket::Recv(socket, buf, count);
				if (rc == SOCKET_ERROR) break;
				buf[rc] = 0;
				str->Append(buf);
				count -= rc;
				if (count == 0) break;
			}
			if (count > 0) break;
			item = socket->recv_queue->Alloc();
			item->str = str;
		} else {
			break;
		}
		socket->recv_queue->Put(item);
		ADvPostMessage(socket->hWnd, socket->msg, socket->socket_id, 0);
	}
	socket->error = "queue recv error";
	socket->recv_thread = NULL;
#ifdef	_AL_OPENSSL
	ERR_remove_state(0);
#endif
	g_thread_exit(NULL);
	return NULL;
}
#endif

#ifdef	_WINDOWS
static DWORD queueSendFunc(ADvSocket* socket) {
	for (;;) {
		int rc; int count; char *buff;
		char header[64];
		ADvDataQueueItem *item = socket->send_queue->Get();
		if (item) {
			if (item->bin) {
				art_strcpy(header, 64, "b");
				count = item->bin->Length();
				buff = (char*)item->bin->Buffer();
			} else if (item->str) {
				art_strcpy(header, 64, "t");
				count = item->str->Length();
				buff = (char*)item->str->Str();
			} else {
				break;
			}
			art_sprintf_1(&header[1], 63, "%d", count);
			art_strcat(header, 64, "\n");
			rc = ADvSocket::Send(socket, header, (long)strlen(header));
			if (rc == SOCKET_ERROR) break;
			rc = ADvSocket::Send(socket, buff, count);
			if (rc == SOCKET_ERROR) break;
			if (item->bin) delete item->bin;
			if (item->str) delete item->str;
			socket->send_queue->Free(item);
			continue;
		}
		::WaitForSingleObject(socket->wait_event, INFINITE);
		::ResetEvent(socket->wait_event);
	}
	socket->error = "queue send error";
	socket->send_thread = NULL;
#ifdef	_AL_OPENSSL
	ERR_remove_state(0);
#endif
	::ExitThread(0);
	return 0;
}
#endif
#ifdef	_LINUX
static gpointer queueSendFunc(gpointer data) {
	ADvSocket* socket = (ADvSocket*)data;
	for (;;) {
		int rc; int count; char *buff;
		char header[64];
		ADvDataQueueItem *item = socket->send_queue->Get();
		if (item) {
			if (item->bin) {
				art_strcpy(header, 64, "b");
				count = item->bin->Length();
				buff = (char*)item->bin->Buffer();
			} else if (item->str) {
				art_strcpy(header, 64, "t");
				count = item->str->Length();
				buff = (char*)item->str->Str();
			} else {
				break;
			}
			art_sprintf_1(&header[1], 63, "%d", count);
			art_strcat(header, 64, "\n");
			rc = ADvSocket::Send(socket, header, strlen(header));
			if (rc == SOCKET_ERROR) break;
			rc = ADvSocket::Send(socket, buff, count);
			if (rc == SOCKET_ERROR) break;
			if (item->bin) delete item->bin;
			if (item->str) delete item->str;
			socket->send_queue->Free(item);
			continue;
		}
		g_mutex_lock(&sock_mutex);
		while (socket->wait_cond_flag == 0) {
			g_cond_wait(&socket->wait_cond, &sock_mutex);
		}
		socket->wait_cond_flag = 0;
		g_mutex_unlock(&sock_mutex);
	}
	socket->error = "queue send error";
	socket->send_thread = NULL;
#ifdef	_AL_OPENSSL
	ERR_remove_state(0);
#endif
	g_thread_exit(NULL);
	return NULL;
}
#endif

#ifdef	_WINDOWS
static DWORD connectFunc(ADvSocket* socket) {
	int rc = connect(socket->s, (SOCKADDR*)socket->addr_port, sizeof(*socket->addr_port));
	if (rc == SOCKET_ERROR) {
		socket->error = ADvSocket::GetErrorString();
	} else {
		socket->connected = 1;
	}
	::SetEvent(socket->wait_event);
#ifdef	_AL_OPENSSL
	ERR_remove_state(0);
#endif
	::ExitThread(0);
	return 0;
}
#endif
#ifdef	_LINUX
static gpointer connectFunc(gpointer data) {
	ADvSocket* socket = (ADvSocket*)data;
	int rc = connect(socket->s, (SOCKADDR*)socket->addr_port, sizeof(*socket->addr_port));
	if (rc == SOCKET_ERROR) {
		socket->error = ADvSocket::GetErrorString();
	} else {
		socket->connected = 1;
	}
	g_mutex_lock(&sock_mutex);
	socket->wait_cond_flag = 1;
	g_cond_signal(&socket->wait_cond);
	g_mutex_unlock(&sock_mutex);
#ifdef	_AL_OPENSSL
	ERR_remove_state(0);
#endif
	g_thread_exit(NULL);
	return NULL;
}
#endif
// ---------------------------------
// ------ end thread function ------
// ---------------------------------

#ifdef	_AL_OPENSSL
int verify_callback(int ok, X509_STORE_CTX *ctx) {
	X509 *err_cert = X509_STORE_CTX_get_current_cert(ctx);
	int err= X509_STORE_CTX_get_error(ctx);
	int depth=	X509_STORE_CTX_get_error_depth(ctx);

	char buf[256];
	X509_NAME_oneline(X509_get_subject_name(err_cert), buf ,256);
	TRACE2(AL_STR("depth=%d %s\n"), depth, buf);
	if (!ok && !err) {
		if (verify_depth >= depth) {
			ok = 1;
		} else {
			TRACE2(AL_STR("verify error:num=%d:%s\n"),
				err, X509_verify_cert_error_string(err));
		}
	}
	switch (ctx->error) {
	case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT:
		TRACE0(AL_STR("X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT\n"));
		break;
	case X509_V_ERR_CERT_NOT_YET_VALID:
		TRACE0(AL_STR("X509_V_ERR_CERT_NOT_YET_VALID\n"));
		break;
	case X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD:
		TRACE0(AL_STR("X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD\n"));
		break;
	case X509_V_ERR_CERT_HAS_EXPIRED:
		TRACE0(AL_STR("X509_V_ERR_CERT_HAS_EXPIRED\n"));
		break;
	case X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD:
		TRACE0(AL_STR("X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD\n"));
		break;
	}
	return ok;
}
#endif

// socket
int AIpProcess::EvalSocket() {
	AGmNode *cmd, *arg1, *arg2, *arg3;
	GetArg(cmd, arg1, arg2, arg3);
	if (!IsString(cmd)) {
		Unref(cmd, arg1, arg2, arg3);
		return interp_error("Socket()");
	}
	int err = 0;
	const char *s = cmd->a.s.v->Str();
	Unref(val); val = NULL;
	if (strcmp(s, "is_available") == 0) {
		// ret_val: abailable
		if (ADvSocket::available) {
			val = Ref(Integer(1));
		}
	} else if (strcmp(s, "get_last_error") == 0) {
		if (arg1 == NULL) {
			// case 1
			// ret_val: global_error_string
			// arg1: null
			if (ADvSocket::global_error) {
				val = Ref(String(ADvSocket::global_error));
			}
		} else if (IsInteger(arg1)) {
			// case 2
			// ret_val: error_string
			// arg1: socket_id
			ADvSocket *socket = ADvSocket::GetSocket((int)arg1->a.i.v);
			if (socket) {
				if (socket->error) {
					val = Ref(String(socket->error));
				}
			} else {
				val = Ref(String("not socket descriptor"));
			}
		} else {
			err = 1;
		}
	} else if (strcmp(s, "gethostname") == 0) {
		char str[512];
		if (gethostname(str, 512) == 0) {
			val = Ref(String(str));
		}
	} else if (strcmp(s, "gethostbyname") == 0) {
		// ret_val: ip_addr
		// arg1: hostname
		if (IsString(arg1)) {
			long ip_addr = get_IP_from_X_X_X_X(arg1->a.s.v->Str());
			if (ip_addr) {
				val = Ref(Integer((unsigned long)ip_addr));
			} else {
				struct hostent *pHost = gethostbyname(arg1->a.s.v->Str());
				if (pHost) {
					memcpy((char*)&ip_addr, pHost->h_addr, pHost->h_length);
					val = Ref(Integer((unsigned long)ntohl(ip_addr)));
				}
			}
		}
	} else if (strcmp(s, "bcast_address") == 0) {
		// ret_val: ip_addr
		long ip_addr = INADDR_BROADCAST;
		val = Ref(Integer(ntohl(ip_addr)));

	} else if (strcmp(s, "address_string") == 0) {
		if (IsInteger(arg1)) {
			AGmString *addr_s = get_X_X_X_X_from_IP((long)arg1->a.i.v);
			val = Ref(String(addr_s));
		}

	} else if (strcmp(s, "getsockname") == 0) {
		// ret_val: (ip_addr, port)
		// arg1: socket_id
		if (IsInteger(arg1)) {
			ADvSocket *socket = ADvSocket::GetSocket((int)arg1->a.i.v);
			if (socket) {
				SOCKADDR_IN addr;
				socklen_t addrsize = sizeof(addr);
				int rc = getsockname(socket->s, (SOCKADDR*)&addr, &addrsize);
				if (rc == SOCKET_ERROR) {
					socket->error = ADvSocket::GetErrorString();
				} else {
					val = Ref(List(
						Integer(ntohl(addr.sin_addr.s_addr)),
						Integer(ntohs(addr.sin_port))));
				}
			}
		} else {
			err = 1;
		}
	} else if (strcmp(s, "getpeername") == 0) {
		// ret_val: (ip_addr, port)
		// arg1: socket_id
		if (IsInteger(arg1)) {
			ADvSocket *socket = ADvSocket::GetSocket((int)arg1->a.i.v);
			if (socket) {
				SOCKADDR_IN addr;
				socklen_t addrsize = sizeof(addr);
				int rc = getpeername(socket->s, (SOCKADDR*)&addr, &addrsize);
				if (rc == SOCKET_ERROR) {
					socket->error = ADvSocket::GetErrorString();
				} else {
					val = Ref(List(
						Integer(ntohl(addr.sin_addr.s_addr)),
						Integer(ntohs(addr.sin_port))));
				}
			}
		} else {
			err = 1;
		}

	} else if (strcmp(s, "socket") == 0) {
		// ret_val: socket_id
		long socket_id = ADvSocket::CreateStreamSocket();
		if (socket_id > 0) {
#ifdef	_AL_OPENSSL
			ADvSocket *socket = ADvSocket::GetSocket(socket_id);
			if (IsString(arg1) && arg1->a.s.v->Compare("SSL") == 0) {
				for (;;) {
					AGmNode *tail;
					char *cert = NULL, *key = NULL, *ca_cert = NULL, *dhparam = NULL;
					char *cipher = NULL;
#if	_WINDOWS && _64BIT
					long long auth;
#else
					long auth;
#endif
					if (Param(arg2, cert, tail)
							|| Param(tail, key, tail)
							|| Param(tail, ca_cert, tail)
							|| Param(tail, auth, tail)) {
						ADvSocket::Close(socket_id);
						socket_id = -1;
						break;
					}
					if (IsString(arg3)) cipher = (char*)arg3->a.s.v->Str();
					socket->auth = (long)auth;
					int server_socket = 1;
					if (Param(tail, dhparam, tail)) {
						server_socket = 0;
						dhparam = NULL;
					}
					SSL_METHOD *meth = NULL;
					if (server_socket) {
						meth=(SSL_METHOD*)SSLv23_server_method();
						// meth=SSLv2_server_method();	 use only ssl2
						// meth=SSLv3_server_method();	 use only ssl3
						// meth=TLSv1_server_method();	 use only tls1
					} else {
						meth=(SSL_METHOD*)SSLv23_client_method();
						// meth=SSLv2_client_method();	 use only ssl2
						// meth=SSLv3_client_method();	 use only ssl3
						// meth=TLSv1_client_method();	 use only tls1
					}
					socket->ctx = SSL_CTX_new(meth);
					int off = 0;
						// off |= SSL_OP_NO_SSLv2;	not use ssl2
						// off |= SSL_OP_NO_SSLv3;	not use ssl3
						// off |= SSL_OP_NO_TLSv1;	not use tls1
					SSL_CTX_set_options(socket->ctx, off);
					if (socket->auth) {
						int verify;
						if (server_socket) {
							verify = SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT;
						} else {
							verify = SSL_VERIFY_PEER;
						}
						SSL_CTX_set_verify(socket->ctx, verify, verify_callback);
					}
					if (cert && !SSL_CTX_use_certificate_file(
							socket->ctx, cert, SSL_FILETYPE_PEM)) {
						ADvSocket::Close(socket_id);
						socket_id = -1;
						break;
					}
					if (key && !SSL_CTX_use_PrivateKey_file(
							socket->ctx, key, SSL_FILETYPE_PEM)) {
						ADvSocket::Close(socket_id);
						socket_id = -1;
						break;
					}
					if (ca_cert && !SSL_CTX_load_verify_locations(
							socket->ctx, ca_cert, 0)) {
						ADvSocket::Close(socket_id);
						socket_id = -1;
						break;
					}
					if (dhparam) {
						BIO *bio = BIO_new_file(dhparam, "r");
						if (bio == NULL) {
							ADvSocket::Close(socket_id);
							socket_id = -1;
							break;
						}
						DH *ret = PEM_read_bio_DHparams(bio, NULL, NULL, NULL);
						BIO_free(bio);
						if (SSL_CTX_set_tmp_dh(socket->ctx, ret) < 0) {
							ADvSocket::Close(socket_id);
							socket_id = -1;
							break;
						}
					}
					if (cipher) {
						if (!SSL_CTX_set_cipher_list(socket->ctx, cipher)) {
							ADvSocket::Close(socket_id);
							socket_id = -1;
							break;
						}
					}
					if (server_socket && cert && key) {
						RSA *rsa = RSA_generate_key(512, RSA_F4, NULL, NULL);
						if (!SSL_CTX_set_tmp_rsa(socket->ctx, rsa)) {
							ADvSocket::Close(socket_id);
							socket_id = -1;
							RSA_free(rsa);
							break;
						}
						RSA_free(rsa);
						int session_id_context;
						SSL_CTX_set_session_id_context(socket->ctx,
							(unsigned char*)&session_id_context,
							sizeof(session_id_context));
					}
					break;
				}
			}
			if (socket_id > 0) {
				val = Ref(Integer(socket_id));
			}
#else
			val = Ref(Integer(socket_id));
#endif
		}
	} else if (strcmp(s, "dgram_socket") == 0) {
		// ret_val: socket_id
		long socket_id = ADvSocket::CreateDgramSocket();
		if (socket_id > 0) {
			val = Ref(Integer(socket_id));
		}
	} else if (strcmp(s, "close") == 0) {
		// ret_val: null, if success
		// arg1: socket_id
		if (IsInteger(arg1)) {
			if (arg1->a.i.v > 0 && ADvSocket::Close((int)arg1->a.i.v)) {
			} else {
				val = Ref(String("not socket descriptor"));
			}
		} else {
			err = 1;
		}

	} else if (strcmp(s, "bind") == 0) {
		// ret_val: null, if success
		// arg1: socket_id
		// arg2: (ip_addr, port)
		ADvSocket *socket;
#if	_WINDOWS && _64BIT
		long long ip_addr, port;
#else
		long ip_addr, port;
#endif
		AGmNode *tail;
		val = Ref(Integer(1));
		if (IsInteger(arg1)
				&& (socket = ADvSocket::GetSocket((int)arg1->a.i.v)) > 0
				&& Param(arg2, ip_addr, port, tail) == 0) {
			SOCKADDR_IN addr;
			addr.sin_family = AF_INET;	
			addr.sin_addr.s_addr = htonl((unsigned long)ip_addr);	
			addr.sin_port = htons((unsigned short)port);
			int rc = bind(socket->s, (SOCKADDR*)&addr, sizeof(addr));
			if (rc == SOCKET_ERROR) {
				socket->error = ADvSocket::GetErrorString();
			} else {
				Unref(val); val = NULL;
			}

		}
	} else if (strcmp(s, "listen") == 0) {
		// ret_val: null, if success
		// arg1: socket_id
		ADvSocket *socket;
		val = Ref(Integer(1));
		if (IsInteger(arg1)
				&& (socket = ADvSocket::GetSocket((int)arg1->a.i.v)) > 0) {
			int rc = listen(socket->s, SOMAXCONN);
			if (rc == SOCKET_ERROR) {
				socket->error = ADvSocket::GetErrorString();
			} else {
				Unref(val); val = NULL;
			}
		}
	} else if (strcmp(s, "accept") == 0) {
		ADvSocket *socket;
#if	_WINDOWS && _64BIT
		long long hwnd, msg;
#else
		long hwnd, msg;
#endif
		AGmNode *tail;
		if (IsInteger(arg1)
				&& (socket = ADvSocket::GetSocket((int)arg1->a.i.v)) > 0) {
			if (arg2 == NULL) {
				// case 1
				// ret_val: socket_id, if success
				// arg1: server_socket_id
				// arg2: null
				int socket_id = ADvSocket::Accept((int)arg1->a.i.v);
				if (socket_id > 0) {
					val = Ref(Integer(socket_id));
				}
			} else if (Param(arg2, hwnd, msg, tail) == 0) {
				// case 2
				// ret_val: null, if success
				// arg1: server_socket_id
				// arg2: (hwnd, msg)
				//		 wparam = server_socket_id
				//		 lparam = socket_id
				ADvSocket *socket = ADvSocket::GetSocket((int)arg1->a.i.v);
				if (socket == NULL) {
					val = Ref(Integer(1));
				} else {
					if (socket->recv_wait == 0) {
						socket->recv_wait = 1;
						socket->msg = msg;
#ifdef	_WINDOWS
						socket->hWnd = (HWND)hwnd;
						DWORD threadID;
						SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
						socket->accept_thread = ::CreateThread(&sa, 0,
							(LPTHREAD_START_ROUTINE)acceptFunc, socket,
							0, &threadID);
#endif
#ifdef	_LINUX
						socket->hWnd = hwnd;
						socket->accept_thread = g_thread_new(AL_STR("socket-accept"), acceptFunc, socket);
#endif
					} else {
						val = Ref(Integer(1));
					}
				}
			}
		}
	} else if (strcmp(s, "connect") == 0) {
		// ret_val: null, if success
		// arg1: socket_id
		// arg2: (ip_addr, port)
		ADvSocket *socket;
#if	_WINDOWS && _64BIT
		long long ip_addr, port;
#else
		long ip_addr, port;
#endif
		AGmNode *tail;
		val = Ref(Integer(1));
		if (IsInteger(arg1)
				&& (socket = ADvSocket::GetSocket((int)arg1->a.i.v)) > 0
				&& Param(arg2, ip_addr, port, tail) == 0) {
			SOCKADDR_IN addr;
			addr.sin_family = AF_INET;	
			addr.sin_addr.s_addr = htonl((unsigned long)ip_addr);	
			addr.sin_port = htons((unsigned short)port);
			int timeout = 0;
			if (IsInteger(arg3)) timeout = (int)arg3->a.i.v;
			if (timeout <= 0) {
				int rc = connect(socket->s, (SOCKADDR*)&addr, sizeof(addr));
				if (rc == SOCKET_ERROR) {
					socket->error = ADvSocket::GetErrorString();
				} else {
					socket->connected = 1;
					Unref(val); val = NULL;
				}
			} else {
				int err = 0;
#ifdef	_WINDOWS
				socket->addr_port = &addr;
				DWORD threadID;
				socket->wait_event = ::CreateEvent(NULL, TRUE, FALSE, NULL);
				SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
				socket->recv_thread = ::CreateThread(&sa, 0,
					(LPTHREAD_START_ROUTINE)connectFunc, socket,
					0, &threadID);
				::WaitForSingleObject(socket->wait_event, timeout);
				if (socket->connected == 0) {
					if (socket->recv_thread) {
						::TerminateThread(socket->recv_thread, (DWORD)-1);
					}
					if (socket->error == NULL) {
						socket->error = "timeout";
					}
					err = 1;
				} else {
					Unref(val); val = NULL;
				}
				::CloseHandle(socket->recv_thread);
				socket->recv_thread = NULL;
				::CloseHandle(socket->wait_event);
				socket->wait_event = NULL;
#endif
#ifdef	_LINUX
				socket->addr_port = &addr;
				socket->wait_cond_flag = 0;
				g_cond_init(&socket->wait_cond);
				socket->recv_thread = g_thread_new(AL_STR("socket-connet"), connectFunc, socket);
				g_mutex_lock(&sock_mutex);
				while (socket->wait_cond_flag == 0) {
					g_cond_wait(&socket->wait_cond, &sock_mutex);
				}
				socket->wait_cond_flag = 0;
				if (socket->connected == 0) {
					if (socket->recv_thread) {
					}
					if (socket->error == NULL) {
						socket->error = "timeout";
					}
					err = 1;
				} else {
					Unref(val); val = NULL;
				}
				socket->recv_thread = NULL;
				g_cond_clear(&socket->wait_cond);
#endif
			}
		}

	} else if (strcmp(s, "SSL_connect") == 0) {
		// ret_val: null, if success
		// arg1: socket_id
		ADvSocket *socket;
		val = Ref(Integer(1));
		if (IsInteger(arg1)
				&& (socket = ADvSocket::GetSocket((int)arg1->a.i.v)) > 0
				&& socket->connected) {
#ifdef	_AL_OPENSSL
			if (socket->ctx && socket->ssl == NULL) {
				socket->ssl = SSL_new(socket->ctx);
				BIO* sbio = BIO_new_socket((int)socket->s, BIO_NOCLOSE);
				SSL_set_bio(socket->ssl, sbio, sbio);
				if (!SSL_connect(socket->ssl)) {
					socket->error = "SSL_connect error";
				} else {
					Unref(val); val = NULL;
				}
						
			} else {
				Unref(val); val = NULL;
			}
#else
			Unref(val); val = NULL;
#endif
		}

	} else if (strcmp(s, "send") == 0) {
		// ret_val: null, if success
		// arg1: socket_id
		// arg2: (buff, index, size) or string
		ADvSocket *socket;
		AGmBinary *bin;
#if	_WINDOWS && _64BIT
		long long index, size = 0;
		long long hwnd, msg;
#else
		long index, size = 0;
		long hwnd, msg;
#endif
		AGmNode *tail;
		char *buff = NULL;
		val = Ref(Integer(1));
		if (IsInteger(arg1)
				&& (socket = ADvSocket::GetSocket((int)arg1->a.i.v)) > 0) {
			if (Param(arg2, bin, tail) == 0
					&& Param(tail, index, size, tail) == 0
					&& index >= 0 && size >= 0
					&& index + size <= bin->Length()) {
				buff = (char*)bin->Buffer();
				buff += index;
			} else if (IsString(arg2)) {
				buff = (char*)arg2->a.s.v->Str();
				size = arg2->a.s.v->Length();
			}
			if (buff) {
				if (arg3 == NULL) {
					int rc = ADvSocket::Send(socket, buff, (int)size);
					if (rc == SOCKET_ERROR) {
					} else {
						Unref(val); val = NULL;
					}
				} else if (Param(arg3, hwnd, msg, tail) == 0) {
					if (socket->send_wait == 0) {
						socket->send_wait = 1;
						bin = new AGmBinary((long)size);
						unsigned char *b = bin->Buffer();
						memcpy(b, buff, size);
						socket->msg2 = msg;
						socket->bin2 = bin;
#ifdef	_WINDOWS
						socket->hWnd2 = (HWND)hwnd;
						DWORD threadID;
						SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
						socket->send_thread = ::CreateThread(&sa, 0,
							(LPTHREAD_START_ROUTINE)sendFunc, socket,
							0, &threadID);
#endif
#ifdef	_LINUX
						socket->hWnd2 = hwnd;
						socket->send_thread = g_thread_new(AL_STR("socket-send"), sendFunc, socket);
#endif
						Unref(val); val = NULL;
					}
				}
			}
		}
	} else if (strcmp(s, "recv") == 0) {
		AGmBinary *bin;
#if	_WINDOWS && _64BIT
		long long index, size;
		long long hwnd, msg, mode = 0, size2 = 0x7fffffff;
#else
		long index, size;
		long hwnd, msg, mode = 0, size2 = 0x7fffffff;
#endif
		AGmNode *tail;
		if (IsInteger(arg1)
				&& Param(arg2, bin, tail) == 0
				&& Param(tail, index, size, tail) == 0
				&& index >= 0
				&& size >= 0 && size - index <= bin->Length()) {
			if (arg3 == NULL) {
				// case 1
				// ret_val: recv_size, if success
				// arg1: socket_id
				// arg2: (buff, index, size)
				// arg3: null
				int rc = ADvSocket::Recv((long)arg1->a.i.v, bin, (long)index, (long)size);
				if (rc == SOCKET_ERROR) {
				} else {
					val = Ref(Integer((long)rc));
				}
			} else if (Param(arg3, hwnd, msg, tail) == 0
						&& (tail == NULL
							|| (Param(tail, mode, tail) == 0
							 && (mode != 3 || Param(tail, size2, tail) == 0)
							)
						)
					) {
				// case 2
				// ret_val: null, if success
				// arg1: socket_id
				// arg2: (buff, index, size)
				// arg3: (hwnd, msg)
				//		 wparam = socket_id
				//		 lparam = recv_size (>0, if success)
				ADvSocket *socket = ADvSocket::GetSocket((int)arg1->a.i.v);
				if (socket == NULL) {
					val = Ref(Integer(1));
				} else {
					if (socket->recv_wait == 0) {
						socket->recv_wait = 1;
						socket->msg = msg;
						socket->bin = bin;
						socket->index = (long)index;
						socket->size = (long)size;
						socket->mode = (long)mode;
						socket->size2 = (long)size2;
#ifdef	_WINDOWS
						socket->hWnd = (HWND)hwnd;
						DWORD threadID;
						SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
						socket->recv_thread = ::CreateThread(&sa, 0,
							(LPTHREAD_START_ROUTINE)recvFunc, socket,
							0, &threadID);
#endif
#ifdef	_LINUX
						socket->hWnd = hwnd;
						socket->recv_thread = g_thread_new(AL_STR("socket_recv"), recvFunc, socket);
#endif
					} else {
						val = Ref(Integer(1));
					}
				}
			} else {
				val = Ref(Integer(1));
			}
		} else {
			val = Ref(Integer(1));
		}

	} else if (strcmp(s, "setsockopt") == 0) {
		// ret_val: null, if success
		// arg1: socket_id
		// arg2: opt
		// arg3: val
		ADvSocket *socket;
		val = Ref(Integer(1));
		if (IsInteger(arg1)
				&& (socket = ADvSocket::GetSocket((int)arg1->a.i.v)) > 0
				&& IsString(arg2) && IsInteger(arg3)) {
			const char *s = arg2->a.s.v->Str();
			int optname = -1;
			if (strcmp(s, "SO_BROADCAST") == 0) {
				optname = SO_BROADCAST;
			}
			if (optname != -1) {
				int value = (int)arg3->a.i.v;
				int rc = setsockopt(socket->s, SOL_SOCKET, optname,
					(char*)&value, sizeof(value));
				if (rc == SOCKET_ERROR) {
					socket->error = ADvSocket::GetErrorString();
				} else {
					Unref(val); val = NULL;
				}
			}
		}

	} else if (strcmp(s, "sendto") == 0) {
		// ret_val: null, if success
		// arg1: socket_id
		// arg2: (ip_addr, port)
		// arg3: (buff, index, size) or string
		ADvSocket *socket;
#if	_WINDOWS && _64BIT
		long long ip_addr, port;
		long long index, size = 0;
#else
		long ip_addr, port;
		long index, size = 0;
#endif
		AGmNode *tail;
		AGmBinary *bin;
		char *buff = NULL;
		val = Ref(Integer(1));
		if (IsInteger(arg1)
				&& (socket = ADvSocket::GetSocket((int)arg1->a.i.v)) > 0
				&& Param(arg2, ip_addr, port, tail) == 0) {
			SOCKADDR_IN addr;
			addr.sin_family = AF_INET;	
			addr.sin_addr.s_addr = htonl((unsigned long)ip_addr);	
			addr.sin_port = htons((unsigned short)port);
			if (Param(arg3, bin, tail) == 0
					&& Param(tail, index, size, tail) == 0
					&& index >= 0 && size >= 0
					&& index + size <= bin->Length()) {
				buff = (char*)bin->Buffer();
				buff += index;
			} else if (IsString(arg3)) {
				buff = (char*)arg3->a.s.v->Str();
				size = arg3->a.s.v->Length();
			}
			if (buff) {
				while (1) {
					int rc = sendto(socket->s, buff, (int)size, 0,
						(SOCKADDR*)&addr, sizeof(addr));
					if (rc == SOCKET_ERROR) {
						socket->error = ADvSocket::GetErrorString();
						break;
					}
					size -= rc;
					if (size == 0) {
						Unref(val); val = NULL;
						break;
					}
					buff += rc;
				}
			} else {
				Unref(val); val = NULL;
			}
		}
	} else if (strcmp(s, "recvfrom") == 0) {
		AGmBinary *bin;
#if	_WINDOWS && _64BIT
		long long index, size;
		long long hwnd, msg;
#else
		long index, size;
		long hwnd, msg;
#endif
		AGmNode *tail;
		if (IsInteger(arg1)
				&& Param(arg2, bin, tail) == 0
				&& Param(tail, index, size, tail) == 0
				&& index >= 0
				&& ( (size >= 0 && size - index <= bin->Length())
				  || (size < 0 && -size - index <= bin->Length())) ) {
			if (arg3 == NULL) {
				// case 1
				// ret_val: recv_size, if success
				// arg1: socket_id
				// arg2: (buff, index, size)
				// arg3: null
				char *buff = (char*)bin->Buffer() + index;
				int rc = ADvSocket::RecvFrom((int)arg1->a.i.v, buff, (long)size);
				if (rc <= 0) {
				} else {
					val = Ref(Integer((long)rc));
				}
			} else if (Param(arg3, hwnd, msg, tail) == 0) {
				// case 2
				// ret_val: null, if success
				// arg1: socket_id
				// arg2: (buff, index, size)
				// arg3: (hwnd, msg)
				//		 wparam = socket_id
				//		 lparam = recv_size (>0, if success)
				ADvSocket *socket = ADvSocket::GetSocket((int)arg1->a.i.v);
				if (socket == NULL) {
					val = Ref(Integer(1));
				} else {
					if (socket->recv_wait == 0) {
						socket->recv_wait = 1;
						socket->msg = msg;
						socket->bin = bin;
						socket->index = (long)index;
						socket->size = (long)size;
#ifdef	_WINDOWS
						socket->hWnd = (HWND)hwnd;
						DWORD threadID;
						SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
						socket->recv_thread = ::CreateThread(&sa, 0,
							(LPTHREAD_START_ROUTINE)recvFromFunc, socket,
							0, &threadID);
#endif
#ifdef	_LINUX
						socket->hWnd = hwnd;
						socket->recv_thread = g_thread_new(AL_STR("socket-recvfrom"), recvFromFunc, socket);
#endif
					} else {
						val = Ref(Integer(1));
					}
				}
			}
		}

	} else if (strcmp(s, "verify_depth") == 0) {
#ifdef	_AL_OPENSSL
		if (arg1 == NULL) {
			val = Ref(Integer((long)verify_depth));
		} else if (IsInteger(arg1)) {
			verify_depth = (int)arg1->a.i.v;
		}
#endif

	} else if (strcmp(s, "cert_info") == 0) {
#ifdef	_AL_OPENSSL
		if (IsInteger(arg1)) {
			ADvSocket *socket = ADvSocket::GetSocket((int)arg1->a.i.v);
			if (socket && socket->ssl) {
				X509 *peer = SSL_get_peer_certificate(socket->ssl);
				if (peer) {
					char commonName[256];
					char countryName[256];
					char localityName[256];
					char stateOrProvinceName[256];
					char organizationName[256];
					char organizationalUnitName[256];
					AGmNode *notBefore;
					AGmNode *notAfter;

					commonName[0] = 0;
					countryName[0] = 0;
					localityName[0] = 0;
					stateOrProvinceName[0] = 0;
					organizationName[0] = 0;
					organizationalUnitName[0] = 0;
					X509_NAME_get_text_by_NID(X509_get_subject_name(peer),
						NID_commonName, commonName, 256); // CN
					X509_NAME_get_text_by_NID(X509_get_subject_name(peer),
						NID_countryName, countryName, 256); // C
					X509_NAME_get_text_by_NID(X509_get_subject_name(peer),
						NID_localityName, localityName, 256); // L
					X509_NAME_get_text_by_NID(X509_get_subject_name(peer),
						NID_stateOrProvinceName, stateOrProvinceName, 256); // ST
					X509_NAME_get_text_by_NID(X509_get_subject_name(peer),
						NID_organizationName, organizationName, 256); // O
					X509_NAME_get_text_by_NID(X509_get_subject_name(peer),
						NID_organizationalUnitName, organizationalUnitName, 256); // OU
					notBefore = from_asn1_time(X509_get_notBefore(peer));
					notAfter  = from_asn1_time(X509_get_notAfter(peer)); // expire
					AGmNode *subject = List(
						String(commonName),
						String(countryName),
						String(localityName),
						String(stateOrProvinceName),
						String(organizationName),
						String(organizationalUnitName),
						notBefore, notAfter);

					commonName[0] = 0;
					countryName[0] = 0;
					localityName[0] = 0;
					stateOrProvinceName[0] = 0;
					organizationName[0] = 0;
					organizationalUnitName[0] = 0;
					X509_NAME_get_text_by_NID(X509_get_issuer_name(peer),
						NID_commonName, commonName, 256); // CN
					X509_NAME_get_text_by_NID(X509_get_issuer_name(peer),
						NID_countryName, countryName, 256); // C
					X509_NAME_get_text_by_NID(X509_get_issuer_name(peer),
						NID_localityName, localityName, 256); // L
					X509_NAME_get_text_by_NID(X509_get_issuer_name(peer),
						NID_stateOrProvinceName, stateOrProvinceName, 256); // ST
					X509_NAME_get_text_by_NID(X509_get_issuer_name(peer),
						NID_organizationName, organizationName, 256); // O
					X509_NAME_get_text_by_NID(X509_get_issuer_name(peer),
						NID_organizationalUnitName, organizationalUnitName, 256); // OU
					AGmNode *issuer = List(
						String(commonName),
						String(countryName),
						String(localityName),
						String(stateOrProvinceName),
						String(organizationName),
						String(organizationalUnitName));

					val = Ref(List(subject, issuer));
				}
			}
		}
#endif

	} else if (strcmp(s, "accept_que") == 0) {
		// ret_val: null, if success
		// arg1: socket_id
		// arg2: (hwnd, msg)
		//		 wparam = server_socket_id
		//		 lparam = socket_id
#if	_WINDOWS && _64BIT
		long long hwnd, msg;
#else
		long hwnd, msg;
#endif
		AGmNode *tail;
		if (IsInteger(arg1)
				&& Param(arg2, hwnd, msg, tail) == 0) {
			ADvSocket *socket = ADvSocket::GetSocket((int)arg1->a.i.v);
			if (socket == NULL || socket->accept_thread) {
				val = Ref(Integer(1));
			} else {
				socket->accept_queue = new ADvDataQueue();
				socket->msg = msg;
#ifdef	_WINDOWS
				socket->hWnd = (HWND)hwnd;
				DWORD threadID;
				SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
				socket->accept_thread = ::CreateThread(&sa, 0,
					(LPTHREAD_START_ROUTINE)queueAcceptFunc, socket,
					0, &threadID);
#endif
#ifdef	_LINUX
				socket->hWnd = hwnd;
				socket->accept_thread = g_thread_new(AL_STR("socket-queue-accept"), queueAcceptFunc, socket);
#endif
			}
		}
	} else if (strcmp(s, "recv_que") == 0) {
		// ret_val: null, if success
		// arg1: socket_id
		// arg2: (hwnd, msg)
		//		 wparam = socket_id
		//		 lparam = binary or string
#if	_WINDOWS && _64BIT
		long long hwnd, msg;
#else
		long hwnd, msg;
#endif
		AGmNode *tail;
		if (IsInteger(arg1)
				&& Param(arg2, hwnd, msg, tail) == 0) {
			ADvSocket *socket = ADvSocket::GetSocket((int)arg1->a.i.v);
			if (socket == NULL || socket->recv_thread) {
				val = Ref(Integer(1));
			} else {
				socket->recv_queue = new ADvDataQueue();
				socket->msg = msg;
#ifdef	_WINDOWS
				socket->hWnd = (HWND)hwnd;
				DWORD threadID;
				SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
				socket->recv_thread = ::CreateThread(&sa, 0,
					(LPTHREAD_START_ROUTINE)queueRecvFunc, socket,
					0, &threadID);
#endif
#ifdef	_LINUX
				socket->hWnd = hwnd;
				socket->recv_thread = g_thread_new(AL_STR("socket-queue-recv"), queueRecvFunc, socket);
#endif
			}
		}
	} else if (strcmp(s, "send_que") == 0) {
		// ret_val: null, if success
		// arg1: socket_id
		if (IsInteger(arg1)) {
			ADvSocket *socket = ADvSocket::GetSocket((int)arg1->a.i.v);
			if (socket == NULL || socket->send_thread) {
				val = Ref(Integer(1));
			} else {
#ifdef	_WINDOWS
				socket->wait_event = ::CreateEvent(NULL, TRUE, FALSE, NULL);
				socket->send_queue = new ADvDataQueue();
				DWORD threadID;
				SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
				socket->send_thread = ::CreateThread(&sa, 0,
					(LPTHREAD_START_ROUTINE)queueSendFunc, socket,
					0, &threadID);
#endif
#ifdef	_LINUX
				g_cond_init(&socket->wait_cond);
				socket->wait_cond_flag = 1;
				socket->send_queue = new ADvDataQueue();
				socket->send_thread = g_thread_new(AL_STR("socket-queue-send"), queueSendFunc, socket);
#endif
			}
		}
		
	} else if (strcmp(s, "get_socket") == 0) {
		// ret_val: binary, string, or null
		// arg1: socket_id
		if (IsInteger(arg1)) {
			ADvSocket *socket = ADvSocket::GetSocket((int)arg1->a.i.v);
			if (socket && socket->accept_queue) {
				ADvDataQueueItem *item = socket->accept_queue->Get();
				if (item) {
					val = Ref(Integer(item->socket_id));
					socket->accept_queue->Free(item);
				}
			} else {
			}
		}
	} else if (strcmp(s, "get_msg") == 0) {
		// ret_val: binary, string, or null
		// arg1: socket_id
		if (IsInteger(arg1)) {
			ADvSocket *socket = ADvSocket::GetSocket((int)arg1->a.i.v);
			if (socket && socket->recv_queue) {
				ADvDataQueueItem *item = socket->recv_queue->Get();
				if (item) {
					if (item->bin) {
						val = Ref(Binary(item->bin));
					} else if (item->str) {
						val = Ref(String(item->str));
					}
					socket->recv_queue->Free(item);
				}
			} else {
			}
		}
	} else if (strcmp(s, "put_msg") == 0) {
		// ret_val: null, if success
		// arg1: socket_id
		// arg2: (binary, index, size) or string
		AGmBinary *bin;
#if	_WINDOWS && _64BIT
		long long index, size;
#else
		long index, size;
#endif
		AGmNode *tail;
		if (IsInteger(arg1)
				&& Param(arg2, bin, tail) == 0
				&& Param(tail, index, size, tail) == 0
				&& index >= 0 && size >= 0
				&& index + size <= bin->Length()) {
			ADvSocket *socket = ADvSocket::GetSocket((int)arg1->a.i.v);
			if (!socket || !socket->send_queue) {
				val = Ref(Integer(1));
			} else {
				socket->send_queue->Put(
					socket->send_queue->Alloc(bin, (long)index, (long)size));
#ifdef	_WINDOWS
				::SetEvent(socket->wait_event);
#endif
			}
		} else if (IsInteger(arg1)
				&& IsString(arg2)) {
			ADvSocket *socket = ADvSocket::GetSocket((int)arg1->a.i.v);
			if (!socket || !socket->send_queue) {
				val = Ref(Integer(1));
			} else {
				socket->send_queue->Put(
					socket->send_queue->Alloc(arg2->a.s.v));
#ifdef	_WINDOWS
				::SetEvent(socket->wait_event);
#endif
			}
		} else {
			val = Ref(Integer(1));
		}

	} else if (strcmp(s, "ping") == 0) {

		if (IsInteger(arg1)) {
			long ip_dest = (long)arg1->a.i.v;
			SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
			if (sock == INVALID_SOCKET) {
				val = Ref(String("create socket failed"));
				Unref(cmd, arg1, arg2, arg3);
				return 0;
			}
			unsigned int bread;
#ifdef	_WINDOWS
			int timeout = 1000;
			bread = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,
				(char*)&timeout, sizeof(timeout));
			if (bread == INVALID_SOCKET) {
				val = Ref(String("failed to set recvfrom timeout"));
				Unref(cmd, arg1, arg2, arg3);
				closesocket(sock);
				return 0;
			}
			timeout = 1000;
			bread = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO,
				(char*)&timeout, sizeof(timeout));
			if (bread == INVALID_SOCKET) {
				val = Ref(String("failed to set sendto timeout"));
				Unref(cmd, arg1, arg2, arg3);
				closesocket(sock);
				return 0;
			}
#endif

			char icmp_dat[1024];
			char recvbuf[1024];
			int datasize = sizeof(IcmpHeader) + 32; 	
			memset(icmp_dat, 0, 1024);

			IcmpHeader *icmp_hdr = (IcmpHeader*)icmp_dat;
			icmp_hdr->i_type = 8; /* ICMP_ECHO */
			icmp_hdr->i_code = 0;
#ifdef	_WINDOWS
			icmp_hdr->i_id = (unsigned short)GetCurrentProcessId();
#endif
#ifdef	_LINUX
			icmp_hdr->i_id = (unsigned short)getpid();
#endif
			icmp_hdr->i_cksum = 0;
			icmp_hdr->i_seq = 0;
			char *datapart = icmp_dat + sizeof(IcmpHeader);
			memset(datapart,'E', datasize - sizeof(IcmpHeader));

			SOCKADDR_IN dest, from;
			memset(&dest, 0, sizeof(dest));
#ifdef	_WINDOWS
			int fromlen = sizeof(from);
#endif
#ifdef	_LINUX
			socklen_t fromlen = sizeof(from);
#endif
			dest.sin_family = AF_INET;	
			dest.sin_addr.s_addr = htonl(ip_dest);

			((IcmpHeader*)icmp_dat)->i_cksum = 0;
#ifdef	_WINDOWS
			((IcmpHeader*)icmp_dat)->timestamp = GetTickCount();
#endif
#ifdef	_LINUX
			struct timeb tb;
			ftime(&tb);
			((IcmpHeader*)icmp_dat)->timestamp = tb.time * 1000 + tb.millitm;
#endif
			((IcmpHeader*)icmp_dat)->i_seq = 1;

			unsigned long cksum = 0;
			unsigned short *buffer = (unsigned short*)icmp_dat;
			int size = datasize;
			while (size > 1) {
				cksum += *buffer++;
				size -= sizeof(unsigned short);
			}
			if (size) {
				cksum += *(unsigned char*)buffer;
			}
			cksum = (cksum >> 16) + (cksum & 0xffff);
			cksum += (cksum >>16);
			((IcmpHeader*)icmp_dat)->i_cksum = (unsigned short)~cksum;

			int bwrote = sendto(sock, icmp_dat, datasize, 0,
				(struct sockaddr*)&dest, sizeof(dest));
			if (bwrote == SOCKET_ERROR) {
				val = Ref(String("sendto failed"));
				Unref(cmd, arg1, arg2, arg3);
				closesocket(sock);
				return 0;
			}

			bread = recvfrom(sock, recvbuf, 1024, 0,	
				(struct sockaddr*)&from, &fromlen);
			if (bread == SOCKET_ERROR) {
				val = Ref(String("recvfrom failed"));
				Unref(cmd, arg1, arg2, arg3);
				closesocket(sock);
				return 0;
			}

			IpHeader *iphdr = (IpHeader *)recvbuf;
			unsigned int iphdrlen = iphdr->h_len * 4 ; // number of 32-bit words *4 = bytes

			if (bread  < iphdrlen + 8) {
				char msg[256];
				art_sprintf_1(msg, 256, "too few bytes from %s", inet_ntoa(from.sin_addr));
				val = Ref(String(msg));
				Unref(cmd, arg1, arg2, arg3);
				closesocket(sock);
				return 0;
				
			}

			IcmpHeader *icmphdr = (IcmpHeader*)(recvbuf + iphdrlen);
#ifdef	_WINDOWS
			if (icmphdr->i_type != 0) {
				char msg[256];
				art_sprintf_1(msg, 256, "non-echo type %d recv", icmphdr->i_type);
				val = Ref(String(msg));
				Unref(cmd, arg1, arg2, arg3);
				closesocket(sock);
				return 0;
			}
			if (icmphdr->i_id != (unsigned short)GetCurrentProcessId()) {
#endif
#ifdef	_LINUX
			if (icmphdr->i_id != (unsigned short)getpid()) {
#endif
				char msg[256];
				art_sprintf_0(msg, 256, "someone else's packet!");
				val = Ref(String(msg));
				Unref(cmd, arg1, arg2, arg3);
				closesocket(sock);
				return 0;
			}
#ifdef	_WINDOWS
			val = Ref(List(
				Integer(bread),
				Integer(GetTickCount() - icmphdr->timestamp) ));
#endif
#ifdef	_LINUX
			ftime(&tb);
			val = Ref(List(
				Integer(bread),
				Integer(tb.time * 1000 + tb.millitm - icmphdr->timestamp) ));
#endif
			closesocket(sock);

		} else {
			val = Ref(String("illegal hostname"));
			Unref(cmd, arg1, arg2, arg3);
			return 0;
		}
	} else {
		err = 1;
	}
	if (err) {
		Unref(cmd, arg1, arg2, arg3);
		return interp_error("Socket()");
	}
	Unref(cmd, arg1, arg2, arg3);
	return 0;
}
