#pragma once
#include "Mesh.h"

class Texture;
class MyDescriptorHeap;

class LineSphere :
	public LineMesh
{
public:
	virtual void Render(ID3D12GraphicsCommandList * commandList) override;

private:

	class SphereVertex
	{
	public:
		XMFLOAT3						position;
		XMFLOAT4						diffuse;

	public:
		SphereVertex() { position = XMFLOAT3(0.0f, 0.0f, 0.0f); diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); }
		SphereVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse) { position = XMFLOAT3(x, y, z); diffuse = xmf4Diffuse; }
		SphereVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)) { position = xmf3Position; diffuse = xmf4Diffuse; }
		~SphereVertex() { }
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
	LineSphere(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList,
		float radius = 500, float height = 500, float topRadius = 10, float bottomRadius = 10, int sectorCount = 10, int stackCount = 10
	);
	virtual ~LineSphere();
	
	virtual void Update(float ElapsedTime) override {};

	void Render(ID3D12GraphicsCommandList *pd3dCommandLis, const XMFLOAT4X4&, bool);
	void RenderInstancing(ID3D12GraphicsCommandList *pd3dCommandList, int count);

};

