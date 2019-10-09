#pragma once
#include "Scene.h"


class UI2DImage;
class Texture;
class MyDescriptorHeap;

class SkillSelectScene
	: public Scene
{
public:
	static MyDescriptorHeap* m_pHeap;
	static Texture* m_pTexture;
	static int m_SelectedIndex[SKILL_SELECTED] ; // 선택된 네 개의 인덱스들
	 
protected:
	void BuildLightsAndMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	void RenderShadowMap(ID3D12GraphicsCommandList *pd3dCommandList);

private:
	void ClickSkillIcon(POINT& cursor);
	void CllckGameStartButton(POINT& cursor);
	
public:
	SkillSelectScene();
	virtual ~SkillSelectScene();

	// 마우스 메시지를 처리합니다.
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;

	// 키보드 메시지를 처리합니다.
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float ElapsedTime) override;

	// 필요한 오브젝트들을 빌드합니다.
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) override;

	virtual void ReleaseUploadBuffers() override;
	virtual void ReleaseObjects() override;

	// 화면에 그림을 그리는 함수.
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override;
	virtual void RenderForShadow(ID3D12GraphicsCommandList *pd3dCommandList) override;
	virtual void RenderForPlayerShadow(ID3D12GraphicsCommandList *pd3dCommandList) override {};

	//// UpdateGamelogic ///////////////////////////////////

	// GameFramework의 UpdateGamelogic 함수 내부에서 UpdatePhysics, AnimateObjects, LastUpdate 순으로 호출되는 함수. 
	virtual bool ProcessInput(HWND hWnd, float ElapsedTime) override;

	// GameFramework의 UpdateGamelogic 함수 내부에서 UpdatePhysics, AnimateObjects, LastUpdate 순으로 호출되는 함수. 
	virtual void UpdatePhysics(float ElapsedTime) override;

	// GameFramework의 UpdateGamelogic 함수 내부에서 UpdatePhysics, AnimateObjects, LastUpdate 순으로 호출되는 함수. 
	virtual void Update(float ElapsedTime) override;

	// GameFramework의 UpdateGamelogic 함수 내부에서 UpdatePhysics, AnimateObjects, LastUpdate 순으로 호출되는 함수. 
	virtual void AnimateObjects(float fTimeElapsed) override;

	// GameFramework의 UpdateGamelogic 함수 내부에서 UpdatePhysics, AnimateObjects, LastUpdate 순으로 호출되는 함수. 

	virtual void LastUpdate(float ElapsedTime) override;
	//// UpdateGamelogic ///////////////////////////////////

	// SkillStg에서 스킬을 꺼내 PlayerSkillMgr에서 사용할 스킬을 설정합니다.
	void FinishSkillSelect();

protected:

private:
	D3D12_VIEWPORT	m_d3dViewport;
	D3D12_RECT		m_d3dScissorRect;

	GameObject* m_TESTGameObject{ nullptr };
	  
	UI2DImage* m_UISkillToChoose[SKILL_TO_CHOOSE]; // 고를 수 있는 스킬 ui 들

	UI2DImage* m_UISkillSelected[SKILL_SELECTED]; // 고른 스킬들

	UI2DImage* m_GameStartButton; // 게임 시작 버튼

	POINT choosePoint[SKILL_TO_CHOOSE] ;
	POINT selectedPoint[SKILL_SELECTED];
};