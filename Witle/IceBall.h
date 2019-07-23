#pragma once
#include "BallEffect.h"
 
class IceBall :
	public BallEffect
{ 
private:  

protected:
	virtual void ReleaseMembers() override {};
	virtual void ReleaseMemberUploadBuffers() override {}; 
	 
public: 
	IceBall(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual ~IceBall(); 
};