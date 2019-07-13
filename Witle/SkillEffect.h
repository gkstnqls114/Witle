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
	
	// ������Ʈ �� ��ġ�� ���߾� Collider�� �����մϴ�.
	// �ݵ�� Transform�� ��ġ�� ������Ʈ�ϰ� ���� ������ּ���.
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
	// distance: 1�ʴ� �����̴� �Ÿ� ... ex: 2000 = 20m
	SkillEffect(const std::string& entityID, float distance = 5000);
	virtual ~SkillEffect();

	void Update(float); 
	void Move(const XMFLOAT3& xmf3Shift);

	// ������ ��ġ���� �븻 ���� ���� ���� ���ӵ��� ����Ͽ� �����մϴ�.
	void SetVelocity(const XMFLOAT3& position, const float OffsetY, const XMFLOAT3& diretcion);

	// ���� �浹ü�� �����մϴ�.
	MyCollider* GetCollier() const { return m_MyCollider; }
};