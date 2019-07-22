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
public:
	static MyDescriptorHeap* m_pHeap;
	static Texture* m_pTexture;
	static int m_SelectedIndex[SKILL_SELECTED] ; // ���õ� �� ���� �ε�����
	 
protected:
	void BuildLightsAndMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	void RenderShadowMap(ID3D12GraphicsCommandList *pd3dCommandList);

private:
	void ClickSkillIcon(POINT cursor);

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

	// ��ų �Ŵ����� �ش� �ؽ��� �ִ´�.
	void FinishSkillSelect();

protected:

private:
	D3D12_VIEWPORT	m_d3dViewport;
	D3D12_RECT		m_d3dScissorRect;

	GameObject* m_TESTGameObject{ nullptr };
	  
	UI2DImage* m_UISkillToChoose[SKILL_TO_CHOOSE]; // �� �� �ִ� ��ų ui ��

	UI2DImage* m_UISkillSelected[SKILL_SELECTED]; // �� ��ų��
	 
	POINT choosePoint[SKILL_TO_CHOOSE] ;
	POINT selectedPoint[SKILL_SELECTED];

	// �ؽ��� ���� ������
	// �ؽ��� ���� ������
};