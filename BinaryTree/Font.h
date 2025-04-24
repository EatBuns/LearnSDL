#pragma once
#include <SDL_ttf.h>
#include <string>
#include <unordered_map>

#define FONT_TEXTURE_SIZE 512
#define NUM_GLYPHS 128
#define MAX_GLYPH_SIZE 8	//��������ֽ���
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

	//���ƣ���Ҫ�ṩ�ʾ䣬���꣬rgba,�ַ���
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

	//˼·�ǣ���һ�����������ȫ�����е�һ�������ϣ��洢ÿ�����εĿ�ߺ����꣬��Ⱦʱ��ֻ���ҵ�Ŀ�����Σ�������Ⱦ.
	//�洢ÿ�������TTF
	std::unordered_map<std::string, TTF_Font*> Fonts;
	//�洢ÿ������Ĺ̶�����λ������.
	//std::vector<std::vector<SDL_Rect>>	glyphs;
	std::vector<SDL_Texture*> fontTextures;
	std::unordered_map<std::string, int> FontsIndex;

	SDL_Rect glyphs[8][NUM_GLYPHS];
	SDL_Color white = { 255,255,255,0xff };
	SDL_Renderer* current_renderer = nullptr;
};