#pragma once
#include "Mesh.h"
 
const int MAX_TRIANGLES = 10000;
const int QUAD = 4;

class TerrainMesh;
class HeightMapImage;

struct INFO
{
	float centerX; // �޽��� �߽� ��ġ
	float centerZ; // �޽��� �߽� ��ġ
	float meshWidth; //�Ž��� �ִ� ����
};

struct QUAD_TREE_NODE
{
	UINT numCreate{ 0 };
	UINT triangleCount{ 0 };
	Mesh* terrainMesh{ nullptr };
	QUAD_TREE_NODE* nodes[QUAD]{ nullptr,  nullptr , nullptr , nullptr };
};

class QuadTreeTerrainMesh
	: public ComponentBase
{
private:
	UINT m_widthTotal{ 0 };
	UINT m_lengthTotal{ 0 };

	const UINT m_lengthMin{ 33 };
	const UINT m_widthMin{ 33 };

	XMFLOAT3 m_xmf3Scale{ 0.f, 0.f, 0.f };
	XMFLOAT4 m_xmf4Color{ 1.f, 0.f, 0.f , 1.f};

	QUAD_TREE_NODE* m_pRootNode{ nullptr };

private:  

	// �ش� �Լ��� ��������� ȣ���ϸ� �ͷ����� �����ϴ� �Լ��Դϴ�.
	void RecursiveCreateTerrain(QUAD_TREE_NODE* node, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, 
		int xStart, int zStart, int nWidth, int nLength, int nBlockWidth, int nBlockLength,
		HeightMapImage *pContext = NULL);

	UINT CalculateVertex(UINT widht, UINT length);
	UINT CalculateTriangles(UINT widthPixel, UINT lengthPixel);

private:

public:
	QuadTreeTerrainMesh(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nWidth, int nLength, XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), HeightMapImage *pContext = NULL);
	virtual ~QuadTreeTerrainMesh();

	void ReleaseUploadBuffers(); 

	virtual void Update(float fTimeElapsed) override {};

	const QUAD_TREE_NODE* GetRootNode() const { return m_pRootNode; }

	void TESTRender(const QUAD_TREE_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList);
private:

};