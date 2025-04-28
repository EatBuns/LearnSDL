#include "CollisionManager.h"

typedef enum {
    COLLIDE_NONE,
    COLLIDE_LEFT,
    COLLIDE_RIGHT,
    COLLIDE_TOP,
    COLLIDE_BOTTOM
} CollisionSide;

CollisionBox* CollisionManager::createBox()
{
	auto box = new CollisionBox();
	m_boxs.push_back(box);
	return box;
}

void CollisionManager::destroyBox(CollisionBox* box)
{
	auto it = std::find(m_boxs.begin(), m_boxs.end(), box);
	if (it != m_boxs.end())
	{
		m_boxs.erase(it);
	}
	else
	{
		SDL_Log("CollisionBox not found!\n");
	}
}

CollisionSide detect_collision_side(const SDL_FRect& boxRect, const SDL_FRect& otherRect) {
    // 计算box的边界
    const float boxLeft = boxRect.x;
    const float boxRight = boxRect.x + boxRect.w;
    const float boxTop = boxRect.y;
    const float boxBottom = boxRect.y + boxRect.h;

    // 计算other的边界
    const float otherLeft = otherRect.x;
    const float otherRight = otherRect.x + otherRect.w;
    const float otherTop = otherRect.y;
    const float otherBottom = otherRect.y + otherRect.h;

    // 计算重叠区域（使用浮点版本计算）
    const float overlapX = fmaxf(0.0f, fminf(boxRight, otherRight) - fmaxf(boxLeft, otherLeft));
    const float overlapY = fmaxf(0.0f, fminf(boxBottom, otherBottom) - fmaxf(boxTop, otherTop));

    // 确定碰撞方向
    CollisionSide side = COLLIDE_NONE;
    if (overlapX < overlapY) {
        // 水平方向碰撞（左/右）
        const float boxCenterX = boxRect.x + boxRect.w / 2;
        const float otherCenterX = otherRect.x + otherRect.w / 2;
        side = (otherCenterX < boxCenterX) ? COLLIDE_LEFT : COLLIDE_RIGHT;
    }
    else {
        // 垂直方向碰撞（上/下）
        const float boxCenterY = boxRect.y + boxRect.h / 2;
        const float otherCenterY = otherRect.y + otherRect.h / 2;
        side = (otherCenterY < boxCenterY) ? COLLIDE_TOP : COLLIDE_BOTTOM;
    }
    return side;
}

void CollisionManager::checkCollision()
{
    for (auto& box : m_boxs)
    {
        if (box->resetcb)
            box->resetcb();
    }

    for (auto& box : m_boxs)
    {
        for (auto& other : m_boxs)
        {
            if (box != other && box->isEnable() && other->isEnable() && SDL_HasIntersectionF(&box->getRect(), &other->getRect()))
            {
                if (other->getSrcLayer() != CollisionBox::CollissionLayer::None && box->getDstLayer()[other->getSrcLayer()] && box->cb)
                {
                    const SDL_FRect boxRect = box->getRect();
                    const SDL_FRect otherRect = other->getRect();
                    CollisionSide side = detect_collision_side(boxRect, other->getRect());
					box->cb(static_cast<int>(other->getSrcLayer()), static_cast<int>(side), otherRect, other->getNodeName());
                }
            }
        }
    }
}


void CollisionManager::on_render(SDL_Renderer* renderer)
{
	for (auto& var : m_boxs)
	{
		var->on_render(renderer);
	}
}

void CollisionManager::on_update(float delta)
{
	for (auto& var : m_boxs)
	{
		var->on_update(delta);
	}
}

CollisionManager::~CollisionManager()
{
	for (auto& box : m_boxs)
	{
		delete box;
	}
	m_boxs.clear();
}
