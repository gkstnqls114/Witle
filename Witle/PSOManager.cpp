#include "stdafx.h"
#include "PSOManager.h"

PSOManager* PSOManager::m_PSOManagerInstance{ nullptr };

PSOManager::PSOManager()
{
}


PSOManager::~PSOManager()
{
}

bool PSOManager::InsertPSO(const std::string& s, ID3D12PipelineState * pso)
{
	auto pair = m_PSOs.insert(std::pair<std::string, ID3D12PipelineState*>(s, pso));
	if (pair.second) {
		return true;
	}
	else {
		return false;
	}
}

ID3D12PipelineState * PSOManager::GetPSO(const std::string & s) const
{
	return (*(m_PSOs.find(s))).second;
}
 