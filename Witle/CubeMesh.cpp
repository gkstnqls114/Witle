#include "stdafx.h"
#include "d3dUtil.h"
#include "CubeMesh.h"
 
#define CUBE_VERTEX_COUNT 36

void CubeMesh::CalculateTriangleListVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, int nVertices)
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

void CubeMesh::CalculateTriangleListVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, UINT nVertices, UINT * pnIndices, UINT nIndices)
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

void CubeMesh::CalculateTriangleStripVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, UINT nVertices, UINT * pnIndices, UINT nIndices)
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

void CubeMesh::CalculateVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, int nVertices, UINT * pnIndices, int nIndices)
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

CubeMesh::CubeMesh(GameObject* pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, float width, float height, float depth)
	:Mesh(pOwner)
{
	m_ComponenetID = MESH_TYPE_ID::CUBE_MESH;

	m_nVertexBufferViews = 1;
	m_pVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_vertexCount = CUBE_VERTEX_COUNT;
	m_nStride = sizeof(CubeVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = width * 0.5f;
	float fy = height * 0.5f;
	float fz = depth * 0.5f;

	XMFLOAT3 pxmf3Positions[CUBE_VERTEX_COUNT];
	int i = 0;
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, -fz);

	pxmf3Positions[i++] = XMFLOAT3(-fx, +fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(-fx, -fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);

	pxmf3Positions[i++] = XMFLOAT3(+fx, +fy, -fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, +fz);
	pxmf3Positions[i++] = XMFLOAT3(+fx, -fy, -fz);

	XMFLOAT4 pxmf3Color[CUBE_VERTEX_COUNT];
	int Ci = 0;
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Red);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Red);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Red);

	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Pink);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Pink);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Pink);

	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Green);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Green);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Green);

	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::GreenYellow);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::GreenYellow);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::GreenYellow);

	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Blue);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Blue);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Blue);

	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Aqua);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Aqua);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Aqua);

	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::White);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::White);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::White);

	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Gray);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Gray);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Gray);

	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Yellow);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Yellow);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Yellow);

	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Orange);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Orange);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Orange);

	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Violet);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Violet);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Violet);

	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Violet);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Violet);
	pxmf3Color[Ci++] = Vector4::XMVectorToFloat4(Colors::Violet);

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

CubeMesh::~CubeMesh()
{
	if (m_pVertexBufferViews) {
		delete[] m_pVertexBufferViews;
		m_pVertexBufferViews = nullptr;
	}

}
