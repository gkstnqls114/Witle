#include "stdafx.h"
#include "Shader.h"
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
 