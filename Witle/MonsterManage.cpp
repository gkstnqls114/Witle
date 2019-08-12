#include "stdafx.h"

#include "MonsterManage.h"
#include "Monster.h"

#include "Player.h"
#include "PlayerManager.h"

Dragon* MonsterManage::m_pMainDragon{ nullptr };

bool MonsterManage::IsNearPlayer(const XMFLOAT3 pos, float distance)
{
	// return Vector3::Length(Vector3::Subtract(pos, m_pMainDragon->GetTransform().GetPosition())) < distance;
	return 0;
}