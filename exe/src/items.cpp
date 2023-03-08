#include "items.h"

#include <layers.h>
#include <player.h>
#include <ui.h>
#include <enemyFunctions.h>
#include <textures.h>
#include <utils.h>
#include <blocks.h>
#include <enemies.h>
#include <sounds.h>
#include <world.h>
#include <particles.h>
#include <layers.h>
#include <sstream>
#include <iomanip>

#define args std::string item, int ent
namespace items
{
	std::unordered_map<std::string, itemInfo> info;
	std::vector<recipe> recipes;
	std::unordered_map<std::string, projectileBase> projectiles;
	std::unordered_map<std::string, naturalChest> naturalChests;
	std::unordered_map<std::string, itemChance> potItems;
	std::unordered_map<std::string, std::function<void(mobC*, physicsC*)>> buffs;
	std::unordered_map<std::string, std::function<void()>> setBonuses;

	void addItem(std::string itemName, std::string tex,
		std::vector<std::function<void(args)>> onleftclick,
		float useSpeed, std::set<itemFamily> families,
		std::unordered_map<std::string, itemStat> stats, std::vector<std::function<bool(itemconditionargs)>> conditions, itemAnim animType, float sizeMod)
	{
		if (info.count(itemName) >= 1) return;
		itemInfo tmp;
		tmp.tex = tex;
		tmp.onleftclick = onleftclick;
		tmp.stats = stats;
		tmp.conditions = conditions;
		tmp.useSpeed = useSpeed;
		tmp.animType = animType;
		tmp.sizeMod = sizeMod;
		tmp.families = families;
		info.insert(std::make_pair(itemName, tmp));
	}
	void addRecipe(recipe rec)
	{
		recipes.push_back(rec);
		int index = recipes.size() - 1;
		if (info.count(rec.craftedItem) >= 1){
			info[rec.craftedItem].fromRecipes.push_back(index);
		}
		
		for (int i = 0; i < rec.items.size(); i++) {
			if (info.count(rec.items[i].item) >= 1) {
				info[rec.items[i].item].inRecipes.push_back(index);
			}
		}
	}
	void addOnPickup(std::string item, std::function<void()> func)
	{
		if (info.count(item) >= 1) {
			info[item].onpickup = func;
		}
	}
	void addDroppedAnim(std::string item, std::string animName)
	{
		if (info.count(item) >= 1) {
			info[item].anim = animName;
		}
	}
	void addStatI(const char* item, const char* statname, int value)
	{
		if (info.count(item) >= 1) {
			info[item].stats[statname].valueInt = value;
		}
	}
	void addStatF(const char* item, const char* statname, float value)
	{
		if (info.count(item) >= 1) {
			info[item].stats[statname].valueFloat = value;
		}
	}
	void addStatB(const char* item, const char* statname, bool value)
	{
		if (info.count(item) >= 1) {
			info[item].stats[statname].valueBool = value;
		}
	}
	void addStatT(const char* item, const char* statname, std::string value)
	{
		if (info.count(item) >= 1) {
			info[item].textStats[statname] = value;
		}
	}
	void addStatV2(const char* item, const char* statname, glm::vec2 value)
	{
		if (info.count(item) >= 1) {
			info[item].stats[statname].valueVec2 = value;
		}
	}
	void addStatV3(const char* item, const char* statname, glm::vec3 value)
	{
		if (info.count(item) >= 1) {
			info[item].stats[statname].valueVec3 = value;
		}
	}
	void addFamily(const char* item, itemFamily fam)
	{
		if (info.count(item) >= 1) {
			info[item].families.insert(fam);
		}
	}
	void addLight(const char* item, glm::vec3 light)
	{
		if (info.count(item) >= 1) {
			info[item].light = light;
		}
	}
	void addSizeMod(const char* item, float sizemod)
	{
		if (info.count(item) >= 1) {
			info[item].sizeMod = sizemod;
		}
	}
	void disableAutouse(const char* item)
	{
		if (info.count(item) >= 1) {
			info[item].autouse = false;
		}
	}

	void addEmmiter(const char* item, particleEmmiterC pec)
	{
		if (info.count(item) >= 1) {
			info[item].emmiter = pec;
		}
	}

	void addOnhit(const char* item, std::function<void(mobCargs)> func)
	{
		if (info.count(item) >= 1) {
			info[item].onhit = func;
		}
	}

	void addRarity(const char* item, Rarity rarity)
	{
		if (info.count(item) >= 1) {
			info[item].rarity = rarity;
		}
	}

