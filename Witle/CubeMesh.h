#pragma once
#include "Mesh.h"


struct CDiffusedVertex { 
	XMFLOAT3 pos;
	XMFLOAT4 color;

	CDiffusedVertex() {};
	CDiffusedVertex(XMFLOAT3 p, XMFLOAT4 c) : pos(p), color(c) {};
};

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

	// ��ġ ����
	UINT							m_VertexCount{ 0 }; // �ε��� ���۰� ���� ��� �����
	XMFLOAT3						*m_pxmf3Positions{ nullptr };
	ID3D12Resource					*m_pPositionBuffer{ nullptr };
	ID3D12Resource					*m_pPositionUploadBuffer{ nullptr };

	// ������ ���� ���� ������
	UINT							m_nVertexBufferViews{ 1 };
	D3D12_VERTEX_BUFFER_VIEW		*m_pVertexBufferViews{ nullptr };

	// �ε��� ����
	UINT							m_IndexCount{ 0 }; 
	UINT							*m_pnIndices{ nullptr };
	ComPtr<ID3D12Resource>			m_pIndexBuffer{ nullptr };
	ComPtr<ID3D12Resource>			m_pIndexUploadBuffer{ nullptr };

	D3D12_INDEX_BUFFER_VIEW			m_IndexBufferView;
};

