#pragma once

#include <core.h>
#include <componentsystems.h>

namespace enemies {
	glm::vec2 fallenStarSpawnFunc();
	glm::vec2 defaultSpawnFunc();
	glm::vec2 flyingSpawnFunc();
	glm::vec2 wormSpawnFunc();
	glm::vec2 vultureSpawnFunc();
	glm::vec2 anywhereSpawnFunc();
}

struct mobDrop {
	int num;
	int randomnum;
	float chance;
};

struct enemyBase {
	std::string name;
	std::vector<std::string> childrenBases;
	std::vector<std::string> possibleSprites;
	std::unordered_map<std::string, mobDrop> drops;
	bool hpbar;
	bool resizeToSprite = false;
	bool behindBlocks = false;
	float randomsize = 0;
	float randomcolor = 0;
	float randomvelangle = 0;
	int prespawntimer = 0;
	int slots = 1;
	std::string bossbar = "";
	std::string prespawntext = "";
	std::string spawntext = "";
	mobC mC;
	physicsC pC;
	aiC aC;
	drawC dC;
	particleEmmiterC pec;
	std::function<glm::vec2()> spawnFunc = enemies::defaultSpawnFunc;
};

struct npc {
	std::vector<std::string> dialogueOptions;
	std::unordered_map<std::string, void(*)(uiC*)> buttons;
};

namespace enemies
{
	extern std::unordered_map<std::string, enemyBase>enemies;
	extern std::unordered_map<std::string, npc>npcs;

	extern int currslots;

	int spawnEnemy(std::string name, glm::vec2 pos, bool usesslots = false, glm::vec2 vel = glm::vec2(-13245, 123432));
	void addNPCDialogue(std::string npc, std::string dialogue);
	void addNPCButton(std::string npc, std::string buttontext,  void(*func)(uiC*));
	void doQueues();
	void spawnNPCS();

	void addEnemyToDay(std::string enemy, std::string biome, float spawnchance = 1);
	void addEnemyToNight(std::string enemy, std::string biome, float spawnchance = 1);
	void addEnemyToUnderground(std::string enemy, std::string biome, float spawnchance = 1);
	void addEnemyToCavern(std::string enemy, std::string biome, float spawnchance = 1);
	void addEnemyToUnderworld(std::string enemy, std::string biome, float spawnchance = 1);
};

