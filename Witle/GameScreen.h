#pragma once

class Camera;

class GameScreen
{
private:
	GameScreen();
	~GameScreen();

	static UINT m_ClientWidth;
	static UINT m_ClientHeight;

	static UINT m_Width;
	static UINT m_Height;

	static UINT m_AnotherWidth;
	static UINT m_AnotherHeight;
	 
	static IDXGISwapChain* m_pSwapChain;

public:

	static UINT GetWidth() { return m_Width; }
	static UINT GetHeight() { return m_Height; }
	static UINT GetAnotherWidth() { return m_AnotherWidth; }
	static UINT GetAnotherHeight() { return m_AnotherHeight; }
	static UINT GetClientWidth() { return m_ClientWidth; }
	static UINT GetClientHeight() { return m_ClientHeight; }
	static bool ChangeWindowScreen(UINT width, UINT height);
	static bool ChangeFullScreen(UINT width, UINT height);
	static void SetScreen(UINT width, UINT height) {
		m_ClientWidth = width;  m_ClientHeight = height;
	};
	 
	static void SetSwapChain(IDXGISwapChain* pSwapChain) { m_pSwapChain = pSwapChain; }
};