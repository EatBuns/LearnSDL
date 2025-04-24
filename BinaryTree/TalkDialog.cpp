#include "TalkDialog.h"

TalkDialog::TalkDialog(SDL_Renderer* renderer):g_renderer(renderer)
{
	dialog = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 800, 100);
	title = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 500, 20);
	//SDL_SetTextureColorMod(title, 0, 0, 100);
	SDL_SetTextureBlendMode(dialog, SDL_BLENDMODE_BLEND);

	pos.x = 200;
	pos.y = 200;

	m_rect.x = 200;
	m_rect.y = 200;

	m_color.r = 255;
	m_color.g = 255;
	m_color.b = 255;
	m_color.a = 255;
}

TalkDialog::~TalkDialog()
{
}

void TalkDialog::on_render()
{
	if (g_renderer == nullptr || title == nullptr || dialog == nullptr)
		return;

	SDL_SetRenderTarget(g_renderer, title);
	SDL_SetRenderDrawColor(g_renderer, 65, 65, 150, 255);
	SDL_RenderClear(g_renderer) ;

	const char* str = "this is a test text!";
	Font::instance().setRender(g_renderer);
	Font::instance().drawText(str, 0,5, m_color.r, m_color.g, m_color.b, m_color.a, "IPix");

	int w, h;
	Font::instance().calcTextDimensions(str, Font::instance().GetFontIndex("IPix"), &w, &h);
	SDL_SetRenderTarget(g_renderer, nullptr);
	
	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = w;
	src.h = 20;

	m_rect.w = 500;
	m_rect.h = 20;
	SDL_RenderCopy(g_renderer, title, &src, &m_rect);
}

void TalkDialog::on_update(float delta)
{

}
