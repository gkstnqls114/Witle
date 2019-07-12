#pragma once  
#include "Transform.h"
class ComponentBase;
class ResourceBase;

class IGameObject
{
protected:
	// ������Ʈ�� �ƴ�, ���ӿ�����Ʈ ������ �����Ҵ�� ��������� �����Ѵ�.
	virtual void ReleaseMembers() = 0;
	// ������Ʈ�� �ƴ�, ���ӿ�����Ʈ ������ �����Ҵ�� ���ε� ���� �����Ѵ�.
	virtual void ReleaseMemberUploadBuffers() = 0;

public: 
	// GameObject::Update�� ��� ������ �Ļ��� ���� ������Ʈ���� �ٸ���.
	virtual void Update(float fElapsedTime) = 0; 
};

class GameObject
	: public IGameObject
{
protected:
	virtual void ReleaseMembers() = 0;
	virtual void ReleaseMemberUploadBuffers() = 0;

public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) = 0;

private:
	std::map< std::string, ComponentBase*> m_Components;
	std::string m_EntityID; 

protected:
	Transform m_Transform; //���庯ȯ�� ���� ��ǥ��
	ComponentBase* GetComponent(const std::string& id) const;
	 
private:
	void ReleaseComponents();
	void ReleaseComponentUploadBuffers();
	 

public:
	GameObject(const std::string& entityID);
	virtual ~GameObject();

	virtual void Update(float fElapsedTime) override {};

	// delete ������ �ݵ�� ȣ��
	void ReleaseObjects();
	void ReleaseUploadBuffers();

	bool InsertComponent(const std::string& ComponenetID, ComponentBase* pComponentBase);

	template <typename T>
	T* GetComponent(const std::string& id = "") const
	{
		assert(id != "");
		return static_cast<T *>(GetComponent(id));
	}

	const Transform& GetTransform() const { return m_Transform; }
	Transform& GetTransform() { return m_Transform; }
	const std::string& GetName() const { return m_EntityID; }
};

class EmptyGameObject
	: public GameObject
{
public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override {};

protected:
	virtual void ReleaseMembers() {};
	virtual void ReleaseMemberUploadBuffers() {};

public:
	EmptyGameObject(const std::string& entityID) : GameObject(entityID) {};
	virtual ~EmptyGameObject() {};
};