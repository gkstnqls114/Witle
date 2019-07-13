#pragma once

class Player;
class SkillEffect;

struct SKILL
{
	SkillEffect* skillEffect{ nullptr };
	XMFLOAT3 spawnPosition;
	bool isActive{ false };
};

// 플레이어가 사용하는 스킬들의 이펙트를 관리하는 클래스입니다.
class PlayerSkillMgr
{
	// 스킬 이펙트와 스킬 이펙트가 사용되는 유무를 나타내는 구조체입니다.
	

	int m_count{ 5 }; // 스킬을 담는 개수
	SKILL* m_skill{ nullptr };

	float m_distance { 1000.f };

private:
	// 스킬 이펙트를 비활성화합니다.
	// 날아갔던 위치에서부터 일정 거리 이상으로 멀어지면 비활성화합니다. 
	void Deactivate();

public:
	PlayerSkillMgr(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~PlayerSkillMgr();
	
	// 가속도를 설정합니다.
	void UpdatePhysics(float fElapsedTime);

	// 활성화된 스킬을 가속도를 통해 이동시킵니다.
	void Update(float fElapsedTime);

	void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);

	// 스킬 이펙트를 활성화합니다.
	// 가속도를 설정하는 부분은 UpdatePhysics에서 수행됩니다.
	void Activate();

	// 해당 index에 해당하는 스킬 이펙트를 비활성화합니다.
	void Deactive(int index);

	SKILL* GetSkillEffect(int index) { return &m_skill[index]; };
	int GetCount() const { return m_count; }
};