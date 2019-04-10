#include "stdafx.h"
#include "d3dUtil.h"
#include "Light.h"

ID3D12Resource*                 Light::m_pd3dcbLights = nullptr;
Light::VS_CB_LIGHTS_INFO*		Light::m_pcbMappedLights = nullptr;
UINT							Light::m_LightsCount = 0;
Light::VS_CB_LIGHTS_INFO*    Light::m_pLights = nullptr;

Light::Light(GameObject * pOwner) 
	: ComponentBase(pOwner)
{
	assert(!(m_LightsCount > MAX_LIGHTS));
	m_FamilyID.InitLight();

	m_numLight = m_LightsCount;
	m_LightsCount += 1;
}

Light::~Light()
{

}

void Light::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_pLights = new VS_CB_LIGHTS_INFO;

	UINT ncbElementBytes = ((sizeof(VS_CB_LIGHTS_INFO) + 255) & ~255); //256ÀÇ ¹è¼ö
	m_pd3dcbLights = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbLights->Map(0, NULL, (void **)&m_pcbMappedLights);
}

void Light::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList, int parameterIndex)
{
	::memcpy(m_pcbMappedLights, m_pLights, sizeof(LIGHT));

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(parameterIndex, d3dcbLightsGpuVirtualAddress); //Lights
}

void Light::ReleaseShaderVariables()
{
	if (m_pcbMappedLights)
	{
		m_pcbMappedLights;
	}
	if (m_pd3dcbLights)
	{
		m_pd3dcbLights;
	}
}
