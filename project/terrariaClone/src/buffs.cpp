#include "buffs.h"

#include <ui.h>
#include <player.h>
#include <gameLoop.h>
#include <particles.h>

namespace buffs {

	std::unordered_map<std::string, Buff> buffs;

	void addBuff(std::string name, std::string tex, std::function<void(ActiveBuff* buff, mobC*, physicsC*)> func, bool endable, std::string description)
	{
		buffs.insert(std::make_pair(name, Buff(func, endable, tex, description)));
	}

	void createUI() {
		for (int i = 0; i < Player::buffs.size(); i++) {
			UI::addElement(ECS::newEntity(), ui_BUFF, { 100 + i * 100, -230 }, { 80, 80 }, gameLoop::buffcontainer, { {"buff", {.buffp  = &Player::buffs[i]}}}, {}, false, anchorTOPLEFT);
		}
	}

	void onfirebuff(ActiveBuff* buff, mobC* p, physicsC* phys)
	{
		if ((buff->starttime - buff->countdown) % 15 != 0) return;

		if (p == nullptr) {
			Player::hp--;
		}
		else {
			p->hp--;
		}

		int e = particles::spawnEffect("onfire", *phys->position);
		ECS::getComponent<particleEmmiterC>(e)->parent = globals::topparticleLayer;
	}
	void happybuff(ActiveBuff* buff, mobC* p, physicsC* phys)
	{
		Player::ms *= 1.2;
		Player::enemyChance -= 0.1;
	}
	void cozyfirebuff(ActiveBuff* buff, mobC* p, physicsC* phys)
	{
		Player::regeneration *= 1.2;
	}
	void nothing(ActiveBuff* buff, mobC* p, physicsC* phys)
	{
	}
	void chilled(ActiveBuff* buff, mobC* p, physicsC* phys)
	{
		if (p == nullptr) {
			Player::ms *= 0.7;
		}
	}
	void poisoned(ActiveBuff* buff, mobC* p, physicsC* phys)
	{
		if ((buff->starttime - buff->countdown) % 15 != 0) return;
		if (p == nullptr) {
			Player::hp--;
		}
		else {
			p->hp--;
		}
	}
}