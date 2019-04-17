#pragma once

class Texture; 
class TextureStorage
{
	static TextureStorage* m_Instance;
	 
	// name°ú texture
	std::map<std::string, Texture*> m_TextureStorage;
	bool m_isCreate{ false };

private:
	TextureStorage();
	~TextureStorage();

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
	void InsertTexture(const std::string& name, Texture* texture); 
	const Texture* GetTexture(const std::string& name);
};