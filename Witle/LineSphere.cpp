#include "stdafx.h"
#include "d3dUtil.h"
#include "ShaderManager.h"
#include "LineSphere.h"

#define CUBE_VERTEX_COUNT 60 

void LineSphere::CalculateTriangleListVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, int nVertices)
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

void LineSphere::CalculateTriangleListVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, UINT nVertices, UINT * pnIndices, UINT nIndices)
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

void LineSphere::CalculateTriangleStripVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, UINT nVertices, UINT * pnIndices, UINT nIndices)
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

void LineSphere::CalculateVertexNormals(XMFLOAT3 * pxmf3Normals, XMFLOAT3 * pxmf3Positions, int nVertices, UINT * pnIndices, int nIndices)
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

void LineSphere::ReleaseObjects()
{ 
	Mesh::ReleaseObjects();
	 
	if (m_pVertexBufferViews)
	{
		delete[] m_pVertexBufferViews;
		m_pVertexBufferViews = nullptr;
	}
}

void LineSphere::ReleaseUploadBuffers()
{
	Mesh::ReleaseUploadBuffers();
}
  
LineSphere::LineSphere(GameObject * pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, float radius, float height, float topRadius, float bottomRadius, int sectorCount, int stackCount)
	:Mesh(pOwner)
{
	m_ComponenetID = MESH_TYPE_ID::CYLINDER_MESH;

	m_nVertexBufferViews = 1;
	m_pVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];
	 
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;

	m_nStride = sizeof(SphereVertex);

	// 출처: http://www.songho.ca/opengl/gl_sphere.html

	// clear memory of prev arrays
	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	SphereVertex* pControlPointVertices = new SphereVertex[(stackCount + 1) * (sectorCount + 1)];
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

LineSphere::~LineSphere()
{
}

void LineSphere::Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT4X4& world, bool isMoved)
{
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, "Line");

	XMFLOAT4X4 xmf4x4World;
	if (isMoved)
	{
		XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&world)));
	}
	else
	{
		XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&Matrix4x4::Identity())));
	}
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 16, &xmf4x4World, 0);

	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[1] = { m_pVertexBufferViews[0] };
	pd3dCommandList->IASetVertexBuffers(m_nSlot, 1, pVertexBufferViews);

	pd3dCommandList->DrawInstanced(m_vertexCount, 1, m_nOffset, 0);
}

void LineSphere::RenderInstancing(ID3D12GraphicsCommandList * pd3dCommandList, int Instancingcount)
{
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[1] = { m_pVertexBufferViews[0] };
	pd3dCommandList->IASetVertexBuffers(m_nSlot, 1, pVertexBufferViews);

	pd3dCommandList->DrawInstanced(m_vertexCount, Instancingcount, m_nOffset, 0);
}
