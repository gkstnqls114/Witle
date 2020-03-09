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
				int nRootChild = FileRead::ReadIntegerFromFile(pInFile); // 모든 오브젝트 개수

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
						// AddCollider 함수를 통해 리프 노드에 충돌체를 추가한다.
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

			// fbx sdk 에서 꺼내올때 x, z좌표에 -부호 붙여야함 ...
			transform._41 = -(temp._41);
			transform._42 = 0; // y좌표는 0으로 고정
			if (!strcmp(name, Flower.c_str())) transform._42 = temp._42;
			transform._43 = -(temp._43);

			assert(!(temp._41 >= 40000));
			assert(!(temp._43 >= 40000));


			// 모델Stg에 존재하는 이름과 현재 이름이 일치하는지 확인한다.
			for (const auto& modelname : ModelStorage::GetInstance()->m_NameList)
			{
				// Object001을 예외로 Cliff , 절벽을 의미한다.
				if (!(!strcmp(name, modelname.c_str()) || !strcmp(name, "Object001")))
				{
					continue;
				}
				
				if (!strcmp(name, "Object001"))
				{
					strncpy(name, "Cliff", 64); // Object001을 Cliff로 취급하도록 이름을 바꾼다.
				}
				 
				// 모든 모델은 충돌체를 지니고있지 않다. 한번 확인해준다.
				MyBOBox* pBoBox = ModelStorage::GetInstance()->GetBOBox(name);
				if (pBoBox == nullptr) continue; // 없으면 넘어간다.
				 
				// 모델 오브젝트에 대한 월드 행렬을 재계산한다. 필수과정!
				LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(name);
				TestObject->SetTransform(transform);
				TestObject->UpdateTransform(NULL);

				// 충돌체를 월드행렬에 맞추어 계산한다. 
				MyBOBox mybobox = *pBoBox;
				mybobox.Move(XMFLOAT3(TestObject->m_pChild->m_xmf4x4World._41, 0, TestObject->m_pChild->m_xmf4x4World._43));

				AddCollider(mybobox, TestObject->m_pChild->m_xmf4x4World);
				 
#ifdef _DEBUG 
				std::cout << name << "... add position... " << TestObject->m_pChild->m_xmf4x4World._41 << " , " << TestObject->m_pChild->m_xmf4x4World._43 << std::endl;
#endif // DEBUG 

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
	// 자식이 없으므로 리프노드라는 뜻이다.
			// 충돌했으므로 노드 리스트에 추가한다.
	node.terrainObjCount += 1;
	node.terrainObjBoBoxs.push_back(collider);

}

void QtTerrainCalculator::ProcessDataOfNode(quadtree::QT_COLLISION_NODE& node, GameObject& gameobj)
{
	// 예외처리 
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


	// 만약 위 조건 넘어갈시 이렇게 진행..
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

		// 다음 프레임에서 터레인 오브젝트와 부딪히는 지 확인한다.
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
	// 나누어진 Quadtree를 통해 지형 오브젝트의 정보를 넣는다.
	CreateTerrainObj(TERRAIN_OBJS_INFO_PATH.c_str());
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

		auto bobox = GetpReaf(x)->BoBox->GetBOBox();
		std::cout << "Index ... " << x <<std::endl;
		std::cout << "Center ... (" << bobox.Center.x << " , "<< bobox.Center.y << " , "<< bobox.Center.z << ")" << std::endl;
		std::cout << "Size ... (" << bobox.Extents.x * 2 << " , " << bobox.Extents.y * 2 << " , " << bobox.Extents.z * 2<< ")" << std::endl;

		if (n % 4 == 0) std::cout << std::endl;
	}
#endif // _DEBUG

}
 