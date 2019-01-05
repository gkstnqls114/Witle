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

	

private:
	// ��ġ ����
	UINT							m_nVertices{ 0 }; // �ε��� ���۰� ���� ��� �����
	XMFLOAT3						*m_pxmf3Positions{ nullptr };
	ComPtr<ID3D12Resource>			m_pd3dPositionBuffer{ nullptr };
	ComPtr<ID3D12Resource>			m_pd3dPositionUploadBuffer{ nullptr };
};

