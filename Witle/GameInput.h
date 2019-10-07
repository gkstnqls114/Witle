#pragma once
// Virtual - Key Codes
#define MYVK_A 0x41
#define MYVK_B 0x42
#define MYVK_C 0x43
#define MYVK_D 0x44
#define MYVK_E 0x45
#define MYVK_F 0x46
#define MYVK_G 0x47 
#define MYVK_H 0x48 
#define MYVK_I 0x49 
#define MYVK_J 0x4A
#define MYVK_K 0x4B 
#define MYVK_L 0x4C 
#define MYVK_M 0x4D
#define MYVK_N 0x4E
#define MYVK_O 0x4F
#define MYVK_P 0x50
#define MYVK_Q 0x51
#define MYVK_R 0x52
#define MYVK_S 0x53
#define MYVK_T 0x54
#define MYVK_U 0x55
#define MYVK_V 0x56
#define MYVK_W 0x57
#define MYVK_X 0x58
#define MYVK_Y 0x59
#define MYVK_Z 0x5A

#define MOUSE_NONE -1 

#define KEYDOWN 1 // 해당 키가 한번 눌러졌다.
#define KEYUP   0 // 해당 키가 토글키 상태 ( 1에서 한번더 누름)
#define KEYUP_PRESSING   128 // 눌러진채로 떨어지지않은 상태
#define KEYDOWN_PRESSING   129 // 토글키인상태에서 떨어지지않은 상태

class Camera;

struct RAY
{
	XMFLOAT3 origin{0.f, 0.f, 0.f};
	XMFLOAT3 direction{0.f, 0.f, 0.f};

	static RAY GeneratePickingRay(const XMFLOAT3& cameraPos, const XMFLOAT2& ScreenPickingPoint, const XMFLOAT4X4 & view, const XMFLOAT4X4 & projection);
	static RAY GeneratePickingRay(const XMFLOAT2& ScreenPickingPoint, Camera* pCamera);
	static XMFLOAT3 GeneratePickingScreenOrigin(const XMFLOAT3& cameraPos, const XMFLOAT2& ScreenPickingPoint, const XMFLOAT4X4 & view, const XMFLOAT4X4 & projection);
};

// 싱글톤 패턴
class GameInput
{
private:
	GameInput();
	~GameInput();

	static bool m_gameActive;

	static HWND m_hWnd;
	  
	static UCHAR m_pKeyBuffer[256]; // 키보드의 input(지속)을 위한 멤버 변수

	static bool m_DragMode; 

	static RAY m_PickingRay;
	static short m_WheelDelta; // 마우스 휠이 움직인 정도
	static float m_DeltaValueX; // 마우스 이동량을 나눈 값
	static float m_DeltaValueY; // 마우스 이동량을 나눈 값 

	static POINT m_downClickCursor; // 한번 클릭했을 때 위치
	static POINT m_downOldCursor; // 이전 프레임에서의 마우스 위치 
	static float m_downDeltaX; // 마우스를 누른 상태로 x축으로 움직인 마우스 이동량  
	static float m_downDeltaY; // 마우스를 누른 상태로 y축으로 움직인 마우스 이동량
	 
private:
	static void UpdateMouseDragRotate(HWND hWnd);
	static void UpdateMouseMoveRotate(HWND hWnd);
	 
	static float GetdownDeltaX() { return m_downDeltaX; }
	static float GetdownDeltaY() { return m_downDeltaY; }
	 
public: 
	static void Update(HWND hWnd); 

	static void Reset();

	static void ChagneDragMode() 
	{
		m_DeltaValueX = 10.f;
		m_DeltaValueY = 10.f;
		m_DragMode = true; 
	};
	static void ChagneMoveMode()
	{
		m_DeltaValueX = 5.f;
		m_DeltaValueY = 5.f;
		m_DragMode = false;
	};

	static void Stop();
	static void Start();
	static void SetHWND(HWND hwnd);
 	 
	static bool GetGameActive() { return m_gameActive; };
	 
	//// Keyboard 관련 ///////////////////////////////////////////
	// 키가 눌렸는지 확인하는 상태
	static bool IsKeydownRIGHT() { return (m_pKeyBuffer[VK_RIGHT] & 0xF0); };
	static bool IsKeydownLEFT() { return (m_pKeyBuffer[VK_LEFT] & 0xF0); };
	static bool IsKeydownUP() { return (m_pKeyBuffer[VK_UP] & 0xF0); };
	static bool IsKeydownDOWN() { return (m_pKeyBuffer[VK_DOWN] & 0xF0); };
	static bool IsKeydownW() { return (m_pKeyBuffer[MYVK_W] & 0xF0); };
	static bool IsKeydownS() { return (m_pKeyBuffer[MYVK_S] & 0xF0); };
	static bool IsKeydownA() { return (m_pKeyBuffer[MYVK_A] & 0xF0); };
	static bool IsKeydownD() { return (m_pKeyBuffer[MYVK_D] & 0xF0); };
	static bool IsKeydownE() { return (m_pKeyBuffer[MYVK_E] & 0xF0); };
	static bool IsKeydownSpace() { return (m_pKeyBuffer[VK_SPACE] & 0xF0); };
	//// Keyboard 관련 ///////////////////////////////////////////


	//// Mouse 관련 ///////////////////////////////////////////
	static float GetDeltaX()
	{
		return GetdownDeltaX();
	}
	static float GetDeltaY()
	{
		return GetdownDeltaY(); 
	}
	static bool GetDragMode() { return m_DragMode; }
	 
	static bool GenerateRayforPicking(const XMFLOAT3& cameraPos, const XMFLOAT4X4 & view, const XMFLOAT4X4 & projection, RAY& ray);
	//// Mouse 관련 ///////////////////////////////////////////


	
};
