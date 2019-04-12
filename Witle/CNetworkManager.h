#pragma once

#include "stdafx.h"

void err_quit(const char * msg);
void err_message(const char * msg);
void err_display(const char * msg);
int recvn(SOCKET s, char* buf, int len, int flags);

#define SERVER_IP  "127.0.0.1"	// �ϴ� �� �ڽ����� ��������!
#define SERVERPORT 9000
#define BUFSIZE    512
#define MAXPLAYER 1000 // �÷��̾ �ִ� ������ ������.... 
#define	WM_SOCKET				WM_USER + 1

#define MAX_BUFF_SIZE   4000  // �������� ���� �� �ִ� �������� �� ũ��
#define MAX_PACKET_SIZE  255  // ��Ŷ���� ó���� �� �ִ� �������� ũ��

class CNetworkManager
{
public:
	CNetworkManager();
	virtual ~CNetworkManager();

	VOID CNetworkManagerInit(); // ��Ʈ��ũ �ʱ�ȭ
	BOOL CNetworkManagerBegin(); // ��Ʈ��ũ
};

struct OVERLAPPED_EX
{
	WSAOVERLAPPED overapped;		// Ȯ���� overlapped ����ü�� �б� ���� ���� overlapped ����ü(id���� ������ ����)
	WSABUF wsabuf;					// ������ ����Ʈ�� ���̸� �ִ� = >���� ��������� �Ѵ�
	UINT operation_type;			// � �۾����� ����(Send, Recv)
	CHAR Socket_buf[MAX_BUFF_SIZE];	// ������ ���� �� �ִ� ��Ŷ�� ��ü ũ��
	CHAR PacketBuf[MAX_PACKET_SIZE];// ó���� �� �ִ� ��Ŷ ũ��
	UINT prev_received;				// ������ ���� ��Ŷ ũ��
	UINT curr_packet_size;			// ���� ��Ŷ�� ������
};

struct ClientID {
	USHORT PlayerID;
};

struct PlayerState {
	BOOL mUsingID;			// ������ id ���� ���Ǵ��� �Ǵ� (�˻��� ���� ������ �߰��� �����ٸ� �ش� ������ m_isEscape���·� ���� ��)
	BOOL m_isLobby;			// ������ �κ� �ִ��� �Ǵ� (�� �����)
	BOOL m_isRoom;			// ������ �濡 �ִ��� �Ǵ�
	BOOL m_isGameReady;		// ������ �غ�Ϸ� ����
	BOOL m_isGameAlive;		// ������ �����ʰ� ���� ������ �Ǵ�
	BOOL m_isGameDie;		// ������ �׾����� �Ǵ�
	BOOL m_isRespon;		// ������ ������ ������ �Ǵ�
	BOOL m_isGamePlay;		// ������ (�ʵ忡��)����������
	BOOL m_isEscape;		// ������ ���������� ������� ������ ������ ���
};

enum SceneType {
	LobbyScene,
	RoomScene,
	GameScene,
	ResponScene,			// �÷��̾ ������ �ɽ� ȭ���� ��� ���ִ°� ������? ��������ó�� �ٸ� ��� �� �� �ֵ���...
	GameResult
};

enum TeamColor {
	// TeamRed,				// �¾��� -> ���帻�� �ٸ� �ɷ� �ұ�...
	// TeamBlue				// �� ��  -> �Ķ��� ���� �ٸ� �ɷ� �ұ�...
	TeamSun,
	Teammoon
};