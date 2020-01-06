#include "stdafx.h"
#include "d3dUtil.h" 
#include "MyBOBox.h" 
#include "Collision.h"
#include "QuadtreeMgr.h"
 
 
void QuadtreeMgr::SetminSize(float min_size)
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

void QuadtreeMgr::CreateQuadTree()
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

	m_pReafNodes = new NODE*[m_ReafNodeCount];

	// �� ��
	int leafnodeIndex = 0;
	CreateRecursiveQuadTree(m_RootNode, leafnodeIndex);
}

void QuadtreeMgr::CreateRecursiveQuadTree(NODE* pNode, int& leafnodeIndex)
{
	// Extents �� x, z �� *2 �� �ϳ��� minSize���� �۰ų� ������ �� �̻� ��带 ������ �ʴ´�.
	XMFLOAT3 nodeExents = pNode->BoBox->GetBOBox().Extents;
	XMFLOAT3 nodeCenter = pNode->BoBox->GetBOBox().Center;
	bool isLeafNode = ((nodeExents.x * 2) <= m_minSize) | ((nodeExents.z * 2) <= m_minSize);
	if (isLeafNode)
	{
		assert(!(m_ReafNodeCount <= leafnodeIndex) && "index is greater than leaf node count");
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
		XMFLOAT3 center[QUAD]
		{
			XMFLOAT3(nodeCenter.x - extents.x, nodeCenter.y, nodeCenter.z - extents.z), // 0
			XMFLOAT3(nodeCenter.x + extents.x, nodeCenter.y, nodeCenter.z - extents.z), // 1
			XMFLOAT3(nodeCenter.x - extents.x, nodeCenter.y, nodeCenter.z + extents.z), // 2
			XMFLOAT3(nodeCenter.x + extents.x, nodeCenter.y, nodeCenter.z + extents.z)  // 3
		};
		 
		for (int x = 0; x < QUAD; ++x)
		{
			pNode->children[x] = new NODE(center[x], extents);
			CreateRecursiveQuadTree(pNode->children[x], leafnodeIndex);
		} 
	}
}

void QuadtreeMgr::ReleaseQuadTree()
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

void QuadtreeMgr::ReleaseRecursiveQuadTree(NODE* pNode)
{
	// �ڽ��� �ִ� ��� ����Լ��� �̿��Ͽ� children ���η� ����.
	bool isHaveChildren = pNode->children[0] != nullptr;
	if (isHaveChildren)
	{
		for (int x = 0; x < QUAD; ++x)
		{
			if (pNode->children[x])
			{
				ReleaseRecursiveQuadTree(pNode->children[x]); 
				pNode->children[x] = nullptr;
			}
		}
	}

	// �ڽ��� ���ų� �� ������ ������ �ش� ��带 delete�Ѵ�.
	delete pNode;   
}

void QuadtreeMgr::AddRecursiveCollider(NODE* pNode, const MyBOBox& collider)
{
	// �ش� ����� �浹ü�� �ε����������� �Ѿ��.
	bool isCollided = Collision::isCollide(*(pNode->BoBox), collider);
	if (!isCollided) return;

	bool isHaveChildren = pNode->children[0] != nullptr;
	if (isHaveChildren)
	{
		// �ڽ��� �ִ� ��� ����Լ��� �̿��Ͽ� children ���η� ����.
		for (int x = 0; x < QUAD; ++x)
		{
			if (pNode->children[x])
			{
				AddRecursiveCollider(pNode->children[x], collider); 
			}
		}
	}
	else
	{
		// �ڽ��� �����Ƿ� ��������� ���̴�.
		// �浹�����Ƿ� ��� ����Ʈ�� �߰��Ѵ�.
		pNode->terrainObjCount += 1;
		pNode->terrainObjBoBoxs.push_back(&collider);
	} 
}
    
  
QuadtreeMgr::QuadtreeMgr()  
{
}

QuadtreeMgr::~QuadtreeMgr()
{
	if (m_RootNode)
	{
		ReleaseQuadTree();
	}
}

void QuadtreeMgr::Init(const XMFLOAT3& center, const XMFLOAT3& extents, float min_size)
{  
	SetminSize(min_size);

	if (m_RootNode == nullptr)
	{
		// ���� Ʈ���� �θ� ��带 ����ϴ�.
		m_RootNode = new NODE(center, extents);
	}

	CreateQuadTree();
}

void QuadtreeMgr::Init(XMFLOAT3&& center, XMFLOAT3&& extents, float min_size)
{
	Init(center, extents, min_size);
}

void QuadtreeMgr::Init(const SingletonInitializer* singletonMgr)
{
}
 
void QuadtreeMgr::Update(float fElapsedTime)
{
}

void QuadtreeMgr::LastUpdate(float fElapsedTime)
{ 
}

void QuadtreeMgr::AddCollider(const MyBOBox& collider)
{
	AddRecursiveCollider(m_RootNode, collider);
}

void QuadtreeMgr::PrintInfo()
{
#ifdef _DEBUG
	std::cout << "Quadtree Leaf Node Info ... " << std::endl;
	int n = 0;
	for (int x = 0; x < m_ReafNodeCount; ++x)
	{
		n += 1;

		auto bobox = m_pReafNodes[x]->BoBox->GetBOBox();
		std::cout << "Index ... " << x <<std::endl;
		std::cout << "Center ... (" << bobox.Center.x << " , "<< bobox.Center.y << " , "<< bobox.Center.z << ")" << std::endl;
		std::cout << "Size ... (" << bobox.Extents.x * 2 << " , " << bobox.Extents.y * 2 << " , " << bobox.Extents.z * 2<< ")" << std::endl;

		if (n % 4 == 0) std::cout << std::endl;
	}
#endif // _DEBUG

}
 
 
QuadtreeMgr::NODE::NODE(XMFLOAT3&& center, XMFLOAT3&& extents)
{
	NODE(center, extents);
}
 

QuadtreeMgr::NODE::NODE(const XMFLOAT3& center, const XMFLOAT3& extents)
{
	BoBox = new MyBOBox(center, extents); 
}

QuadtreeMgr::NODE::~NODE()
{
	if (BoBox)
	{
		delete BoBox;
		BoBox = nullptr;
	}

	for (int x = 0; x < QUAD; ++x)
	{
		if (children[x])
		{
			delete children[x];
			children[x] = nullptr;
		}
	}
}
