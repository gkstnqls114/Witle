#include "stdafx.h"
#include "d3dUtil.h"
#include "LineShader.h"
#include "LineCube.h"

#define CUBE_VERTEX_COUNT 60 

void LineCube::CalculateTriangleListVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, int nVertices)
{
	int nPrimitives = nVertices / 3;
	UINT nIndex0, nIndex1, nIndex2;
	for (int i = 0; i < nPrimitives; i++)
	{
		nIndex0 = i * 3 + 0;
		nIndex1 = i * 3 + 1;
		nIndex2 = i * 3 + 2;
		XMFLOAT3 xmf3Edge01 = Vector3::Subtract(pxmf3Positions[nIndex1], pxmf3Positions[nIndex0]);
		XMFLOAT3 xmf3Edge02 = Vector3::Subtract(pxmf3Positions[nIndex2], pxmf3Positions[nIndex0]);
		pxmf3Normals[nIndex0] = pxmf3Normals[nIndex1] = pxmf3Normals[nIndex2] = Vector3::CrossProduct(xmf3Edge01, xmf3Edge02, true);
	}
}

void LineCube::CalculateTriangleListVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, UINT nVertices, UINT * pnIndices, UINT nIndices)
{
	UINT nPrimitives = (pnIndices) ? (nIndices / 3) : (nVertices / 3);
	XMFLOAT3 xmf3SumOfNormal, xmf3Edge01, xmf3Edge02, xmf3Normal;
	UINT nIndex0, nIndex1, nIndex2;
	for (UINT j = 0; j < nVertices; j++)
	{
		xmf3SumOfNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		for (UINT i = 0; i < nPrimitives; i++)
		{
			nIndex0 = pnIndices[i * 3 + 0];
			nIndex1 = pnIndices[i * 3 + 1];
			nIndex2 = pnIndices[i * 3 + 2];
			if (pnIndices && ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j)))
			{
				xmf3Edge01 = Vector3::Subtract(pxmf3Positions[nIndex1], pxmf3Positions[nIndex0]);
				xmf3Edge02 = Vector3::Subtract(pxmf3Positions[nIndex2], pxmf3Positions[nIndex0]);
				xmf3Normal = Vector3::CrossProduct(xmf3Edge01, xmf3Edge02, false);
				xmf3SumOfNormal = Vector3::Add(xmf3SumOfNormal, xmf3Normal);
			}
		}
		pxmf3Normals[j] = Vector3::Normalize(xmf3SumOfNormal);
	}
}

void LineCube::CalculateTriangleStripVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, UINT nVertices, UINT * pnIndices, UINT nIndices)
{
	UINT nPrimitives = (pnIndices) ? (nIndices - 2) : (nVertices - 2);
	XMFLOAT3 xmf3SumOfNormal(0.0f, 0.0f, 0.0f);
	UINT nIndex0, nIndex1, nIndex2;
	for (UINT j = 0; j < nVertices; j++)
	{
		xmf3SumOfNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		for (UINT i = 0; i < nPrimitives; i++)
		{
			nIndex0 = ((i % 2) == 0) ? (i + 0) : (i + 1);
			if (pnIndices) nIndex0 = pnIndices[nIndex0];
			nIndex1 = ((i % 2) == 0) ? (i + 1) : (i + 0);
			if (pnIndices) nIndex1 = pnIndices[nIndex1];
			nIndex2 = (pnIndices) ? pnIndices[i + 2] : (i + 2);
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j))
			{
				XMFLOAT3 xmf3Edge01 = Vector3::Subtract(pxmf3Positions[nIndex1], pxmf3Positions[nIndex0]);
				XMFLOAT3 xmf3Edge02 = Vector3::Subtract(pxmf3Positions[nIndex2], pxmf3Positions[nIndex0]);
				XMFLOAT3 xmf3Normal = Vector3::CrossProduct(xmf3Edge01, xmf3Edge02, true);
				xmf3SumOfNormal = Vector3::Add(xmf3SumOfNormal, xmf3Normal);
			}
		}
		pxmf3Normals[j] = Vector3::Normalize(xmf3SumOfNormal);
	}
}

void LineCube::CalculateVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, int nVertices, UINT * pnIndices, int nIndices)
{
	switch (m_d3dPrimitiveTopology)
	{
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		if (pnIndices)
			CalculateTriangleListVertexNormals(pxmf3Normals, pxmf3Positions, nVertices, pnIndices, nIndices);
		else
			CalculateTriangleListVertexNormals(pxmf3Normals, pxmf3Positions, nVertices);
		break;
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		CalculateTriangleStripVertexNormals(pxmf3Normals, pxmf3Positions, nVertices, pnIndices, nIndices);
		break;
	default:
		break;
	}
}

