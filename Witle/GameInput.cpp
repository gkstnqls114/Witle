//-----------------------------------------------------------------------------
// File: GameInput.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameScreen.h"
#include "GameObject.h"
#include "Camera.h"
#include "GameInput.h"


HWND GameInput::m_hWnd;

UCHAR GameInput::m_pKeyBuffer[256]; 
const bool GameInput::m_DragMode{ true };
 
POINT GameInput::m_moveCursor;        // �ѹ� Ŭ������ �� ��ġ
POINT GameInput::m_moveOldCursor{ -1, -1};     // ���� �����ӿ����� ���콺 ��ġ 
float GameInput::m_moveDeltaX = 0.0f; // ���콺�� ���� ���·� x������ ������ ���콺 �̵���  
float GameInput::m_moveDeltaY = 0.0f; // ���콺�� ���� ���·� y������ ������ ���콺 �̵���

float GameInput::m_downDeltaX = 0.0f; // ���콺�� ���� ���·� x������ ������ ���콺 �̵���  
float GameInput::m_downDeltaY = 0.0f; // ���콺�� ���� ���·� y������ ������ ���콺 �̵���
POINT GameInput::m_downOldCursor; // ���� �����ӿ����� ���콺 ��ġ
POINT GameInput::m_downClickCursor{ -1, -1 }; // �ѹ� Ŭ������ �� ��ġ

const float GameInput::m_DeltaValueX = 10.0f; // ���콺 �̵��� �� 
const float GameInput::m_DeltaValueY = 10.0f; // ���콺 �̵��� �� 
short GameInput::m_WheelDelta; // ���콺 ���� ������ ����

GameInput::GameInput()
{
}

GameInput::~GameInput()
{
}

bool GameInput::GenerateRayforPicking(const XMFLOAT3& cameraPos, const XMFLOAT4X4 & view, const XMFLOAT4X4 & projection, RAY& ray)
{
	if (m_downClickCursor.x < 0 || m_downClickCursor.y < 0 || m_downClickCursor.x > GameScreen::GetWidth() || m_downClickCursor.y > GameScreen::GetHeight())
	{
		return false;
	}
	
	ray = RAY::GeneratePickingRay(cameraPos, XMFLOAT2(m_downClickCursor.x, m_downClickCursor.y), view, projection);
	 
	return true;
}

void GameInput::UpdateMouseDragRotate(HWND hWnd)
{
	POINT ptCursorPos;

	// ���콺�� ĸ�������� ���콺�� �󸶸�ŭ �̵��Ͽ��� ���� ����Ѵ�.
	// ���콺 ���� �Ǵ� ������ ��ư�� ������ ���� �޽���(WM_LBUTTONDOWN, WM_RBUTTONDOWN)�� ó���� �� 
	// ���콺�� ĸ���Ͽ���. �׷��Ƿ� ���콺�� ĸ�ĵ� ���� ���콺 ��ư�� ������ ���¸� �ǹ��Ѵ�.
	// ���콺 ��ư�� ������ ���¿��� ���콺�� �¿� �Ǵ� ���Ϸ� �����̸� �÷��̾ x-�� �Ǵ� y-������ ȸ���Ѵ�.
	if (::GetCapture() == hWnd)
	{
		//���콺 Ŀ���� ȭ�鿡�� ���ش�(������ �ʰ� �Ѵ�).
		::SetCursor(NULL);

		//���� ���콺 Ŀ���� ��ġ�� �����´�.
		::GetCursorPos(&ptCursorPos);
		//���콺 ��ư�� ���� ���¿��� ���콺�� ������ ���� ���Ѵ�.
		m_downDeltaX = (float)(ptCursorPos.x - m_downOldCursor.x) / m_DeltaValueX;
		m_downDeltaY = (float)(ptCursorPos.y - m_downOldCursor.y) / m_DeltaValueY;

		//���콺 Ŀ���� ��ġ�� ���콺�� �������� ��ġ�� �����Ѵ�.
		::SetCursorPos(m_downOldCursor.x, m_downOldCursor.y);
	}
}

void GameInput::UpdateMouseMoveRotate(HWND hWnd)
{
}

void GameInput::Update(HWND hWnd)
{ 
	// Ű������ pKeyBuffer�� ���Ѵ�.
	::GetKeyboardState(m_pKeyBuffer);

	if (m_DragMode)
	{ 
		UpdateMouseDragRotate(hWnd);
	} 
}
 
void GameInput::Reset()
{
	m_downClickCursor.x = MOUSE_NONE;
	m_downClickCursor.y = MOUSE_NONE;
	m_moveDeltaX = 0.f;
	m_moveDeltaY = 0.f; 

	if (!m_DragMode)
	{ 
		if (m_moveOldCursor.x == MOUSE_NONE && m_moveOldCursor.y == MOUSE_NONE) return; // �ʱⰪ�̶�� �Ѿ.
		::SetCursorPos(m_moveOldCursor.x, m_moveOldCursor.y);
		m_moveCursor = m_moveOldCursor;
	}
}

void GameInput::SetHWND(HWND hwnd)
{
	m_hWnd = hwnd;
}

