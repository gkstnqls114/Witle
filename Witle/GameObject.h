#pragma once  
class Transform;
class ComponentBase;

class GameObject
{
protected:
	Transform* m_Transform{ nullptr }; //���庯ȯ�� ���� ��ǥ��
	ComponentBase* GetComponent(const std::string& id) const;

public:
	GameObject(std::string entityID);
	virtual ~GameObject();

	void Update();
	
	bool InsertComponent(const std::string& ComponenetID, ComponentBase* pComponentBase);
	
	template <typename T>
	T* GetComponent(const std::string& id) const
	{
		return static_cast<T *>(GetComponent(id));
	}
	template <>
	Transform* GetComponent(const std::string& id) const { return m_Transform; }
	
private:
	std::map< std::string, ComponentBase*> m_Components;
	std::string m_EntityID;
};
 