#pragma once
#include "BallEffect.h"
 
class LightningBall :
	public BallEffect
{ 
private:   

public: 
	LightningBall(const std::string& entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual ~LightningBall(); 
};