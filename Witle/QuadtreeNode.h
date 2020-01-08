#pragma once 
#include "MyBOBox.h"

namespace quadtree
{
	struct NODE
	{
		struct TerrainObj // Terrain 위에 존재하는 지형 오브젝트
		{
			MyBOBox BoBox;      // ModelStg에서 갖고올 모델의 충돌박스
			XMFLOAT4X4 World;				// 월드행렬

			TerrainObj(MyBOBox bobox, XMFLOAT4X4 world) : BoBox(bobox), World(world) {}
		};

		MyBOBox* BoBox{ nullptr };  // 해당 노드(맵)의 충돌체 
		NODE* children[4]{ nullptr,  nullptr , nullptr , nullptr }; // 자식 노드들

		int terrainObjCount = -1; // 만약 leafnode일 경우 해당 노드에 존재하는 지형 오브젝트의 충돌체 개수

		// 만약 leafnode일 경우 해당 노드에 존재하는 지형 오브젝트의 충돌체들 (포인터)
		std::list<TerrainObj> terrainObjBoBoxs;

	private:
		NODE(NODE const&) = delete;            // 복사 숨김
		NODE& operator=(NODE const&) = delete; // 할당 숨김

	public:
		NODE(XMFLOAT3&& center, XMFLOAT3&& extents);
		NODE(const XMFLOAT3& center, const XMFLOAT3& extents);
		~NODE();
	};
}
