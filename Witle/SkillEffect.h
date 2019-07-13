#pragma once
#include "GameObject.h"

class SphereMesh;
class Movement;
class MyCollider;

class SkillEffect :
	public GameObject
{
public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override = 0;
	
	// 업데이트 된 위치에 맞추어 Collider를 설정합니다.
	// 반드시 Transform의 위치를 업데이트하고 나서 사용해주세요.
	virtual void UpdateCollider() = 0;

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;

private: 
	Movement* m_Movement { nullptr }; 

protected:
	MyCollider* m_MyCollider{ nullptr };

protected: 
	void UpdateMovement();
	 
public: 
	// distance: 1초당 움직이는 거리 ... ex: 2000 = 20m
	SkillEffect(const std::string& entityID, float distance = 5000);
	virtual ~SkillEffect();

	void Update(float); 
	void Move(const XMFLOAT3& xmf3Shift);

	// 설정된 위치값과 노말 방향 값을 통해 가속도를 계산하여 설정합니다.
	void SetVelocity(const XMFLOAT3& position, const float OffsetY, const XMFLOAT3& diretcion);

	// 현재 충돌체를 리턴합니다.
	MyCollider* GetCollier() const { return m_MyCollider; }
};