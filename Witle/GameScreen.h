#pragma once

class Camera;

class GameScreen
{
private:
	GameScreen();
	~GameScreen();

	static UINT m_Width;
	static UINT m_Height;
	static Camera* m_pMainCamera;
	static IDXGISwapChain* m_pSwapChain;

public:
	static UINT GetWidth() { return m_Width; }
	static UINT GetHeight() { return m_Height; }
	static bool ChangeWindowScreen(UINT width, UINT height);
	static bool ChangeFullScreen(UINT width, UINT height);
	static void SetScreen(UINT width, UINT height) { m_Width = width;  m_Height = height; };
	
	static void SetCamera(Camera* pCamera) { m_pMainCamera = pCamera; }
	static void SetSwapChain(IDXGISwapChain* pSwapChain) { m_pSwapChain = pSwapChain; }
};