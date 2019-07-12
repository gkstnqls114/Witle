#pragma once
#include "GameObject.h"

class SphereMesh;
class Movement;

class SkillEffect :
	public GameObject
{
public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override = 0;

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;

private: 
	Movement* m_Movement { nullptr }; 

protected: 
	void UpdateMovement();
	 
public: 
	// distance: 1초당 움직이는 거리 ... ex: 2000 = 20m
	SkillEffect(const std::string& entityID, float distance = 5000);
	virtual ~SkillEffect();

	void Update(float); 
	void Move(const XMFLOAT3& xmf3Shift);

	// 설정된 위치값과 노말 방향 값을 통해 가속도를 계산하여 설정합니다.
	void SetVelocity(const XMFLOAT3& position, const XMFLOAT3& diretcion);
};