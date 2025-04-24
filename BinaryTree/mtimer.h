#pragma once
#include <functional>
#include "XNode.h"

class mTimer :public XNode
{
public:
	mTimer() = default;
	~mTimer() = default;
	mTimer(const mTimer& other) = default;
	mTimer& operator=(const mTimer& other) = default;

	void reset();			//重置
	void setInterval(float interv);		//设置间隔时间
	void setSingleShot(bool SingleShot);
	void on_update(float delta) override;		//更新
	void on_render() override {};				//渲染
	void start();
	void start(int delay);
	void setCallBack(const std::function<void(int)>& callback);
	float getInterval()const;
	bool getSignleShot()const;

private:
	bool checkDelay(int delta);
	float passtime = 0;	//经过时间
	float interval = 0;	//间隔时间
	float delaytime = 0;	//延迟时间
	int TriggleCount = 0;
	int passDelayTime = 0;
	bool isSingleShot = false;	//单次触发
	bool isStart = false;		//是否开始
	std::function<void(int)> cb;
};
