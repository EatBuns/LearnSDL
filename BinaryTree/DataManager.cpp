#include "DataManager.h"

#include <cJSON.h>
#include <tuple>

vecsInfo loadJson::loadResourcefile(const std::string& path)
{
	vecsInfo ret;
	FILE* fp = nullptr;
#ifdef _WIN32
	fopen_s(&fp, path.data(), "r");
#else
	fp = fopen(path.data(), "r");
#endif // 

	if (fp == nullptr)
	{
		return ret;
	}
	
	fseek(fp, 0, SEEK_END);
	long len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* data = new char[len + 1];
	memset(data, 0, len + 1);
	fread(data, 1, len, fp);

	fclose(fp);

	cJSON *root = cJSON_Parse(data);
	if (root == nullptr)
	{
		delete[] data;
		return ret;
	}

	std::vector<AtlasInfo> atinfo;
	cJSON* atlas = cJSON_GetObjectItemCaseSensitive(root, "atlas");
	if (cJSON_IsObject(atlas)) {
		cJSON* atlas_item = NULL;
		cJSON_ArrayForEach(atlas_item, atlas) {
			AtlasInfo info;
			const char* atlas_name = atlas_item->string;
			printf("Atlas name: %s\n", atlas_name);
			info.name = atlas_name;

			if (cJSON_IsArray(atlas_item)) {
				int array_size = cJSON_GetArraySize(atlas_item);
				for (int i = 0; i < array_size; i++) {
					cJSON* item = cJSON_GetArrayItem(atlas_item, i);
					if (cJSON_IsString(item) && item->valuestring != NULL) {
						auto path = getAbsolutePath(item->valuestring);
						printf("  Image path %d: %s\n", i + 1, path.data());
						info.files.push_back(path);
					}
				}
			}
			atinfo.emplace_back(info);
		}
	}

	// 解析 "image" 对象
	std::vector<otherSrcInfo> iminfo;
	cJSON* image = cJSON_GetObjectItemCaseSensitive(root, "image");
	if (cJSON_IsObject(image)) {
		cJSON* image_item = NULL;
		cJSON_ArrayForEach(image_item, image) {
			otherSrcInfo info;
			const char* image_name = image_item->string;
			info.name = image_name;
			if (cJSON_IsString(image_item) && image_item->valuestring != NULL) {
				auto path = getAbsolutePath(image_item->valuestring);
				printf("Image name: %s, Image path: %s\n", image_name, path.data());
				info.files = path;
			}
			iminfo.emplace_back(info);
		}
	}

	// 解析 "ttf" 对象
	std::vector<otherSrcInfo> ttfinfo;
	cJSON* ttf = cJSON_GetObjectItemCaseSensitive(root, "ttf");
	if (cJSON_IsObject(ttf)) {
		cJSON* image_item = NULL;
		cJSON_ArrayForEach(image_item, ttf) {
			otherSrcInfo info;
			const char* image_name = image_item->string;
			info.name = image_name;
			if (cJSON_IsString(image_item) && image_item->valuestring != NULL) {
				auto path = getAbsolutePath(image_item->valuestring);
				printf("ttf name: %s, ttf path: %s\n", image_name, path.data());
				info.files = path;
			}
			ttfinfo.emplace_back(info);
		}
	}
	
	cJSON_Delete(root);

	ret.atlas = std::move(atinfo);
	ret.otherInfos["image"] = std::move(iminfo);
	ret.otherInfos["ttf"] = std::move(ttfinfo);
	delete[] data;
	return ret;
}

