#include "stdafx.h"

#include "GameScreen.h"
#include "MyRectangle.h"
#include "LineSphere.h"
#include "ShaderManager.h"
#include "DebugOption.h"

#include "RecognitionRange.h"

void RecognitionRange::ReleaseObjects()
{
#ifdef SHOW_DEBUGMESH
	if (m_DebugMesh)
	{
		m_DebugMesh->ReleaseObjects();
		delete m_DebugMesh;
		m_DebugMesh = nullptr;
	}
#endif // SHOW_DEBUGMESH
}

void RecognitionRange::ReleaseUploadBuffers()
{
#ifdef SHOW_DEBUGMESH
	if (m_DebugMesh) m_DebugMesh->ReleaseUploadBuffers();
#endif // SHOW_DEBUGMESH
} 
 
RecognitionRange::~RecognitionRange()
{
}

void RecognitionRange::CreateDebugMesh(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
#ifdef SHOW_DEBUGMESH
	m_DebugMesh = new LineSphere(m_pOwner, pd3dDevice, pd3dCommandList, XMFLOAT4(1, 0, 0, 0), m_RecognitionRange, m_RecognitionRange);
#endif // SHOW_DEBUGMESH
}

void RecognitionRange::Update(float fTimeElapsed)
{

}
 
void RecognitionRange::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{ 
	if (!DebugOption::GetisShowSphere()) return;

#ifdef SHOW_DEBUGMESH
	if(m_DebugMesh) m_DebugMesh->Render(pd3dCommandList, false);
#endif // SHOW_DEBUGMESH
}
 
