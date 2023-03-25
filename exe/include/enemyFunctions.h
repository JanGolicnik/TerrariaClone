#pragma once

#include <core.h>
#include <ECS/ECS.h>
#include <componentsystems.h>

#define args  aiC* p, int entity, physicsC* phys, ComponentArray<aiC>*, aiAction* action, int ctime
namespace mobFunctions
{
	void nothing(args);
	void slimeOnUpdate(args);
	void slimeWindup(args);
	void slimeJump(args);
	void zombieOnUpdate(args);
	void testOnUpdate(args);
	void demoneyeOnUpdate(args);
	void sandOnUpdate(args);
	void bunnyUpdate(args);
	void arrowUpdate(args);
	void bulletUpdate(args);
	void waterboltUpdate(args);
	void aquaboltUpdate(args);
	void magicprojectileUpdate(args);
	void particleOnUpdate(args);
	void starOnUpdate(args);
	void bombOnUpdate(args);
	void bombExplode(args);
	void guideOnUpdate(args);
	void starfurystarUpdate(args);
	void boomerangOnUpdate(args);
	void boomerangChangeDir(args);
	void lightdiscOnUpdate(args);
	void icebladeprojectileOnUpdate(args);
	void terrabladeswingOnUpdate(args);
	void terrabladeprojectileOnUpdate(args);
	void enchantedswordUpdate(args);
	void vilethornUpdate(args);
	void beeUpdate(args);
	void twinssummon1OnUpdate(args);
	void twinssummon2OnUpdate(args);
	void impOnUpdate(args);
	void damageTextOnUpdate(args);
	void guideOnClick(args);
	void slimeFireProjectile(args);
	void eaterOfSoulsOnUpdate(args);
	void batOnUpdate(args);
	void demonShoot(args);
	void hornetShoot(args);
	void demonscytheOnUpdate(args);
	void vultureOnUpdate(args);
	void casterTeleport(args);
	void casterShoot(args);
	void wormHeadOnUpdate(args);
	void wormCreateBody(args);
	void wormBodyOnUpdate(args);
	void eyeofcthuluOnUpdate(args);
	void eaterofworldsbodyOnUpdate(args);
	void eaterofworldsHeadOnUpdate(args);
	void cloudOnUpdate(args);
	void bladeOfGrassOnUpdate(args);
	void fireflyOnUpdate(args);
	void tombstoneOnUpdate(args);
	void goreOnUpdate(args);

	void ondie_nothing(mobDeathargs);
	void ondie_waterbolt(mobDeathargs);
	void ondie_magicprojectile(mobDeathargs);
	void ondie_aquabolt(mobDeathargs);
	void ondie_bullet(mobDeathargs);
	void ondie_starfurystar(mobDeathargs);
	void ondie_icebladeprojectile(mobDeathargs);
	void ondie_snowball(mobDeathargs);
	void ondie_bee(mobDeathargs);
	void ondie_impprojectile(mobDeathargs);
	void ondie_bladeofgrassprojectile(mobDeathargs);
	void ondie_eaterofworlds(mobDeathargs);
};
#undef args

namespace collisionFs {
	void damage(mobCargs);
	void damageBoth1(mobCargs);
	void damagePlayer(mobCWPargs);
	void damagePlayerIce(mobCWPargs);
	void damagePlayerPoison(mobCWPargs);
	void damagePlayerBurn(mobCWPargs);
	void terraBladedamage(mobCargs);
	void volcanoDamage(mobCargs);
	void bladeofgrassDamage(mobCargs);
	void moltenPickaxeDamage(mobCargs);
	void moltenHamaxeDamage(mobCargs);
}
