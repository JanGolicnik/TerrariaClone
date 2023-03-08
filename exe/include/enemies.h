#pragma once

#include <core.h>
#include <componentsystems.h>

struct mobDrop {
	int num;
	int randomnum;
	float chance;
};

struct enemyBase {
	std::string name;
	std::vector<std::string> childrenBases;
	std::unordered_map<std::string, mobDrop> drops;
	bool hpbar;
	int spawntime = globals::dayLength/2;
	int spawnrange = globals::dayLength/2;
	float randomsize = 0;
	float randomcolor = 0;
	std::string bossbar = "";
	mobC mC;
	physicsC pC;
	aiC aC;
	drawC dC;
	particleEmmiterC pec;
	std::function<glm::vec2()> spawnFunc;
};

struct npc {
	std::vector<std::string> dialogueOptions;
	std::unordered_map<std::string, void(*)(uiC*)> buttons;
};

namespace enemies
{
	extern std::unordered_map<std::string, enemyBase>enemies;
	extern std::unordered_map<std::string, npc>npcs;

	int spawnEnemy(std::string name, glm::vec2 pos, glm::vec2 vel = glm::vec2(-13245,123432));
	void addEnemyToBiome(std::string enemy, std::string biome, float spawnchance);
	void addNPCDialogue(std::string npc, std::string dialogue);
	void addNPCButton(std::string npc, std::string buttontext,  void(*func)(uiC*));
	glm::vec2 fallenStarSpawnFunc();
	glm::vec2 defaultSpawnFunc();
	glm::vec2 flyingSpawnFunc();
	glm::vec2 wormSpawnFunc();
	glm::vec2 vultureSpawnFunc();
};

