#pragma once

#include "IGameObject.h"

#include <memory>
#include <array>
namespace RenderEngine {
	class Sprite;
}

class Ice : public IGameObject
{
public:


	enum class EBlockLocation : uint8_t {
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};

	Ice(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
	virtual void render()const override;

private:
	void renderBlock(const EBlockLocation eBlockLocation) const;

	std::array<glm::vec2, 4> m_blockOffsets;
	std::shared_ptr<RenderEngine::Sprite> m_sprite;
};