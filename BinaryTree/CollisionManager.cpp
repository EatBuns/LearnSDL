#include "CollisionManager.h"
#include "DataManager.h"

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
    // ����box�ı߽�
    const float boxLeft = boxRect.x;
    const float boxRight = boxRect.x + boxRect.w;
    const float boxTop = boxRect.y;
    const float boxBottom = boxRect.y + boxRect.h;

    // ����other�ı߽�
    const float otherLeft = otherRect.x;
    const float otherRight = otherRect.x + otherRect.w;
    const float otherTop = otherRect.y;
    const float otherBottom = otherRect.y + otherRect.h;

    // �����ص�����ʹ�ø���汾���㣩
    const float overlapX = fmaxf(0.0f, fminf(boxRight, otherRight) - fmaxf(boxLeft, otherLeft));
    const float overlapY = fmaxf(0.0f, fminf(boxBottom, otherBottom) - fmaxf(boxTop, otherTop));

    // ȷ����ײ����
    CollisionSide side = COLLIDE_NONE;
    if (overlapX < overlapY) {
        // ˮƽ������ײ����/�ң�
        const float boxCenterX = boxRect.x + boxRect.w / 2;
        const float otherCenterX = otherRect.x + otherRect.w / 2;
        side = (otherCenterX < boxCenterX) ? COLLIDE_LEFT : COLLIDE_RIGHT;
    }
    else {
        // ��ֱ������ײ����/�£�
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

		// ���������������ľ���λ��
#if 0
        auto box_rect = var->getRect();

        auto cam_rect =  DataManager::GetInstance().m_camera.CameraRect;
        if (box_rect.x + box_rect.w < cam_rect.x ||
            box_rect.x > cam_rect.x + cam_rect.w ||
            box_rect.y + box_rect.h < cam_rect.y ||
            box_rect.y > cam_rect.y + cam_rect.h)
        {
			// �������������Ұ�⣬������ײ
            SDL_Log("node:%s,collision disEnable",var->getNodeName().data());
            var->setEnable(false);
        }
        else
        {
			// �������������Ұ�ڣ�������ײ
			var->setEnable(true);
        }
#endif
        if(cm_axis != 0)
            var->setPosition(var->getPos_x() - DataManager::GetInstance().m_camera.CameraRect.x, var->getPos_y() - DataManager::GetInstance().m_camera.CameraRect.y);
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
