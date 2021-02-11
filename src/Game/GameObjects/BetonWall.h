#pragma once

#include "IGameObject.h"

#include <memory>
#include <array>
namespace RenderEngine {
	class Sprite;
}

class BetonWall : public IGameObject
{
public:

	enum class EBlockWallType {
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

	enum class EBlockWallState {
		Enable = 0,
		Destroyed
	};

	enum class EBlockLocation {
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};

	BetonWall(const EBlockWallType eBrickWallType, const glm::vec2& position, const glm::vec2& size, const float rotation);
	virtual void render()const override;
	virtual void update(const uint64_t delta)override;

private:
	void renderBlock(const EBlockLocation eBrickLocation) const;

	std::array<glm::vec2, 4> m_blockOffets;
	std::array<EBlockWallState, 4> m_eCurrentBlockState;
	std::shared_ptr<RenderEngine::Sprite> m_sprites;
};