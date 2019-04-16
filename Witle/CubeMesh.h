#pragma once
#include "Mesh.h"

class CubeMesh :
	public Mesh
{
private: 
	class CubeVertex
	{
	public:
		XMFLOAT3						m_xmf3Position;
		XMFLOAT4						m_xmf4Diffuse;
		XMFLOAT3						m_xmf4Normal;
		XMFLOAT2						uv;

	public:
		CubeVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); }
		CubeVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse, XMFLOAT3 xmf3normal) { m_xmf3Position = XMFLOAT3(x, y, z); m_xmf4Diffuse = xmf4Diffuse; m_xmf4Normal = xmf3normal; }
		CubeVertex(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3normal, XMFLOAT4 xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)) { m_xmf3Position = xmf3Position; m_xmf4Diffuse = xmf4Diffuse;  m_xmf4Normal = xmf3normal; }
		~CubeVertex() { }
	};

private:
	void CalculateTriangleListVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, int nVertices);
	void CalculateTriangleListVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, UINT nVertices, UINT *pnIndices, UINT nIndices);
	void CalculateTriangleStripVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, UINT nVertices, UINT *pnIndices, UINT nIndices);
	void CalculateVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, int nVertices, UINT *pnIndices, int nIndices);

public:
	CubeMesh(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float width, float height, float depth);
	virtual ~CubeMesh();
	
	virtual void Update(float ElapsedTime) override {};
	
private:
};

