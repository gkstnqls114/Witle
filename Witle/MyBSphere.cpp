#include "stdafx.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "LineCube.h"
#include "MyBSphere.h"

MyBSphere::MyBSphere(GameObject* pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, float radius, XMFLOAT3 center)
	:MyCollider(pOwner, COLLIDER_TYPE::BOUNDING_SPHERE)
{
	m_BSphere = BoundingSphere(center, radius); 

	m_pLineCube = new LineCube(pOwner, pd3dDevice, pd3dCommandList, m_BSphere.Center, XMFLOAT3(radius, radius, radius));


}

MyBSphere::~MyBSphere()
{

	if (m_pLineCube)
	{
		delete m_pLineCube;
	}

}

void MyBSphere::ReleaseObjects()
{

	if(m_pLineCube) m_pLineCube->ReleaseObjects();

}

void MyBSphere::ReleaseUploadBuffers()
{

	if (m_pLineCube) m_pLineCube->ReleaseUploadBuffers();

}



void MyBSphere::Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT4X4& xmf4x4World)
{
	if (m_pLineCube)
	{
		pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Line")->GetPSO());
		m_pLineCube->Render(pd3dCommandList, xmf4x4World, true);
	}
}

 

void MyBSphere::Move(const XMFLOAT3 & xmf3Shift)
{
	m_BSphere.Center = Vector3::Add(m_BSphere.Center, xmf3Shift);
}
