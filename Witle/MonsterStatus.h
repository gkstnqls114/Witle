#pragma once
#include "Status.h"

class MyRectangle;

// HP ���� ����
class MonsterStatus
	: public Status
{ 
private:
	virtual void SetAnimationStateToHit() override;
	 
private:
	MyRectangle* m_HpBar{ nullptr };
	 
public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;

public:
	MonsterStatus(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~MonsterStatus();

	// Update: �̵��� ���õ� �ൿ�� ����
	virtual void Update(float) override;
	
	// Update ���� ���� �ݵ�� ȣ�� 
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void Damage(int damage, int state) override;

};