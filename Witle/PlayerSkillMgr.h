#pragma once

class Player;
class SkillEffect;
class PlayerStatus;
 
struct SKILL
{
	SkillEffect* skillEffect{ nullptr };
	XMFLOAT3 spawnPosition; // ��ų ���� ����
	bool isActive{ false }; // Ȱ��ȭ ����
	float RemainCoolTime; // ���� ��Ÿ��
	float RemainCoolTimePrecentage; // ���� ��Ÿ�� .. 0�̸� ��ų ��밡�� 1�̸� �� ��ų �����
};

// �÷��̾ ����ϴ� ��ų���� ����Ʈ�� �����ϴ� Ŭ�����Դϴ�.
// �ش� Ŭ���� ���ο��� ��ų�� �������� �ʰ� SkillStg�� ���� �������� �����ؾ��մϴ�.
class PlayerSkillMgr
{
	static PlayerSkillMgr* m_Instance;

	// ��ų ����Ʈ�� ��ų ����Ʈ�� ���Ǵ� ������ ��Ÿ���� ����ü�Դϴ�. 
	SKILL* m_skill{ nullptr }; 

	// ��ų�� �����ƴ� �ִ� �Ÿ�
	const float m_distance{ 2000.f };

private:
	// ��ų ����Ʈ�� ��Ȱ��ȭ�մϴ�.
	// ���ư��� ��ġ�������� ���� �Ÿ� �̻����� �־����� ��Ȱ��ȭ�մϴ�. 
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

	// ���ӵ��� �����մϴ�.
	void UpdatePhysics(float fElapsedTime);

	// Ȱ��ȭ�� ��ų�� ���ӵ��� ���� �̵���ŵ�ϴ�.
	void Update(float fElapsedTime);

	void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);

	// ��ų ����Ʈ�� Ȱ��ȭ�մϴ�.
	// playerStaus: ���� . skilltype: Ȱ��ȭ ��ų ��ų Ÿ��
	// ���ӵ��� �����ϴ� �κ��� UpdatePhysics���� ����˴ϴ�.
	void Activate(PlayerStatus* MPStaus, int index);

	// �ش� index�� �ش��ϴ� ��ų ����Ʈ�� ��Ȱ��ȭ�մϴ�.
	void Deactive(int index);

	SKILL* GetSkillEffect(int index) { return &m_skill[index]; }; 
	
	// ��ų ����Ʈ�� �����մϴ�.
	void SetSkillEffect(SkillEffect* skilleffect, UINT index) 
	{
		m_skill[index].skillEffect = skilleffect;
	}
};