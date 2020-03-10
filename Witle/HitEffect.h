#pragma once
#include "ComponentBase.h"

class UI3DImage;

// Hit Effect
class HitEffect
	: public ComponentBase
{ 
	const float m_ChangeIndexTime{ 0.1f };
	float m_TotalTime{ 0.f };

	bool isActive{ false };

	float m_CurrIndex{ 0 };
	
	// �ش� ��������Ʈ�� ���η� ��� �ִ���
	float m_ResolX{ 4 }; 
	
	// �ش� ��������Ʈ�� ���η� ��� �ִ���
	float m_ResolY { 1 };
	
	bool isBillBoard{ true };
	
protected: 
	UI3DImage*			m_HitEffect{ nullptr }; 
	  
public: 
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;
	 
public:
	HitEffect(GameObject* pOwner, float resolX, float resolY, float changeIndexTime = 0.1f);
	virtual ~HitEffect();

	// Update: �ð��� ���� index ����
	virtual void Update(float) override;

	// Update ���� ���� �ݵ�� ȣ�� 
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, const XMFLOAT3& pos);
	
	bool IsActive() const { return isActive; }

	void Active() { isActive = true; }
	void Deactive() { isActive = false; }

	void DontBillboard() { isBillBoard = false; }
};