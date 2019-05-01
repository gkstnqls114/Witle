#include "stdafx.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "LineCube.h"
#include "MyBSphere.h"

MyBSphere::MyBSphere(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, XMFLOAT3 center, float radius)
{
	m_BSphere = BoundingSphere(center, radius); 
#ifdef _SHOW_BOUNDINGBOX
	m_pLineCube = new LineCube(pd3dDevice, pd3dCommandList, m_BSphere.Center, XMFLOAT3(radius, radius, radius));
#endif // DEBUG 
}

MyBSphere::~MyBSphere()
{
#ifdef _SHOW_BOUNDINGBOX
	if (m_pLineCube)
	{
		delete m_pLineCube;
	}
#endif
}

void MyBSphere::ReleaseObjects()
{
#ifdef _SHOW_BOUNDINGBOX
	if(m_pLineCube) m_pLineCube->ReleaseObjects();
#endif // _SHOW_BOUNDINGBOX 
}

void MyBSphere::ReleaseUploadBuffers()
{
#ifdef _SHOW_BOUNDINGBOX
	if (m_pLineCube) m_pLineCube->ReleaseUploadBuffers();
#endif // _SHOW_BOUNDINGBOX 
}


#ifdef _SHOW_BOUNDINGBOX
void MyBSphere::Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT4X4& xmf4x4World)
{
	if (m_pLineCube)
	{
		pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Line")->GetPSO());
		m_pLineCube->Render(pd3dCommandList, xmf4x4World, true);
	}
}
#endif
 

void MyBSphere::Move(const XMFLOAT3 & xmf3Shift)
{
	m_BSphere.Center = Vector3::Add(m_BSphere.Center, xmf3Shift);
}
