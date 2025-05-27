#pragma once
#include "XNode.h"
#include "Animation.h"

enum ItemType
{
	HEAD,
	BODY,
	LEGS,
	WEAPON,
	CONSUMABLE
};

enum ItemID
{
	HEAD_WANGBA = 0,
	BODY_WANGBA,
	LEGS_WANGBA,
	
	HEAD_STEAL = 4,
	BODY_STEAL,
	LEGS_STEAL,
};

class xItem :public XNode
{
public:
	xItem(const std::string& name, SDL_Renderer* renderer, ItemType t, ItemID id) :XNode(name), m_type(t), m_id(id)
	{
		m_animation.setRenderer(renderer);
	}
	virtual	~xItem() = default;

	void on_update(float delat) override;
	void on_render()override;
	void setPosition(SDL_FPoint& p);
	virtual void onEnable() = 0;
	ItemID getItemID() { return m_id; }
	ItemType getItemType() { return m_type; }

protected:
	Animation	m_animation;
	ItemType	m_type;
	ItemID		m_id;
	std::function<void()> cb;
};

class Head :public xItem
{
public:
	Head(const std::string& name, SDL_Renderer* renderer, ItemID id) :xItem(name, renderer, ItemType::HEAD, id) {}
	virtual void onEnable() = 0;
	virtual ~Head() {}
protected:
	
};

class Body :public xItem{
public:
	Body(const std::string& name, SDL_Renderer* renderer, ItemID id) :xItem(name, renderer, ItemType::BODY, id) {}
	virtual void onEnable() = 0;
	virtual ~Body() {}
};

class Legs :public xItem{
public:
	Legs(const std::string& name, SDL_Renderer* renderer, ItemID id) :xItem(name, renderer, ItemType::LEGS, id) {}
	virtual void onEnable() = 0;
	virtual ~Legs() {}
};

class WANGBA_Head :public Head
{
public:
	WANGBA_Head(const std::string& name, SDL_Renderer* renderer, ItemID id) :Head(name, renderer, id) {
		m_animation.setPlaySpeed(24);
		m_animation.setLoop(true);
		m_animation.setAnchor(Animation::AnimationAnchor::leftup);
	}
	virtual void onEnable() override;
	~WANGBA_Head() {};
};

class WANGBA_Body :public Body
{
public:
	WANGBA_Body(const std::string& name, SDL_Renderer* renderer, ItemID id) :Body(name, renderer, id) {
		m_animation.setPlaySpeed(24);
		m_animation.setLoop(true);
		m_animation.setAnchor(Animation::AnimationAnchor::leftup);
	}
	virtual void onEnable() override;
	~WANGBA_Body() {};
};

class WANGBA_Legs :public Legs
{
public:
	WANGBA_Legs(const std::string& name, SDL_Renderer* renderer, ItemID id) :Legs(name, renderer, id) {
		m_animation.setPlaySpeed(24);
		m_animation.setLoop(true);
		m_animation.setAnchor(Animation::AnimationAnchor::leftup);
	}
	virtual void onEnable() override;
	~WANGBA_Legs() {};
};

class skillNode :public XNode
{
public:
	skillNode(const std::string& name, SDL_Renderer* renderer) :XNode(name), m_renderer(renderer), m_Enable(false),skillname(name)
	{
		m_animation.setRenderer(renderer);
	}
	virtual ~skillNode() = default;
	void on_update(float delat) override {}
	void on_render()override {
		m_animation.on_render();
	}
	void setPosition(SDL_FPoint& p) {
		m_animation.setPos(p);
	}
	virtual void onEnable() {
		m_Enable != m_Enable;
	};
protected:
	Animation	m_animation;
	SDL_Renderer* m_renderer;
	std::function<void()> cb;
	std::string skillname;
	bool m_Enable;
};

class SkillNodeTree
{
public:
	void addChild(std::unique_ptr<skillNode> node)
	{
		childs.push_back(std::move(node));
	}
private:
	std::vector<std::unique_ptr<skillNode>> childs;
};

void WANGBA_SPECIAL();