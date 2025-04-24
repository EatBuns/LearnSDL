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

	void reset();			//����
	void setInterval(float interv);		//���ü��ʱ��
	void setSingleShot(bool SingleShot);
	void on_update(float delta) override;		//����
	void on_render() override {};				//��Ⱦ
	void start();
	void start(int delay);
	void setCallBack(const std::function<void(int)>& callback);
	float getInterval()const;
	bool getSignleShot()const;

private:
	bool checkDelay(int delta);
	float passtime = 0;	//����ʱ��
	float interval = 0;	//���ʱ��
	float delaytime = 0;	//�ӳ�ʱ��
	int TriggleCount = 0;
	int passDelayTime = 0;
	bool isSingleShot = false;	//���δ���
	bool isStart = false;		//�Ƿ�ʼ
	std::function<void(int)> cb;
};
