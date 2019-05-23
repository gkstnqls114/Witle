#pragma once
#include "Monster.h"

class Slime
	: public Monster
{
	XMFLOAT3 m_Direction{ 0, 0, 0 };


public:
	Slime(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~Slime();

	virtual void Update(float) override;

	// 현재 슬라임의 AI는 어떤 한 방향에 대해서 직진만 수행합니다.
	virtual void ProcessAI(float fElapsedTime) override;

};