#pragma once
#include "ComponentBase.h"

class Shader;
class MyRectangle;

// HP ���� ����
class Status
	: ComponentBase
{ 
public:
	int m_HP{ 1000 };

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	Status(GameObject* pOwner );
	virtual ~Status(); 

};