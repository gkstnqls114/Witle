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

public:
	static UINT GetWidth() { return m_Width; }
	static UINT GetHeight() { return m_Height; }
	static bool ChangeScreen(UINT width, UINT height);
	
	static void SetCamera(Camera* pCamera) { m_pMainCamera = pCamera; }
};