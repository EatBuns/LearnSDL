#include "Charactor.h"

AnimationState::AnimationState(const std::string& name, SDL_Renderer* renderer):m_animation(name)
{
	m_animation.setLoop(true);
	m_animation.setPlaySpeed(24);
	m_animation.setXName(name);
	m_animation.setRenderer(renderer);
	m_animation.setAnchor(Animation::AnimationAnchor::bottom);
}

AnimationState::AnimationState(const std::string& name, int idx, SDL_Renderer* renderer):m_animation(name,idx)
{
	m_animation.setLoop(true);
	m_animation.setPlaySpeed(24);
	m_animation.setXName(name);
	m_animation.setRenderer(renderer);
	m_animation.setAnchor(Animation::AnimationAnchor::bottom);
}

AnimationState::AnimationState(SDL_Renderer* renderer)
{
	m_animation.setLoop(true);
	m_animation.setPlaySpeed(24);
	m_animation.setRenderer(renderer);
	m_animation.setAnchor(Animation::AnimationAnchor::bottom);
}

AnimationState::~AnimationState()
{
}

void AnimationState::on_update(float delta)
{
	m_animation.on_update(delta);
}

void AnimationState::on_renderer()
{
	m_animation.on_render();
}

void AnimationState::setPos(SDL_FPoint& pos)
{
	m_animation.setPos(pos);
}

void Charactor::setPosition(const SDL_FPoint& p)
{
	Position = p;
}

void Charactor::setCollisionBox(CollisionBox* box)
{
	m_box = box;
	float x, y;

	switch (m_anchor)
	{
	case Animation::AnimationAnchor::leftup:
		break;
	case Animation::AnimationAnchor::up:
		break;
	case Animation::AnimationAnchor::rightup:
		break;
	case Animation::AnimationAnchor::leftcenter:
		break;
	case Animation::AnimationAnchor::center:
		break;
	case Animation::AnimationAnchor::rightcenter:
		break;
	case Animation::AnimationAnchor::leftbottom:
		break;
	case Animation::AnimationAnchor::bottom:
		y = Position.y - Actual_h;
		x = Position.x - Actual_w / 2;
		m_box->setPosition(x, y);
		m_box->setSize(Actual_w, Actual_h);
		break;
	case Animation::AnimationAnchor::rightbottom:
		break;
	default:
		break;
	}
}

buffManager::buffManager()
{
	m_buffs[(int)buff::buffType::SPEED] = std::make_shared<speedBuff>();
	m_buffs[(int)buff::buffType::PHY_ATTACK] = std::make_shared<speedBuff>();
	m_buffs[(int)buff::buffType::PHY_DEFENCE] = std::make_shared<speedBuff>();

	m_iter = m_buffs.begin();
	m_end = m_buffs.end();
}

buffManager::~buffManager()
{
}  

void buff::on_update(float delta)
{
	buffTimer.on_update(delta);
}
