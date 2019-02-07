#pragma once

class GameScreen
{
private:
	GameScreen();
	~GameScreen();

	static UINT m_Width;
	static UINT m_Height;

public:
	static UINT GetWidth() { return m_Width; }
	static UINT GetHeight() { return m_Height; }

};