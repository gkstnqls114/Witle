#pragma once
#include "GameObject.h"

// CHeightMapTerrain �Դϴ�.
class Player :
	public GameObject
{
public:
	Player(const std::string& entityID);
	virtual ~Player();

	virtual void Update() override;

};