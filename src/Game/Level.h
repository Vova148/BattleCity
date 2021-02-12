#pragma once

#include <vector>
#include <string>
#include <memory>

class IGameObject;

class Level {
public:
	Level(const std::vector<std::string>& LevelDiscription);
	void render()const;
	void update(size_t delta);
	size_t getLevelWidth()const;
	size_t getLevelHeight()const;

private:
	size_t m_width;
	size_t m_height;

	std::vector<std::shared_ptr<IGameObject>> m_mapObjects;
};