#pragma once

class Player;
class PlayerStatus;
class SkillEffect;
class SelectableSkill;
  
// 플레이어가 사용하는 스킬들의 이펙트를 관리하는 클래스입니다.
// 해당 클래스 내부에서 스킬을 생성하지 않고 SkillStg를 통해 가져오고 설정해야합니다.
class PlayerSkillMgr
{
	static PlayerSkillMgr* m_Instance;

	// 스킬 이펙트와 스킬 이펙트가 사용되는 유무를 나타내는 구조체입니다. 
	SelectableSkill* m_skill{ nullptr };

	// 스킬이 날가아는 최대 거리
	const float m_distance{ 2000.f };

private:
	// 스킬 이펙트를 비활성화합니다.
	// 날아갔던 위치에서부터 일정 거리 이상으로 멀어지면 비활성화합니다. 
	void Deactivate();

	PlayerSkillMgr();
	virtual ~PlayerSkillMgr();
	
public:
	static PlayerSkillMgr* GetInstance()
	{
		if (nullptr == m_Instance)
		{
			m_Instance = new PlayerSkillMgr;
		}

		return m_Instance;
	}

	// 가속도를 설정합니다.
	void UpdatePhysics(float fElapsedTime);

	// 활성화된 스킬을 가속도를 통해 이동시킵니다.
	void Update(float fElapsedTime);

	void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);

	// 스킬 이펙트를 활성화합니다.
	// playerStaus: 마나 . skilltype: 활성화 시킬 스킬 타입
	// 가속도를 설정하는 부분은 UpdatePhysics에서 수행됩니다.
	void Activate(PlayerStatus* MPStaus, int index);

	// 해당 index에 해당하는 스킬 이펙트를 비활성화합니다.
	void Deactive(int index);

	SelectableSkill* GetSkillEffect(int index);
	
	// 스킬 이펙트를 설정합니다.
	void SetSkillEffect(SkillEffect* skilleffect, UINT index);

	// 해당 스킬이 활성화 되었는지 확인합니다.
	// 만약 고른 스킬 중에서 해당 스킬이 존재하지 않을 경우 false를 반환합니다.
	bool isActive(ENUM_SELECTABLESKILL skill);
};