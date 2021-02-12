#include "BrickWall.h"

#include "../../Renderer/Sprite.h"
#include "../../Resource/RecourceManager.h"

BrickWall::BrickWall(const EBrickWallType eBrickWallType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
	: IGameObject(position, size, rotation, layer)
	, m_eCurrentBrickState{ EBrickWallState::Destroyed,
							EBrickWallState::Destroyed,
							EBrickWallState::Destroyed,
							EBrickWallState::Destroyed }
	, m_blockOffets{ glm::vec2(0, m_size.y / 2.f),
					 glm::vec2(m_size.x / 2.f, m_size.y / 2.f),
					 glm::vec2(0, 0),
					 glm::vec2(m_size.x / 2.f, 0) }


{
	m_sprites[static_cast<size_t>(EBrickWallState::All)] = ResourceManager::getSprite("brickWall_All");
	m_sprites[static_cast<size_t>(EBrickWallState::TopLeft)] = ResourceManager::getSprite("brickWall_TopLeft");
	m_sprites[static_cast<size_t>(EBrickWallState::TopRight)] = ResourceManager::getSprite("brickWall_TopRight");
	m_sprites[static_cast<size_t>(EBrickWallState::Top)] = ResourceManager::getSprite("brickWall_Top");
	m_sprites[static_cast<size_t>(EBrickWallState::BottomLeft)] = ResourceManager::getSprite("brickWall_BottomLeft");
	m_sprites[static_cast<size_t>(EBrickWallState::Left)] = ResourceManager::getSprite("brickWall_Left");
	m_sprites[static_cast<size_t>(EBrickWallState::TopRight_BottomLeft)] = ResourceManager::getSprite("brickWall_TopRight_BottomLeft");
	m_sprites[static_cast<size_t>(EBrickWallState::Top_BottomLeft)] = ResourceManager::getSprite("brickWall_Top_BottomLeft");
	m_sprites[static_cast<size_t>(EBrickWallState::BottomRight)] = ResourceManager::getSprite("brickWall_BottomRight");
	m_sprites[static_cast<size_t>(EBrickWallState::TopLeft_BottomRight)] = ResourceManager::getSprite("brickWall_TopLeft_BottomRight");
	m_sprites[static_cast<size_t>(EBrickWallState::Right)] = ResourceManager::getSprite("brickWall_Right");
	m_sprites[static_cast<size_t>(EBrickWallState::Top_BottomRight)] = ResourceManager::getSprite("brickWall_Top_BottomRight");
	m_sprites[static_cast<size_t>(EBrickWallState::Bottom)] = ResourceManager::getSprite("brickWall_Bottom");
	m_sprites[static_cast<size_t>(EBrickWallState::TopLeft_Bottom)] = ResourceManager::getSprite("brickWall_TopLeft_Bottom");
	m_sprites[static_cast<size_t>(EBrickWallState::TopRight_Bottom)] = ResourceManager::getSprite("brickWall_TopRight_Bottom");
		

	switch (eBrickWallType)
	{
	case EBrickWallType::All:
		m_eCurrentBrickState.fill(EBrickWallState::All);
		break;
	case EBrickWallType::Top:
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::TopLeft)] = EBrickWallState::All;
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::TopRight)] = EBrickWallState::All;
		break;
	case EBrickWallType::Bottom:
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::BottomLeft)] = EBrickWallState::All;
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::BottomRight)] = EBrickWallState::All;
		break;
	case EBrickWallType::Left:
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::TopLeft)] = EBrickWallState::All;
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::BottomLeft)] = EBrickWallState::All;
		break;
	case EBrickWallType::Right:
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::TopRight)] = EBrickWallState::All;
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::BottomRight)] = EBrickWallState::All;
		break;
	case EBrickWallType::TopLeft:
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::TopLeft)] = EBrickWallState::All;
		break;
	case EBrickWallType::TopRight:
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::TopRight)] = EBrickWallState::All;
		break;
	case EBrickWallType::BottomLeft:
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::BottomLeft)] = EBrickWallState::All;
		break;
	case EBrickWallType::BottomRight:
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::BottomRight)] = EBrickWallState::All;
		break;	
	}
}

void BrickWall::renderBrick(const EBlockLocation eBlockLocation) const
{
	const EBrickWallState state = m_eCurrentBrickState[static_cast<size_t>(eBlockLocation)];
	if(state != EBrickWallState::Destroyed)
	{
		m_sprites[static_cast<size_t>(state)]->render(m_position + m_blockOffets[static_cast<size_t>(eBlockLocation)], m_size / 2.f, m_rotation, m_layer);
	}
}

void BrickWall::render()const
{
	renderBrick(EBlockLocation::TopLeft);
	renderBrick(EBlockLocation::TopRight);
	renderBrick(EBlockLocation::BottomLeft);
	renderBrick(EBlockLocation::BottomRight);
}

void BrickWall::update(const uint64_t delta)
{

}