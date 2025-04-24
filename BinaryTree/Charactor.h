#pragma once

#include "Animation.h"
#include "CollisionBox.h"
#include <map>

class xState
{
public:
	xState() = default;
	virtual ~xState() = default;
	virtual void on_update(float delta) = 0;
	virtual void on_renderer() = 0;
	virtual void on_enter() = 0;
	virtual void on_exit() = 0;
};

class AnimationState : public xState
{
public:
	AnimationState(const std::string& name, SDL_Renderer* renderer);
	AnimationState(const std::string& name, int idx, SDL_Renderer* renderer);
	AnimationState(SDL_Renderer* renderer);
	virtual ~AnimationState();

	void on_update(float delta) override;
	void on_renderer() override;
	void on_enter() final {};
	void on_exit() final {};

	void setPos(SDL_FPoint& pos);
	void setFilp(bool f) { m_animation.setFlip(f); }
	void setPlaySpeed(int speed) { m_animation.setPlaySpeed(speed); }
	void setLoop(bool loop) { m_animation.setLoop(loop); }
	void setAnchor(Animation::AnimationAnchor anch) {m_animation.setAnchor(anch);}
	void setRender(SDL_Renderer* r) { m_animation.setRenderer(r); }
	void setcallback(std::function<void()>& ccb) { m_animation.setCallBack(ccb); }
	void reset() { m_animation.reset(); }
	void setXName(const std::string& name) { m_animation.setXName(name); }
	void setScalex(float x) { m_animation.setScaleX(x); }
	void setScaley(float y) { m_animation.setScaleY(y); }
	void addFrame(std::shared_ptr<SDL_Texture>& frame, float sf = 1.0f) { m_animation.AddFrame(frame, sf); }

private:
	Animation m_animation;
};

class StateMachine
{
public:
	StateMachine() = default;
	~StateMachine() {}

	void registerState(const std::string& name, std::shared_ptr<xState>&& state) {
		m_states[name] = std::move(state);
	}

	void setCurrentState(const std::string& name) {
		auto it = m_states.find(name);
		if (it != m_states.end()) {
			m_currentState = it->second;
		}
		else {
			SDL_LogError(0, "State %s not found\n", name.c_str());
		}
	}

	void switchTo(const std::string& name)
	{
		if (m_currentState) m_currentState->on_exit();
		setCurrentState(name);
		if (m_currentState)m_currentState->on_enter();
	}

	void on_update(float delta) {
		if (!m_currentState) return;
		if (needInit)
		{
			m_currentState->on_enter();
			needInit = false;
		}
		m_currentState->on_update(delta);
	}
	void on_render() { m_currentState->on_renderer(); }

private:
	std::map<std::string, std::shared_ptr<xState>> m_states;
	std::shared_ptr<xState> m_currentState;
	bool needInit = true;
};

class Charactor :public XNode
{
public:
	Charactor() = default;
	virtual ~Charactor() = default;
	Charactor(const std::string& name, Animation::AnimationAnchor anch):XNode(name),vx(0),isOnFlooor(false),m_anchor(anch){
		Position.x = 0;
		Position.y = 0;

		INvinTimer.setInterval(1000);
		INvinTimer.setSingleShot(true);
		INvinTimer.setCallBack([&](int i) {
			isInvincible = false;
			m_box->setDstLayer(CollisionBox::CollissionLayer::layer2, true);
		});

		renderTimer.setInterval(50);
		renderTimer.setSingleShot(false);
		renderTimer.setCallBack([&](int i) {
			isNeedRender = !isNeedRender;
		});
		renderTimer.start();
	}

	void on_update(float delat)override {
		INvinTimer.on_update(delat);
		renderTimer.on_update(delat);
	}
	void on_render() override {}

	void setPosition(const SDL_FPoint& p);
	void setVx(float x) { vx = x; }
	void setActualH(float h) { Actual_h = h; }
	void setActualW(float w) { Actual_w = w; }
	void setOnfloor(bool o) { isOnFlooor = o; }
	bool getIsOnFloor() { return isOnFlooor; }
	const SDL_FPoint& getPosition()const { return Position; }
	void setCollisionBox(CollisionBox* box);
	//设置动画锚点
	void setAnchor(Animation::AnimationAnchor anch) {
		m_anchor = anch;
	}
	CollisionBox* getCollisionBox() { return m_box; }
	void setInvincible() { 
		if (!isInvincible)
		{
			m_box->setDstLayer(CollisionBox::CollissionLayer::layer2,false);
			isInvincible = true; 
			INvinTimer.reset(); 
			INvinTimer.start();
		}
	}

protected:
	//AnimationState* current_state;
	SDL_FPoint Position;						//角色位置
	float vx;									//横向速度
	float Actual_h, Actual_w;					//实际的角色的高度
	Animation::AnimationAnchor m_anchor;		//锚点
	bool isOnFlooor;							//是否在地面
	CollisionBox* m_box;
	bool isInvincible = false, isNeedRender = false;
	mTimer INvinTimer, renderTimer;
};

