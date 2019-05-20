#pragma once
#include "GameObject.h"

class Camera;
class CLoadedModelInfo;
class LoadObject;
class MyBOBox;
class MonsterStatus;
class MonsterMovement;
class Broom;
class Sniping;
class Texture;
class MyDescriptorHeap;
class BroomEffectRect;
class MyRectangle;

// CHeightMapTerrain 입니다.
class Monster :
	public GameObject
{
protected:
	static GameObject*  m_pTarget;

protected:
	MonsterMovement*    m_MonsterMovement{ nullptr };
	MonsterStatus*	   m_MonsterStatus{ nullptr };
	MyBOBox*		   m_pMyBOBox{ nullptr };

	Texture*			m_pTexture{ nullptr };
	MyDescriptorHeap*	m_pHaep{ nullptr };

	CLoadedModelInfo*  m_MonsterModel{ nullptr };
	LoadObject*		   m_pLoadObject{ nullptr };

private:
	int m_CurrAnimation{ 0 }; // 현재 사용하는 애니메이션
	bool isDead{ false };

	bool m_isRendering{ true };
	bool m_isAttacking{ false };

	int m_PrevAnimation{ 0 }; // 현재 사용하는 애니메이션

	 

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;

public:
	static void SetTarget(GameObject* player) { m_pTarget = player; }
	virtual void Update(float fElapsedTime) = 0;

private:
	void OnMonsterUpdateCallback(float fTimeElapsed);
	void OnCameraUpdateCallback(float fTimeElapsed, Camera* camera);
	void SetTrackAnimationSet();

public:
	Monster(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL);
	virtual ~Monster();
	 
	void DoNotRendering() { m_isRendering = false; }
	void DoRendering() { m_isRendering = true; }

	XMFLOAT3 CalculateAlreadyVelocity(float fTimeElapsed);
	BoundingOrientedBox CalculateAlreadyBoundingBox(float fTimeElapsed);
	XMFLOAT3 CalculateAlreadyPosition(float fTimeElapsed);

	void SubstractHP(int sub);
	void Animate(float fElapsedTime);
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);
	void RenderHpStatus(ID3D12GraphicsCommandList *pd3dCommandList);

	void Move(ULONG nDirection, float fDistance, bool bVelocity = false);
	void Move(const XMFLOAT3& xmf3Shift);
	void MoveVelocity(const XMFLOAT3& xmf3Shift);
	void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);
	void Rotate(float x, float y, float z);

	void ProcessInput(float fTimeElapsed);
	void ProcessInputAI(float fTimeElapsed);

	bool Attack();

	/////////////////////// Skill
	void UseSkill_Broom();
	/////////////////////// Skill

	/////////////////////// Get
	AXIS GetCoorAxis() { return m_Transform.GetCoorAxis(); }
	XMFLOAT3 GetVelocity() const;
	MyBOBox* GetBOBox() { return m_pMyBOBox; } 
	bool IsAttacking() const { return m_isAttacking; }
	/////////////////////// Get

	/////////////////////// Set	
	void SetVelocity(const XMFLOAT3& velocity); 
	/////////////////////// Set
};