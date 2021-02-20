#include "Bullet.h"

#include "../../Renderer/Sprite.h"
#include "../../Resource/RecourceManager.h"

Bullet::Bullet(const double velocity,
	const glm::vec2& position,
	const glm::vec2& size,
	const float layer)
	: IGameObject(IGameObject::EObjectType::Bullet, position, size, 0.f, layer)
	, m_eOrientation(EOrientation::Top)
	, m_pSprite_top(ResourceManager::getSprite("bullet_Top"))
	, m_pSprite_bottom(ResourceManager::getSprite("bullet_Bottom"))
	, m_pSprite_left(ResourceManager::getSprite("bullet_Left"))
	, m_pSprite_right(ResourceManager::getSprite("bullet_Right"))
	, m_isActive(false)
	, m_maxVelocity(velocity)
{
	m_colliders.emplace_back(glm::vec2(0), m_size);
}

void Bullet::render()const
{
	if (m_isActive)
	{
		switch (m_eOrientation)
		{
		case Bullet::EOrientation::Top:
			m_pSprite_top->render(m_position, m_size, m_rotation, m_layer);
			break;
		case Bullet::EOrientation::Bottom:
			m_pSprite_bottom->render(m_position, m_size, m_rotation, m_layer);
			Bullet;
		case Bullet::EOrientation::Left:
			m_pSprite_left->render(m_position, m_size, m_rotation, m_layer);
			break;
		case Bullet::EOrientation::Right:
			m_pSprite_right->render(m_position, m_size, m_rotation, m_layer);
			break;
		default:
			break;
		}
	}

}

void Bullet::fire(const glm::vec2& position, const glm::vec2& direction)
{
	m_position = position;
	m_direction = direction;
	if (m_direction.x == 0.f)
	{
		m_eOrientation = (m_direction.y < 0) ? EOrientation::Bottom : EOrientation::Top;
	}
	else
	{
		m_eOrientation = (m_direction.x < 0) ? EOrientation::Left : EOrientation::Right;
	}

	m_isActive = true;
	setVelocity(m_maxVelocity);
}

void Bullet::onCollision()
{
	setVelocity(0);
	m_isActive = false;
}