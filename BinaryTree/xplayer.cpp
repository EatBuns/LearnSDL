#include "xplayer.h"
#include "CollisionManager.h"

xplayer* playerState::m_player = nullptr;

xplayer::xplayer(SDL_Renderer* renderer, Animation::AnimationAnchor anch, float vx, NodeStatus& s):Charactor("xplayer",anch,vx,s)
{
	auto idleAnima = std::make_shared<AnimationState>("Role_Idle", renderer);
	idleAnima->setAnchor(anch);
	auto runAnima = std::make_shared<AnimationState>("Role_Run", renderer);
	runAnima->setAnchor(anch);
	auto jumpAnima = std::make_shared<AnimationState>(renderer);
	jumpAnima->addFrame(DataManager::GetInstance().findAtlasIndex("Role_Jump", 0));
	jumpAnima->addFrame(DataManager::GetInstance().findAtlasIndex("Role_Jump", 1));
	jumpAnima->addFrame(DataManager::GetInstance().findAtlasIndex("Role_Jump", 2));
	jumpAnima->addFrame(DataManager::GetInstance().findAtlasIndex("Role_Jump", 3));
	jumpAnima->addFrame(DataManager::GetInstance().findAtlasIndex("Role_Jump", 4));
	jumpAnima->addFrame(DataManager::GetInstance().findAtlasIndex("Role_Jump", 5));
	auto fallAnima = std::make_shared<AnimationState>(renderer);
	fallAnima->addFrame(DataManager::GetInstance().findAtlasIndex("Role_Jump", 6));
	fallAnima->addFrame(DataManager::GetInstance().findAtlasIndex("Role_Jump", 7));
	auto attackAnima = std::make_shared<AnimationState>("Role_Attack", renderer);
	attackAnima->setAnchor(anch);

	playerState::setPlayer(this);
	auto idle = std::make_shared<IdleState>(std::move(idleAnima));
	auto run = std::make_shared<RunState>(std::move(runAnima));
	auto jump = std::make_shared<JumpState>(std::move(jumpAnima));
	auto fall = std::make_shared<FallState>(std::move(fallAnima));
	auto attack = std::make_shared<AttackState>(std::move(attackAnima));
	m_machine.registerState("Idle", std::move(idle));
	m_machine.registerState("Run", std::move(run));
	m_machine.registerState("Jump", std::move(jump));
	m_machine.registerState("Fall", std::move(fall));
	m_machine.registerState("Attack", std::move(attack));

	m_machine.setCurrentState("Fall");

	//开启输入
	setInput(true);
	ic.changeKeyMap("LEFT", SDL_KeyCode::SDLK_a);
	ic.changeKeyMap("RIGHT", SDL_KeyCode::SDLK_d);
	//ic.changeInputMode(inputControl::InputMode::gamecontrol);

	m_buffManager.setEnterCB(0, [&]() {
		m_box->setDstLayer(CollisionBox::CollissionLayer::layer3, false);
	});
	m_buffManager.setExitCB(0, [&]() {
		m_box->setDstLayer(CollisionBox::CollissionLayer::layer3, true);
	});
}

void xplayer::on_input(SDL_Event& e)
{
	ic.InputEvent(e);
}

void xplayer::on_update(float delat)
{
	Charactor::on_update(delat);
	int Axis = (int)(ic.isRight() - ic.isLeft());
	v_vx = Axis * up_vx * (delat / 1000);
	if (Axis == 1) isfaceright = true;
	else if (Axis == -1) isfaceright = false;

	m_box->setVx(v_vx);
	Position.x += v_vx;

	if (m_box->isGravityEnable() && !isOnFlooor)
	{
		Position.y += m_box->getNextFrameVy();
	}
		
	m_machine.on_update(delat);
}

void xplayer::on_render()
{
	Charactor::on_render();
}

void xplayer::on_CollisionCb(int layer,int collisionSide, SDL_FRect rect, std::string& name)
{
	/*COLLIDE_LEFT,     ===> 1
    COLLIDE_RIGHT,		===> 2
    COLLIDE_TOP,		===> 3
    COLLIDE_BOTTOM		===> 4 */
	switch (layer)
	{
	case 1:
		switch (collisionSide)
		{
		case 1:
			Position.x = rect.x + rect.w + Actual_w/2;
			m_box->setPosition(rect.x + rect.w, m_box->getPos_y());
			break;
		case 2:
			Position.x = rect.x - Actual_w/2;
			m_box->setPosition(rect.x - Actual_w, m_box->getPos_y());
			break;
		case 3:
			
			break;
		case 4:
			//if(m_box->getVy() > 10)
			Position.y = rect.y;
			m_box->setPosition(m_box->getPos_x(), rect.y - Actual_h + 1);	//y坐标每帧产生碰撞
			isOnFlooor = true;
			break;
		default:
			break;
		}
		break;
	case 2:
		SDL_Log("%s\n",name.data());
		{
 			int monster_atk = DataManager::GetInstance().findMonster(name).phy_atk;
			int self_def = m_status.phy_def;
			if (monster_atk - self_def <= 0)	m_status.hp--;
			else m_status.hp -= (monster_atk - self_def);
			setInvincible();
		}
		break;
	case 3:
		SDL_Log("speed add\n");
		startBuff(0, 3000, 1.0f, base_vx, 0);
		break;
	default:
		break;
	}
}

