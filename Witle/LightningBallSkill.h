#pragma once
#include "BallSkill.h"

class LightningBallSkill :
	public BallSkill
{
public:
	LightningBallSkill(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual ~LightningBallSkill();

};

