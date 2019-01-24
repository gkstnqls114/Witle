#include "stdafx.h"
#include "32bitRootConstants.h"

 

RootConstants::RootConstants(UINT paraIndex, UINT elementSize)
{
	m_parameterIndex = paraIndex;
	m_DataSize = elementSize; 
}

RootConstants::~RootConstants()
{

}

void RootConstants::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
	pd3dCommandList->SetGraphicsRoot32BitConstants(m_parameterIndex, (m_DataSize / NUM32BITTOBYTE), m_pData, m_DataSize);
}
