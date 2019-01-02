#pragma once

// �̱��� �������� ������ PSO Manager
// PSO�� map���� �����ϰ� �ִ�.
// std::pair<std::string, PSO> 
class PSOManager
{
private:
	static PSOManager* m_PSOManagerInstance;
	PSOManager();
	~PSOManager();
	
	std::map<std::string, ID3D12PipelineState*> m_PSOs;

public:
	static PSOManager* GetInstance()
	{
		if (!m_PSOManagerInstance) {
			m_PSOManagerInstance = new PSOManager;
		}
		return m_PSOManagerInstance;
	}
	static void ReleaseInstance()
	{
		// ��ȸ�ϸ� �޸� �Ҵ� ����.
		for (auto& pso : m_PSOManagerInstance->m_PSOs) {
			delete pso.second;
			pso.second = nullptr;
		}

		if (m_PSOManagerInstance) {
			delete m_PSOManagerInstance;
			m_PSOManagerInstance = nullptr;
		}
	}

	bool InsertPSO(const std::string& s, ID3D12PipelineState* pso);
	ID3D12PipelineState* GetPSO(const std::string&) const;
};

