#pragma once
#include "Status.h"

class Shader;
class MyRectangle;

// HP ���� ����
class PlayerStatus
	: public Status
{
private:
	virtual void SetAnimationStateToHit() override;

private:
	Shader* m_pHPBarShader{ nullptr };
	MyRectangle* m_HpBar{ nullptr };
	 
public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;

public:
	PlayerStatus(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~PlayerStatus();

	virtual void Update(float) override;
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);

};