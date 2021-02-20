#include "PhysicsEngine.h"
#include "../Game/GameObjects/IGameObject.h"
#include "../Game/Level.h"

namespace Physics {
	std::unordered_set<std::shared_ptr<IGameObject>> PhysicsEngine::m_dynamicObjects;
	std::shared_ptr<Level> PhysicsEngine::m_pCurrentLevel;

	void PhysicsEngine::intit()
	{

	}

	void PhysicsEngine::terminate()
	{
		m_dynamicObjects.clear();
		m_pCurrentLevel.reset();
	}

	void PhysicsEngine::update(const double delta)
	{
		for (auto& currentObject : m_dynamicObjects)
		{
			if (currentObject->getCurrentVelocity() > 0)
			{
				if (currentObject->getCurrentDirection().x != 0.f)
				{
					currentObject->getCurrentPosition() = glm::vec2(currentObject->getCurrentPosition().x, static_cast<unsigned int>(currentObject->getCurrentPosition().y / 4.f + 0.5f) * 4.f);
				}
				else if (currentObject->getCurrentDirection().y != 0.f)
				{
					currentObject->getCurrentPosition() = glm::vec2(static_cast<unsigned int>(currentObject->getCurrentPosition().x / 4.f + 0.5f) * 4.f, currentObject->getCurrentPosition().y);
				}
				const auto newPosition = currentObject->getCurrentPosition() + currentObject->getCurrentDirection() * static_cast<float>(currentObject->getCurrentVelocity() * delta);
				const auto& colliders = currentObject->getColliders();
				std::vector<std::shared_ptr<IGameObject>> ObjectToCheck = m_pCurrentLevel->getObjectsInArea(newPosition, newPosition + currentObject->getSize());
				
				bool hasCollision = false;

				for (const auto& currentObjectToCheck : ObjectToCheck)
				{
					const auto& collidersToCheck = currentObjectToCheck->getColliders();
					if (currentObjectToCheck->colliders(currentObject->getObjectType()) && !collidersToCheck.empty())
					{
						if (hasIntersection(colliders, newPosition, collidersToCheck, currentObjectToCheck->getCurrentPosition()))
						{
							hasCollision = true;
							currentObjectToCheck->onCollision();
							break;
						}
					}
				}
				if (!hasCollision)
				{
					currentObject->getCurrentPosition() = newPosition;
				}
				else
				{
					if (currentObject->getCurrentDirection().x != 0.f)
					{
						
						currentObject->getCurrentPosition() = glm::vec2(static_cast<unsigned int>(currentObject->getCurrentPosition().x / 8.f + 0.5f) * 8.f, currentObject->getCurrentPosition().y);
					}
					else if (currentObject->getCurrentDirection().y != 0.f)
					{
						currentObject->getCurrentPosition() = glm::vec2(currentObject->getCurrentPosition().x, static_cast<unsigned int>(currentObject->getCurrentPosition().y / 8.f + 0.5f) * 8.f);
					}
					currentObject->onCollision();
				}
			}
		}
	}

	void PhysicsEngine::addDynamicGamebject(std::shared_ptr<IGameObject> pGameObjects)
	{
		m_dynamicObjects.insert(std::move(pGameObjects));
	}

	void PhysicsEngine::setCurrentLevel(std::shared_ptr<Level> pLevel)
	{
		m_pCurrentLevel.swap(pLevel);
	}

	bool PhysicsEngine::hasIntersection(const std::vector<AABB>& colliders1, const glm::vec2& position1,
											   const std::vector<AABB>& colliders2, const glm::vec2& position2)
	{
		for (const auto& currentCollider1 : colliders1)
		{
			const glm::vec2 currentCollider1_topRight_world = currentCollider1.topRight + position1;
			const glm::vec2 currentCollider1_bottomLeft_world = currentCollider1.bottomLeft + position1;

			for (const auto& currentCollider2 : colliders2)
			{
				const glm::vec2 currentCollider2_topRight_world = currentCollider2.topRight + position2;
				const glm::vec2 currentCollider2_bottomLeft_world = currentCollider2.bottomLeft + position2;

				if (currentCollider1_bottomLeft_world.x >= currentCollider2_topRight_world.x)
				{
					continue;
				}
				if (currentCollider1_topRight_world.x <= currentCollider2_bottomLeft_world.x)
				{
					continue;
				}

				if (currentCollider1_bottomLeft_world.y >= currentCollider2_topRight_world.y)
				{
					continue;
				}
				if (currentCollider1_topRight_world.y <= currentCollider2_bottomLeft_world.y)
				{
					continue;
				}
				return true;
			}
		}
		return false;
	}
}