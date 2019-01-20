#include "stdafx.h"
#include "32bitRootConstants.h"


RootConstants::RootConstants(UINT paraIndex, UINT resourceCount, UINT StartOffset)
{
	m_parameterIndex = paraIndex;
	m_ResourceCount = resourceCount;

	DestOffsets = new UINT[m_ResourceCount];
	DestOffsets[0] = StartOffset;
	for (int x = 1; x < m_ResourceCount; ++x)
	{
		DestOffsets[x] = StartOffset + (sizeof(XMFLOAT4X4) / 4);
		std::cout << sizeof(XMFLOAT4X4);
	}
}

RootConstants::~RootConstants()
{
	if (DestOffsets)
	{
		delete[] DestOffsets;
		DestOffsets = nullptr;
	}
}

void RootConstants::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT4X4 *resources)
{
	for (int x = 0; x < m_ResourceCount; ++x)
	{
		pd3dCommandList->SetGraphicsRoot32BitConstants(m_parameterIndex, 16, &(resources[x]), DestOffsets[x]);
	}
}
