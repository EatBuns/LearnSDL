#include "CollisionBox.h"

void CollisionBox::on_update(float delat)
{
	if (isGravity)
	{
		m_rect.y += v_y;
	}

	m_rect.x += v_x;

	//updateFunc(this, delat);
}
