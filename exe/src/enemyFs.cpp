#include "enemyFs.h"
#include <Layers.h>
#include <utils.h>
#include <Player.h>
#include <animations.h>
#include <globals.h>
#include <Terraria.h>
#include <componentsystems.h>
#include <particles.h>
#include <Input.h>
#include <Inventory.h>
#include <MainLoop.h>

#define args aiC* p, int entity, physicsC* phys, ComponentArray<aiC>* arr, aiAction* action, int ctime

namespace mobFuncs {
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
		if(phys->collidebot){
			p->actions.push_back({16, slimeJump, 1, ctime });
			auto draw = ECS::getComponent<drawC>(entity);
			AnimManager::watchAnim("slimejump", &draw->anim);
		}
	}
	void bombExplode(args)
	{
		particleEmmiterC pec;
		pec.amount = 15;
		pec.dir = glm::vec2(-1, 0);
		pec.disappearAsDie = true;
		pec.killentity = true;
		pec.lifespan = 2;
		pec.ms = 0.02;
		pec.randomizems = 0.18;
		pec.size = glm::vec2(1.5);
		pec.randomizescale = 0.5;
		pec.particleLifespan = 40;
		pec.randangle = 360;
		pec.randomizeamount = 5;
		pec.randomizelifespan = 20;
		pec.randrotation = 5;
		pec.rotation = 10;
		pec.rate = 1;
		pec.stoponcollision = false;
		pec.tex = "particle:smokepart";
		pec.weight = 0;
		ECS::addComponent<particleEmmiterC>(entity, pec);
		glm::vec2 pos = *phys->position;
		pos = round(pos);
		Layers::damageBlock(Layers::getLayer("blocks"), pos, 100, 10, true);
		Layers::damageBlock(Layers::getLayer("bg"), pos, 100, 7, true);
		auto mC = ECS::getComponent<mobC>(entity);
		ECS::getComponent<mobC>(mC->children[0])->killin = 2;
	}
	void guideOnUpdate(args)
	{
		if (glm::distance(*phys->position, Player::pos) > 120) return;
		auto bs = Layers::getLayer("blocks");
		auto draw = ECS::getComponent<drawC>(entity);
		phys->stoponcollision = true;
		phys->weight = 1;
		bool goUp = true;
		if (p->state == 0) {
			AnimManager::removeAnim(&draw->anim);
			if (rand() % 100 == 0) {
				AnimManager::watchAnim("playerwalkleft", &draw->anim, true);
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
			if (rand() % 90 == 0) {
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
	
		if (Input::mousePressed(GLFW_MOUSE_BUTTON_RIGHT)) {
			glm::vec2 mc = globals::mouseBlockCoords(false);
			if (phys->position->x - phys->size.x / 2.0f < mc.x &&
				phys->position->x + phys->size.x / 2.0f > mc.x &&
				phys->position->y + phys->size.y / 2.0f > mc.y &&
				phys->position->y - phys->size.y / 2.0f < mc.y) {
				MainLoop::openDialogue("guide");
			}
		}

		
	}
	void slimeJump(args) {
		auto draw = ECS::getComponent<drawC>(entity);
		phys->vel.y = 0.4 + (rand() % 2)/10.0f;
		phys->vel.x = 0.3 + (rand() % 2) / 10.0f;
		if (rand() % 2 == 0) {
			phys->vel.x *= -1;
		}
		AnimManager::removeAnim(&draw->anim);
	}

	void zombieOnUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		AnimManager::watchAnim("zombiewalk", &(draw->anim), false);
		float dir = Player::pos.x - phys->position->x;
		if (dir > 0) {
			draw->flipX = true;
			dir = 1;
		}
		else if (dir < 0) {
			draw->flipX = false;
			dir = -1;
		}

		phys->vel.x += utils::approach(phys->vel.x, dir * 0.1, 40);
		if (phys->collidebot) {
			if (phys->vel.x < 0) {
				if (Layers::queryBlockInfo(Layers::getLayer("blocks"), *phys->position + glm::vec2(-1 * (phys->size.x / 2 + 0.6), -phys->size.y / 2))->collidableRight)
				{
					phys->vel.y = Player::jumpms;
				}
			}
			else {
				if (Layers::queryBlockInfo(Layers::getLayer("blocks"), *phys->position + glm::vec2((phys->size.x / 2 + 0.6), -phys->size.y / 2))->collidableLeft)
				{
					phys->vel.y = Player::jumpms;
				}
			}
		}
	}
	void starOnUpdate(args)
	{
		auto draw = ECS::getComponent<drawC>(entity);
		AnimManager::watchAnim("fallenstar", &(draw->anim), false);
		if (phys->collidebot && draw->hasmat) {
			draw->hasmat = false;
			phys->vel = glm::vec2(0, 0.3 + (rand() % 30) / 100.0f);
			ECS::addComponent<droppedItemC>(entity, { "fallenstar", 1, 0 });
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
			AnimManager::removeAnim(&draw->anim);
			if (rand() % 100 == 0) {
				AnimManager::watchAnim("bunnyhop", &draw->anim, true);
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
					ECS::addComponent<droppedItemC>(entity, dc);
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

	void waterboltUpdate(args)
	{
		ECS::getComponent<particleEmmiterC>(entity)->dir = -glm::normalize(phys->vel);
	}

	void particleOnUpdate(args)
	{
		auto dc = ECS::getComponent<drawC>(entity);
		float rotation = Terraria::aiSys->getStat(p, "rotation")->floatVal;
		dc->hasmat = false;
		if (rotation != 0) {
			dc->hasmat = true;
			dc->mat = glm::rotate(dc->mat, (float)(rotation * PI / 180), glm::vec3(0, 0, 1));
		}
		ECS::getComponent<drawC>(entity)->opacity -= Terraria::aiSys->getStat(p, "opacity")->floatVal;
		float size = Terraria::aiSys->getStat(p, "size")->floatVal;
		dc->size -= size;
		phys->size = dc->size;
	}

	void ondie_nothing(mobDeathargs) {

	}

	void ondie_waterbolt(mobDeathargs) {
		int waterexplosion = ECS::newEntity();
		particleEmmiterC pec;
		pec.amount = 40;
		pec.tex = "waterbolt";
		pec.lifespan = 1;
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
		ECS::addComponent<mobC>(waterexplosion, mc);
		ECS::addComponent<particleEmmiterC>(waterexplosion, pec);
		ECS::addComponent<physicsC>(waterexplosion, pc);
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
		ECS::addComponent<mobC>(dirt, mc);
		ECS::addComponent<particleEmmiterC>(dirt, pec);
		ECS::addComponent<physicsC>(dirt, pc);
	}
};

#undef args
void collisionFs::damage(mobCargs)
{
	if (p2->iframes < 0) {
		p2->hp -= p1->damage;
		float dir = -1;
		if (phys1->position->x < phys2->position->x) {
			dir = 1;
		}
		phys2->vel.x += p1->knockback * p2->kbWeakness * dir;
		p2->iframes = globals::iframes;
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
		Player::hp -= dmg;
		Player::iframes = 40;
		Player::velocity()->x += utils::sign(Player::pos.x - phys->position->x) * p->knockback;
	}
}
