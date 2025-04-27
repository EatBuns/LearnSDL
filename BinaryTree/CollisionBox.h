#pragma once
#include <SDL.h>
#include <map>
#include <functional>
#include <string>

class CollisionBox
{
public:
	friend class CollisionManager;
	enum class CollissionLayer
	{
		None = -1,
		layer0 = 0,		//玩家
		layer1,			//平台
		layer2,			//敌人
		layer3,			//道具
		layer4,			//玩家武器碰撞层
		layer5
	};
	~CollisionBox() {};
	void on_update(float delat);
	void on_render(SDL_Renderer* renderer){
		if (!m_isEnable) return;
		SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
		SDL_RenderFillRectF(renderer, &m_rect);
	};

	void setPosition(float x, float y) { m_rect.x = x; m_rect.y = y; }
	void setSize(float w, float h) { m_rect.w = w; m_rect.h = h; }	
	void setcolor(int r, int g, int b, int a) { m_color.r = r; m_color.g = g; m_color.b = b; m_color.a = a; }
	void setSrcLayer(CollissionLayer layer) { src_layer = layer; }
	void setDstLayer(CollissionLayer layer, bool isEnable) { dst_layer[layer] = isEnable; }
	void setGravityEnable(bool isEnable) { isGravity = isEnable; }
	void setGy(float y) { g_y = y; }
	void setGx(float x) { g_x = x; }
	void setVy(float y) { v_y = y; }
	void setVx(float x) { v_x = x; }
	void setCallBack(const std::function<void(int, int, SDL_FRect)>& ccb) { cb = ccb; }
	void setResetCb(const std::function<void()> ccb) { resetcb = ccb; }
	void setEnable(bool e) { m_isEnable = e; }
	SDL_FRect& getRect() { return m_rect; }
	float getVy() { return v_y; }
	float getVx() { return v_x; }
	float getNextFrameVy(){
		if (v_y < 10)
			v_y += g_y;
		return v_y;
	}
	bool isGravityEnable() { return isGravity; }
	bool isEnable() { return m_isEnable; }
	std::map<CollissionLayer, bool>& getDstLayer() { return dst_layer; }
	CollissionLayer getSrcLayer() { return src_layer; }
	float getPos_x() { return m_rect.x; }
	float getPos_y() { return m_rect.y; }
	void setNodeName(const std::string& name) { NodeName = name; }
	std::string getNodeName() { return NodeName; }

private:
	CollisionBox() {
		dst_layer[CollissionLayer::layer0] = false;
		dst_layer[CollissionLayer::layer1] = false;
		dst_layer[CollissionLayer::layer2] = false;
		dst_layer[CollissionLayer::layer3] = false;
		dst_layer[CollissionLayer::layer4] = false;
	}

	SDL_FRect m_rect;
	SDL_Color m_color;
	float g_x = 0.0f, g_y = 0.3f;
	float v_x = 0.0f, v_y = 0.0f;
	bool isGravity = false;
	bool m_isEnable = true;
	CollissionLayer src_layer;
	std::map<CollissionLayer, bool> dst_layer;
	std::function<void(int, int,SDL_FRect)> cb;
	std::function<void()> resetcb = nullptr;
	std::function<void(CollisionBox* ptr, float delat)> updateFunc;
	std::string NodeName;
};

