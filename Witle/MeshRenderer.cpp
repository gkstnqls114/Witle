#include "stdafx.h"
#include "Mesh.h"
#include "MeshRenderer.h"


MeshRenderer::MeshRenderer() 
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Update()
{
}

void MeshRenderer::Render(ID3D12GraphicsCommandList * pd3dCommandList, Mesh * mesh)
{
	UINT componenetID = mesh->GetComponentID();

}
