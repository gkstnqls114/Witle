#pragma once

// �̱��� �������� ������ PSO Manager
// PSO�� map���� �����ϰ� �ִ�.
// std::pair<std::string, Shader *>
class Shader;

class ShaderManager
{
private:
	static ShaderManager* m_ShaderManagerInstance;
	ShaderManager();
	~ShaderManager();
	
	std::map<std::string, Shader*> m_Shaders;

public:
	static ShaderManager* GetInstance()
	{
		if (!m_ShaderManagerInstance) {
			m_ShaderManagerInstance = new ShaderManager;
		}
		return m_ShaderManagerInstance;
	}
	static void ReleaseInstance()
	{
		// ��ȸ�ϸ� �޸� �Ҵ� ����.
		for (auto& pso : m_ShaderManagerInstance->m_Shaders) {
			delete pso.second;
			pso.second = nullptr;
		}

		if (m_ShaderManagerInstance) {
			delete m_ShaderManagerInstance;
			m_ShaderManagerInstance = nullptr;
		}
	}

	bool InsertShader(const std::string& s, Shader* pso);
	Shader * GetShader(const std::string & s) const;
};

