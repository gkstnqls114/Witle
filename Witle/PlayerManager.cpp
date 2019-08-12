#include "stdafx.h"
#include "Player.h"
#include "PlayerManager.h"

Player* PlayerManager::m_pMainPlayer{ nullptr };

bool PlayerManager::IsNearPlayer(const XMFLOAT3 pos, float distance)
{
	return Vector3::Length(Vector3::Subtract(pos, m_pMainPlayer->GetTransform().GetPosition())) < distance;
}