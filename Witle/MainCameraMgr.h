#pragma once

class CameraObject;

// ������ ���� ��ü������ ȭ�鿡 �׷����� ī�޶� ������
class MainCameraMgr
{
private: 
	static CameraObject* m_pMainCamera;

public:
	static void SetMainPlayer(CameraObject* player)
	{ 
		m_pMainCamera = player;
	}
	static const CameraObject* GetMainCamera() { return m_pMainCamera; }
};