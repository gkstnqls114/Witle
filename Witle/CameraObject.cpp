#include "stdafx.h"
#include "Camera.h"
#include "CameraObject.h"

CameraObject::CameraObject(const std::string & entityID)
	:GameObject(entityID)
{

}

CameraObject::~CameraObject()
{
}

void CameraObject::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
}

void CameraObject::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
}

void CameraObject::ReleaseShaderVariables()
{
}

void CameraObject::ChangeCamera(Camera * pNewCamera)
{
}
