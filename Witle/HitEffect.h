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
	
	// 해당 스프라이트가 가로로 몇개가 있는지
	float m_ResolX{ 4 }; 
	
	// 해당 스프라이트가 세로로 몇개가 있는지
	float m_ResolY { 1 };
	
protected: 
	UI3DImage*			m_HitEffect{ nullptr }; 
	  
public: 
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;
	 
public:
	HitEffect(GameObject* pOwner, float resolX, float resolY, float changeIndexTime = 0.1f);
	virtual ~HitEffect();

	// Update: 시간에 대한 index 관리
	virtual void Update(float) override;

	// Update 수행 이전 반드시 호출 
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, const XMFLOAT3& pos);
	
	bool IsActive() const { return isActive; }

	void Active() { isActive = true; }
	void Deactive() { isActive = false; }
};