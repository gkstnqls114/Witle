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

	// ���� �������� AI�� � �� ���⿡ ���ؼ� ������ �����մϴ�.
	virtual void ProcessAI(float fElapsedTime) override;

};