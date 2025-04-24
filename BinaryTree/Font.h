#pragma once
#include <SDL_ttf.h>
#include <string>
#include <unordered_map>

#define FONT_TEXTURE_SIZE 512
#define NUM_GLYPHS 128
#define MAX_GLYPH_SIZE 8	//字体最大字节数
#define MAX_LINE_LENGTH 1200
#define MAX_WORD_LENGTH 600

typedef enum {
	TEXT_ALIGN_LEFT = 0,
	TEXT_ALIGN_CENTER,
	TEXT_ALIGN_RIGHT
}TEXT_ALIGN_TYPE;

class Font
{
public:
	static Font& instance();
	void SetFont(const std::string& path, const std::string& name, uint16_t ptSize);
	TTF_Font* GetFont(const std::string& name);
	int GetFontIndex(const std::string& name) { return FontsIndex[name]; }
	void setFontSize(const std::string& name, uint16_t ptSize);

	//绘制，需要提供词句，坐标，rgba,字符集
	void drawText(const char *str, int x, int y, int r, int g, int b, int a, const std::string& name);
	int drawTextWrapped(const char *str, int x, int y, int r, int g, int b, int a, const std::string& name, int aglin, int maxwidth, int dodraw);
	void drawTextLine(const char* str, int x, int y, int r, int g, int b, int a, int index, int align, int maxWidth);
	void calcTextDimensions(const char* str, int index, int* w,int* h);
	void shakeText(int strength, int duration);
	void setRender(SDL_Renderer* render);

	SDL_TimerID id;
	static int FontNUM;
private:
	Font();
	~Font();

	void initFont(const std::string& name);
	SDL_Texture* toTexture( SDL_Surface* surface);
	int nextGlyph(const char* str, int* i, char* glyphBuffer);

	//思路是：将一种字体的字形全部集中到一个纹理上，存储每个字形的宽高和坐标，渲染时，只需找到目标字形，即可渲染.
	//存储每个字体的TTF
	std::unordered_map<std::string, TTF_Font*> Fonts;
	//存储每种字体的固定字形位置坐标.
	//std::vector<std::vector<SDL_Rect>>	glyphs;
	std::vector<SDL_Texture*> fontTextures;
	std::unordered_map<std::string, int> FontsIndex;

	SDL_Rect glyphs[8][NUM_GLYPHS];
	SDL_Color white = { 255,255,255,0xff };
	SDL_Renderer* current_renderer = nullptr;
};