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
	// 끝나는 조건 확인
	virtual bool IsFinish() = 0;

};

class FixedSkill
	: public ISkill
{ 
protected:
	bool  m_isPrepare    { false };   // 스킬을 현재 사용주인
	bool  m_isUsing        { false }; // 스킬을 현재 사용주인
	float m_UsingTime   { 0.f }; // 재사용 대기시간
	float m_SkillTime      { 0.f }; // 스킬 사용 중인 시간

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
	virtual void Deactive() = 0;
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
	const float m_CoolTime{ 0.f }; // 스킬을 재시작 하는 쿨타임

public:
	SkillEffect* m_skillEffect{ nullptr };
	XMFLOAT3 spawnPosition; // 스킬 시작 지점
	const float m_UsingMp{ 0.f };
	bool isActive{ false }; // 활성화 여부
	float RemainCoolTime; // 남은 쿨타임
	float RemainCoolTimePrecentage; // 남은 쿨타임 .. 0이면 스킬 사용가능 1이면 막 스킬 사용함

};