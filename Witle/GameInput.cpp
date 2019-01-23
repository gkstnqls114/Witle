//-----------------------------------------------------------------------------
// File: GameInput.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameInput.h"

UCHAR GameInput::m_pKeyBuffer[256];

float GameInput::m_cDeltaX = 0.0f; // ���콺�� ���� ���·� x������ ������ ���콺 �̵���  
float GameInput::m_cDeltaY = 0.0f; // ���콺�� ���� ���·� y������ ������ ���콺 �̵���
POINT GameInput::m_oldCursor; // ���� �����ӿ����� ���콺 ��ġ

GameInput::GameInput()
{
}

GameInput::~GameInput()
{
}

void GameInput::Update(HWND hWnd)
{ 
	// Ű������ pKeyBuffer�� ���Ѵ�.
	::GetKeyboardState(m_pKeyBuffer);
	 
	POINT ptCursorPos;
	/*���콺�� ĸ�������� ���콺�� �󸶸�ŭ �̵��Ͽ��� ���� ����Ѵ�. ���콺 ���� �Ǵ� ������ ��ư�� ������ ����
	�޽���(WM_LBUTTONDOWN, WM_RBUTTONDOWN)�� ó���� �� ���콺�� ĸ���Ͽ���. �׷��Ƿ� ���콺�� ĸ�ĵ�
	���� ���콺 ��ư�� ������ ���¸� �ǹ��Ѵ�. ���콺 ��ư�� ������ ���¿��� ���콺�� �¿� �Ǵ� ���Ϸ� �����̸� ��
	���̾ x-�� �Ǵ� y-������ ȸ���Ѵ�.*/
	if (::GetCapture() == hWnd)
	{
		//���콺 Ŀ���� ȭ�鿡�� ���ش�(������ �ʰ� �Ѵ�).
		::SetCursor(NULL);
		//���� ���콺 Ŀ���� ��ġ�� �����´�.
		::GetCursorPos(&ptCursorPos);
		//���콺 ��ư�� ���� ���¿��� ���콺�� ������ ���� ���Ѵ�.
		m_cDeltaX = (float)(ptCursorPos.x - m_oldCursor.x) / 3.0f;
		m_cDeltaY = (float)(ptCursorPos.y - m_oldCursor.y) / 3.0f;

		//���콺 Ŀ���� ��ġ�� ���콺�� �������� ��ġ�� �����Ѵ�.
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
