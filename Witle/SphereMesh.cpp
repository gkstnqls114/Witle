#include "stdafx.h"
#include "d3dUtil.h"
#include "MyDescriptorHeap.h"
#include "Texture.h"
#include "ShaderManager.h"
#include "Shader.h"
#include "SphereMesh.h"

#define CYILNDER_VERTEX_COUNT 36

void SphereMesh::CalculateTriangleListVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, int nVertices)
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

void SphereMesh::CalculateTriangleListVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, UINT nVertices, UINT * pnIndices, UINT nIndices)
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

void SphereMesh::CalculateTriangleStripVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, UINT nVertices, UINT * pnIndices, UINT nIndices)
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

void SphereMesh::CalculateVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, int nVertices, UINT * pnIndices, int nIndices)
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

void SphereMesh::ReleaseObjects()
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

void SphereMesh::ReleaseUploadBuffers()
{
	Mesh::ReleaseUploadBuffers();
	if (m_Texture) m_Texture->ReleaseUploadBuffers();
}

SphereMesh::SphereMesh(GameObject* pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList,
	bool isBlend, float radius , float height , float topRadius , float bottomRadius , int sectorCount, int stackCount)
	:Mesh(pOwner)
{
	if(isBlend)
	{
		m_pShader = ShaderManager::GetInstance()->GetShader(SHADER_BLENDMESH);
	}
	else
	{
		m_pShader = ShaderManager::GetInstance()->GetShader(SHADER_CUBE);
	}
	m_ComponenetID = MESH_TYPE_ID::CYLINDER_MESH;

	m_nVertexBufferViews = 1;
	m_pVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_nStride = sizeof(SphereVertex);
	 
	// 출처: http://www.songho.ca/opengl/gl_sphere.html

	// clear memory of prev arrays
	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	SphereVertex* pControlPointVertices = new SphereVertex[(stackCount + 1) * (sectorCount+ 1)];
	int Cindex = 0;
	for (int i = 0; i < stackCount + 1; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j < sectorCount + 1; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			pControlPointVertices[Cindex].position = XMFLOAT3(x, y, z);

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			pControlPointVertices[Cindex].normal = XMFLOAT3(nx, ny, nz);
			 
			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / sectorCount;
			t = (float)i / stackCount;
			pControlPointVertices[Cindex].uv = XMFLOAT2(s, t); 

			Cindex += 1;
		}
	}

	// generate CCW index list of sphere triangles 
	int maybe_vertexCount = stackCount * sectorCount * 6; // 최대 크기로 더 많이 할당한다.
	SphereVertex* pVertices = new SphereVertex[maybe_vertexCount];
	int k1, k2;
	int index = 0;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack
		 
		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				pVertices[index++] = pControlPointVertices[k1];
				pVertices[index++] = pControlPointVertices[k2];
				pVertices[index++] = pControlPointVertices[k1 + 1]; 
			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{ 
				pVertices[index++] = pControlPointVertices[k1 + 1];
				pVertices[index++] = pControlPointVertices[k2];
				pVertices[index++] = pControlPointVertices[k2 + 1]; 
			}  
		}
	} 
	m_vertexCount = index; 

	m_pPositionBuffer = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, 
		pVertices,
		m_nStride * m_vertexCount, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pPositionUploadBuffer
	);

	m_pVertexBufferViews[0].BufferLocation = m_pPositionBuffer->GetGPUVirtualAddress();
	m_pVertexBufferViews[0].StrideInBytes = m_nStride;
	m_pVertexBufferViews[0].SizeInBytes = m_nStride * m_vertexCount;
	 
	delete[] pControlPointVertices;
	delete[] pVertices;
}

SphereMesh::~SphereMesh()
{
}

void SphereMesh::CreateTexture(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const wchar_t *pszFileName)
{
	m_Heap = new MyDescriptorHeap();
	m_Heap->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1, 0);
	m_Texture = new Texture(1);
	m_Texture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pszFileName, 0);
	m_Heap->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_Texture, ROOTPARAMETER_TEXTURE, false);
}

void SphereMesh::Render(ID3D12GraphicsCommandList * commandList, bool isGBuffers)
{
	if(isGBuffers) m_pShader->OnPrepareRenderForGBuffers(commandList);
	else m_pShader->OnPrepareRender(commandList);

	commandList->IASetPrimitiveTopology(GetPrimitiveTopology());

	if (m_Heap) m_Heap->UpdateShaderVariable(commandList);
	if (m_Texture) m_Texture->UpdateShaderVariables(commandList);

	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[] = { GetVertexBufferView(0) };
	commandList->IASetVertexBuffers(0, _countof(pVertexBufferViews), pVertexBufferViews);

	commandList->DrawInstanced(GetVertexCount(), 1, 0, 0);
}
