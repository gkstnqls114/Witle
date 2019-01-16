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

	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);

	virtual void Create();
	virtual void Init();

protected:
	HeightMapImage				*m_pHeightMapImage;

	int							m_nWidth;
	int							m_nLength;

	XMFLOAT3					m_xmf3Scale;


public:
	float GetHeight(float x, float z, bool bReverseQuad = false);
	XMFLOAT3 GetNormal(float x, float z); 

	int GetHeightMapWidth();
	int GetHeightMapLength();

	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	float GetWidth() { return(m_nWidth * m_xmf3Scale.x); }
	float GetLength() { return(m_nLength * m_xmf3Scale.z); }

	Texture* GetTexture() const { return m_ResourceBase; }
};