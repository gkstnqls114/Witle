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
		bool isActive{ false };
	};

	int m_count{ 5 }; // ��ų�� ��� ����
	SKILL* m_skill{ nullptr };

public:
	PlayerSkillMgr(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~PlayerSkillMgr();
	 
	void Update(float fElapsedTime);
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void Activate(int index);
};