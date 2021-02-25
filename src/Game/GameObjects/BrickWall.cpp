#include "BrickWall.h"

#include "../../Renderer/Sprite.h"
#include "../../Resource/RecourceManager.h"

#include <iostream>

BrickWall::EBrickWallState BrickWall::getBrickStateAfterCollision(const EBrickWallState currentState, const Physics::ECollisionDirection direction)
{
	switch (currentState)
	{
	case EBrickWallState::All:
		switch (direction)
		{
		case Physics::ECollisionDirection::Left:
			return EBrickWallState::Right;
		case Physics::ECollisionDirection::Right:
			return EBrickWallState::Left;
		case Physics::ECollisionDirection::Top:
			return EBrickWallState::Bottom;
		case Physics::ECollisionDirection::Bottom:
			return EBrickWallState::Top;
		}

	case EBrickWallState::Top:
		switch (direction)
		{
		case Physics::ECollisionDirection::Left:
			return EBrickWallState::TopRight;
		case Physics::ECollisionDirection::Right:
			return EBrickWallState::TopLeft;
		case Physics::ECollisionDirection::Top:
			return EBrickWallState::Destroyed;
		case Physics::ECollisionDirection::Bottom:
			return EBrickWallState::Destroyed;
		}

	case EBrickWallState::Bottom:
		switch (direction)
		{
		case Physics::ECollisionDirection::Left:
			return EBrickWallState::BottomRight;
		case Physics::ECollisionDirection::Right:
			return EBrickWallState::BottomLeft;
		case Physics::ECollisionDirection::Top:
			return EBrickWallState::Destroyed;
		case Physics::ECollisionDirection::Bottom:
			return EBrickWallState::Destroyed;
		}

	case EBrickWallState::Left:
		switch (direction)
		{
		case Physics::ECollisionDirection::Left:
			return EBrickWallState::Destroyed;
		case Physics::ECollisionDirection::Right:
			return EBrickWallState::Destroyed;
		case Physics::ECollisionDirection::Top:
			return EBrickWallState::BottomLeft;
		case Physics::ECollisionDirection::Bottom:
			return EBrickWallState::TopLeft;
		}

	case EBrickWallState::Right:
		switch (direction)
		{
		case Physics::ECollisionDirection::Left:
			return EBrickWallState::Destroyed;
		case Physics::ECollisionDirection::Right:
			return EBrickWallState::Destroyed;
		case Physics::ECollisionDirection::Top:
			return EBrickWallState::BottomRight;
		case Physics::ECollisionDirection::Bottom:
			return EBrickWallState::TopRight;
		}

	default:
		return EBrickWallState::Destroyed;
	}
}

Physics::AABB BrickWall::getAABBForBrickState(const EBlockLocation location, const EBrickWallState eBrickWallState, const glm::vec2& size)
{
	glm::vec2 blockOffset(0);
	switch (location)
	{
	case EBlockLocation::BottomLeft:
		break;
	case EBlockLocation::BottomRight:
		blockOffset.x += size.x / 2;
		break;
	case EBlockLocation::TopLeft:
		blockOffset.y += size.y / 2;
		break;
	case EBlockLocation::TopRight:
		blockOffset += size / 2.f;
		break;
	}

	glm::vec2 bottomLeft(0);
	glm::vec2 topRight(0);
	switch (eBrickWallState)
	{
	case EBrickWallState::All:
		topRight = glm::vec2(size.x / 2, size.y / 2);
		break;
	case EBrickWallState::TopLeft:
		bottomLeft = glm::vec2(0, size.y / 4);
		topRight = glm::vec2(size.x / 4, size.y / 2);
		break;
	case EBrickWallState::TopRight:
		bottomLeft = glm::vec2(size.x / 4, size.y / 4);
		topRight = glm::vec2(size.x / 2, size.y / 2);
		break;
	case EBrickWallState::Top:
		bottomLeft = glm::vec2(0, size.y / 4);
		topRight = glm::vec2(size.x / 2, size.y / 2);
		break;
	case EBrickWallState::BottomLeft:
		topRight = glm::vec2(size.x / 4, size.y / 4);
		break;
	case EBrickWallState::Left:
		topRight = glm::vec2(size.x / 4, size.y / 2);
		break;
	case EBrickWallState::TopRight_BottomLeft:
		topRight = glm::vec2(size.x / 2, size.y / 2);
		break;
	case EBrickWallState::Top_BottomLeft:
		topRight = glm::vec2(size.x / 2, size.y / 2);
		break;
	case EBrickWallState::BottomRight:
		bottomLeft = glm::vec2(size.x / 4, 0);
		topRight = glm::vec2(size.x / 2, size.y / 4);
		break;
	case EBrickWallState::TopLeft_BottomRight:
		topRight = glm::vec2(size.x / 2, size.y / 2);
		break;
	case EBrickWallState::Right:
		bottomLeft = glm::vec2(size.x / 4, 0);
		topRight = glm::vec2(size.x / 2, size.y / 2);
		break;
	case EBrickWallState::Top_BottomRight:
		topRight = glm::vec2(size.x / 2, size.y / 2);
		break;
	case EBrickWallState::Bottom:
		topRight = glm::vec2(size.x / 2, size.y / 4);
		break;
	case EBrickWallState::TopLeft_Bottom:
		topRight = glm::vec2(size.x / 2, size.y / 2);
		break;
	case EBrickWallState::TopRight_Bottom:
		topRight = glm::vec2(size.x / 2, size.y / 2);
		break;
	case EBrickWallState::Destroyed:
		break;
	}

	return { bottomLeft + blockOffset, topRight + blockOffset };
}

