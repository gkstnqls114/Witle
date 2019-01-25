#include "stdafx.h"
#include "32bitRootConstants.h"

 

RootConstants::RootConstants(UINT paraIndex, UINT elementSize)
{
	m_parameterIndex = paraIndex; 
}

RootConstants::~RootConstants()
{

}
void RootConstants::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList, const SourcePtr & source)
{
	pd3dCommandList->SetGraphicsRoot32BitConstants(m_parameterIndex, (source.m_byteSize / NUM32BITTOBYTE), source.m_Ptr, 0);
}
