#include "Sprite.h"

#include "ShaderProgram.h"
#include "Texture2D.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>



namespace Renderer {
	Sprite::Sprite(const std::shared_ptr<Texture2D> pTexture,
		const std::string initialSubTexture,
		const std::shared_ptr<ShaderProgram> pSharedProgram,
		const glm::vec2& position,
		const glm::vec2& size,
		const float rotation)
		: m_pTexture(std::move(pTexture))
		, m_pShaderProgram(std::move(pSharedProgram))
		, m_position(position)
		, m_size(size)
		, m_rotation(rotation)
	{
		const GLfloat vertexCoords[] = {
			//X  Y
			0.f, 0.f,
			0.f, 1.f, 
		    1.f, 1.f, 
			1.f, 0.f, 
		};

		auto subTexture = m_pTexture->getSubTexture(std::move(initialSubTexture));

		const GLfloat textureCoords[] = {
			//U  V
			subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
			subTexture.leftBottomUV.x, subTexture.rigthTopUV.y,
			subTexture.rigthTopUV.x, subTexture.rigthTopUV.y,
			subTexture.rigthTopUV.x, subTexture.leftBottomUV.y,
		};

		const GLint indices[] = {
			0, 1, 2,	
			2, 3, 0
		};



		m_vertexCoordsBuffer.init(vertexCoords, 2 * 4 * sizeof(GLfloat));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);


		m_textureCoordsBuffer.init(textureCoords, 2 * 4 * sizeof(GLfloat));
		VertexBufferLayout textureCoordsLayout;
		textureCoordsLayout.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_textureCoordsBuffer, textureCoordsLayout);

		m_indexBuffer.init(indices, 6 * sizeof(GLuint));

		m_vertexArray.unbind();
		m_indexBuffer.unbind();
	}

	Sprite::~Sprite()
	{
	}

	void Sprite::render()const
	{
		m_pShaderProgram->use();

		glm::mat4 model(1.f);
		
		model = glm::translate(model, glm::vec3(m_position, 0.f));
		model = glm::translate(model, glm::vec3(0.5f * m_size.x, 0.5f * m_size.y, 0.f));
		model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.f, 0.f, 1.f));
		model = glm::translate(model, glm::vec3(-0.5f * m_size.x, -0.5f * m_size.y, 0.f));
		model = glm::scale(model, glm::vec3(m_size, 1.f));

		m_vertexArray.bind();
		m_pShaderProgram->setMatrix4("modelMat", model);

		glActiveTexture(GL_TEXTURE);
		m_pTexture->bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		m_vertexArray.unbind();
	}

	void Sprite::setPosition(const glm::vec2& position)
	{
		m_position = position;
	}

	void Sprite::setSize(const glm::vec2& size)
	{
		m_size = size;
	}

	void Sprite::setRotation(const float rotation)
	{
		m_rotation = rotation;
	}
 }