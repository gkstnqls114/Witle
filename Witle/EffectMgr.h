#pragma once
 
class HitEffect;

class EffectMgr
{
	static EffectMgr* m_Instance;
	 
	HitEffect* m_HitEffect{ nullptr };

	std::list<XMFLOAT3> m_effectPositionList;

public:
	EffectMgr();
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