#pragma once 
#include "MyBOBox.h"

class Mesh;

namespace quadtree
{
	struct BASE_NODE
	{
		struct TerrainObj // Terrain ���� �����ϴ� ���� ������Ʈ
		{
			MyBOBox BoBox;      // ModelStg���� ����� ���� �浹�ڽ�
			XMFLOAT4X4 World;				// �������

			TerrainObj(MyBOBox bobox, XMFLOAT4X4 world) : BoBox(bobox), World(world) {}
		};

		MyBOBox* BoBox{ nullptr };  // �ش� ���(��)�� �浹ü 
		BASE_NODE* children[4]{ nullptr,  nullptr , nullptr , nullptr }; // �ڽ� ����

#if defined(DEBUG) | defined(_DEBUG)
		int reafIndex = -1; // �ڽĳ���϶��� �����ϴ� ����׿� ����
#endif  
		int terrainObjCount = -1; // ���� leafnode�� ��� �ش� ��忡 �����ϴ� ���� ������Ʈ�� �浹ü ����

		// ���� leafnode�� ��� �ش� ��忡 �����ϴ� ���� ������Ʈ�� �浹ü�� (������)
		std::list<TerrainObj> terrainObjBoBoxs;

	private:
		BASE_NODE(BASE_NODE const&) = delete;            // ���� ����
		BASE_NODE& operator=(BASE_NODE const&) = delete; // �Ҵ� ����

	public:
		BASE_NODE(XMFLOAT3&& center, XMFLOAT3&& extents);
		BASE_NODE(const XMFLOAT3& center, const XMFLOAT3& extents);
		~BASE_NODE();
	};

	struct QT_DRAWER_NODE
	{
		BoundingBox boundingBox; // �ش� �ͷ��ο� ���ϴ°� Ȯ���� ���ִ� �ٿ���ڽ�
		bool isRendering{ false }; // ������ �� ���ΰ�, �� ���ΰ�. 
		int id{ -1 }; // �ͷ��� ���̵� �ѹ�
		Mesh* terrainMesh{ nullptr }; // ������ �ͷ��� �޽�
		QT_DRAWER_NODE* children[4]{ nullptr,  nullptr , nullptr , nullptr };
	};
}
