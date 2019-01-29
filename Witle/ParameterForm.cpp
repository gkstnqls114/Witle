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

		// 릴리즈
		m_resourceVector[x]->ReleaseShaderVariables();
		// 딜리트
		delete m_resourceVector[x];
		// 널포인터 설정
		m_resourceVector[x] = nullptr;
	}
}

void ParameterForm::InsertResource(int parametersIndex, const std::string& name, const D3D12_ROOT_PARAMETER & rootParameter)
{
	assert(!(parametersIndex >= m_resourceVector.size())); // 만약 인덱스가 벡터의 크기보다 크다면 오류를 발생한다.
	
	ResourceBase* value = nullptr;
	switch (rootParameter.ParameterType)
	{
	case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS: // 루트 상수
		value = new RootConstants(parametersIndex, rootParameter.Constants.Num32BitValues);
		break;

	case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE: // 서술자 테이블

		break;

	case D3D12_ROOT_PARAMETER_TYPE_CBV: // 상수 버퍼 뷰

		break;

	case D3D12_ROOT_PARAMETER_TYPE_SRV:

		break;

	case D3D12_ROOT_PARAMETER_TYPE_UAV:

		break;

	default:
		break;
	}

	assert(!(value == nullptr));
	m_resourceMap.insert(std::pair<std::string, int>(name, m_VectorIndex));
	m_resourceVector[m_VectorIndex] = value;
	m_VectorIndex += 1;
}

void ParameterForm::UpdateShaderVariable(ID3D12GraphicsCommandList * commandList, ID3D12RootSignature * CurrentRootsignature, const std::string & name, const SourcePtr & resource)
{
#ifdef CHECK_ROOT_SIGNATURE
	assert(!(CurrentRootsignature != m_pRootSignature)); // 만약 같지 않다면 경고창을 띄운다.
#endif // CHECK_ROOT_SIGNATURE

	// 업데이트한다.
	GetResource(name)->UpdateShaderVariables(commandList, resource);
}

void ParameterForm::UpdateShaderVariable(ID3D12GraphicsCommandList * commandList, ID3D12RootSignature * CurrentRootsignature, UINT index, const SourcePtr & resource)
{
#ifdef CHECK_ROOT_SIGNATURE
	assert(!(CurrentRootsignature != m_pRootSignature)); // 만약 같지 않다면 경고창을 띄운다.
#endif // CHECK_ROOT_SIGNATURE

	// 업데이트한다.
	GetResource(index)->UpdateShaderVariables(commandList, resource);
}
