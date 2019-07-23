#pragma once
#include "BallEffect.h"
 
class FireBall :
	public BallEffect
{ 
protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;
	 
public: 
	FireBall(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~FireBall(); 
};