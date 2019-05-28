#pragma once
#include "Status.h"

class MyRectangle;

// HP 관련 정보
class MonsterStatus
	: public Status
{
	MyRectangle* m_HpBar{ nullptr };
	 
public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;

public:
	MonsterStatus(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~MonsterStatus();

	// Update: 이동에 관련된 행동을 수행
	virtual void Update(float) override;
	
	// Update 수행 이전 반드시 호출 
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);

};