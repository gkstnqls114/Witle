#pragma once
#include "ComponentBase.h"

// Mesh Class
// 모델 안에 애니메이션 정보(정점 안에 들어가는 뼈대에 대한 정보)가 들어가지 않습니다.
class Mesh :
	public ComponentBase
{
protected:
	// 위치 버퍼
	UINT							m_vertexCount{ 0 }; // 인덱스 버퍼가 없을 경우 사용함
	XMFLOAT3						*m_pxmf3Positions{ nullptr };
	ID3D12Resource					*m_pPositionBuffer{ nullptr };
	ID3D12Resource					*m_pPositionUploadBuffer{ nullptr };

	// 정점에 대한 버퍼 서술자
	UINT							m_nVertexBufferViews{ 1 };
	D3D12_VERTEX_BUFFER_VIEW		*m_pVertexBufferViews{ nullptr };

	// 인덱스 버퍼
	UINT							m_IndexCount{ 0 };
	UINT		                    *m_pnIndices{ nullptr };
	ID3D12Resource                  *m_pIndexBuffer{ nullptr };
	ID3D12Resource                  *m_pIndexUploadBuffer{ nullptr };

	D3D12_INDEX_BUFFER_VIEW			m_IndexBufferView;

	D3D12_PRIMITIVE_TOPOLOGY		m_d3dPrimitiveTopology;
	UINT							m_nSlot = 0;
	UINT							m_nStride = 0;
	UINT							m_nOffset = 0;

	UINT							m_nStartIndex = 0;
	int								m_nBaseVertex = 0;


protected:
	Mesh() {};

public:
	Mesh(GameObject* pOwner);
	virtual ~Mesh();

	void ReleaseUploadBuffers();

	/////////////////////////////////////////////////////////////////////////// Get
	UINT GetVertexCount() const { return m_vertexCount; }
	const ID3D12Resource* GetPositonBuffer() const { m_pPositionBuffer; }
	const ID3D12Resource* GetPositionUploadBuffer() const { m_pPositionUploadBuffer; }

	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView(int index) const { return m_pVertexBufferViews[index]; }
	UINT GetVertexBufferViewCount() const { return m_nVertexBufferViews; }

	UINT GetIndexCount() const { return m_IndexCount; }
	const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView() const { return m_IndexBufferView; }

	D3D12_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const { return m_d3dPrimitiveTopology; }
	/////////////////////////////////////////////////////////////////////////// Get

	
};

