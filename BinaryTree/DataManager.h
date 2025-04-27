#pragma once

#include <unordered_map>
#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include "util.h"

using namespace GameEngine2D;

class loadImpl
{
public:
	loadImpl() = default;
	~loadImpl() = default;
	virtual vecsInfo loadResourcefile(const std::string& path) = 0;
	virtual void loadDatafile(const std::string& path) = 0;
};

class loadJson :public loadImpl
{
public:
	loadJson() = default;
	~loadJson() = default;
	vecsInfo loadResourcefile(const std::string& path) override;
	void loadDatafile(const std::string& path)override;
};

class loadXml :public loadImpl
{
public:
	loadXml() = default;
	~loadXml() = default;
	vecsInfo loadResourcefile(const std::string& path) override;
	void loadDatafile(const std::string& path)override;
};

class DataManager
{
public:
	static DataManager& GetInstance()
	{
		static DataManager instance;
		return instance;
	}
	DataManager(const DataManager&) = delete;
	DataManager& operator=(const DataManager&) = delete;
	void loadResource(loadImpl *impl, const std::string& path, SDL_Renderer* renderer);
	void loadData(loadImpl* impl, const std::string& path);
	std::shared_ptr<SDL_Texture>& findImage(const std::string& name){ return gResources[name]; }
	std::vector<std::shared_ptr<SDL_Texture>>& findAtlas(const std::string& name) { return gAtlas[name]; }
	std::shared_ptr<SDL_Texture>& findAtlasIndex(const std::string& name, int index);
	NodeStatus& findMonster(const std::string& name) { return gMonsters[name]; }
	PlayerStatus& getPlayerState() { return gPlayerStatus; }
	std::string getFontPath(const std::string& name);
	
private:
	DataManager() = default;
	~DataManager() = default;

	std::unordered_map<std::string, std::shared_ptr<SDL_Texture> > gResources;
	std::unordered_map<std::string, std::vector<std::shared_ptr<SDL_Texture>>> gAtlas;
	std::unordered_map<std::string, NodeStatus> gMonsters;
	PlayerStatus gPlayerStatus;
	vecsInfo srcPath;
};

