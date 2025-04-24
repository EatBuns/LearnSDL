#include "Animation.h"

Animation::Animation(const std::string& animation_name) :speed(12), is_loop(false), g_renderer(nullptr),XNode(animation_name),
														scale_x(1.0f),scale_y(1.0f),m_anchor(AnimationAnchor::bottom), pos({ 0.0,0.0 })
{
	auto& vec = DataManager::GetInstance().findAtlas(animation_name);
	if (vec.size() == 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "animation %s not found", animation_name.data());
		return;
	}

	for (auto& t : vec)
	{
		AddFrame(t);
	}

	timer.setInterval((float)(1000.0 / speed));
	timer.setSingleShot(is_loop);
	timer.setCallBack(std::bind(&Animation::onCallback,this));
	timer.start();
}

Animation::Animation(const std::string& animation_name, int idx):speed(12), is_loop(false), g_renderer(nullptr), XNode(animation_name),
																 scale_x(1.0f), scale_y(1.0f), m_anchor(AnimationAnchor::bottom), pos({ 0.0,0.0 })
{
	auto& vec = DataManager::GetInstance().findAtlasIndex(animation_name,idx);
	if (vec == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "animation %s not found", animation_name.data());
		return;
	}

	AddFrame(vec);
	timer.setInterval((float)(1000.0 / speed));
	timer.setSingleShot(is_loop);
	timer.setCallBack(std::bind(&Animation::onCallback, this));
	timer.start();
}

Animation::Animation() :speed(12), is_loop(false), g_renderer(nullptr), scale_x(1.0f), scale_y(1.0f), m_anchor(AnimationAnchor::bottom), pos({0.0,0.0})
{
	timer.setInterval((float)(1000.0 / speed));
	timer.setSingleShot(is_loop);
	timer.setCallBack(std::bind(&Animation::onCallback, this));
	timer.start();
}

Animation::~Animation()
{
}

void Animation::reset()
{
	timer.reset();
	timer.start();
	frameIndex = 0;
}

void Animation::on_update(float delta)
{
	for (auto& child : x_childs)
	{
		child->on_update(delta);
	}

	timer.on_update(delta);
}

void Animation::on_render()
{
	if (frameIndex > frames.size() - 1) return;

	auto curTexture = frames[frameIndex];
	int w, h;
	const int fillsize = 10;		//ÃªµãµÄsize
	SDL_QueryTexture(curTexture.get(), NULL, NULL, &w, &h);
	SDL_Rect src, dest, R_anch;
	src.x = 0;
	src.y = 0;
	src.w = w;
	src.h = h;

	w *= scale_x;
	h *= scale_y;
	dest.w = w;
	dest.h = h;
	R_anch.w = fillsize;
	R_anch.h = fillsize;

	switch (m_anchor)
	{	
	case Animation::AnimationAnchor::leftup:
		dest.x = pos.x;
		dest.y = pos.y;
		R_anch.x = pos.x - fillsize/2;
		R_anch.y = pos.y - fillsize/2;
		break;
	case Animation::AnimationAnchor::up:
		dest.x = pos.x - w/ 2;
		dest.y = pos.y;
		R_anch.x = dest.w / 2 + dest.x - fillsize / 2;
		R_anch.y = pos.y - fillsize / 2;
		break;
	case Animation::AnimationAnchor::rightup:
		dest.x = pos.x - w;
		dest.y = pos.y;
		R_anch.x = dest.x + dest.w - R_anch.w/2;
		R_anch.y = pos.y - fillsize / 2;
		break;
	case Animation::AnimationAnchor::leftcenter:
		dest.x = pos.x;
		dest.y = pos.y - h / 2;
		R_anch.x = pos.x - fillsize / 2;
		R_anch.y = dest.h / 2 + dest.y - fillsize / 2;
		break;
	case Animation::AnimationAnchor::center:
		dest.x = pos.x - w / 2;
		dest.y = pos.y - h / 2;
		R_anch.x = dest.w / 2 + dest.x - fillsize / 2;
		R_anch.y = dest.h / 2 + dest.y - fillsize / 2;
		break;
	case Animation::AnimationAnchor::rightcenter:
		dest.x = pos.x - w;
		dest.y = pos.y - h / 2;
		R_anch.x = dest.x + dest.w - R_anch.w / 2;
		R_anch.y = dest.h / 2 + dest.y - fillsize / 2;
		break;
	case Animation::AnimationAnchor::leftbottom:
		dest.x = pos.x;
		dest.y = pos.y - h;
		R_anch.x = pos.x - fillsize / 2;
		R_anch.y = dest.y + dest.h - fillsize / 2;
		break;
	case Animation::AnimationAnchor::bottom:
		dest.x = pos.x - w / 2;
		dest.y = pos.y - h;
		R_anch.x = dest.w / 2 + dest.x - fillsize/2;
		R_anch.y = dest.y + dest.h - fillsize / 2;
		break;
	case Animation::AnimationAnchor::rightbottom:
		dest.x = pos.x - w;
		dest.y = pos.y - h;
		R_anch.x = dest.x + dest.w - R_anch.w / 2;
		R_anch.y = dest.y + dest.h - fillsize / 2;
		break;
	default:
		dest.x = pos.x;
		dest.y = pos.y;
		R_anch.x = pos.x - fillsize / 2;
		R_anch.y = pos.y - fillsize / 2;
		break;
	}

	if (g_renderer)
	{
		is_filp ? SDL_RenderCopyEx(g_renderer, curTexture.get(), &src, &dest, 0, NULL, SDL_FLIP_HORIZONTAL) :
				  SDL_RenderCopyEx(g_renderer, curTexture.get(), &src, &dest, 0, NULL, SDL_FLIP_NONE);
		
		//»­äÖÈ¾¿ò
		SDL_SetRenderDrawColor(g_renderer, 0, 50, 150, 255);
		SDL_RenderDrawRect(g_renderer, &dest);
		//»­Ãªµã
		SDL_SetRenderDrawColor(g_renderer, 255, 100, 100, 255);
		SDL_RenderFillRect(g_renderer, &R_anch);
		//±ê×¢×ø±ê
		char buf[32];
		memset(buf, 0, 32);
		sprintf_s(buf, 32, "x:%.2f,y:%.2f\n", pos.x,pos.y);
		Font::instance().drawText(buf, pos.x - 16, pos.y - 16, 255, 255, 255, 255, "IPix");
	}
		
	else
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "animation %s renderer is NULL", x_name.data());
}

void Animation::setCallBack(std::function<void()>& ccb)
{
	cb = ccb;
}

void Animation::setAnchor(AnimationAnchor anch)
{
	m_anchor = anch;
}

void Animation::onCallback()
{
	frameIndex++;
	if (frameIndex > frames.size() - 1)
	{
		frameIndex = is_loop ? 0 : frames.size() - 1;
		timer.setInterval((float)(1000.0 / speed) * Scale_factor[frameIndex]);
		if (!is_loop && cb)
			cb();
	}
}
