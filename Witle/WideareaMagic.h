#pragma once
#include "Skill.h"

class CylinderMesh;
class GameObject;

class WideareaMagic :
	public Skill
{
	GameObject* m_GameObject;
	CylinderMesh* m_CylinderMesh{ nullptr };

public:
	WideareaMagic(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~WideareaMagic();
	 
public:
	virtual void DoNotUse() override;
	virtual void DoUse() override;

	void Render(ID3D12GraphicsCommandList* commandList);
};

