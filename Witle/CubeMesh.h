#pragma once
#include "Mesh.h"

class CubeMesh :
	public Mesh
{
public:
	CubeMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float width, float height, float depth);
	virtual ~CubeMesh();
	 
	virtual void Create();
	virtual void Init();
	void Update() {};
	
	ID3D12Resource* GetPositionBuffer() const { m_pPositionBuffer; }
	ID3D12Resource* GetPositionUploadBuffer() const { m_pPositionUploadBuffer; }
	UINT GetVertexCount() const { return m_VertexCount; }

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(int index) const { return m_pVertexBufferViews[index]; }
	UINT GetVertexBufferViewCount() const { return m_nVertexBufferViews; }

	UINT GetIndexCount() const { return m_IndexCount; }
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const { return m_IndexBufferView; }


private:
	int m_nStride{ 0 };
	D3D12_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;

	// 위치 버퍼
	UINT							m_VertexCount{ 0 }; // 인덱스 버퍼가 없을 경우 사용함
	XMFLOAT3						*m_pxmf3Positions{ nullptr };
	ID3D12Resource					*m_pPositionBuffer{ nullptr };
	ID3D12Resource					*m_pPositionUploadBuffer{ nullptr };

	// 정점에 대한 버퍼 서술자
	UINT							m_nVertexBufferViews{ 1 };
	D3D12_VERTEX_BUFFER_VIEW		*m_pVertexBufferViews{ nullptr };

	// 인덱스 버퍼
	UINT							m_IndexCount{ 0 }; 
	UINT							*m_pnIndices{ nullptr };
	ComPtr<ID3D12Resource>			m_pIndexBuffer{ nullptr };
	ComPtr<ID3D12Resource>			m_pIndexUploadBuffer{ nullptr };

	D3D12_INDEX_BUFFER_VIEW			m_IndexBufferView;
};

