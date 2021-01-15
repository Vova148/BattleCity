#pragma once

#include "Sprite.h"

#include <vector>
#include <map>


namespace RenderEngine {

	class AnimatedSprite : public Sprite {
	public:
		AnimatedSprite(const std::shared_ptr<Texture2D> pTexture,
			const std::string initialSubTexture,
			const std::shared_ptr<ShaderProgram> pSharedProgram,
			const glm::vec2& position = glm::vec2(0.f),
			const glm::vec2& size = glm::vec2(1.f),
			const float rotation = 0.f);

		void insertState(std::string, std::vector < std::pair < std::string, uint64_t>> subTextureDuration);

		void render()const override;

		void setState(std::string& newState);

		void update(const uint64_t delta);

	private:
		std::map <std::string, std::vector <std::pair<std::string, uint64_t>>> m_statesMap;
		size_t m_currentFrame = 0;
		uint64_t m_currentAnimationTime = 0;
		std::map <std::string, std::vector <std::pair<std::string, uint64_t>>>::const_iterator m_pCurrentAnimationDurations;
		mutable bool m_dirty = false;
	};
}