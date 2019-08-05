#pragma once
#include "GameObject.h"

class Camera;
class CLoadedModelInfo;
class LoadObject;
class MyBOBox;
class MonsterStatus;
class MonsterMovement;
class Texture;
class MyDescriptorHeap;

class LineMesh;
class UI3DImage;

class RecognitionRange;
class MyRectangle;

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

private:
	static bool RENDER_DEBUG;

public:
	virtual void Update(float fElapsedTime) = 0;
	virtual void UpdateState(float fElapsedTime) = 0; 

protected:
	UI3DImage*			m_MonsterHPUI{ nullptr };
	RecognitionRange*   m_RecognitionRange{ nullptr };
	MonsterMovement*    m_MonsterMovement{ nullptr };
	MonsterStatus*	   m_MonsterHPStatus{ nullptr };
	MyBOBox*		   m_pMyBOBox{ nullptr };
	 
	Texture*			m_pTexture{ nullptr }; 

	CLoadedModelInfo*  m_MonsterModel{ nullptr };
	LoadObject*		   m_pLoadObject{ nullptr };

	bool m_isAttacking{ false };

	XMFLOAT3 m_SpawnPoint;
	const float    m_SpawnRange{ 500.f };
	const float m_HpBarY{ 200.f };

#ifdef _DEBUG 
	GameObject * m_pDebugObject{ nullptr };
	// Spawn Point ������ Range�� Ȯ���ϱ� ���� �ӽ÷� ����
	LineMesh* m_pDebugSpawnMesh{ nullptr };
#endif

private: 
	int m_CurrAnimation{ 0 }; // ���� ����ϴ� �ִϸ��̼�
	int m_PrevAnimation{ 0 }; // ���� ����ϴ� �ִϸ��̼�

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override; 
	void SetTrackAnimationSet();

public:
	static void CHANGEMODE()
	{
		RENDER_DEBUG = !RENDER_DEBUG;
	}

	// float HpBarY: HP Bar UI�� Y �� ��ġ
	Monster(const std::string& entityID, float spawnRange, const XMFLOAT3& SpwanPoint, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, float HpBarY = 200.f);
	virtual ~Monster();
	 
	void RenderHpStatus(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	  
	XMFLOAT3 CalculateAlreadyVelocity(float fTimeElapsed);
	BoundingOrientedBox CalculateAlreadyBoundingBox(float fTimeElapsed);
	XMFLOAT3 CalculateAlreadyPosition(float fTimeElapsed);

	void SubstractHP(int sub);
	virtual void Animate(float fElapsedTime);

	void Move(ULONG nDirection, float fDistance, bool bVelocity = false);
	void Move(const XMFLOAT3& xmf3Shift);
	void MoveVelocity(const XMFLOAT3& xmf3Shift);
	void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);
	void Rotate(float x, float y, float z);
	  
	//// Get /////////////////// 
	MonsterMovement* GetMovement() { return m_MonsterMovement; }
	MonsterStatus* GetStatus() { return m_MonsterHPStatus; }
	RecognitionRange* GetRecognitionRange() const { return m_RecognitionRange;};
	AXIS GetCoorAxis() const { return m_Transform.GetCoorAxis(); } 
	MyBOBox* GetBOBox() const { return m_pMyBOBox; }
	XMFLOAT3 GetSpawnPoint() const { return m_SpawnPoint; }
	float GetSpawnRange() const { return m_SpawnRange; }
	LoadObject* GetpLoadObject() const { return m_pLoadObject; }
	bool GetisAttacking() const { return m_isAttacking; }
	XMFLOAT3 GetVelocity() const;
	//// Get /////////////////// 
	
	//// Set /////////////////// 
	void SetVelocity(const XMFLOAT3& velocity);
	void SetAnimationState(int state);
	void SetisAttacking(bool b) { m_isAttacking = b; }
	//// Set /////////////////// 

};