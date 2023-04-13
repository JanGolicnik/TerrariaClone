#pragma once

#include <core.h>

#include <enemies.h>
#include <mutex>
#include <thread>

namespace map
{
	struct npc {
		int currName;
		std::vector<std::string> names;
		std::string base = "guide";
		bool canspawn = false;
		bool living = false;
		int current = -1;
	};

	extern std::string name;
	extern std::string tmpname;

	extern std::string seed;
	extern int mapX;
	extern int mapY;
	extern glm::vec2 PlayerSpawn;

	extern float surfaceH;
	extern float surfaceScale;
	extern int surfaceCaveRate;
	extern int undergroundDirtRate;
	extern int cavernHoleRate;
	extern int cavernDirtRate;
	extern int undergroundCaveRate;
	extern float underworldH;
	extern float ironRate;
	extern float copperRate;
	extern float underworldH;

	extern std::mutex worldGenProgress_mutex;
	extern std::string worldGenProgress;

	extern std::thread worldgenthread;

	extern int moonphase;
	extern std::array<std::string, 8> moonphases;

	extern int shadoworbsbroken;

	extern std::unordered_map<std::string, npc> npcs;
	
	void growBlock(glm::vec2 pos, std::string block, int depth, int chance, std::set<std::string>ignore = {});

	bool save();
	bool load();
	worldData loadToData(std::string name);
	bool loadSpecific(std::string NAME);

	void clear();

	void createWorld();
	void generateGame();
	void generateTest();

	void makeDesert(bool left);
	void makeSnow(bool left);
	void makeJungle(bool left);

	void makeCorruption(bool left);
	void makeCrimson(bool left);

	void modifyWorldProgress(std::string progress);

	void decorate();

	void placeChests();
	void placePots();

	void fillChest(glm::vec2 pos, std::string chestName);

	void genStructures();
	void addUndergroundHouse(glm::vec2 pos);

	void placeCactus(glm::vec2 pos);
	void placeRandomCactus();

	void makeHell();

	void liquidHole(glm::vec2 pos, std::string liquid, glm::vec2 size, bool half, bool breakwall);
	void placeLiquids();

	void resetNPCS();
	void spawnLivingNPCS();
	void saveNPCS(std::ofstream* file);
	void loadNPCS(std::ifstream* file);

	void spawnDebugSetup(glm::vec2 pos);
};

