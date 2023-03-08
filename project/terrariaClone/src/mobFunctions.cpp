#include "enemyFunctions.h"

#include <layers.h>
#include <utils.h>
#include <player.h>
#include <animations.h>
#include <globals.h>
#include <game.h>
#include <componentSystems.h>
#include <particles.h>
#include <input.h>
#include <ui.h>
#include <gameLoop.h>
#include <enemies.h>
#include <buffs.h>
#include <world.h>

#define args aiC* p, int entity, physicsC* phys, ComponentArray<aiC>* arr, aiAction* action, int ctime

namespace mobFunctions {
	void nothing(args)
	{
	}

	void slimeOnUpdate(args)
	{
		if (phys->collidebot) {
			phys->friction = true;
		}
		else {
			phys->friction = false;
		}
	}
	void slimeWindup(args)
	{
		if (phys->collidebot) {
			p->actions.push_back({ 16, slimeJump, 1, ctime });
			auto draw = ECS::getComponent<drawC>(entity);
			auto jumpanim = game::aiSys->getStat(p, "jumpanim", {.charp = nullptr})->charp;
			if (jumpanim != nullptr) {
				animations::watchAnim(jumpanim, &draw->anim);
			}
		}
	}
	void bombExplode(args)
	{
		particles::spawnEffect("bombexplosion", *phys->position);
		glm::vec2 pos = round(*phys->position);
		Layers::damageBlock(Layers::getLayer("blocks"), pos, 100, 7, true);
		Layers::damageBlock(Layers::getLayer("bg"), pos, 100, 5, true);
		auto mC = ECS::getComponent<mobC>(entity);
		mC->hitboxradius = 11;
		mC->killin = 1;
		ECS::getComponent<mobC>(mC->children[0])->killin = 2;
	}
	void boomerangOnUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		draw->mat = glm::rotate(draw->mat, (float)(21 * PI / 180.0f), glm::vec3(0, 0, 1.0f));
		if (p->state == 0) {
			if (phys->collidebot || phys->collidetop) {
				phys->vel.y = -phys->vel.y;
				p->state = 1;
			}

			if (phys->collideleft || phys->collideright || ECS::getComponent<mobC>(entity)->hp < 121321) {
				phys->vel.x = -phys->vel.x;
				p->state = 1;
			}
		}
		if (p->state == 1) {
			phys->vel += utils::approach(phys->vel, glm::normalize(Player::pos - *phys->position) * glm::vec2(1.5), 30);
			if (glm::distance(Player::pos, *phys->position) < 2.5) {
				ECS::getComponent<mobC>(entity)->hp = 0;
				Player::boomerangsout--;
			}
		}
	}
	void lightdiscOnUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		draw->mat = glm::rotate(draw->mat, (float)(21 * PI / 180.0f), glm::vec3(0, 0, 1.0f));
		if (p->state == 1) {
			phys->bounce = false;
			phys->vel += utils::approach(phys->vel, glm::normalize(Player::pos - *phys->position) * glm::vec2(1.5), 20);
			if (glm::distance(Player::pos, *phys->position) < 2.5) {
				ECS::getComponent<mobC>(entity)->hp = 0;
				Player::boomerangsout--;
			}
		}
	}
	void icebladeprojectileOnUpdate(args)
	{
		ECS::getComponent<particleEmmiterC>(entity)->dir = glm::normalize(phys->vel);
		if (phys->collidebot || phys->collideleft || phys->collideright || phys->collidetop) {
			ECS::getComponent<mobC>(entity)->hp = 0;
		}
	}
	void terrabladeswingOnUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		draw->flipX = false;
		if (Player::dir < 0) {
			draw->flipX = true;
		}
		int maxframes = game::aiSys->getStat(p, "maxframes")->intVal;
		auto currframes = game::aiSys->getStat(p, "currframes");
		int rotoffset = game::aiSys->getStat(p, "rotoffset")->intVal;
		float percentlifespan = (maxframes - currframes->intVal) / (float)maxframes;
		*(phys->position) = Player::pos;
		float angle = Player::dir * (-135.0f * percentlifespan + rotoffset);
		draw->mat = glm::translate(glm::mat4(1.0f), glm::vec3(Player::dir * -phys->size.x / 2, -phys->size.y / 2, 0));
		draw->mat = glm::rotate(draw->mat, float(angle * PI / 180.0f), glm::vec3(0, 0, 1));
		draw->mat = glm::translate(draw->mat, glm::vec3(Player::dir * phys->size.x / 2, phys->size.y / 2, 0));
		if (currframes->intVal > 0) {
			draw->opacity += utils::approach(draw->opacity, 0.9, maxframes * 2);
		}
		else {
			draw->opacity += utils::approach(draw->opacity, 0, 4);
		}
		float increaseintensity = game::aiSys->getStat(p, "increaseintensity", { .floatVal = 1 })->floatVal;
		draw->color *= increaseintensity;
		currframes->intVal--;
	}
	void terrabladeprojectileOnUpdate(args)
	{
		ECS::getComponent<particleEmmiterC>(entity)->dir = glm::normalize(phys->vel);
		int rotoffset = game::aiSys->getStat(p, "rotoffset")->intVal;
		auto draw = ECS::getComponent<drawC>(entity);

		float increaseintensity = game::aiSys->getStat(p, "increaseintensity", { .floatVal = 1 })->floatVal;
		draw->color *= increaseintensity;
		auto mobc = ECS::getComponent<mobC>(entity);
		if (mobc->killin < 40) {
			draw->size += utils::approach(draw->size, glm::vec2(0), 30);
			if (mobc->killin < 20) {
				draw->size += utils::approach(draw->size, glm::vec2(0), 10);
			}
			draw->opacity += utils::approach(draw->opacity, 0, 10);
		}
		else {
			draw->size += utils::approach(draw->size, glm::vec2(14.5f), 3);
			draw->opacity += utils::approach(draw->opacity, game::aiSys->getStat(p, "maxopacity", { .floatVal = 0.8 })->floatVal, 10);
		}
		phys->size = draw->size * glm::vec2(0.2);
		if (phys->collidebot || phys->collideleft || phys->collideright || phys->collidetop) {
			p->state = 1;
			if (mobc->killin > 70) mobc->killin = 70;
		}
		if (p->state == 1) {
			phys->vel += utils::approach(phys->vel, glm::vec2(0), 2.2);
		}
		else {
			float a = int(utils::angleOfVector(-phys->vel)) % 360;
			draw->mat = glm::rotate(glm::mat4(1.0f), float((a + rotoffset) * PI / 180.0f), glm::vec3(0, 0, 1));
			phys->vel += utils::approach(phys->vel, glm::vec2(0), 20);
		}
	}
	void boomerangChangeDir(args)
	{
		p->state = 1;
	}
	void guideOnUpdate(args)
	{
		if (glm::distance(*phys->position, Player::pos) > 30) return;
		auto bs = Layers::getLayer("blocks");
		auto draw = ECS::getComponent<drawC>(entity);
		phys->stoponcollision = true;
		phys->weight = 1;
		bool goUp = true;
		if (p->state == 0) {
			animations::removeAnim(&draw->anim);
			if (rand() % 100 == 0) {
				animations::watchAnim("guidewalk", &draw->anim, true);
				if (rand() % 2 == 0) {
					draw->flipX = true;
					p->state = 1;
					phys->vel.x = 0.03;
				}
				else {
					draw->flipX = false;
					p->state = 2;
					phys->vel.x = -0.03;
				}
				globals::engine->play2D("MJ.mp3");
			}
		}
		else {
			if (rand() % 140 == 0) {
				if (p->state == 1) {
					draw->flipX = true;
				}
				else {
					draw->flipX = false;
				}
				p->state = 0;
				phys->vel.x = 0;
			}


			if (phys->vel.y == 0 && phys->vel.x != 0) {
				if (phys->vel.x < 0) {
					for (int i = 0; i < phys->size.y - 1; i++) {
						if (Layers::queryBlockInfo(bs, *phys->position + glm::vec2(-1 * (phys->size.x / 2.0f + 0.2), -phys->size.y / 2.0f + 1 + i))->collidableRight) {
							goUp = false;
							break;
						}
					}
					if (goUp) {
						if (Layers::queryBlockInfo(bs, *phys->position + glm::vec2(-1 * (phys->size.x / 2.0f + 0.2), -phys->size.y / 2.0f))->collidableRight) {
							phys->weight = 0;
							phys->stoponcollision = false;
							phys->position->y += 0.2;
						}
					}
				}
				else {
					for (int i = 0; i < phys->size.y - 1; i++) {
						if (Layers::queryBlockInfo(bs, *phys->position + glm::vec2((phys->size.x / 2.0f + 0.2), -phys->size.y / 2.0f + 1 + i))->collidableLeft) {
							goUp = false;
							break;
						}
					}
					if (goUp) {
						if (Layers::queryBlockInfo(bs, *phys->position + glm::vec2((phys->size.x / 2.0f + 0.2), -phys->size.y / 2.0f))->collidableLeft) {
							phys->weight = 0;
							phys->stoponcollision = false;
							phys->position->y += 0.2;
						}
					}
				}

			}
		}

		


	}
	void starfurystarUpdate(args)
	{
		ECS::getComponent<particleEmmiterC>(entity)->dir = glm::normalize(phys->vel);
		if (p->state == 0) {
			if (!(phys->collidebot || phys->collideleft || phys->collideright || phys->collidetop)) {
				p->state = 1;
			}
			return;
		}
		if (phys->collidebot || phys->collideleft || phys->collideright || phys->collidetop) {
			ECS::getComponent<mobC>(entity)->hp = 0;
		}

	}
	void slimeJump(args) {
		auto draw = ECS::getComponent<drawC>(entity);
		phys->vel.y = 0.4 + (rand() % 2) / 10.0f;
		phys->vel.x = 0.3 + (rand() % 2) / 10.0f;

		int dir = game::aiSys->getStat(p, "dir", { .intVal = rand() % 2 })->intVal;
		if (dir == 0) dir = -1;

		auto mC = ECS::getComponent<mobC>(entity);
		if (mC->hp < mC->maxhp || (globals::cdayTime > 1800)) {
			dir = utils::sign(Player::pos.x - phys->position->x);
		}
		else {
			if (phys->collideleft) dir = 1; else if(phys->collideright) dir = -1;
		}

		phys->vel.x *= dir;

		if (dir > 0) {
			if (game::aiSys->getStat(p, "flip", { .boolVal = false })->boolVal) {
				draw->flipX = true;
			}
		}
		else {
			draw->flipX = false;
		}

		animations::removeAnim(&draw->anim);
	}

	void zombieOnUpdate(args)
	{
		auto bs = Layers::getLayer("blocks");
		auto draw = ECS::getComponent<drawC>(entity);
		const char* walk = game::aiSys->getStat(p, "walkanim", { .charp = nullptr })->charp;
		if (walk != nullptr) {
			animations::watchAnim(walk, &(draw->anim), false);
		}
		float dir = Player::pos.x - phys->position->x;
		auto triedgettingtoplayer = &game::aiSys->getStat(p, "counter", { .intVal = 0 })->intVal;

		if (*triedgettingtoplayer < 0) {
			*triedgettingtoplayer += 1;
			dir *= -1;
		}

		if (globals::cdayTime > 0 && globals::cdayTime < 1800) {
			dir = -utils::sign(Player::pos.x - phys->position->x);
		}

		if (dir > 0) {
			draw->flipX = true;
			dir = 1;
		}
		else if (dir < 0) {
			draw->flipX = false;
			dir = -1;
		}

		phys->stoponcollision = true;
		phys->weight = 1;
		bool goUp = true;

		if (*triedgettingtoplayer == 8) {
			*triedgettingtoplayer = -(rand()%200 + 300);
		}
		float ms = game::aiSys->getStat(p, "ms", { .floatVal = 0.1 })->floatVal;
		phys->vel.x += utils::approach(phys->vel.x, dir * ms, 40);

		if (phys->vel.y == 0 && phys->vel.x != 0) {
			if (phys->vel.x < 0) {
				for (int i = 0; i < phys->size.y - 1; i++) {
					if (Layers::queryBlockInfo(bs, *phys->position + glm::vec2(-1 * (phys->size.x / 2.0f + 0.2), -phys->size.y / 2.0f + 1 + i))->collidableRight) {
						goUp = false;
						break;
					}
				}
				if (goUp) {
					if (Layers::queryBlockInfo(bs, *phys->position + glm::vec2(-1 * (phys->size.x / 2.0f + 0.2), -phys->size.y / 2.0f))->collidableRight) {
						phys->weight = 0;
						phys->stoponcollision = false;
						phys->position->y += 0.2;
					}
				}
			}
			else {
				for (int i = 0; i < phys->size.y - 1; i++) {
					if (Layers::queryBlockInfo(bs, *phys->position + glm::vec2((phys->size.x / 2.0f + 0.2), -phys->size.y / 2.0f + 1 + i))->collidableLeft) {
						goUp = false;
						break;
					}
				}
				if (goUp) {
					if (Layers::queryBlockInfo(bs, *phys->position + glm::vec2((phys->size.x / 2.0f + 0.2), -phys->size.y / 2.0f))->collidableLeft) {
						phys->weight = 0;
						phys->stoponcollision = false;
						phys->position->y += 0.2;
					}
				}
			}
		}

		if (!phys->collidebot)return;

		if (phys->collideright || phys->collideleft) {
			phys->vel.y = Player::jumpms * 1.2;
		}
		if (Player::pos.y > phys->position->y) {
			float disty = glm::distance(*phys->position, Player::pos);
			if (disty < 10 && disty > 4) {
				phys->vel.y = Player::jumpms * 1.2;
				*triedgettingtoplayer += 1;
			}
		}
		else {
			*triedgettingtoplayer = 0;
		}

		if (phys->vel.x < 0) {
			if (!(Layers::queryBlockInfo(bs, *phys->position + glm::vec2(-phys->size.x / 2.0f, -phys->size.y / 2.0f - 1))->notReplacable ||
				Layers::queryBlockInfo(bs, *phys->position + glm::vec2(-phys->size.x / 2.0f, -phys->size.y / 2.0f - 2))->notReplacable)) {
				phys->vel.y = Player::jumpms * 1.2;
			}
		}
		else {
			if (!(Layers::queryBlockInfo(bs, *phys->position + glm::vec2(phys->size.x / 2.0f, -phys->size.y / 2.0f - 1))->notReplacable ||
				Layers::queryBlockInfo(bs, *phys->position + glm::vec2(phys->size.x / 2.0f, -phys->size.y / 2.0f - 2))->notReplacable)) {
				phys->vel.y = Player::jumpms * 1.2;
			}
		}
	}
	void starOnUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		animations::watchAnim("fallenstar", &(draw->anim), false);
		if (p->state == 0 && phys->collidebot && draw->hasmat) {
			draw->hasmat = false;
			phys->vel = glm::vec2(0, 0.3 + (rand() % 30) / 100.0f);
			ECS::queueComponent<droppedItemC>(entity, { "fallenstar", 1, 0 });
			p->state = 1;
		}
		if (draw->hasmat) {
			draw->mat = glm::rotate(draw->mat, float(10 * PI / 180.0f), glm::vec3(0, 0, 1));
		}
	}
	void bombOnUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		float length = glm::length(phys->vel) * 30;
		if (phys->vel.x > 0) {
			length *= -1;
		}
		draw->hasmat = true;
		draw->mat = glm::rotate(draw->mat, float(length * PI / 180.0f), glm::vec3(0, 0, 1));
		auto mC = ECS::getComponent<mobC>(entity);
		auto childPhys = ECS::getComponent<physicsC>(mC->children[0]);
		if (childPhys != nullptr) *childPhys->position = *phys->position;
	}
	void testOnUpdate(args)
	{
	}
	void demoneyeOnUpdate(args)
	{
		phys->vel += utils::approach(phys->vel, glm::normalize((Player::pos - *phys->position)) * glm::vec2(0.6), 50);
		if (globals::cdayTime > 0 && globals::cdayTime < 1800) {
			phys->vel = -phys->vel;
		}
		
		auto draw = ECS::getComponent<drawC>(entity);
		float a = int(utils::angleOfVector(phys->vel)) % 360;
		draw->hasmat = true;
		draw->mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));
	}
	void sandOnUpdate(args)
	{
		if (phys->collidebot) {
			ECS::queueDeletion(entity);
			Layers::placeBlock(*phys->position, "sand");
		}
	}

	void bunnyUpdate(args) {
		auto draw = ECS::getComponent<drawC>(entity);
		if (p->state == 0) {
			auto standinganim = game::aiSys->getStat(p, "standinganim", { .charp = nullptr })->charp;
			if (standinganim != nullptr) {
				animations::watchAnim(standinganim, &draw->anim, false);
			}
			if (rand() % 100 == 0) {
				auto walkinganim = game::aiSys->getStat(p, "walkinganim", { .charp = nullptr })->charp;
				if (walkinganim != nullptr) {
					animations::watchAnim(walkinganim, &draw->anim);
				}
				if (rand() % 2 == 0) {
					draw->flipX = true;
					p->state = 1;
					phys->vel.x = 0.03 * game::aiSys->getStat(p, "ms", { .floatVal = 1 })->floatVal;
				}
				else {
					draw->flipX = false;
					p->state = 2;
					phys->vel.x = -0.03;
				}
			}
		}
		if (rand() % 40 == 0) {
			if (p->state == 1) {
				draw->flipX = true;
			}
			else {
				draw->flipX = false;
			}
			p->state = 0;
			phys->vel.x = 0;
			animations::removeAnim(&draw->anim);
		}

		phys->stoponcollision = true;
		if (phys->vel.x < 0) {
			if (!Layers::queryBlockInfo(Layers::getLayer("blocks"), *phys->position + glm::vec2(-1 * (phys->size.x / 2.0f + 0.5), phys->size.y / 2.0f))->collidableRight)
				if (Layers::queryBlockInfo(Layers::getLayer("blocks"), *phys->position + glm::vec2(-1 * (phys->size.x / 2.0f + 0.5), -phys->size.y / 2.0f))->collidableRight) {
					phys->position->y += 0.25;
					phys->vel.y = 0;
					phys->stoponcollision = false;
				}
		}
		else if (phys->vel.x > 0) {
			if (!Layers::queryBlockInfo(Layers::getLayer("blocks"), *phys->position + glm::vec2((phys->size.x / 2.0f + 0.5), phys->size.y / 2.0f))->collidableLeft)
				if (Layers::queryBlockInfo(Layers::getLayer("blocks"), *phys->position + glm::vec2((phys->size.x / 2.0f + 0.5), -phys->size.y / 2.0f))->collidableLeft) {
					phys->position->y += 0.25;
					phys->vel.y = 0;
					phys->stoponcollision = false;
				}
		}
	}

	void arrowUpdate(args)
	{
		if (p->state == 0) {
			auto draw = ECS::getComponent<drawC>(entity);
			float a = int(utils::angleOfVector(phys->vel)) % 360;
			draw->mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));
			if (phys->collidebot || phys->collideleft || phys->collideright || phys->collidetop) {
				phys->isstatic = true;
				auto dc = ECS::getComponent<droppedItemC>(entity);
				if (dc == nullptr) {
					droppedItemC dc;
					dc.amount = 1;
					dc.timer = 0;
					dc.rotate = false;
					dc.name = ECS::getComponent<drawC>(entity)->tex;
					ECS::queueComponent<droppedItemC>(entity, dc);
				}
				p->state = 1;
			}
		}

	}

	void bulletUpdate(args)
	{
		if (phys->collidebot || phys->collideleft || phys->collideright || phys->collidetop) {
			ECS::getComponent<mobC>(entity)->hp = 0;
		}
		auto draw = ECS::getComponent<drawC>(entity);
		float a = int(utils::angleOfVector(phys->vel) + 90) % 360;
		draw->mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));
	}

	void enchantedswordUpdate(args)
	{
		if (phys->collidebot || phys->collideleft || phys->collideright || phys->collidetop) {
			ECS::getComponent<mobC>(entity)->hp = 0;
		}
		auto draw = ECS::getComponent<drawC>(entity);
		float a = int(utils::angleOfVector(phys->vel) + 145) % 360;
		draw->mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));
	}

	void vilethornUpdate(args)
	{
		auto mobc = ECS::getComponent<mobC>(entity);
		auto drawc = ECS::getComponent<drawC>(entity);
		if (mobc->killin < 20) {
			drawc->opacity += utils::approach(drawc->opacity, 0, 12);
			return;
		}
		if (mobc->killin < 30) {
			if (mobc->onCollision.count(mf_ENEMY) <= 0) {
				mobc->onCollision.insert(std::make_pair(mf_ENEMY, collisionFs::damage));
			}
			drawc->opacity += utils::approach(drawc->opacity, 0.8, 4);
		}
		if (mobc->killin == 32) {
			if (mobc->onCollision.count(mf_ENEMY) <= 0) {
				mobc->onCollision.insert(std::make_pair(mf_ENEMY, collisionFs::damage));
			}
		}
	}
	void beeUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		auto mobc = ECS::getComponent<mobC>(entity);
		animations::watchAnim("bee", &draw->anim, false);
		draw->flipY = phys->vel.x > 0;
		float a = int(utils::angleOfVector(phys->vel)) % 360;
		draw->mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));
		if (phys->collidebot || phys->collideleft || phys->collideright || phys->collidetop) {
			game::aiSys->getStat(p, "counter", { .intVal = 0 })->intVal++;
		}
		if (game::aiSys->getStat(p, "counter", { .intVal = 0 })->intVal == 3) {
			ECS::getComponent<mobC>(entity)->hp = 0;
			return;
		}

		auto target = game::aiSys->getStat(p, "target", { .intVal = -1 });
		//if no target search for one
		if (target->intVal == -1) {
			auto mobarr = static_cast<ComponentArray<mobC>*>(game::mobSys->componentArray.get());
			float mindistance = 40;
			for (int i = 0; i < mobarr->components.size(); i++) {
				if (mobarr->components[i].families.count(mf_ENEMY) <= 0) continue;
				auto phys2 = ECS::getComponent<physicsC>(mobarr->componentToEntity[i]);
				float distance = glm::distance(*phys->position, *phys2->position);
				if (distance < mindistance) {
					mindistance = distance;
					target->intVal = mobarr->componentToEntity[i];
					break;
				}
			}
		}
		auto targetphys = ECS::getComponent<physicsC>(target->intVal);
		if (targetphys == nullptr) {
			target->intVal = -1;
			return;
		}

		glm::vec2 targetvec = *targetphys->position - *phys->position;
		phys->vel += utils::approach(phys->vel, glm::normalize(targetvec) * glm::vec2(0.6), 10);

	}

	void twinssummon1OnUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		auto mobc = ECS::getComponent<mobC>(entity);
		auto targetent = game::aiSys->getStat(p, "targetent", { .intVal = -1 });
		auto target = game::aiSys->getStat(p, "target", { .vec2Val = {-10, -10} });
		auto counter = game::aiSys->getStat(p, "counter", { .intVal = 0 });
		draw->flipY = phys->vel.x > 0;
		float a = int(utils::angleOfVector(phys->vel)) % 360;
		draw->mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));

		if (!Player::hasBuff("twins")) {
			mobc->hp = 0;
			Player::currsummons-=0.5;
			return;
		}

		if (p->state == 0) {
			target->vec2Val = Player::pos + glm::vec2(-6 + rand() % 12, 2 + rand() % 4);
			p->state = 2;
		}

		if (p->state == 2) {
			auto mobarr = static_cast<ComponentArray<mobC>*>(game::mobSys->componentArray.get());
			float mindistance = 24;
			for (int i = 0; i < mobarr->components.size(); i++) {
				if (mobarr->components[i].families.count(mf_ENEMY) <= 0) continue;
				auto phys2 = ECS::getComponent<physicsC>(mobarr->componentToEntity[i]);
				float distance = glm::distance(*phys->position, *phys2->position);
				if (distance < mindistance) {
					mindistance = distance;
					targetent->intVal = mobarr->componentToEntity[i];
					p->state = 1;
					break;
				}
			}

			if (glm::distance(target->vec2Val, *phys->position) < 0.5) {
				target->vec2Val = Player::pos + glm::vec2(-6 + rand() % 12, 2 + rand() % 4);
			}
		}

		if (p->state == 1) {
			auto targetphys = ECS::getComponent<physicsC>(targetent->intVal);
			if (targetphys == nullptr) {
				targetent->intVal = -1;
				p->state = 0;
			}
			else {
				target->vec2Val = *targetphys->position;
				if (glm::distance(target->vec2Val, *phys->position) < 10) {
					p->state = 4;
					glm::vec2 targetvec = target->vec2Val - *phys->position;
					phys->vel = glm::normalize(targetvec) * glm::vec2(1);
					target->vec2Val = Player::pos + glm::vec2(-6 + rand() % 12, 2 + rand() % 4);
					counter->intVal = 10;
					targetent->intVal = -1;
				}
			}
		}
		

		if (p->state == 3) {
			counter->intVal--;
			if (counter->intVal == 0) {
				p->state = 0;
			}
		}
		if (p->state == 4) {
			counter->intVal--;
			if (counter->intVal == 0) {
				p->state = 3;
				counter->intVal = 45;
			}
			return;
		}

		glm::vec2 targetvec = target->vec2Val - *phys->position;
		phys->vel += utils::approach(phys->vel, glm::normalize(targetvec) * glm::vec2(0.3), 10);
	}

	void twinssummon2OnUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		auto mobc = ECS::getComponent<mobC>(entity);
		auto targetent = game::aiSys->getStat(p, "targetent", { .intVal = -1 });
		auto target = game::aiSys->getStat(p, "target", { .vec2Val = {-10, -10} });
		auto counter = game::aiSys->getStat(p, "counter", { .intVal = 0 });
		draw->flipY = phys->vel.x > 0;
		float rotangle = 0;

		if (!Player::hasBuff("twins")) {
			mobc->hp = 0;
			Player::currsummons -= 0.5;
			return;
		}

		if (p->state == 0) {
			target->vec2Val = Player::pos + glm::vec2(-6 + rand() % 12, 2 + rand() % 4);
			p->state = 1;
		}

		if (p->state == 1) {
			if (glm::distance(target->vec2Val, *phys->position) < 0.5) {
				target->vec2Val = Player::pos + glm::vec2(-6 + rand() % 12, 2 + rand() % 4);
			}
		}

		auto targetphys = ECS::getComponent<physicsC>(targetent->intVal);
		if (targetphys == nullptr) {

			targetent->intVal = -1;
			auto mobarr = static_cast<ComponentArray<mobC>*>(game::mobSys->componentArray.get());
			float mindistance = 24;
			for (int i = 0; i < mobarr->components.size(); i++) {
				if (mobarr->components[i].families.count(mf_ENEMY) <= 0) continue;
				auto phys2 = ECS::getComponent<physicsC>(mobarr->componentToEntity[i]);
				float distance = glm::distance(*phys->position, *phys2->position);
				if (distance < mindistance) {
					mindistance = distance;
					targetent->intVal = mobarr->componentToEntity[i];
					p->state = 1;
					counter->intVal = 0;
					break;
				}
			}

			rotangle = int(utils::angleOfVector(phys->vel)) % 360;
		}
		else {
			glm::vec2 targetvec = *targetphys->position - *phys->position;
			rotangle = int(utils::angleOfVector(targetvec)) % 360;
			if (counter->intVal <= 0) {
				counter->intVal = 60;
				//shoot
				{
					int e = ECS::newEntity();
					projectileBase projectile = items::projectiles["miniretinalaser"];
					projectile.dc.parent = globals::projectileLayer;
					projectile.pc.prevpos = phys->prevpos;
					auto ppos = std::make_shared<glm::vec2>(*phys->position);
					projectile.dc.position = ppos;
					projectile.pc.position = ppos;
					projectile.pc.vel = glm::normalize(targetvec) * glm::vec2(1);

					drawSystem::addComponent(e, &projectile.dc, false);
					ECS::queueComponent<physicsC>(e, projectile.pc);
					ECS::queueComponent<mobC>(e, projectile.mc);
					ECS::queueComponent<aiC>(e, projectile.ac);
				}
			}
			counter->intVal--;
		}

		glm::vec2 targetvec = target->vec2Val - *phys->position;
		phys->vel += utils::approach(phys->vel, glm::normalize(targetvec) * glm::vec2(0.3), 10);

		draw->mat = glm::rotate(glm::mat4(1.0f), float(rotangle * PI / 180.0f), glm::vec3(0, 0, 1));

	}

	void impOnUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		auto mobc = ECS::getComponent<mobC>(entity);
		auto pec = ECS::getComponent<particleEmmiterC>(entity);
		pec->parent = globals::topparticleLayer;
		auto targetent = game::aiSys->getStat(p, "targetent", { .intVal = -1 });
		auto target = game::aiSys->getStat(p, "target", { .vec2Val = {-10, -10} });
		auto counter = game::aiSys->getStat(p, "counter", { .intVal = 0 });
		draw->flipX = phys->vel.x > 0;

		animations::watchAnim("imp", &draw->anim, false);

		if (!Player::hasBuff("imp")) {
			mobc->hp = 0;
			Player::currsummons -= 1;
			return;
		}

		if (p->state == 0) {
			target->vec2Val = Player::pos + glm::vec2(-6 + rand() % 12, 2 + rand() % 4);
			p->state = 1;
		}

		if (p->state == 1) {
			if (glm::distance(target->vec2Val, *phys->position) < 0.5) {
				target->vec2Val = Player::pos + glm::vec2(-6 + rand() % 12, 2 + rand() % 4);
			}
		}

		auto targetphys = ECS::getComponent<physicsC>(targetent->intVal);
		if (targetphys == nullptr) {

			targetent->intVal = -1;
			auto mobarr = static_cast<ComponentArray<mobC>*>(game::mobSys->componentArray.get());
			float mindistance = 24;
			for (int i = 0; i < mobarr->components.size(); i++) {
				if (mobarr->components[i].families.count(mf_ENEMY) <= 0) continue;
				auto phys2 = ECS::getComponent<physicsC>(mobarr->componentToEntity[i]);
				float distance = glm::distance(*phys->position, *phys2->position);
				if (distance < mindistance) {
					mindistance = distance;
					targetent->intVal = mobarr->componentToEntity[i];
					p->state = 1;
					counter->intVal = 0;
					break;
				}
			}
		}
		else {
			glm::vec2 targetvec = *targetphys->position - *phys->position;
			if (counter->intVal <= 0) {
				counter->intVal = 60;
				//shoot
				{
					int e = ECS::newEntity();
					projectileBase projectile = items::projectiles["impfireball"];
					projectile.dc.parent = globals::projectileLayer;
					projectile.pc.prevpos = phys->prevpos;
					auto ppos = std::make_shared<glm::vec2>(*phys->position);
					projectile.dc.position = ppos;
					projectile.pc.position = ppos;
					projectile.pc.vel = glm::normalize(targetvec) * glm::vec2(1);

					drawSystem::addComponent(e, &projectile.dc, false);
					ECS::queueComponent<physicsC>(e, projectile.pc);
					ECS::queueComponent<mobC>(e, projectile.mc);
					ECS::queueComponent<aiC>(e, projectile.ac);
					ECS::queueComponent<particleEmmiterC>(e, projectile.pec);
				}
			}
			counter->intVal--;
		}

		glm::vec2 targetvec = target->vec2Val - *phys->position;
		phys->vel += utils::approach(phys->vel, glm::normalize(targetvec) * glm::vec2(0.3), 20);

		const float maxangle = 50;
		float strength = phys->vel.x / 0.5f;
		strength = glm::clamp(strength, -0.5f, 0.5f);
		float angle = strength * maxangle;
		draw->mat = glm::rotate(glm::mat4(1.0f), float(angle * PI / 180.0f), glm::vec3(0, 0, 1));


	}

	void damageTextOnUpdate(args)
	{
		auto counter = game::aiSys->getStat(p, "counter", { .intVal = 0 });
		auto draw = ECS::getComponent<drawC>(entity);
		phys->vel += utils::approach(phys->vel, glm::vec2(0), 10 + rand()% 60);
		counter->floatVal += 2.5;
		if (counter->floatVal > 60 * 2.5) {
			if (draw->opacity == 0) {
				ECS::getComponent<mobC>(entity)->hp = 0;
			}
			draw->opacity += utils::approach(draw->opacity, 0, 20);
		}
		else {
			draw->opacity = abs(sin(counter->floatVal * PI / 180.0f)) / 2.0f + 0.5;
		}
	}

	void guideOnClick(args)
	{
		gameLoop::openDialogue("guide");
	}

	void slimeFireProjectile(args)
	{
		auto proj = game::aiSys->getStat(p, "projectile", { .charp = nullptr })->charp;
		if (proj == nullptr) return;
		int num = rand() % 2 + 1;
		for (int i = 0; i < num; i++) {
			int e = ECS::newEntity();

			glm::vec2 targetvec = glm::vec2(0, 1);
			float randangle = (rand() % 60) - 30;
			targetvec.x = -sin(randangle * PI / 180.0f);
			targetvec.y =  cos(randangle * PI / 180.0f);
			
			projectileBase projectile = items::projectiles[proj];
			projectile.dc.parent = globals::projectileLayer;
			projectile.pc.prevpos = phys->prevpos;
			auto ppos = std::make_shared<glm::vec2>(*phys->position);
			projectile.dc.position = ppos;
			projectile.pc.position = ppos;
			projectile.pc.vel = glm::normalize(targetvec) * glm::vec2(0.67);

			drawSystem::addComponent(e, &projectile.dc, false);
			ECS::queueComponent<physicsC>(e, projectile.pc);
			ECS::queueComponent<mobC>(e, projectile.mc);
			ECS::queueComponent<aiC>(e, projectile.ac);
			ECS::queueComponent<particleEmmiterC>(e, projectile.pec);
		}
	}

	void eaterOfSoulsOnUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		animations::watchAnim("eaterofsouls", &draw->anim, false);
		float a = int(utils::angleOfVector(glm::normalize(Player::pos - *phys->position)) - 90) % 360;
		draw->hasmat = true;
		draw->mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));

		if (p->state == 0) {
			glm::vec2* target = &game::aiSys->getStat(p, "target", { .vec2Val = glm::vec2(0) })->vec2Val;
			phys->vel += utils::approach(phys->vel, glm::normalize((Player::pos + *target - *phys->position)) * glm::vec2(0.6), 180);

			if ((glm::distance(*phys->position, Player::pos + *target) < 5) || Layers::queryBlockInfo(Layers::getLayer("blocks"), Player::pos + *target)->notReplacable) {
				*target = glm::vec2((rand() % 1000) - 500, (rand() % 1000) - 500);
				*target = glm::normalize(*target) * glm::vec2(15);
			}

			if (rand() % 600 == 0) {
				p->state = 1;
			}
			return;
		}

		if (p->state == 1) {
			phys->vel += utils::approach(phys->vel, glm::normalize((Player::pos - *phys->position)) * glm::vec2(0.6), 120);
			if (glm::distance(*phys->position, Player::pos) < 5) {
				p->state = 0;
			}
			return;
		}
	}

	void batOnUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);


		if (glm::distance(*phys->position, Player::pos) > 50) {
			if (Layers::queryBlockInfo(Layers::getLayer("blocks"), *phys->position + glm::vec2(0, 1))->collidableBot) {
				p->state = 1;
				const char* hangingtex = game::aiSys->getStat(p, "hangingtex", { .charp = nullptr })->charp;
				if (hangingtex != nullptr) {
					animations::watchAnim(hangingtex, &draw->anim, false);
				}
				phys->isstatic = true;
				draw->hasmat = false;
			}
		}
		else {
			p->state = 0;
		}
		

		if (p->state == 0) {
			const char* flyingtex = game::aiSys->getStat(p, "flyingtex", { .charp = nullptr })->charp;
			if (flyingtex != nullptr) {
				animations::watchAnim(flyingtex, &draw->anim, false);
			}
			float a = int(utils::angleOfVector(phys->vel)) % 360;
			phys->isstatic = false;
			draw->hasmat = true;

			if (phys->vel.x < 0) {
				draw->flipX = false;
			}
			else
			{
				draw->flipX = true;
				a += 180;
			}

			if (a > 25 && a < 180) a = 25;
			if (a > 365 && a > 180) a = 365;
			draw->mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));

			glm::vec2* target = &game::aiSys->getStat(p, "target", { .vec2Val = glm::vec2(0) })->vec2Val;
			phys->vel += utils::approach(phys->vel, glm::normalize((Player::pos + *target - *phys->position)) * glm::vec2(0.6), 120);
			if ((glm::distance(*phys->position, Player::pos + *target) < 5) || Layers::queryBlockInfo(Layers::getLayer("blocks"), Player::pos + *target)->notReplacable) {
				*target = glm::vec2(((rand() % 1000)/500.0f) - 1, ((rand() % 1000) / 500.0f) - 1);
				*target *= game::aiSys->getStat(p, "followrange", {.floatVal = 6})->floatVal;
			}
			if (rand() % 600 == 0) {
				*target = glm::vec2(((rand() % 1000) / 500.0f) - 1, ((rand() % 1000) / 500.0f) - 1);
				*target *= 60;
			}

			return;
		}
	}

	void demonShoot(args)
	{
		for (int i = 0; i < 3; i++) {
			int e = ECS::newEntity();

			glm::vec2 targetvec = Player::pos - *phys->position;

projectileBase projectile = items::projectiles["demonscythe"];
projectile.dc.parent = globals::projectileLayer;
projectile.pc.prevpos = phys->prevpos;
auto ppos = std::make_shared<glm::vec2>(*phys->position);
projectile.dc.position = ppos;
projectile.pc.position = ppos;
projectile.pc.vel = glm::normalize(targetvec) * glm::vec2(0.01);
projectile.ac.stats["timer"].intVal = i * 30 + 30;

drawSystem::addComponent(e, &projectile.dc, false);
ECS::queueComponent<physicsC>(e, projectile.pc);
ECS::queueComponent<mobC>(e, projectile.mc);
ECS::queueComponent<aiC>(e, projectile.ac);
ECS::queueComponent<particleEmmiterC>(e, projectile.pec);
		}
	}

	void hornetShoot(args)
	{
		int e = ECS::newEntity();

		glm::vec2 targetvec = Player::pos - *phys->position;

		projectileBase projectile = items::projectiles["hornetsting"];
		projectile.dc.parent = globals::projectileLayer;
		projectile.pc.prevpos = phys->prevpos;
		auto ppos = std::make_shared<glm::vec2>(*phys->position);
		projectile.dc.position = ppos;
		projectile.pc.position = ppos;
		projectile.pc.vel = glm::normalize(targetvec) * glm::vec2(0.8);

		drawSystem::addComponent(e, &projectile.dc, false);
		ECS::queueComponent<physicsC>(e, projectile.pc);
		ECS::queueComponent<mobC>(e, projectile.mc);
		ECS::queueComponent<aiC>(e, projectile.ac);
		ECS::queueComponent<particleEmmiterC>(e, projectile.pec);
	}

	void demonscytheOnUpdate(args)
	{

		if (phys->collidebot || phys->collideleft || phys->collideright || phys->collidetop) {
			ECS::queueDeletion(entity);
			return;
		}

		auto draw = ECS::getComponent<drawC>(entity);
		draw->mat = glm::rotate(draw->mat, (float)(18 * PI / 180.0f), glm::vec3(0, 0, 1));
		if (p->state == 0) {
			int* timer = &game::aiSys->getStat(p, "timer", { .intVal = 0 })->intVal;
			if (*timer > 0) {
				(*timer)--;
				return;
			}
			else {
				p->state = 1;
			}
		}

		ECS::getComponent<particleEmmiterC>(entity)->dir = glm::normalize(phys->vel);
		if (glm::length(phys->vel) < 2) {
			phys->vel *= 1.04;
		}
	}

	void vultureOnUpdate(args)
	{
		if (glm::distance(Player::pos, *phys->position) < 15) p->state = 1;
		if (p->state != 1) return;

		auto draw = ECS::getComponent<drawC>(entity);
		animations::watchAnim("vulturefly", &draw->anim, false);

		if (phys->vel.x < 0) {
			draw->flipX = false;
		}
		else
		{
			draw->flipX = true;
		}

		glm::vec2* target = &game::aiSys->getStat(p, "target", { .vec2Val = glm::vec2(0) })->vec2Val;
		phys->vel += utils::approach(phys->vel, glm::normalize((Player::pos + *target - *phys->position)) * glm::vec2(0.5), 70);
		if (glm::distance(*phys->position, Player::pos + *target) < 5) {
			*target = glm::vec2(0, rand() % 5);
			if (!(rand() % 15)) {
				target->y *= 2;
			}
		}
	}

	void casterTeleport(args)
	{
		std::vector<glm::vec2> candidates;
		auto bs = Layers::getLayer("blocks");
		for (int x = Player::pos.x - 20; x < Player::pos.x + 40; x++) {
			for (int y = Player::pos.y - 20; y < Player::pos.y + 40; y++) {
				if (!Layers::queryBlockInfo(bs, { x,y })->notReplacable) continue;
				if(Layers::isAreaEmpty({ x,y+1 }, { 4,3 })){
					candidates.push_back({ x,y + phys->size.y/2 });
				}
			}
		}
		if (candidates.size() == 0) return;
		*phys->position = candidates[rand() % candidates.size()];
		auto draw = ECS::getComponent<drawC>(entity);
		animations::removeAnim(&draw->anim);
	}

	void casterShoot(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		animations::watchAnim("darkcastercast", &draw->anim, true, false, draw->flipX);
		auto proj = game::aiSys->getStat(p, "projectile", { .charp = nullptr })->charp;
		if (proj == nullptr) return;
		int num = rand() % 2 + 1;
		int e = ECS::newEntity();

		glm::vec2 targetvec = glm::normalize(Player::pos - *phys->position);

		projectileBase projectile = items::projectiles[proj];
		projectile.dc.parent = globals::projectileLayer;
		projectile.pc.prevpos = phys->prevpos;
		auto ppos = std::make_shared<glm::vec2>(*phys->position);
		projectile.dc.position = ppos;
		projectile.pc.position = ppos;
		projectile.pc.vel = glm::normalize(targetvec) * glm::vec2(0.4);

		drawSystem::addComponent(e, &projectile.dc, false);
		ECS::queueComponent<physicsC>(e, projectile.pc);
		ECS::queueComponent<mobC>(e, projectile.mc);
		ECS::queueComponent<aiC>(e, projectile.ac);
		ECS::queueComponent<particleEmmiterC>(e, projectile.pec);
		p->actions.push_back({ rand()%30 + 100, mobFunctions::casterShoot, 1});
	}

	void wormHeadOnUpdate(args)
	{
		auto target = &game::aiSys->getStat(p, "target", { .vec2Val = glm::vec2(0, 0) })->vec2Val;
		if (Layers::queryBlockInfo(Layers::getLayer("blocks"), *phys->position)->notReplacable) {
			phys->vel += utils::approach(phys->vel, glm::normalize(Player::pos + *target - *phys->position), 200);
		}
		else {
			phys->vel += utils::approach(phys->vel, glm::vec2(0, -1), 120);
		}
		
		if (glm::distance(Player::pos + *target, *phys->position) < 5 ||rand()%300==0) {
			*target = glm::vec2(rand() % 60 - 30, rand() % 60 - 30);
		}
		auto draw = ECS::getComponent<drawC>(entity);
		float a = int(utils::angleOfVector(glm::normalize(phys->vel)) + 90) % 360;
		draw->hasmat = true;
		draw->mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));

	}

	void wormCreateBody(args)
	{
		int parent = entity;
		int length = game::aiSys->getStat(p, "length", { .intVal = 0 })->intVal;
		const char* body = game::aiSys->getStat(p, "body", { .charp = nullptr })->charp;
		const char* tail = game::aiSys->getStat(p, "tail", { .charp = nullptr })->charp;
		bool redirectdmg = game::aiSys->getStat(p, "redirectdmgtohead", { .boolVal = true })->boolVal;
		if (body == nullptr || tail == nullptr) return;
		
		for (int i = 0; i < length; i++) {
			int ent = enemies::spawnEnemy(body, * phys->position);
			ECS::getComponent<aiC>(ent)->stats["parent"].intVal = parent;
			if(redirectdmg)
				ECS::getComponent<mobC>(ent)->directDamageTo = entity;
			parent = ent;
		}
		int ent = enemies::spawnEnemy(tail, * phys->position);
		ECS::getComponent<aiC>(ent)->stats["parent"].intVal = parent;
		if (redirectdmg)
			ECS::getComponent<mobC>(ent)->directDamageTo = entity;
	}

	void wormBodyOnUpdate(args)
	{
		auto mob = ECS::getComponent<mobC>(entity);
		auto draw = ECS::getComponent<drawC>(entity);
		int parent = game::aiSys->getStat(p, "parent", { .intVal = -1 })->intVal;
		if (parent == -1) { mob->hp = 0; return; }
		auto parentphys = ECS::getComponent<physicsC>(parent);
		auto parentmob = ECS::getComponent<mobC>(parent);
		if(parentphys == nullptr) { ECS::getComponent<mobC>(entity)->hp = 0; return; }

		float avgdist = (((phys->size.x + phys->size.y ) / 2.0f) + ((parentphys->size.x + parentphys->size.y) / 2.0f))/2.0f * 0.9;
		if (glm::distance(*phys->position, *parentphys->position) > avgdist * 0.9) {
			*phys->position = * parentphys->position - glm::normalize(*parentphys->position - *phys->position) * glm::vec2(avgdist);
			float a = int(utils::angleOfVector(glm::normalize(*parentphys->position - *phys->position)) + 90) % 360;
			draw->hasmat = true;
			draw->mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));
		}
	
		if (parentmob->hp < mob->hp) mob->hp = parentmob->hp;
	}

	void eyeofcthuluOnUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		auto mob = ECS::getComponent<mobC>(entity);
		auto pec = ECS::getComponent<particleEmmiterC>(entity);
		pec->parent = globals::topparticleLayer;
		auto target = game::aiSys->getStat(p, "target", { .vec2Val = glm::vec2(0) });
		auto counter = game::aiSys->getStat(p, "counter", { .intVal = 0 });
		auto angle = game::aiSys->getStat(p, "angle", { .floatVal = 0.1 });

		animations::watchAnim("eyeofcthulu1", &draw->anim, false);

		//0 -> gre nad tabo pa spawna une uckote
		//1 -> trikrat ga dasha pase vrne nazaj u 0
		//2 -> se vrti k se spreminja u drug state
		//3 -> expert mode dash sam brez unga efekta, alpa nvm ce ga loh z particli nardim
		//4 -> mal pavze
		//5 ->ceje dan zbezi

		if (globals::cdayTime > 0 && globals::cdayTime < 1800) {
			phys->vel += utils::approach(phys->vel, glm::vec2(0, 2), 120);
			return;
		}

		float ms = 0.2;
		if (p->state == 0) {
			target->vec2Val = Player::pos + glm::vec2(0, 20);
			counter->intVal += 1;
			if (counter->intVal % 100 == 0) {
				enemies::spawnEnemy("servantofcthulu", *phys->position);
			}
			if (counter->intVal == 450) {
				p->state = 1;
				counter->intVal = 0;
				target->vec2Val = *phys->position;
			}
			phys->vel += utils::approach(phys->vel, glm::normalize(target->vec2Val - *phys->position) * glm::vec2(ms), 120);
		}

		if (p->state == 1) {
			float ms = 0.35;
			if (glm::distance(target->vec2Val, *phys->position) < 1) {

				target->vec2Val = *phys->position + glm::normalize(Player::pos - *phys->position) * (glm::distance(Player::pos, *phys->position) + 20);
				counter->intVal += 1;;
				phys->vel = glm::normalize(target->vec2Val - *phys->position) * glm::vec2(ms);
			}
			if (counter->intVal == 4) {
				counter->intVal = 0;
				p->state = 0;
			}
		}

		if (mob->hp < mob->maxhp / 2.0f && p->state < 2) {
			p->state = 2;
			mob->defense = 0;
			mob->damage *= 1.5;
		}

		if (p->state == 2) {
			angle->floatVal *= 1.07;
			if (angle->floatVal > 50000) {
				p->state = 4;
				counter->intVal = 0;
				animations::watchAnim("eyeofcthulu2", &draw->anim);
				pec->rate /= 2;
			}
		}
		else if(p->state > 2) {
			angle->floatVal += utils::approach(angle->floatVal, 0, 20);
		}

		if (p->state == 3) {
			float ms = 0.95;
			if (glm::distance(target->vec2Val, *phys->position) < 4 || glm::distance(Player::pos, *phys->position) > 20) {
				counter->intVal += 1;;
				glm::vec2 tmp = glm::normalize(Player::pos - *phys->position);
				float a = int(utils::angleOfVector(tmp)) % 360;
				a = rand()%30 - 15;
				float cosa = cos(a * PI / 180.0f);
				float sina = sin(a * PI / 180.0f);
				tmp = { cosa * tmp.x - sina * tmp.y, sina * tmp.x + cosa * tmp.y };
				target->vec2Val = *phys->position +tmp * (glm::distance(Player::pos, *phys->position) + 20);
				
				//hocmo da zadnih 10% hpja skos dasha
				float ratio = mob->hp / (float)mob->maxhp;
				ratio = glm::clamp(ratio * 2, 0.1f, 1.0f); //0.1 - 1
				//zdj rab bit da k je 0.1 da je enako 3 pa pr 1 da je enako 0
				ratio = (1 - ratio) * 3.333;
				if (counter->intVal > 4 - ratio) {
					ms = 1.6;
				}
				if(counter->intVal > 3){
					if (rand() % 2 == 0) {
						p->state = 4;
						counter->intVal = 0;
					}
				}
				phys->vel = glm::normalize(target->vec2Val - *phys->position) * glm::vec2(ms);
			}
		}

		if (glm::length(phys->vel)> 1.3) {
			glm::vec2 neki = Player::pos - *phys->position;
			particles::spawnEffect("eyeofcthuludash", *phys->position, glm::vec2(neki.y, -neki.x));
		}

		if (p->state == 4) {
			ms = 1;
			angle->floatVal += utils::approach(angle->floatVal, 0, 20);
			counter->intVal += 1;
			if (counter->intVal > 720 * (float)mob->hp / mob->maxhp) {
				counter->intVal = 0;
				p->state = 3;
			}
			phys->vel += utils::approach(phys->vel, glm::normalize(target->vec2Val - *phys->position) * glm::vec2(ms), 120);
		}


		float a = int(utils::angleOfVector(glm::normalize(Player::pos - *phys->position)) - 90) % 360;
		draw->hasmat = true;
		draw->mat = glm::rotate(glm::mat4(1.0f), float((a+angle->floatVal) * PI / 180.0f), glm::vec3(0, 0, 1));
	}

	void eaterofworldsHeadOnUpdate(args)
	{
		auto target = &game::aiSys->getStat(p, "target", { .vec2Val = glm::vec2(0, -40) })->vec2Val;
		bool offscreen = glm::distance(Player::pos, *phys->position) > 45;
		
		if(p->state != 2){
			if (offscreen || Layers::queryBlockInfo(Layers::getLayer("blocks"), *phys->position)->notReplacable) {
				phys->vel += utils::approach(phys->vel, glm::normalize(Player::pos + *target - *phys->position) * glm::vec2(1.5f), 120);
			}
			else {
				phys->vel += utils::approach(phys->vel, glm::vec2(0, -1), 120);
			}
			if (glm::distance(Player::pos + *target, *phys->position) < 5) {
				if (p->state == 0) {
					*target = glm::vec2(rand() % 10 - 5, rand() % 80 - 40);
					p->state = 1;
				}
				else {
					*target = glm::vec2(rand() % 20 - 10, rand() % 20);
					p->state = 0;
				}
			}
			if (!(rand() % 300)) {
				*target = glm::vec2(rand() % 10 - 5, rand() % 50 - 80);
				p->state = 1;
			}
		}
		
		auto draw = ECS::getComponent<drawC>(entity);
		float a = int(utils::angleOfVector(glm::normalize(phys->vel)) + 90) % 360;
		draw->hasmat = true;
		draw->mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));

	}

	void cloudOnUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		glm::vec2 off = game::aiSys->getStat(p, "off", { .vec2Val = *phys->position - Player::pos })->vec2Val;
		auto timer = game::aiSys->getStat(p, "timer", { .floatVal = 0 });
		if (timer->intVal > 10000) {
			draw->opacity += utils::approach(draw->opacity, 0, 800);
			if (draw->opacity < 0.01) {
				ECS::getComponent<mobC>(entity)->hp = 0;
			}
		}
		else {
			draw->opacity += utils::approach(draw->opacity, 1, 800);
		}
		timer->floatVal++;
		
		*draw->position = Player::pos + off;
	}

	void bladeOfGrassOnUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		animations::watchAnim("bladeofgrassprojectile", &draw->anim, false);
		float a = int(utils::angleOfVector(phys->vel)) % 360;
		draw->mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));

		auto dir = game::aiSys->getStat(p, "dir", {.intVal = (rand() % 2) == 0 ? 1 : -1 });
		dir->intVal += 1 * utils::sign(dir->intVal);
		if (rand() % 2 == 0) {
			dir->intVal += 1;
		}

		utils::rotateVecByAngleRad(&phys->vel, dir->intVal/10.0f);
		phys->vel = glm::normalize(phys->vel);
		phys->vel *= 0.8;
	}

	void fireflyOnUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		if (p->state == 0) {
			animations::watchAnim("firefly", &draw->anim, true);
			p->state = 1;
		}
		if (p->state == 2)
		{
			draw->opacity += utils::approach(draw->opacity, 0.5, 300);
			if (draw->opacity < 0.55) {
				animations::watchAnim("firefly", &draw->anim, true);
				if (rand() % 100 == 0) {
					animations::watchAnim("fireflylit", &draw->anim, true);
					p->state = 1;
				}
			}
		}

		if (p->state == 1) {
			draw->opacity += utils::approach(draw->opacity, 1.0f, 300);
			if (draw->opacity > 0.95) {
				animations::watchAnim("firefly", &draw->anim, true);
				p->state = 2;
			}
		}

		auto dir = game::aiSys->getStat(p, "dir", { .intVal = rand() % 2 })->intVal;
		if (dir == 0) {
			utils::rotateVecByAngleRad(&phys->vel, (rand() % 4) / 100.0f);
		}
		else {
			utils::rotateVecByAngleRad(&phys->vel, (rand() % 4) / -100.0f );
		}
		draw->flipX = phys->vel.x > 0;

		if (phys->vel != glm::vec2(0)) {
			phys->vel = glm::normalize(phys->vel);
			phys->vel *= 0.004;
		}
	}

	void tombstoneOnUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		float length = glm::length(phys->vel) * 30;
		if (phys->vel.x > 0) {
			length *= -1;
		}
		draw->hasmat = true;
		draw->mat = glm::rotate(draw->mat, float(length * PI / 180.0f), glm::vec3(0, 0, 1));

		auto bs = Layers::getLayer("blocks");
		if (Layers::queryBlockInfo(bs, *phys->position + glm::vec2(1, -1))->notReplacable) return;
		if (Layers::queryBlockInfo(bs, round(*phys->position) + glm::vec2(-1, -1))->notReplacable) return;
		if (!Layers::queryBlockInfo(bs, round(*phys->position) + glm::vec2(-1, -2))->notReplacable) return;
		if (!Layers::queryBlockInfo(bs, round(*phys->position) + glm::vec2(1, -2))->notReplacable) return;
		Layers::placeBlock(round(*phys->position + glm::vec2(0, -1)), "gravestone");
		ECS::getComponent<mobC>(entity)->hp = 0;
	}

	void eaterofworldsbodyOnUpdate(args)
	{
		auto mob = ECS::getComponent<mobC>(entity);
		auto draw = ECS::getComponent<drawC>(entity);
		int parent = game::aiSys->getStat(p, "parent", { .intVal = -1 })->intVal;
		if (parent == -1) { p->onupdate = eaterofworldsHeadOnUpdate; draw->size = glm::vec2(5); draw->tex = "eaterofworldshead"; return; }
		auto parentphys = ECS::getComponent<physicsC>(parent);
		auto parentmob = ECS::getComponent<mobC>(parent);
		if (parentphys == nullptr) { p->onupdate = eaterofworldsHeadOnUpdate; draw->size = glm::vec2(5); draw->tex = "eaterofworldshead"; return; }

		float avgdist = (((phys->size.x + phys->size.y) / 2.0f) + ((parentphys->size.x + parentphys->size.y) / 2.0f)) / 2.0f * 0.9;
		if (glm::distance(*phys->position, *parentphys->position) > avgdist) {
			*phys->position = *parentphys->position - glm::normalize(*parentphys->position - *phys->position) * glm::vec2(avgdist);
		}
		float a = int(utils::angleOfVector(glm::normalize(*parentphys->position - *phys->position)) + 90) % 360;
		draw->hasmat = true;
		draw->mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));
	}

	void waterboltUpdate(args)
	{
		ECS::getComponent<particleEmmiterC>(entity)->dir = -glm::normalize(phys->vel);
	}

	void aquaboltUpdate(args)
	{
		auto pec = ECS::getComponent<particleEmmiterC>(entity);
		if (pec != nullptr) {
			pec->dir = glm::normalize(phys->vel);
		}
		if (phys->collidebot || phys->collideleft || phys->collideright || phys->collidetop) {
			ECS::getComponent<mobC>(entity)->hp = 0;
		}
	}

	void magicprojectileUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		draw->hasmat = true;
		
		glm::vec2 moveVector = globals::mouseBlockCoordsZoomed(false) - *phys->position;
		if (p->state == 0 && (input::held(k_PRIMARY) || input::pressed(k_PRIMARY))) {
			float a = int(utils::angleOfVector(phys->vel) + 90) % 360;
			draw->mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));
			phys->vel = moveVector * glm::vec2(0.3f);
			if (glm::length(phys->vel) > 1.5) {
				phys->vel = glm::normalize(phys->vel) * glm::vec2(1.9);
			}
		}
		else {
			p->state = 1;
			phys->vel = glm::normalize(phys->vel) * glm::vec2(1.9);
		}
		
		float len = glm::length(phys->vel);
		if (len > 0.3) {
			draw->size.x += utils::approach(draw->size.x, 0.1, 10);
		}
		else {
			draw->size.x += utils::approach(draw->size.x, 2, 5);
		}

		if (phys->collidebot || phys->collideleft || phys->collideright || phys->collidetop) {
			ECS::getComponent<mobC>(entity)->hp = 0;
		}
	}

	void particleOnUpdate(args)
	{
		auto killin = &game::aiSys->getStatFast(p, "killin")->intVal;
		(*killin)--;
		if (*killin <= 0) {
			ECS::queueDeletion(entity);
			return;
		}

		auto dc = ECS::getComponent<drawC>(entity);
		float rotation = game::aiSys->getStatFast(p, "rotation")->floatVal;
		if (rotation != 0) {
			dc->hasmat = true;
			dc->mat = glm::rotate(dc->mat, (float)(rotation * PI / 180), glm::vec3(0, 0, 1));
		}
		dc->opacity += game::aiSys->getStat(p, "opacity")->floatVal;
		float size = game::aiSys->getStatFast(p, "size")->floatVal;
		dc->size -= size;
		phys->size = dc->size;
		phys->vel += game::aiSys->getStatFast(p, "velovertime")->vec2Val;


		glm::vec3 light = game::aiSys->getStatFast(p, "light")->vec3Val;
		if (light.r > 0 || light.g > 0 || light.b > 0)
			Layers::addLight(*(phys->position), light);
	}


	void ondie_nothing(mobDeathargs) {

	}

	void ondie_waterbolt(mobDeathargs) {
		int waterexplosion = ECS::newEntity();
		particleEmmiterC pec;
		pec.amount = 40;
		pec.tex = "waterbolt";
		pec.lifespan = 1;
		pec.rate = 1;
		pec.ms = 0.1;
		pec.dir = { 0, 1 };
		pec.randangle = 80;
		pec.particleLifespan = 60;
		pec.randomizelifespan = 20;
		pec.smallerAsDie = true;
		pec.randrotation = 40;
		pec.weight = 0.8;
		pec.randomizems = 0.2;
		pec.stoponcollision = false;
		pec.size = { .6,.6 };
		pec.randomizescale = 0.1;
		mobC mc;
		mc.hp = 1;
		mc.killin = 2;
		physicsC pc;
		glm::vec2 pos = *phys->position;
		pc.position = std::make_shared<glm::vec2>(pos);
		ECS::queueComponent<mobC>(waterexplosion, mc);
		ECS::queueComponent<particleEmmiterC>(waterexplosion, pec);
		ECS::queueComponent<physicsC>(waterexplosion, pc);
	}
	void ondie_aquabolt(mobDeathargs) {
		int waterexplosion = ECS::newEntity();
		particleEmmiterC pec;
		pec.amount = 4;
		pec.tex = "aquabolt";
		pec.lifespan = 2;
		pec.rate = 1;
		pec.ms = 0.03;
		pec.dir = { 0, 1 };
		pec.randangle = 360;
		pec.particleLifespan = 40;
		pec.randomizelifespan = 10;
		pec.smallerAsDie = true;
		pec.randrotation = 40;
		pec.weight = 0.7;
		pec.randomizems = 0.2;
		pec.stoponcollision = false;
		pec.size = { .35,.35 };
		pec.randomizescale = 0.1;
		physicsC pc;
		glm::vec2 pos = *phys->position;
		pc.position = std::make_shared<glm::vec2>(pos);
		ECS::queueComponent<particleEmmiterC>(waterexplosion, pec);
		ECS::queueComponent<physicsC>(waterexplosion, pc);
	}
	void ondie_magicprojectile(mobDeathargs) {
		int waterexplosion = ECS::newEntity();
		particleEmmiterC pec;
		pec.amount = 4;
		pec.rate = 1;
		pec.tex = "particle:magicpart";
		pec.lifespan = 2;
		pec.ms = 0.03;
		pec.dir = { 0, 1 };
		pec.randangle = 360;
		pec.particleLifespan = 40;
		pec.randomizelifespan = 10;
		pec.smallerAsDie = true;
		pec.randrotation = 40;
		pec.weight = 0.7;
		pec.randomizems = 0.2;
		pec.stoponcollision = false;
		pec.size = { .35,.35 };
		pec.randomizescale = 0.1;
		physicsC pc;
		glm::vec2 pos = *phys->position;
		pc.position = std::make_shared<glm::vec2>(pos);
		ECS::queueComponent<particleEmmiterC>(waterexplosion, pec);
		ECS::queueComponent<physicsC>(waterexplosion, pc);
	}
	void ondie_bullet(mobDeathargs)
	{
		int dirt = ECS::newEntity();
		particleEmmiterC pec;
		pec.amount = 10;
		pec.rate = 1;
		pec.tex = "dirtparticle";
		pec.lifespan = 1;
		pec.ms = 0.1;
		pec.dir = -glm::normalize(phys->vel);
		pec.randangle = 30;
		pec.particleLifespan = 30;
		pec.randomizelifespan = 10;
		pec.smallerAsDie = false;
		pec.randrotation = 40;
		pec.weight = 1.2;
		pec.randomizems = 0.08;
		pec.stoponcollision = true;
		pec.size = { 0.2,0.2 };
		pec.randomizescale = 0.15;
		mobC mc;
		mc.hp = 1;
		mc.killin = 2;
		physicsC pc;
		glm::vec2 pos = *phys->position;
		pc.dontcollide = true;
		pc.isstatic = true;
		pc.position = std::make_shared<glm::vec2>(pos);
		ECS::queueComponent<mobC>(dirt, mc);
		ECS::queueComponent<particleEmmiterC>(dirt, pec);
		ECS::queueComponent<physicsC>(dirt, pc);
	}
	void ondie_starfurystar(mobDeathargs)
	{
		int waterexplosion = ECS::newEntity();
		particleEmmiterC pec;
		pec.amount = 4;
		pec.rate = 1;
		pec.tex = "particle:star1part";
		pec.lifespan = 2;
		pec.ms = 0.03;
		pec.dir = { 0, 1 };
		pec.randangle = 360;
		pec.particleLifespan = 40;
		pec.randomizelifespan = 10;
		pec.smallerAsDie = true;
		pec.randrotation = 40;
		pec.weight = 0.7;
		pec.randomizems = 0.2;
		pec.stoponcollision = false;
		pec.size = { .35,.35 };
		pec.randomizescale = 0.1;
		physicsC pc;
		glm::vec2 pos = *phys->position;
		pc.position = std::make_shared<glm::vec2>(pos);
		ECS::queueComponent<particleEmmiterC>(waterexplosion, pec);
		ECS::queueComponent<physicsC>(waterexplosion, pc);
	}
	void ondie_icebladeprojectile(mobDeathargs)
	{
		int waterexplosion = ECS::newEntity();
		particleEmmiterC pec;
		pec.amount = 4;
		pec.rate = 1;
		pec.tex = "particle:icepart";
		pec.lifespan = 2;
		pec.ms = 0.03;
		pec.dir = { 0, 1 };
		pec.randangle = 180;
		pec.particleLifespan = 40;
		pec.randomizelifespan = 10;
		pec.smallerAsDie = true;
		pec.randrotation = 40;
		pec.weight = 0;
		pec.randomizems = 0.2;
		pec.stoponcollision = false;
		pec.size = { .65,.65 };
		pec.randomizescale = 0.1;
		physicsC pc;
		glm::vec2 pos = *phys->position;
		pc.position = std::make_shared<glm::vec2>(pos);
		ECS::queueComponent<particleEmmiterC>(waterexplosion, pec);
		ECS::queueComponent<physicsC>(waterexplosion, pc);
	}
	void ondie_impprojectile(mobDeathargs)
	{
		int waterexplosion = ECS::newEntity();
		particleEmmiterC pec;
		pec.amount = 4;
		pec.rate = 1;
		pec.tex = "particle:torchpart";
		pec.lifespan = 2;
		pec.ms = 0.03;
		pec.dir = { 0, 1 };
		pec.randangle = 180;
		pec.particleLifespan = 40;
		pec.randomizelifespan = 10;
		pec.smallerAsDie = true;
		pec.randrotation = 40;
		pec.weight = 0;
		pec.randomizems = 0.2;
		pec.stoponcollision = false;
		pec.size = { .65,.65 };
		pec.randomizescale = 0.1;
		physicsC pc;
		glm::vec2 pos = *phys->position;
		pc.position = std::make_shared<glm::vec2>(pos);
		ECS::queueComponent<particleEmmiterC>(waterexplosion, pec);
		ECS::queueComponent<physicsC>(waterexplosion, pc);
	}
	void ondie_bladeofgrassprojectile(mobDeathargs)
	{
		int waterexplosion = ECS::newEntity();
		particleEmmiterC pec;
		pec.amount = 4;
		pec.rate = 1;
		pec.tex = "particle:grasspart";
		pec.lifespan = 2;
		pec.ms = 0.03;
		pec.dir = { 0, 1 };
		pec.randangle = 180;
		pec.particleLifespan = 40;
		pec.randomizelifespan = 10;
		pec.smallerAsDie = true;
		pec.randrotation = 40;
		pec.weight = 0;
		pec.randomizems = 0.2;
		pec.stoponcollision = false;
		pec.size = { .65,.65 };
		pec.randomizescale = 0.1;
		physicsC pc;
		glm::vec2 pos = *phys->position;
		pc.position = std::make_shared<glm::vec2>(pos);
		ECS::queueComponent<particleEmmiterC>(waterexplosion, pec);
		ECS::queueComponent<physicsC>(waterexplosion, pc);
	}
	void ondie_eaterofworlds(mobDeathargs)
	{
		auto aic = ECS::getComponent<aiC>(ent);
		int parent = game::aiSys->getStat(aic, "parent", { .intVal = -1 })->intVal;
		if (parent == -1) return;
		auto parentdraw = ECS::getComponent<drawC>(parent);
		if (parentdraw == nullptr) return;
		parentdraw->tex = "eaterofworldstail";
		parentdraw->size = glm::vec2(5);
	}
	void ondie_snowball(mobDeathargs)
	{
		int waterexplosion = ECS::newEntity();
		particleEmmiterC pec;
		pec.amount = 4;
		pec.rate = 1;
		pec.tex = "particle:cloudpart";
		pec.lifespan = 2;
		pec.ms = 0.02;
		pec.dir = { 0, 1 };
		pec.randangle = 180;
		pec.particleLifespan = 30;
		pec.randomizelifespan = 10;
		pec.smallerAsDie = true;
		pec.randrotation = 40;
		pec.weight = 0.2;
		pec.randomizems = 0.2;
		pec.stoponcollision = false;
		pec.size = { .65,.65 };
		pec.randomizescale = 0.1;
		physicsC pc;
		glm::vec2 pos = *phys->position;
		pc.position = std::make_shared<glm::vec2>(pos);
		ECS::queueComponent<particleEmmiterC>(waterexplosion, pec);
		ECS::queueComponent<physicsC>(waterexplosion, pc);
	}

	void ondie_bee(mobDeathargs)
	{
		int waterexplosion = ECS::newEntity();
		particleEmmiterC pec;
		pec.amount = 3;
		pec.rate = 1;
		pec.tex = "particle:bloodpart";
		pec.lifespan = 2;
		pec.ms = 0.02;
		pec.dir = { 0, 1 };
		pec.randangle = 180;
		pec.particleLifespan = 60;
		pec.randomizelifespan = 10;
		pec.smallerAsDie = true;
		pec.randrotation = 40;
		pec.weight = 0.5;
		pec.randomizems = 0.2;
		pec.stoponcollision = false;
		pec.size = { .45,.45 };
		pec.randomizescale = 0.1;
		physicsC pc;
		glm::vec2 pos = *phys->position;
		pc.position = std::make_shared<glm::vec2>(pos);
		ECS::queueComponent<particleEmmiterC>(waterexplosion, pec);
		ECS::queueComponent<physicsC>(waterexplosion, pc);
	}
};

