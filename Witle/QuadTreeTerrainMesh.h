#pragma once
#include "Mesh.h"
 
const int MAX_TRIANGLES = 10000;

class TerrainMesh;

class QuadTreeTerrainMesh
	: public Mesh
{
private: 

	struct NODE_TYPE
	{
		float positionX, positionZ, width;
		int triangleCount;
		ID3D12Resource * vertexBuffer;
		ID3D12Resource * indexBuffer; 
		NODE_TYPE* nodes[4];
	};

private:
	void CalculateMeshDimensions(int, float&, float&, float&);
	void CreateTreeNode(NODE_TYPE*, float, float, float, ID3D12Device*);
	int CountTriangles(float, float, float);
	bool IsTriangleContained(int, float, float, float);
	void ReleaseNode(NODE_TYPE*);
	// void RenderNode(NODE_TYPE*, FrustumClass*, ID3D11DeviceContext*, TerrainShaderClass*);

public:
	QuadTreeTerrainMesh(GameObject* pOwner);
	QuadTreeTerrainMesh(const QuadTreeTerrainMesh&);
	virtual ~QuadTreeTerrainMesh();

	virtual void Create() = 0;
	virtual void Init() = 0;

	bool Initialize(TerrainMesh* pQuadTerrain, ID3D12Device* pd3dDevice);
	void Shutdown();
	void Render(ID3D12GraphicsCommandList* commandList);

	int GetDrawCount();

private: 
	UINT m_triangleCount{ 0 };
	UINT m_drawCount{ 0 };
	CDiffused2TexturedVertex* m_vertexList = nullptr;
	NODE_TYPE* m_parentNode = nullptr;

};