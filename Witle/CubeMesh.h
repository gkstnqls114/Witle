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

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(int index) const { return m_pd3dVertexBufferViews[index]; }
	UINT GetVertexBufferViewCount() const { return m_nVertexBufferViews; }
private:
	// ��ġ ����
	UINT							m_VertexCount{ 0 }; // �ε��� ���۰� ���� ��� �����
	XMFLOAT3						*m_pxmf3Positions{ nullptr };
	ID3D12Resource					*m_pPositionBuffer{ nullptr };
	ID3D12Resource					*m_pPositionUploadBuffer{ nullptr };

	// ������ ���� ���� ������
	UINT							m_nVertexBufferViews{ 0 };
	D3D12_VERTEX_BUFFER_VIEW		*m_pd3dVertexBufferViews{ nullptr };

};

