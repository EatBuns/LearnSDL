#include "Font.h"

int Font::FontNUM = 0;

Uint32 timeout(Uint32 interval, void* param)
{

	SDL_Log("timeout!");

	Font* f=  (Font*)param;
	SDL_RemoveTimer(f->id);

	return -1;
}

Font::Font()
{
	
}

Font::~Font()
{
	auto it = Fonts.begin();
	for (; it != Fonts.end(); ++it)
	{
		TTF_CloseFont(it->second);
	}
}

void Font::initFont(const std::string& name)
{
	int i;
	char c[2];	//存储字形
	//创建RGBA表面
	SDL_Surface *surface = SDL_CreateRGBSurface(0, FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE, 32, 0, 0, 0, 0);
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, 0, 0, 0, 0));	//设置字体key，就不会渲染了
	int index = FontsIndex[name];

	memset( &glyphs[index], 0, sizeof(SDL_Rect) * NUM_GLYPHS);
	SDL_Rect dest;
	dest.x = dest.y = 0;

	for (i = ' '; i <= 'z'; ++i)
	{
		c[0] = i;
		c[1] = 0;
		SDL_Surface* text = TTF_RenderUTF8_Blended(Fonts[name], c, white);
		TTF_SizeText(Fonts[name], c, &dest.w, &dest.h);
		
		if (dest.x + dest.w >= FONT_TEXTURE_SIZE)
		{
			dest.x = 0;
			dest.y += dest.h + 1;

			if (dest.h + dest.y >= FONT_TEXTURE_SIZE)
			{
				SDL_Log("Out of glyph space in %dx%d font atlas texture map.\n");
				exit(1);
			}
		}

		SDL_BlitSurface(text, NULL, surface, &dest);
		SDL_FreeSurface(text);

		SDL_Rect* g = &glyphs[index][i];
		g->x = dest.x;
		g->y = dest.y;
		g->w = dest.w;
		g->h = dest.h;

		dest.x += dest.w;
	}
	fontTextures.push_back(toTexture(surface));
	SDL_FreeSurface(surface);
}

SDL_Texture* Font::toTexture(SDL_Surface *surface)
{
	if (current_renderer)
		SDL_LogError(0, "Font init failed:renderer is NULL\n");
	return SDL_CreateTextureFromSurface(current_renderer, surface);
}

int Font::nextGlyph(const char* str, int* i, char* glyphBuffer)
{
	int len = 1;
	unsigned int bit;
 	bit = (unsigned char)str[*i];
	const char* p;

	if (bit < ' ')
		return 0;

	//四个字节
	if (bit >= 0xF0)
	{
		bit = (int)(str[*i] & 0x07) << 18;
		bit |= (int)(str[*i + 1] & 0x3F) << 12;
		bit |= (int)(str[*i + 2] & 0x3F) << 6;
		bit |= (int)(str[*i + 1] & 0x3F);
		len = 4;
	}
	//三个字节
	else if (bit >= 0xE0)
	{
		bit = (int)(str[*i] & 0x07) << 12;
		bit |= (int)(str[*i + 1] & 0x3F) << 12;
		bit |= (int)(str[*i + 2] & 0x3F) << 6;
		len = 3;
	}
	//2个字节
	else if (bit >= 0xC0)
	{
		bit = (int)(str[*i] & 0x07) << 6;
		bit |= (int)(str[*i + 1] & 0x3F);
		len = 2;
	}

	if (glyphBuffer != NULL)
	{
		p = str + *i;
		memset(glyphBuffer, 0, MAX_GLYPH_SIZE);
		memcpy(glyphBuffer, str, len);
	}
	
	*i = *i + len;

	return bit;		//返回unicode码点
}

Font& Font::instance()
{
	static Font instance;
	return instance;
}

void Font::SetFont(const std::string& path, const std::string& name, uint16_t ptSize)
{
	TTF_Font* font = TTF_OpenFont(path.c_str(), ptSize);
	if (!font)
	{
		SDL_Log("Font Constructor failed\n");
		return;
	}
	//TTF_SetFontHinting(font, TTF_HINTING_LIGHT);
	Fonts.insert(std::make_pair(name, font));
	FontsIndex.insert(std::make_pair(name, FontNUM++));
	initFont(name);
}

TTF_Font* Font::GetFont(const std::string& name)
{
	return Fonts[name];
}

