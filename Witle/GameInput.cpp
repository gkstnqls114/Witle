//-----------------------------------------------------------------------------
// File: GameInput.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameScreen.h"
#include "GameInput.h"

HWND GameInput::m_hWnd;

UCHAR GameInput::m_pKeyBuffer[256];

bool GameInput::m_isDragRotate{ true };
 
POINT GameInput::m_moveCursor;        // 한번 클릭했을 때 위치
POINT GameInput::m_moveOldCursor{ -1, -1};     // 이전 프레임에서의 마우스 위치 
float GameInput::m_moveDeltaX = 0.0f; // 마우스를 누른 상태로 x축으로 움직인 마우스 이동량  
float GameInput::m_moveDeltaY = 0.0f; // 마우스를 누른 상태로 y축으로 움직인 마우스 이동량

float GameInput::m_downDeltaX = 0.0f; // 마우스를 누른 상태로 x축으로 움직인 마우스 이동량  
float GameInput::m_downDeltaY = 0.0f; // 마우스를 누른 상태로 y축으로 움직인 마우스 이동량
POINT GameInput::m_downOldCursor; // 이전 프레임에서의 마우스 위치
POINT GameInput::m_downClickCursor{ -1, -1 }; // 한번 클릭했을 때 위치

const float GameInput::m_DeltaValueX = 10.0f; // 마우스 이동량 값 
const float GameInput::m_DeltaValueY = 10.0f; // 마우스 이동량 값 
short GameInput::m_WheelDelta; // 마우스 휠이 움직인 정도

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
	ray.origin = cameraPos;

	// 왼쪽 상단이 (0, 0)로 표현되어있는 윈도우 좌표계의 클릭 커서를
	// 화면 중심이 (0, 0)이며 각 축이 -1~1 로 표현되는, 즉 투영 좌표계로 변경한다.  
	ray.direction.x = float(((2.0f * m_downClickCursor.x) / float(GameScreen::GetWidth())) - 1.0f);
	ray.direction.y = float((-(2.0f * m_downClickCursor.y) / float(GameScreen::GetHeight())) + 1.0f);

	//현재는 투영좌표계의 Direction 좌표
	//즉 Direction의 x, y좌표는 -1과 1사이이다.

	// 클릭 좌표를 투영좌표계에서 카메라 좌표계로 변경
	ray.direction.x = ray.direction.x / projection._11;
	ray.direction.y = ray.direction.y / projection._22;
	ray.direction.z = 1.0f;

	// 현재 카메라 좌표계...

	// 카메라 좌표계에서 월드 좌표계로 변환
	XMFLOAT4X4 InverseView = Matrix4x4::Inverse(view);  
	ray.direction = Vector3::TransformCoord(ray.direction, InverseView);
	
	// 카메라 위치에서부터 방향 벡터를 구한다
	ray.direction = Vector3::Normalize(Vector3::Subtract(ray.direction, ray.origin));

	return true;
}

void GameInput::UpdateMouseDragRotate(HWND hWnd)
{
	POINT ptCursorPos;

	// 마우스를 캡쳐했으면 마우스가 얼마만큼 이동하였는 가를 계산한다.
	// 마우스 왼쪽 또는 오른쪽 버튼이 눌러질 때의 메시지(WM_LBUTTONDOWN, WM_RBUTTONDOWN)를 처리할 때 
	// 마우스를 캡쳐하였다. 그러므로 마우스가 캡쳐된 것은 마우스 버튼이 눌려진 상태를 의미한다.
	// 마우스 버튼이 눌려진 상태에서 마우스를 좌우 또는 상하로 움직이면 플레이어를 x-축 또는 y-축으로 회전한다.
	if (::GetCapture() == hWnd)
	{
		//마우스 커서를 화면에서 없앤다(보이지 않게 한다).
		::SetCursor(NULL);

		//현재 마우스 커서의 위치를 가져온다.
		::GetCursorPos(&ptCursorPos);
		//마우스 버튼이 눌린 상태에서 마우스가 움직인 양을 구한다.
		m_downDeltaX = (float)(ptCursorPos.x - m_downOldCursor.x) / m_DeltaValueX;
		m_downDeltaY = (float)(ptCursorPos.y - m_downOldCursor.y) / m_DeltaValueY;

		//마우스 커서의 위치를 마우스가 눌려졌던 위치로 설정한다.
		::SetCursorPos(m_downOldCursor.x, m_downOldCursor.y);
	}
}

void GameInput::UpdateMouseMoveRotate(HWND hWnd)
{
}

void GameInput::Update(HWND hWnd)
{ 
	// 키보드의 pKeyBuffer를 구한다.
	::GetKeyboardState(m_pKeyBuffer);
	 
	//if (m_isDragRotate)
	//{ 
	//	UpdateMouseDragRotate(hWnd);
	//}
	//else
	//{
	//	UpdateMouseMoveRotate(hWnd);
	//} 
}

void GameInput::Reset()
{
	m_downClickCursor.x = -1;
	m_downClickCursor.y = -1;
	m_moveDeltaX = 0.f;
	m_moveDeltaY = 0.f; 
	if (m_moveOldCursor.x == -1 && m_moveOldCursor.y == -1) return; // 초기값이라면 넘어감.
	::SetCursorPos(m_moveOldCursor.x, m_moveOldCursor.y);
	 m_moveCursor = m_moveOldCursor;
}

void GameInput::SetHWND(HWND hwnd)
{
	m_hWnd = hwnd;
}

void GameInput::MouseMove(LPARAM lParam)
{     
	::GetCursorPos(&m_moveCursor);
	  
	if (m_moveCursor.x == m_moveOldCursor.x && m_moveCursor.y == m_moveOldCursor.y) return;

	if ((m_moveOldCursor.x == -1 && m_moveOldCursor.y == -1)) // 초기값의 경우
	{ 
		m_moveOldCursor = m_moveCursor; 
	}
	else
	{ 
		m_moveDeltaX = (float)(m_moveCursor.x - m_moveOldCursor.x) / m_DeltaValueX;
		m_moveDeltaY = (float)(m_moveCursor.y - m_moveOldCursor.y) / m_DeltaValueY; 
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
