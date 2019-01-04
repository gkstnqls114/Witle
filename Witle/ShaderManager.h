#pragma once

// 싱글톤 패턴으로 구현한 PSO Manager
// PSO를 map으로 관리하고 있다.
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
	static ShaderManager* GetInstance();
	static void ReleaseInstance();

	bool InsertShader(const std::string& s, Shader* pso);
	Shader * GetShader(const std::string & s) const;
};

