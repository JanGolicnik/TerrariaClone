#pragma once

#include <irrKlang/irrKlang.h>

namespace sounds {
	extern irrklang::ISoundEngine* music;
	extern irrklang::ISoundEngine* sounds;

	extern float mastervolume;
	extern float musicvolume;
	extern float soundsvolume;

	void init();

	void digSound();
	void openDoor();
	void closeDoor();
	void doubleJump();
	void drown();
	void aquastaff();
	void bombExplosion();
	void gun();
	void eat();
	void drink();
	void bow();
	void magicmirror();
	void spacegun();
	void star();
	void swing();
	void lavabg();
	void lavapluswater();
	void water();
	void manafull();
	void menuclose();
	void menuopen();
	void menuclick();
	void normalhit();
	void skeletonhit();
	void slimehit();
	void normaldie();
	void zombiedie();
	void batdie();
	void pickup();
	void playerhit();
	void playerdie();
	void bossroar();
	void watersplash();
	void lavasplash();
	void hardDigSound();
	void grass();
	void zombie();
	void frog();
	void bird();
	void mouse();
	void coin();
}