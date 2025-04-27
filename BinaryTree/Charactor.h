#pragma once

#include "Animation.h"
#include "CollisionBox.h"
#include "util.h"
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

class buff {
public:
	enum buffType
	{
		SPEED = 0,		//速度
		PHY_ATTACK,		//物理攻击
		PHY_DEFENCE		//物理防御
	};
	buff() {
		buffTimer.setInterval(1000);
		buffTimer.setSingleShot(true);
		std::function<void(int i)> cb = std::bind(&buff::onCallback, this, std::placeholders::_1);
		buffTimer.setCallBack(cb);
	}
	buff(float times) {
		buffTimer.setInterval(times);
		buffTimer.setSingleShot(true);
		std::function<void(int i)> cb = std::bind(&buff::onCallback, this, std::placeholders::_1);
		buffTimer.setCallBack(cb);
	}
	~buff() = default;
	buff(const buff& o) {
		value = o.value;
		buffTimer.setInterval(o.buffTimer.getInterval());
		buffTimer.setSingleShot(o.buffTimer.getSignleShot());
		std::function<void(int i)> cb = std::bind(&buff::onCallback, this, std::placeholders::_1);
		buffTimer.setCallBack(cb);
	}
	buff& operator=(const buff& o) {
		if (this != &o) {
			value = o.value;
			buffTimer.setInterval(o.buffTimer.getInterval());
			buffTimer.setSingleShot(o.buffTimer.getSignleShot());
			std::function<void(int i)> cb = std::bind(&buff::onCallback, this, std::placeholders::_1);
			buffTimer.setCallBack(cb);
		}
		return *this;
	}
	virtual void add(float v) { value += v; }
	virtual void sub(float v) { value -= v; }
	virtual void onCallback(int i) {
		if(exit)
			exit();
	}

	void on_update(float delta);
	void setInterval(float interval) { buffTimer.setInterval(interval); }
	void onStart() { buffTimer.start();if(enter) enter(); }
	void setEnable(bool e) { m_isEnabled = e; }
	void setBaseValue(float v) { base_value = v; }
	bool isEnabled() { return m_isEnabled; }
	float getValue() { return value * base_value; }
	void setEnterCallBack(std::function<void()> cb) {
		this->enter = cb;
	}
	void setExitCallBack(std::function<void()> cb) {
		this->exit = cb;
	}

protected:
	float value = 1.0f, base_value = 1.0f;
	mTimer buffTimer;
	bool m_isEnabled = false;
	std::function<void()> enter,exit;
};

class speedBuff : public buff
{
public:
	speedBuff() :buff() {}

	void add(float v) { 
		buff::add(v);
		if (value >= 2.0f)
		{
			value = 2.0f;
		}
	}
	void onCallback(int i)override {
		buff::onCallback(i);
		value = 1.0f;
	}
};

class buffManager
{
public:
	buffManager();
	~buffManager();

	void setBaseSpeed(float v) { m_buffs[0]->setBaseValue(v); }

	void startBuff(int type, float times, float value, float baseValue, int valueType) {
		auto it = m_buffs.find(type);
		if (it != m_buffs.end()) {
			switch (valueType)
			{
			case 0:
				it->second->add(value);
				break;
			case 1:
				it->second->sub(value);
				break;
			default:
				break;
			}
			it->second->setBaseValue(baseValue);
			it->second->setInterval(times);
			it->second->onStart();
		}
	}
	void on_update(float delta) {
		for (m_iter = m_buffs.begin(); m_iter != m_end; ++m_iter)
		{
			m_iter->second->on_update(delta);
		}
	}
	float getBuffValue(int type) {
		auto it = m_buffs.find(type);
		if (it != m_buffs.end()) {
			return it->second->getValue();
		}
		return 1.0f;
	}

	void setEnterCB(int type, std::function<void()> cb) {
		auto it = m_buffs.find(type);
		if (it != m_buffs.end()) {
			it->second->setEnterCallBack(cb);
		}
	}
	void setExitCB(int type, std::function<void()> cb) {
		auto it = m_buffs.find(type);
		if (it != m_buffs.end()) {
			it->second->setExitCallBack(cb);
		}
	}
private:
	std::map<int, std::shared_ptr<buff>> m_buffs;
	std::map<int, std::shared_ptr<buff>>::iterator m_iter;
	std::map<int, std::shared_ptr<buff>>::iterator m_end;
};

class Charactor :public XNode
{
public:
	Charactor() = default;
	virtual ~Charactor() = default;
	Charactor(const std::string& name, Animation::AnimationAnchor anch, float vx, NodeStatus& s):XNode(name),vx(vx),isOnFlooor(false),m_anchor(anch), m_status(s){
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

		m_buffManager.setBaseSpeed(vx);
	}

	void on_update(float delat)override {
		INvinTimer.on_update(delat);
		renderTimer.on_update(delat);
		m_buffManager.on_update(delat);

		vx = m_buffManager.getBuffValue(0);
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
	void setNodeStatus(const GameEngine2D::NodeStatus& s) { m_status = s; }
	GameEngine2D::NodeStatus& getNodeStatus() { return m_status; }

	//buff类型,持续时间ms,数值,数值类型(0,加;1,减)
	void startBuff(int type, float times, float value,float baseValue, int valueType) {
		m_buffManager.startBuff(type, times, value, vx, valueType);
	}
	int getHP() { 
		return m_status.hp; 
	}
	//获取每秒移动像素值
	float getVx() { return vx; }
	float getActH() { return Actual_h; }
	float getActW() { return Actual_w; }

protected:
	SDL_FPoint Position;						//角色位置
	float vx;									//横向速度
	float Actual_h, Actual_w;					//实际的角色的高度
	Animation::AnimationAnchor m_anchor;		//锚点
	bool isOnFlooor;							//是否在地面
	CollisionBox* m_box;
	bool isInvincible = false, isNeedRender = false;
	mTimer INvinTimer, renderTimer;
	GameEngine2D::NodeStatus m_status;
	buffManager m_buffManager;
};

