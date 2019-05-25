#pragma once

class CameraObject;

// 실제로 현재 전체적으로 화면에 그려지는 카메라를 관리함
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