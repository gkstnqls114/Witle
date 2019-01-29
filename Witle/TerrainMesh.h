#pragma once
#include "Mesh.h"

// CHeightMapGridMesh ¿‘¥œ¥Ÿ.
class TerrainMesh
	:public Mesh
{

protected:
	int							m_nWidth;
	int							m_nLength;
	XMFLOAT3					m_xmf3Scale;

	CDiffused2TexturedVertex *m_pVertices{ nullptr };

public:
	TerrainMesh(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int xStart, int zStart, int nWidth, int nLength, XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), void *pContext = NULL);
	virtual ~TerrainMesh();

	XMFLOAT3 GetScale() { return(m_xmf3Scale); } 
	int GetWidth() { return(m_nWidth); }
	int GetLength() { return(m_nLength); }

	virtual float OnGetHeight(int x, int z, void *pContext);
	virtual XMFLOAT4 OnGetColor(int x, int z, void *pContext);


	virtual void Update(float fTimeElapsed) override {}
	virtual void Create() override {}
	virtual void Init() override {}

	void TerrainMesh::CopyVertexArray(const SourcePtr& vertexList)
	{
		assert(!(vertexList.m_byteSize != sizeof(CDiffused2TexturedVertex)));
		memcpy(vertexList.m_Ptr, m_pVertices, sizeof(CDiffused2TexturedVertex) * m_vertexCount);
	}

};