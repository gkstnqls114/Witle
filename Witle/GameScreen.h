#pragma once

class Camera;

class GameScreen
{
private:
	GameScreen();
	~GameScreen();

	static float m_ClientWidth;
	static float m_ClientHeight;

	static float m_Width;
	static float m_Height;

	static float m_AnotherWidth;
	static float m_AnotherHeight;
	 
	static IDXGISwapChain* m_pSwapChain;

public:

	static float GetWidth() { return m_Width; }
	static float GetHeight() { return m_Height; }
	static float GetAnotherWidth() { return m_AnotherWidth; }
	static float GetAnotherHeight() { return m_AnotherHeight; }
	static float GetClientWidth() { return m_ClientWidth; }
	static float GetClientHeight() { return m_ClientHeight; }
	static bool ChangeWindowScreen(float width, float height);
	static bool ChangeFullScreen(float width, float height);
	static void SetScreen(float width, float height) {
		m_ClientWidth = width;  m_ClientHeight = height;
	};
	 
	static void SetSwapChain(IDXGISwapChain* pSwapChain) { m_pSwapChain = pSwapChain; }
};