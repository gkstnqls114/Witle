#pragma once
#include "Scene.h"
 
class Button;
class Texture;
class MyRectangle;
class GameObject;
class UI2DImage;
class Shader;

class MainScene
	: public Scene
{ 
protected: 
	void BuildLightsAndMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	void RenderShadowMap(ID3D12GraphicsCommandList *pd3dCommandList);


public:
	MainScene();
	virtual ~MainScene();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float ElapsedTime) override;

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) override;
	virtual void ReleaseObjects() override;

	virtual bool ProcessInput(HWND hWnd, float ElapsedTime) override;
	virtual void UpdatePhysics(float ElapsedTime) override;

	virtual void AnimateObjects(float fTimeElapsed) override;
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override;
	virtual void RenderForShadow(ID3D12GraphicsCommandList *pd3dCommandList) override;
	virtual void RenderForPlayerShadow(ID3D12GraphicsCommandList *pd3dCommandList) override {};

	virtual void ReleaseUploadBuffers() override;

	virtual void Update(float ElapsedTime) override;
	virtual void LastUpdate(float ElapsedTime) override;
	 
protected:  

private:
	D3D12_VIEWPORT	m_d3dViewport;
	D3D12_RECT		m_d3dScissorRect;

	Shader* m_pSceneShader{ nullptr };
	GameObject* m_gameobject{ nullptr };
	UI2DImage* m_Background{ nullptr }; 
};