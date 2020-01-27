#pragma once 
#include "MyBOBox.h"

class Mesh;
class LoadObject;
class Texture;

namespace quadtree
{ 
	struct COLLIDER // Terrain ���� �����ϴ� ���� ������Ʈ
	{ 
	private:
		COLLIDER& operator=(COLLIDER const&) = delete; // �Ҵ� ����

	public: 
		MyBOBox BoBox;      // ModelStg���� ����� ���� �浹�ڽ�
		XMFLOAT4X4 World;				// �������

		COLLIDER(COLLIDER const& other);      

		COLLIDER(MyBOBox bobox, XMFLOAT4X4 world) : BoBox(bobox), World(world) {}
	};

	struct DRAWER_INFO
	{
		LoadObject* pLoadObject{ nullptr }; // ���� ������Ʈ(ex: ����) �ϳ��� �׸������� �� ����
		Texture* pTexture{ nullptr };       // ���������� �׸��� ���� �ؽ��� ����

		ID3D12Resource* m_pd3dcbGameObjects         { nullptr }; // �ν��Ͻ��� ���� ���Ǵ� ����
		VS_SRV_INSTANCEINFO* m_pcbMappedGameObjects { nullptr }; // �ν��Ͻ��� ���� ���Ǵ� ����
		int         TerrainObjectCount              { 0 };		 // �ش� ���� ��� ��ġ�Ǵ� ��, ���� �ǹ�
		std::vector<XMFLOAT4X4> TransformList;                   // �ش� ���� ��ġ�Ǵ� ������Ʈ�� ���� ��ĵ�
	};

	struct BASE_NODE
	{ 
		MyBOBox* BoBox{ nullptr };  // �ش� ���(��)�� �浹ü 
		
#if defined(DEBUG) | defined(_DEBUG)
		int reafIndex = -1; // �ڽĳ���϶��� �����ϴ� ����׿� ����
#endif  
		int terrainObjCount = -1; // ���� leafnode�� ��� �ش� ��忡 �����ϴ� ���� ������Ʈ�� �浹ü ����

	private:
		BASE_NODE(BASE_NODE const&) = delete;            // ���� ����
		BASE_NODE& operator=(BASE_NODE const&) = delete; // �Ҵ� ����

	public:
		BASE_NODE(XMFLOAT3&& center, XMFLOAT3&& extents);
		BASE_NODE(const XMFLOAT3& center, const XMFLOAT3& extents);
		virtual ~BASE_NODE();
	};

	struct COLLISION_NODE : public BASE_NODE
	{
		COLLISION_NODE* children[4]{ nullptr,  nullptr , nullptr , nullptr }; // �ڽ� ����

		// ���� leafnode�� ��� �ش� ��忡 �����ϴ� ���� ������Ʈ�� �浹ü�� (������)
		std::list<COLLIDER> terrainObjBoBoxs;

	private:
		COLLISION_NODE(COLLISION_NODE const&) = delete;            // ���� ����
		COLLISION_NODE& operator=(COLLISION_NODE const&) = delete; // �Ҵ� ����

	public:
		COLLISION_NODE(XMFLOAT3&& center, XMFLOAT3&& extents);
		COLLISION_NODE(const XMFLOAT3& center, const XMFLOAT3& extents);
		virtual ~COLLISION_NODE();
	};

	struct QT_DRAWER_NODE
		: public BASE_NODE
	{
		BoundingBox boundingBox; // �ش� �ͷ��ο� ���ϴ°� Ȯ���� ���ִ� �ٿ���ڽ�
		bool isRendering{ false }; // ������ �� ���ΰ�, �� ���ΰ�.  
		QT_DRAWER_NODE* children[4]{ nullptr,  nullptr , nullptr , nullptr };
		int id = -1;
		Mesh* terrainMesh{ nullptr };       // ������ �ͷ��� �޽�

		// ���� leafnode�� ��� �ش� ��忡 �����ϴ� ���� ������Ʈ�� �浹ü�� (������)
		std::map<std::string, DRAWER_INFO> terrainObjBoBoxs;
		 
		QT_DRAWER_NODE(const XMFLOAT3& center, const XMFLOAT3& extents);
	};
}
