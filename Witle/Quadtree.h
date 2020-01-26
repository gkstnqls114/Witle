#pragma once

#ifndef QUADTREE
#define QUADTREE
 
class GameObject;

template<typename T, typename S>
class Quadtree
{ 
private:
	Quadtree(Quadtree const&) = delete;            // 복사 숨김
	Quadtree& operator=(Quadtree const&) = delete; // 할당 숨김
	  
private:
	T* m_RootNode{ nullptr }; // 루트 노드

	int m_ReafNodeCount{ 0 };
	T** m_pReafNodes{ nullptr }; // 리프 노드를 접근할 동적 배열로 컨테이너에 불과함

	float m_minSize{ 0 }; // 제일 작은 width or height의 조건. 둘 중 하나가 minSize라면 더이상 노드를 만들지 않는다.
	 
private: 
	void CreateRecursiveQuadTree(T* pNode, int& leafnodeIndex)
	{
		// Extents 의 x, z 의 *2 중 하나라도 minSize보다 작거나 같으면 더 이상 노드를 만들지 않는다.
		XMFLOAT3 nodeExents = pNode->BoBox->GetBOBox().Extents;
		XMFLOAT3 nodeCenter = pNode->BoBox->GetBOBox().Center;
		bool isLeafNode = ((nodeExents.x * 2) <= m_minSize) | ((nodeExents.z * 2) <= m_minSize);
		if (isLeafNode)
		{
			assert(!(m_ReafNodeCount <= leafnodeIndex) && "index is greater than leaf node count");

#if defined(DEBUG) | defined(_DEBUG)
			pNode->reafIndex = leafnodeIndex;
#endif   
			m_pReafNodes[leafnodeIndex] = pNode;
			leafnodeIndex += 1;
		}
		else
		{
			// y 값은 변화하지 않습니다.
			XMFLOAT3 extents{ nodeExents.x / 2.f, nodeExents.y, nodeExents.z / 2.f };

			// ▲ Z
			// ┌───┬───┐
			// │ 2 │ 3 │
			// ├───┼───┤
			// │ 0 │ 1 │
			// └───┴───┘ ▶ x
			// 위와 같은 형식과 순서로 생성합니다. y는 그대로입니다.
			XMFLOAT3 center[4]
			{
				XMFLOAT3(nodeCenter.x - extents.x, nodeCenter.y, nodeCenter.z - extents.z), // 0
				XMFLOAT3(nodeCenter.x + extents.x, nodeCenter.y, nodeCenter.z - extents.z), // 1
				XMFLOAT3(nodeCenter.x - extents.x, nodeCenter.y, nodeCenter.z + extents.z), // 2
				XMFLOAT3(nodeCenter.x + extents.x, nodeCenter.y, nodeCenter.z + extents.z)  // 3
			};

			for (int x = 0; x < 4; ++x)
			{
				pNode->children[x] = new T(center[x], extents);
				CreateRecursiveQuadTree(pNode->children[x], leafnodeIndex);
			}
		}
	}


	void ReleaseRecursiveQuadTree(T* node)
	{
		// 자식이 있는 경우 재귀함수를 이용하여 children 내부로 들어간다.
		bool isHaveChildren = node->children[0] != nullptr;
		if (isHaveChildren)
		{
			for (int x = 0; x < 4; ++x)
			{
				if (node->children[x])
				{
					ReleaseRecursiveQuadTree(node->children[x]);
					node->children[x] = nullptr;
				}
			}
		}

		// 자식이 없거나 위 과정이 끝나면 해당 노드를 delete한다.
		delete node;
	}

protected: 

	void SetminSize(float min_size)
	{ 
		// 이미 minSize가 설정된 경우
		bool isAlreadySeted = m_minSize != 0;
		assert(!(isAlreadySeted) && "minSize is already seted");
		if (isAlreadySeted) return;

		// 함수 인자가 0과 작거나 같은 경우
		bool isLessEqualZero = min_size <= 0;
		assert(!(isLessEqualZero) && "min_size is less than or equal 0");
		if (isLessEqualZero) return;

		m_minSize = min_size;
	}

	float GetminSize() { return m_minSize; }

