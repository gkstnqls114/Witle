#include "stdafx.h"

#include "d3dUtil.h" 
#include "Object.h"
#include "MyBOBox.h"
#include "ModelStorage.h"
#include "Collision.h" 
#include "Player.h"

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

	m_pReafNodes = new quadtree::NODE*[m_ReafNodeCount];

	// �� ��
	int leafnodeIndex = 0;
	CreateRecursiveQuadTree(m_RootNode, leafnodeIndex);
}

void QuadtreeMgr::CreateRecursiveQuadTree(quadtree::NODE* pNode, int& leafnodeIndex)
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
			pNode->children[x] = new quadtree::NODE(center[x], extents);
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

void QuadtreeMgr::ReleaseRecursiveQuadTree(quadtree::NODE* pNode)
{
	// �ڽ��� �ִ� ��� ����Լ��� �̿��Ͽ� children ���η� ����.
	bool isHaveChildren = pNode->children[0] != nullptr;
	if (isHaveChildren)
	{
		for (int x = 0; x < 4; ++x)
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

void QuadtreeMgr::CreateTerrainObj(const char* terrain_info_path)
{
	FILE* pInFile = NULL;
	::fopen_s(&pInFile, terrain_info_path, "rb");
	::rewind(pInFile);

	char pstrToken[64] = { '\0' };
	 
	for (; ; )
	{
		if (FileRead::ReadStringFromFile(pInFile, pstrToken))
		{
			if (!strcmp(pstrToken, "<Hierarchy>:"))
			{
				int nRootChild = FileRead::ReadIntegerFromFile(pInFile); // ��� ������Ʈ ����

				FileRead::ReadStringFromFile(pInFile, pstrToken); //<ObjectCount>: 
				int nObjects = FileRead::ReadIntegerFromFile(pInFile);
				for (int i = 0; i < nObjects; ++i)
				{
					FileRead::ReadStringFromFile(pInFile, pstrToken); // Object Name
					FileRead::ReadIntegerFromFile(pInFile); // Object Count
				}

				for (int i = 0; i < nRootChild; ++i)
				{
					FileRead::ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<Frame>:"))
					{
						// AddCollider �Լ��� ���� ���� ��忡 �浹ü�� �߰��Ѵ�.
						CreateTerrainObj(pInFile);
					}

				}
			}
			else if (!strcmp(pstrToken, "</Hierarchy>"))
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

#ifdef _WITH_DEBUG_FRAME_HIERARCHY
	TCHAR pstrDebug[256] = { 0 };
	_stprintf_s(pstrDebug, 256, _T("Frame Hierarchy\n"));
	OutputDebugString(pstrDebug);

	LoadObject::PrintFrameInfo(pLoadedModel->m_pModelRootObject, NULL);
#endif

	::fclose(pInFile);
}

void QuadtreeMgr::CreateTerrainObj(FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	char name[64];

	int nFrame = FileRead::ReadIntegerFromFile(pInFile);
	FileRead::ReadStringFromFile(pInFile, name);

	for (; ; )
	{
		FileRead::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<GlobalTransform>:"))
		{ 
			XMFLOAT4X4 temp;
			nReads = (UINT)::fread(&temp, sizeof(XMFLOAT4X4), 1, pInFile);

			FileRead::ReadStringFromFile(pInFile, pstrToken); // <GlobalRotation>:
			XMFLOAT4 rotationXYZ;
			nReads = (UINT)::fread(&rotationXYZ, sizeof(XMFLOAT4), 1, pInFile);
			XMFLOAT4X4 rotate180Y = Matrix4x4::RotateMatrix(0.f, 0.f, 0.f);
			XMFLOAT4X4 rotateInfo = Matrix4x4::RotateMatrix(0.f, rotationXYZ.z, 0.f);

			// XMFLOAT4X4 transform = Matrix4x4::Multiply(rotate180Y, rotateInfo);
			XMFLOAT4X4 transform = Matrix4x4::Identity();

			// fbx sdk ���� �����ö� x, z��ǥ�� -��ȣ �ٿ����� ...
			transform._41 = -(temp._41);
			transform._42 = 0; // y��ǥ�� 0���� ����
			if (!strcmp(name, Flower)) transform._42 = temp._42;
			transform._43 = -(temp._43);

			assert(!(temp._41 >= 40000));
			assert(!(temp._43 >= 40000));


			// ��Stg�� �����ϴ� �̸��� ���� �̸��� ��ġ�ϴ��� Ȯ���Ѵ�.
			for (const auto& modelname : ModelStorage::GetInstance()->m_NameList)
			{
				// Object001�� ���ܷ� Cliff , ������ �ǹ��Ѵ�.
				if (!(!strcmp(name, modelname.c_str()) || !strcmp(name, "Object001")))
				{
					continue;
				}
				
				if (!strcmp(name, "Object001"))
				{
					strncpy(name, "Cliff", 64); // Object001�� Cliff�� ����ϵ��� �̸��� �ٲ۴�.
				}
				
 
				// ��� ���� �浹ü�� ���ϰ����� �ʴ�. �ѹ� Ȯ�����ش�.
				MyBOBox* pBoBox = ModelStorage::GetInstance()->GetBOBox(modelname);
				if (pBoBox == nullptr) continue;

#ifdef _DEBUG
				std::cout << name << " ... Quaddtree Add" << std::endl;
#endif // DEBUG

				// �� ������Ʈ�� ���� ���� ����� �����Ѵ�. �ʼ�����!
				LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(name);
				TestObject->SetTransform(transform);
				TestObject->UpdateTransform(NULL);

				// �浹ü�� ������Ŀ� ���߾� ����Ѵ�. 
				MyBOBox mybobox = *pBoBox;
				mybobox.Move(XMFLOAT3(TestObject->m_pChild->m_xmf4x4World._41, 0, TestObject->m_pChild->m_xmf4x4World._43));

				AddCollider(mybobox, TestObject->m_pChild->m_xmf4x4World);
				 
				delete TestObject;
				TestObject = nullptr;

			}

		}
		else if (!strcmp(pstrToken, "<Children>:"))
		{
			int nChilds = FileRead::ReadIntegerFromFile(pInFile);
			if (nChilds > 0)
			{
				for (int i = 0; i < nChilds; i++)
				{
					FileRead::ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<Frame>:"))
					{
						CreateTerrainObj(pInFile);
#ifdef _WITH_DEBUG_FRAME_HIERARCHY
						TCHAR pstrDebug[256] = { 0 };
						_stprintf_s(pstrDebug, 256, _T("(Frame: %p) (Parent: %p)\n"), pChild, pGameObject);
						OutputDebugString(pstrDebug);
#endif
					}
				}
			}
		}
		else if (!strcmp(pstrToken, "</Frame>"))
		{
			break;
		}
	}
}

void QuadtreeMgr::ProcessRecursiveCollide(const quadtree::NODE& node, Player& player, const MyBOBox& collider, float fElapsedTime)
{
	bool isCollided = Collision::isCollide(*node.BoBox, collider);
	if (!isCollided) return;

	bool isHaveChildren = node.children[0] != nullptr; // �ڽ��� �ִ� ���
	if (isHaveChildren)
	{
		for (int x = 0; x < 4; ++x)
		{
			ProcessRecursiveCollide(*node.children[x], player, collider, fElapsedTime);
		}
	}
	else
	{
		BoundingOrientedBox AlreadyPlayerBBox = player.CalculateAlreadyBoundingBox(fElapsedTime);
		XMFLOAT3 AlreadyPositon{ AlreadyPlayerBBox.Center.x, AlreadyPlayerBBox.Center.y, AlreadyPlayerBBox.Center.z };

		for (const auto& tobj : node.terrainObjBoBoxs)
		{
			XMFLOAT3 slideVector{ 0.f, 0.f, 0.f };

			bool isSlide = Collision::ProcessCollision(
				AlreadyPlayerBBox,
				tobj.BoBox,
				player.GetTransform().GetPosition(),
				player.GetVelocity(),
				fElapsedTime,
				true,
				slideVector);

			if (isSlide)
			{
				player.SetVelocity(slideVector);
			} 
		} 
	}
}

void QuadtreeMgr::AddRecursiveCollider(quadtree::NODE* pNode, const MyBOBox& BoBox, const XMFLOAT4X4& world)
{
	// �ش� ����� �浹ü�� �ε����������� �Ѿ��.
	bool isCollided = Collision::isCollide(*(pNode->BoBox), BoBox);
	if (!isCollided) return;

	bool isHaveChildren = pNode->children[0] != nullptr;
	if (isHaveChildren)
	{
		// �ڽ��� �ִ� ��� ����Լ��� �̿��Ͽ� children ���η� ����.
		for (int x = 0; x < 4; ++x)
		{
			if (pNode->children[x])
			{
				AddRecursiveCollider(pNode->children[x], BoBox, world);
			}
		}
	}
	else
	{
		// �ڽ��� �����Ƿ� ��������� ���̴�.
		// �浹�����Ƿ� ��� ����Ʈ�� �߰��Ѵ�.
		pNode->terrainObjCount += 1; 
		pNode->terrainObjBoBoxs.emplace_back(BoBox, world);
		return;
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
		m_RootNode = new quadtree::NODE(center, extents);
	}

	// Quadtree�� �����Ѵ�.
	CreateQuadTree();

	// �������� Quadtree�� ���� ���� ������Ʈ�� ������ �ִ´�.
	CreateTerrainObj(TERRAIN_OBJS_INFO_PATH);
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

void QuadtreeMgr::AddCollider(const MyBOBox& collider, const XMFLOAT4X4& world)
{
	AddRecursiveCollider(m_RootNode, collider, world);
}

void QuadtreeMgr::ProcessCollide(Player& player, const MyBOBox& collider, float fElapsedTime)
{
	ProcessRecursiveCollide(*m_RootNode, player, collider, fElapsedTime);
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
 