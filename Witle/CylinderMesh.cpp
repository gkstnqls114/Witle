#include "stdafx.h"
#include "d3dUtil.h"
#include "MyDescriptorHeap.h"
#include "Texture.h"
#include "ShaderManager.h"
#include "CylinderMesh.h"

#define CYILNDER_VERTEX_COUNT 36

void CylinderMesh::CalculateTriangleListVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, int nVertices)
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

void CylinderMesh::CalculateTriangleListVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, UINT nVertices, UINT * pnIndices, UINT nIndices)
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

void CylinderMesh::CalculateTriangleStripVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, UINT nVertices, UINT * pnIndices, UINT nIndices)
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

void CylinderMesh::CalculateVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, int nVertices, UINT * pnIndices, int nIndices)
{
	switch (GetPrimitiveTopology())
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

void CylinderMesh::ReleaseObjects()
{
	Mesh::ReleaseObjects();

	if (m_Heap)
	{
		m_Heap->ReleaseObjects();
		delete m_Heap;
		m_Heap = nullptr;
	}
	if (m_Texture)
	{
		delete m_Texture;
		m_Texture = nullptr;
	}
	if (m_pVertexBufferViews)
	{
		delete[] m_pVertexBufferViews;
		m_pVertexBufferViews = nullptr;
	}
}

void CylinderMesh::ReleaseUploadBuffers()
{
	Mesh::ReleaseUploadBuffers();
	if(m_Texture) m_Texture->ReleaseUploadBuffers();
}

CylinderMesh::CylinderMesh(GameObject* pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, float bottomRadius, float topRadius, float height, int sliceCount, int stackCount)
	:Mesh(pOwner)
{
	m_ComponenetID = MESH_TYPE_ID::CYLINDER_MESH;

	m_nVertexBufferViews = 1;
	m_pVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];
	 
	m_nStride = sizeof(CylinderVertex);

	float stackHeight = height / stackCount;
	float radiusStep = (topRadius - bottomRadius) / stackCount;
	int ringCount = stackCount + 1;

	CylinderVertex* pControlPointVertices;
	pControlPointVertices = new CylinderVertex[ringCount * (sliceCount + 1) + 1];
	int index = 0;
	for (int i = 0; i < ringCount; i++) {
		float y = -0.5f*height + i * stackHeight;
		float r = bottomRadius + i * radiusStep;
		float dTheta = 2.0f* float(PI) / float(sliceCount);
		for (int j = 0; j <= sliceCount; j++) {

			float c = cos(j*dTheta);
			float s = sin(j*dTheta);

			XMFLOAT3 v( r*c, y, r*s );
			XMFLOAT2 uv ((float)j / sliceCount, 1.0f - (float)i / stackCount);
			XMFLOAT3 t (-s, 0.0f, c);

			int dr = bottomRadius - topRadius;
			XMFLOAT3 bitangent (dr*c, -height, dr*s);

			XMFLOAT3 n = Vector3::CrossProduct(t, bitangent);
			n = Vector3::Normalize(n);

			pControlPointVertices[index] = CylinderVertex(v, n);
			pControlPointVertices[index].uv = uv;
			index++;
		}
	} 

	m_vertexCount = stackCount * sliceCount * 6 + 1;
	CylinderVertex* pVertices;
	pVertices = new CylinderVertex[m_vertexCount];

	int k = 0;
	int ringVertexCount = sliceCount + 1;
	for (int i = 0; i < stackCount; i++) {
		for (int j = 0; j < sliceCount; j++) {
			pVertices[k++] = pControlPointVertices[i * ringVertexCount + j];
			pVertices[k++] = pControlPointVertices[(i + 1) * ringVertexCount + j];
			pVertices[k++] = pControlPointVertices[(i + 1) * ringVertexCount + j + 1]; 

			pVertices[k++] = pControlPointVertices[i * ringVertexCount + j];
			pVertices[k++] = pControlPointVertices[(i + 1) * ringVertexCount + j + 1];
			pVertices[k++] = pControlPointVertices[i * ringVertexCount + j + 1];
		}
	}  
	// BuildCylinderTopCap(topRadius, height, sliceCount, ref ret);
	// BuildCylinderBottomCap(bottomRadius, height, sliceCount, ref ret);
	  
	m_pPositionBuffer = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices,
		m_nStride * m_vertexCount, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pPositionUploadBuffer);

	m_pVertexBufferViews[0].BufferLocation = m_pPositionBuffer->GetGPUVirtualAddress();
	m_pVertexBufferViews[0].StrideInBytes = m_nStride;
	m_pVertexBufferViews[0].SizeInBytes = m_nStride * m_vertexCount;
	
	delete[] pVertices;
	delete[] pControlPointVertices;
}

CylinderMesh::~CylinderMesh()
{
}

void CylinderMesh::CreateTexture(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const wchar_t *pszFileName)
{
	m_Heap = new MyDescriptorHeap();
	m_Heap->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1, 0, ENUM_SCENE::SCENE_GAME);
	m_Texture = new Texture(ENUM_SCENE::SCENE_GAME, 1);
	m_Texture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pszFileName, 0);
	m_Heap->CreateShaderResourceViews(pd3dDevice,  m_Texture, ROOTPARAMETER_TEXTURE, false);
}

void CylinderMesh::Render(ID3D12GraphicsCommandList * commandList, bool isGBuffers)
{ 
	ShaderManager::GetInstance()->SetPSO(commandList, "Cube", isGBuffers);
	commandList->IASetPrimitiveTopology( GetPrimitiveTopology());

	if (m_Heap) m_Heap->UpdateShaderVariable(commandList);
	if (m_Texture) m_Texture->UpdateShaderVariables(commandList);

	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[] = { GetVertexBufferView(0) };
	commandList->IASetVertexBuffers(0, _countof(pVertexBufferViews), pVertexBufferViews);

	commandList->DrawInstanced( GetVertexCount(), 1, 0, 0);
}
