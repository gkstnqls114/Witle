#pragma once
#include "ComponentBase.h"

class MyRectangle;

// HP 관련 정보
class PlayerStatus
	: ComponentBase
{

public:
	int m_HP{ 1000 };

public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;

public:
	PlayerStatus(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~PlayerStatus();

	virtual void Update(float) override;
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);

};