#pragma once
#include "Skill.h" 
class PlayerMovement;

class Broom
	: public FixedSkill
{
public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override {};

private:
	PlayerMovement* m_pPlayerMovement{ nullptr };

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;

	virtual void PrepareMember() override;
private:

public:
	Broom(PlayerMovement* playerStatus);
	virtual ~Broom();

	virtual void DoNotUse() override;
	virtual void DoUse() override;
};