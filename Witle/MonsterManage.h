#pragma once

class  Dragon;

class MonsterManage
{
private:
	static Dragon* m_pMainDragon;

public:
	static Dragon* GetMainDragon() { return m_pMainDragon; }

	static bool IsNearPlayer(const XMFLOAT3 pos, float distance);
};