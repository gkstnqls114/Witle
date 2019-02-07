#pragma once

class ResourceBase;

// 루트 시그니처, 루트 패러미터를 통해 업데이트 해야할 리소스의 틀
// 
class ParameterForm
{ 
	class Form
	{
	public:
		virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, UINT index, const SourcePtr& resource) = 0;
		virtual void ReleaseShaderVariables() = 0;
	};

	class FormCBV : public Form
	{
	public:
		virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, UINT index, const SourcePtr& resource) override;
		virtual void ReleaseShaderVariables();
	};

	class FormConstant : public Form
	{
		UINT m_4ByteSize{ 0 };
	public:
		FormConstant(int num4byte) : m_4ByteSize(num4byte) {};
		virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, UINT index, const SourcePtr& resource) override;
		virtual void ReleaseShaderVariables() {};
	};

#ifdef CHECK_ROOT_SIGNATURE
	ID3D12RootSignature* m_pRootSignature{ nullptr }; // 현재 사용하는 루트 시그니처와 맞는지 체크하기 위해 사용
#endif // CHECK_ROOT_SIGNATURE

	// 인덱스 값으로도 찾고, 인덱스로도 찾기 위해 두 개의 자료구조 사용
	std::map<std::string, int> m_resourceMap;
	std::vector<Form*> m_resourceVector;

private:

public:
	// 루트 시그니처의 크기 만큼 리소스를 가진다.
	ParameterForm(int rootSignatureSize) :m_resourceVector(rootSignatureSize, nullptr) {};
	~ParameterForm();

	void ReleaseObjects();
	void SetRootSignature(ID3D12RootSignature* rootsignature)
	{
		assert(!(m_pRootSignature != nullptr)); // 한번만 설정할 수 있습니다.

		m_pRootSignature = rootsignature;
	}
	 
	Form* GetResource(int index) const { return m_resourceVector[index]; }
	Form* GetResource(const std::string& name, int& index) const
	{
		assert(m_resourceMap.find(name) != m_resourceMap.end());
		index = (*m_resourceMap.find(name)).second;
		return m_resourceVector[index];
	}
	int GetIndex(const std::string& name) const
	{ 
		assert(m_resourceMap.find(name) != m_resourceMap.end());
		return (*m_resourceMap.find(name)).second;
	}

	// 해당 루트 패러미터에 맞춰서 Resource를 구성한다.
	// int parametersInde
	void InsertResource(int parametersIndex, const std::string& name, const D3D12_ROOT_PARAMETER& rootParameter);

	void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* CurrentRootsignature, const std::string& name, const SourcePtr& resource);
	void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* CurrentRootsignature, UINT index, const SourcePtr& resource);
};