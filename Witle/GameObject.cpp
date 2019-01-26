#include "stdafx.h"
#include "Transform.h"
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

GameObject::GameObject(const std::string & entityID)
	: m_EntityID(entityID)
{
	m_Transform = new Transform(this);
}

GameObject::~GameObject()
{
	if (m_Transform)
	{
		delete m_Transform;
		m_Transform = nullptr;
	}
}

bool GameObject::InsertComponent(const std::string & ComponenetID, ComponentBase * pComponentBase)
{
	auto pair = m_Components.insert(std::pair< std::string, ComponentBase*>(ComponenetID, pComponentBase));
	if (pair.second) {
#ifdef CONSOLE_TEST
		std::cout << m_EntityID << " ... " << "( ID: " << ComponenetID << " , " << pComponentBase << " ... Insert Componenet SUCCESS" << std::endl;
#endif // CONSOLE_TEST 
		return true;
	}
	else { 
#ifdef CONSOLE_TEST
		std::cout << m_EntityID << " ... " << "( ID: " << ComponenetID << " , " << pComponentBase << " ... Insert Componenet FAIL" << std::endl;
#endif // CONSOLE_TEST 
		return false;
	}
}

ComponentBase * GameObject::GetComponent(const std::string & id) const
{ 
	assert(m_Components.find(id) != m_Components.end()); // ���� �ش� id�� ������Ʈ ���������ʴ´ٸ� ���

	ComponentBase* result = (*(m_Components.find(id))).second;
	return result;
}
 