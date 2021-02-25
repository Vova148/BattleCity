#pragma once

#include <vector>
#include <string>
#include <memory>

#include <glm/vec2.hpp>

#include "IGameState.h"

class IGameObject;

class Level : IGameState {
public:
	static constexpr unsigned int BLOCK_SIZE = 16;
	Level(const std::vector<std::string>& LevelDiscription);
	void render()const;
	void update(double delta);
	virtual unsigned int getStateWidth()const override;
	virtual unsigned int getStateHeight()const override;

	const glm::ivec2& getPlayerRespawn_1() const { return m_playerRespawn_1; }
	const glm::ivec2& getPlayerRespawn_2() const { return m_playerRespawn_2; }
	const glm::ivec2& getEnemyRespawn_1() const { return m_enemyRespawn_1; }
	const glm::ivec2& getEnemyRespawn_2() const { return m_enemyRespawn_2; }
	const glm::ivec2& getEnemyRespawn_3() const { return m_enemyRespawn_3; }

	std::vector<std::shared_ptr<IGameObject>> getObjectsInArea(const glm::vec2& bottomLeft, const glm::vec2& topRight);

private:
	size_t m_widthBlocks;
	size_t m_heightBlocks;
	unsigned int m_widthPixels = 0;
	unsigned int m_heightPixels = 0;

	glm::ivec2 m_playerRespawn_1;
	glm::ivec2 m_playerRespawn_2;
	glm::ivec2 m_enemyRespawn_1;
	glm::ivec2 m_enemyRespawn_2;
	glm::ivec2 m_enemyRespawn_3;

	std::vector<std::shared_ptr<IGameObject>> m_mapObjects;
};