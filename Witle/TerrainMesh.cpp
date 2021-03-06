#include "stdafx.h"
#include "d3dUtil.h"
#include "HeightMapImage.h"
#include "TerrainMesh.h"

TerrainMesh::TerrainMesh(GameObject* pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int xStart, int zStart, int nWidth, int nLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color, void * pContext)
	:Mesh(pOwner, D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP)
{
	m_ComponenetID = TERRAIN_MESH;

	m_vertexCount = nWidth * nLength;
	//	m_nStride = sizeof(CTexturedVertex);
	m_nStride = sizeof(CDiffused2TexturedVertex);
	m_nOffset = 0;
	m_nSlot = 0;
	
	m_nWidth = nWidth;
	m_nLength = nLength;
	m_xmf3Scale = xmf3Scale;

	//	CTexturedVertex *m_pVertices = new CTexturedVertex[m_nVertices];
	m_pVertices = new CDiffused2TexturedVertex[m_vertexCount];

	HeightMapImage *pHeightMapImage = (HeightMapImage *)pContext;
	int cxHeightMap = pHeightMapImage->GetHeightMapWidth();
	int czHeightMap = pHeightMapImage->GetHeightMapLength();

#ifdef _WITH_DEBUG_TERRAIN_MAX_POS
	XMFLOAT3 max{ 0.f, 0.f, 0.f };
#endif
	float fHeight = 0.0f, fMinHeight = +FLT_MAX, fMaxHeight = -FLT_MAX;
	for (int i = 0, z = zStart; z < (zStart + nLength); z++)
	{
		for (int x = xStart; x < (xStart + nWidth); x++, i++)
		{
			fHeight = OnGetHeight(x, z, pContext);
			m_pVertices[i].m_xmf3Position = XMFLOAT3((x*m_xmf3Scale.x), fHeight, (z*m_xmf3Scale.z));
#ifdef _WITH_DEBUG_TERRAIN_MAX_POS
			if (max.x < m_pVertices[i].m_xmf3Position.x) max.x = m_pVertices[i].m_xmf3Position.x;
			if (max.y < m_pVertices[i].m_xmf3Position.y) max.y = m_pVertices[i].m_xmf3Position.y;
			if (max.z < m_pVertices[i].m_xmf3Position.z) max.z = m_pVertices[i].m_xmf3Position.z;
#endif
			m_pVertices[i].m_xmf4Diffuse = Vector4::Add(OnGetColor(x, z, pContext), xmf4Color);
			m_pVertices[i].m_xmf2TexCoord0 = XMFLOAT2(float(x) / float(cxHeightMap - 1), float(czHeightMap - 1 - z) / float(czHeightMap - 1));
			m_pVertices[i].m_xmf2TexCoord1 = XMFLOAT2(float(x) / float(m_xmf3Scale.x*0.5f), float(z) / float(m_xmf3Scale.z*0.5f));
			if (fHeight < fMinHeight) fMinHeight = fHeight;
			if (fHeight > fMaxHeight) fMaxHeight = fHeight;
		}
	}
#ifdef _WITH_DEBUG_TERRAIN_MAX_POS
	TCHAR pstrDebug[256] = { 0 };
	_stprintf_s(pstrDebug, 256, L"(Terrain value (x,y,z): %f, %f, %f)\n", max.x, max.y, max.z);
	OutputDebugString(pstrDebug);
#endif
	m_pPositionBuffer = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pVertices, m_nStride * m_vertexCount, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pPositionUploadBuffer);

	m_pVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW;

	m_pVertexBufferViews[0].BufferLocation = m_pPositionBuffer->GetGPUVirtualAddress();
	m_pVertexBufferViews[0].StrideInBytes = m_nStride;
	m_pVertexBufferViews[0].SizeInBytes = m_nStride * m_vertexCount;

	m_IndexCount = ((nWidth * 2)*(nLength - 1)) + ((nLength - 1) - 1);
	UINT *pnIndices = new UINT[m_IndexCount];

	for (int j = 0, z = 0; z < nLength - 1; z++)
	{
		if ((z % 2) == 0)
		{
			for (int x = 0; x < nWidth; x++)
			{
				if ((x == 0) && (z > 0)) pnIndices[j++] = (UINT)(x + (z * nWidth));
				pnIndices[j++] = (UINT)(x + (z * nWidth));
				pnIndices[j++] = (UINT)((x + (z * nWidth)) + nWidth);
			}
		}
		else
		{
			for (int x = nWidth - 1; x >= 0; x--)
			{
				if (x == (nWidth - 1)) pnIndices[j++] = (UINT)(x + (z * nWidth));
				pnIndices[j++] = (UINT)(x + (z * nWidth));
				pnIndices[j++] = (UINT)((x + (z * nWidth)) + nWidth);
			}
		}
	}

	m_pIndexBuffer = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, pnIndices, sizeof(UINT) * m_IndexCount, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_pIndexUploadBuffer);

	m_IndexBufferView.BufferLocation = m_pIndexBuffer->GetGPUVirtualAddress();
	m_IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_IndexBufferView.SizeInBytes = sizeof(UINT) * m_IndexCount;

	delete[] pnIndices;
}

TerrainMesh::~TerrainMesh()
{
	if (m_pVertices)
	{
		delete[] m_pVertices;
		m_pVertices = nullptr;
	}
	if (m_pVertexBufferViews)
	{
		delete[] m_pVertexBufferViews;
		m_pVertexBufferViews = nullptr;
	} 
}


float TerrainMesh::OnGetHeight(int x, int z, void * pContext)
{
	HeightMapImage *pHeightMapImage = (HeightMapImage *)pContext;
	BYTE *pHeightMapPixels = pHeightMapImage->GetHeightMapPixels();
	XMFLOAT3 xmf3Scale = pHeightMapImage->GetScale();
	int nWidth = pHeightMapImage->GetHeightMapWidth();
	float fHeight = pHeightMapPixels[x + (z*nWidth)] * xmf3Scale.y;
	return(fHeight);
}

XMFLOAT4 TerrainMesh::OnGetColor(int x, int z, void * pContext)
{
	XMFLOAT3 xmf3LightDirection = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	xmf3LightDirection = Vector3::Normalize(xmf3LightDirection);
	HeightMapImage *pHeightMapImage = (HeightMapImage *)pContext;
	XMFLOAT3 xmf3Scale = pHeightMapImage->GetScale();
	XMFLOAT4 xmf4IncidentLightColor(0.9f, 0.8f, 0.4f, 1.0f);
	float fScale = Vector3::DotProduct(pHeightMapImage->GetHeightMapNormal(x, z), xmf3LightDirection);
	fScale += Vector3::DotProduct(pHeightMapImage->GetHeightMapNormal(x + 1, z), xmf3LightDirection);
	fScale += Vector3::DotProduct(pHeightMapImage->GetHeightMapNormal(x + 1, z + 1), xmf3LightDirection);
	fScale += Vector3::DotProduct(pHeightMapImage->GetHeightMapNormal(x, z + 1), xmf3LightDirection);
	fScale = (fScale / 4.0f) + 0.05f;
	if (fScale > 1.0f) fScale = 1.0f;
	if (fScale < 0.25f) fScale = 0.25f;
	XMFLOAT4 xmf4Color = Vector4::Multiply(fScale, xmf4IncidentLightColor);
	return(xmf4Color);
}
