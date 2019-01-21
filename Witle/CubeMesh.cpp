#include "stdafx.h"
#include "d3dUtil.h"
#include "CubeMesh.h"
 

CubeMesh::CubeMesh(GameObject* pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, float width, float height, float depth)
	:Mesh(pOwner)
{
	m_ComponenetID = MESH_TYPE_ID::CUBE_MESH;

	m_nVertexBufferViews = 1;
	m_pVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	//직육면체는 꼭지점(정점)이 8개이다.
	m_VertexCount = 8;
	m_nStride = sizeof(CDiffusedVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	float fx = width * 0.5f;
	float fy = height * 0.5f;
	float fz = depth * 0.5f;
	//정점 버퍼는 직육면체의 꼭지점 8개에 대한 정점 데이터를 가진다.
	CDiffusedVertex pVertices[8];
	pVertices[0] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), RANDOM_COLOR);
	pVertices[1] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), RANDOM_COLOR);
	pVertices[2] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), RANDOM_COLOR);
	pVertices[3] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), RANDOM_COLOR);
	pVertices[4] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), RANDOM_COLOR);
	pVertices[5] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), RANDOM_COLOR);
	pVertices[6] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), RANDOM_COLOR);
	pVertices[7] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), RANDOM_COLOR);
	m_pPositionBuffer = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices,
		m_nStride * m_VertexCount, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pPositionUploadBuffer);
	m_pVertexBufferViews[0].BufferLocation = m_pPositionBuffer->GetGPUVirtualAddress();

	m_pVertexBufferViews[0].StrideInBytes = m_nStride;
	m_pVertexBufferViews[0].SizeInBytes = m_nStride * m_VertexCount;
	/*인덱스 버퍼는 직육면체의 6개의 면(사각형)에 대한 기하 정보를 갖는다. 삼각형 리스트로 직육면체를 표현할 것이
	므로 각 면은 2개의 삼각형을 가지고 각 삼각형은 3개의 정점이 필요하다. 즉, 인덱스 버퍼는 전체 36(=6*2*3)개의 인
	덱스를 가져야 한다.*/
	m_IndexCount = 36;
	UINT pnIndices[36];
	//ⓐ 앞면(Front) 사각형의 위쪽 삼각형
	pnIndices[0] = 3; pnIndices[1] = 1; pnIndices[2] = 0;
	//ⓑ 앞면(Front) 사각형의 아래쪽 삼각형
	pnIndices[3] = 2; pnIndices[4] = 1; pnIndices[5] = 3;
	//ⓒ 윗면(Top) 사각형의 위쪽 삼각형
	pnIndices[6] = 0; pnIndices[7] = 5; pnIndices[8] = 4;
	//ⓓ 윗면(Top) 사각형의 아래쪽 삼각형
	pnIndices[9] = 1; pnIndices[10] = 5; pnIndices[11] = 0;
	//ⓔ 뒷면(Back) 사각형의 위쪽 삼각형
	pnIndices[12] = 3; pnIndices[13] = 4; pnIndices[14] = 7;
	//ⓕ 뒷면(Back) 사각형의 아래쪽 삼각형
	pnIndices[15] = 0; pnIndices[16] = 4; pnIndices[17] = 3;
	//ⓖ 아래면(Bottom) 사각형의 위쪽 삼각형
	pnIndices[18] = 1; pnIndices[19] = 6; pnIndices[20] = 5;
	//ⓗ 아래면(Bottom) 사각형의 아래쪽 삼각형
	pnIndices[21] = 2; pnIndices[22] = 6; pnIndices[23] = 1;
	//ⓘ 옆면(Left) 사각형의 위쪽 삼각형
	pnIndices[24] = 2; pnIndices[25] = 7; pnIndices[26] = 6;
	//ⓙ 옆면(Left) 사각형의 아래쪽 삼각형
	pnIndices[27] = 3; pnIndices[28] = 7; pnIndices[29] = 2;
	//ⓚ 옆면(Right) 사각형의 위쪽 삼각형
	pnIndices[30] = 6; pnIndices[31] = 4; pnIndices[32] = 5;
	//ⓛ 옆면(Right) 사각형의 아래쪽 삼각형
	pnIndices[33] = 7; pnIndices[34] = 4; pnIndices[35] = 6;
	//인덱스 버퍼를 생성한다.

	m_pIndexBuffer = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, pnIndices,
		sizeof(UINT) * m_IndexCount, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER,
		&m_pIndexUploadBuffer);
	//인덱스 버퍼 뷰를 생성한다.
	m_IndexBufferView.BufferLocation = m_pIndexBuffer->GetGPUVirtualAddress();
	m_IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_IndexBufferView.SizeInBytes = sizeof(UINT) * m_IndexCount;
}

CubeMesh::~CubeMesh()
{
	if (m_pVertexBufferViews) {
		delete[] m_pVertexBufferViews;
		m_pVertexBufferViews = nullptr;
	}

}

void CubeMesh::Create()
{
}

void CubeMesh::Init()
{
}
