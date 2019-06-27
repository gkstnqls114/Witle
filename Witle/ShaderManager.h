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

private:
	void SetPSOForSwapChain(ID3D12GraphicsCommandList * pd3dCommandList, const std::string& name) const;
	void SetPSOForGBuffers(ID3D12GraphicsCommandList * pd3dCommandList, const std::string& name) const;

public:
	static ShaderManager* GetInstance();
	static void ReleaseInstance();

	void ReleaseObjects();
	void BuildShaders(ID3D12Device * pd3dDevice, ID3D12RootSignature * const pd3dGraphicsRootSignature);
	bool InsertShader(const std::string& s, Shader* pso);
	Shader * GetShader(const std::string & s) const;

	void SetPSO(ID3D12GraphicsCommandList * pd3dCommandList, const std::string& name, bool isGBuffers) const;

};

