#pragma once
#include "Mesh.h"

class Texture;
class MyDescriptorHeap;
class CylinderMesh :
	public Mesh
{
public:
	virtual void Render(ID3D12GraphicsCommandList * commandList) override;

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

	MyDescriptorHeap * m_Heap{ nullptr };
	Texture*           m_Texture{ nullptr };
private:
	void CalculateTriangleListVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, int nVertices);
	void CalculateTriangleListVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, UINT nVertices, UINT *pnIndices, UINT nIndices);
	void CalculateTriangleStripVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, UINT nVertices, UINT *pnIndices, UINT nIndices);
	void CalculateVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, int nVertices, UINT *pnIndices, int nIndices);
	 
public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;

public:
	CylinderMesh(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float bottomRadius, float topRadius, float height, int sliceCount, int stackCount);
	virtual ~CylinderMesh();
	void CreateTexture(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandListconst, const wchar_t *pszFileName);

	virtual void Update(float ElapsedTime) override {}; 
private:
};

