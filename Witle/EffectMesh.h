#pragma once
#include "Mesh.h"

class Texture;
class MyDescriptorHeap;

class EffectMesh :
	public Mesh
{
public:
	virtual void Render(ID3D12GraphicsCommandList * commandList, bool isGBuffers) override;

private:
	class EffectVertex
	{
	public:
		XMFLOAT3 position;
		XMFLOAT3 velocity;
		XMFLOAT4 diffuse;
		float startTime;
		float lifeTime;

	public:
		EffectVertex() { position = XMFLOAT3(0.0f, 0.0f, 0.0f); diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); }
		EffectVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse, XMFLOAT3 xmf3normal) { position = XMFLOAT3(x, y, z); diffuse = xmf4Diffuse; }
		EffectVertex(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3normal, XMFLOAT4 xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)) { position = xmf3Position; diffuse = xmf4Diffuse; }
		~EffectVertex() { }
	};

public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;

public:
	EffectMesh(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, UINT meshCount, 
		float startTimeMax = 6.f, float lifeTimeMax = 3.f, XMFLOAT4 diffuse = XMFLOAT4{1, 1,1,1});
	virtual ~EffectMesh();
	void CreateTexture(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandListconst, const wchar_t *pszFileName);

	virtual void Update(float ElapsedTime) override {};
private:
};

