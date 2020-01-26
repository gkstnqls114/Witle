#pragma once 
#include "MyBOBox.h"

class Mesh;
class LoadObject;
class Texture;

namespace quadtree
{ 
	struct COLLIDER // Terrain 위에 존재하는 지형 오브젝트
	{ 
	private:
		COLLIDER& operator=(COLLIDER const&) = delete; // 할당 숨김

	public: 
		MyBOBox BoBox;      // ModelStg에서 갖고올 모델의 충돌박스
		XMFLOAT4X4 World;				// 월드행렬

		COLLIDER(COLLIDER const& other);      

		COLLIDER(MyBOBox bobox, XMFLOAT4X4 world) : BoBox(bobox), World(world) {}
	};

	struct DRAWER_INFO
	{
		LoadObject* pLoadObject{ nullptr }; // 지형 오브젝트(ex: 나무) 하나를 그리기위한 모델 정보
		Texture* pTexture{ nullptr };       // 마찬가지로 그리기 위한 텍스쳐 정보

		ID3D12Resource* m_pd3dcbGameObjects         { nullptr }; // 인스턴싱을 위해 사용되는 정보
		VS_SRV_INSTANCEINFO* m_pcbMappedGameObjects { nullptr }; // 인스턴싱을 위해 사용되는 정보
		int         TerrainObjectCount              { 0 };		 // 해당 모델이 몇개나 배치되는 지, 개수 의미
		std::vector<XMFLOAT4X4> TransformList;                   // 해당 모델이 배치되는 오브젝트의 월드 행렬들
	};

	struct BASE_NODE
	{ 
		MyBOBox* BoBox{ nullptr };  // 해당 노드(맵)의 충돌체 
		
#if defined(DEBUG) | defined(_DEBUG)
		int reafIndex = -1; // 자식노드일때만 설정하는 디버그용 변수
#endif  
		int terrainObjCount = -1; // 만약 leafnode일 경우 해당 노드에 존재하는 지형 오브젝트의 충돌체 개수

	private:
		BASE_NODE(BASE_NODE const&) = delete;            // 복사 숨김
		BASE_NODE& operator=(BASE_NODE const&) = delete; // 할당 숨김

	public:
		BASE_NODE(XMFLOAT3&& center, XMFLOAT3&& extents);
		BASE_NODE(const XMFLOAT3& center, const XMFLOAT3& extents);
		virtual ~BASE_NODE();
	};

	struct COLLISION_NODE : public BASE_NODE
	{
		COLLISION_NODE* children[4]{ nullptr,  nullptr , nullptr , nullptr }; // 자식 노드들

		// 만약 leafnode일 경우 해당 노드에 존재하는 지형 오브젝트의 충돌체들 (포인터)
		std::list<COLLIDER> terrainObjBoBoxs;

	private:
		COLLISION_NODE(COLLISION_NODE const&) = delete;            // 복사 숨김
		COLLISION_NODE& operator=(COLLISION_NODE const&) = delete; // 할당 숨김

	public:
		COLLISION_NODE(XMFLOAT3&& center, XMFLOAT3&& extents);
		COLLISION_NODE(const XMFLOAT3& center, const XMFLOAT3& extents);
		virtual ~COLLISION_NODE();
	};

	struct QT_DRAWER_NODE
	{
		BoundingBox boundingBox; // 해당 터레인에 속하는가 확인을 해주는 바운딩박스
		bool isRendering{ false }; // 렌더링 할 것인가, 말 것인가.  
		QT_DRAWER_NODE* children[4]{ nullptr,  nullptr , nullptr , nullptr };
		int id = -1;
		Mesh* terrainMesh{ nullptr };       // 렌더할 터레인 메쉬

		// 만약 leafnode일 경우 해당 노드에 존재하는 지형 오브젝트의 충돌체들 (포인터)
		std::map<std::string, DRAWER_INFO> terrainObjBoBoxs;
	};
}
