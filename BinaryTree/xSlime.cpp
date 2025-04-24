#include "xSlime.h"

xSlime *SlimeState::m_slime = nullptr;

xSlime::xSlime(SDL_Renderer* renderer, Animation::AnimationAnchor anch):Charactor("slime",anch)
{
	auto idleAnima = std::make_shared<AnimationState>("KingSlimeRed", 0, renderer);
	idleAnima->setAnchor(anch);
	auto JumpAnima = std::make_shared<AnimationState>("KingSlimeRed", renderer);
	JumpAnima->setAnchor(anch);

	SlimeState::setPlayer(this);
	auto idle = std::make_shared<SlimeIdleState>(std::move(idleAnima));
	auto jump = std::make_shared<SlimeJumpState>(std::move(JumpAnima));
	m_machine.registerState("Idle", std::move(idle));
	m_machine.registerState("Jump", std::move(jump));

	m_machine.setCurrentState("Idle");

	//¿ªÆôÊäÈë
	setInput(false);
	vx = 100;
}

void xSlime::on_update(float delat)
{
	Charactor::on_update(delat);

	v_vx = direction * vx * (delat / 1000);
	Position.x += v_vx;
	m_box->setVx(v_vx);
	if (m_box->isGravityEnable() && !isOnFlooor)
	{
		Position.y += m_box->getNextFrameVy();
	}

	m_machine.on_update(delat);
}

void xSlime::on_render()
{
	m_machine.on_render();
}

void xSlime::on_CollisionCb(int layer, int collisionSide, SDL_FRect rect)
{
	//collisionSide
	//1:Åö×²µ½SrcLayerµÄ×ó±ß
	//2:Åö×²µ½SrcLayerµÄÓÒ±ß
	switch (layer)
	{
	case 0:
		break;
	case 1:
		switch (collisionSide)
		{
		case 1:
			Position.x = rect.x + rect.w + Actual_w/2;
			m_box->setPosition(rect.x + rect.w, m_box->getPos_y());
			direction = 1;
			break;
		case 2:
			Position.x = rect.x - Actual_w/2;
			m_box->setPosition(rect.x - Actual_w, m_box->getPos_y());
			direction = -1;
			break;
		case 3:


			break;
		case 4:
			Position.y = rect.y;
			m_box->setPosition(m_box->getPos_x(), rect.y - Actual_h+1);
			isOnFlooor = true;
			break;
		default:
			break;
		}
		break;
	case 2:

		break;
	default:
		break;
	}
}

void SlimeState::on_update(float delat)
{
	auto pp = m_slime->getPosition();
	m_AnimationState->setPos(pp);
	m_AnimationState->on_update(delat);
}

void SlimeState::on_renderer()
{
	m_AnimationState->on_renderer();
}

void SlimeState::on_enter()
{
	auto pp = m_slime->getPosition();
	m_AnimationState->setPos(pp);
}

void SlimeState::on_exit()
{
}

SlimeJumpState::SlimeJumpState(std::shared_ptr<AnimationState>&& state) :SlimeState(std::move(state))
{
	m_AnimationState->setLoop(false);
	m_AnimationState->setScalex(0.5f);
	m_AnimationState->setScaley(0.5f);
	auto func = std::function<void()>([&]() { SlimeState::m_slime->switchState("Idle"); });
	m_AnimationState->setcallback(func);
}

void SlimeJumpState::on_update(float delat)
{
	SlimeState::on_update(delat);
}

void SlimeJumpState::on_enter()
{
	SlimeState::on_enter();
	m_AnimationState->reset();
	m_slime->getCollisionBox()->setVy(-3);
	m_slime->setVx(50);
	m_slime->setOnfloor(false);
}

void SlimeJumpState::on_exit()
{
	SlimeState::on_exit();
}

SlimeIdleState::SlimeIdleState(std::shared_ptr<AnimationState>&& state):SlimeState(std::move(state))
{
	m_AnimationState->setScalex(0.5f);
	m_AnimationState->setScaley(0.5f);
}

void SlimeIdleState::on_update(float delat)
{
	SlimeState::on_update(delat);
	if (m_slime->getIsOnFloor())
		timer.start();
	timer.on_update(delat);
}

void SlimeIdleState::on_enter()
{
	SlimeState::on_enter();

	m_slime->setVx(0);

	timer.setInterval(100);
	timer.setSingleShot(true);
	timer.setCallBack([](int i) {
		SDL_Log("use %d\n",i);
		m_slime->switchState("Jump");
	});
}

void SlimeIdleState::on_exit()
{
}
