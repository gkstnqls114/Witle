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
	virtual void Update(float fElapsedTime) = 0;
	virtual void UpdateState(float fElapsedTime) = 0;

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


	XMFLOAT3 m_SpawnPoint;
	const float    m_SpawnRange{ 1500.f };
	// Spawn Point 주위로 Range를 확인하기 위해 임시로 설정
	GameObject * m_pDebugObject{ nullptr };
	LineMesh* m_pDebugSpawnMesh{ nullptr };

private: 
	int m_CurrAnimation{ 0 }; // 현재 사용하는 애니메이션
	int m_PrevAnimation{ 0 }; // 현재 사용하는 애니메이션

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;
	 
private: 
	void SetTrackAnimationSet();

public:
	Monster(const std::string& entityID, const XMFLOAT3& SpwanPoint, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~Monster();
	 
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);
	void RenderHpStatus(ID3D12GraphicsCommandList *pd3dCommandList);
	  
	XMFLOAT3 CalculateAlreadyVelocity(float fTimeElapsed);
	BoundingOrientedBox CalculateAlreadyBoundingBox(float fTimeElapsed);
	XMFLOAT3 CalculateAlreadyPosition(float fTimeElapsed);

	void SubstractHP(int sub);
	void Animate(float fElapsedTime);

	void Move(ULONG nDirection, float fDistance, bool bVelocity = false);
	void Move(const XMFLOAT3& xmf3Shift);
	void MoveVelocity(const XMFLOAT3& xmf3Shift);
	void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);
	void Rotate(float x, float y, float z);
	  
	//// Get /////////////////// 
	RecognitionRange* GetRecognitionRange() const { return m_RecognitionRange;};
	AXIS GetCoorAxis() const { return m_Transform.GetCoorAxis(); } 
	MyBOBox* GetBOBox() const { return m_pMyBOBox; }
	XMFLOAT3 GetSpawnPoint() const { return m_SpawnPoint; }
	float GetSpawnRange() const { return m_SpawnRange; }
	//// Get /////////////////// 
	 
};