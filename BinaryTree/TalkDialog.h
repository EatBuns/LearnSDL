#pragma once

#include "XNode.h"
#include "Font.h"



class TalkDialog :public XNode
{

public:
	TalkDialog(SDL_Renderer* renderer);
	~TalkDialog();
	void setPos(const SDL_Point& p) { pos = p; }
	void setRenderer(SDL_Renderer* renderer) { g_renderer = renderer; }
	void setText(const std::string& text) { m_text = text; }
	void setFont(const std::string& font) { m_font = font; }
	void setColor(const SDL_Color& color) { m_color = color; }
	void on_render()override;
	void on_update(float delta)override;
private:
	SDL_Renderer* g_renderer = nullptr;
	SDL_Texture* dialog = nullptr, *title = nullptr;
	SDL_Point pos;
	std::string m_text;
	std::string m_font;
	SDL_Color m_color;
	SDL_Rect m_rect;
};

