#pragma once

struct SEND_BUFFER
{
	XMFLOAT4X4 playerWorld;
	UCHAR keyBuffer[256];
};

class PointerForNetwork
{
	static SEND_BUFFER m_SendBuffer;

	static XMFLOAT4X4* m_pPlayerWorld;
	static UCHAR* m_pKeyBuffer;

public:
	static void SetpPlayerWorld(XMFLOAT4X4* world)
	{
		assert(!(m_pPlayerWorld != nullptr));
		m_pPlayerWorld = world;
	}
	static void SetpKeyBuffer(UCHAR* keybuffer)
	{
		assert(!(m_pKeyBuffer != nullptr));
		m_pKeyBuffer = keybuffer;
	}

	static XMFLOAT4X4 GetPlayerWorld() { m_SendBuffer.playerWorld; }

	static void Update()
	{
		assert(!(m_pPlayerWorld == nullptr));
		assert(!(m_pKeyBuffer == nullptr));

		m_SendBuffer.playerWorld = *m_pPlayerWorld;
		for (int i = 0; i < 256; ++i)
		{
			m_SendBuffer.keyBuffer[i] = m_pKeyBuffer[i];
		}
	}

	static char* GetSendBuffer() { return (char*)(&m_SendBuffer); }
	static UINT GetSendBufferSize() { return sizeof(SEND_BUFFER); }

};