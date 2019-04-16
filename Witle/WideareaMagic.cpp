#include "stdafx.h"
#include "WideareaMagic.h"


WideareaMagic::WideareaMagic()
{
}


WideareaMagic::~WideareaMagic()
{
}

void WideareaMagic::DoNotUse()
{
	m_isUsing = false;
}

void WideareaMagic::DoUse()
{
	m_isUsing = true;

}

void WideareaMagic::Render(ID3D12CommandList* commandList)
{

}
