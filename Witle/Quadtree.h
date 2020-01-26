#pragma once

#ifndef QUADTREE
#define QUADTREE
 
class GameObject;

template<typename T, typename S>
class Quadtree
{ 
private:
	Quadtree(Quadtree const&) = delete;            // ���� ����
	Quadtree& operator=(Quadtree const&) = delete; // �Ҵ� ����
	  
private:
	T* m_RootNode{ nullptr }; // ��Ʈ ���

	int m_ReafNodeCount{ 0 };
	T** m_pReafNodes{ nullptr }; // ���� ��带 ������ ���� �迭�� �����̳ʿ� �Ұ���

	float m_minSize{ 0 }; // ���� ���� width or height�� ����. �� �� �ϳ��� minSize��� ���̻� ��带 ������ �ʴ´�.
	 
private: 
	void CreateRecursiveQuadTree(T* pNode, int& leafnodeIndex)
	{
		// Extents �� x, z �� *2 �� �ϳ��� minSize���� �۰ų� ������ �� �̻� ��带 ������ �ʴ´�.
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
			// y ���� ��ȭ���� �ʽ��ϴ�.
			XMFLOAT3 extents{ nodeExents.x / 2.f, nodeExents.y, nodeExents.z / 2.f };

			// �� Z
			// ������������������
			// �� 2 �� 3 ��
			// ������������������
			// �� 0 �� 1 ��
			// ������������������ �� x
			// ���� ���� ���İ� ������ �����մϴ�. y�� �״���Դϴ�.
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
		// �ڽ��� �ִ� ��� ����Լ��� �̿��Ͽ� children ���η� ����.
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

		// �ڽ��� ���ų� �� ������ ������ �ش� ��带 delete�Ѵ�.
		delete node;
	}

protected: 

	void SetminSize(float min_size)
	{ 
		// �̹� minSize�� ������ ���
		bool isAlreadySeted = m_minSize != 0;
		assert(!(isAlreadySeted) && "minSize is already seted");
		if (isAlreadySeted) return;

		// �Լ� ���ڰ� 0�� �۰ų� ���� ���
		bool isLessEqualZero = min_size <= 0;
		assert(!(isLessEqualZero) && "min_size is less than or equal 0");
		if (isLessEqualZero) return;

		m_minSize = min_size;
	}

	float GetminSize() { return m_minSize; }

	void CreateQuadTree()
	{
		// ���� ���� ����Ʈ�� ����� 0���� ���ų� �۽��ϴ�.
		bool isLessEqualZero = m_minSize <= 0;
		assert(!(isLessEqualZero) && "minSize is less than or equal 0");
		if (isLessEqualZero) return;

		// m_pReafNodes �� ������ ����Ͽ� ���� �迭 �����Ѵ�.
		int leafnodeX = ceil((m_RootNode->BoBox->GetBOBox().Extents.x * 2.f) / int(m_minSize)); // x �࿡�� �������� ��
		int leafnodeZ = ceil((m_RootNode->BoBox->GetBOBox().Extents.z * 2.f) / int(m_minSize)); // z �࿡�� �������� ��

		// ū ���� ��� �����͸� ��� ���� �迭 ����
		m_ReafNodeCount = (leafnodeX > leafnodeZ) ? leafnodeX : leafnodeZ;
		m_ReafNodeCount = m_ReafNodeCount * m_ReafNodeCount; // �簢���̹Ƿ� N^2

		m_pReafNodes = new T * [m_ReafNodeCount];

		// �� ��
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

	// node �� gameobj �ٲ� �� �����Ƿ� &�� ����
	void ProcessRecursiveDataOfNode(T& node, const MyBOBox& collider, GameObject& gameobj)
	{
		// ���� ������ ��ġ���� node�� �ε������� Ȯ��.
		bool isCollided = Collision::isCollide(*node.BoBox, collider);
		if (!isCollided) return;

		bool isHaveChildren = node.children[0] != nullptr; // �ڽ��� �ִ� ���
		if (isHaveChildren)
		{
			for (int x = 0; x < 4; ++x)
			{
				ProcessRecursiveDataOfNode(*node.children[x], collider, gameobj);
			}
		}
		else
		{
			ProcessDataOfNode(node, gameobj);
		}
	} 
	 
	void AddRecursiveDataOfNode(T& node, const MyBOBox& collider, const S& data)
	{
		// �ش� ����� �浹ü�� �ε����������� �Ѿ��.
		bool isCollided = Collision::isCollide(*(node.BoBox), collider);
		if (!isCollided) return;

		bool isHaveChildren = node.children[0] != nullptr;
		if (isHaveChildren)
		{
			// �ڽ��� �ִ� ��� ����Լ��� �̿��Ͽ� children ���η� ����.
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
		}
	}

	virtual void AddDataListOfNode(T& node, const S& world) = 0; 
	virtual void ProcessDataOfNode(T& node, GameObject& gameobj) = 0;

public: 
	Quadtree(const XMFLOAT3& center, const XMFLOAT3& extents, float min_size)
	{
		bool isNotAppropriateParent = dynamic_cast<quadtree::BASE_NODE*>(m_RootNode) == nullptr;
		assert(isNotAppropriateParent && "template T class is not approriate parent struct. you have to inherit quadtree::BASE_NODE.");
		 
		SetminSize(min_size);

		if (m_RootNode == nullptr)
		{
			// ���� Ʈ���� �θ� ��带 ����ϴ�.
			m_RootNode = new T(center, extents); 
		}

		// Quadtree�� �����Ѵ�.
		CreateQuadTree();
	}

	virtual ~Quadtree()
	{
		if (m_RootNode)
		{
			ReleaseQuadTree();
		}
	}
	  
	virtual void Init() = 0;
	virtual void PrintInfo() = 0;
	 
	// ��������� ������ ��ȯ�մϴ�.
	int GetReafNodeCount()   { return m_ReafNodeCount; } 

	// Root �����͸� ��ȯ�մϴ�.
	T* const GetpRoot() const { return m_RootNode;     }
	void SetpRoot(T* root) { m_RootNode = root; }
	const T& GetReaf(int index) const 
	{ 
		assert(!(index < 0 || index >= m_minSize) && "range over index");
		return *(m_pReafNodes[index]);
	}

}; 

#endif

