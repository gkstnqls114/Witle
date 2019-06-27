#pragma once
#include "Mesh.h"

// CHeightMapGridMesh ¿‘¥œ¥Ÿ.
class TerrainMesh
	:public Mesh
{ 
public: 
	virtual void Render(ID3D12GraphicsCommandList * commandList, bool isGBuffers) override {};

private:
	class CDiffused2TexturedVertex 
	{
	public:
		XMFLOAT3						m_xmf3Position;

		XMFLOAT4						m_xmf4Diffuse;
		XMFLOAT2						m_xmf2TexCoord0;
		XMFLOAT2						m_xmf2TexCoord1;
	
	public:
		CDiffused2TexturedVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); m_xmf2TexCoord0 = m_xmf2TexCoord1 = XMFLOAT2(0.0f, 0.0f); }
		CDiffused2TexturedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse, XMFLOAT2 xmf2TexCoord0, XMFLOAT2 xmf2TexCoord1) { m_xmf3Position = XMFLOAT3(x, y, z); m_xmf4Diffuse = xmf4Diffuse; m_xmf2TexCoord0 = xmf2TexCoord0; m_xmf2TexCoord1 = xmf2TexCoord1; }
		CDiffused2TexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT2 xmf2TexCoord0 = XMFLOAT2(0.0f, 0.0f), XMFLOAT2 xmf2TexCoord1 = XMFLOAT2(0.0f, 0.0f)) { m_xmf3Position = xmf3Position; m_xmf4Diffuse = xmf4Diffuse; m_xmf2TexCoord0 = xmf2TexCoord0; m_xmf2TexCoord1 = xmf2TexCoord1; }
		~CDiffused2TexturedVertex() { }
	};

protected:
	int							m_nWidth;
	int							m_nLength;
	XMFLOAT3					m_xmf3Scale;

	CDiffused2TexturedVertex *m_pVertices{ nullptr };

public:
	TerrainMesh(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int xStart, int zStart, int nWidth, int nLength, XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), void *pContext = NULL);
	virtual ~TerrainMesh();
	
	virtual void Update(float fTimeElapsed) override {} 
	
	XMFLOAT3 GetScale() { return(m_xmf3Scale); } 
	int GetWidth() { return(m_nWidth); }
	int GetLength() { return(m_nLength); }

	virtual float OnGetHeight(int x, int z, void *pContext);
	virtual XMFLOAT4 OnGetColor(int x, int z, void *pContext);

};