	void addToSet(const char* item, std::string set)
	{
		if (info.count(item) >= 1) {
			info[item].set = set;
		}
	}
	void addSet(std::string set, std::function<void()> func)
	{
		setBonuses.insert(std::make_pair(set, func));
	}


	void printItems()
	{
		for (auto& item : info) {
			std::cout << item.first << ": {\n";
			std::cout << "tex: " << item.second.tex << "\n";
			std::cout << "}\n";
		}
	}
	itemInfo* getInfo(std::string item)
	{
		if (info.count(item) >= 1) {
			return &info[item];
		}
		else
			return &info["empty"];
	}
	glm::vec3 getRarityColor(Rarity rarity)
	{
		switch (rarity) {
		case r_GRAY:
			return glm::vec3(0.2);
		case r_WHITE:
			return glm::vec3(1);
		case r_BLUE:
			return glm::vec3(0.2, 0.2, 0.8);
		case r_GREEN:
			return glm::vec3(0.55, 0.95, 0.55);
		case r_ORANGE:
			return glm::vec3(0.8, 0.3, 0);
		case r_LIGHTED:
			return glm::vec3(0.8, 0.3, 0.3);
		case r_PINK:
			return glm::vec3(0.8, 0.5, 0.5);
		case r_LIGHTPURPLE:
			return glm::vec3(0.8, 0.5, 0.8);
		case r_LIME:
			return glm::vec3(0.5, 0.95, 0.1);
		case r_YELLOW:
			return glm::vec3(0.8, 0.8, 0.1);
		case r_CYAN:
			return glm::vec3(0.2, 0.8, 0.9);
		case r_RED:
			return glm::vec3(1, 0.1, 0.1);
		case r_PURPLE:
			return glm::vec3(0.9, 0.1, 0.9);
		default:
			return glm::vec3(1);
		}
	}
	std::string getRarityColorString(Rarity rarity)
	{
		std::string tmp;
		glm::vec3 val = getRarityColor(rarity);
		std::stringstream ss;
		ss << std::setw(3) << std::setfill('0') << int(val.r * 255) << std::setw(3) << std::setfill('0') << int(val.g * 255) << std::setw(3) << std::setfill('0') << int(val.b * 255);
		tmp += ss.str();
		return tmp;
	}
	itemStat getStat(std::string item, const char* stat, itemStat def)
	{
		if (info[item].stats.count(stat) <= 0) {
			return def;
		}
		return info[item].stats.at(stat);
	}
	std::string_view getStatT(std::string item, const char* stat, const char* def)
	{
		if (info[item].textStats.count(stat) <= 0) {
			return def;
		}
		return info[item].textStats.at(stat);
	}
	void addItemToNaturalChest(std::string chestName, std::string item, float chance, int num, int random)
	{
		itemChance tmp;
		tmp.chance = chance;
		tmp.num = num;
		tmp.random = random;
		naturalChests[chestName].items.insert(std::make_pair(item, tmp));
	}
	void addItemToPot(std::string item, float chance, int num, int random)
	{
		potItems.insert(std::make_pair(item, itemChance(chance, num, random)));
	}
	void clean()
	{
		info.clear();
		recipes.clear();
	}

	void playeronfirebuff(mobC* p, physicsC* phys)
	{
		Player::hp--;
		Player::position()->y++;
	}
};

namespace itemFuncs {
	void nothing(args) {

	}

	void place(args) {
		glm::vec2 coords = globals::mouseBlockCoordsZoomed();
		float dist = sqrt(pow(coords.x - Player::pos.x, 2) + pow(coords.y - Player::pos.y, 2));
		if (dist < Player::editRange) {
			auto blocks = Layers::getLayer("blocks");
			std::string places = items::getStatT(item, "places").data();
			if (Layers::placeBlock(coords, places, glm::vec3(0), Player::editsize, &blocks::nameToInfo[places].conditions)) {
				removeAmmo(item, ent);
				Layers::updateBlock(Layers::getLayer("blocks"), coords);
				Layers::updateBlock(Layers::getLayer("bg"), coords);
				sounds::digSound();
			}
		}
	}

