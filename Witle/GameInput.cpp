//-----------------------------------------------------------------------------
// File: GameInput.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameScreen.h"
#include "GameObject.h"
#include "Camera.h"
#include "GameInput.h"

HWND GameInput::m_hWnd{ NULL };

UCHAR GameInput::m_pKeyBuffer[256]; 
bool GameInput::m_DragMode{ false };
bool GameInput::m_gameActive{ false };

bool GameInput::m_Moving{ false };
 
float GameInput::m_downDeltaX = 0.0f; // 마우스를 누른 상태로 x축으로 움직인 마우스 이동량  
float GameInput::m_downDeltaY = 0.0f; // 마우스를 누른 상태로 y축으로 움직인 마우스 이동량
POINT GameInput::m_downOldCursor; // 이전 프레임에서의 마우스 위치
POINT GameInput::m_downClickCursor{ -1, -1 }; // 한번 클릭했을 때 위치

float GameInput::m_DeltaValueX = 10.0f; // 마우스 이동량 값 
float GameInput::m_DeltaValueY = 10.0f; // 마우스 이동량 값 
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
	
	ray = RAY::GeneratePickingRay(cameraPos, XMFLOAT2(m_downClickCursor.x, m_downClickCursor.y), view, projection);
	 
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

void GameInput::FixMouseInApp(HWND hWnd)
{
	RECT rc;
	POINT p1, p2;

	GetClientRect(hWnd, &rc);    // 클라이언트 크기

	// 클라이언트 크기를 좌표로 변환
	p1.x = rc.left;
	p1.y = rc.top;
	p2.x = rc.right;
	p2.y = rc.bottom;

	// 클라이언트 크기를 스크린 크기로 변환
	ClientToScreen(hWnd, &p1);
	ClientToScreen(hWnd, &p2);

	rc.left = p1.x;
	rc.top = p1.y;
	rc.right = p2.x;
	rc.bottom = p2.y;

	//해당 좌표를 기준으로 커서를 고정
	ClipCursor(&rc); 
}

void GameInput::FixMouseInWindows(HWND hWnd)
{ 
	//해당 좌표를 기준으로 커서를 고정
	ClipCursor(NULL);
}

POINT GameInput::GetAppCenter(HWND hWnd)
{
	POINT windowCenter{ GameScreen::GetWidth() / 2 , GameScreen::GetHeight() / 2 };
	ClientToScreen(hWnd, &windowCenter);

	return windowCenter;
}

void GameInput::SetCursorAppCenter(HWND hWnd)
{
	POINT appCenter = GetAppCenter(hWnd);
	::SetCursorPos(appCenter.x, appCenter.y);
}

XMFLOAT2 GameInput::MouseMoveForGameScene(HWND hWnd, const GameScene* const pGameScene)
{
	XMFLOAT2 delta{ 0.f, 0.f };

	if (!m_gameActive) return delta;
	if (m_DragMode) return delta;

	POINT appCenter = GameInput::GetAppCenter(hWnd);

	POINT CurrCursor;
	::GetCursorPos(&CurrCursor);

	bool SameCurrAppCenter = CurrCursor.x == appCenter.x && CurrCursor.y == appCenter.y;
	if (SameCurrAppCenter) return delta;

	// 플레이어 회전에 대한 처리
	delta.x = float(CurrCursor.x - appCenter.x) / m_DeltaValueX;
	delta.y = float(CurrCursor.y - appCenter.y) / m_DeltaValueY;

	SetCursorAppCenter(hWnd);

	return delta;
}

void GameInput::Update(HWND hWnd)
{ 
	// 키보드의 pKeyBuffer를 구한다.
	::GetKeyboardState(m_pKeyBuffer);

	if (m_DragMode)
	{ 
		UpdateMouseDragRotate(hWnd);
	} 
	else
	{
		//마우스 커서를 화면에서 없앤다(보이지 않게 한다).
		::SetCursor(NULL);
	}
}
 
void GameInput::Reset()
{
	m_downClickCursor.x = MOUSE_NONE;
	m_downClickCursor.y = MOUSE_NONE;
}

void GameInput::ChagneDragMode()
{
	m_DeltaValueX = 10.f;
	m_DeltaValueY = 10.f;
	m_DragMode = true;

	if (m_hWnd != NULL)
	{
		FixMouseInWindows(m_hWnd);
	}
}

