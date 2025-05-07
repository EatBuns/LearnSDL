#include "Application.h"
#include "DataManager.h"
#include "Animation.h"
#include "Font.h"
#include "xplayer.h"
#include "xSlime.h"
#include "TalkDialog.h"
#include "CollisionManager.h"
#include <imgui.h>
#include <imgui_impl_sdlrenderer.h>
#include <imgui_impl_sdl.h>
#include <imgui_stdlib.h>
#include <chrono>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

using namespace std;
using namespace std::chrono;
xplayer* main_player;
SDL_Texture* grayTexure;

#define WINDOW_W 800
#define WINDOW_H 600

Application::~Application()
{
	ImGui_ImplSDL2_Shutdown();
	//ImGui_ImplSDLRenderer_Shutdown();

	ImGui::DestroyContext();
	TTF_Quit();		//字体
	IMG_Quit();		//图片
	//Mix_Quit();		//音乐
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Application::Run()
{
	time_point<steady_clock> last_point;
	const nanoseconds frame_time(1000000000/60);
	last_point = steady_clock::now();

	bool quit = false;
	while (!quit)
	{
		SDL_Event e;
		if (SDL_PollEvent(&e))
		{
			ImGui_ImplSDL2_ProcessEvent(&e);
			on_input(e);
		}

		if (main_player->getPosition().x - DataManager::GetInstance().m_camera.CameraRect.x > 250)
		{
			DataManager::GetInstance().m_camera.movePos(0.01f, 0);
		}

		auto frame_start = steady_clock::now();
		auto delta = frame_start - last_point;

		m_delta = duration<float, milli>(delta).count();
		
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		
		CollisionManager::instance().checkCollision();
		this->on_update(m_delta);
		CollisionManager::instance().on_update(m_delta);
		
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255); // 设置绘制颜色为黑色
		SDL_RenderClear(m_renderer);
		this->on_render();
		
		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

		SDL_RenderPresent(m_renderer);

		last_point = frame_start;
		auto duration = frame_time - (steady_clock::now() - frame_start);
		if (duration > nanoseconds(0))
		{
			SDL_Delay(duration_cast<milliseconds>(duration).count());
		}
	}
}

void Application::on_input(SDL_Event& e)
{
	for (auto& child : x_childs)
	{
		if (child->isNeedInput())
		{
			auto p = static_cast<xplayer*> (child.get());
			p->on_input(e);
		}
	}
}

Application::Application() :m_renderer(NULL), m_window(NULL),testTexture(NULL)
{
	setXName("App");
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	m_window = SDL_CreateWindow("BinaryTree", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_OPENGL);
	if (m_window == NULL)
	{
		cout << SDL_GetError();
		return;
	}
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);

	if(TTF_Init() < 0)
		SDL_Log("TTF_Init failed!\n");

	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer);
	ImGui_ImplSDLRenderer_Init(m_renderer);

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	style.FrameBorderSize = 1.0f;
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
	style.FrameRounding = 2.0f;
	style.FrameBorderSize = 1.0f;

	SDL_RendererInfo m_driverinfo;
	SDL_GetRenderDriverInfo(SDL_GetNumRenderDrivers(), &m_driverinfo);

	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

	auto resPath = getAbsolutePath("resLoad/res.json");
	loadJson ljson;
	DataManager::GetInstance().loadResource(&ljson, resPath,m_renderer);
	resPath = getAbsolutePath("resLoad/val.json");
	DataManager::GetInstance().loadData(&ljson, resPath);

	Font::instance().setRender(m_renderer);
	Font::instance().SetFont(DataManager::GetInstance().getFontPath("SarasaMonoSC"), "SarasaMonoSC", 16);
	Font::instance().SetFont(DataManager::GetInstance().getFontPath("IPix"), "IPix", 16);

	resPath = getAbsolutePath("resource/Platform.png");
	testTexture = IMG_LoadTexture(m_renderer, resPath.data());
	if (!testTexture)
	{
		SDL_Log("err,testTexture load failed!\n");
	}

