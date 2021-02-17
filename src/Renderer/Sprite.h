#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <memory>
#include <string>


namespace RenderEngine {
	class Texture2D;
	class ShaderProgram;

	class Sprite {
	public:

		struct FrameDescription
		{
			FrameDescription(const glm::vec2 _leftBottomUV, const glm::vec2 _rightTopUV, const double _duration)
				: leftBottomUV(_leftBottomUV)
				, rightTopUV(_rightTopUV)
				, duration(_duration)
			{}
			glm::vec2 leftBottomUV;
			glm::vec2 rightTopUV;
			double duration;
		};

		Sprite(const std::shared_ptr<Texture2D> pTexture,
			const std::string initialSubTexture,
			const std::shared_ptr<ShaderProgram> pSharedProgram);

		~Sprite();

		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;

		void render(const glm::vec2& position,
					const glm::vec2& size,
					const float rotation, 
					const float layer = 0.f,
					const size_t frameId = 0)const;

		void insertFrames(std::vector<FrameDescription> framesDescription);
		double getFrameDuration(const size_t frameId)const;
		size_t getFrameCount()const;

	protected:
		std::shared_ptr<Texture2D> m_pTexture;
		std::shared_ptr<ShaderProgram> m_pShaderProgram;


		VertexArray m_vertexArray;
		VertexBuffer m_vertexCoordsBuffer;
		VertexBuffer m_textureCoordsBuffer;
		IndexBuffer m_indexBuffer;
		
		std::vector<FrameDescription> m_framesDescription;

		mutable size_t m_lastFrameId;
	};
}