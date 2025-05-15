#pragma once
#include "CollisionBox.h"
#include <vector>
#include <memory>

class CollisionManager
{
public:
	CollisionBox* createBox();
	void destroyBox(CollisionBox* box);
	void checkCollision();
	void on_render(SDL_Renderer* renderer);
	void on_update(float delta);

	static CollisionManager& instance()
	{
		static CollisionManager instance;
		return instance;
	}
	~CollisionManager();

	int cm_axis = 0;

private:
	CollisionManager() {};
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator=(const CollisionManager&) = delete;

	std::vector<CollisionBox*> m_boxs;

};

