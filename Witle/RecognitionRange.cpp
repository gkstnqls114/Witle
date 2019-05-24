#include "stdafx.h"

#include "GameScreen.h"
#include "MyRectangle.h"
#include "LineSphere.h"
#include "ShaderManager.h"

#include "RecognitionRange.h"

void RecognitionRange::ReleaseObjects()
{

}

void RecognitionRange::ReleaseUploadBuffers()
{

}

RecognitionRange::RecognitionRange(GameObject * pOwner)
	: ComponentBase(pOwner)
{

}
 
RecognitionRange::~RecognitionRange()
{
}

void RecognitionRange::CreateDebugMesh(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_DebugMesh = new LineSphere(m_pOwner, pd3dDevice, pd3dCommandList);
}

void RecognitionRange::Update(float fTimeElapsed)
{

}
 
void RecognitionRange::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{

}

void RecognitionRange::RenderDebug(ID3D12GraphicsCommandList * pd3dCommandList)
{
	if (m_DebugMesh) m_DebugMesh->Render(pd3dCommandList);
}
