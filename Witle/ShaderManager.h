#pragma once

// 싱글톤 패턴으로 구현한 PSO Manager
// PSO를 map으로 관리하고 있다.
// std::pair<std::string, Shader *>
class Shader;

class ShaderManager
{
private:
	static ShaderManager* m_PSOManagerInstance;
	ShaderManager();
	~ShaderManager();
	
	std::map<std::string, Shader*> m_Shaders;

public:
	static ShaderManager* GetInstance()
	{
		if (!m_PSOManagerInstance) {
			m_PSOManagerInstance = new ShaderManager;
		}
		return m_PSOManagerInstance;
	}
	static void ReleaseInstance()
	{
		// 순회하며 메모리 할당 제거.
		for (auto& pso : m_PSOManagerInstance->m_Shaders) {
			delete pso.second;
			pso.second = nullptr;
		}

		if (m_PSOManagerInstance) {
			delete m_PSOManagerInstance;
			m_PSOManagerInstance = nullptr;
		}
	}

	bool InsertShader(const std::string& s, Shader* pso);
	Shader * GetShader(const std::string & s) const;
};

