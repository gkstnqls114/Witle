#pragma once
#include "GameObject.h"

class PlayerStatus;
class SkillEffect;

class ISkill
	: public GameObject
{  
protected:
	ISkill(const std::string& entityID) : GameObject(entityID) {};
	~ISkill() {};
	 
protected:
	virtual void ReleaseMembers() = 0;
	virtual void ReleaseMemberUploadBuffers() = 0;
	 
public:
	virtual void DoNotUse() = 0;
	virtual void DoUse() = 0;
	virtual void IsFinish() = 0;

};

class FixedSkill
	: public ISkill
{ 
protected:
	bool  m_isPrepare    { false };   // ��ų�� ���� �������
	bool  m_isUsing        { false }; // ��ų�� ���� �������
	float m_UsingTime   { 0.f }; // ���� ���ð�
	float m_SkillTime      { 0.f }; // ��ų ��� ���� �ð�

protected:
	virtual void ReleaseMembers() = 0;
	virtual void ReleaseMemberUploadBuffers() = 0;

	virtual void PrepareMember() = 0;

	virtual void IsFinish() override {};

public:
	FixedSkill() : ISkill("Skill") {};
	virtual ~FixedSkill();

	bool GetisUsing() const { return m_isUsing; }
	virtual void Update(float fTimeElapsed);
	virtual void DoNotUse() override = 0;
	virtual void DoUse() override = 0;

	void Prepare() 
	{  
		m_isPrepare = true; 
		PrepareMember();
	}
	bool GetisPrepare() const { return m_isPrepare; }

};


class SelectableSkill
	: public ISkill
{
protected: 

protected:
	virtual void ReleaseMembers();
	virtual void ReleaseMemberUploadBuffers();

	virtual void PrepareMember() {};

	virtual void DoNotUse() {};
	virtual void DoUse() {};
	virtual void IsFinish() override {};
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override {};

	virtual void Active() {};
public:
	SelectableSkill() : ISkill("Skill") {};
	virtual ~SelectableSkill();

	ENUM_SELECTABLESKILL GetSelectableSkillType() const { return m_SelectableSkillType; }

private:
	ENUM_SELECTABLESKILL m_SelectableSkillType{ ENUM_SELECTABLESKILL::SELECTABLESKILL_NONE };

public:
	SkillEffect* m_skillEffect{ nullptr };
	XMFLOAT3 spawnPosition; // ��ų ���� ����
	bool isActive{ false }; // Ȱ��ȭ ����
	float RemainCoolTime; // ���� ��Ÿ��
	float RemainCoolTimePrecentage; // ���� ��Ÿ�� .. 0�̸� ��ų ��밡�� 1�̸� �� ��ų �����

};