	void placeSapling(args)
	{
		glm::vec2 coords = globals::mouseBlockCoordsZoomed();
		coords.y--;
		auto block = Layers::queryBlockName(Layers::getLayer("blocks"), coords);
		coords.y++;
		if (*block == "grass") {
			Layers::placeBlock(coords, "sapling", { 0,0,0 }, 1, &blocks::nameToInfo["sapling"].conditions);
		}else if(*block == "junglegrass") {
			Layers::placeBlock(coords, "mahoganysapling", { 0,0,0 }, 1, &blocks::nameToInfo["mahoganysapling"].conditions);

		}else if(*block == "corruptgrass") {
			Layers::placeBlock(coords, "corruptsapling", { 0,0,0 }, 1, &blocks::nameToInfo["corruptsapling"].conditions);

		}else if (*block == "snow") {
			Layers::placeBlock(coords, "borealsapling", { 0,0,0 }, 1, &blocks::nameToInfo["borealsapling"].conditions);

		}
	}

	void dig(args) {
		glm::vec2 coords = globals::mouseBlockCoordsZoomed();
		float dist = sqrt(pow(coords.x - Player::pos.x, 2) + pow(coords.y - Player::pos.y, 2));
		if (dist < Player::editRange) {
			if (Layers::damageBlock(Layers::getLayer(items::getStatT(item, "layer", "blocks")), coords, items::getStat(item, "digstrength", {.valueFloat = 0}).valueFloat, Player::editsize, true, items::getInfo(item))) {
				sounds::digSound();
			}
		}
	}
	void shootArrow(args)
	{
		std::string projName = items::getStatT(item, "projectile").data();
		if (items::projectiles.count(projName) <= 0) return;
		int e = ECS::newEntity();
		projectileBase projectile = items::projectiles[projName];
		
		auto ppos = std::make_shared<glm::vec2>(*(ECS::getComponent<physicsC>(ent)->position));
		projectile.dc.position = ppos;
		projectile.dc.size += items::getStat(item, "projectileSize").valueVec2;
		projectile.dc.parent = globals::projectileLayer;

		projectile.pc.position = ppos;
		projectile.pc.prevpos = *Player::position();
		projectile.pc.bounce += items::getStat(item, "projectileBounce").valueBool;
		projectile.pc.size += items::getStat(item, "projectileHitbox").valueVec2;
		float spread = ((rand()%1000)/500.0f - 1) * items::getStat(item, "projectileSpread").valueFloat;
		glm::vec2 target = globals::mouseBlockCoordsZoomed(false) - Player::pos;
		float angleRad = spread * PI / 180.0f;
		glm::vec2 vel = glm::normalize(target);
		vel.x = cos(angleRad) * vel.x - sin(angleRad) * vel.y;
		vel.y = sin(angleRad) * vel.x + cos(angleRad) * vel.y;
		vel *= items::getStat(item, "projectileSpeed").valueFloat;
		vel *= items::getStat(projName, "ms").valueFloat;
		projectile.pc.vel = vel;
		projectile.pc.weight += items::getStat(item, "projectileWeight").valueFloat;

		projectile.mc.damage += items::getStat(item, "projectileDamage").valueFloat;
		projectile.mc.hp += items::getStat(item, "projectilePierce").valueInt;
		projectile.mc.knockback += items::getStat(item, "projectileKnockback").valueFloat;
		projectile.mc.light += items::getStat(item, "projectileLight").valueVec3;
		projectile.mc.killin += items::getStat(item, "projectileLifespan").valueFloat;
		projectile.mc.onCollision.insert(std::make_pair(mf_ENEMY, collisionFs::damageBoth1));
		float a = int(utils::angleOfVector(projectile.pc.vel)) % 360;
		projectile.dc.mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));

		drawSystem::addComponent(e, &projectile.dc, false);
		ECS::queueComponent<physicsC>(e, projectile.pc);
		ECS::queueComponent<mobC>(e, projectile.mc);
		ECS::queueComponent<aiC>(e, projectile.ac);
		if (projectile.pec.rate > 0) {
			ECS::queueComponent<particleEmmiterC>(e, projectile.pec);
		}
	}
	void shootBoomerang(args)
	{
		std::string projName = items::getStatT(item, "projectile").data();
		if (items::projectiles.count(projName) <= 0) return;
		int e = ECS::newEntity();
		projectileBase projectile = items::projectiles[projName];
		Player::boomerangsout += 1;
		projectile.dc.parent = globals::projectileLayer;

		projectile.pc.prevpos = Player::pos;
		auto ppos = std::make_shared<glm::vec2>(Player::pos);
		projectile.dc.position = ppos;
		projectile.pc.position = ppos;
		projectile.pc.vel = glm::normalize(globals::mouseBlockCoordsZoomed(false) - Player::pos);

		drawSystem::addComponent(e, &projectile.dc, false);
		ECS::queueComponent<physicsC>(e, projectile.pc);
		ECS::queueComponent<mobC>(e, projectile.mc);
		ECS::queueComponent<aiC>(e, projectile.ac);
		if (projectile.pec.rate > 0) {
			ECS::queueComponent<particleEmmiterC>(e, projectile.pec);
		}
	}
	void throwSelf(args)
	{
		enemies::spawnEnemy("bomb", Player::center, glm::normalize(globals::mouseBlockCoordsZoomed(false) - Player::center));
	}
	void removeAmmo(args)
	{
		if(!UI::hotbar->remove(items::getStatT(item, "ammo").data()))
		UI::inventory->remove(items::getStatT(item, "ammo").data());
	}
	void summonEye(args)
	{
		enemies::spawnEnemy("demoneye", globals::mouseBlockCoordsZoomed(false));
	}
	void takeMana(args)
	{
		Player::mana -= items::getStat(item, "manacost", { .valueFloat = 0 }).valueFloat;
		Player::manaregentimer = 0;
	}
	void starfuryStar(args)
	{
		int distX = rand() % 13;
		glm::vec2 pos = glm::vec2(globals::mouseBlockCoordsZoomed(false).x - Player::dir * distX, Player::pos.y + Layers::trueBsOnScr.y / 2 + 10);
		std::string projName = items::getStatT(item, "projectile").data();
		if (items::projectiles.count(projName) <= 0) return;
		int e = ECS::newEntity();
		projectileBase projectile = items::projectiles[projName];
		projectile.dc.parent = globals::projectileLayer;

		projectile.pc.prevpos = pos;
		auto ppos = std::make_shared<glm::vec2>(pos);
		projectile.dc.position = ppos;
		projectile.pc.position = ppos;
		projectile.pc.vel = glm::normalize(globals::mouseBlockCoordsZoomed(false) - pos) * glm::vec2(1);

		drawSystem::addComponent(e, &projectile.dc, false);
		ECS::queueComponent<physicsC>(e, projectile.pc);
		ECS::queueComponent<mobC>(e, projectile.mc);
		ECS::queueComponent<aiC>(e, projectile.ac);
		if (projectile.pec.rate > 0) {
			ECS::queueComponent<particleEmmiterC>(e, projectile.pec);
		}
	}
	void terraBlade(args)
	{
		{
			projectileBase projectile = items::projectiles["terrabladeswing"];
			auto ppos = std::make_shared<glm::vec2>(*(ECS::getComponent<physicsC>(ent)->position));
			projectile.pc.position = ppos;
			projectile.pc.prevpos = *projectile.pc.position;
			projectile.dc.position = projectile.pc.position;
			projectile.dc.parent = globals::projectileLayer;
			projectile.ac.stats["maxframes"].intVal = items::info[item].useSpeed;
			projectile.ac.stats["currframes"].intVal = items::info[item].useSpeed;
			projectile.ac.stats["rotoffset"].intVal = 135;
			projectile.dc.color = glm::vec3(0.6, 0.9, 0.6);
			projectile.mc.killin = items::info[item].useSpeed + 10;
			projectile.dc.opacity = 0;
			int e = ECS::newEntity();
			drawSystem::addComponent(e, &projectile.dc, false);
			ECS::queueComponent<physicsC>(e, projectile.pc);
			ECS::queueComponent<mobC>(e, projectile.mc);
			ECS::queueComponent<aiC>(e, projectile.ac);
			ECS::queueComponent<particleEmmiterC>(e, projectile.pec);

			projectile.dc.color = glm::vec3(0.1, 0.4, 0.8);
			projectile.ac.stats["rotoffset"].intVal = 195;
			e = ECS::newEntity();
			drawSystem::addComponent(e, &projectile.dc, false);
			ECS::queueComponent<physicsC>(e, projectile.pc);
			ECS::queueComponent<mobC>(e, projectile.mc);
			ECS::queueComponent<aiC>(e, projectile.ac);
			ECS::queueComponent<particleEmmiterC>(e, projectile.pec);

			projectile.dc.color = glm::vec3(0.3, 0.8, 0.3);
			projectile.ac.stats["rotoffset"].intVal = 165;
			e = ECS::newEntity();
			drawSystem::addComponent(e, &projectile.dc, false);
			ECS::queueComponent<physicsC>(e, projectile.pc);
			ECS::queueComponent<mobC>(e, projectile.mc);
			ECS::queueComponent<aiC>(e, projectile.ac);
			ECS::queueComponent<particleEmmiterC>(e, projectile.pec);

			projectile.dc.tex = "terrabladeprojectile4";
			projectile.dc.color = glm::vec3(0.3, 0.6, 0.37);
			projectile.pec.randradius = 0;
			projectile.pec.radius = 8.5;
			projectile.pec.rate = 2;
			projectile.dc.opacity = 1;
			projectile.ac.stats["increaseintensity"].floatVal = 1.1f;
			e = ECS::newEntity();
			drawSystem::addComponent(e, &projectile.dc, false);
			ECS::queueComponent<physicsC>(e, projectile.pc);
			ECS::queueComponent<mobC>(e, projectile.mc);
			ECS::queueComponent<aiC>(e, projectile.ac);
			ECS::queueComponent<particleEmmiterC>(e, projectile.pec);
		}

		{
			projectileBase projectile = items::projectiles["terrabladeprojectile"];

			auto ppos = std::make_shared<glm::vec2>(*(ECS::getComponent<physicsC>(ent)->position));
			projectile.pc.position = ppos;
			projectile.pc.prevpos = *projectile.pc.position;
			projectile.dc.position = projectile.pc.position;
			projectile.dc.parent = globals::projectileLayer;
			projectile.pc.vel = glm::normalize(globals::mouseBlockCoordsZoomed(false) - Player::pos) * glm::vec2(1.2);
			projectile.dc.opacity = 0;

			projectile.ac.stats["rotoffset"].intVal = 15;
			projectile.dc.color = glm::vec3(0.1, 0.4, 0.8);
			projectile.ac.stats["maxopacity"].floatVal = 0.6f;
			int e = ECS::newEntity();
			drawSystem::addComponent(e, &projectile.dc, false);
			ECS::queueComponent<physicsC>(e, projectile.pc);
			ECS::queueComponent<mobC>(e, projectile.mc);
			ECS::queueComponent<aiC>(e, projectile.ac);
			ECS::queueComponent<particleEmmiterC>(e, projectile.pec);

			projectile.dc.color = glm::vec3(0.6, 0.8, 0.6);
			projectile.ac.stats["rotoffset"].intVal = -15;
			projectile.ac.stats["maxopacity"].floatVal = 0.6f;
			e = ECS::newEntity();
			drawSystem::addComponent(e, &projectile.dc, false);
			ECS::queueComponent<physicsC>(e, projectile.pc);
			ECS::queueComponent<mobC>(e, projectile.mc);
			ECS::queueComponent<aiC>(e, projectile.ac);
			ECS::queueComponent<particleEmmiterC>(e, projectile.pec);

			projectile.dc.color = glm::vec3(0.3, 0.9, 0.3);
			projectile.ac.stats["rotoffset"].intVal = 0;
			projectile.ac.stats["maxopacity"].floatVal = 1.0f;
			projectile.ac.stats["increaseintensity"].floatVal = 1.0022f;
			e = ECS::newEntity();
			drawSystem::addComponent(e, &projectile.dc, false);
			ECS::queueComponent<physicsC>(e, projectile.pc);
			ECS::queueComponent<mobC>(e, projectile.mc);
			ECS::queueComponent<aiC>(e, projectile.ac);
			ECS::queueComponent<particleEmmiterC>(e, projectile.pec);

			projectile.dc.tex = "terrabladeprojectile4";
			projectile.dc.color = glm::vec3(0.3, 0.6, 0.37);
			projectile.dc.opacity = 1;
			projectile.pec.randradius = 0;
			projectile.pec.radius = 8.5;
			projectile.pec.rate = 2;
			projectile.ac.stats["increaseintensity"].floatVal = 1.03f;
			e = ECS::newEntity();
			drawSystem::addComponent(e, &projectile.dc, false);
			ECS::queueComponent<physicsC>(e, projectile.pc);
			ECS::queueComponent<mobC>(e, projectile.mc);
			ECS::queueComponent<aiC>(e, projectile.ac);
			ECS::queueComponent<particleEmmiterC>(e, projectile.pec);
		}
	}
	void vilethorn(args)
	{
		glm::vec2 dir = glm::normalize(globals::mouseBlockCoordsZoomed(false) - Player::pos);
		float a = int(utils::angleOfVector(dir) + 90) % 360;
		glm::mat4 mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));

		glm::vec2 currpos = Player::pos;
		for (int i = 0; i < 15; i++) {
			currpos += dir;
			projectileBase projectile = items::projectiles["vilethorn"];
			projectile.pc.position = std::make_shared<glm::vec2>(currpos);
			projectile.mc.killin += i * 2;
			projectile.pc.prevpos = *projectile.pc.position; 
			projectile.dc.position = projectile.pc.position;
			projectile.dc.parent = globals::projectileLayer;
			projectile.dc.mat = mat;
			if (i == 14) {
				projectile.dc.tex = "vilethornhead";
			}
			int e = ECS::newEntity();
			drawSystem::addComponent(e, &projectile.dc, false);
			ECS::queueComponent<physicsC>(e, projectile.pc);
			ECS::queueComponent<mobC>(e, projectile.mc);
			ECS::queueComponent<aiC>(e, projectile.ac);
			ECS::queueComponent<particleEmmiterC>(e, projectile.pec);
		}
	}
	void beegun(args)
	{
		int num = rand() % 4 + 1;
		for (int i = 0; i < num; i++) {
			shootArrow(item, ent);
		}
	}
	void opticStaff(args)
	{
		enemies::spawnEnemy("twinssummon1", globals::mouseBlockCoordsZoomed(false));
		enemies::spawnEnemy("twinssummon2", globals::mouseBlockCoordsZoomed(false));
		Player::addBuff("twins", -1);
		Player::currsummons++;
	}
	void impStaff(args)
	{
		enemies::spawnEnemy("imp", globals::mouseBlockCoordsZoomed(false));
		Player::addBuff("imp", -1);
		Player::currsummons++;
	}
	void increaseMaxHealth(args)
	{
		Player::heartcrystals += 1;
		Player::hp += 20;
	}
	void heal(args)
	{
		Player::hp += items::getStat(item, "heal", {.valueFloat = 0}).valueFloat;
		Player::addBuff("potionsickness", items::getStat(item, "sickness", { .valueInt = 0 }).valueInt);
	}
	void nonearmorbonus()
	{
	}
	void ironarmorbonus()
	{
		Player::defense += 2;
	}
	void silverarmorbonus()
	{
		Player::defense += 3;
	}
	void goldarmorbonus()
	{
		Player::defense += 3;
	}
	void copperarmorbonus()
	{
		Player::defense += 2;
	}
	void moltenarmorbonus()
	{
		Player::meeleDamage += 0.1;
	}
	void cactusarmorbonus()
	{
		Player::thorns += 15;
	}
	void coppercoinonpickup()
	{
		if (UI::inventory->contains("coppercoin", 100)) {
			if (UI::inventory->add("silvercoin", 1) == 0) {
				UI::inventory->take("coppercoin", 100);
				silvercoinonpickup();
			}
		}
	}
	void goldcoinonpickup()
	{
		if (UI::inventory->contains("goldcoin", 100)) {
			if (UI::inventory->add("platinumcoin", 1) == 0) {
				UI::inventory->take("goldcoin", 100);
			}
		}
	}
	void silvercoinonpickup()
	{
		if (UI::inventory->contains("silvercoin", 100)) {
			if (UI::inventory->add("goldcoin", 1) == 0) {
				UI::inventory->take("silvercoin", 100);
				goldcoinonpickup();
			}
		}
	}
	void teleportToSpawn(args)
	{
		particles::spawnEffect("magicmirror", Player::pos);
		*Player::position() = map::PlayerSpawn;
		particles::spawnEffect("magicmirror", *Player::position());
	}
}

bool itemConditions::nothing(itemconditionargs)
{
	return true;
}

bool itemConditions::hasAmmo(itemconditionargs)
{
	return UI::inventory->has(items::getStatT(item, "ammo").data()) || UI::hotbar->has(items::getStatT(item, "ammo").data());
}
bool itemConditions::hasMana(itemconditionargs)
{
	if (Player::mana >= items::getStat(item, "mana", { .valueFloat = 0 }).valueFloat) {
		return true;
	}
	return false;
}
bool itemConditions::boomerang(itemconditionargs)
{
	return Player::boomerangsout < 1;
}
bool itemConditions::lightdisc(itemconditionargs)
{
	return Player::boomerangsout < 6;
}
bool itemConditions::hasSummonSpace(itemconditionargs)
{
	return Player::currsummons < Player::maxsummons;
}
bool itemConditions::heartCrystal(itemconditionargs)
{
	return Player::heartcrystals < 15;
}
bool itemConditions::heal(itemconditionargs)
{
	if (Player::hp >= Player::currmaxhp) return false;
	return !Player::hasBuff("potionsickness");
}
#undef args
