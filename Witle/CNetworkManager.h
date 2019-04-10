#pragma once

#include "stdafx.h"

void err_quit(const char * msg);
void err_message(const char * msg);
void err_display(const char * msg);
int recvn(SOCKET s, char* buf, int len, int flags);

#define SERVER_IP  "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512
#define	WM_SOCKET				WM_USER + 1

class CNetworkManager
{
public:
	CNetworkManager();
	virtual ~CNetworkManager();

	VOID CNetworkManagerInit(); // 네트워크 초기화
	BOOL CNetworkManagerBegin(); // 네트워크
};