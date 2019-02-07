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

struct RAY
{
	XMFLOAT3 origin{0.f, 0.f, 0.f};
	XMFLOAT3 direction{0.f, 0.f, 0.f};
};

// 싱글톤 패턴
class GameInput
{
private:
	GameInput();
	~GameInput();

	static UCHAR m_pKeyBuffer[256]; // 키보드의 input을 위한 멤버 변수

	static RAY m_PickingRay;

	static POINT m_clickCursor; // 한번 클릭했을 때 위치
	static const float m_DeltaValue; // 마우스 이동량 값
	static float m_cDeltaX; // 마우스를 누른 상태로 x축으로 움직인 마우스 이동량  
	static float m_cDeltaY; // 마우스를 누른 상태로 y축으로 움직인 마우스 이동량
	static POINT m_oldCursor; // 이전 프레임에서의 마우스 위치 
	static short m_WheelDelta; // 마우스 휠이 움직인 정도

public: 
	static void Update(HWND hWnd);

	// 키가 눌렸는지 확인하는 상태
	static bool IsKeydownRIGHT() { return (m_pKeyBuffer[VK_RIGHT] & 0xF0); };
	static bool IsKeydownLEFT() { return (m_pKeyBuffer[VK_LEFT] & 0xF0); };
	static bool IsKeydownUP() { return (m_pKeyBuffer[VK_UP] & 0xF0); };
	static bool IsKeydownDOWN() { return (m_pKeyBuffer[VK_DOWN] & 0xF0); };
	static bool IsKeydownW() { return (m_pKeyBuffer[MYVK_W] & 0xF0); };
	static bool IsKeydownS() { return (m_pKeyBuffer[MYVK_S] & 0xF0); };

	static void SetCapture(HWND hWnd);
	static void RotateWheel(WPARAM wParam);
	static void ReleaseCapture();
	static float GetcDeltaX() { return m_cDeltaX; }
	static float GetcDeltaY() { return m_cDeltaY; }

	static RAY GenerateRayforPicking(const XMFLOAT3& cameraPos, const XMFLOAT4X4& view, const XMFLOAT4X4 & projection);

};
