#include "BrickWall.h"

#include "../../Renderer/Sprite.h"
#include "../../Resource/RecourceManager.h"

BrickWall::BrickWall(const EBrickWallType eBrickWallType, const glm::vec2& position, const glm::vec2& size, const float rotation)
	: IGameObject(position, size, rotation)
	, m_eCurrentBrickState{ EBrickWallState::Destroyed,
						    EBrickWallState::Destroyed,
						    EBrickWallState::Destroyed,
						    EBrickWallState::Destroyed }
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
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::TopLeft)] = EBrickWallState::All;
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::TopRight)] = EBrickWallState::All;
		break;
	case EBrickWallType::Bottom:
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::BottomLeft)] = EBrickWallState::All;
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::BottomRight)] = EBrickWallState::All;
		break;
	case EBrickWallType::Left:
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::TopLeft)] = EBrickWallState::All;
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::BottomLeft)] = EBrickWallState::All;
		break;
	case EBrickWallType::Right:
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::TopRight)] = EBrickWallState::All;
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::BottomRight)] = EBrickWallState::All;
		break;
	case EBrickWallType::TopLeft:
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::TopLeft)] = EBrickWallState::All;
		break;
	case EBrickWallType::TopRight:
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::TopRight)] = EBrickWallState::All;
		break;
	case EBrickWallType::BottomLeft:
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::BottomLeft)] = EBrickWallState::All;
		break;
	case EBrickWallType::BottomRight:
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::BottomRight)] = EBrickWallState::All;
		break;	
	}
}

void BrickWall::renderBrick(const EBrickLocation eBrickLocation) const
{
	static const std::array<glm::vec2, 4> offets = {
		glm::vec2(0, m_size.y / 2.f),
		glm::vec2(m_size.x / 2.f, m_size.y / 2.f),
		glm::vec2(0, 0),
		glm::vec2(m_size.x / 2.f, 0)
	};
	const EBrickWallState state = m_eCurrentBrickState[static_cast<size_t>(eBrickLocation)];
	if(state != EBrickWallState::Destroyed)
	{
		m_sprites[static_cast<size_t>(state)]->render(m_position + offets[static_cast<size_t>(eBrickLocation)], m_size / 2.f, m_rotation);
	}
}

void BrickWall::render()const
{
	renderBrick(EBrickLocation::TopLeft);
	renderBrick(EBrickLocation::TopRight);
	renderBrick(EBrickLocation::BottomLeft);
	renderBrick(EBrickLocation::BottomRight);
}

void BrickWall::update(const uint64_t delta)
{

}