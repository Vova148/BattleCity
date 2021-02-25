#pragma once
#include <array>
#include <glm/vec2.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW\glfw3.h>
#include <memory>


class Tank;
class Level;
class StartScreen;

class Game {
public:
	Game(const glm::ivec2& windowSize);
	~Game();

	void render();
	void update(const double delta);
	void setKey(const int key, const int action);
	bool init();
	unsigned int getCurrentWidth()const;
	unsigned int getCurrentHeight()const;

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
	std::shared_ptr<Tank> m_pTank;
	std::shared_ptr<Level> m_pLevel;
	std::shared_ptr<StartScreen> m_pStartScreen;
};