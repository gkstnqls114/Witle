#include "stdafx.h"
#include "Shader.h"
#include "ShaderManager.h"

ShaderManager* ShaderManager::m_PSOManagerInstance{ nullptr };

ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
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
 