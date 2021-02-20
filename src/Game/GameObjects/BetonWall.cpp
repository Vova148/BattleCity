#include "BetonWall.h"

#include "../../Renderer/Sprite.h"
#include "../../Resource/RecourceManager.h"

BetonWall::BetonWall(const EBlockWallType eBlockWallType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
	: IGameObject(IGameObject::EObjectType::BetonWall, position, size, rotation, layer)
	, m_eCurrentBlockState{ EBlockWallState::Destroyed,
							EBlockWallState::Destroyed,
							EBlockWallState::Destroyed,
							EBlockWallState::Destroyed }
	, m_sprites(ResourceManager::getSprite("betonWall"))
	, m_blockOffets{ glm::vec2(0, m_size.y / 2.f),
					 glm::vec2(m_size.x / 2.f, m_size.y / 2.f),
					 glm::vec2(0, 0),
					 glm::vec2(m_size.x / 2.f, 0) }

{
	

	switch (eBlockWallType)
	{
	case EBlockWallType::All:
		m_eCurrentBlockState.fill(EBlockWallState::Enable);
		m_colliders.emplace_back(glm::vec2(0), m_size);
		break;
	case EBlockWallType::Top:
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::TopLeft)] = EBlockWallState::Enable;
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::TopRight)] = EBlockWallState::Enable;
		m_colliders.emplace_back(glm::vec2(0, m_size.y / 2), m_size);
		break;
	case EBlockWallType::Bottom:
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::BottomLeft)] = EBlockWallState::Enable;
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::BottomRight)] = EBlockWallState::Enable;
		m_colliders.emplace_back(glm::vec2(0), glm::vec2(m_size.x, m_size.y / 2));
		break;
	case EBlockWallType::Left:
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::TopLeft)] = EBlockWallState::Enable;
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::BottomLeft)] = EBlockWallState::Enable;
		m_colliders.emplace_back(glm::vec2(0), glm::vec2(m_size.x / 2, m_size.y));
		break;
	case EBlockWallType::Right:
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::TopRight)] = EBlockWallState::Enable;
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::BottomRight)] = EBlockWallState::Enable;
		m_colliders.emplace_back(glm::vec2(m_size.x / 2, 0), m_size);
		break;
	case EBlockWallType::TopLeft:
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::TopLeft)] = EBlockWallState::Enable;
		m_colliders.emplace_back(glm::vec2(0, m_size.y / 2), glm::vec2(m_size.x / 2, m_size.y));
		break;
	case EBlockWallType::TopRight:
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::TopRight)] = EBlockWallState::Enable;
		m_colliders.emplace_back(glm::vec2(m_size.x / 2, m_size.y / 2), m_size);
		break;
	case EBlockWallType::BottomLeft:
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::BottomLeft)] = EBlockWallState::Enable;
		m_colliders.emplace_back(glm::vec2(0), glm::vec2(m_size.x / 2, m_size.y / 2));
		break;
	case EBlockWallType::BottomRight:
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::BottomRight)] = EBlockWallState::Enable;
		m_colliders.emplace_back(glm::vec2(m_size.x / 2, 0), glm::vec2(m_size.x, m_size.y / 2));
		break;
	}
}

void BetonWall::renderBlock(const EBlockLocation eBlockLocation) const
{
	const EBlockWallState state = m_eCurrentBlockState[static_cast<size_t>(eBlockLocation)];
	if(state != EBlockWallState::Destroyed)
	{
		m_sprites->render(m_position + m_blockOffets[static_cast<size_t>(eBlockLocation)], m_size / 2.f, m_rotation, m_layer);
	}
}

void BetonWall::render()const
{
	renderBlock(EBlockLocation::TopLeft);
	renderBlock(EBlockLocation::TopRight);
	renderBlock(EBlockLocation::BottomLeft);
	renderBlock(EBlockLocation::BottomRight);
}

void BetonWall::update(const double delta)
{

}