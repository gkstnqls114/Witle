#pragma once

class SelectableSkill;
class SkillEffect;
class Dragon;

// 선택할수 있는 스킬인 SelectableSkill 클래스와 관련되는 인스턴스들을 저장하는 클래스입니다.
// 단순하게 저장하고 꺼낼 수 만 있습니다.
class BossSkillMgr
{
	static BossSkillMgr* m_Instance;
	
	const float m_DeltaTime{ 1.f };

	const int m_count{ 10 };
	SelectableSkill** m_SelectableSkills;

public:
	BossSkillMgr();
	virtual ~BossSkillMgr();

	static BossSkillMgr* GetInstance()
	{
		if (nullptr == m_Instance)
		{
			m_Instance = new BossSkillMgr;
		}

		return m_Instance;
	}

	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseUploadBuffers();
	void ReleaseObjects();
	 
	// 가속도를 설정합니다.
	void UpdatePhysics(float fElapsedTime);

	// 활성화된 스킬을 가속도를 통해 이동시키고 시간을 계산하는 등의 일을 합니다.
	// 쉴드의 경우, 위치를 플레이어 포지션에 맞춰야 하므로 반드시 Player Update 이후에 호출해야합니다.
	void Update(float fElapsedTime);

	void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);

	// 스킬 이펙트를 활성화합니다.
	// Dragon: 드래곤 자신의 정보를 갖고와 look 과 spawn을 설정합니다.
	// 가속도를 설정하는 부분은 UpdatePhysics에서 수행됩니다.
	void Activate(Dragon* dragon, int index);

	// 해당 index에 해당하는 스킬 이펙트를 비활성화합니다.
	void Deactive(int index);

	SelectableSkill* GetpSelectableSkill(int index);

	// 스킬 이펙트를 설정합니다.
	void SetSkill(SelectableSkill* skilleffect, UINT index);

	// 해당 스킬이 활성화 되었는지 확인합니다.
	// 만약 고른 스킬 중에서 해당 스킬이 존재하지 않을 경우 false를 반환합니다.
	bool isActive(ENUM_SELECTABLESKILL skill);
};