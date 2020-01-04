#pragma once
#include "Scene.h"

// Console�� ���� �׽�Ʈ�� ��
class TestScene
	: public Scene
{
private:
	D3D12_VIEWPORT	m_d3dViewport;
	D3D12_RECT		m_d3dScissorRect;


protected: 
	 
public:
	TestScene();
	virtual ~TestScene();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float ElapsedTime) override;

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) override;
	virtual void ReleaseObjects() override;

	virtual bool ProcessInput(HWND hWnd, float ElapsedTime) override;

	virtual void AnimateObjects(float fTimeElapsed) override;
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, bool isGBuffers) override;

	// �׸��ڸ� �������ϱ� ���� �Լ��Դϴ�.
	virtual void RenderForShadow(ID3D12GraphicsCommandList* pd3dCommandList) override;

	// �÷��̾� �׸��ڸ� �������ϱ� ���� �Լ��Դϴ�.
	virtual void RenderForPlayerShadow(ID3D12GraphicsCommandList* pd3dCommandList) override;

	virtual void ReleaseUploadBuffers() override;

	virtual void UpdatePhysics(float ElapsedTime) override;
	virtual void Update(float ElapsedTime) override;
	virtual void LastUpdate(float ElapsedTime) override;

protected:

	 
};