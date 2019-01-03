#pragma once

// �̱��� �������� ������ PSO Manager
// PSO�� map���� �����ϰ� �ִ�.
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
		// ��ȸ�ϸ� �޸� �Ҵ� ����.
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

