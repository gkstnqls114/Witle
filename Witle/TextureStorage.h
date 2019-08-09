#pragma once

class Texture; 
class MyDescriptorHeap;

class TextureStorage
{
	static TextureStorage* m_Instance;
	 
	// name°ú texture
	std::map<std::string, Texture*> m_TextureStorage;
	std::list<std::string> m_expection; 

	bool m_isCreate{ false };

private:
	TextureStorage();
	~TextureStorage();

	void CreateModelTexture(const std::string& name, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	void CreateImageTexture(const std::string& name, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
public:
	static TextureStorage* GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new TextureStorage();
		}
		return m_Instance;
	}
	static void ReleaseInstance()
	{
		delete m_Instance;
		m_Instance = nullptr;
	}


	void ReleaseUploadBuffers();
	void ReleaseObjects();
	
	void CreateTextures(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	Texture* const GetTexture(const std::string& name);
};