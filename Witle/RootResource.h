#pragma once

class ResourceBase;

// 루트 시그니처, 루트 패러미터를 통해 업데이트 하는 리소스 템플릿
class RootResource
{
	// 인덱스 값으로도 찾고, 인덱스로도 찾기 위해 두 개의 자료구조 사용
	std::map<std::string, int> m_resourceMap;
	std::vector<ResourceBase*> m_resourceVector;

public:
	// 루트 시그니처의 크기 만큼 리소스를 가진다.
	RootResource(int rootSignatureSize) : m_resourceVector(rootSignatureSize) {};
	~RootResource();

	void Release();

	// 해당 루트 패러미터에 맞춰서 Resource를 구성한다.
	void InsertResource(int parametersIndex, const D3D12_ROOT_PARAMETER& rootParameter);

	template<typename T>
	void UpdateShaderVariables(ID3D12CommandList* commandList, const T& resource)
	{
		//만약 타입 T의 사이즈가 update하는 리소스의 크기보다 클 경우 경고한다.
		
		// 업데이트한다.
	};
};