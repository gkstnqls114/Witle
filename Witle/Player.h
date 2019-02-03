#pragma once
#include "GameObject.h"

class Camera;

// CHeightMapTerrain �Դϴ�.
class Player :
	public GameObject
{
	XMFLOAT3 m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_xmf3Gravity = XMFLOAT3(0.0f, -10.0f, 0.0f); 
	float m_fMaxVelocityXZ = 300.0f;
	float m_fMaxVelocityY = 400.0f;
	float m_fFriction = 25.0f;

	// ȸ����
	float m_fPitch = 0.0f;
	float m_fRoll = 0.0f;
	float m_fYaw = 0.0f;

	LPVOID m_pPlayerUpdatedContext { nullptr };
	LPVOID m_pCameraUpdatedContext { nullptr };

protected:
	virtual void ReleaseMembers() override {};
	virtual void ReleaseMemberUploadBuffers() override {};

private:
	void OnPlayerUpdateCallback(float fTimeElapsed);
	void OnCameraUpdateCallback(float fTimeElapsed, Camera* camera);

public:
	Player(const std::string& entityID);
	virtual ~Player();

	virtual void Update(float fElapsedTime) override; 

	void VelocityMove(const XMFLOAT3& vMove);

	/////////////////////// Get
	XMFLOAT3 GetVelocity() const { return m_xmf3Velocity; };
	/////////////////////// Get

	/////////////////////// Set	
	void SetPlayerUpdatedContext(LPVOID pContext) { 
		m_pCameraUpdatedContext = pContext;
			m_pPlayerUpdatedContext = pContext; }
	/////////////////////// Set
};