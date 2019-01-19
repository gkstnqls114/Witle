//-----------------------------------------------------------------------------
// File: GameInput.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameInput.h"

UCHAR GameInput::m_pKeyBuffer[256];

float GameInput::m_cDeltaX = 0.0f; // 마우스를 누른 상태로 x축으로 움직인 마우스 이동량  
float GameInput::m_cDeltaY = 0.0f; // 마우스를 누른 상태로 y축으로 움직인 마우스 이동량
POINT GameInput::m_oldCursor; // 이전 프레임에서의 마우스 위치

GameInput::GameInput()
{
}

GameInput::~GameInput()
{
}

void GameInput::Update(HWND hWnd)
{ 
	// 키보드의 pKeyBuffer를 구한다.
	::GetKeyboardState(m_pKeyBuffer);
	 
	POINT ptCursorPos;
	/*마우스를 캡쳐했으면 마우스가 얼마만큼 이동하였는 가를 계산한다. 마우스 왼쪽 또는 오른쪽 버튼이 눌러질 때의
	메시지(WM_LBUTTONDOWN, WM_RBUTTONDOWN)를 처리할 때 마우스를 캡쳐하였다. 그러므로 마우스가 캡쳐된
	것은 마우스 버튼이 눌려진 상태를 의미한다. 마우스 버튼이 눌려진 상태에서 마우스를 좌우 또는 상하로 움직이면 플
	레이어를 x-축 또는 y-축으로 회전한다.*/
	if (::GetCapture() == hWnd)
	{
		//마우스 커서를 화면에서 없앤다(보이지 않게 한다).
		::SetCursor(NULL);
		//현재 마우스 커서의 위치를 가져온다.
		::GetCursorPos(&ptCursorPos);
		//마우스 버튼이 눌린 상태에서 마우스가 움직인 양을 구한다.
		m_cDeltaX = (float)(ptCursorPos.x - m_oldCursor.x) / 3.0f;
		m_cDeltaY = (float)(ptCursorPos.y - m_oldCursor.y) / 3.0f;

		//마우스 커서의 위치를 마우스가 눌려졌던 위치로 설정한다.
		::SetCursorPos(m_oldCursor.x, m_oldCursor.y);
	}
}

void GameInput::SetCapture(HWND hWnd)
{
	::SetCapture(hWnd);
	::GetCursorPos(&m_oldCursor);
}

void GameInput::ReleaseCapture()
{
	::ReleaseCapture();
}