void Font::setFontSize(const std::string& name, uint16_t ptSize)
{
	TTF_SetFontSize(Fonts[name], ptSize);
}

void Font::drawText(const char* str, int x, int y, int r, int g, int b, int a, const std::string& name)
{
	//绘制到目标坐标
	SDL_Rect dest;
	//字符集下标
	int index = FontsIndex[name] , c, i = 0;
	const char* p = str;

	SDL_SetTextureColorMod(fontTextures[index], r, g, b);
	SDL_SetTextureAlphaMod(fontTextures[index], a);

	c = str[i++];

	while (c)
	{
		dest.x = x;
		dest.y = y;
		dest.w = glyphs[index][c].w;
		dest.h = glyphs[index][c].h;

		if(current_renderer)
			SDL_RenderCopy(current_renderer, fontTextures[index], &glyphs[index][c], &dest);
		else
			SDL_Log("Font Can't Find Renderer!\n");

		x += glyphs[index][*p].w;
		c = str[i++];
	}
}

int Font::drawTextWrapped(const char* str, int x, int y, int r, int g, int b, int a, const std::string& name, int align, int maxwidth, int dodraw)
{
	char word[MAX_WORD_LENGTH], line[MAX_LINE_LENGTH];

	memset(word, 0, MAX_WORD_LENGTH);
	memset(line, 0, MAX_LINE_LENGTH);

	SDL_Rect rect_pos;
	rect_pos.x = x;
	rect_pos.y = y;
	rect_pos.w = maxwidth;
	rect_pos.h = maxwidth / 2;
	SDL_RenderFillRect(current_renderer, &rect_pos);
	SDL_RenderDrawRect(current_renderer, &rect_pos);

	int i = 0, c = 0,  len = 0, n = 0, index = FontsIndex[name];
	int wordWidth = 0, lineWidth = 0;

	c = str[i++];
	len = strlen(str);
	while (c)
	{
		wordWidth += glyphs[index][c].w;

		if (c != ' ')
		{
			word[n++] = c;
		}

		if (c == ' ' || i == len)
		{
			if (lineWidth + wordWidth >= maxwidth)
			{
				if (dodraw)
				{
					drawTextLine(line, x, y, r, g, b,a, index, align, maxwidth);
				}

				memset(line, 0, MAX_LINE_LENGTH);

				y += glyphs[index][' '].h;

				lineWidth = 0;
			}
			else if (lineWidth != 0)
			{
				strcat_s(line, " ");
			}

			strcat_s(line, word);

			lineWidth += wordWidth;

			memset(word, 0, MAX_WORD_LENGTH);

			wordWidth = 0;
			n = 0;
		}

		c = str[i++];
	}
	if (dodraw)
	{
		drawTextLine(line, x, y, r, g, b,a, index, align, maxwidth);
	}

	return y + glyphs[index][' '].h;
}

void Font::drawTextLine(const char* str, int x, int y, int r, int g, int b, int a, int index, int align,int maxWidth)
{
	int i, character, w, h;
	SDL_Rect* glyph, dest;

	if (align != TEXT_ALIGN_LEFT)
	{
		calcTextDimensions(str, index, &w, &h);

		if (align == TEXT_ALIGN_CENTER)
		{
			x += (maxWidth - w) / 2;
		}
		else if (align == TEXT_ALIGN_RIGHT)
		{
			x += maxWidth - w;
		}
	}

	SDL_SetTextureColorMod(fontTextures[index], r, g, b);

	i = 0;

	character = str[i++];

	while (character)
	{
		glyph = &glyphs[index][character];

		dest.x = x;
		dest.y = y;
		dest.w = glyph->w;
		dest.h = glyph->h;

		if (current_renderer)
			SDL_RenderCopy(current_renderer, fontTextures[index], glyph, &dest);
		else
			SDL_Log("Font Can't Find Renderer!\n");

		x += glyph->w;

		character = str[i++];
	}
}

void Font::calcTextDimensions(const char* str, int index, int* w, int* h)
{
	int i, character;
	SDL_Rect* g;

	*w = *h = 0;

	i = 0;

	character = str[i++];

	while (character)
	{
		g = &glyphs[index][character];

		*w += g->w; 
		*h = std::max(g->h, *h);

		character = str[i++];
	}
}

void Font::shakeText(int strength, int intreval)
{
	id = SDL_AddTimer(intreval, timeout, this);
}

void Font::setRender(SDL_Renderer* render)
{
	current_renderer = render;
}
