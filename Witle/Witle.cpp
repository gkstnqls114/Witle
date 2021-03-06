// LabProject02.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Witle.h"
#include "GameTimer.h" 
#include "GameScreen.h" 
#include "SoundManager.h"
#include "GameFramework.h"
 
//////////////////////////////// 전역변수
GameFramework gGameFramework;

MeshRenderer gMeshRenderer; 
//////////////////////////////// 전역변수

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

												// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                CalculateFrameStats(HWND hWnd); // 현재 프레임 레이트를 타이틀 화면에 출력한다.

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// 메모리 누수 디버깅 기능
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif  
	// _CrtSetBreakAlloc(6746); //해당 N번째 메모리 할당하는곳에 중단점으로 멈춘다.
	// _CrtSetBreakAlloc(229);
	// _CrtSetBreakAlloc(230);
	// _CrtSetBreakAlloc(231);
	//_CrtSetBreakAlloc(232);
	// _CrtSetBreakAlloc(233);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WITLE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WITLE));

	MSG msg;

	while (1)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			if (!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
		else
		{ 
			CGameTimer::GetInstance()->Tick(60.f); // fElapsedTime을 갱신한다. 
			
			// 게임 프레임 워크 내부에서 업데이트 이후 렌더한다.
			gGameFramework.UpdateGamelogic(); // ProcessInput, Update, LastUpdate 호출
			SoundManager::GetInstance()->Update();
			gGameFramework.Render();
			gGameFramework.Debug();

#ifdef CHECK_FRAMERATE
			CalculateFrameStats(msg.hwnd);
#endif

		}
	}
	gGameFramework.OnDestroy();

	// 싱글톤 패턴으로 선언한 것들 릴리즈
	CGameTimer::ReleaseInstance(); 

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_WITLE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//주 윈도우의 메뉴가 나타나지 않도록 한다.
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = ::LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
	 
#ifdef CHECK_SUBVIEWS
	RECT rc = { 0, 0, GameScreen::GetWidth() , GameScreen::GetHeight() + GameScreen::GetAnotherHeight() };
#else 
	RECT rc = { 0, 0, GameScreen::GetWidth(), GameScreen::GetHeight() };
#endif // CHECK_SUBVIEWS

	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_BORDER;
	AdjustWindowRect(&rc, dwStyle, FALSE);

	HWND hMainWnd = CreateWindow(szWindowClass, szTitle,
		dwStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top,
		NULL, NULL, hInstance, NULL);

	if (!hMainWnd) return(FALSE);

	gGameFramework.OnCreate(hInstance, hMainWnd);

	ShowWindow(hMainWnd, nCmdShow);
	UpdateWindow(hMainWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 어차피 해당 함수 안에서 message 값에 대해 처리해준다.
	// 예외처리는 혹시 몰라서 해둔 것
	if (message != WM_CREATE || message != WM_DESTROY) {
		gGameFramework.OnProcessingWindowMessage(hWnd, message, wParam, lParam);
	}

	switch (message)
	{ 
	case WM_CREATE:
		// 윈도우 생성 시 화면 중앙에 위치
		int x, y, width, height;
		RECT rtDesk, rtWindow;
		GetWindowRect(GetDesktopWindow(), &rtDesk);
		GetWindowRect(hWnd, &rtWindow);

		width = rtWindow.right - rtWindow.left;
		height = rtWindow.bottom - rtWindow.top;

		x = (rtDesk.right - width) / 2;
		y = (rtDesk.bottom - height) / 2;

		MoveWindow(hWnd, x, y, width, height, TRUE);

		// 출처: http://changmyeong.tistory.com/32 [개발노트]
		break; 
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void CalculateFrameStats(HWND hWnd)
{ 
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	++frameCnt;

	// Compute averages over one second period.
	if ((CGameTimer::GetInstance()->GetTotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wstring fpsStr = std::to_wstring(fps);
		std::wstring mspfStr = std::to_wstring(mspf);

		std::wstring windowText = szTitle;
		windowText +=
			L" ( fps: " + fpsStr +
			L" , mspf: " + mspfStr + L" ) ";

		SetWindowText(hWnd, windowText.c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}
