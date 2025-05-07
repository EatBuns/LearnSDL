#include "util.h"

void GameEngine2D::createGrayTexture(const SDL_Surface* source_surface, SDL_Surface** dst_surface)
{
	if (SDL_LockSurface((SDL_Surface*)source_surface) != 0) {
		printf("锁定失败: %s\n", SDL_GetError());
		return;
	}

	*dst_surface = SDL_CreateRGBSurfaceWithFormat(
		0,                                      // 标志位
		source_surface->w,                      // 宽度
		source_surface->h,                      // 高度
		source_surface->format->BitsPerPixel,   // 每像素位数
		source_surface->format->format          // 像素格式
	);

	SDL_BlitSurface((SDL_Surface*)source_surface, NULL, (SDL_Surface*)(*dst_surface), NULL);

	if (*dst_surface == NULL)
	{
		printf("创建surface失败: %s\n", SDL_GetError());
		return;
	}

	Uint32* src_pixels = (Uint32*)source_surface->pixels;
	Uint32* dst_pixels = (Uint32*)(*dst_surface)->pixels;

	int width = source_surface->w;
	int height = source_surface->h;

	for (int i = 0; i < width * height; i++) {
		Uint8 r, g, b, a;
		SDL_GetRGBA(src_pixels[i], (*dst_surface)->format, &r, &g, &b, &a);
		Uint8 gray = (Uint8)(0.299 * r + 0.587 * g + 0.114 * b);
		dst_pixels[i] = SDL_MapRGBA(source_surface->format, gray, gray, gray, a);
	}

	SDL_UnlockSurface((SDL_Surface*)source_surface);
}

float GameEngine2D::linear_interpolate_float(float start, float end, float t)
{
	if (t < 0) t = 0;
	if (t > 1) t = 1;
	return start + (end - start) * t;
}
