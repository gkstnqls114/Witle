#pragma once
#include "Monster.h"

class Slime
	: public Monster
{
	
public:
	Slime(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~Slime();
};