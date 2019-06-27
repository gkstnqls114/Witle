#pragma once
#include "ComponentBase.h"

// Mesh Class
// �� �ȿ� �ִϸ��̼� ����(���� �ȿ� ���� ���뿡 ���� ����)�� ���� �ʽ��ϴ�.
class Mesh :
	public ComponentBase
{
public:
	virtual void Render(ID3D12GraphicsCommandList * commandList, bool isGBuffers) = 0;

private:
	D3D12_PRIMITIVE_TOPOLOGY		m_d3dPrimitiveTopology;

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

	UINT							m_nSlot = 0;
	UINT							m_nStride = 0;
	UINT							m_nOffset = 0;

	UINT							m_nStartIndex = 0;
	int								m_nBaseVertex = 0;
	

public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;
	
public:
	Mesh(GameObject* pOwner, D3D12_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	virtual ~Mesh();
	
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

// Topology�� LINE_STRIP���� �������ֵ��� �ϴ� Ŭ����.
class LineMesh
	: public Mesh
{

public:
	LineMesh(GameObject* pOwner) : Mesh(pOwner, D3D_PRIMITIVE_TOPOLOGY_LINELIST) {};
	virtual ~LineMesh();
};