void loadJson::loadDatafile(const std::string& path, std::unordered_map<std::string, NodeStatus>& nodeStatus, PlayerStatus& playerStatus)
{
	FILE* fp = nullptr;
#ifdef _WIN32
	fopen_s(&fp, path.data(), "r");
#else
	fp = fopen(path.data(), "r");
#endif // 

	if (fp == nullptr)
	{
		return;
	}

	fseek(fp, 0, SEEK_END);
	long len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* data = new char[len + 1];
	memset(data, 0, len + 1);
	fread(data, 1, len, fp);

	fclose(fp);

	cJSON* root = cJSON_Parse(data);
	if (root == nullptr)
	{
		delete[] data;
		return;
	}

	cJSON* monsters = cJSON_GetObjectItemCaseSensitive(root, "monsters");
	if (cJSON_IsArray(monsters))
	{
		int array_size = cJSON_GetArraySize(monsters);
		NodeStatus tNode;
		for (int i = 0; i < array_size; i++) {
			cJSON* monster = cJSON_GetArrayItem(monsters, i);
			cJSON* name = cJSON_GetObjectItemCaseSensitive(monster, "name");
			cJSON* health_points = cJSON_GetObjectItemCaseSensitive(monster, "health_points");
			cJSON* physical_attack = cJSON_GetObjectItemCaseSensitive(monster, "physical_attack");
			cJSON* physical_defense = cJSON_GetObjectItemCaseSensitive(monster, "physical_defense");
			cJSON* magical_attack = cJSON_GetObjectItemCaseSensitive(monster, "magical_attack");
			cJSON* magical_defense = cJSON_GetObjectItemCaseSensitive(monster, "magical_defense");
			cJSON* mana = cJSON_GetObjectItemCaseSensitive(monster, "mana");
			cJSON* speed = cJSON_GetObjectItemCaseSensitive(monster, "speed");

			if (cJSON_IsString(name) && cJSON_IsNumber(health_points) && cJSON_IsNumber(physical_attack) &&
				cJSON_IsNumber(physical_defense) && cJSON_IsNumber(magical_attack) && cJSON_IsNumber(magical_defense) &&
				cJSON_IsNumber(mana) && cJSON_IsNumber(speed)) {
				printf("Monster %d:\n", i + 1);
				printf("  Name: %s\n", name->valuestring);
				printf("  Health Points: %d\n", (int)health_points->valuedouble);
				printf("  Physical Attack: %d\n", (int)physical_attack->valuedouble);
				printf("  Physical Defense: %d\n", (int)physical_defense->valuedouble);
				printf("  Magical Attack: %d\n", (int)magical_attack->valuedouble);
				printf("  Magical Defense: %d\n", (int)magical_defense->valuedouble);
				printf("  Mana: %d\n", (int)mana->valuedouble);
				printf("  Speed: %d\n", (int)speed->valuedouble);

				tNode.hp = (int)health_points->valuedouble;
				tNode.phy_atk = (int)physical_attack->valuedouble;
				tNode.mag_atk = (int)magical_attack->valuedouble;
				tNode.phy_def = (int)physical_defense->valuedouble;
				tNode.mag_def = (int)magical_defense->valuedouble;
				tNode.mana = (int)mana->valuedouble;
				tNode.vx_s = (int)speed->valuedouble;

				nodeStatus[name->valuestring] = tNode;
			}
		}
	}

	cJSON* player = cJSON_GetObjectItemCaseSensitive(root, "player");
	cJSON* name = cJSON_GetObjectItemCaseSensitive(player, "name");
	cJSON* health_points = cJSON_GetObjectItemCaseSensitive(player, "health_points");
	cJSON* physical_attack = cJSON_GetObjectItemCaseSensitive(player, "physical_attack");
	cJSON* physical_defense = cJSON_GetObjectItemCaseSensitive(player, "physical_defense");
	cJSON* magical_attack = cJSON_GetObjectItemCaseSensitive(player, "magical_attack");
	cJSON* magical_defense = cJSON_GetObjectItemCaseSensitive(player, "magical_defense");
	cJSON* mana = cJSON_GetObjectItemCaseSensitive(player, "mana");
	cJSON* speed = cJSON_GetObjectItemCaseSensitive(player, "speed");

	if (cJSON_IsString(name) && cJSON_IsNumber(health_points) && cJSON_IsNumber(physical_attack) &&
		cJSON_IsNumber(physical_defense) && cJSON_IsNumber(magical_attack) && cJSON_IsNumber(magical_defense) &&
		cJSON_IsNumber(mana) && cJSON_IsNumber(speed)) {
		printf("Player:\n");
		printf("  Name: %s\n", name->valuestring);
		printf("  Health Points: %d\n", (int)health_points->valuedouble);
		printf("  Physical Attack: %d\n", (int)physical_attack->valuedouble);
		printf("  Physical Defense: %d\n", (int)physical_defense->valuedouble);
		printf("  Magical Attack: %d\n", (int)magical_attack->valuedouble);
		printf("  Magical Defense: %d\n", (int)magical_defense->valuedouble);
		printf("  Mana: %d\n", (int)mana->valuedouble);
		printf("  Speed: %d\n", (int)speed->valuedouble);
		
		NodeStatus tNode;
		tNode.hp = (int)health_points->valuedouble;
		tNode.phy_atk = (int)physical_attack->valuedouble;
		tNode.mag_atk = (int)magical_attack->valuedouble;
		tNode.phy_def = (int)physical_defense->valuedouble;
		tNode.mag_def = (int)magical_defense->valuedouble;
		tNode.mana = (int)mana->valuedouble;
		tNode.vx_s = (int)speed->valuedouble;
		playerStatus.status = tNode;
	}

	cJSON_Delete(root);
}

