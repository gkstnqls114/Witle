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

// �̱��� ����
class GameInput
{
private:
	GameInput();
	~GameInput();

	static HWND m_hWnd;
	  
	static UCHAR m_pKeyBuffer[256]; // Ű������ input�� ���� ��� ����

	static const bool m_isDragRotate;

	static RAY m_PickingRay;
	static short m_WheelDelta; // ���콺 ���� ������ ����
	static const float m_DeltaValueX; // ���콺 �̵��� ��
	static const float m_DeltaValueY; // ���콺 �̵��� �� 

	static POINT m_downClickCursor; // �ѹ� Ŭ������ �� ��ġ
	static POINT m_downOldCursor; // ���� �����ӿ����� ���콺 ��ġ 
	static float m_downDeltaX; // ���콺�� ���� ���·� x������ ������ ���콺 �̵���  
	static float m_downDeltaY; // ���콺�� ���� ���·� y������ ������ ���콺 �̵���
	 
	static POINT m_moveCursor; // �ѹ� Ŭ������ �� ��ġ
	static POINT m_moveOldCursor; // ���� �����ӿ����� ���콺 ��ġ 
	static float m_moveDeltaX; // ���콺�� ������ ���� ���·� x������ ������ ���콺 �̵���
	static float m_moveDeltaY; // ���콺�� ������ ���� ���·� y������ ������ ���콺 �̵���

private:
	static void UpdateMouseDragRotate(HWND hWnd);
	static void UpdateMouseMoveRotate(HWND hWnd);
	 
	static float GetdownDeltaX() { return m_downDeltaX; }
	static float GetdownDeltaY() { return m_downDeltaY; }
	static float GetmoveDeltaX() { return m_moveDeltaX; };
	static float GetmoveDeltaY() { return m_moveDeltaY; };

public: 
	static void Update(HWND hWnd);
	static void Reset();

	static void SetHWND(HWND hwnd);
	static void MouseMove(LPARAM lParam);

	// Ű�� ���ȴ��� Ȯ���ϴ� ����
	static bool IsKeydownRIGHT() { return (m_pKeyBuffer[VK_RIGHT] & 0xF0); };
	static bool IsKeydownLEFT() { return (m_pKeyBuffer[VK_LEFT] & 0xF0); };
	static bool IsKeydownUP() { return (m_pKeyBuffer[VK_UP] & 0xF0); };
	static bool IsKeydownDOWN() { return (m_pKeyBuffer[VK_DOWN] & 0xF0); };
	static bool IsKeydownW() { return (m_pKeyBuffer[MYVK_W] & 0xF0); };
	static bool IsKeydownS() { return (m_pKeyBuffer[MYVK_S] & 0xF0); };
	static bool IsKeydownA() { return (m_pKeyBuffer[MYVK_A] & 0xF0); };
	static bool IsKeydownD() { return (m_pKeyBuffer[MYVK_D] & 0xF0); };

	static void SetCapture(HWND hWnd);
	static void RotateWheel(WPARAM wParam);
	static void ReleaseCapture();

	static float GetDeltaX() 
	{
		if (m_isDragRotate) return GetdownDeltaX();
		else  return GetmoveDeltaX();
	}
	static float GetDeltaY() 
	{
		if (m_isDragRotate) return GetdownDeltaY();
		else return GetmoveDeltaY();
	}


	static POINT GetdownClickcursor() { return m_downClickCursor; }


	static bool GenerateRayforPicking(const XMFLOAT3& cameraPos, const XMFLOAT4X4 & view, const XMFLOAT4X4 & projection, RAY& ray);

};
