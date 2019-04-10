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
	static ShaderManager* GetInstance();
	static void ReleaseInstance();

	bool InsertShader(const std::string& s, Shader* pso);
	Shader * GetShader(const std::string & s) const;

	void SetPSO(ID3D12GraphicsCommandList * pd3dCommandList, const std::string name) const;
};

