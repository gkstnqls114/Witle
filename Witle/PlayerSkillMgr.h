#pragma once

class Player;
class SkillEffect;

// 플레이어가 사용하는 스킬들의 이펙트를 관리하는 클래스입니다.
class PlayerSkillMgr
{
	// 스킬 이펙트와 스킬 이펙트가 사용되는 유무를 나타내는 구조체입니다.
	struct SKILL
	{ 
		SkillEffect* skillEffect{ nullptr };
		bool isActive{ false };
	};

	int m_count{ 5 }; // 스킬을 담는 개수
	SKILL* m_skill{ nullptr };

public:
	PlayerSkillMgr(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~PlayerSkillMgr();
	 
	void Update(float fElapsedTime);
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void Activate(int index);
};