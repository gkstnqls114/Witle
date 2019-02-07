#pragma once

//#define FRAME_BUFFER_WIDTH	960 //1920 / 2 (기준)
//#define FRAME_BUFFER_HEIGHT 540//1080 /2  (기준)

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