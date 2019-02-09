#include "stdafx.h"
#include "32bitRootConstants.h"
#include "ParameterForm.h"

ParameterForm::~ParameterForm()
{
}

void ParameterForm::ReleaseObjects()
{
	for (int x = 0; x < m_resourceVector.size(); ++x)
	{
		if (!m_resourceVector[x]) continue;

		// ������
		m_resourceVector[x]->ReleaseShaderVariables();
		// ����Ʈ
		delete m_resourceVector[x];
		// �������� ����
		m_resourceVector[x] = nullptr;
	}
}

void ParameterForm::InsertResource(int parametersIndex, const std::string& name, const D3D12_ROOT_PARAMETER & rootParameter)
{
	assert(!(parametersIndex >= m_resourceVector.size())); // ���� �ε����� ������ ũ�⺸�� ũ�ٸ� ������ �߻��Ѵ�.
	
	Form* value = nullptr;
	switch (rootParameter.ParameterType)
	{
	case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS: // ��Ʈ ���
		value = new FormConstant(rootParameter.Constants.Num32BitValues);
		break;

	case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE: // ������ ���̺�
		break;

	case D3D12_ROOT_PARAMETER_TYPE_CBV: // ��� ���� ��
		value = new FormCBV();

		break;

	case D3D12_ROOT_PARAMETER_TYPE_SRV:

		break;

	case D3D12_ROOT_PARAMETER_TYPE_UAV:

		break;

	default:
		break;
	}

	// assert(!(value == nullptr));
	m_resourceMap.insert(std::pair<std::string, int>(name, parametersIndex));
	m_resourceVector[parametersIndex] = value; 
}

void ParameterForm::UpdateShaderVariable(ID3D12GraphicsCommandList * commandList, ID3D12RootSignature * CurrentRootsignature, const std::string & name, const SourcePtr & resource)
{
#ifdef CHECK_ROOT_SIGNATURE
	assert(!(CurrentRootsignature != m_pRootSignature)); // ���� ���� �ʴٸ� ���â�� ����.
#endif // CHECK_ROOT_SIGNATURE

	// ������Ʈ�Ѵ�.
	int index = 0;
	GetResource(name, index)->UpdateShaderVariable(commandList, index, resource);
}

void ParameterForm::UpdateShaderVariable(ID3D12GraphicsCommandList * commandList, ID3D12RootSignature * CurrentRootsignature, UINT index, const SourcePtr & resource)
{
#ifdef CHECK_ROOT_SIGNATURE
	assert(!(CurrentRootsignature != m_pRootSignature)); // ���� ���� �ʴٸ� ���â�� ����.
#endif // CHECK_ROOT_SIGNATURE

	// ������Ʈ�Ѵ�.
	GetResource(index)->UpdateShaderVariable(commandList, index, resource);
}

void ParameterForm::FormCBV::UpdateShaderVariable(ID3D12GraphicsCommandList * commandList, UINT index, const SourcePtr & resource)
{
	//XMFLOAT4X4 xmf4x4View;
	//XMStoreFloat4x4(&xmf4x4View, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4View)));
	//::memcpy(&m_MappedResource->m_xmf4x4View, &xmf4x4View, sizeof(XMFLOAT4X4));

	//XMFLOAT4X4 xmf4x4Projection;
	//XMStoreFloat4x4(&xmf4x4Projection, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4Projection)));
	//::memcpy(&m_MappedResource->m_xmf4x4Projection, &xmf4x4Projection, sizeof(XMFLOAT4X4));

	//::memcpy(&m_pcbMappedCamera->m_xmf3Position, &m_xmf3Position, sizeof(XMFLOAT3));

	//D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pResource->GetGPUVirtualAddress();
	//pd3dCommandList->SetGraphicsRootConstantBufferView(index, d3dGpuVirtualAddress);
}

void ParameterForm::FormCBV::ReleaseShaderVariables()
{
}

void ParameterForm::FormConstant::UpdateShaderVariable(ID3D12GraphicsCommandList * commandList, UINT index, const SourcePtr & resource)
{
	assert(!(m_4ByteSize != (resource.m_byteSize / NUM32BITTOBYTE))); // ũ�Ⱑ ���� �ʴ´ٸ� ����Ѵ�.
	commandList->SetGraphicsRoot32BitConstants(index, m_4ByteSize, resource.m_Ptr, 0);
}
