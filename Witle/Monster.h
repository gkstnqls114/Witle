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
	static bool RENDER_DEBUG;

public:
	virtual void Update(float fElapsedTime) = 0;
	virtual void UpdateState(float fElapsedTime) = 0;
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList);

protected:
	UI3DImage*			m_MonsterHP{ nullptr };
	RecognitionRange*   m_RecognitionRange{ nullptr };
	MonsterMovement*    m_MonsterMovement{ nullptr };
	MonsterStatus*	   m_MonsterStatus{ nullptr };
	MyBOBox*		   m_pMyBOBox{ nullptr };
	 
	Texture*			m_pTexture{ nullptr };
	MyDescriptorHeap*	m_pHaep{ nullptr };

	CLoadedModelInfo*  m_MonsterModel{ nullptr };
	LoadObject*		   m_pLoadObject{ nullptr };

	bool m_isAttacking{ false };

	XMFLOAT3 m_SpawnPoint;
	const float    m_SpawnRange{ 1500.f };
	// Spawn Point ������ Range�� Ȯ���ϱ� ���� �ӽ÷� ����
	GameObject * m_pDebugObject{ nullptr };
	LineMesh* m_pDebugSpawnMesh{ nullptr };

private: 
	int m_CurrAnimation{ 0 }; // ���� ����ϴ� �ִϸ��̼�
	int m_PrevAnimation{ 0 }; // ���� ����ϴ� �ִϸ��̼�

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;
	 
private: 
	void SetTrackAnimationSet();

public:
	static void CHANGEMODE()
	{
		RENDER_DEBUG = !RENDER_DEBUG;
	}

	Monster(const std::string& entityID, const XMFLOAT3& SpwanPoint, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~Monster();
	 
	void RenderHpStatus(ID3D12GraphicsCommandList *pd3dCommandList);
	  
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
	MonsterStatus* GetStatus() { return m_MonsterStatus; }
	RecognitionRange* GetRecognitionRange() const { return m_RecognitionRange;};
	AXIS GetCoorAxis() const { return m_Transform.GetCoorAxis(); } 
	MyBOBox* GetBOBox() const { return m_pMyBOBox; }
	XMFLOAT3 GetSpawnPoint() const { return m_SpawnPoint; }
	float GetSpawnRange() const { return m_SpawnRange; }
	LoadObject* GetpLoadObject() const { return m_pLoadObject; }
	bool GetisAttacking() const { return m_isAttacking; }
	//// Get /////////////////// 
	
	//// Set /////////////////// 
	void SetAnimationState(int state);
	void SetisAttacking(bool b) { m_isAttacking = b; }
	//// Set /////////////////// 

};