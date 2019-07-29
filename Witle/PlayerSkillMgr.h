#pragma once

class Player;
class PlayerStatus;
class SkillEffect;
class SelectableSkill;
  
// �÷��̾ ����ϴ� ��ų���� ����Ʈ�� �����ϴ� Ŭ�����Դϴ�.
// �ش� Ŭ���� ���ο��� ��ų�� �������� �ʰ� SkillStg�� ���� �������� �����ؾ��մϴ�.
class PlayerSkillMgr
{
	static PlayerSkillMgr* m_Instance;

	// SkillStg���� ������ SelectableSkill �����͸� �����ϴ� �迭.
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

	// ���ӵ��� �����մϴ�.
	void UpdatePhysics(float fElapsedTime);

	// Ȱ��ȭ�� ��ų�� ���ӵ��� ���� �̵���Ű�� �ð��� ����ϴ� ���� ���� �մϴ�.
	void Update(float fElapsedTime);

	void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);

	// ��ų ����Ʈ�� Ȱ��ȭ�մϴ�.
	// playerStaus: ���� . skilltype: Ȱ��ȭ ��ų ��ų Ÿ��
	// ���ӵ��� �����ϴ� �κ��� UpdatePhysics���� ����˴ϴ�.
	void Activate(PlayerStatus* MPStaus, int index);

	// �ش� index�� �ش��ϴ� ��ų ����Ʈ�� ��Ȱ��ȭ�մϴ�.
	void Deactive(int index);

	SelectableSkill* GetSkillEffect(int index);
	
	// ��ų ����Ʈ�� �����մϴ�.
	void SetSkill(SelectableSkill* skilleffect, UINT index);

	// �ش� ��ų�� Ȱ��ȭ �Ǿ����� Ȯ���մϴ�.
	// ���� �� ��ų �߿��� �ش� ��ų�� �������� ���� ��� false�� ��ȯ�մϴ�.
	bool isActive(ENUM_SELECTABLESKILL skill);
};