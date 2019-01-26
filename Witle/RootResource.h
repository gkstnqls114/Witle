#pragma once

class ResourceBase;

// 루트 시그니처, 루트 패러미터를 통해 업데이트 해야할 리소스들
// 
class RootResource
{ 
#ifdef CHECK_ROOT_SIGNATURE
	ID3D12RootSignature* m_pRootSignature{ nullptr }; // 현재 사용하는 루트 시그니처와 맞는지 체크하기 위해 사용
#endif // CHECK_ROOT_SIGNATURE

	int m_VectorIndex{ 0 };
	// 인덱스 값으로도 찾고, 인덱스로도 찾기 위해 두 개의 자료구조 사용
	std::map<std::string, int> m_resourceMap;
	std::vector<ResourceBase*> m_resourceVector;

private:

public:
	// 루트 시그니처의 크기 만큼 리소스를 가진다.
	RootResource(int rootSignatureSize) :m_resourceVector(rootSignatureSize, nullptr) {};
	~RootResource();

	void ReleaseObjects();
	void SetRootSignature(ID3D12RootSignature* rootsignature)
	{
		assert(!(m_pRootSignature != nullptr)); // 한번만 설정할 수 있습니다.

		m_pRootSignature = rootsignature;
	}
	 
	ResourceBase* GetResource(int index) const { return m_resourceVector[index]; }
	ResourceBase* GetResource(const std::string& name) const
	{
		assert(m_resourceMap.find(name) != m_resourceMap.end());
		return m_resourceVector[(*m_resourceMap.find(name)).second];
	}

	// 해당 루트 패러미터에 맞춰서 Resource를 구성한다.
	// int parametersInde
	void InsertResource(int parametersIndex, const std::string& name, const D3D12_ROOT_PARAMETER& rootParameter);

	void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* CurrentRootsignature, const std::string& name, const SourcePtr& resource);
	void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* CurrentRootsignature, UINT index, const SourcePtr& resource);
};