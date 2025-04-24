#include "mtimer.h"
#include <iostream>

void mTimer::reset()
{
	passtime = 0;
	TriggleCount = 0;
	isStart = false;
}

void mTimer::setInterval(float interv)
{
	interval = interv;
}

void mTimer::setSingleShot(bool SingleShot)
{
	isSingleShot = SingleShot;
}

void mTimer::on_update(float delta)
{
	if(!checkDelay(delta)) return;
	if (!isStart) return;

	passtime += delta;
	if(passtime >= interval)
	{
		if ((isStart || !isSingleShot) && cb)
		{
			cb(interval);
			TriggleCount++;
		}
		if (isSingleShot) isStart = false;
		if (TriggleCount > 1000) TriggleCount = 0;
		passtime = 0;
	}
}

void mTimer::start()
{
	isStart = true;
}

void mTimer::start(int delay)
{
	delaytime = delay;
	passDelayTime = 0;
	isStart = true;
}

void mTimer::setCallBack(const std::function<void(int)>& callback)
{
	cb = callback;
}

float mTimer::getInterval()const
{
	return interval;
}

bool mTimer::getSignleShot() const
{
	return isSingleShot;
}

bool mTimer::checkDelay(int delta)
{
	passDelayTime += delta;
	if (passDelayTime < delaytime) return false;
	passDelayTime = delaytime;
	return true;
}
