#include "xCamera.h"
#include "util.h"

void xCamera::movePos(float x, float y, int ms)
{
	if (moveing || GameEngine2D::isZero(x)) return;

	SDL_Log("camera moving");
	moveing = true;
	m_timer.setInterval(ms);

	startP = CameraRect.x;
	endP = CameraRect.x + x;
	SDL_Log("startp:%.2f,endP:%.2f\n",startP,endP);
//CameraRect.y += y;

	float f = ms / 100.0f;
	t = 0.0f;
	p = 1 / f;


	m_timer.start();
	m_freqTimer.start();
}

void xCamera::onFreq( )
{
	SDL_Log("t:%.2f", t);
	float temp = GameEngine2D::linear_interpolate_float(startP, endP, t);
	
	CameraRect.x = temp;
	diffx = endP - temp;
	SDL_Log("calc:%.2f, diffx:%.2f\n", temp, diffx);
}
