#pragma once

class Player;
class SkillEffect;
class PlayerStaus;

enum ENUM_SKILL
{
	SKILL_FIREBALL,
	SKILL_ICEBALL,
	SKILL_ELECTRICBALL,
	SKILL_DEBUFF
}; 

struct SKILL
{
	SkillEffect* skillEffect{ nullptr };
	XMFLOAT3 spawnPosition; // ��ų ���� ����
	bool isActive{ false }; // Ȱ��ȭ ����
	float RemainCoolTime; // ���� ��Ÿ��
	float RemainCoolTimePrecentage; // ���� ��Ÿ�� .. 0�̸� ��ų ��밡�� 1�̸� �� ��ų �����
};

// �÷��̾ ����ϴ� ��ų���� ����Ʈ�� �����ϴ� Ŭ�����Դϴ�.
class PlayerSkillMgr
{
	// ��ų ����Ʈ�� ��ų ����Ʈ�� ���Ǵ� ������ ��Ÿ���� ����ü�Դϴ�.
	
	int m_count{ 4 }; // ��ų�� ��� ����
	SKILL* m_skill{ nullptr };

	float m_distance { 1000.f };

private:
	// ��ų ����Ʈ�� ��Ȱ��ȭ�մϴ�.
	// ���ư��� ��ġ�������� ���� �Ÿ� �̻����� �־����� ��Ȱ��ȭ�մϴ�. 
	void Deactivate();

public:
	PlayerSkillMgr(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~PlayerSkillMgr();
	
	// ���ӵ��� �����մϴ�.
	void UpdatePhysics(float fElapsedTime);

	// Ȱ��ȭ�� ��ų�� ���ӵ��� ���� �̵���ŵ�ϴ�.
	void Update(float fElapsedTime);

	void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);

	// ��ų ����Ʈ�� Ȱ��ȭ�մϴ�.
	// playerStaus: ���� . skilltype: Ȱ��ȭ ��ų ��ų Ÿ��
	// ���ӵ��� �����ϴ� �κ��� UpdatePhysics���� ����˴ϴ�.
	void Activate(PlayerStatus* MPStaus, ENUM_SKILL skilltype);

	// �ش� index�� �ش��ϴ� ��ų ����Ʈ�� ��Ȱ��ȭ�մϴ�.
	void Deactive(int index);

	SKILL* GetSkillEffect(int index) { return &m_skill[index]; };
	int GetCount() const { return m_count; }
};