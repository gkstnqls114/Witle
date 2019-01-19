#pragma once

// 싱글톤 패턴
class GameInput
{
private:
	static UCHAR pKeyBuffer[256]; // 키 input을 위한 변수

public:
	GameInput();
	~GameInput();

	// static void Update();

	//// 키가 눌렸는지 확인하는 상태
	//static bool isKeyDownRIGHT() { return (pKeyBuffer[VK_RIGHT] & 0xF0); };
	//static bool isKeyDownLEFT() { return (pKeyBuffer[VK_LEFT] & 0xF0); };
	//static bool isKeyDownUP() { return (pKeyBuffer[VK_UP] & 0xF0); };
	//static bool isKeyDownDOWN() { return (pKeyBuffer[VK_DOWN] & 0xF0); };
};