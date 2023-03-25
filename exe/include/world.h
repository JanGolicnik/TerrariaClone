#pragma once

#include <core.h>
#include <componentsystems.h>

#include <mutex>
#include <thread>

namespace map
{
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

	extern std::string guideName;
	extern int guideEnt;
	extern std::vector<std::string> guidenames;

	extern int moonphase;
	extern std::array<std::string, 8> moonphases;

	extern int shadoworbsbroken;

	void growBlock(glm::vec2 pos, std::string block, int depth, int chance, std::set<std::string>ignore = {});
	void clear();
	void generateTest();
	void generategame();

	void createWorld();

	bool save();
	bool load();
	bool loadSpecific(std::string NAME);

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

	void spawnDebugSetup(glm::vec2 pos);

	void liquidHole(glm::vec2 pos, std::string liquid, glm::vec2 size, bool half, bool breakwall);
	void placeLiquids();
};

