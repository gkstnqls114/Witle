#pragma once
  
class SelectableSkill;
class SkillEffect;

// 선택할수 있는 스킬인 SelectableSkill 클래스와 관련되는 인스턴스들을 저장하는 클래스입니다.
// 단순하게 저장하고 꺼낼 수 만 있습니다.
class SkillStg
{
	static SkillStg* m_Instance;

	SelectableSkill* m_SelectableSkills[SKILL_TO_CHOOSE];

public:
	SkillStg();
	virtual ~SkillStg();

	static SkillStg* GetInstance() 
	{
		if (nullptr == m_Instance)
		{
			m_Instance = new SkillStg;
		}

		return m_Instance;
	}
	 
	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseUploadBuffers();
	void ReleaseObjects();

	SkillEffect* GetSkillEffect(int index) const;
};