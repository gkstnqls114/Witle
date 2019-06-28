#include "stdafx.h"
#include "d3dUtil.h"
#include "Object.h"
#include "MyBOBox.h"
#include "Shader.h"
#include "Object.h"
#include "ShaderManager.h"
#include "ModelStorage.h"
#include "StaticObject.h"

void StaticObject::ReleaseMembers()
{
}

void StaticObject::ReleaseMemberUploadBuffers()
{
}
  
StaticObject::StaticObject(const std::string &EntityID)
	:GameObject(EntityID)
{
}
 