#if 1
	auto path = getAbsolutePath("resource\\testIcon.png");
	SDL_Surface *sur = IMG_Load(path.data());
	SDL_Surface* dst;
	createGrayTexture(sur, &dst);
	grayTexure = SDL_CreateTextureFromSurface(m_renderer, dst);


	SDL_FPoint p;
	p.x = 100.0f;
	p.y = 100.0f;
	auto player = std::make_shared<xplayer>(m_renderer, Animation::AnimationAnchor::bottom, DMInstance.getPlayerState().status.vx_s, DMInstance.getPlayerState().status);
	player->setPosition(p);
	player->setActualH(60);
	player->setActualW(50);
	main_player = player.get();

	Body* b = new WANGBA_Body("wangba_body", m_renderer, ItemID::BODY_WANGBA);
	Head* h = new WANGBA_Head("wangba_head", m_renderer, ItemID::HEAD_WANGBA);
	Legs* l = new WANGBA_Legs("wangba_legs", m_renderer, ItemID::LEGS_WANGBA);
	auto box = CollisionManager::instance().createBox();
	box->setcolor(150, 50, 200, 150);
	box->setGravityEnable(true);
	box->setSrcLayer(CollisionBox::CollissionLayer::layer0);
	box->setDstLayer(CollisionBox::CollissionLayer::layer1, true);
	box->setDstLayer(CollisionBox::CollissionLayer::layer2, true);
	box->setDstLayer(CollisionBox::CollissionLayer::layer3, true);
	box->setResetCb(std::bind(&xplayer::resetFunc, player.get()));
	box->setCallBack(std::bind(&xplayer::on_CollisionCb, player.get(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	box->setNodeName("player");
	player->setCollisionBox(box);
	player->setSuit(h, b, l);

	p.x = 200.0f;
	p.y = 200.0f;
	auto slime = std::make_shared<xSlime>(m_renderer, Animation::AnimationAnchor::bottom, DMInstance.findMonster("slime").vx_s, DataManager::GetInstance().findMonster("slime"));
	slime->setPosition(p);
	slime->setActualH(20);
	slime->setActualW(20);

	auto box1 = CollisionManager::instance().createBox();
	box1->setcolor(200, 100, 200, 150);
	box1->setGravityEnable(true);
	box1->setSrcLayer(CollisionBox::CollissionLayer::layer2);
	box1->setDstLayer(CollisionBox::CollissionLayer::layer4, true);
	box1->setDstLayer(CollisionBox::CollissionLayer::layer1, true);
	box1->setResetCb(std::bind(&xSlime::resetFunc, slime.get()));
	box1->setCallBack(std::bind(&xSlime::on_CollisionCb, slime.get(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	box1->setNodeName("slime");
	slime->setCollisionBox(box1);

	auto platform = CollisionManager::instance().createBox();
	platform->setcolor(10, 10, 200, 200);
	platform->setSrcLayer(CollisionBox::CollissionLayer::layer1);
	platform->setPosition(-50, 550);
	platform->setSize(2900, 50);

	platform = CollisionManager::instance().createBox();
	platform->setcolor(200, 200, 200, 200);
	platform->setSrcLayer(CollisionBox::CollissionLayer::layer1);
	platform->setPosition(400, 500);
	platform->setSize(100, 100);

	platform = CollisionManager::instance().createBox();
	platform->setcolor(200, 200, 200, 200);
	platform->setSrcLayer(CollisionBox::CollissionLayer::layer1);
	platform->setPosition(0, 500);
	platform->setSize(100, 100);

	platform = CollisionManager::instance().createBox();
	platform->setcolor(200, 50, 10, 200);
	platform->setDstLayer(CollisionBox::CollissionLayer::layer0, true);
	platform->setSrcLayer(CollisionBox::CollissionLayer::layer3);
	platform->setPosition(200, 500);
	platform->setSize(20, 20);
	//const std::function<void(int, int, SDL_FRect)> cb = [platform](int layer, int collisionSide, SDL_FRect rect) {
	//	platform->setEnable(false);
	//	//CollisionManager::instance().destroyBox(platform);
	//};
	//platform->setCallBack(cb);

	addChild(player);
	addChild(slime);
#endif 

	borad = std::make_shared< engineBorad>(m_renderer);
	borad->setApp_Node(this);
}

void Application::on_update(float delat)
{
	borad->on_update(delat);

	for (auto& child : x_childs)
	{
		child->on_update(delat);
	}
}

void Application::on_render()
{
	borad->on_render();

	/*char buf[32];
	memset(buf, 0, 32);
	sprintf_s(buf, 32, "delta:%.2f\n", m_delta);
	Font::instance().setRender(m_renderer);
	Font::instance().drawText(buf, 0, 0, 255, 255, 255, 255, "SarasaMonoSC");*/
}

engineBorad::engineBorad(SDL_Renderer* r):renderer(r)
{
	targetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, 800 * 0.6, 600);
	iconTexture = DataManager::GetInstance().findImage("testIcon").get();
}

void engineBorad::on_update(float delta)
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

#if 0
	static bool my_tool_active = true;
	static float my_color[4];

	ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
			if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// Edit a color stored as 4 floats
	ImGui::ColorEdit4("Color", my_color);

	SDL_Log("ImGui::GetTime():%f\n", ImGui::GetTime());
	// Generate samples and plot them
	float samples[100];
	for (int n = 0; n < 100; n++)
		samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
	ImGui::PlotLines("Samples", samples, 100);

	// Display contents in a scrolling region
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
	ImGui::End();

	static bool isOpen = true;
	ImGui::Begin("My First Tool", &isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	static bool isShow = false;
	if (ImGui::Button(u8"clicked"))
	{
		isShow = !isShow;
	}

	if (isShow)
	{
		// Begin a child window with scrollbars enabled
		if (ImGui::BeginChild("ChildWindow", { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y }, true, ImGuiWindowFlags_HorizontalScrollbar)) {
			ImGui::Text("This is inside the child window.");
			ImGui::Button("Button in Child");

			int w, h;
			//SDL_QueryTexture(testTexture, NULL, NULL, &w, &h);

			// Use ImGui::Image to display the texture, allowing the child window to scroll if the image is larger than the available space
			//ImGui::Image(testTexture, { (float)w, (float)h });
		}
		ImGui::EndChild();
	}

	ImGuiTreeNodeFlags baseflags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	if (ImGui::TreeNodeEx("ddasdsadsda", baseflags))
	{
		ImGui::Text("vsynv");
		//ImGui::Checkbox("VSync", &need_show_popup);
		ImGui::TreePop();
	}

	if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		need_show_popup = true;

	if (need_show_popup)
	{
		ImGui::OpenPopup("context menu");
		need_show_popup = false;
	}


	if (ImGui::BeginPopup("context menu"))
	{
		if (ImGui::BeginMenu("addNode"))
		{
			ImGui::MenuItem("label1");
			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
	ImGui::End();
#endif
	
	ImGui::Begin("Test");

	passtime += delta;
	if (passtime >= interval)
	{
		cur_delta = delta;
		passtime = 0.0f;
	}
	ImGui::Text("delta:%.1f", cur_delta);

	int playerHP = -1;
	float speed = 0;
	for (auto& child : app_node->get_child_list())
	{
		xplayer* p = nullptr;
		if ( (p = dynamic_cast<xplayer*>(child.get())) != nullptr)
		{
			playerHP = p->getHP();
			speed = p->getUpVx();
			break;
		}
	}

	ImVec2 avail = ImGui::GetContentRegionAvail();
	float tree_width = avail.x * 0.2f;
	float scene_width = avail.x * 0.6f;
	float inspector_width = avail.x * 0.2f;
	{
		ImGui::BeginChild("tree_view", { tree_width,ImGui::GetContentRegionAvail().y },
			ImGuiChildFlags_Border, ImGuiWindowFlags_AlwaysHorizontalScrollbar);
		render_tree_view(app_node);
		ImGui::EndChild();
	}
	ImGui::SameLine();
	{
		ImGui::BeginChild("scene_view", { scene_width,ImGui::GetContentRegionAvail().y }, ImGuiChildFlags_Border);
		ImGui::Image(targetTexture, { scene_width,ImGui::GetContentRegionAvail().y });
		ImGui::EndChild();
	}
	ImGui::SameLine();
	{
		ImGui::BeginChild("inspector", { inspector_width,ImGui::GetContentRegionAvail().y }, ImGuiChildFlags_Border);
		ImGui::Text("player hp:%d\n",playerHP);
		ImGui::Text("player speed:%.2f\n", speed);
		ImGui::EndChild();
	}
	ImGui::End();
}

void engineBorad::on_render()
{
	// Create a window called "My First Tool", with a menu bar.
	SDL_SetRenderTarget(renderer, targetTexture);
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	SDL_RenderClear(renderer);

	int w, h;
	
	SDL_QueryTexture(iconTexture, NULL, NULL, &w, &h);

	SDL_Rect src{ 0,0,w,h };
	SDL_Rect dst{ 20,20,w,h };
	/*typedef enum
{
    SDL_BLENDMODE_NONE = 0x00000000,     /**< no blending
                                              dstRGBA = srcRGBA 
	SDL_BLENDMODE_BLEND = 0x00000001,    /**< alpha blending
											  dstRGB = (srcRGB * srcA) + (dstRGB * (1-srcA))
											  dstA = srcA + (dstA * (1-srcA)) 
		SDL_BLENDMODE_ADD = 0x00000002,      /**< additive blending
												  dstRGB = (srcRGB * srcA) + dstRGB
												  dstA = dstA 
		SDL_BLENDMODE_MOD = 0x00000004,      /**< color modulate
												  dstRGB = srcRGB * dstRGB
												  dstA = dstA 
		SDL_BLENDMODE_MUL = 0x00000008,      /**< color multiply
												  dstRGB = (srcRGB * dstRGB) + (dstRGB * (1-srcA))
												  dstA = dstA 
		SDL_BLENDMODE_INVALID = 0x7FFFFFFF

		/* Additional custom blend modes can be returned by SDL_ComposeCustomBlendMode() 

} SDL_BlendMode; */
	SDL_SetTextureBlendMode(iconTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(iconTexture, 200);
	SDL_SetTextureColorMod(iconTexture, 128, 128, 128);
	//sdl_settextu
	SDL_RenderCopy(renderer, iconTexture, &src, &dst);

	int ssize = 50;
	dst.x += ssize;
	SDL_SetTextureBlendMode(iconTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(iconTexture, 255);
	SDL_SetTextureColorMod(iconTexture, 0, 255, 0);
	SDL_RenderCopy(renderer, iconTexture, &src, &dst);

	dst.x += ssize;
	SDL_SetTextureBlendMode(iconTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(iconTexture, 255);
	SDL_SetTextureColorMod(iconTexture, 0, 0, 255);
	SDL_RenderCopy(renderer, iconTexture, &src, &dst);

	dst.x += ssize;
	SDL_SetTextureBlendMode(iconTexture, SDL_BLENDMODE_MUL);
	SDL_SetTextureAlphaMod(iconTexture, 255);
	SDL_RenderCopy(renderer, iconTexture, &src, &dst);

	dst.x += ssize;
	SDL_SetTextureBlendMode(grayTexure, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(renderer, grayTexure, &src, &dst);


	CollisionManager::instance().on_render(renderer);
	for (auto& child : app_node->get_child_list())
	{
		child->on_render();
	}

	SDL_SetRenderTarget(renderer, nullptr);
}

void engineBorad::render_tree_view(XNode* node)
{
	static const ImVec2 size_icon = { ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight() };
	static const ImGuiTreeNodeFlags base_flag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	bool is_open = false;
	bool has_child = node->has_child();

	if (!has_child)
	{
		ImGui::TreeNodeEx((void*)node->get_name().c_str(), base_flag
			| ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen |
			((node_selected == node) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None), "");
	}
	else
	{
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		is_open = ImGui::TreeNodeEx((void*)node->get_name().c_str(), base_flag |
			((node_selected == node) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None), "");
	}

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right))
		node_selected = node;

	if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		need_show_popup = true;

	ImGui::SameLine();
	ImGui::TextUnformatted(node->get_name().c_str());

	if (has_child && is_open)
	{
		for (auto& child : node->get_child_list())
			render_tree_view(child.get());

		ImGui::TreePop();
	}
}
