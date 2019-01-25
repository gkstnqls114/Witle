#include "stdafx.h"
#include "32bitRootConstants.h"
#include "RootResource.h"

RootResource::~RootResource()
{
}

void RootResource::Release()
{
	for (int x = 0; x < m_resourceVector.size(); ++x)
	{
		// 릴리즈
		// 딜리트
		// 널포인터 설정
	}

	for (auto& p : m_resourceMap)
	{
		// 널 포인터 설정
	} 
}

void RootResource::InsertResource(int parametersIndex, const std::string& name, const D3D12_ROOT_PARAMETER & rootParameter)
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

	m_resourceMap.insert(std::pair<std::string, int>(name, m_VectorIndex));
	m_resourceVector[m_VectorIndex] = value;
	m_VectorIndex += 1;
}

void RootResource::UpdateShaderVariables(ID3D12CommandList * commandList, ID3D12RootSignature * CurrentRootsignature, const SourcePtr & resource)
{
#ifdef CHECK_ROOT_SIGNATURE
	assert(CurrentRootsignature != m_pRootSignature);
#endif // CHECK_ROOT_SIGNATURE

	//만약 타입 T의 사이즈가 update하는 리소스의 크기보다 클 경우 경고한다.

	// 업데이트한다.
}
