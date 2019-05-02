#pragma once
#pragma once
#include "ComponentBase.h"

class MyDescriptorHeap;

// Shape Class
// �� �ȿ� �ִϸ��̼� ����(���� �ȿ� ���� ���뿡 ���� ����)�� ���� �ʽ��ϴ�.
class Shape :
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
	Shape() {};

public:
	Shape(GameObject* pOwner);
	virtual ~Shape();

	virtual void Update(float fTimeElapsed) override {};
	void ReleaseObjects();
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

	virtual void Render(ID3D12GraphicsCommandList * pd3dCommandList) = 0;
	// pos�� ��ũ�� ��ǥ��(�������� 0,0)�� �������� �� ��ġ
	virtual void Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT2& pos) = 0;
};

