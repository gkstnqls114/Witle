#pragma once
#include "ComponentBase.h"

// Mesh Class
// �� �ȿ� �ִϸ��̼� ����(���� �ȿ� ���� ���뿡 ���� ����)�� ���� �ʽ��ϴ�.
class Mesh :
	public ComponentBase
{
protected:
	// ��ġ ����
	UINT							m_vertexCount{ 0 }; // �ε��� ���۰� ���� ��� �����
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


	D3D12_PRIMITIVE_TOPOLOGY		m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT							m_nSlot = 0;
	UINT							m_nStride = 0;
	UINT							m_nOffset = 0;

	UINT							m_nStartIndex = 0;
	int								m_nBaseVertex = 0;



public:
	Mesh(GameObject* pOwner);
	virtual ~Mesh();

	virtual void Create() = 0;
	virtual void Init() = 0;
	

	/////////////////////////////////////////////////////////////////////////// Get
	UINT GetVertexCount() const { return m_vertexCount; }
	ID3D12Resource* GetPositonBuffer() const { m_pPositionBuffer; }
	ID3D12Resource* GetPositionUploadBuffer() const { m_pPositionUploadBuffer; }


	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(int index) const { return m_pVertexBufferViews[index]; }
	UINT GetVertexBufferViewCount() const { return m_nVertexBufferViews; }

	UINT GetIndexCount() const { return m_IndexCount; }
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const { return m_IndexBufferView; }
	/////////////////////////////////////////////////////////////////////////// Get

	
};

