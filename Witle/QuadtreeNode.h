#pragma once 
#include "MyBOBox.h"

namespace quadtree
{
	struct NODE
	{
		struct TerrainObj // Terrain ���� �����ϴ� ���� ������Ʈ
		{
			MyBOBox BoBox;      // ModelStg���� ����� ���� �浹�ڽ�
			XMFLOAT4X4 World;				// �������

			TerrainObj(MyBOBox bobox, XMFLOAT4X4 world) : BoBox(bobox), World(world) {}
		};

		MyBOBox* BoBox{ nullptr };  // �ش� ���(��)�� �浹ü 
		NODE* children[4]{ nullptr,  nullptr , nullptr , nullptr }; // �ڽ� ����

		int terrainObjCount = -1; // ���� leafnode�� ��� �ش� ��忡 �����ϴ� ���� ������Ʈ�� �浹ü ����

		// ���� leafnode�� ��� �ش� ��忡 �����ϴ� ���� ������Ʈ�� �浹ü�� (������)
		std::list<TerrainObj> terrainObjBoBoxs;

	private:
		NODE(NODE const&) = delete;            // ���� ����
		NODE& operator=(NODE const&) = delete; // �Ҵ� ����

	public:
		NODE(XMFLOAT3&& center, XMFLOAT3&& extents);
		NODE(const XMFLOAT3& center, const XMFLOAT3& extents);
		~NODE();
	};
}
