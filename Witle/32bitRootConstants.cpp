#include "stdafx.h"
#include "32bitRootConstants.h"

 

RootConstants::RootConstants(UINT paraIndex, UINT elementSize)
{
	m_parameterIndex = paraIndex; 
	m_4ByteSize = elementSize;
}

RootConstants::~RootConstants()
{

}
void RootConstants::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList, const SourcePtr & source)
{
	assert(!(m_4ByteSize != (source.m_byteSize / NUM32BITTOBYTE))); // 크기가 맞지 않는다면 경고한다.
	pd3dCommandList->SetGraphicsRoot32BitConstants(m_parameterIndex, m_4ByteSize, source.m_Ptr, 0);
}
