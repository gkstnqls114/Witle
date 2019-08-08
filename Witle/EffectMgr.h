#pragma once
 
class EffectMgr
{
	static EffectMgr* m_Instance;
	 
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
	 
};