void GameInput::ChagneMoveMode()
{
	m_DeltaValueX = 20.f;
	m_DeltaValueY = 20.f;
	m_DragMode = false;
	
	if (m_hWnd != NULL)
	{
		FixMouseInApp(m_hWnd);
		SetCursorAppCenter(m_hWnd);
	}
}

void GameInput::Stop(HWND hwnd)
{
	m_gameActive = false;
}

void GameInput::Start(HWND hwnd)
{
	m_gameActive = true;
}

void GameInput::SetHWND(HWND hwnd)
{
	m_hWnd = hwnd;
}
 
RAY RAY::GeneratePickingRay(const XMFLOAT3 & cameraPos, const XMFLOAT2 & ScreenPickingPoint, const XMFLOAT4X4 & view, const XMFLOAT4X4 & projection)
{
	RAY ray;

	ray.origin = cameraPos;

	// 왼쪽 상단이 (0, 0)로 표현되어있는 윈도우 좌표계의 클릭 커서를
	// 화면 중심이 (0, 0)이며 각 축이 -1~1 로 표현되는, 즉 투영 좌표계로 변경한다.  
	ray.direction.x = float(((2.0f * ScreenPickingPoint.x) / float(GameScreen::GetWidth())) - 1.0f);
	ray.direction.y = float((-(2.0f * ScreenPickingPoint.y) / float(GameScreen::GetHeight())) + 1.0f);

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

	return ray;
}

RAY RAY::GeneratePickingRay(const XMFLOAT2 & ScreenPickingPoint, Camera * pCamera)
{
	RAY ray;

	ray.origin = pCamera->GetpOwner()->GetTransform().GetPosition();

	// 왼쪽 상단이 (0, 0)로 표현되어있는 윈도우 좌표계의 클릭 커서를
	// 화면 중심이 (0, 0)이며 각 축이 -1~1 로 표현되는, 즉 투영 좌표계로 변경한다.  
	ray.direction.x = float(((2.0f * ScreenPickingPoint.x) / float(GameScreen::GetWidth())) - 1.0f);
	ray.direction.y = float((-(2.0f * ScreenPickingPoint.y) / float(GameScreen::GetHeight())) + 1.0f);

	//현재는 투영좌표계의 Direction 좌표
	//즉 Direction의 x, y좌표는 -1과 1사이이다.

	// 클릭 좌표를 투영좌표계에서 카메라 좌표계로 변경
	ray.direction.x = ray.direction.x / pCamera->GetProjectionMatrix()._11;
	ray.direction.y = ray.direction.y / pCamera->GetProjectionMatrix()._22;
	ray.direction.z = 1.0f;

	// 현재 카메라 좌표계...

	// 카메라 좌표계에서 월드 좌표계로 변환
	XMFLOAT4X4 InverseView = Matrix4x4::Inverse(pCamera->GetViewMatrix());
	ray.direction = Vector3::TransformCoord(ray.direction, InverseView);

	// 카메라 위치에서부터 방향 벡터를 구한다
	ray.direction = Vector3::Normalize(Vector3::Subtract(ray.direction, ray.origin));

	return ray;
}

XMFLOAT3 RAY::GeneratePickingScreenOrigin(const XMFLOAT3 & cameraPos, const XMFLOAT2 & ScreenPickingPoint, const XMFLOAT4X4 & view, const XMFLOAT4X4 & projection)
{ 
	XMFLOAT3 origin = cameraPos;
	XMFLOAT3 direction;

	// 왼쪽 상단이 (0, 0)로 표현되어있는 윈도우 좌표계의 클릭 커서를
	// 화면 중심이 (0, 0)이며 각 축이 -1~1 로 표현되는, 즉 투영 좌표계로 변경한다.  
	direction.x = float(((2.0f * ScreenPickingPoint.x) / float(GameScreen::GetWidth())) - 1.0f);
	direction.y = float((-(2.0f * ScreenPickingPoint.y) / float(GameScreen::GetHeight())) + 1.0f);

	//현재는 투영좌표계의 Direction 좌표
	//즉 Direction의 x, y좌표는 -1과 1사이이다.

	// 클릭 좌표를 투영좌표계에서 카메라 좌표계로 변경
	direction.x = direction.x / projection._11;
	direction.y = direction.y / projection._22;
	direction.z = 1.0f;

	// 현재 카메라 좌표계...

	// 카메라 좌표계에서 월드 좌표계로 변환
	XMFLOAT4X4 InverseView = Matrix4x4::Inverse(view);
	direction = Vector3::TransformCoord(direction, InverseView);

	return direction;
}
 