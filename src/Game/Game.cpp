#include "Game.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Renderer/ShaderProgram.h"
#include "../Resource/RecourceManager.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"

#include "GameObjects/Tank.h"
#include "Level.h"

#include <iostream>

Game::Game(const glm::ivec2& windowSize)
	: m_eCurrentGameState(EGameState::Active)
    , m_windowSize(windowSize)
{
	m_keys.fill(false);
}

Game::~Game()
{

}

void Game::render()
{
   // ResourceManager::getAnimatedSprite("NewAnimatedSprite")->render();
    if (m_pTank)
    {
        m_pTank->render();
    }
    if (m_pLevel)
    {
        m_pLevel->render();
    }
}

void Game::update(const uint64_t delta)
{
    if (m_pLevel)
    {
        m_pLevel->update(delta);
    }

    if (m_pTank)
    {
        if (m_keys[GLFW_KEY_W])
        {
            m_pTank->setOrientation(Tank::EOrientation::Top);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_A])
        {
            m_pTank->setOrientation(Tank::EOrientation::Left);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_S])
        {
            m_pTank->setOrientation(Tank::EOrientation::Bottom);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_D])
        {
            m_pTank->setOrientation(Tank::EOrientation::Right);
            m_pTank->move(true);
        }
        else
        {
            m_pTank->move(false);
        }
        m_pTank->update(delta);
    }
}

void Game::setKey(const int key, const int action)
{
	m_keys[key] = action;
}

bool Game::init()
{
   ResourceManager::loadJSONResources("res/resources.json");

    auto pSpriteShaderProgram = ResourceManager::getShaderProgram("spriteShader");
    if (!pSpriteShaderProgram)
    {
        std::cerr << "Can't find shader program: " << "spriteShader" << std::endl;
        return false;
    }
    m_pLevel = std::make_unique<Level>(ResourceManager::getLevels()[1]);
    m_windowSize.x = static_cast<int>(m_pLevel->getLevelWidth());
    m_windowSize.y = static_cast<int>(m_pLevel->getLevelHeight());
    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);


    m_pTank = std::make_unique<Tank>(0.0000001f, glm::vec2(0), glm::vec2(16.f, 16.f), 0.f);
   
    return true;
}

size_t Game::getCurrentLevelWidth()const
{
    return m_pLevel->getLevelWidth();
}

size_t Game::getCurrentLevelHeight()const
{
    return m_pLevel->getLevelHeight();
}