#undef args
void collisionFs::damage(mobCargs)
{
	if (p2->iframes < 0) {
		int damage = p1->damage * (1 +( (rand() % 100) / 1000.0f)- 0.05);
		damage -= p2->defense;
		if (damage < 1) damage = 1;
		if (p2->directDamageTo != -1) {
			ECS::getComponent<mobC>(p2->directDamageTo)->hp -= damage;
		}
		else {
			p2->hp -= damage;
		}
		float dir = -1;
		if (phys1->position->x < phys2->position->x) {
			dir = 1;
		}
		phys2->vel.x += p1->knockback * (1-p2->kbResist) * dir;
		p2->iframes = globals::iframes;
		float xvel = (rand() % 100) / 100.0f - 0.5;
		float yvel = 0.28;
		xvel /= 6;
		int ent = enemies::spawnEnemy("damagetext", *phys2->position, { xvel, yvel });
		ECS::getComponent<drawC>(ent)->text = std::to_string(damage);
	}
}
void collisionFs::damageBoth1(mobCargs)
{
	damage(p1, p2, phys1, phys2, p1_i, p2_i, arr);
	p1->iframes = globals::iframes;
	p1->hp--;
}
void collisionFs::damagePlayer(mobCWPargs)
{
	int dmg = p->damage - Player::defense;
	if (dmg < 1) dmg = 1;
	if(Player::iframes < 0){
		Player::regentimer = -12 * 60;
		Player::hp -= dmg;
		Player::iframes = 40;
		Player::velocity()->x += utils::sign(Player::pos.x - phys->position->x) * p->knockback;
		if (Player::thorns > 0) {
			int damage = Player::thorns * (1 + ((rand() % 100) / 1000.0f) - 0.05);
			damage -= p->defense;
			if (damage < 1) damage = 1;
			p->hp -= damage;
			phys->vel.x = -utils::sign(Player::pos.x - phys->position->x) * 0.4 * (1-p->kbResist);
			p->iframes = globals::iframes;
			float xvel = (rand() % 100) / 100.0f - 0.5;
			float yvel = 0.28;
			xvel /= 6;
			int ent = enemies::spawnEnemy("damagetext", *phys->position, { xvel, yvel });
			ECS::getComponent<drawC>(ent)->text = std::to_string(damage);
		}
	}
}

