#pragma once
#include "Mesh.h"

class CylinderMesh :
	public Mesh
{
private:
	class CylinderVertex
	{
	public:
		XMFLOAT3						position;
		XMFLOAT4						diffuse;
		XMFLOAT3						normal;
		XMFLOAT2						uv;

	public:
		CylinderVertex() { position = XMFLOAT3(0.0f, 0.0f, 0.0f); diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); }
		CylinderVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse, XMFLOAT3 xmf3normal) { position = XMFLOAT3(x, y, z); diffuse = xmf4Diffuse; normal = xmf3normal; }
		CylinderVertex(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3normal, XMFLOAT4 xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)) { position = xmf3Position; diffuse = xmf4Diffuse;  normal = xmf3normal; }
		~CylinderVertex() { }
	};

private:
	void CalculateTriangleListVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, int nVertices);
	void CalculateTriangleListVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, UINT nVertices, UINT *pnIndices, UINT nIndices);
	void CalculateTriangleStripVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, UINT nVertices, UINT *pnIndices, UINT nIndices);
	void CalculateVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, int nVertices, UINT *pnIndices, int nIndices);

public:
	CylinderMesh(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float bottomRadius, float topRadius, float height, int sliceCount, int stackCount);
	virtual ~CylinderMesh();

	virtual void Update(float ElapsedTime) override {};
	void Render(ID3D12GraphicsCommandList *commandList);
private:
};

