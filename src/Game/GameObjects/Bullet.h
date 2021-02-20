#pragma once

#include "IGameObject.h"

#include <memory>
#include <array>
namespace RenderEngine {
	class Sprite;
}

class Bullet : public IGameObject
{
public:


	enum class EOrientation : uint8_t {
		Top,
		Bottom,
		Left,
		Right
	};

	Bullet(const double velocity, const glm::vec2& position, const glm::vec2& size,  const float layer);
	virtual void render()const override;
	bool isActive() const { return m_isActive; }
	void fire(const glm::vec2& position, const glm::vec2& direction);
	virtual void onCollision()override;

private:
	EOrientation m_eOrientation;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_top;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_bottom;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_left;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_right;
	bool m_isActive;
	double m_maxVelocity;

};