LineCube::LineCube(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, XMFLOAT3 center, XMFLOAT3 extents, bool isMoved)
{
	m_isMoved = isMoved;
	m_ComponenetID = MESH_TYPE_ID::CUBE_MESH;

	m_nVertexBufferViews = 1;
	m_pVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_vertexCount = CUBE_VERTEX_COUNT;
	m_nStride = sizeof(CubeVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;

	float fx = extents.x * 0.5f;
	float fy = extents.y * 0.5f;
	float fz = extents.z * 0.5f;

	XMFLOAT3 pxmf3Positions[CUBE_VERTEX_COUNT];
	int i = 0;
	// 첫번째면 (-fz 고정)
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, +fy + center.y, -fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, +fy + center.y, -fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, +fy + center.y, -fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, -fy + center.y, -fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, -fy + center.y, -fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, -fy + center.y, -fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, -fy + center.y, -fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, +fy + center.y, -fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, +fy + center.y, -fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, -fy + center.y, -fz + center.z);

	// 두 번째면 (+fy 고정)
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, +fy + center.y, +fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, +fy + center.y, +fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, +fy + center.y, +fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, +fy + center.y, -fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, +fy + center.y, -fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, +fy + center.y, -fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, +fy + center.y, -fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, +fy + center.y, +fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, +fy + center.y, -fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, +fy + center.y, -fz + center.z);


	// 세 번째 면 (+fz 고정)
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, -fy + center.y, +fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, -fy + center.y, +fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, -fy + center.y, +fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, +fy + center.y, +fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, -fy + center.y, +fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, -fy + center.y, +fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, -fy + center.y, +fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, +fy + center.y, +fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, +fy + center.y, +fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, +fy + center.y, +fz + center.z);


	// 네 번째면 (-fy 고정)
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, -fy + center.y, -fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, -fy + center.y, -fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, -fy + center.y, -fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, -fy + center.y, +fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, -fy + center.y, +fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, -fy + center.y, -fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, -fy + center.y, +fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, -fy + center.y, +fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, -fy + center.y, +fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, -fy + center.y, +fz + center.z);

	// 다섯번째면 (-fx 고정)
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, +fy + center.y, +fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, +fy + center.y, -fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, +fy + center.y, -fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, -fy + center.y, -fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, -fy + center.y, -fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, +fy + center.y, +fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, -fy + center.y, -fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, -fy + center.y, +fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, -fy + center.y, +fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(-fx + center.x, -fy + center.y, +fz + center.z);

	// 여섯 번째면 (+fx 고정)
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, +fy + center.y, -fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, +fy + center.y, +fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, +fy + center.y, +fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, -fy + center.y, +fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, -fy + center.y, +fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, +fy + center.y, -fz + center.z); 
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, +fy + center.y, -fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, -fy + center.y, +fz + center.z);  
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, -fy + center.y, +fz + center.z);
	pxmf3Positions[i++] = XMFLOAT3(+fx + center.x, -fy + center.y, -fz + center.z);

	XMFLOAT4 pxmf3Color[CUBE_VERTEX_COUNT];
	for (int Ci = 0; Ci < CUBE_VERTEX_COUNT; ++Ci)
	{
		pxmf3Color[Ci] = Vector4::XMVectorToFloat4(Colors::Red);
	}

	XMFLOAT3 pxmf3Normals[CUBE_VERTEX_COUNT];
	CalculateVertexNormals(pxmf3Normals, pxmf3Positions, m_vertexCount, NULL, 0);

	CubeVertex pVertices[CUBE_VERTEX_COUNT];
	for (int x = 0; x < CUBE_VERTEX_COUNT; ++x)
	{
		pVertices[x] = CubeVertex(pxmf3Positions[x], pxmf3Normals[x], pxmf3Color[x]);
	}

	m_pPositionBuffer = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices,
		m_nStride * m_vertexCount, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pPositionUploadBuffer);

	m_pVertexBufferViews[0].BufferLocation = m_pPositionBuffer->GetGPUVirtualAddress();
	m_pVertexBufferViews[0].StrideInBytes = m_nStride;
	m_pVertexBufferViews[0].SizeInBytes = m_nStride * m_vertexCount;
}

