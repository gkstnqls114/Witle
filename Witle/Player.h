#pragma once
#include "GameObject.h"

class Camera;
class CLoadedModelInfo;
class LoadObject;
class MyBOBox;
class PlayerStatus;
class PlayerMovement;
class Broom;
class Sniping;

// CHeightMapTerrain 입니다.
class Player :
	public GameObject
{
	bool m_isRendering{ true };
	
	// 공격 시 해당 범위안에 충돌박스

	PlayerMovement*    m_pPlayerMovement{ nullptr };
	PlayerStatus*	   m_pPlayerStatus{ nullptr };
	MyBOBox*		   m_pMyBOBox{ nullptr };
	CLoadedModelInfo*  m_PlayerModel {nullptr };
	LoadObject*		   m_pLoadObject{ nullptr }; // delete 금지
	 
	Broom* m_pBroom{ nullptr };
	Sniping* m_pSniping{ nullptr };
	 
	LPVOID m_pPlayerUpdatedContext { nullptr };
	LPVOID m_pCameraUpdatedContext { nullptr };

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;

public:
	virtual void Update(float fElapsedTime) override;

private:
	void OnPlayerUpdateCallback(float fTimeElapsed);
	void OnCameraUpdateCallback(float fTimeElapsed, Camera* camera);

public:
	Player(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL);
	virtual ~Player();

	void SetSniping(Sniping* sniping) { m_pSniping = sniping; }
	void DoNotRendering() { m_isRendering = false; }
	void DoRendering() { m_isRendering = true; }

	XMFLOAT3 CalculateAlreadyVelocity(float fTimeElapsed);
	BoundingOrientedBox CalculateAlreadyBoundingBox(float fTimeElapsed);
	XMFLOAT3 CalculateAlreadyPosition(float fTimeElapsed);

	void SubstractHP(int sub);
	void Animate(float fElapsedTime);
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);
  
	void Move(ULONG nDirection, float fDistance, bool bVelocity = false);
	void SetTrackAnimationSet(ULONG nDirection);
	void Move(const XMFLOAT3& xmf3Shift);
	void MoveVelocity(const XMFLOAT3& xmf3Shift);
	void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);
	void Rotate(float x, float y, float z);

	void ProcessInput(float fTimeElapsed);

	/////////////////////// Skill
	void UseSkill_Broom();
	/////////////////////// Skill

	/////////////////////// Get
	AXIS GetCoorAxis() { return m_Transform.GetCoorAxis(); }
	XMFLOAT3 GetVelocity() const;
	MyBOBox* GetBOBox() { return m_pMyBOBox; }
	/////////////////////// Get

	/////////////////////// Set	
	void SetVelocity(const XMFLOAT3& velocity);
	void SetUpdatedContext(LPVOID pContext)
	{ 
		m_pCameraUpdatedContext = pContext;
		m_pPlayerUpdatedContext = pContext;
	} 
	/////////////////////// Set
};