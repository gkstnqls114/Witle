#pragma once
#include "Scene.h"

#define SKILL_TO_CHOOSE 8
#define SKILL_SELECTED 4

class UI2DImage;
class Texture;
class MyDescriptorHeap;

class SkillSelectScene
	: public Scene
{
private:
	static ID3D12DescriptorHeap			*m_pd3dCbvSrvDescriptorHeap;

	static D3D12_CPU_DESCRIPTOR_HANDLE	m_d3dCbvCPUDescriptorStartHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_d3dCbvGPUDescriptorStartHandle;
	static D3D12_CPU_DESCRIPTOR_HANDLE	m_d3dSrvCPUDescriptorStartHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_d3dSrvGPUDescriptorStartHandle;

	static D3D12_CPU_DESCRIPTOR_HANDLE	m_d3dCbvCPUDescriptorNextHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_d3dCbvGPUDescriptorNextHandle;
	static D3D12_CPU_DESCRIPTOR_HANDLE	m_d3dSrvCPUDescriptorNextHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_d3dSrvGPUDescriptorNextHandle;

protected:
	static void CreateCbvSrvDescriptorHeaps(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews);

	static D3D12_GPU_DESCRIPTOR_HANDLE CreateConstantBufferViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, ID3D12Resource *pd3dConstantBuffers, UINT nStride);
	static D3D12_GPU_DESCRIPTOR_HANDLE CreateShaderResourceViews(ID3D12Device *pd3dDevice, Texture *pTexture, UINT nRootParameter, bool bAutoIncrement);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUCbvDescriptorStartHandle() { return(m_d3dCbvCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUCbvDescriptorStartHandle() { return(m_d3dCbvGPUDescriptorStartHandle); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSrvDescriptorStartHandle() { return(m_d3dSrvCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSrvDescriptorStartHandle() { return(m_d3dSrvGPUDescriptorStartHandle); }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUCbvDescriptorNextHandle() { return(m_d3dCbvCPUDescriptorNextHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUCbvDescriptorNextHandle() { return(m_d3dCbvGPUDescriptorNextHandle); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSrvDescriptorNextHandle() { return(m_d3dSrvCPUDescriptorNextHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSrvDescriptorNextHandle() { return(m_d3dSrvGPUDescriptorNextHandle); }

protected:
	void BuildLightsAndMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	void RenderShadowMap(ID3D12GraphicsCommandList *pd3dCommandList);


public:
	SkillSelectScene();
	virtual ~SkillSelectScene();

	// ���콺 �޽����� ó���մϴ�.
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;

	// Ű���� �޽����� ó���մϴ�.
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float ElapsedTime) override;

	// �ʿ��� ������Ʈ���� �����մϴ�.
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) override;

	virtual void ReleaseUploadBuffers() override;
	virtual void ReleaseObjects() override;

	// ȭ�鿡 �׸��� �׸��� �Լ�.
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override;
	virtual void RenderForShadow(ID3D12GraphicsCommandList *pd3dCommandList) override;

	//// UpdateGamelogic ///////////////////////////////////

	// GameFramework�� UpdateGamelogic �Լ� ���ο��� UpdatePhysics, AnimateObjects, LastUpdate ������ ȣ��Ǵ� �Լ�. 
	virtual bool ProcessInput(HWND hWnd, float ElapsedTime) override;

	// GameFramework�� UpdateGamelogic �Լ� ���ο��� UpdatePhysics, AnimateObjects, LastUpdate ������ ȣ��Ǵ� �Լ�. 
	virtual void UpdatePhysics(float ElapsedTime) override;

	// GameFramework�� UpdateGamelogic �Լ� ���ο��� UpdatePhysics, AnimateObjects, LastUpdate ������ ȣ��Ǵ� �Լ�. 
	virtual void Update(float ElapsedTime) override;

	// GameFramework�� UpdateGamelogic �Լ� ���ο��� UpdatePhysics, AnimateObjects, LastUpdate ������ ȣ��Ǵ� �Լ�. 
	virtual void AnimateObjects(float fTimeElapsed) override;

	// GameFramework�� UpdateGamelogic �Լ� ���ο��� UpdatePhysics, AnimateObjects, LastUpdate ������ ȣ��Ǵ� �Լ�. 

	virtual void LastUpdate(float ElapsedTime) override;
	//// UpdateGamelogic ///////////////////////////////////


protected:

private:
	D3D12_VIEWPORT	m_d3dViewport;
	D3D12_RECT		m_d3dScissorRect;

	GameObject* m_TESTGameObject{ nullptr };
	  
	UI2DImage* m_UISkillToChoose[SKILL_TO_CHOOSE]; // �� �� �ִ� ��ų ui ��

	UI2DImage* m_UISkillSelected[SKILL_SELECTED]; // �� ��ų��


	MyDescriptorHeap* m_pHeap{ nullptr };
	Texture* m_pTexture{ nullptr };

	UINT m_SelectedIndex[SKILL_SELECTED]{ 0, 0, 0, 0 };
};