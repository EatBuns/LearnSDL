#pragma once

#include "XNode.h"
#include "mtimer.h"
#include "Font.h"
#include <vector>
#include "DataManager.h"

class Animation :public XNode
{
public:
	Animation(const std::string& animation_name);
	Animation(const std::string& animation_name, int idx);
	Animation();
	~Animation();

	enum class AnimationAnchor
	{
		leftup,
		up,
		rightup,
		leftcenter,
		center,
		rightcenter,
		leftbottom,
		bottom,
		rightbottom
	};

	void setPlaySpeed(int speed) { this->speed = speed; }
	void setLoop(bool loop) { this->is_loop = loop; }
	int  getPlaySpeed()const { return speed; }
	bool getLoop()const { return is_loop; }
	bool getFlip()const { return is_filp; }
	void AddFrame(std::shared_ptr<SDL_Texture>& frame, float sf = 1.0f) { frames.push_back(frame); Scale_factor.push_back(sf);}
	void setPos(const SDL_FPoint& p) { pos = p; }
	void setRenderer(SDL_Renderer* renderer) { g_renderer = renderer; }
	void reset();

	void on_update(float delta) override;
	void on_render() override;
	void setCallBack(std::function<void()>& ccb);
	void setAnchor(AnimationAnchor anch);
	void setFlip(bool f) { is_filp = f; }
	void setScaleX(float x) { scale_x = x; }
	void setScaleY(float y) { scale_y = y; }
	const float getScaleX() const { return scale_x; }
	const float getScaleY() const { return scale_y; }

private:
	void onCallback();

	int speed;				//�����ٶ�,ÿ��֡��
	bool is_loop;			//�Ƿ�ѭ������
	bool is_filp = false;	//�Ƿ���Ҫ��ת
	std::vector<std::shared_ptr<SDL_Texture>> frames;	//����֡
	std::vector<float> Scale_factor;	//ʱ�����ű���
	mTimer timer;
	int frameIndex = 0;
	SDL_Renderer* g_renderer;
	SDL_FPoint pos;
	std::function<void()> cb;
	AnimationAnchor m_anchor;		//ê��
	float scale_x, scale_y;
};

