#pragma once
#include "XNode.h"
#include <SDL.h>

class xCamera:public XNode
{
public:
	xCamera() :CameraRect({ 0,0,0,0 }) { setInput(false); };
	xCamera(std::string& name) :XNode(name), CameraRect({0,0,0,0}) {
		setInput(false);
	}
	void on_update(float delat) override {}
	void on_render() override {}
	void setPosition(const SDL_FPoint& p) { CameraRect.x = p.x; CameraRect.y = p.y; }
	void setSize(float w, float h) { CameraRect.w = w; CameraRect.h = h; }
	void movePos(float x, float y) { CameraRect.x += x; CameraRect.y += y; }

	SDL_FRect CameraRect;
};

