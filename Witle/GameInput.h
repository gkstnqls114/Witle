#pragma once

// �̱��� ����
class GameInput
{
private:
	GameInput();
	~GameInput();
	
	static UCHAR m_pKeyBuffer[256]; // Ű������ input�� ���� ��� ����

	static float m_cDeltaX; // ���콺�� ���� ���·� x������ ������ ���콺 �̵���  
	static float m_cDeltaY; // ���콺�� ���� ���·� y������ ������ ���콺 �̵���
	static POINT m_oldCursor; // ���� �����ӿ����� ���콺 ��ġ

public: 
	static void Update(HWND hWnd);

	// Ű�� ���ȴ��� Ȯ���ϴ� ����
	static bool IsKeydownRIGHT() { return (m_pKeyBuffer[VK_RIGHT] & 0xF0); };
	static bool IsKeydownLEFT() { return (m_pKeyBuffer[VK_LEFT] & 0xF0); };
	static bool IsKeydownUP() { return (m_pKeyBuffer[VK_UP] & 0xF0); };
	static bool IsKeydownDOWN() { return (m_pKeyBuffer[VK_DOWN] & 0xF0); };

	static void SetCapture(HWND hWnd);
	static void ReleaseCapture();
	static float GetcDeltaX() { return m_cDeltaX; }
	static float GetcDeltaY() { return m_cDeltaY; }

};