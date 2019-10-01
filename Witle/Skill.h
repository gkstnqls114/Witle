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
	// ������ ���� Ȯ��
	virtual bool IsFinish() = 0;

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

	virtual bool IsFinish() override { return true; };

public:
	FixedSkill(const std::string& entityID) : ISkill(entityID) {};
	virtual ~FixedSkill();

	void Init();

	virtual void Update(float fTimeElapsed);
	virtual void DoNotUse() override = 0;
	virtual void DoUse() override = 0;

	void Prepare() 
	{  
		m_isPrepare = true; 
		PrepareMember();
	}

	//// Get //////////////////////////////////////////////
	bool GetisPrepare() const { return m_isPrepare; }  
	bool GetisUsing() const { return m_isUsing; }
	//// Get //////////////////////////////////////////////

};


class SelectableSkill
	: public ISkill
{  
protected:
	virtual void ReleaseMembers();
	virtual void ReleaseMemberUploadBuffers();

	virtual void PrepareMember() {};

	virtual void DoNotUse() {};
	virtual void DoUse() {};
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override {};
	
	virtual void UpdateActive(float fElapsedTime) = 0;

public:
	virtual void Active() = 0;
	virtual bool IsFinish() override = 0;
	virtual void Update(float fElapsedTime) override;

public: 
	SelectableSkill(const std::string& entityID, ENUM_SELECTABLESKILL skilltype, float cooltime, float mp)
		: ISkill(entityID) , m_UsingMp(mp), m_SelectableSkillType(skilltype), m_CoolTime(cooltime) {};
	virtual ~SelectableSkill();

	void Init();

	float GetCoolTime() const { return m_CoolTime; }
	ENUM_SELECTABLESKILL GetSelectableSkillType() const { return m_SelectableSkillType; }

private:
	ENUM_SELECTABLESKILL m_SelectableSkillType{ ENUM_SELECTABLESKILL::SELECTABLESKILL_NONE };
	const float m_CoolTime{ 0.f }; // ��ų�� ����� �ϴ� ��Ÿ��

public:
	SkillEffect* m_skillEffect{ nullptr };
	XMFLOAT3 spawnPosition; // ��ų ���� ����
	const float m_UsingMp{ 0.f };
	bool isActive{ false }; // Ȱ��ȭ ����
	float RemainCoolTime; // ���� ��Ÿ��
	float RemainCoolTimePrecentage; // ���� ��Ÿ�� .. 0�̸� ��ų ��밡�� 1�̸� �� ��ų �����

};