LineCube::LineCube(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, float width, float height, float depth, bool isMoved)
{
	m_isMoved = isMoved;
	m_ComponenetID = MESH_TYPE_ID::CUBE_MESH;

	m_nVertexBufferViews = 1;
	m_pVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_vertexCount = CUBE_VERTEX_COUNT;
	m_nStride = sizeof(CubeVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;

	float fx = width * 0.5f;
	float fy = height * 0.5f;
	float fz = depth * 0.5f;

	XMFLOAT3 pxmf3Positions[CUBE_VERTEX_COUNT];
	int i = 0;
	// 첫번째면 (-fz 고정)
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, -fz);

	// 두 번째면 (+fy 고정)
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);


	// 세 번째 면 (+fz 고정)
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, +fz);


	// 네 번째면 (-fy 고정)
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);

	// 다섯번째면 (-fx 고정)
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);

	// 여섯 번째면 (+fx 고정)
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, -fz);

	XMFLOAT4 pxmf3Color[CUBE_VERTEX_COUNT];
	for (int Ci = 0; Ci < CUBE_VERTEX_COUNT; ++Ci)
	{
		pxmf3Color[Ci] = Vector4::XMVectorToFloat4(Colors::Red);
	}

	XMFLOAT3 pxmf3Normals[CUBE_VERTEX_COUNT];
	CalculateVertexNormals(pxmf3Normals, pxmf3Positions, m_vertexCount, NULL, 0);

	CubeVertex pVertices[CUBE_VERTEX_COUNT];
	for (int x = 0; x < CUBE_VERTEX_COUNT; ++x)
	{
		pVertices[x] = CubeVertex(pxmf3Positions[x], pxmf3Normals[x], pxmf3Color[x]);
	}

	m_pPositionBuffer = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices,
		m_nStride * m_vertexCount, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pPositionUploadBuffer);

	m_pVertexBufferViews[0].BufferLocation = m_pPositionBuffer->GetGPUVirtualAddress();
	m_pVertexBufferViews[0].StrideInBytes = m_nStride;
	m_pVertexBufferViews[0].SizeInBytes = m_nStride * m_vertexCount;

}

LineCube::LineCube(GameObject* pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, float width, float height, float depth)
	:Mesh(pOwner)
{
	m_ComponenetID = MESH_TYPE_ID::CUBE_MESH;

	m_nVertexBufferViews = 1;
	m_pVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_vertexCount = CUBE_VERTEX_COUNT;
	m_nStride = sizeof(CubeVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;

	float fx = width * 0.5f;
	float fy = height * 0.5f;
	float fz = depth * 0.5f;

	XMFLOAT3 pxmf3Positions[CUBE_VERTEX_COUNT];
	int i = 0;
	// 첫번째면 (-fz 고정)
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, -fz);

	// 두 번째면 (+fy 고정)
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);


	// 세 번째 면 (+fz 고정)
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, +fz);

	// 네 번째면 (-fy 고정)
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);

	// 다섯번째면 (-fx 고정)
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);

	// 여섯 번째면 (+fx 고정)
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, -fz);

	XMFLOAT4 pxmf3Color[CUBE_VERTEX_COUNT];
	for (int Ci = 0; Ci < CUBE_VERTEX_COUNT; ++Ci)
	{
		pxmf3Color[Ci] = Vector4::XMVectorToFloat4(Colors::Red);
	}
	
	XMFLOAT3 pxmf3Normals[CUBE_VERTEX_COUNT];
	CalculateVertexNormals(pxmf3Normals, pxmf3Positions, m_vertexCount, NULL, 0);

	CubeVertex pVertices[CUBE_VERTEX_COUNT];
	for (int x = 0; x < CUBE_VERTEX_COUNT; ++x)
	{
		pVertices[x] = CubeVertex(pxmf3Positions[x], pxmf3Normals[x], pxmf3Color[x]);
	}

	m_pPositionBuffer = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices,
		m_nStride * m_vertexCount, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pPositionUploadBuffer);

	m_pVertexBufferViews[0].BufferLocation = m_pPositionBuffer->GetGPUVirtualAddress();
	m_pVertexBufferViews[0].StrideInBytes = m_nStride;
	m_pVertexBufferViews[0].SizeInBytes = m_nStride * m_vertexCount;

}

LineCube::~LineCube()
{
	if (m_pVertexBufferViews) {
		delete[] m_pVertexBufferViews;
		m_pVertexBufferViews = nullptr;
	}

}

void LineCube::Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT4X4& world)
{ 
	XMFLOAT4X4 xmf4x4World;
	//if (m_isMoved)
	//{
		// XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&world))); 
	//}
	//else 
	//{ 
		XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&Matrix4x4::Identity())));
	//}
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 16, &xmf4x4World, 0);
	
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[1] = { m_pVertexBufferViews[0] };
	pd3dCommandList->IASetVertexBuffers(m_nSlot, 1, pVertexBufferViews);

	pd3dCommandList->DrawInstanced(m_vertexCount, 1, m_nOffset, 0); 
}