void xplayer::setSuit(Head* h, Body* b, Legs* l)
{
	m_body = b;
	m_head = h;
	m_legs = l;

	m_body->onEnable();
	m_head->onEnable();
	m_legs->onEnable();

	if (m_body->getItemID() < 4 && m_head->getItemID() < 4 && m_head->getItemID() < 4)
		WANGBA_SPECIAL();
}

void xplayer::setInvincible()
{
	if(!isInvincible)
	{
		m_box->setDstLayer(CollisionBox::CollissionLayer::layer2, false);
		isInvincible = true;
		INvinTimer.reset();
		INvinTimer.start();
	}
}

void xplayer::onInvincibleTimeOut(int ununsed)
{
	isInvincible = false;
	m_box->setDstLayer(CollisionBox::CollissionLayer::layer2, true);
}

void JumpState::on_update(float delat)
{
	playerState::on_update(delat);

	if (m_player->getCollisionBox()->getVy() >= 0)
		m_player->switchState("Fall");
}

void JumpState::on_enter()
{
	playerState::on_enter();
	m_AnimationState->reset();
	m_player->getCollisionBox()->setVy(-7);
	m_player->setOnfloor(false);
}

void JumpState::on_exit()
{
	playerState::on_exit();
	SDL_Log("player jump exit");
}

void FallState::on_update(float delat)
{
	playerState::on_update(delat);

	if (m_player->getIsOnFloor())
		m_player->switchState("Idle");
}

void FallState::on_enter()
{
	playerState::on_enter();
	SDL_Log("player fall enter");
	m_AnimationState->reset();
}

void FallState::on_exit()
{
	playerState::on_exit();
	SDL_Log("player fall exit");
}

AttackState::AttackState(std::shared_ptr<AnimationState>&& state) :playerState(std::move(state))
{
	m_box = CollisionManager::instance().createBox();
	m_box->setSrcLayer(CollisionBox::CollissionLayer::layer4);
	m_box->setSize(50, 30);
	m_box->setcolor(10, 220, 30, 200);
	m_box->setEnable(false);
	
	m_box->setGravityEnable(false);

	m_AnimationState->setLoop(false);
	auto func = std::function<void()>([&]() { 
		if (m_player->getIsOnFloor())
			m_player->switchState("Idle"); 
		else if (m_player->getIsOnFloor() && m_player->getVvx() != 0)
			m_player->switchState("Run");
		else if (!m_player->getIsOnFloor())
			m_player->switchState("Fall"); 
	});
	m_AnimationState->setcallback(func);
}

void AttackState::on_update(float delat)
{
	playerState::on_update(delat);

	if(m_ftr)
		m_AnimationState->setFilp(false);
	else
		m_AnimationState->setFilp(true);
}

void AttackState::on_enter()
{
	playerState::on_enter();
	SDL_Log("player attack enter");
	m_AnimationState->reset();

	m_ftr = m_player->is_face_to_right();
	p_vx = m_player->getBaseVx();
	m_player->setVx(0);
	m_box->setEnable(true);
	if (m_ftr)
	{	
		m_box->setPosition(m_player->getPosition().x + m_player->getActW(), m_player->getPosition().y);
	}
	else
	{
		m_box->setPosition(m_player->getPosition().x - m_player->getActW(), m_player->getPosition().y);
	}
}

void AttackState::on_exit()
{
	playerState::on_exit();
	SDL_Log("player attack exit");
	m_box->setEnable(false);
	m_player->setVx(p_vx);

	
}

void playerState::on_update(float delat)
{
	auto pp = m_player->getPosition();
	m_AnimationState->setFilp(m_player->is_face_to_right() ? false : true);
	m_AnimationState->setPos(pp);
	m_AnimationState->on_update(delat);
}

void playerState::on_renderer()
{
	m_AnimationState->on_renderer();
}

void playerState::on_enter()
{
	auto pp = m_player->getPosition();
	m_AnimationState->setPos(pp);
}

void playerState::on_exit()
{
}

void IdleState::on_update(float delat)
{
	playerState::on_update(delat);

	if (m_player->getVvx() != 0 && m_player->getIsOnFloor())
		m_player->switchState("Run");
	else if (m_player->isJump())
		m_player->switchState("Jump");
	else if (!m_player->getIsOnFloor())
		m_player->switchState("Fall");
	else if (m_player->isAttack())
		m_player->switchState("Attack");
}

void IdleState::on_enter()
{
	SDL_Log("idle state enter");
	playerState::on_enter();
	m_player->getCollisionBox()->setVy(0);
}

void IdleState::on_exit()
{
	SDL_Log("idle state exit");

}

void RunState::on_update(float delat)
{
	playerState::on_update(delat);

	if (m_player->getVvx() == 0)
		m_player->switchState("Idle");
	else if (m_player->isJump())
		m_player->switchState("Jump");
	else if (!m_player->getIsOnFloor())
		m_player->switchState("Fall");
}

void RunState::on_enter()
{
	SDL_Log("run state enter");
	playerState::on_enter();
}

void RunState::on_exit()
{
	SDL_Log("run state exit");
}

//技能系统(树)
//战士、坦克、法师分支
//加攻击


