#pragma once
#include "Status.h"

class Shader;
class MyRectangle;

// 플레이어의 hp나 mp와 같은 게이지 관련 정보를 관리하고 렌더링하는 클래스입니다.
class PlayerStatus
	: public Status
{
private:
	virtual void SetAnimationStateToHit() override;

private:
	Shader* m_pGuageBarShader{ nullptr }; 
	MyRectangle* m_GuageBar{ nullptr }; 
	 
public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;

public:
	PlayerStatus(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList,
		POINT point, float width, float height, const wchar_t * filepath);
	virtual ~PlayerStatus();

	virtual void Update(float) override;
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);

	virtual void SubstractHP(float damage) override;
	virtual void Damage(int damage, int state) {};
};