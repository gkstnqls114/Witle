#include "stdafx.h"
#include "HealingEffect.h"
#include "Healing.h"


Healing::Healing(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_skillEffect = new HealingEffect("HealingEffect", pd3dDevice, pd3dCommandList);

}


Healing::~Healing()
{
}
