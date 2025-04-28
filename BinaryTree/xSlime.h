#pragma once

#include "Charactor.h"
#include "CollisionBox.h"
#include "mtimer.h"

class xSlime;
class SlimeState :public xState
{
public:
	SlimeState(std::shared_ptr<AnimationState>&& state) :m_AnimationState(std::move(state)) {}
	virtual ~SlimeState() {}

	void on_update(float delat) override;
	void on_renderer() final;
	void on_enter()override;
	void on_exit()override;
	static void setPlayer(xSlime* p) { m_slime = p; }

protected:
	std::shared_ptr<AnimationState> m_AnimationState;
	static xSlime* m_slime;
};

class SlimeIdleState :public SlimeState
{
public:
	SlimeIdleState(std::shared_ptr<AnimationState>&& state);
	~SlimeIdleState() {}
	void on_update(float delat) override;
	void on_enter() override;
	void on_exit() override;

private:
	mTimer timer;
};

class SlimeJumpState :public SlimeState
{
public:
	SlimeJumpState(std::shared_ptr<AnimationState>&& state);
	~SlimeJumpState() {}
	void on_update(float delat) override;
	void on_enter() override;
	void on_exit() override;
};

class xSlime :public Charactor
{
public:
	xSlime(SDL_Renderer* renderer, Animation::AnimationAnchor anch,float vx, NodeStatus& s);
	~xSlime() {}
	void on_update(float delat)override;
	void on_render() override;
	void on_CollisionCb(int layer, int collisionSide, SDL_FRect rect, std::string& name);
	void switchState(const std::string& name) { m_machine.switchTo(name); }
	float getVvx() { return v_vx; }
	void resetFunc() { isOnFlooor = false; }
	void setInvincible()override;
	void onInvincibleTimeOut(int ununsed)override;

private:
	float v_vx, v_vy;			//每帧速度
	int direction = 1;			//1:向右;	-1:向左
};