void GameInput::MouseMove(LPARAM lParam)
{     
	if (!m_DragMode)
	{
		::GetCursorPos(&m_moveCursor);

		if (m_moveCursor.x == m_moveOldCursor.x && m_moveCursor.y == m_moveOldCursor.y) return;

		if ((m_moveOldCursor.x == MOUSE_NONE && m_moveOldCursor.y == MOUSE_NONE)) // �ʱⰪ�� ���
		{
			m_moveOldCursor = m_moveCursor;
		}
		else
		{
			m_moveDeltaX = (float)(m_moveCursor.x - m_moveOldCursor.x) / m_DeltaValueX;
			m_moveDeltaY = (float)(m_moveCursor.y - m_moveOldCursor.y) / m_DeltaValueY;
		}
	}
}

void GameInput::SetCapture(HWND hWnd)
{
	::SetCapture(hWnd);
	::GetCursorPos(&m_downOldCursor);
	::GetCursorPos(&m_downClickCursor);
	::ScreenToClient(hWnd, &m_downClickCursor);
}

void GameInput::RotateWheel(WPARAM wParam)
{
	m_WheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
}

void GameInput::ReleaseCapture()
{
	::ReleaseCapture();
}
 
RAY RAY::GeneratePickingRay(const XMFLOAT3 & cameraPos, const XMFLOAT2 & ScreenPickingPoint, const XMFLOAT4X4 & view, const XMFLOAT4X4 & projection)
{
	RAY ray;

	ray.origin = cameraPos;

	// ���� ����� (0, 0)�� ǥ���Ǿ��ִ� ������ ��ǥ���� Ŭ�� Ŀ����
	// ȭ�� �߽��� (0, 0)�̸� �� ���� -1~1 �� ǥ���Ǵ�, �� ���� ��ǥ��� �����Ѵ�.  
	ray.direction.x = float(((2.0f * ScreenPickingPoint.x) / float(GameScreen::GetWidth())) - 1.0f);
	ray.direction.y = float((-(2.0f * ScreenPickingPoint.y) / float(GameScreen::GetHeight())) + 1.0f);

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

	return ray;
}

RAY RAY::GeneratePickingRay(const XMFLOAT2 & ScreenPickingPoint, Camera * pCamera)
{
	RAY ray;

	ray.origin = pCamera->GetpOwner()->GetTransform().GetPosition();

	// ���� ����� (0, 0)�� ǥ���Ǿ��ִ� ������ ��ǥ���� Ŭ�� Ŀ����
	// ȭ�� �߽��� (0, 0)�̸� �� ���� -1~1 �� ǥ���Ǵ�, �� ���� ��ǥ��� �����Ѵ�.  
	ray.direction.x = float(((2.0f * ScreenPickingPoint.x) / float(GameScreen::GetWidth())) - 1.0f);
	ray.direction.y = float((-(2.0f * ScreenPickingPoint.y) / float(GameScreen::GetHeight())) + 1.0f);

	//����� ������ǥ���� Direction ��ǥ
	//�� Direction�� x, y��ǥ�� -1�� 1�����̴�.

	// Ŭ�� ��ǥ�� ������ǥ�迡�� ī�޶� ��ǥ��� ����
	ray.direction.x = ray.direction.x / pCamera->GetProjectionMatrix()._11;
	ray.direction.y = ray.direction.y / pCamera->GetProjectionMatrix()._22;
	ray.direction.z = 1.0f;

	// ���� ī�޶� ��ǥ��...

	// ī�޶� ��ǥ�迡�� ���� ��ǥ��� ��ȯ
	XMFLOAT4X4 InverseView = Matrix4x4::Inverse(pCamera->GetViewMatrix());
	ray.direction = Vector3::TransformCoord(ray.direction, InverseView);

	// ī�޶� ��ġ�������� ���� ���͸� ���Ѵ�
	ray.direction = Vector3::Normalize(Vector3::Subtract(ray.direction, ray.origin));

	return ray;
}

XMFLOAT3 RAY::GeneratePickingScreenOrigin(const XMFLOAT3 & cameraPos, const XMFLOAT2 & ScreenPickingPoint, const XMFLOAT4X4 & view, const XMFLOAT4X4 & projection)
{ 
	XMFLOAT3 origin = cameraPos;
	XMFLOAT3 direction;

	// ���� ����� (0, 0)�� ǥ���Ǿ��ִ� ������ ��ǥ���� Ŭ�� Ŀ����
	// ȭ�� �߽��� (0, 0)�̸� �� ���� -1~1 �� ǥ���Ǵ�, �� ���� ��ǥ��� �����Ѵ�.  
	direction.x = float(((2.0f * ScreenPickingPoint.x) / float(GameScreen::GetWidth())) - 1.0f);
	direction.y = float((-(2.0f * ScreenPickingPoint.y) / float(GameScreen::GetHeight())) + 1.0f);

	//����� ������ǥ���� Direction ��ǥ
	//�� Direction�� x, y��ǥ�� -1�� 1�����̴�.

	// Ŭ�� ��ǥ�� ������ǥ�迡�� ī�޶� ��ǥ��� ����
	direction.x = direction.x / projection._11;
	direction.y = direction.y / projection._22;
	direction.z = 1.0f;

	// ���� ī�޶� ��ǥ��...

	// ī�޶� ��ǥ�迡�� ���� ��ǥ��� ��ȯ
	XMFLOAT4X4 InverseView = Matrix4x4::Inverse(view);
	direction = Vector3::TransformCoord(direction, InverseView);

	return direction;
}
 