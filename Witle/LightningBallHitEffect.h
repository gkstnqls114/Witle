#pragma once
#include "ComponentBase.h"

class UI3DImage;

// Hit Effect
class LightningBallHitEffect
	: public ComponentBase
{ 
private: 
	const float m_ChangeIndexTime{ 0.1f };
	float m_TotalTime{ 0.f };

	bool isActive{ false };

	float m_CurrIndex{ 0 };
	
	// 해당 스프라이트가 가로로 몇개가 있는지
	float m_ResolX{ 4 }; 
	
	// 해당 스프라이트가 세로로 몇개가 있는지
	float m_ResolY { 1 };
	
	UI3DImage*			m_LightningBallHitEffect{ nullptr }; 
	  
public: 
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;
	 
public:
	LightningBallHitEffect(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~LightningBallHitEffect();

	// Update: 이동에 관련된 행동을 수행
	virtual void Update(float) override;

	// Update 수행 이전 반드시 호출 
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, const XMFLOAT3& pos);
	
	bool IsActive() const { return isActive; }

	void Active() { isActive = true; }
	void Deactive() { isActive = false; }
};