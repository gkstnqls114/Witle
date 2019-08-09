#pragma once
#include "GameObject.h"
 
class HitEffect;

class HitEffectMgr
{
	static HitEffectMgr* m_Instance;

	struct HITEFFECT
	{
		HitEffect* pEffect;
		XMFLOAT3 pos;
	};
	const int m_MaxEffect{ 10 };

	std::vector<HITEFFECT> m_MonsterHitEffectList;

	std::vector<HITEFFECT> m_IceBallHitEffectList;
	std::vector<HITEFFECT> m_FireBallHitEffectList;
	std::vector<HITEFFECT> m_LightningBallHitEffectList;
	std::vector<HITEFFECT> m_NormalHitEffectList;

	GameObject* m_GameObject{ nullptr };
	 
private: 
	void AddIceBallHitEffectPosition(const XMFLOAT3 pos);
	void AddFireBallHitEffectPosition(const XMFLOAT3 pos);
	void AddLightningBallHitEffectPosition(const XMFLOAT3 pos);

public:
	HitEffectMgr() :
		m_MonsterHitEffectList(m_MaxEffect),
		m_IceBallHitEffectList(m_MaxEffect),
		m_FireBallHitEffectList(m_MaxEffect),
		m_LightningBallHitEffectList(m_MaxEffect),
		m_NormalHitEffectList(m_MaxEffect)
	{};
	virtual ~HitEffectMgr();

	static HitEffectMgr* GetInstance()
	{
		if (nullptr == m_Instance)
		{
			m_Instance = new HitEffectMgr;
		}

		return m_Instance;
	}

	static void ReleaseInstance()
	{
		if (m_Instance)
		{
			delete m_Instance;
			m_Instance = nullptr;
		}
	}

	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseUploadBuffers();
	void ReleaseObjects();

	void Update(float fElapsedTime);

	void Render(ID3D12GraphicsCommandList *pd3dCommandList);
	 
	// 설정된 pos에 hit effect 위치 및 활성화
	void AddMonsterHitEffectPosition(const XMFLOAT3 pos);
	void AddPlayerSkillHitEffectPosition(ENUM_SELECTABLESKILL skilltype, const XMFLOAT3 pos);
	void AddNormalHitEffectPosition(const XMFLOAT3 pos);

};