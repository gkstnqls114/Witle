#pragma once

// 싱글톤 패턴으로 구현한 PSO Manager
// PSO를 map으로 관리하고 있다.
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
		// 순회하며 메모리 할당 제거.
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

