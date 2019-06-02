#pragma once
#include "Monster.h"

class SpaceCat
	: public Monster
{   
public:
	SpaceCat(const std::string& entityID, const XMFLOAT3& SpawnPoint, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~SpaceCat();

	virtual void Update(float) override;

	// 현재 수행할 액션(가속도관리하는클래스)를 관리합니다.
	virtual void UpdateState(float fElapsedTime) override;

	virtual void Animate(float fElapsedTime) override;

};