#include "stdafx.h" 
#include "Scene.h"

#define _WITH_PLAYER_TOP

CScene::CScene()
{

}

CScene::~CScene()
{
}

void CScene::UpdateGamelogic(float ElapsedTime)
{
	Update(ElapsedTime);
	LastUpdate();
}
