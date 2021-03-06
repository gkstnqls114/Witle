#pragma once
#include "QuadtreeNode.h"
#include "GameObject.h"
#include "Quadtree.h"
 
const int MAX_TRIANGLES = 10000;
const int QUAD = 4;

class TerrainMesh;
class HeightMapImage;
class MyFrustum;
class Mesh;
class Terrain;
class MyDescriptorHeap;
class LoadObject;
class Texture;
class GameObject;
class Shader;

/*
    쿼드트리와 인스턴싱을 이용하여 지형과 지형 오브젝트를 렌더링을 하는 클래스
*/
class QtTerrainInstancingDrawer
	: public Quadtree<quadtree::QT_DRAWER_NODE, quadtree::QT_DRAWER_ADDER>
{ 
	std::vector<XMFLOAT4X4> m_AltarTransformStorage; // Altar transform 위치 저장하는 곳
	  
public:
	  
	XMFLOAT4X4 GetAltarTransform(int index, const std::string& name);

	// StaticObjectStorage 에서 가져옴 //////////////////////

	GameObject* m_EmptyObj{ nullptr };

public:
	virtual void Init() override;
	virtual void PrintInfo() override;

private: 

	virtual void AddDataListOfNode(quadtree::QT_DRAWER_NODE& node, const quadtree::QT_DRAWER_ADDER& world) override;
	virtual void ProcessDataOfNode(quadtree::QT_DRAWER_NODE& node, GameObject& gameobj) override;

	// 컴포넌트가 아닌, 게임오브젝트 내에서 동적할당된 멤버변수를 해제한다.
	void ReleaseMembers();
	// 컴포넌트가 아닌, 게임오브젝트 내에서 동적할당된 업로드 힙을 해제한다.
	void ReleaseMemberUploadBuffers();

private: 
	const UINT m_lengthMin{ 256 + 1 }; // 나누어지는 픽셀 크기
	const UINT m_widthMin{ 256 + 1 }; // 나누어지는 픽셀 크기

	XMFLOAT3 m_xmf3Scale{ 0.f, 0.f, 0.f };
	XMFLOAT4 m_xmf4Color{ 1.f, 0.f, 0.f , 1.f};
	 
private:  
	//// 터레인 오브젝트들의 초기 위치 정보 로드 //////////////
	void LoadTerrainObjectFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const char* pstrFileName, const QtTerrainInstancingDrawer const* pTerrain);
	void LoadNameAndPositionFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile, const QtTerrainInstancingDrawer const* pTerrain);
	bool LoadTransform(char* name, const char* comp_name, const XMFLOAT4X4& tr);
	//// 터레인 오브젝트들의 초기 위치 정보 로드 //////////////
	 
	// node 에 해당하는 터레인를 렌더링합니다.
	void RenderTerrain(ID3D12GraphicsCommandList* pd3dCommandList, const quadtree::QT_DRAWER_NODE& node, const Terrain* pTerrain, bool isGBuffers);
	 
	// node 에 위치하는 터레인 오브젝트들을 렌더링합니다.
	void RenderTerrainObjs(ID3D12GraphicsCommandList* pd3dCommandList, const quadtree::QT_DRAWER_NODE& node, bool isGBuffers) const;
	
	// node 에 위치하는 터레인 오브젝트들을 렌더링합니다. 그림자용으로 텍스쳐 설정 안합니다.
	void RenderTerrainObjsForShadow(ID3D12GraphicsCommandList* pd3dCommandList, const quadtree::QT_DRAWER_NODE& node, bool isGBuffers);

    // node 에 위치하는 터레인 오브젝트들의 바운딩 박스를 렌더링합니다.
	void RenderObjBOBox(ID3D12GraphicsCommandList* pd3dCommandList, const quadtree::QT_DRAWER_NODE& node);

	// 각 node 의 인스턴싱 쉐이더 변수를 생성합니다.
	void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
 
	void RecursiveRenderTerrainObjs(const quadtree::QT_DRAWER_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void RecursiveRenderTerrainObjectsForShadow(const quadtree::QT_DRAWER_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void RecursiveRenderTerrain(const quadtree::QT_DRAWER_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList, const Terrain* pTerrain, bool isGBuffers);
	void RecursiveRenderTerrainObjects_BOBox(const quadtree::QT_DRAWER_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList);
	
	void RecursiveReleaseUploadBuffers(quadtree::QT_DRAWER_NODE* node);
	void RecursiveReleaseObjects(quadtree::QT_DRAWER_NODE* node);
	   
	void CreateReafNode(quadtree::QT_DRAWER_NODE* node, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, HeightMapImage* pContext = NULL);

public:
	QtTerrainInstancingDrawer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, 
		const XMFLOAT3& center, const XMFLOAT3& extents, float min_size,
		int nWidth, int nLength, XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), HeightMapImage *pContext = NULL);
	virtual ~QtTerrainInstancingDrawer();
	   
	void AddWorldMatrix(const MyBOBox& collider, const std::string& model_name, const XMFLOAT4X4& world);
	 
	// delete 이전에 반드시 호출
	void ReleaseObjects();
	void ReleaseUploadBuffers();

	//// Render 함수 ///////////////
	void RenderObjAll(ID3D12GraphicsCommandList* pd3dCommandList, Terrain* pTerrain, bool isGBuffers);
	void RenderInstancingObjectsForShadow(ID3D12GraphicsCommandList* pd3dCommandList);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, Terrain* pTerrain, ID3D12DescriptorHeap* pHeap, bool isGBuffers);
	//// Render 함수 ///////////////

private:

};