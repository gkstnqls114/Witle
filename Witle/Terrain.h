#pragma once
#include "GameObject.h"

class ResourceBase;
class HeightMapImage;
class Texture;

// CHeightMapTerrain ¿‘¥œ¥Ÿ.
class Terrain :
	public GameObject
{
	Texture * m_ResourceBase{ nullptr };

public:
	Terrain(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color);
	virtual ~Terrain();

	virtual void Update(float fElapsedTime) override {};

	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);

	virtual void Create();
	virtual void Init();

protected:
	HeightMapImage				*m_pHeightMapImage;

	int							m_nWidth;
	int							m_nLength;

	XMFLOAT3					m_xmf3Scale;

protected:
	virtual void ReleaseMembers() override;

public:
	float GetHeight(float x, float z, bool bReverseQuad = false);
	XMFLOAT3 GetNormal(float x, float z); 

	int GetHeightMapWidth();
	int GetHeightMapLength();

	XMFLOAT3 GetScale() const { return(m_xmf3Scale); }
	float GetWidth() const { return(m_nWidth * m_xmf3Scale.x); }
	float GetLength() const { return(m_nLength * m_xmf3Scale.z); }

	Texture* GetTexture() const { return m_ResourceBase; }
	HeightMapImage* GetHeightMapImage() { return m_pHeightMapImage; }
};