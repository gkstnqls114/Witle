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
	 
	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseUploadBuffers();
	void ReleaseObjects();

	SkillEffect* GetSkillEffect(int index) const;
};