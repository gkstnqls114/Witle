#pragma once
#include "GameObject.h"
 
class HitEffect;

class EffectMgr
{
	static EffectMgr* m_Instance;

	struct HITEFFECT
	{
		HitEffect* pEffect;
		XMFLOAT3 pos;
	};
	const int m_MaxEffect{ 10 };
	std::vector<HITEFFECT> m_EffectList;

	GameObject* m_GameObject{ nullptr };

public:
	EffectMgr() : m_EffectList(m_MaxEffect) {};
	virtual ~EffectMgr();

	static EffectMgr* GetInstance()
	{
		if (nullptr == m_Instance)
		{
			m_Instance = new EffectMgr;
		}

		return m_Instance;
	}

	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseUploadBuffers();
	void ReleaseObjects();

	void Update(float fElapsedTime);

	void Render(ID3D12GraphicsCommandList *pd3dCommandList);
	 
	void AddEffectPosition(ENUM_EFFECT type, const XMFLOAT3 pos);
};