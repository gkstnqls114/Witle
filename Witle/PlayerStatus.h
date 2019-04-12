#pragma once
#include "ComponentBase.h"

class MyRectangle;

class PlayerStatus
	: ComponentBase
{
	MyRectangle* m_pHpBar;

public:
	int m_HP{ 0 };

public:
	PlayerStatus(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~PlayerStatus();

	virtual void Update(float) override;
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);

};