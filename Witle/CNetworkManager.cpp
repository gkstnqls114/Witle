#include "stdafx.h"
#include "CNetworkManager.h"

// 데이터 통신에 사용할 변수
char m_Buf[BUFSIZE + 1];
int m_Len{ 0 };

HANDLE hClientUpdate;
HWND hWnd;
HWND main_window_handle = NULL; // save the window handle
// HINSTANCE main_instance = NULL; // save the instance

void err_quit(const char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_message(const char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
}

void err_display(const char * msg)
{
	{
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, NULL);
		printf("[%s] %s", msg, (char *)lpMsgBuf);
		LocalFree(lpMsgBuf);
	}
}

int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char * ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		else if (received == 0) {
			break;
		}
		left -= received;
		ptr += received;
	}
	return len - left;
}

CNetworkManager::CNetworkManager()
{
}

CNetworkManager::~CNetworkManager()
{
}


VOID CNetworkManager::CNetworkManagerInit()
{
	// 윈속 초기화
	WSADATA gWSA;
	WSAStartup(MAKEWORD(2, 2), &gWSA);
}

BOOL CNetworkManager::CNetworkManagerBegin()
{
	SOCKET gSocket;
	SOCKADDR_IN gServerAddr;
	HANDLE hProcessServer;

	//socket()
	gSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
	if (gSocket == INVALID_SOCKET) {
		DestroyWindow(hWnd);
		err_quit("socket()");
		return false;
	}

	//connet()
	ZeroMemory(&gServerAddr, sizeof(gServerAddr));
	gServerAddr.sin_family = AF_INET;
	gServerAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	gServerAddr.sin_port = htons(SERVERPORT);
	int retval = connect(gSocket, (SOCKADDR *)&gServerAddr, sizeof(gServerAddr));
	if (retval == SOCKET_ERROR) {
		err_quit("connect()");
		return false;
	}

	WSAAsyncSelect(gSocket, main_window_handle, WM_SOCKET, FD_CLOSE | FD_READ);
}