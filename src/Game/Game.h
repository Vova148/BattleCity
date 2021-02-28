#pragma once
#include <array>
#include <glm/vec2.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW\glfw3.h>
#include <memory>


class IGameState;

namespace RenderEngine {
	class ShaderProgram;
}

class Game {
public:
	Game(const glm::uvec2& windowSize);
	~Game();

	void render();
	void update(const double delta);
	void setKey(const int key, const int action);
	bool init();
	unsigned int getCurrentWidth()const;
	unsigned int getCurrentHeight()const;
	void startNewLevel(const size_t level);
	void setWindowSize(const glm::uvec2& windowSize);
	void updateViewport();

private:
	std::array<bool, 349> m_keys;
	enum EGameState {
		startScreen,
		LevelStart,
		CurrentLevel,
		Pause,
		Scores,
		GameOver
	};

	glm::ivec2 m_windowSize;
	EGameState m_eCurrentGameState;

	std::shared_ptr<IGameState> m_pCurrentGameState;
	std::shared_ptr<RenderEngine::ShaderProgram> m_pSpriteShaderProgram;
};