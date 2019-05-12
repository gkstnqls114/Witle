#pragma once
#include "Skill.h" 
class PlayerMovement;

class Broom
	: public Skill
{
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