void BrickWall::onCollisionCallback(const EBlockLocation location, const IGameObject& object, const Physics::ECollisionDirection direction)
{
	if (object.getObjectType() != IGameObject::EObjectType::Bullet) return;
	const EBrickWallState newBrickState = getBrickStateAfterCollision(m_eCurrentBrickState[static_cast<size_t>(location)], direction);
	m_eCurrentBrickState[static_cast<size_t>(location)] = newBrickState;
	if (newBrickState != EBrickWallState::Destroyed)
	{
		m_brickLocationToColliderMap[static_cast<size_t>(location)]->boundingBox = getAABBForBrickState(location, newBrickState, m_size);
	}
	else
	{
		m_brickLocationToColliderMap[static_cast<size_t>(location)]->isActive = false;
	}
};

BrickWall::BrickWall(const EBrickWallType eBrickWallType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
	: IGameObject(IGameObject::EObjectType::BrickWall, position, size, rotation, layer)
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
		
	auto onCollisionCallbackTopLeft = [&](const IGameObject& object, const Physics::ECollisionDirection direction)
	{
		onCollisionCallback(EBlockLocation::TopLeft, object, direction);
	};
	auto onCollisionCallbackTopRight = [&](const IGameObject& object, const Physics::ECollisionDirection direction)
	{
		onCollisionCallback(EBlockLocation::TopRight, object, direction);
	};
	auto onCollisionCallbackBottomLeft = [&](const IGameObject& object, const Physics::ECollisionDirection direction)
	{
		onCollisionCallback(EBlockLocation::BottomLeft, object, direction);
	};
	auto onCollisionCallbackBottomRight = [&](const IGameObject& object, const Physics::ECollisionDirection direction)
	{
		onCollisionCallback(EBlockLocation::BottomRight, object, direction);
	};

	m_brickLocationToColliderMap.fill(nullptr);
	m_colliders.reserve(4);

	switch (eBrickWallType)
	{
	case EBrickWallType::All:
		m_eCurrentBrickState.fill(EBrickWallState::All);
		m_brickLocationToColliderMap[static_cast<size_t>(EBlockLocation::TopLeft)] = &(m_colliders.emplace_back(getAABBForBrickState(EBlockLocation::TopLeft, EBrickWallState::All, m_size), onCollisionCallbackTopLeft));
		m_brickLocationToColliderMap[static_cast<size_t>(EBlockLocation::TopRight)] = &(m_colliders.emplace_back(getAABBForBrickState(EBlockLocation::TopRight, EBrickWallState::All, m_size), onCollisionCallbackTopRight));
		m_brickLocationToColliderMap[static_cast<size_t>(EBlockLocation::BottomLeft)] = &(m_colliders.emplace_back(getAABBForBrickState(EBlockLocation::BottomLeft, EBrickWallState::All, m_size), onCollisionCallbackBottomLeft));
		m_brickLocationToColliderMap[static_cast<size_t>(EBlockLocation::BottomRight)] = &(m_colliders.emplace_back(getAABBForBrickState(EBlockLocation::BottomRight, EBrickWallState::All, m_size), onCollisionCallbackBottomRight));
		break;
	case EBrickWallType::Top:
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::TopLeft)] = EBrickWallState::All;
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::TopRight)] = EBrickWallState::All;
		m_brickLocationToColliderMap[static_cast<size_t>(EBlockLocation::TopLeft)] = &(m_colliders.emplace_back(getAABBForBrickState(EBlockLocation::TopLeft, EBrickWallState::All, m_size), onCollisionCallbackTopLeft));
		m_brickLocationToColliderMap[static_cast<size_t>(EBlockLocation::TopRight)] = &(m_colliders.emplace_back(getAABBForBrickState(EBlockLocation::TopRight, EBrickWallState::All, m_size), onCollisionCallbackTopRight));
		break;
	case EBrickWallType::Bottom:
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::BottomLeft)] = EBrickWallState::All;
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::BottomRight)] = EBrickWallState::All;
		m_brickLocationToColliderMap[static_cast<size_t>(EBlockLocation::BottomLeft)] = &(m_colliders.emplace_back(getAABBForBrickState(EBlockLocation::BottomLeft, EBrickWallState::All, m_size), onCollisionCallbackBottomLeft));
		m_brickLocationToColliderMap[static_cast<size_t>(EBlockLocation::BottomRight)] = &(m_colliders.emplace_back(getAABBForBrickState(EBlockLocation::BottomRight, EBrickWallState::All, m_size), onCollisionCallbackBottomRight));
		break;
	case EBrickWallType::Left:
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::TopLeft)] = EBrickWallState::All;
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::BottomLeft)] = EBrickWallState::All;
		m_brickLocationToColliderMap[static_cast<size_t>(EBlockLocation::TopLeft)] = &(m_colliders.emplace_back(getAABBForBrickState(EBlockLocation::TopLeft, EBrickWallState::All, m_size), onCollisionCallbackTopLeft));
		m_brickLocationToColliderMap[static_cast<size_t>(EBlockLocation::BottomLeft)] = &(m_colliders.emplace_back(getAABBForBrickState(EBlockLocation::BottomLeft, EBrickWallState::All, m_size), onCollisionCallbackBottomLeft));
		break;
	case EBrickWallType::Right:
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::TopRight)] = EBrickWallState::All;
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::BottomRight)] = EBrickWallState::All;
		m_brickLocationToColliderMap[static_cast<size_t>(EBlockLocation::TopRight)] = &(m_colliders.emplace_back(getAABBForBrickState(EBlockLocation::TopRight, EBrickWallState::All, m_size), onCollisionCallbackTopRight));
		m_brickLocationToColliderMap[static_cast<size_t>(EBlockLocation::BottomRight)] = &(m_colliders.emplace_back(getAABBForBrickState(EBlockLocation::BottomRight, EBrickWallState::All, m_size), onCollisionCallbackBottomRight));
		break;
	case EBrickWallType::TopLeft:
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::TopLeft)] = EBrickWallState::All;
		m_brickLocationToColliderMap[static_cast<size_t>(EBlockLocation::TopLeft)] = &(m_colliders.emplace_back(getAABBForBrickState(EBlockLocation::TopLeft, EBrickWallState::All, m_size), onCollisionCallbackTopLeft));
		break;
	case EBrickWallType::TopRight:
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::TopRight)] = EBrickWallState::All;
		m_brickLocationToColliderMap[static_cast<size_t>(EBlockLocation::TopRight)] = &(m_colliders.emplace_back(getAABBForBrickState(EBlockLocation::TopRight, EBrickWallState::All, m_size), onCollisionCallbackTopRight));
		break;
	case EBrickWallType::BottomLeft:
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::BottomLeft)] = EBrickWallState::All;
		m_brickLocationToColliderMap[static_cast<size_t>(EBlockLocation::BottomLeft)] = &(m_colliders.emplace_back(getAABBForBrickState(EBlockLocation::BottomLeft, EBrickWallState::All, m_size), onCollisionCallbackBottomLeft));
		break;
	case EBrickWallType::BottomRight:
		m_eCurrentBrickState[static_cast<size_t>(EBlockLocation::BottomRight)] = EBrickWallState::All;
		m_brickLocationToColliderMap[static_cast<size_t>(EBlockLocation::BottomRight)] = &(m_colliders.emplace_back(getAABBForBrickState(EBlockLocation::BottomRight, EBrickWallState::All, m_size), onCollisionCallbackBottomRight));
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

