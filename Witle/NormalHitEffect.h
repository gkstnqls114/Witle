#pragma once
#include "ComponentBase.h"

class UI3DImage;

// Hit Effect
class NormalHitEffect
	: public ComponentBase
{ 
private: 
	const float m_ChangeIndexTime{ 0.1f };
	float m_TotalTime{ 0.f };

	bool isActive{ false };

	float m_CurrIndex{ 0 };
	
	// �ش� ��������Ʈ�� ���η� ��� �ִ���
	float m_ResolX{ 4 }; 
	
	// �ش� ��������Ʈ�� ���η� ��� �ִ���
	float m_ResolY { 1 };
	
	UI3DImage*			m_NormalHitEffect{ nullptr }; 
	  
public: 
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;
	 
public:
	NormalHitEffect(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~NormalHitEffect();

	// Update: �̵��� ���õ� �ൿ�� ����
	virtual void Update(float) override;

	// Update ���� ���� �ݵ�� ȣ�� 
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, const XMFLOAT3& pos);
	
	bool IsActive() const { return isActive; }

	void Active() { isActive = true; }
	void Deactive() { isActive = false; }
};