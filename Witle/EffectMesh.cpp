#include "stdafx.h"
#include "d3dUtil.h"
#include "MyDescriptorHeap.h"
#include "Texture.h"
#include "ShaderManager.h"
#include "EffectMesh.h"

#define CYILNDER_VERTEX_COUNT 36
 
void EffectMesh::ReleaseObjects()
{
	Mesh::ReleaseObjects();
	 
	if (m_pVertexBufferViews)
	{
		delete[] m_pVertexBufferViews;
		m_pVertexBufferViews = nullptr;
	}
}

void EffectMesh::ReleaseUploadBuffers()
{
	Mesh::ReleaseUploadBuffers(); 
}

EffectMesh::EffectMesh(GameObject * pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, UINT meshCount, float startTimeMax, float lifeTimeMax, XMFLOAT4 diffuse)
	:Mesh(pOwner)
{ 
	m_ComponenetID = MESH_TYPE_ID::CYLINDER_MESH;

	m_nVertexBufferViews = 1;
	m_pVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_nStride = sizeof(EffectVertex);
	m_vertexCount = meshCount * 6;

	int arr[12]{ -1,-1,		-1,1,	1,1,	-1,-1,	1,1,	1,-1 };

	float size = 0.5f;
	EffectVertex* pVertices = new EffectVertex[m_vertexCount];
	for (int x = 0; x < meshCount; ++x)
	{
		XMFLOAT3 randVelocity = XMFLOAT3(
			0.1f * (((float)rand() / (float)RAND_MAX) - 0.5f),
			0.1f * (((float)rand() / (float)RAND_MAX) - 0.5f), 
			0.1f * (((float)rand() / (float)RAND_MAX) - 0.5f)
		);
		float startTime = ((float)rand() / (float)RAND_MAX) * startTimeMax;
		float lifeTime = ((float)rand() / (float)RAND_MAX) * lifeTimeMax;
		for (int n = 0, index = 0; n < 6; ++n)
		{ 
			pVertices[(x * 6) + n].position = XMFLOAT3(size * arr[index++], size * arr[index++], 0.F);
			pVertices[(x * 6) + n].diffuse = diffuse;
			pVertices[(x * 6) + n].velocity = randVelocity;
			pVertices[(x * 6) + n].startTime = startTime;
			pVertices[(x * 6) + n].lifeTime = lifeTime;
		}
	}

	m_pPositionBuffer = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList,
		pVertices,
		m_nStride * m_vertexCount, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pPositionUploadBuffer
	);

	m_pVertexBufferViews[0].BufferLocation = m_pPositionBuffer->GetGPUVirtualAddress();
	m_pVertexBufferViews[0].StrideInBytes = m_nStride;
	m_pVertexBufferViews[0].SizeInBytes = m_nStride * m_vertexCount;
	 
	delete[] pVertices;
}

	EffectMesh::~EffectMesh()
{
}

void EffectMesh::CreateTexture(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const wchar_t *pszFileName)
{

}

void EffectMesh::Render(ID3D12GraphicsCommandList * commandList, bool isGBuffers)
{
	// SHADER_EFFECTMESH 아직 없음...
	//ShaderManager::GetInstance()->SetPSO(commandList, SHADER_EFFECTMESH, isGBuffers);
	//commandList->IASetPrimitiveTopology(GetPrimitiveTopology());

	//D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[] = { GetVertexBufferView(0) };
	//commandList->IASetVertexBuffers(0, _countof(pVertexBufferViews), pVertexBufferViews);

	//commandList->DrawInstanced(GetVertexCount(), 1, 0, 0);
}
