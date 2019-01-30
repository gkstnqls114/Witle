#pragma once
#include "Mesh.h"
 
const int MAX_TRIANGLES = 10000;
const int QUAD = 4;

class TerrainMesh;

struct INFO
{
	float centerX; // 메쉬의 중심 위치
	float centerZ; // 메쉬의 중심 위치
	float meshWidth; //매쉬의 최대 직경
};

class QuadTreeTerrainMesh
	: public Mesh
{
private: 

	struct NODE_TYPE
	{
		float positionX, positionZ, width, length;
		int triangleCount;
		ID3D12Resource * vertexBuffer;
		ID3D12Resource * indexBuffer; 
		NODE_TYPE* nodes[QUAD];
	};

	UINT CalculateTriangles(UINT widthPixel, UINT lengthPixel);
	
	bool IsCheckTriangleCount(NODE_TYPE* node, UINT numTriangles); 
	INFO CalculateMeshDimensions(int vertexCount);
private:
	
	void CreateTreeNode(NODE_TYPE*, float, float, float, float, ID3D12Device*);
	void ReleaseNode(NODE_TYPE*);
	// void RenderNode(NODE_TYPE*, FrustumClass*, ID3D11DeviceContext*, TerrainShaderClass*);

public:
	QuadTreeTerrainMesh(GameObject* pOwner);
	QuadTreeTerrainMesh(const QuadTreeTerrainMesh&);
	virtual ~QuadTreeTerrainMesh();

	virtual void Create() = 0;
	virtual void Init() = 0;

	bool Initialize(TerrainMesh* pQuadTerrain, ID3D12Device* pd3dDevice);
	void ReleaseUploadBuffers(); 

	UINT GetDrawCount() const { return m_drawCount; };

private: 
	UINT m_triangleCount{ 0 };
	UINT m_drawCount{ 0 };
	CDiffused2TexturedVertex* m_vertexList = nullptr;
	NODE_TYPE* m_parentNode = nullptr;

};