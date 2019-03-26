#pragma once
#include "Mesh.h"

class LineShader;
class LineCube :
	public Mesh
{ 
private: 
	bool m_isMoved{ false }; // 움직이는 객체인가?

	class CubeVertex
	{
	public:
		XMFLOAT3						m_xmf3Position;
		XMFLOAT4						m_xmf4Diffuse; 

	public:
		CubeVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); }
		CubeVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse, XMFLOAT3 xmf3normal) { m_xmf3Position = XMFLOAT3(x, y, z); m_xmf4Diffuse = xmf4Diffuse; }
		CubeVertex(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3normal, XMFLOAT4 xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)) { m_xmf3Position = xmf3Position; m_xmf4Diffuse = xmf4Diffuse;}
		~CubeVertex() { }
	};

private:
	void CalculateTriangleListVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, int nVertices);
	void CalculateTriangleListVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, UINT nVertices, UINT *pnIndices, UINT nIndices);
	void CalculateTriangleStripVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, UINT nVertices, UINT *pnIndices, UINT nIndices);
	void CalculateVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, int nVertices, UINT *pnIndices, int nIndices);

public:
	LineCube(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT3 center, XMFLOAT3 extents, bool isMoved);
	LineCube(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float width, float height, float depth, bool isMoved);
	LineCube(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float width, float height, float depth);
	virtual ~LineCube();

	virtual void Update(float ElapsedTime) override {};
	void Render(ID3D12GraphicsCommandList *pd3dCommandLis, const XMFLOAT4X4&, bool);

private:
};

