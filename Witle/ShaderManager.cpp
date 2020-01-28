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
#include "UIMapForPlayerShader.h"
#include "UIScreenShader.h"
#include "UIWorldShader.h"
#include "DefferedShader.h"
#include "InstancingStandardShaderForShadow.h"
#include "TerrainForShadow.h"
#include "SkillIconShader.h"
#include "BlendMeshShader.h" 
#include "ToneCuveAndBloomShader.h"
#include "HitEffectShader.h"
#include "BloomShader.h"
#include "ToneCuveShader.h"
#include "AltarSphereShader.h"
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
	m_ShaderManagerInstance->ReleaseObjects(); 

	delete m_ShaderManagerInstance;
	m_ShaderManagerInstance = nullptr;

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
	InsertShader(SHADER_CUBE, pCubeShader);

	Shader* pTerrainShader = new TerrainShader();
	pTerrainShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader(SHADER_TERRAIN, pTerrainShader);

	Shader* pStandardShader = new StandardShader();
	pStandardShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader("StandardShader", pStandardShader);

	Shader* pInstancingStandardShader = new InstancingStandardShader();
	pInstancingStandardShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader(SHADER_INSTANCING_STANDARD, pInstancingStandardShader);

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

	Shader* pUIMapForPlayerShader = new UIMapForPlayerShader();
	pUIMapForPlayerShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader(SHADER_UIMAPFORPLAYER, pUIMapForPlayerShader);

	Shader* pDefferedShader = new DefferedShader();
	pDefferedShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader(SHADER_DEFFREDRENDER, pDefferedShader);

	Shader* pInstancingStandardShaderForShadow = new InstancingStandardShaderForShadow();
	pInstancingStandardShaderForShadow->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader(SHADER_INSTACINGSTANDARDFORSHADOW, pInstancingStandardShaderForShadow);

	TerrainForShadow* pTerrainForShadow = new TerrainForShadow();
	pTerrainForShadow->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader(SHADER_TERRAIN_FORSHADOW, pTerrainForShadow);
	 
	SkillIconShader* pSkillIconShader = new SkillIconShader();
	pSkillIconShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader(SHADER_SKILLICON, pSkillIconShader);

	BlendMeshShader* pBlendMeshShader = new BlendMeshShader();
	pBlendMeshShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader(SHADER_BLENDMESH, pBlendMeshShader);

	ToneCuveAndBloomShader* pTonemappingShader = new ToneCuveAndBloomShader();
	pTonemappingShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader(SHADER_TONECURVEANDBLOOM, pTonemappingShader);

	ToneCuveShader* pToneCuveShader = new ToneCuveShader();
	pToneCuveShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader(SHADER_TONECURVE, pToneCuveShader);

	BloomShader* pBloomShader = new BloomShader();
	pBloomShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader(SHADER_BLOOM, pBloomShader);

	HitEffectShader* pHitEffectShader = new HitEffectShader();
	pHitEffectShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader(SHADER_HITEFFECT, pHitEffectShader);
	 
	AltarSphereShader* pAltarSphereShader = new AltarSphereShader();
	pAltarSphereShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	InsertShader(SHADER_ALTARSPHERE, pAltarSphereShader);
}

bool ShaderManager::InsertShader(std::string&& s, Shader * pso)
{
	auto pair = m_Shaders.insert(std::pair<std::string, Shader*>(s, pso));
	if (pair.second) {
		return true;
	}
	else {
		return false;
	}
}

bool ShaderManager::InsertShader(const std::string& s, Shader* pso)
{ 
	auto pair = m_Shaders.insert(std::pair<std::string, Shader*>(s, pso));
	if (pair.second) {
		return true;
	}
	else {
		return false;
	}
}

Shader * ShaderManager::GetShader(const std::string& s) const
{
	return (*(m_Shaders.find(s))).second;
}

Shader * ShaderManager::GetShader(std::string && s) const
{
	return (*(m_Shaders.find(s))).second;
}

void ShaderManager::SetPSO(ID3D12GraphicsCommandList * pd3dCommandList, std::string&& name, bool isGBuffers) const
{
	if (isGBuffers) SetPSOForGBuffers(pd3dCommandList, name);
	else SetPSOForSwapChain(pd3dCommandList, name);
}

void ShaderManager::SetPSO(ID3D12GraphicsCommandList* pd3dCommandList, const std::string& name, bool isGBuffers) const
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
 