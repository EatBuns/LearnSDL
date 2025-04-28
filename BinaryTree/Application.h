#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_audio.h>
#include <SDL_ttf.h>
#include "XNode.h"

class engineBorad
{
public:
	engineBorad(SDL_Renderer* r);
	~engineBorad() = default;

	void on_update(float delta);
	void on_render();
	void render_tree_view(XNode* node);
	void setApp_Node(XNode* n) { app_node = n; }

private:
	bool need_show_popup = false;
	float cur_delta = 0.0f, interval = 1000.0f, passtime = 0.0f;
	XNode* node_selected = nullptr, *app_node = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* targetTexture = nullptr;
	SDL_Texture* iconTexture = nullptr;
};

class Application : public XNode
{
public:
	static Application& GetInstance()
	{
		static Application instance;
		return instance;
	}
	~Application();
	void Run();
	void on_input(SDL_Event& e);


private:
	Application();
	void on_update(float delat) override;
	void on_render() override;

	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	SDL_Texture* testTexture;

	float m_delta;
	std::shared_ptr<engineBorad> borad = nullptr;
};


