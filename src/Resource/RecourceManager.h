#pragma once
#include <string>
#include <memory>
#include <map>

namespace Renderer {
	class ShaderProgram;
	class Texture2D;
}
	class ResourceManager {
	public:
		ResourceManager(const std::string& executablePath);
		~ResourceManager() = default;

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;

		//завантаження шейдерів
		std::shared_ptr<Renderer::ShaderProgram> loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
		//отримання шейдера
		std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(const std::string& shaderName);
		//завантаження текстур
		std::shared_ptr<Renderer::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
		//отримання текстур
		std::shared_ptr<Renderer::Texture2D> getTexture(const std::string& textureName);
		//завантаження текстур
		
	private:
		std::string getFileString(const std::string& relativeFilePath)const;

		typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> SharedProgramsMap;
		SharedProgramsMap m_sharedPrograms;

		typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
		TexturesMap m_textures;

		std::string m_path;
	};
