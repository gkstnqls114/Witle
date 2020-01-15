#pragma once
#include "Scene.h"

class UI2DImage;
class GameObject;

class LoseScene
	: public Scene
{ 
protected:
	void BuildLightsAndMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	void RenderShadowMap(ID3D12GraphicsCommandList *pd3dCommandList);
	 
public:
	LoseScene();
	virtual ~LoseScene();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) override;
	virtual void ReleaseObjects() override;

	virtual bool ProcessInput(HWND hWnd) override;

	virtual void AnimateObjects() override;
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override;
	
	// 그림자를 렌더링하기 위한 함수입니다.
	virtual void RenderForShadow(ID3D12GraphicsCommandList *pd3dCommandList) override;

	// 플레이어 그림자를 렌더링하기 위한 함수입니다.
	virtual void RenderForPlayerShadow(ID3D12GraphicsCommandList *pd3dCommandList) override;

	virtual void ReleaseUploadBuffers() override;

	virtual void UpdatePhysics() override;
	virtual void Update() override;
	virtual void LastUpdate() override;

protected:

private:
	D3D12_VIEWPORT	m_d3dViewport;
	D3D12_RECT		m_d3dScissorRect;

	float m_WaitingTime{ 0.f };
	float m_MAXWaitingTime{ 3.f };

	GameObject* m_gameobject{ nullptr };
	UI2DImage* m_Background{ nullptr };
};