void collisionFs::damagePlayerIce(mobCWPargs)
{
	if (Player::iframes < 0) {
		Player::addBuff("chilled", 120);
		damagePlayer(p, phys, i, arr);
	}
}

void collisionFs::damagePlayerPoison(mobCWPargs)
{
	if (Player::iframes < 0) {
		Player::addBuff("poisoned", 600);
		damagePlayer(p, phys, i, arr);
	}
}

void collisionFs::damagePlayerBurn(mobCWPargs)
{
	if (Player::iframes < 0) {
		Player::addBuff("onfire", 120);
		damagePlayer(p, phys, i, arr);
	}
}

void collisionFs::terraBladedamage(mobCargs)
{
	damage(p1, p2, phys1, phys2, p1_i, p2_i, arr);
	particles::spawnEffect("terrabladedamage", *phys2->position);
}

void collisionFs::volcanoDamage(mobCargs)
{
	damage(p1, p2, phys1, phys2, p1_i, p2_i, arr);
	if (rand() % 2 == 0) {
		p2->buffs.push_back({ "onfire", 180, globals::time });
	}
}

void collisionFs::bladeofgrassDamage(mobCargs)
{
	damage(p1, p2, phys1, phys2, p1_i, p2_i, arr);
	if (rand() % 2 == 0) {
		p2->buffs.push_back({ "poisoned", 180, globals::time });
	}
}

void collisionFs::moltenPickaxeDamage(mobCargs)
{
	damage(p1, p2, phys1, phys2, p1_i, p2_i, arr);
	if (rand() % 10 == 0) {
		p2->buffs.push_back({ "onfire", 180, globals::time });
	}
}

void collisionFs::moltenHamaxeDamage(mobCargs)
{
	damage(p1, p2, phys1, phys2, p1_i, p2_i, arr);
	if (rand() % 5 == 0) {
		p2->buffs.push_back({ "onfire", 180, globals::time });
	}
}
