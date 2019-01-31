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
	
	ResourceBase* value = nullptr;
	switch (rootParameter.ParameterType)
	{
	case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS: // ��Ʈ ���
		value = new RootConstants(parametersIndex, rootParameter.Constants.Num32BitValues);
		break;

	case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE: // ������ ���̺�
		
		break;

	case D3D12_ROOT_PARAMETER_TYPE_CBV: // ��� ���� ��
		
		break;

	case D3D12_ROOT_PARAMETER_TYPE_SRV:

		break;

	case D3D12_ROOT_PARAMETER_TYPE_UAV:

		break;

	default:
		break;
	}

	// assert(!(value == nullptr));
	m_resourceMap.insert(std::pair<std::string, int>(name, m_VectorIndex));
	m_resourceVector[m_VectorIndex] = value;
	m_VectorIndex += 1;
}

void ParameterForm::UpdateShaderVariable(ID3D12GraphicsCommandList * commandList, ID3D12RootSignature * CurrentRootsignature, const std::string & name, const SourcePtr & resource)
{
#ifdef CHECK_ROOT_SIGNATURE
	assert(!(CurrentRootsignature != m_pRootSignature)); // ���� ���� �ʴٸ� ���â�� ����.
#endif // CHECK_ROOT_SIGNATURE

	// ������Ʈ�Ѵ�.
	GetResource(name)->UpdateShaderVariables(commandList, resource);
}

void ParameterForm::UpdateShaderVariable(ID3D12GraphicsCommandList * commandList, ID3D12RootSignature * CurrentRootsignature, UINT index, const SourcePtr & resource)
{
#ifdef CHECK_ROOT_SIGNATURE
	assert(!(CurrentRootsignature != m_pRootSignature)); // ���� ���� �ʴٸ� ���â�� ����.
#endif // CHECK_ROOT_SIGNATURE

	// ������Ʈ�Ѵ�.
	GetResource(index)->UpdateShaderVariables(commandList, resource);
}