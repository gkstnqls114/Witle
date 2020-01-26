#pragma once 
#include "MyBOBox.h"

class Mesh;

namespace quadtree
{ 
	struct COLLIDER // Terrain 위에 존재하는 지형 오브젝트
	{ 
	public: 
		MyBOBox BoBox;      // ModelStg에서 갖고올 모델의 충돌박스
		XMFLOAT4X4 World;				// 월드행렬

		COLLIDER(MyBOBox bobox, XMFLOAT4X4 world) : BoBox(bobox), World(world) {}
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
		int id{ -1 }; // 터레인 아이디 넘버
		Mesh* terrainMesh{ nullptr }; // 렌더할 터레인 메쉬
		QT_DRAWER_NODE* children[4]{ nullptr,  nullptr , nullptr , nullptr };
	};
}
