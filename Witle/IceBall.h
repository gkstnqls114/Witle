#pragma once
#include "BallEffect.h"
 
class IceBall :
	public BallEffect
{  
public: 
	IceBall(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual ~IceBall(); 
};