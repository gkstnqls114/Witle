#include "stdafx.h" 
#include "Mesh.h"
#include "ComponentBase.h"
#include "GameObject.h"
 
void GameObject::ReleaseComponents()
{ 
	for (auto& component : m_Components)
	{
		delete component.second;
		component.second = nullptr;
	}

	m_Components.clear();
}

void GameObject::ReleaseComponentUploadBuffers()
{ 
	for (auto& component : m_Components)
	{
		if (component.second->GetFamilyID() == FAMILYID_MESH)
		{
			static_cast<Mesh*>(component.second)->ReleaseUploadBuffers();
		}
	}
}

GameObject::GameObject(const std::string & entityID)
	: m_EntityID(entityID), m_Transform(this)
{

}

GameObject::~GameObject()
{

}

void GameObject::ReleaseObjects()
{
	ReleaseComponents();
	ReleaseMembers();
}

void GameObject::ReleaseUploadBuffers()
{
	ReleaseComponentUploadBuffers();
	ReleaseMemberUploadBuffers();
}

bool GameObject::InsertComponent(const std::string & ComponenetID, ComponentBase * pComponentBase)
{
	auto pair = m_Components.insert(std::pair< std::string, ComponentBase*>(ComponenetID, pComponentBase));
	if (pair.second) {
#ifdef CHECK_CONSOLE_TEST
		std::cout << m_EntityID << " ... " << "( ID: " << ComponenetID << " , " << pComponentBase << " ... Insert Componenet SUCCESS" << std::endl;
#endif // CHECK_CONSOLE_TEST 
		return true;
	}
	else { 
#ifdef CHECK_CONSOLE_TEST
		std::cout << m_EntityID << " ... " << "( ID: " << ComponenetID << " , " << pComponentBase << " ... Insert Componenet FAIL" << std::endl;
#endif // CHECK_CONSOLE_TEST 
		return false;
	}
}

ComponentBase * GameObject::GetComponent(const std::string & id) const
{ 
	assert(m_Components.find(id) != m_Components.end()); // 만약 해당 id의 컴포넌트 존재하지않는다면 경고

	ComponentBase* result = (*(m_Components.find(id))).second;
	return result;
}
 