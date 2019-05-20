#pragma once
#include "ComponentBase.h"

class MyRectangle;

// HP ���� ����
class MonsterStatus
	: ComponentBase
{
	MyRectangle* m_HpBar{ nullptr };

public:
	int m_HP{ 1000 };

public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;

public:
	MonsterStatus(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~MonsterStatus();

	// Update: �̵��� ���õ� �ൿ�� ����
	virtual void Update(float) override;
	
	// Update ���� ���� �ݵ�� ȣ��
	virtual void UpdateAI(float);
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);

};