	void CreateQuadTree()
	{
		// 제일 작은 쿼드트리 사이즈가 0보다 같거나 작습니다.
		bool isLessEqualZero = m_minSize <= 0;
		assert(!(isLessEqualZero) && "minSize is less than or equal 0");
		if (isLessEqualZero) return;

		// m_pReafNodes 의 개수를 계산하여 동적 배열 생성한다.
		int leafnodeX = ceil((m_RootNode->BoBox->GetBOBox().Extents.x * 2.f) / int(m_minSize)); // x 축에서 나눠지는 수
		int leafnodeZ = ceil((m_RootNode->BoBox->GetBOBox().Extents.z * 2.f) / int(m_minSize)); // z 축에서 나눠지는 수

		// 큰 값을 골라 포인터를 담는 동적 배열 생성
		m_ReafNodeCount = (leafnodeX > leafnodeZ) ? leafnodeX : leafnodeZ;
		m_ReafNodeCount = m_ReafNodeCount * m_ReafNodeCount; // 사각형이므로 N^2

		m_pReafNodes = new T * [m_ReafNodeCount];

		// 그 후
		int leafnodeIndex = 0;
		CreateRecursiveQuadTree(m_RootNode, leafnodeIndex);
	}

	void ReleaseQuadTree()
	{
		if (m_RootNode != nullptr)
		{
			ReleaseRecursiveQuadTree(m_RootNode);
			m_RootNode = nullptr;
		}

		if (m_pReafNodes != nullptr)
		{
			delete[] m_pReafNodes;
			m_pReafNodes = nullptr;
		}
	}

	//   
	//void ProcessRecursiveCollide(const T& node, Movement& movement, const BoundingOrientedBox& nextFrameBoBox, const MyBOBox& collider)
	//{
	//	// 현재 프레임 위치에서 node와 부딪히는지 확인.
	//	bool isCollided = Collision::isCollide(*node.BoBox, collider);
	//	if (!isCollided) return;

	//	bool isHaveChildren = node.children[0] != nullptr; // 자식이 있는 경우
	//	if (isHaveChildren)
	//	{
	//		for (int x = 0; x < 4; ++x)
	//		{
	//			ProcessRecursiveCollide(*node.children[x], movement, nextFrameBoBox, collider);
	//		}
	//	}
	//	else
	//	{
	//		float fElapsedTime = CGameTimer::GetInstance()->GetTimeElapsed();
	//		for (const auto& tobj : node.terrainObjBoBoxs)
	//		{
	//			XMFLOAT3 slideVector{ 0.f, 0.f, 0.f };

	//			// 다음 프레임에서 터레인 오브젝트와 부딪히는 지 확인한다.
	//			bool isSlide = Collision::ProcessCollision(
	//				nextFrameBoBox,
	//				tobj.BoBox,
	//				movement.GetpOwner()->GetTransform().GetPosition(),
	//				movement.GetVelocity(),
	//				fElapsedTime,
	//				true,
	//				slideVector);

	//			if (isSlide)
	//			{
	//				movement.SetVelocity(slideVector);
	//			}
	//		}
	//	}
	//} 
	 
	void AddRecursiveDataOfNode(T& node, const MyBOBox& collider, const S& data)
	{
		// 해당 노드의 충돌체와 부딪히지않으면 넘어간다.
		bool isCollided = Collision::isCollide(*(node.BoBox), collider);
		if (!isCollided) return;

		bool isHaveChildren = node.children[0] != nullptr;
		if (isHaveChildren)
		{
			// 자식이 있는 경우 재귀함수를 이용하여 children 내부로 들어간다.
			for (int x = 0; x < 4; ++x)
			{
				if (node.children[x])
				{
					AddRecursiveDataOfNode(*(node.children[x]), collider, data);
				}
			}
		}
		else
		{
			AddDataListOfNode(node, data);
			return;
		}
	}

	virtual void AddDataListOfNode(T& node, const S& world) = 0; 

public: 
	Quadtree(const XMFLOAT3& center, const XMFLOAT3& extents, float min_size)
	{
		bool isNotAppropriateParent = dynamic_cast<quadtree::BASE_NODE*>(m_RootNode) == nullptr;
		assert(isNotAppropriateParent && "template T class is not approriate parent struct. you have to inherit quadtree::BASE_NODE.");
		 
		SetminSize(min_size);

		if (m_RootNode == nullptr)
		{
			// 쿼드 트리의 부모 노드를 만듭니다.
			m_RootNode = new T(center, extents); 
		}

		// Quadtree를 구성한다.
		CreateQuadTree();
	}

	virtual ~Quadtree()
	{
		if (m_RootNode)
		{
			ReleaseQuadTree();
		}
	}

	void Release()
	{

	}


	virtual void Init() = 0;
	virtual void PrintInfo() = 0;


public: 
	// 리프노드의 개수를 반환합니다.
	int GetReafNodeCount()   { return m_ReafNodeCount; } 

	// Root 포인터를 반환합니다.
	T* const GetpRoot() const { return m_RootNode;     }
	void SetpRoot(T* root) { m_RootNode = root; }
	const T& GetReaf(int index) const 
	{ 
		assert(!(index < 0 || index >= m_minSize) && "range over index");
		return *(m_pReafNodes[index]);
	}

}; 

#endif

