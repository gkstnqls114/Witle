#pragma once

class SelectableSkill;
class SkillEffect;
class Dragon;

// �����Ҽ� �ִ� ��ų�� SelectableSkill Ŭ������ ���õǴ� �ν��Ͻ����� �����ϴ� Ŭ�����Դϴ�.
// �ܼ��ϰ� �����ϰ� ���� �� �� �ֽ��ϴ�.
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
	 
	// ���ӵ��� �����մϴ�.
	void UpdatePhysics(float fElapsedTime);

	// Ȱ��ȭ�� ��ų�� ���ӵ��� ���� �̵���Ű�� �ð��� ����ϴ� ���� ���� �մϴ�.
	// ������ ���, ��ġ�� �÷��̾� �����ǿ� ����� �ϹǷ� �ݵ�� Player Update ���Ŀ� ȣ���ؾ��մϴ�.
	void Update(float fElapsedTime);

	void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);

	// ��ų ����Ʈ�� Ȱ��ȭ�մϴ�.
	// Dragon: �巡�� �ڽ��� ������ ����� look �� spawn�� �����մϴ�.
	// ���ӵ��� �����ϴ� �κ��� UpdatePhysics���� ����˴ϴ�.
	void Activate(Dragon* dragon, int index);

	// �ش� index�� �ش��ϴ� ��ų ����Ʈ�� ��Ȱ��ȭ�մϴ�.
	void Deactive(int index);

	SelectableSkill* GetpSelectableSkill(int index);

	// ��ų ����Ʈ�� �����մϴ�.
	void SetSkill(SelectableSkill* skilleffect, UINT index);

	// �ش� ��ų�� Ȱ��ȭ �Ǿ����� Ȯ���մϴ�.
	// ���� �� ��ų �߿��� �ش� ��ų�� �������� ���� ��� false�� ��ȯ�մϴ�.
	bool isActive(ENUM_SELECTABLESKILL skill);
};