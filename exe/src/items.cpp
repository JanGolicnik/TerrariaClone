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
#include <animations.h>
#include <gameLoop.h>
#include <Window.h>

#define args std::string item, int ent
namespace items
{
	std::unordered_map<std::string, itemInfo> info;
	std::vector<recipe> recipes;
	std::unordered_map<std::string, naturalChest> naturalChests;
	std::unordered_map<std::string, itemChance> potItems;
	std::unordered_map<std::string, std::function<void(mobC*, physicsC*)>> buffs;
	std::unordered_map<std::string, setBonus> setBonuses;
	std::unordered_map<std::string, std::array<glm::vec4, 13>> armorsprites;

	void addItem(std::string itemName, std::string displayName, std::string tex,
		std::vector<std::function<void(args)>> onleftclick,
		float useSpeed, std::set<itemFamily> families,
		std::unordered_map<std::string, itemStat> stats, std::vector<std::function<bool(itemconditionargs)>> conditions, itemAnim animType, float sizeMod)
	{
		if (info.count(itemName) >= 1) return;
		itemInfo tmp;
		tmp.tex = tex;
		tmp.displayName = displayName;
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
				info[rec.items[i].item].families.insert(if_MATERIAL);
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
	void addSound(const char* item, void(*func)())
	{
		if (info.count(item) >= 1) {
			info[item].soundsfunc = func;
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
	void addSet(std::string set, std::function<void()> func, std::string description)
	{
		setBonuses.insert(std::make_pair(set, setBonus(func, description)));
	}


	void printItems()
	{
		for (auto& item : info) {
			std::cout << item.first << ": {\n";
			std::cout << "tex: " << item.second.tex << "\n";
			std::cout << "}\n";
		}
	}
	void addEntity(std::string item, std::string entity)
	{
		if (info.count(item) >= 1) {
			info[item].entity = entity;
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
	void addItemToNaturalChest(std::string chestName, std::string item, float chance, int num, int random, bool israre)
	{
		if (items::info.count(item) <= 0) {
			std::cout << "ERROR ADDING ITEM TO CHEST: adding " << item << " to " << chestName << " failed because the item doesnt exist\n";
			return;
		}
		itemChance tmp;
		tmp.chance = chance;
		tmp.num = num;
		tmp.random = random;
		if (israre) {
			naturalChests[chestName].rareitems.push_back(std::make_pair(item, tmp));
		}else
		naturalChests[chestName].items.push_back(std::make_pair(item, tmp));
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

	void addArmorSprites(std::string name, glm::vec4 coords, glm::vec4 legcoords)
	{
		/*
		0 body
		1 roka dol
		2 roka za udart
		3 shoulderguard
		4 druga roka skok
		5 roka hods
		6 druga roka hods
		7 druga roka stojis
		8 glava
		9 legsstand
		10 legsjump
		*/
		std::array<glm::vec4, 13> sprites;
		glm::vec4 spritesheet = glm::vec4(globals::spritesheetWH.x, globals::spritesheetWH.y, globals::spritesheetWH.x, globals::spritesheetWH.y);
		sprites[0] = coords;
		sprites[1] = coords + glm::vec4(20, 0, 20, 0) / spritesheet;
		sprites[2] = coords + glm::vec4(40, 0, 40, 0) / spritesheet;
		sprites[3] = coords + glm::vec4(0, -24, 0, -24) / spritesheet;
		sprites[4] = coords + glm::vec4(20, -24, 20, -24) / spritesheet;
		sprites[5] = coords + glm::vec4(40, -24, 40, -24) / spritesheet;
		sprites[6] = coords + glm::vec4(40, -48, 40, -48) / spritesheet;
		sprites[7] = coords + glm::vec4(20, -48, 20, -48) / spritesheet;
		sprites[8] = coords + glm::vec4(0, -24, 0, -24) / spritesheet;
		sprites[9] = coords + glm::vec4(-20, 0, -20, 0) / spritesheet;
		sprites[10] = coords + glm::vec4(-20, -24, -20, -24) / spritesheet;
		sprites[11] = coords + glm::vec4(0, -48, 0, -48) / spritesheet;
		sprites[12] = coords + glm::vec4(-20, -48, -20, -48) / spritesheet;
		armorsprites.insert(std::make_pair(name, sprites));

		std::vector<animations::keyframe> tmpframes;
		int pixels = 20;
		for (int i = 0; i < 4; i++) {
			tmpframes.push_back({ i * 5, {i * pixels, 0, i * pixels, 0}, ft_OFFSET1 });
		}
		animations::createAnim(name + "armuse", items::armorsprites[name][2], tmpframes, 5 * 4);

		tmpframes.clear();
		pixels = 20;
		for (int i = 0; i < 4; i++) {
			tmpframes.push_back({ i * 28, {i * pixels, 0, i * pixels, 0}, ft_OFFSET1 });
		}
		animations::createAnim(name + "frontarmwalk", items::armorsprites[name][5], tmpframes, 28 * 4);

		tmpframes.clear();
		pixels = 20;
		for (int i = 0; i < 4; i++) {
			tmpframes.push_back({ i * 28, {i * pixels, 0, i * pixels, 0}, ft_OFFSET1 });
		}
		animations::createAnim(name + "backarmwalk", items::armorsprites[name][6], tmpframes, 28 * 4);

		tmpframes.clear();
		pixels = 24;
		for (int i = 0; i < 14; i++) {
			tmpframes.push_back({ i * 8, {0, -i * pixels, 0,  -i * pixels}, ft_OFFSET1 });
		}
		animations::createAnim(name + "legswalk", legcoords, tmpframes, 8 * 14);
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
		glm::vec2 coords = Window::mouseBlockCoords();
		float dist = sqrt(pow(coords.x - Player::pos.x, 2) + pow(coords.y - Player::pos.y, 2));
		if (dist < Player::editRange) {
			auto blocks = &Layers::blocks;
			std::string places = items::getStatT(item, "places").data();
			if (Layers::placeBlock(coords, places, Player::editsize, &blocks::nameToInfo[places].conditions)) {
				removeAmmo(item, ent);
				Layers::updateBlock(&Layers::blocks, coords);
				Layers::updateBlock(&Layers::walls, coords);
				sounds::digSound();
			}
		}
	}

	void placeSapling(args)
	{
		glm::vec2 coords =Window::mouseBlockCoords();
		coords.y--;
		auto block = Layers::queryBlockName(&Layers::blocks, coords);
		coords.y++;
		if (*block == "grass") {
			Layers::placeBlock(coords, "sapling", 1, &blocks::nameToInfo["sapling"].conditions);
		}else if(*block == "junglegrass") {
			Layers::placeBlock(coords, "mahoganysapling", 1, &blocks::nameToInfo["mahoganysapling"].conditions);

		}else if(*block == "corruptgrass") {
			Layers::placeBlock(coords, "corruptsapling", 1, &blocks::nameToInfo["corruptsapling"].conditions);

		}else if (*block == "snow") {
			Layers::placeBlock(coords, "borealsapling", 1, &blocks::nameToInfo["borealsapling"].conditions);

		}
	}

	void dig(args) {
		glm::vec2 coords =Window::mouseBlockCoords();
		float dist = sqrt(pow(coords.x - Player::pos.x, 2) + pow(coords.y - Player::pos.y, 2));
		if (dist < Player::editRange) {
			auto info = items::getInfo(item);
			if (info->families.count(if_PICKAXE) > 0) {
				float digstrength = items::getStat(item, "digstrength", { .valueFloat = 0 }).valueFloat;
				if (Layers::damageBlock(&Layers::blocks, coords, digstrength , Player::editsize, true, if_PICKAXE)) {
					sounds::digSound();
				}
			}
			if (info->families.count(if_HAMMER) > 0) {
				float hammerstrength = items::getStat(item, "hammerstrength", { .valueFloat = 0 }).valueFloat;
				if (Layers::damageBlock(&Layers::walls, coords, hammerstrength, Player::editsize, true, if_HAMMER)) {
					sounds::digSound();
				}
				if (Layers::damageBlock(&Layers::blocks, coords, hammerstrength, Player::editsize, true, if_HAMMER)) {
					sounds::digSound();
				}
			}
			if (info->families.count(if_AXE) > 0) {
				float axestrength = items::getStat(item, "axestrength", { .valueFloat = 0 }).valueFloat;
				if (Layers::damageBlock(&Layers::blocks, coords, axestrength, Player::editsize, true, if_AXE)) {
					sounds::digSound();
				}
			}
		}
	}
	void shootArrow(args)
	{
		std::string projName = items::getStatT(item, "projectile").data();
		if (enemies::projectiles.count(projName) <= 0) return;
		int e = ECS::newEntity();
		projectileBase projectile = enemies::projectiles[projName];
		
		auto ppos = std::make_shared<glm::vec2>(*(ECS::getComponent<physicsC>(ent)->position));
		projectile.dc.position = ppos;
		projectile.dc.size += items::getStat(item, "projectileSize").valueVec2;
		projectile.dc.parent = globals::projectileLayer;

		projectile.pc.position = ppos;
		projectile.pc.prevpos = *Player::position();
		projectile.pc.bounce += items::getStat(item, "projectileBounce").valueBool;
		projectile.pc.size += items::getStat(item, "projectileHitbox").valueVec2;
		float spread = ((rand()%1000)/500.0f - 1) * items::getStat(item, "projectileSpread").valueFloat;
		glm::vec2 target = Window::mouseBlockCoords(false) - Player::pos;
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
		if (enemies::projectiles.count(projName) <= 0) return;
		int e = ECS::newEntity();
		projectileBase projectile = enemies::projectiles[projName];
		Player::boomerangsout += 1;
		projectile.dc.parent = globals::projectileLayer;

		projectile.pc.prevpos = Player::pos;
		auto ppos = std::make_shared<glm::vec2>(Player::pos);
		projectile.dc.position = ppos;
		projectile.pc.position = ppos;
		projectile.pc.vel = glm::normalize(Window::mouseBlockCoords(false) - Player::pos);

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
		enemies::spawnEnemy("bomb", Player::center, false, glm::normalize(Window::mouseBlockCoords(false) - Player::center));
	}
	void removeAmmo(args)
	{
		std::string ammo = items::getStatT(item, "ammo").data();
		if (UI::cursorItem.item == item) {
			UI::cursorItem.num--;
			if (UI::cursorItem.num <= 0) {
				UI::cursorItem.item = "empty";
			}
			return;
		}
		if (UI::hotbar->remove(ammo)) return;
		if (UI::inventory->remove(ammo)) return;

	}
	void summonEye(args)
	{
		if(globals::cdayTime > globals::dayLength/2.0f)
		enemies::spawnEnemy("eyeofcthulu", Window::mouseBlockCoords(false));
	}
	void takeMana(args)
	{
		Player::mana -= items::getStat(item, "manacost", { .valueFloat = 0 }).valueFloat;
		Player::manaregentimer = 0;
	}
	void starfuryStar(args)
	{
		int distX = rand() % 13;
		glm::vec2 pos = glm::vec2(Window::mouseBlockCoords(false).x - Player::dir * distX, Player::pos.y + Layers::trueBsOnScr.y / 2 + 10);
		std::string projName = items::getStatT(item, "projectile").data();
		if (enemies::projectiles.count(projName) <= 0) return;
		int e = ECS::newEntity();
		projectileBase projectile = enemies::projectiles[projName];
		projectile.dc.parent = globals::projectileLayer;

		projectile.pc.prevpos = pos;
		auto ppos = std::make_shared<glm::vec2>(pos);
		projectile.dc.position = ppos;
		projectile.pc.position = ppos;
		projectile.pc.vel = glm::normalize(Window::mouseBlockCoords(false) - pos) * glm::vec2(1);

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
			projectileBase projectile = enemies::projectiles["terrabladeswing"];
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
			projectileBase projectile = enemies::projectiles["terrabladeprojectile"];

			auto ppos = std::make_shared<glm::vec2>(*(ECS::getComponent<physicsC>(ent)->position));
			projectile.pc.position = ppos;
			projectile.pc.prevpos = *projectile.pc.position;
			projectile.dc.position = projectile.pc.position;
			projectile.dc.parent = globals::projectileLayer;
			projectile.pc.vel = glm::normalize(Window::mouseBlockCoords(false) - Player::pos) * glm::vec2(1.2);
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
		glm::vec2 dir = glm::normalize(Window::mouseBlockCoords(false) - Player::pos);
		float a = int(utils::angleOfVector(dir) + 90) % 360;
		glm::mat4 mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));

		glm::vec2 currpos = Player::pos;
		for (int i = 0; i < 15; i++) {
			currpos += dir;
			projectileBase projectile = enemies::projectiles["vilethorn"];
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
		enemies::spawnEnemy("twinssummon1", Window::mouseBlockCoords(false));
		enemies::spawnEnemy("twinssummon2", Window::mouseBlockCoords(false));
		Player::addBuff("twins", -1);
		Player::currsummons++;
	}
	void impStaff(args)
	{
		enemies::spawnEnemy("imp", Window::mouseBlockCoords(false));
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
	void bladeOfGrass(args)
	{
		projectileBase projectile = enemies::projectiles["bladeofgrassprojectile"];
		auto ppos = std::make_shared<glm::vec2>(*(ECS::getComponent<physicsC>(ent)->position));
		projectile.pc.position = ppos;
		projectile.pc.prevpos = *projectile.pc.position;
		projectile.dc.position = projectile.pc.position;
		projectile.dc.parent = globals::projectileLayer;
		projectile.pc.vel = glm::normalize(Window::mouseBlockCoords(false) - Player::pos) * glm::vec2(0.8);
		int e = ECS::newEntity();
		drawSystem::addComponent(e, &projectile.dc, false);
		ECS::queueComponent<physicsC>(e, projectile.pc);
		ECS::queueComponent<mobC>(e, projectile.mc);
		ECS::queueComponent<aiC>(e, projectile.ac);
		ECS::queueComponent<particleEmmiterC>(e, projectile.pec);
	}
	void manaCrystal(args)
	{
		Player::manacrystals += 1;
		Player::mana += 20;
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
		map::npcs["merchant"].canspawn = true;

		if (UI::hotbar->contains("coppercoin", 100)) {
			if (UI::hotbar->add("silvercoin", 1) == 0) {
				UI::hotbar->take("coppercoin", 100);
				silvercoinonpickup();
			}else if(UI::inventory->add("silvercoin", 1) == 0) {
				UI::inventory->take("coppercoin", 100);
				silvercoinonpickup();
			}
		}
		else if (UI::inventory->contains("coppercoin", 100)) {
			if (UI::hotbar->add("silvercoin", 1) == 0) {
				UI::hotbar->take("coppercoin", 100);
				silvercoinonpickup();
			}
			else if (UI::inventory->add("silvercoin", 1) == 0) {
				UI::inventory->take("coppercoin", 100);
				silvercoinonpickup();
			}
		}
	}
	void goldcoinonpickup()
	{
		if (UI::hotbar->contains("goldcoin", 100)) {
			if (UI::hotbar->add("platinumcoin", 1) == 0) {
				UI::hotbar->take("goldcoin", 100);
				silvercoinonpickup();
			}
			else if (UI::inventory->add("platinumcoin", 1) == 0) {
				UI::inventory->take("goldcoin", 100);
				silvercoinonpickup();
			}
		}
		else if (UI::inventory->contains("goldcoin", 100)) {
			if (UI::hotbar->add("platinumcoin", 1) == 0) {
				UI::hotbar->take("goldcoin", 100);
				silvercoinonpickup();
			}
			else if (UI::inventory->add("platinumcoin", 1) == 0) {
				UI::inventory->take("goldcoin", 100);
				silvercoinonpickup();
			}
		}
	}
	void silvercoinonpickup()
	{
		if (UI::hotbar->contains("silvercoin", 100)) {
			if (UI::hotbar->add("goldcoin", 1) == 0) {
				UI::hotbar->take("silvercoin", 100);
				silvercoinonpickup();
			}
			else if (UI::inventory->add("goldcoin", 1) == 0) {
				UI::inventory->take("silvercoin", 100);
				silvercoinonpickup();
			}
		}
		else if (UI::inventory->contains("silvercoin", 100)) {
			if (UI::hotbar->add("goldcoin", 1) == 0) {
				UI::hotbar->take("silvercoin", 100);
				silvercoinonpickup();
			}
			else if (UI::inventory->add("goldcoin", 1) == 0) {
				UI::inventory->take("silvercoin", 100);
				silvercoinonpickup();
			}
		}
	}
	void teleportToSpawn(args)
	{
		if (Player::hasBuff("horrified")) {
			Player::hp = -100;
			gameLoop::toast("$NAME$ tried to escape");
		}
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
	std::string ammo = items::getStatT(item, "ammo").data();
	return UI::inventory->has(ammo) || UI::hotbar->has(ammo) || UI::cursorItem.item == ammo;
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
bool itemConditions::manaCrystal(itemconditionargs)
{
	return Player::manacrystals < 9;
}
bool itemConditions::heal(itemconditionargs)
{
	if (Player::hp >= Player::currmaxhp) return false;
	return !Player::hasBuff("potionsickness");
}
#undef args
