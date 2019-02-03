#pragma once
#include "Mesh.h"

class CubeMesh :
	public Mesh
{
public:
	CubeMesh(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float width, float height, float depth);
	virtual ~CubeMesh();
	
	virtual void Update(float ElapsedTime) override {};
	
private:
};

