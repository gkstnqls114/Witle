#pragma once
  
class SelectableSkill;
class SkillEffect;

// �����Ҽ� �ִ� ��ų�� SelectableSkill Ŭ������ ���õǴ� �ν��Ͻ����� �����ϴ� Ŭ�����Դϴ�.
// �ܼ��ϰ� �����ϰ� ���� �� �� �ֽ��ϴ�.
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
	 
	static void ReleaseInstance()
	{
		if (m_Instance)
		{ 
			delete m_Instance;
			m_Instance = nullptr;
		}
	}

	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseUploadBuffers();
	void ReleaseObjects();

	SelectableSkill* GetSkill(int index) const;
};