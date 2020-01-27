#pragma once 
#include "MyBOBox.h"

class Mesh;
class LoadObject;
class Texture;

namespace quadtree
{ 
	struct QT_BASE_ADDER
	{
		// ��Ӹ��� ���� Ŭ������ �ƹ��͵� �������� �ʴ´�.
	};

	struct QT_BASE_NODE
	{
		MyBOBox* BoBox{ nullptr };  // �ش� ���(��)�� �浹ü 

#if defined(DEBUG) | defined(_DEBUG)
		int reafIndex = -1; // �ڽĳ���϶��� �����ϴ� ����׿� ����
#endif  
		int terrainObjCount = -1; // ���� leafnode�� ��� �ش� ��忡 �����ϴ� ���� ������Ʈ�� �浹ü ����

	private:
		QT_BASE_NODE(QT_BASE_NODE const&) = delete;            // ���� ����
		QT_BASE_NODE& operator=(QT_BASE_NODE const&) = delete; // �Ҵ� ����

	public:
		QT_BASE_NODE(XMFLOAT3&& center, XMFLOAT3&& extents);
		QT_BASE_NODE(const XMFLOAT3& center, const XMFLOAT3& extents);
		virtual ~QT_BASE_NODE();
	};

	struct QT_COLLIDER_ADDER : public QT_BASE_ADDER
	{ 
	private:
		QT_COLLIDER_ADDER& operator=(QT_COLLIDER_ADDER const&) = delete; // �Ҵ� ����

	public: 
		MyBOBox BoBox;      // ModelStg���� ����� ���� �浹�ڽ�
		XMFLOAT4X4 World;				// �������

		QT_COLLIDER_ADDER(QT_COLLIDER_ADDER const& other);      
		QT_COLLIDER_ADDER(MyBOBox bobox, XMFLOAT4X4 world) : BoBox(bobox), World(world) {}
	};
	 
	struct QT_DRAWER_INFO
	{
	private: 
		QT_DRAWER_INFO& operator=(QT_DRAWER_INFO const&) = delete; // �Ҵ� ���� 
		QT_DRAWER_INFO(QT_DRAWER_INFO const& other) = delete; // ���� ����
		 
	public:
		LoadObject* pLoadObject{ nullptr }; // ���� ������Ʈ(ex: ����) �ϳ��� �׸������� �� ����
		Texture* pTexture{ nullptr };       // ���������� �׸��� ���� �ؽ��� ����

		ID3D12Resource* m_pd3dcbGameObjects         { nullptr }; // �ν��Ͻ��� ���� ���Ǵ� ����
		VS_SRV_INSTANCEINFO* m_pcbMappedGameObjects { nullptr }; // �ν��Ͻ��� ���� ���Ǵ� ����
		
		int         TerrainObjectCount              { 0 };		 // �ش� ���� ��� ��ġ�Ǵ� ��, ���� �ǹ�
		std::vector<XMFLOAT4X4> TransformList;                   // �ش� ���� ��ġ�Ǵ� ������Ʈ�� ���� ��ĵ�
	};

	struct QT_DRAWER_ADDER : public QT_BASE_ADDER
	{
	private:
		QT_DRAWER_ADDER& operator=(QT_DRAWER_ADDER const&) = delete; // �Ҵ� ���� 
		QT_DRAWER_ADDER(QT_DRAWER_ADDER const& other) = delete; // ���� ����

	public:
		std::string model_name;
		XMFLOAT4X4 world;

		QT_DRAWER_ADDER(const std::string& name, const XMFLOAT4X4& w) : model_name(name), world(w) {}
	};

	struct QT_COLLISION_NODE : public QT_BASE_NODE
	{
		QT_COLLISION_NODE* children[4]{ nullptr,  nullptr , nullptr , nullptr }; // �ڽ� ����

		// ���� leafnode�� ��� �ش� ��忡 �����ϴ� ���� ������Ʈ�� �浹ü�� (������)
		std::list<QT_COLLIDER_ADDER> terrainObjBoBoxs;

	private:
		QT_COLLISION_NODE(QT_COLLISION_NODE const&) = delete;            // ���� ����
		QT_COLLISION_NODE& operator=(QT_COLLISION_NODE const&) = delete; // �Ҵ� ����

	public:
		QT_COLLISION_NODE(XMFLOAT3&& center, XMFLOAT3&& extents);
		QT_COLLISION_NODE(const XMFLOAT3& center, const XMFLOAT3& extents);
		virtual ~QT_COLLISION_NODE();
	};

	struct QT_DRAWER_NODE : public QT_BASE_NODE
	{
		BoundingBox boundingBox; // �ش� �ͷ��ο� ���ϴ°� Ȯ���� ���ִ� �ٿ���ڽ�
		bool isRendering{ false }; // ������ �� ���ΰ�, �� ���ΰ�.  
		QT_DRAWER_NODE* children[4]{ nullptr,  nullptr , nullptr , nullptr };
		int id = -1;
		Mesh* terrainMesh{ nullptr };       // ������ �ͷ��� �޽�

		// ���� leafnode�� ��� �ش� ��忡 �����ϴ� ���� ������Ʈ�� �׸��� ������
		std::map<std::string, QT_DRAWER_INFO> ModelInfoList;
		 
		QT_DRAWER_NODE(const XMFLOAT3& center, const XMFLOAT3& extents);
	};
}
