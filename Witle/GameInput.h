#pragma once

// �̱��� ����
class GameInput
{
private:
	static UCHAR pKeyBuffer[256]; // Ű input�� ���� ����

public:
	GameInput();
	~GameInput();

	// static void Update();

	//// Ű�� ���ȴ��� Ȯ���ϴ� ����
	//static bool isKeyDownRIGHT() { return (pKeyBuffer[VK_RIGHT] & 0xF0); };
	//static bool isKeyDownLEFT() { return (pKeyBuffer[VK_LEFT] & 0xF0); };
	//static bool isKeyDownUP() { return (pKeyBuffer[VK_UP] & 0xF0); };
	//static bool isKeyDownDOWN() { return (pKeyBuffer[VK_DOWN] & 0xF0); };
};