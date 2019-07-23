#include "stdafx.h"

#include "Movement.h"

#include "IceBall.h"
 
IceBall::IceBall(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	:BallEffect(entityID, pd3dDevice, pd3dCommandList)
{ 
}

IceBall::~IceBall()
{
}
 