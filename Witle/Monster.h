#pragma once
#include "GameObject.h"

class Camera;
class CLoadedModelInfo;
class LoadObject;
class MyBOBox;
class RecognitionRange;
class MyDescriptorHeap;
class Texture;
class LineMesh;
class UI3DImage;
class MyRectangle;

class MonsterStatus;
class MonsterMovement;
class ActionMgr;

enum MONSTER_STATE
{
	IDEL,
	MOVE,
	ATTACK,
	DIE,
	SEARCH,
	CHASE
};

class Monster :
	public GameObject
{
public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override;
	virtual void RenderForShadow(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual int GetAnimationHitID() = 0;
	 
public:
	virtual void Update(float fElapsedTime);
	virtual void UpdateState(float fElapsedTime);

protected: 
	ActionMgr*   m_MonsterActionMgr{ nullptr };

	UI3DImage*			m_MonsterHPUI{ nullptr };
	RecognitionRange*   m_RecognitionRange{ nullptr };
	MonsterMovement*    m_MonsterMovement{ nullptr };
	MonsterStatus*	    m_MonsterHPStatus{ nullptr };
	MyBOBox*		    m_pMyBOBox{ nullptr };
	 
	Texture*			m_pTexture{ nullptr }; 

	CLoadedModelInfo*  m_MonsterModel{ nullptr };
	LoadObject*		   m_pLoadObject{ nullptr };

	bool m_isAttacking{ false };

	// 플레이어와 공격이 충돌되면 true 함
	bool m_isFinishAttack{ false };

	// 해당 아래 변수 시간이 되면 m_isFinishAttack = false가 됨
	const float m_FinishAttackUnlockTime{ 3.f };

	// 해당 아래 변수 시간을 통해 관리한다.
	float m_TotalTime{ 0.f };

	XMFLOAT3 m_SpawnPoint;
	const float    m_SpawnRange{ 500.f };
	const float m_HpBarY{ 200.f }; 
	 
#ifdef SHOW_DEBUGMESH
	GameObject * m_pDebugObject{ nullptr };

	// Spawn Point 주위로 Range를 확인하기 위해 임시로 설정
	LineMesh* m_pDebugSpawnMesh{ nullptr }; 
#endif // SHOW_DEBUGMESH

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override; 
	void Init(bool isBoss);

public:  
	// float HpBarY: HP Bar UI의 Y 값 위치
	Monster(const std::string& entityID, float spawnRange, const XMFLOAT3& SpwanPoint, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, float attackanimeTime,float HpBarY = 200.f);
	virtual ~Monster();
	
	virtual void Init() = 0;
	 
	void RenderHpStatus(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);

	XMFLOAT3 CalculateAlreadyVelocity(float fTimeElapsed);
	BoundingOrientedBox CalculateAlreadyBoundingBox(float fTimeElapsed);
	XMFLOAT3 CalculateAlreadyPosition(float fTimeElapsed);

	virtual void SubstractHP(int sub);
	virtual void Animate(float fElapsedTime);

	void RenderDebug(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);

	void Move(ULONG nDirection, float fDistance, bool bVelocity = false);
	void Move(const XMFLOAT3& xmf3Shift);
	void MoveVelocity(const XMFLOAT3& xmf3Shift);
	void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);
	virtual void Rotate(float x, float y, float z);

	//// Get /////////////////// 
	virtual Movement* const GetpMovement() const override;

	virtual MyBOBox* GetBOBox() const { return m_pMyBOBox; }
	ActionMgr* GetActionMgr() const { return m_MonsterActionMgr; }
	MonsterMovement* GetMovement() { return m_MonsterMovement; }
	MonsterStatus* GetStatus() { return m_MonsterHPStatus; }
	RecognitionRange* GetRecognitionRange() const { return m_RecognitionRange; };
	AXIS GetCoorAxis() const { return m_Transform.GetCoorAxis(); }
	XMFLOAT3 GetSpawnPoint() const { return m_SpawnPoint; }
	float GetSpawnRange() const { return m_SpawnRange; }
	LoadObject* GetpLoadObject() const { return m_pLoadObject; }
	bool GetisAttacking() const { return m_isAttacking; }
	bool GetisFinishAttack() const { return m_isFinishAttack; } 
	XMFLOAT3 GetVelocity() const;
	//// Get /////////////////// 

	//// Set /////////////////// 
	void SetVelocity(const XMFLOAT3& velocity);
	void SetAnimationID(ENUM_ANIMATIONID state);
	void SetisAttacking(bool b) { m_isAttacking = b; }
	void FinishAttack() { m_isFinishAttack = true; }
	//// Set /////////////////// 

};