//-----------------------------------------------------------------------------
// File: GameInput.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameScreen.h"
#include "GameInput.h"

HWND GameInput::m_hWnd;

UCHAR GameInput::m_pKeyBuffer[256];

float GameInput::m_cDeltaX = 0.0f; // ���콺�� ���� ���·� x������ ������ ���콺 �̵���  
float GameInput::m_cDeltaY = 0.0f; // ���콺�� ���� ���·� y������ ������ ���콺 �̵���
POINT GameInput::m_oldCursor; // ���� �����ӿ����� ���콺 ��ġ
POINT GameInput::m_clickCursor{ -1, -1 }; // �ѹ� Ŭ������ �� ��ġ
const float GameInput::m_DeltaValue = 3.0f; // ���콺 �̵��� �� 
short GameInput::m_WheelDelta; // ���콺 ���� ������ ����

GameInput::GameInput()
{
}

GameInput::~GameInput()
{
}

bool GameInput::GenerateRayforPicking(const XMFLOAT3& cameraPos, const XMFLOAT4X4 & view, const XMFLOAT4X4 & projection, RAY& ray)
{
	if (m_clickCursor.x < 0 || m_clickCursor.y < 0 || m_clickCursor.x > GameScreen::GetWidth() || m_clickCursor.y > GameScreen::GetHeight())
	{
		return false;
	}
	ray.origin = cameraPos;

	// ���� ����� (0, 0)�� ǥ���Ǿ��ִ� ������ ��ǥ���� Ŭ�� Ŀ����
	// ȭ�� �߽��� (0, 0)�̸� �� ���� -1~1 �� ǥ���Ǵ�, �� ���� ��ǥ��� �����Ѵ�.  
	ray.direction.x = float(((2.0f * m_clickCursor.x) / float(GameScreen::GetWidth())) - 1.0f);
	ray.direction.y = float((-(2.0f * m_clickCursor.y) / float(GameScreen::GetHeight())) + 1.0f);

	//����� ������ǥ���� Direction ��ǥ
	//�� Direction�� x, y��ǥ�� -1�� 1�����̴�.

	// Ŭ�� ��ǥ�� ������ǥ�迡�� ī�޶� ��ǥ��� ����
	ray.direction.x = ray.direction.x / projection._11;
	ray.direction.y = ray.direction.y / projection._22;
	ray.direction.z = 1.0f;

	// ���� ī�޶� ��ǥ��...

	// ī�޶� ��ǥ�迡�� ���� ��ǥ��� ��ȯ
	XMFLOAT4X4 InverseView = Matrix4x4::Inverse(view);  
	ray.direction = Vector3::TransformCoord(ray.direction, InverseView);
	
	// ī�޶� ��ġ�������� ���� ���͸� ���Ѵ�
	ray.direction = Vector3::Normalize(Vector3::Subtract(ray.direction, ray.origin));

	return true;
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
		m_cDeltaX = (float)(ptCursorPos.x - m_oldCursor.x) / m_DeltaValue;
		m_cDeltaY = (float)(ptCursorPos.y - m_oldCursor.y) / m_DeltaValue;

		//���콺 Ŀ���� ��ġ�� ���콺�� �������� ��ġ�� �����Ѵ�.
		::SetCursorPos(m_oldCursor.x, m_oldCursor.y);

	}
	
}

void GameInput::Reset()
{
	m_clickCursor.x = -1;
	m_clickCursor.y = -1;
}

void GameInput::SetCapture(HWND hWnd)
{
	::SetCapture(hWnd);
	::GetCursorPos(&m_oldCursor);
	::GetCursorPos(&m_clickCursor);
	::ScreenToClient(hWnd, &m_clickCursor);
}

void GameInput::RotateWheel(WPARAM wParam)
{
	m_WheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
}

void GameInput::ReleaseCapture()
{
	::ReleaseCapture();
}
