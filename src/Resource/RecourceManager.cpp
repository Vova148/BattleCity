#include "RecourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"


ResourceManager::SharedProgramsMap ResourceManager::m_sharedPrograms;
ResourceManager::TexturesMap ResourceManager::m_textures;
ResourceManager::SpritesMap ResourceManager::m_sprites;
ResourceManager::AnimatedSpritesMap ResourceManager::m_animatedSprites;
std::string ResourceManager::m_path;


void ResourceManager::unloadAllResource()
{
	m_sharedPrograms.clear();
	m_textures.clear();
	m_sprites.clear();
	m_animatedSprites.clear();
}
void ResourceManager::setExecutablePath(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath)
{
	std::ifstream f;
	f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);

	if (!f.is_open()) {
		std::cerr << "Failde to open file: " << relativeFilePath << std::endl;
		return std::string();
	}

	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath) 
{
	std::string vertexString = getFileString(vertexPath);
	if (vertexPath.empty())
	{
		std::cerr << "No vertex shader!" << std::endl;
		return nullptr;
	}
	
	std::string fragmentString = getFileString(fragmentPath);
	if (fragmentPath.empty())
	{
		std::cerr << "No fragment shader!" << std::endl;
		return nullptr;
	}

	std::shared_ptr<RenderEngine::ShaderProgram>& newShader = m_sharedPrograms.emplace(shaderName, std::make_shared<RenderEngine::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}

	std::cerr << "Can't load shader program:\n"
		<< "Vertex: " << vertexPath << "\n"
		<< "Fragment: " << fragmentPath << std::endl;

	return nullptr;
}

std::shared_ptr<RenderEngine::ShaderProgram>  ResourceManager::getShaderProgram(const std::string& shaderName)
{
	SharedProgramsMap::const_iterator it = m_sharedPrograms.find(shaderName);
	if (it != m_sharedPrograms.end())
	{
		return it->second;
	}
	std::cerr << "Can'd find the shader program: " << shaderName << std::endl;

	return nullptr;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
	int channel = 0;
	int widht = 0;
	int height = 0;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &widht, &height, &channel, 0);
	if (!pixels)
	{
		std::cerr << "Can't load image: " << texturePath << std::endl;
		return nullptr;
	}

	std::shared_ptr<RenderEngine::Texture2D> newTexture = m_textures.emplace(textureName, std::make_shared<RenderEngine::Texture2D>(widht,
																															height,
																															pixels,
																															channel,
																															GL_NEAREST,
																															GL_CLAMP_TO_EDGE)).first->second;
	stbi_image_free(pixels);

	return newTexture;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::getTexture(const std::string& textureName) {
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		return it->second;
	}
	std::cerr << "Can'd find the shader program: " << textureName << std::endl;

	return nullptr;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::loadSprite(const std::string& spriteName,
																const std::string& textureName,
																const std::string& shaderName,
																const unsigned int spriteWidth,
																const unsigned int spriteHeight,
																const std::string& subTextureName)
{
	auto pTexture = getTexture(textureName);
	if (!pTexture) {
		std::cerr << "Can't find the texture: " << textureName << "for the sprite: " << spriteName << std::endl;
	}

	auto pShader = getShaderProgram(shaderName);
	if (!pShader) {
		std::cerr << "Can't find the shader: " << shaderName << "for the sprite: " << spriteName << std::endl;
	}

	std::shared_ptr<RenderEngine::Sprite> newSprite = m_sprites.emplace(spriteName, 															   
																	std::make_shared<RenderEngine::Sprite>(pTexture,
																	subTextureName,
																	pShader,
																	glm::vec2(0.f, 0.f),
																	glm::vec2(spriteWidth, spriteHeight))).first->second;
	return newSprite;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::getSprite(const std::string& spriteName)
{
	SpritesMap::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end())
	{
		return it->second;
	}
	std::cerr << "Can'd find the sprite: " << spriteName << std::endl;

	return nullptr;
}


std::shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::loadAnimatedSprite(const std::string& spriteName,
	const std::string& textureName,
	const std::string& shaderName,
	const unsigned int spriteWidth,
	const unsigned int spriteHeight,
	const std::string& subTextureName)
{
	auto pTexture = getTexture(textureName);
	if (!pTexture) {
		std::cerr << "Can't find the texture: " << textureName << "for the sprite: " << spriteName << std::endl;
	}

	auto pShader = getShaderProgram(shaderName);
	if (!pShader) {
		std::cerr << "Can't find the shader: " << shaderName << "for the sprite: " << spriteName << std::endl;
	}

	std::shared_ptr<RenderEngine::AnimatedSprite> newSprite = m_animatedSprites.emplace(spriteName,
		std::make_shared<RenderEngine::AnimatedSprite>(pTexture,
			subTextureName,
			pShader,
			glm::vec2(0.f, 0.f),
			glm::vec2(spriteWidth, spriteHeight))).first->second;

	return newSprite;
}

std::shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::getAnimatedSprite(const std::string& spriteName)
{
	auto it = m_animatedSprites.find(spriteName);
	if (it != m_animatedSprites.end())
	{
		return it->second;
	}
	std::cerr << "Can'd find the sprite: " << spriteName << std::endl;

	return nullptr;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTextureAtlas(const std::string textureName,
																		const std::string texturePath,
																		const std::vector<std::string> subTextures,
																		const unsigned int subTextureWidth,
																		const unsigned int subTextureHeight)
{
	auto pTexture = loadTexture(std::move(textureName), std::move(texturePath));

	if (pTexture)
	{
		const unsigned int TextureWidth = pTexture->width();
		const unsigned int TextureHeight = pTexture->heigth();
		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = TextureHeight;
		//проходим по всім іменам
		for (const auto& currentSubTextureName : subTextures)
		{
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / TextureWidth, 
								   static_cast<float>(currentTextureOffsetY - subTextureHeight) / TextureHeight);
			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / TextureWidth,
								 static_cast<float>(currentTextureOffsetY) / TextureHeight);

			pTexture->addSubTexture(std::move(currentSubTextureName), leftBottomUV, rightTopUV);


			currentTextureOffsetX += subTextureWidth;
			if (currentTextureOffsetX >= TextureWidth)
			{
				currentTextureOffsetX = 0;
				currentTextureOffsetY -= subTextureHeight;
			}
		}
	}

	return pTexture;
}