#pragma once
#include "GameObject.h"

class Camera;
class CLoadedModelInfo;
class MyCollider;
class LoadObject;
class MyBOBox;
class PlayerStatus;
class  Status;
class PlayerMovement;
class Broom;
class Sniping;
class Texture;
class MyDescriptorHeap; 
class BroomEffectRect;
class MyRectangle;
class PlayerActionMgr;

// CHeightMapTerrain 입니다.
class Player :
	public GameObject
{
public:
	int m_PrevAnimation{ 0 }; // 현재 사용하는 애니메이션
	int m_CurrAnimation{ 0 }; // 현재 사용하는 애니메이션
	 
private:
	PlayerActionMgr* m_PlayerActionMgr{ nullptr }; // Player의 액션(모델의 동작)을 관리

	bool isDead{ false };

	bool m_isRendering{ true };
	bool m_isAttacking{ false };
	

	PlayerMovement*    m_pPlayerMovement{ nullptr };
	PlayerStatus*	   m_pPlayerHPStatus{ nullptr };
	PlayerStatus*	   m_pPlayerMPStatus{ nullptr };
	MyBOBox*		   m_pMyBOBox{ nullptr };

	Texture*			m_pTexture_Cloth{ nullptr };
	Texture*			m_pTexture_Body{ nullptr };

	// Player 모델 정보

	CLoadedModelInfo*  m_PlayerModel_Cloth{ nullptr };
	CLoadedModelInfo*  m_PlayerModel_Body{nullptr };
	LoadObject*		   m_pLoadObject_Cloth{ nullptr };
	LoadObject*		   m_pLoadObject_Body{ nullptr };

	// Player 모델 정보

	Broom* m_Broom{ nullptr };
	Sniping* m_pSniping{ nullptr };

	BroomEffectRect*   m_BroomEffectRect{ nullptr };
	MyRectangle*       m_BroomLineEffectRect{ nullptr };
	 
	LPVOID m_pPlayerUpdatedContext { nullptr };
	LPVOID m_pCameraUpdatedContext { nullptr };

public:
	float shield = 0.f; // 경감데미지 
	float blessing = 1.f; // n배의속도

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;

public:
	void SetAnimationState(int state);

	virtual void Update(float fElapsedTime) override;

private:
	void OnPlayerUpdateCallback(float fTimeElapsed);
	void OnCameraUpdateCallback(float fTimeElapsed, Camera* camera);
	void SetTrackAnimationSet();

public:
	Player(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL);
	virtual ~Player();

	void Init();

	void ActiveShield();
	void DeactiveShield();

	void ActiveBlessing();
	void DeactiveBlessing();

	void LastUpdate(float);

	void SetSniping(Sniping* sniping) { m_pSniping = sniping; }
	void DoNotRendering() { m_isRendering = false; }
	void DoRendering() { m_isRendering = true; }

	XMFLOAT3 CalculateAlreadyVelocity(float fTimeElapsed);
	BoundingOrientedBox CalculateAlreadyBoundingBox(float fTimeElapsed);
	XMFLOAT3 CalculateAlreadyPosition(float fTimeElapsed);

	void SubstractHP(int sub);
	void Animate(float fElapsedTime);
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool);
	void RenderForShadow(ID3D12GraphicsCommandList *pd3dCommandList);
	void RenderStatus(ID3D12GraphicsCommandList *pd3dCommandList, bool); // 체력
	
	void Move(const XMFLOAT3& xmf3Shift);
	void Move(const XMFLOAT3& xmf3Shift, DWORD dir);
	void Rotate(float x, float y, float z);

	void ProcessInput(float fTimeElapsed);
	void ProcessInputAI(float fTimeElapsed);

	bool Attack(Status* status, MyCollider* collider, XMFLOAT2 aimPoint ,Camera* pMainCaemra, bool);

	/////////////////////// Skill
	void UseSkill_Broom();
	/////////////////////// Skill

	/////////////////////// Get
	const Broom* GetpBroom() { return m_Broom; }
	PlayerStatus* GetHPStatus() { return m_pPlayerHPStatus; };
	PlayerStatus* GetMPStatus() { return m_pPlayerMPStatus; };
	AXIS GetCoorAxis() { return m_Transform.GetCoorAxis(); }
	XMFLOAT3 GetVelocity() const;
	MyBOBox* GetBOBox() { return m_pMyBOBox; }
	const Broom* GetBroom() const { return m_Broom; }
	bool IsAttacking() const { return m_isAttacking; }
	/////////////////////// Get

	/////////////////////// Set	
	void isAttacking()  {
		m_isAttacking = true; }

	void SetVelocity(const XMFLOAT3& velocity);
	void SetUpdatedContext(LPVOID pContext)
	{ 
		m_pCameraUpdatedContext = pContext;
		m_pPlayerUpdatedContext = pContext;
	} 
	/////////////////////// Set
};