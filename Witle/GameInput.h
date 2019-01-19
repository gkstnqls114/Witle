#pragma once

// 싱글톤 패턴
class GameInput
{
private:
	GameInput();
	~GameInput();
	
	static UCHAR m_pKeyBuffer[256]; // 키보드의 input을 위한 멤버 변수

	static float m_cDeltaX; // 마우스를 누른 상태로 x축으로 움직인 마우스 이동량  
	static float m_cDeltaY; // 마우스를 누른 상태로 y축으로 움직인 마우스 이동량
	static POINT m_oldCursor; // 이전 프레임에서의 마우스 위치

public: 
	static void Update(HWND hWnd);

	// 키가 눌렸는지 확인하는 상태
	static bool IsKeydownRIGHT() { return (m_pKeyBuffer[VK_RIGHT] & 0xF0); };
	static bool IsKeydownLEFT() { return (m_pKeyBuffer[VK_LEFT] & 0xF0); };
	static bool IsKeydownUP() { return (m_pKeyBuffer[VK_UP] & 0xF0); };
	static bool IsKeydownDOWN() { return (m_pKeyBuffer[VK_DOWN] & 0xF0); };

	static void SetCapture(HWND hWnd);
	static void ReleaseCapture();
	static float GetcDeltaX() { return m_cDeltaX; }
	static float GetcDeltaY() { return m_cDeltaY; }

};