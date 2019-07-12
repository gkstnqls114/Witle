#pragma once

class Player;
class SkillEffect;

// �÷��̾ ����ϴ� ��ų���� ����Ʈ�� �����ϴ� Ŭ�����Դϴ�.
class PlayerSkillMgr
{
	// ��ų ����Ʈ�� ��ų ����Ʈ�� ���Ǵ� ������ ��Ÿ���� ����ü�Դϴ�.
	struct SKILL
	{ 
		SkillEffect* skillEffect{ nullptr };
		XMFLOAT3 spawnPosition;
		bool isActive{ false }; 
	};

	int m_count{ 5 }; // ��ų�� ��� ����
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
	// ���ӵ��� �����ϴ� �κ��� UpdatePhysics���� ����˴ϴ�.
	void Activate(); 

};