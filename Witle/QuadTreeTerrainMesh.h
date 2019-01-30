#pragma once
#include "Mesh.h"
 
const int MAX_TRIANGLES = 10000;
const int QUAD = 4;

class TerrainMesh;
class HeightMapImage;

struct INFO
{
	float centerX; // 메쉬의 중심 위치
	float centerZ; // 메쉬의 중심 위치
	float meshWidth; //매쉬의 최대 직경
};

struct QUAD_TREE_NODE
{
	UINT triangleCount{ 0 };
	TerrainMesh* terrainMesh{ nullptr };
	QUAD_TREE_NODE* nodes[QUAD]{ nullptr,  nullptr , nullptr , nullptr };
};

class QuadTreeTerrainMesh
	: public ComponentBase
{
private:
	UINT m_widthTotal{ 0 };
	UINT m_lengthTotal{ 0 };

	const UINT m_lengthMin{ 129 };
	const UINT m_widthMin{ 129 };

	XMFLOAT3 m_xmf3Scale{ 0.f, 0.f, 0.f };
	XMFLOAT4 m_xmf4Color{ 1.f, 0.f, 0.f , 1.f};

	QUAD_TREE_NODE* m_pRootNode{ nullptr };

private: 
	struct NODE_TYPE
	{
		float positionX, positionZ, width, length;
		int triangleCount;
		ID3D12Resource * vertexBuffer;
		ID3D12Resource * indexBuffer; 
		NODE_TYPE* nodes[QUAD];
	};

	// 해당 함수를 재귀적으로 호출하며 터레인을 생성하는 함수입니다.
	void RecursiveCreateTerrain(QUAD_TREE_NODE* node, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int xStart, int zStart, int nWidth, int nLength, HeightMapImage *pContext = NULL);

	UINT CalculateVertex(UINT widht, UINT length);
	UINT CalculateTriangles(UINT widthPixel, UINT lengthPixel);

	bool IsCheckTriangleCount(NODE_TYPE* node, UINT numTriangles); 
	
private:
	void CreateTreeNode(NODE_TYPE*, float, float, float, float, ID3D12Device*);
	void ReleaseNode(NODE_TYPE*);
	// void RenderNode(NODE_TYPE*, FrustumClass*, ID3D11DeviceContext*, TerrainShaderClass*);

public:
	QuadTreeTerrainMesh(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nWidth, int nLength, XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), HeightMapImage *pContext = NULL);
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