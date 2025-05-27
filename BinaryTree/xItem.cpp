#include "xItem.h"


void xItem::on_update(float delat)
{
	m_animation.on_update(delat);
}

void xItem::on_render()
{
	m_animation.on_render();
}

void xItem::setPosition(SDL_FPoint& p)
{
	m_animation.setPos(p);
}

void WANGBA_Head::onEnable()
{
	DataManager::GetInstance().getPlayerState().status.phy_def += 5;
}

void WANGBA_Body::onEnable()
{
	DataManager::GetInstance().getPlayerState().status.phy_def += 10;
}

void WANGBA_Legs::onEnable()
{
	DataManager::GetInstance().getPlayerState().status.phy_def += 5;
}

void WANGBA_SPECIAL()
{
	PlayerStatus& playerStatus = DataManager::GetInstance().getPlayerState();
	playerStatus.pht_atk_ratio += 0.33;
	playerStatus.status.phy_atk = playerStatus.status.base_phy_atk * playerStatus.pht_atk_ratio;
}
