#pragma once
#include "Light.h"
#include "GameObject.h"

class LightObject
	: public GameObject
{
	Light* m_pLight{ nullptr };

private:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;

public:
	LightObject(const std::string& entityID);
	virtual ~LightObject();

	virtual void Update(float fElapsedTime) override;
};