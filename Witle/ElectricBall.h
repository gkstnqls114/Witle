#pragma once
#include "SkillEffect.h"

class SphereMesh;
class Movement;

class ElectricBall :
	public SkillEffect
{
public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override = 0;
	  
private:  

protected:
	virtual void ReleaseMembers() override {};
	virtual void ReleaseMemberUploadBuffers() override {}; 
	 
public: 
	ElectricBall(const std::string& entityID);
	virtual ~ElectricBall(); 
};