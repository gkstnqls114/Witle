#pragma once
#include "BallSkill.h"

class FireBallSkill :
	public BallSkill
{

public:
	FireBallSkill(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual ~FireBallSkill();

};