vecsInfo loadXml::loadResourcefile(const std::string& path)
{
	vecsInfo ret;
	return ret;
}

void loadXml::loadDatafile(const std::string& path, std::unordered_map<std::string, NodeStatus>& node, PlayerStatus& playerStatus)
{

}

void DataManager::loadResource(loadImpl* impl, const std::string& path, SDL_Renderer* renderer)
{
	srcPath = std::move(impl->loadResourcefile(path));
	
	for (auto& atlas : srcPath.atlas)
	{
		std::vector<std::shared_ptr<SDL_Texture>> textures;
		for (auto& file : atlas.files)
		{
			SDL_Surface* surface = IMG_Load(file.data());
			if (surface == nullptr)
			{
				continue;
			}
			std::shared_ptr<SDL_Texture> texture(SDL_CreateTextureFromSurface(renderer, surface), SDL_DestroyTexture);
			SDL_FreeSurface(surface);
			if (texture == nullptr)
			{
				continue;
			}
			textures.emplace_back(texture);
		}
		gAtlas[atlas.name] = textures;
	}

	for (auto& other : srcPath.otherInfos)
	{
		auto name = other.first;
		auto& infos = other.second;
		if (name == "image")
		{
			for (auto& info : infos)
			{
				SDL_Surface* surface = IMG_Load(info.files.data());
				if (surface == nullptr)
				{
					continue;
				}
				std::shared_ptr<SDL_Texture> texture(SDL_CreateTextureFromSurface(renderer, surface), SDL_DestroyTexture);
				SDL_FreeSurface(surface);
				if (texture == nullptr)
				{
					continue;
				}
				gResources[info.name] = texture;
			}
		}
	}
}

void DataManager::loadData(loadImpl* impl, const std::string& path)
{
	impl->loadDatafile(path, gMonsters,gPlayerStatus);
}

std::shared_ptr<SDL_Texture>& DataManager::findAtlasIndex(const std::string& name, int index)  
{  
   static std::shared_ptr<SDL_Texture> nullTexture = nullptr;  

   // 检查 name 是否存在于 gAtlas 中  
   auto it = gAtlas.find(name);  
   if (it == gAtlas.end())  
   {  
       return nullTexture;  
   }  

   // 检查 index 是否在有效范围内  
   if (index < 0 || index >= static_cast<int>(it->second.size()))  
   {  
       return nullTexture;  
   }  

   return it->second[index];  
}

std::string DataManager::getFontPath(const std::string& name)
{
	auto ttf = srcPath.otherInfos["ttf"];
	for (auto& t : ttf)
	{
		if (t.name == name)
		{
			return t.files;
		}
	}
}
