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

	// SkillStg에서 가져온 SelectableSkill 포인터를 저장하는 배열.
	SelectableSkill** m_skill{ nullptr };


private:
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

	void Init();

	// 가속도를 설정합니다.
	void UpdatePhysics(float fElapsedTime);

	// 활성화된 스킬을 가속도를 통해 이동시키고 시간을 계산하는 등의 일을 합니다.
	// 쉴드의 경우, 위치를 플레이어 포지션에 맞춰야 하므로 반드시 Player Update 이후에 호출해야합니다.
	void Update(float fElapsedTime);

	void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);

	// 스킬 이펙트를 활성화합니다.
	// playerStaus: 마나 . skilltype: 활성화 시킬 스킬 타입
	// 가속도를 설정하는 부분은 UpdatePhysics에서 수행됩니다.
	void Activate(PlayerStatus* MPStaus, int index);

	// 해당 index에 해당하는 스킬 이펙트를 비활성화합니다.
	void Deactive(int index);

	SelectableSkill* GetpSelectableSkill(int index);
	
	// 스킬 이펙트를 설정합니다.
	void SetSkill(SelectableSkill* skilleffect, UINT index);

	// 해당 스킬이 활성화 되었는지 확인합니다.
	// 만약 고른 스킬 중에서 해당 스킬이 존재하지 않을 경우 false를 반환합니다.
	bool isActive(ENUM_SELECTABLESKILL skill);

	// 해당 n 번째 스킬이 활성화 되었는지 확인합니다.
	// 만약 고른 스킬 중에서 해당 스킬이 존재하지 않을 경우 false를 반환합니다.
	bool isActive(int index);
};