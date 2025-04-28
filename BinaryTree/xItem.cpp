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
	DataManager::GetInstance().getPlayerState().status.phy_atk *= 1.33;
}
