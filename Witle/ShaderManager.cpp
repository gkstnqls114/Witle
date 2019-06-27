#include "stdafx.h" 

//// Shader ////////////////////////// 
#include "SkyBoxShader.h"
#include "StandardShader.h"
#include "TerrainShader.h"
#include "SkinnedShader.h"
#include "HorizonBlurShader.h"
#include "VerticalBlurShader.h"
#include "InstancingStandardShader.h"
#include "ShowTextureShader.h"
#include "ScreenShader.h"
#include "CubeShader.h"
#include "InstancingLineShader.h"
#include "PickingPointShader.h"
#include "LineShader.h" 
#include "UIScreenShader.h"
#include "UIWorldShader.h"
//// Shader ////////////////////////// 

#include "ShaderManager.h"

ShaderManager* ShaderManager::m_ShaderManagerInstance{ nullptr };

ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
}

ShaderManager * ShaderManager::GetInstance()
{
	if (!m_ShaderManagerInstance) {
		m_ShaderManagerInstance = new ShaderManager;
	}
	return m_ShaderManagerInstance;
}

void ShaderManager::ReleaseInstance()
{
	if (!m_ShaderManagerInstance) return;

	// 순회하며 메모리 할당 제거.
	for (auto& pso : m_ShaderManagerInstance->m_Shaders) {
		delete pso.second;
		pso.second = nullptr;
	}

	if (m_ShaderManagerInstance) {
		delete m_ShaderManagerInstance;
		m_ShaderManagerInstance = nullptr;
	}
}

void ShaderManager::ReleaseObjects()
{
	for (auto& shader : m_Shaders)
	{ 
		shader.second->ReleaseObjects();
		delete shader.second;
		shader.second = nullptr;
	}
	m_Shaders.clear();
}

void ShaderManager::BuildShaders(ID3D12Device * pd3dDevice, ID3D12RootSignature * const pd3dGraphicsRootSignature)
{ 
	Shader* pCubeShader = new CubeShader();
	pCubeShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader("Cube", pCubeShader);

	Shader* pTerrainShader = new TerrainShader();
	pTerrainShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader("Terrain", pTerrainShader);

	Shader* pStandardShader = new StandardShader();
	pStandardShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader("StandardShader", pStandardShader);

	Shader* pInstancingStandardShader = new InstancingStandardShader();
	pInstancingStandardShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader("InstancingStandardShader", pInstancingStandardShader);

	Shader* pSkinnedShader = new SkinnedShader();
	pSkinnedShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader("SkinnedShader", pSkinnedShader);

	Shader* pScreenShader = new ScreenShader();
	pScreenShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader(SHADER_SCREEN, pScreenShader);

	Shader* pSkyBoxShader = new SkyBoxShader();
	pSkyBoxShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader("SkyBoxShader", pSkyBoxShader);

	Shader* pPickingPointShader = new PickingPointShader();
	pPickingPointShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader(SHADER_PICKINGPOINT, pPickingPointShader);

	Shader* pLineShader = new LineShader();
	pLineShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader("Line", pLineShader);

	Shader* pInstancingLineShader = new InstancingLineShader();
	pInstancingLineShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader("InstancingLine", pInstancingLineShader);

	Shader* pUIScreenShader = new UIScreenShader();
	pUIScreenShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader(SHADER_UISCREEN, pUIScreenShader);

	Shader* pUIWorldShader = new UIWorldShader();
	pUIWorldShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader(SHADER_UIWORLD, pUIWorldShader); 

	Shader* pShowTextureShader = new ShowTextureShader();
	pShowTextureShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader(SHADER_SHOWTEXTURE, pShowTextureShader);
}

bool ShaderManager::InsertShader(const std::string& s, Shader * pso)
{
	auto pair = m_Shaders.insert(std::pair<std::string, Shader*>(s, pso));
	if (pair.second) {
		return true;
	}
	else {
		return false;
	}
}

Shader * ShaderManager::GetShader(const std::string & s) const
{
	return (*(m_Shaders.find(s))).second;
}

void ShaderManager::SetPSO(ID3D12GraphicsCommandList * pd3dCommandList, const std::string& name, bool isGBuffers) const
{
	if (isGBuffers) SetPSOForGBuffers(pd3dCommandList, name);
	else SetPSOForSwapChain(pd3dCommandList, name);
}

void ShaderManager::SetPSOForSwapChain(ID3D12GraphicsCommandList * pd3dCommandList, const std::string& name) const
{
	pd3dCommandList->SetPipelineState(GetShader(name)->GetPSO());
}

void ShaderManager::SetPSOForGBuffers(ID3D12GraphicsCommandList * pd3dCommandList, const std::string& name) const
{
	pd3dCommandList->SetPipelineState(GetShader(name)->GetPSOForGBuffers());
}
 