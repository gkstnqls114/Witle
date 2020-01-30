#pragma once 
#include "MyBOBox.h"

class Mesh;
class LoadObject;
class Texture;

namespace quadtree
{ 
	struct QT_BASE_ADDER
	{
		// 상속만을 위한 클래스로 아무것도 존재하지 않는다.
	};

	struct QT_BASE_NODE
	{
		MyBOBox* BoBox{ nullptr };  // 해당 노드(맵)의 충돌체 

#if defined(DEBUG) | defined(_DEBUG)
		int reafIndex = -1; // 자식노드일때만 설정하는 디버그용 변수
#endif  
		int terrainObjCount = -1; // 만약 leafnode일 경우 해당 노드에 존재하는 지형 오브젝트의 충돌체 개수

	private:
		QT_BASE_NODE(QT_BASE_NODE const&) = delete;            // 복사 숨김
		QT_BASE_NODE& operator=(QT_BASE_NODE const&) = delete; // 할당 숨김

	public:
		QT_BASE_NODE(XMFLOAT3&& center, XMFLOAT3&& extents);
		QT_BASE_NODE(const XMFLOAT3& center, const XMFLOAT3& extents);
		virtual ~QT_BASE_NODE();
	};

	struct QT_COLLIDER_ADDER : public QT_BASE_ADDER
	{ 
	private:
		QT_COLLIDER_ADDER& operator=(QT_COLLIDER_ADDER const&) = delete; // 할당 숨김

	public: 
		MyBOBox BoBox;      // ModelStg에서 갖고올 모델의 충돌박스
		XMFLOAT4X4 World;				// 월드행렬

		QT_COLLIDER_ADDER(QT_COLLIDER_ADDER const& other);      
		QT_COLLIDER_ADDER(MyBOBox bobox, XMFLOAT4X4 world) : BoBox(bobox), World(world) {}
	};
	 
	struct QT_DRAWER_INFO
	{
	private: 
		QT_DRAWER_INFO& operator=(QT_DRAWER_INFO const&) = delete; // 할당 숨김 
		QT_DRAWER_INFO(QT_DRAWER_INFO const& other) = delete; // 복사 숨김
		 
	public:
		LoadObject* pLoadObject{ nullptr }; // 지형 오브젝트(ex: 나무) 하나를 그리기위한 모델 정보
		Texture* pTexture{ nullptr };       // 마찬가지로 그리기 위한 텍스쳐 정보

		ID3D12Resource* m_pd3dcbGameObjects         { nullptr }; // 인스턴싱을 위해 사용되는 정보
		VS_SRV_INSTANCEINFO* m_pcbMappedGameObjects { nullptr }; // 인스턴싱을 위해 사용되는 정보
		
		int         TerrainObjectCount              { 0 };		 // 해당 모델이 몇개나 배치되는 지, 개수 의미
		
		// 해당 모델이 배치되는 오브젝트의 월드 행렬들
		// m_pcbMappedGameObjects 를 통해 Map으로 올려지고 나면, clear하여 삭제됩니다.
		std::vector<XMFLOAT4X4> TransformList;                 

	public:
		QT_DRAWER_INFO() = default;
		void ReleaseObject();
	};

	struct QT_DRAWER_ADDER : public QT_BASE_ADDER
	{
	private:
		QT_DRAWER_ADDER& operator=(QT_DRAWER_ADDER const&) = delete; // 할당 숨김 
		QT_DRAWER_ADDER(QT_DRAWER_ADDER const& other) = delete; // 복사 숨김

	public:
		std::string model_name;
		XMFLOAT4X4 world;

		QT_DRAWER_ADDER(const std::string& name, const XMFLOAT4X4& w) : model_name(name), world(w) {}
	};

	struct QT_COLLISION_NODE : public QT_BASE_NODE
	{
		QT_COLLISION_NODE* children[4]{ nullptr,  nullptr , nullptr , nullptr }; // 자식 노드들

		// 만약 leafnode일 경우 해당 노드에 존재하는 지형 오브젝트의 충돌체들 (포인터)
		std::list<QT_COLLIDER_ADDER> terrainObjBoBoxs;

	private:
		QT_COLLISION_NODE(QT_COLLISION_NODE const&) = delete;            // 복사 숨김
		QT_COLLISION_NODE& operator=(QT_COLLISION_NODE const&) = delete; // 할당 숨김

	public:
		QT_COLLISION_NODE(XMFLOAT3&& center, XMFLOAT3&& extents);
		QT_COLLISION_NODE(const XMFLOAT3& center, const XMFLOAT3& extents);
		virtual ~QT_COLLISION_NODE();
	};

	struct QT_DRAWER_NODE : public QT_BASE_NODE
	{
		bool isRendering{ true }; // 렌더링 할 것인가, 말 것인가.  
		Mesh* terrainMesh{ nullptr };       // 렌더할 터레인 메쉬

		// 만약 leafnode일 경우 해당 노드에 존재하는 지형 오브젝트의 그리기 정보들
		std::map<std::string, QT_DRAWER_INFO> ModelInfoList;

		QT_DRAWER_NODE* children[4]{ nullptr,  nullptr , nullptr , nullptr }; 
		 
		QT_DRAWER_NODE(const XMFLOAT3& center, const XMFLOAT3& extents);
	};
}
