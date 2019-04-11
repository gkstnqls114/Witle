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

#define MAX_BUFF_SIZE   4000  // 서버에서 받을 수 있는 데이터의 총 크기
#define MAX_PACKET_SIZE  255  // 패킷에서 처리할 수 있는 데이터의 크기

class CNetworkManager
{
public:
	CNetworkManager();
	virtual ~CNetworkManager();

	VOID CNetworkManagerInit(); // 네트워크 초기화
	BOOL CNetworkManagerBegin(); // 네트워크
};

struct OVERLAPPED_EX
{
	WSAOVERLAPPED overapped;		// 확장한 overlapped 구조체를 읽기 위한 원래 overlapped 구조체(id값과 포인터 관리)
	WSABUF wsabuf;					// 버퍼의 포인트와 길이만 있다 = >각각 지정해줘야 한다
	UINT operation_type;			// 어떤 작업인지 구분(Send, Recv)
	CHAR Socket_buf[MAX_BUFF_SIZE];	// 서버가 받을 수 있는 패킷의 전체 크기
	CHAR PacketBuf[MAX_PACKET_SIZE];// 처리할 수 있는 패킷 크기
	UINT prev_received;				// 이전에 받은 패킷 크기
	UINT curr_packet_size;			// 받을 패킷의 사이즈
};