#pragma once
#include "Renderer.h"

class Mesh;
class TerrainMesh;

class MeshRenderer :
	public Renderer
{ 
private:
	void RenderCubeMesh(ID3D12GraphicsCommandList * pd3dCommandList, Mesh* mesh);
	void RenderTerrainMesh(ID3D12GraphicsCommandList * pd3dCommandList, Mesh* mesh);

public:
	MeshRenderer();
	virtual ~MeshRenderer();
	 
	virtual void Update();
	virtual void Render(ID3D12GraphicsCommandList * pCommandList, Mesh* mesh);

};

