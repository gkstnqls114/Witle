#pragma once
#include "BallSkill.h"

class IceBallSkill :
	public BallSkill
{
public:
	IceBallSkill(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual ~IceBallSkill();

};

