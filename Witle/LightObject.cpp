#include "stdafx.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "LightObject.h"

void LightObject::ReleaseMembers()
{
}

void LightObject::ReleaseMemberUploadBuffers()
{
}

LightObject::LightObject(const std::string& entityID)
	:GameObject(entityID)
{
	// ���⼺ ������ �⺻���� �����Ѵ�.
	m_pLight = new DirectionalLight(this);
}

LightObject::~LightObject()
{
	if (m_pLight)
	{
		delete m_pLight;
		m_pLight = nullptr;
	}
}

void LightObject::Update(float fElapsedTime)
{
	m_pLight->Update(fElapsedTime);
}
