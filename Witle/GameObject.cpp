#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject(EntityID entityID)
{
}

GameObject::~GameObject()
{
}

bool GameObject::InsertComponent(ComponentBase * pComponentBase)
{
	return false;
}

ComponentBase * GameObject::GetComponent(const ComponenetID & id)
{
	return nullptr;
}

const ComponentBase * GameObject::GetComponent(const ComponenetID & id) const
{
	return nullptr;
}
