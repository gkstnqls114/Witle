#pragma once
 
class SkillEffect;

// ��ų����Ʈ Ŭ������ ���õǴ� Ŭ�������� �����ϴ� Ŭ�����Դϴ�.
// �ܼ��ϰ� �����ϰ� ���� �� �� �ֽ��ϴ�.
class SkillStg
{
	static SkillStg* m_Instance;

	SkillEffect* skillEffect[8];

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

	SkillEffect* GetSkillEffect(int index) const { return skillEffect[index]; }
};