#pragma once

#include "Charactor.h"
#include "inputAbstract.h"
#include "xItem.h"

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
	AttackState(std::shared_ptr<AnimationState>&& state);
	~AttackState() {}
	void on_update(float delat) override;
	void on_enter() override;
	void on_exit() override;

private:
	CollisionBox* m_box = nullptr;
	bool m_ftr = true;
	float p_vx;
};

class xplayer:public Charactor
{
public:
	xplayer(SDL_Renderer* renderer, Animation::AnimationAnchor anch,float vx, NodeStatus& s);
	~xplayer() {};

	void on_input(SDL_Event& e);
	void on_update(float delat)override;
	void on_render() override;
	
	void on_CollisionCb(int layer,int collisionSide, SDL_FRect rect, std::string& name);
	void switchState(const std::string& name) { m_machine.switchTo(name); }
	//获取每帧移动像素值
	float getVvx() { return v_vx; }
	bool is_face_to_right() { return isfaceright; }
	bool isJump() { return ic.isSpace() && isOnFlooor; }
	bool isAttack() { return ic.isAttack(); }
	void resetFunc() { isOnFlooor = false; }
	void setSuit(Head* h, Body* b, Legs* l);
	void setInvincible()override;
	void onInvincibleTimeOut(int ununsed)override;

private:
	inputControl ic;
	float v_vx = 0.0f, v_vy = 0.0f;					//每帧速度
	bool  isfaceright = 0;							//控制轴
	Head* m_head = nullptr;
	Body* m_body = nullptr;
	Legs* m_legs = nullptr;
};

