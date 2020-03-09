#include "stdafx.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "LineCube.h"
#include "DebugOption.h"
#include "MyBSphere.h"

MyBSphere::MyBSphere(GameObject* pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, float radius, XMFLOAT3 center)
	:MyCollider(pOwner, COLLIDER_TYPE::BOUNDING_SPHERE)
{
	m_BSphere = BoundingSphere(center, radius);  

#ifdef SHOW_DEBUGMESH
	m_pLineCube = new LineCube(pOwner, pd3dDevice, pd3dCommandList, m_BSphere.Center, XMFLOAT3(radius, radius, radius));
#endif // SHOW_DEBUGMESH
}

MyBSphere::~MyBSphere()
{
#ifdef SHOW_DEBUGMESH
	if (m_pLineCube)
	{
		delete m_pLineCube;
	}
#endif // SHOW_DEBUGMESH
}

void MyBSphere::ReleaseObjects()
{
#ifdef SHOW_DEBUGMESH
	if(m_pLineCube) m_pLineCube->ReleaseObjects();
#endif // SHOW_DEBUGMESH
}

void MyBSphere::ReleaseUploadBuffers()
{
#ifdef SHOW_DEBUGMESH
	if (m_pLineCube) m_pLineCube->ReleaseUploadBuffers();
#endif // SHOW_DEBUGMESH
}

void MyBSphere::Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT4X4& xmf4x4World)
{
	if (!DebugOption::GetisShowSphere()) return;

#ifdef SHOW_DEBUGMESH
	if (m_pLineCube)
	{
		ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_LINE, false);
		m_pLineCube->Render(pd3dCommandList, xmf4x4World, true);
	}
#endif // SHOW_DEBUGMESH
}

 

void MyBSphere::Move(const XMFLOAT3 & xmf3Shift)
{
	m_BSphere.Center = Vector3::Add(m_BSphere.Center, xmf3Shift);
}
