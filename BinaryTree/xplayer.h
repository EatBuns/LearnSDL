#pragma once

#include "Charactor.h"
#include "inputAbstract.h"


class xplayer;

/*
class IdleAnimationState :public AnimationState
{
public:
	IdleAnimationState(const std::string& name, SDL_Renderer* renderer) :AnimationState(name, renderer) {};
	~IdleAnimationState() {};
};

class RunAnimationState :public AnimationState
{
public:
	RunAnimationState(const std::string& name, SDL_Renderer* renderer) :AnimationState(name, renderer) {};
	~RunAnimationState() {};
};

class JumpAnimationState :public AnimationState
{
public:
	JumpAnimationState(const std::string& name, SDL_Renderer* renderer) :AnimationState(name,renderer) {};
	~JumpAnimationState() {};
};

class AttackAnimationState :public AnimationState
{
public:
	AttackAnimationState(const std::string& name, SDL_Renderer* renderer) :AnimationState(name,renderer) {};
	~AttackAnimationState() {};
};*/

class playerState :public xState
{
public:
	playerState( std::shared_ptr<AnimationState>&& state) :m_AnimationState(std::move(state)) {}
	virtual ~playerState() {}

	void on_update(float delat) override;
	void on_renderer() final;
	void on_enter()override;
	void on_exit()override;
	static void setPlayer(xplayer* p) { m_player = p; }

protected:
	std::shared_ptr<AnimationState> m_AnimationState;
	static xplayer* m_player;
};

class IdleState :public playerState
{
public:
	IdleState( std::shared_ptr<AnimationState>&& state) :playerState(std::move(state)) {}
	~IdleState() {}
	void on_update(float delat) override;
	void on_enter() override;
	void on_exit() override;
};

class RunState :public playerState
{
public:
	RunState(std::shared_ptr<AnimationState>&& state) :playerState(std::move(state)) {}
	~RunState() {}
	void on_update(float delat) override;
	void on_enter() override;
	void on_exit() override;
};

class JumpState :public playerState
{
public:
	JumpState(std::shared_ptr<AnimationState>&& state) :playerState(std::move(state)) {
		m_AnimationState->setLoop(false);
	}
	~JumpState() {}
	void on_update(float delat) override;
	void on_enter() override;
	void on_exit() override;
};

class FallState :public playerState
{
public:
	FallState(std::shared_ptr<AnimationState>&& state) :playerState(std::move(state)) {
		m_AnimationState->setLoop(false);
		m_AnimationState->setPlaySpeed(36);
	}
	~FallState() {}
	void on_update(float delat) override;
	void on_enter() override;
	void on_exit() override;
};

class AttackState :public playerState
{
public:
	AttackState(std::shared_ptr<AnimationState>&& state) :playerState(std::move(state)) {
		m_AnimationState->setLoop(false);
	}
	~AttackState() {}
	void on_update(float delat) override;
	void on_enter() override;
	void on_exit() override;
};

class xplayer:public Charactor
{
public:
	xplayer(SDL_Renderer* renderer, Animation::AnimationAnchor anch);
	~xplayer() {};

	void on_input(SDL_Event& e);
	void on_update(float delat)override;
	void on_render() override;
	
	void on_CollisionCb(int layer,int collisionSide, SDL_FRect rect);
	void switchState(const std::string& name) { m_machine.switchTo(name); }
	float getVvx() { return v_vx; }
	bool is_face_to_right() { return isfaceright; }
	bool isJump() { return ic.isSpace() && isOnFlooor; }
	bool isAttack() { return ic.isAttack(); }
	void resetFunc() { isOnFlooor = false; }

private:
	inputControl ic;
	StateMachine m_machine;
	float v_vx,v_vy;					//每帧速度
	bool  isfaceright = 0;				//控制轴

};

