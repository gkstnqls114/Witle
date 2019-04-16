#pragma once
#include "Skill.h"
class WideareaMagic :
	public Skill
{
public:
	WideareaMagic();
	virtual ~WideareaMagic();
	 
public:
	virtual void DoNotUse() override;
	virtual void DoUse() override;

	void Render(ID3D12CommandList* commandList);
};

