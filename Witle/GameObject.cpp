#include "stdafx.h" 
#include "Mesh.h"
#include "ComponentBase.h"
#include "GameObject.h"
 
void GameObject::ReleaseComponents()
{  
	for (auto& component : m_Components)
	{
		if (component.second)
		{
			component.second->ReleaseObjects();
			delete component.second;
			component.second = nullptr;
		}
	}

	m_Components.clear();
}

void GameObject::ReleaseComponentUploadBuffers()
{ 
	for (auto& component : m_Components)
	{
		if (component.second->GetFamilyID().isMesh())
		{
			static_cast<Mesh*>(component.second)->ReleaseUploadBuffers();
		}
	}
}

GameObject::GameObject(const std::string & entityID)
#ifdef _DEBUG
	: m_EntityID(entityID), m_Transform(this)
#else // _DEBUG
	: m_Transform(this)
#endif
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
	if (pair.second)
	{
		return true;
	}
	else
	{
		return false;
	}
}

ComponentBase * GameObject::GetComponent(const std::string & id) const
{ 
	assert(m_Components.find(id) != m_Components.end()); // ���� �ش� id�� ������Ʈ ���������ʴ´ٸ� ���

	ComponentBase* result = (*(m_Components.find(id))).second;
	return result;
}
 