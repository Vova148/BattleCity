#pragma once

#include "IGameObject.h"

#include <memory>
#include <array>
namespace RenderEngine {
	class Sprite;
}

class BrickWall : public IGameObject
{
public:

	enum class EBrickWallType : uint8_t {
		All,
		Top,
		Bottom,
		Left,
		Right,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};

	enum class EBrickWallState : uint8_t {
		All = 0,
		TopLeft,
		TopRight,
		Top,
		BottomLeft,
		Left,
		TopRight_BottomLeft,
		Top_BottomLeft,
		BottomRight,
		TopLeft_BottomRight,
		Right,
		Top_BottomRight,
		Bottom,
		TopLeft_Bottom,
		TopRight_Bottom,
		Destroyed
	};

	enum class EBlockLocation : uint8_t {
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};

	BrickWall(const EBrickWallType eBrickWallType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
	virtual void render()const override;
	virtual void update(const double delta)override;

private:
	void renderBrick(const EBlockLocation eBrickLocation) const;

	std::array<glm::vec2, 4> m_blockOffets;
	std::array<EBrickWallState, 4> m_eCurrentBrickState;
	std::array<std::shared_ptr<RenderEngine::Sprite>, 15> m_sprites;
};