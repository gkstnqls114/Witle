#pragma once

#include "stdafx.h"

void err_quit(const char * msg);
void err_message(const char * msg);
void err_display(const char * msg);
int recvn(SOCKET s, char* buf, int len, int flags);

#define SERVER_IP  "127.0.0.1"	// 일단 나 자신한테 연결하자!
#define SERVERPORT 9000
#define BUFSIZE    512
#define MAXPLAYER 1000 // 플레이어를 최대 몇명까지 받을까.... 
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

struct ClientID {
	USHORT PlayerID;
	OVERLAPPED_EX Player_OVERLAPPED;
	BOOL mUsingID;			// 유저의 id 값이 사용되는지 판단 (검사후 만약 유저가 중간에 나갔다면 해당 유저를 m_isEscape상태로 만들 것)
};

struct PlayerState {
	BOOL m_isLobby;			// 유저가 로비에 있는지 판단 (방 만들기)
	BOOL m_isRoom;			// 유저가 방에 있는지 판단
	BOOL m_isGameReady;		// 유저의 준비완료 여부
	BOOL m_isGameAlive;		// 유저가 죽지않고 게임 중인지 판단
	BOOL m_isGameDie;		// 유저가 죽었는지 판단
	BOOL m_isRespon;		// 유저가 리스폰 중인지 판단
	BOOL m_isGamePlay;		// 유저가 (필드에서)게임중인지
	BOOL m_isEscape;		// 유저가 비정상적인 방법으로 게임을 나갔을 경우
};

enum SceneType {
	LobbyScene,
	RoomScene,
	GamePlayerScene,
	ResponScene,			// 플레이어가 리스폰 될시 화면을 어ㅔ 해주는게 나을까? 사이퍼즈처럼 다른 사람 볼 수 있도록...
	GameResult
};

enum TeamColor {
	// TeamRed,				// 태양팀 -> 레드말고 다른 걸로 할까...
	// TeamBlue				// 달 팀  -> 파란팀 말고 다른 걸로 할까...
	TeamSun,
	Teammoon
};