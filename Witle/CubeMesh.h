#pragma once
#include "Mesh.h"

class CubeMesh :
	public Mesh
{
	class CubeVertex : public CVertex
	{
	public:
		XMFLOAT4						m_xmf4Diffuse;
		XMFLOAT3						m_xmf4Normal;

	public:
		CubeVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); }
		CubeVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse, XMFLOAT3 xmf3normal) { m_xmf3Position = XMFLOAT3(x, y, z); m_xmf4Diffuse = xmf4Diffuse; m_xmf4Normal = xmf3normal; }
		CubeVertex(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3normal, XMFLOAT4 xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)) { m_xmf3Position = xmf3Position; m_xmf4Diffuse = xmf4Diffuse;  m_xmf4Normal = xmf3normal; }
		~CubeVertex() { }
	};

public:
	CubeMesh(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float width, float height, float depth);
	virtual ~CubeMesh();
	
	virtual void Update(float ElapsedTime) override {};
	
private:
};

