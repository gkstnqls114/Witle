#pragma once
#include "ComponentBase.h"

struct LIGHT
{
	XMFLOAT4				Ambient;
	XMFLOAT4				Diffuse;
	XMFLOAT4				Specular;
	XMFLOAT3				Position;
	float 					fFalloff;
	XMFLOAT3				Direction;
	float 					fTheta; //cos(m_fTheta)
	XMFLOAT3				Attenuation;
	float					fPhi; //cos(m_fPhi)
	bool					bEnable;
	int						nType;
	float					fRange;
	float					padding;
};

class Light
	: public ResourceComponentBase
{
private:
	struct VS_CB_LIGHTS_INFO
	{
		LIGHT					m_pLights[MAX_LIGHTS];
		XMFLOAT4				m_xmf4GlobalAmbient;
	};

	static ID3D12Resource		*m_pd3dcbLights;
	static VS_CB_LIGHTS_INFO	*m_pcbMappedLights;
	static UINT m_LightsCount;
	static VS_CB_LIGHTS_INFO*    m_pLights;

protected:
	UINT m_numLight{ 0 };
	LIGHT m_Light;

public:
	Light(GameObject* pOwner);
	virtual ~Light();

	static void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	static void UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList, int parameterIndex);
	static void ReleaseShaderVariables();

	virtual void Update(float fTimeElapsed) = 0;

};
