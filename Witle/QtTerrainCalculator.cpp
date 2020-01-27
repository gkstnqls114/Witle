#include "stdafx.h"

#include "d3dUtil.h" 
#include "Object.h"
#include "MyBOBox.h"
#include "ModelStorage.h"
#include "Collision.h"  
#include "Movement.h"
#include "Transform.h"
#include "GameObject.h"
#include "GameTimer.h"
#include "Monster.h"
#include "Player.h"

#include "QtTerrainCalculator.h"
 
    
void QtTerrainCalculator::CreateTerrainObj(const char* terrain_info_path)
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

void QtTerrainCalculator::CreateTerrainObj(FILE* pInFile)
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
 
void QtTerrainCalculator::AddDataListOfNode(quadtree::QT_COLLISION_NODE& node, const quadtree::QT_COLLIDER_ADDER& collider)
{
	// �ڽ��� �����Ƿ� ��������� ���̴�.
			// �浹�����Ƿ� ��� ����Ʈ�� �߰��Ѵ�.
	node.terrainObjCount += 1;
	node.terrainObjBoBoxs.push_back(collider);

}

void QtTerrainCalculator::ProcessDataOfNode(quadtree::QT_COLLISION_NODE& node, GameObject& gameobj)
{
	// ����ó�� 
	if (gameobj.GetpMovement() == nullptr) return;
	bool isNotMonster = dynamic_cast<Monster*>(&gameobj) == nullptr;
	bool isNotPlayer = dynamic_cast<Player*>(&gameobj) == nullptr;
	if (isNotMonster && isNotPlayer)
	{
		assert(!(isNotMonster && isNotPlayer) && "is not player or monster class");
	}
	else if (!isNotMonster && !isNotPlayer)
	{
		assert(!(!isNotMonster && !isNotPlayer) && "is player and monster class, why? i don't know..."); 
	}


	// ���� �� ���� �Ѿ�� �̷��� ����..
	float fElapsedTime = CGameTimer::GetInstance()->GetTimeElapsed();
	Movement* movement = gameobj.GetpMovement();
	BoundingOrientedBox nextFrameBoBox;
	if (!isNotMonster)
	{ 
		nextFrameBoBox = dynamic_cast<Monster*>(&gameobj)->CalculateAlreadyBoundingBox(fElapsedTime);
	}
	else if (!isNotPlayer)
	{ 
		nextFrameBoBox = dynamic_cast<Player*>(&gameobj)->CalculateAlreadyBoundingBox(fElapsedTime);
	}

	for (const auto& tobj : node.terrainObjBoBoxs)
	{
		XMFLOAT3 slideVector{ 0.f, 0.f, 0.f };

		// ���� �����ӿ��� �ͷ��� ������Ʈ�� �ε����� �� Ȯ���Ѵ�.
		bool isSlide = Collision::ProcessCollision(
			nextFrameBoBox,
			tobj.BoBox,
			movement->GetpOwner()->GetTransform().GetPosition(),
			movement->GetVelocity(),
			fElapsedTime,
			true,
			slideVector);

		if (isSlide)
		{
			movement->SetVelocity(slideVector);
		}
	}
}
  
QtTerrainCalculator::QtTerrainCalculator(const XMFLOAT3& center, const XMFLOAT3& extents, float min_size)
	: Quadtree<quadtree::QT_COLLISION_NODE, quadtree::QT_COLLIDER_ADDER>(center, extents, min_size)
{
}

QtTerrainCalculator::~QtTerrainCalculator()
{ 
}

void QtTerrainCalculator::Init()
{   
	// �������� Quadtree�� ���� ���� ������Ʈ�� ������ �ִ´�.
	CreateTerrainObj(TERRAIN_OBJS_INFO_PATH);
}
  
void QtTerrainCalculator::AddCollider(const MyBOBox& collider, const XMFLOAT4X4& world)
{
	quadtree::QT_COLLIDER_ADDER add_data(collider, world);
	AddRecursiveDataOfNode(*(GetpRoot()), collider, add_data);
}
 
void QtTerrainCalculator::ProcessCollide(const MyBOBox& collider, GameObject& gameobj)
{
	ProcessRecursiveDataOfNode(*GetpRoot(), collider, gameobj);
}
 
void QtTerrainCalculator::PrintInfo()
{
#ifdef _DEBUG
	std::cout << "Quadtree Leaf Node Info ... " << std::endl;
	int n = 0;
	for (int x = 0; x < GetReafNodeCount(); ++x)
	{
		n += 1;

		auto bobox = GetReaf(x).BoBox->GetBOBox();
		std::cout << "Index ... " << x <<std::endl;
		std::cout << "Center ... (" << bobox.Center.x << " , "<< bobox.Center.y << " , "<< bobox.Center.z << ")" << std::endl;
		std::cout << "Size ... (" << bobox.Extents.x * 2 << " , " << bobox.Extents.y * 2 << " , " << bobox.Extents.z * 2<< ")" << std::endl;

		if (n % 4 == 0) std::cout << std::endl;
	}
#endif // _DEBUG

}
 