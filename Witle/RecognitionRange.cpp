#include "stdafx.h"

#include "GameScreen.h"
#include "MyRectangle.h"
#include "LineSphere.h"
#include "ShaderManager.h"

#include "RecognitionRange.h"

void RecognitionRange::ReleaseObjects()
{
	if (m_DebugMesh)
	{
		m_DebugMesh->ReleaseObjects();
		delete m_DebugMesh;
		m_DebugMesh = nullptr;
	}
}

void RecognitionRange::ReleaseUploadBuffers()
{
	if (m_DebugMesh) m_DebugMesh->ReleaseUploadBuffers();
} 
 
RecognitionRange::~RecognitionRange()
{
}

void RecognitionRange::CreateDebugMesh(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_DebugMesh = new LineSphere(m_pOwner, pd3dDevice, pd3dCommandList, m_RecognitionRange, m_RecognitionRange);
}

void RecognitionRange::Update(float fTimeElapsed)
{

}
 
void RecognitionRange::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{

}

void RecognitionRange::RenderDebug(ID3D12GraphicsCommandList * pd3dCommandList)
{
	if (m_DebugMesh)
	{
		m_DebugMesh->Render(pd3dCommandList);
	}
}
