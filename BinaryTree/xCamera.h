#pragma once
#include "XNode.h"
#include "mtimer.h"
#include <SDL.h>

class xCamera:public XNode
{
public:
	xCamera():xCamera("camera") {
		
	};
	xCamera(const std::string& name) :XNode(name), CameraRect({0,0,0,0}) {
		setInput(false);
		m_timer.setSingleShot(true);
		m_timer.setCallBack([&](int i) {
			moveing = false;
			m_freqTimer.reset();
			diffx = 0.0f;
		});

		m_freqTimer.setInterval(100);
		m_freqTimer.setSingleShot(false);
		m_freqTimer.setCallBack([&](int i) {
			if (moveing)
			{
				t += p;
				onFreq();
			}
		});
	}
	void on_update(float delat) override {
		m_timer.on_update(delat);
		m_freqTimer.on_update(delat);
	}
	void on_render() override {}
	void setPosition(const SDL_FPoint& p) { CameraRect.x = p.x; CameraRect.y = p.y; }
	void setSize(float w, float h) { CameraRect.w = w; CameraRect.h = h; }
	void movePos(float x, float y, int ms);

	SDL_FRect CameraRect;
	float diffx = 0.0f;
private:
	void onFreq();
	mTimer m_timer, m_freqTimer;
	bool moveing = false;
	float startP = 0.0f, endP = 0.0f;
	float t = 0.0f, p = 0.0f;
};

