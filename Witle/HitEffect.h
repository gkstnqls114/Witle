#pragma once
#include "GameObject.h"

class MyRectangle;

// Hit Effect
class HitEffect
	: public GameObject
{ 
private: 
	const float m_ChangeIndexTime{ 0.1f };
	float m_TotalTime{ 0.f };

	bool isActive{ true };

	float m_CurrIndex{ 0 };
	
	// �ش� ��������Ʈ�� ���η� ��� �ִ���
	float m_ResolX{ 4 }; 
	
	// �ش� ��������Ʈ�� ���η� ��� �ִ���
	float m_ResolY { 1 };
	
	UI3DImage*			m_HitEffect{ nullptr }; 
	 
protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;
	 
public:
	HitEffect(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~HitEffect();

	// Update: �̵��� ���õ� �ൿ�� ����
	virtual void Update(float) override;

	// Update ���� ���� �ݵ�� ȣ�� 
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, const XMFLOAT4X4 world);
	
};