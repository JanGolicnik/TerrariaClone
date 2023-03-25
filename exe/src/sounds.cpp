#include <sounds.h>
#include <globals.h>

namespace sounds {

	irrklang::ISoundEngine* music;
	irrklang::ISoundEngine* sounds;

	float mastervolume = 1;
	float musicvolume = 1;
	float soundsvolume = 1;

	void init()
	{
		music = irrklang::createIrrKlangDevice();
		if (!music) { std::cout << "\n IRRKLANG ERROR MUSIC DOESNT WORK\n"; return; }
			music->play2D("assets\\sounds\\tehma123.mp3", true);
		sounds = irrklang::createIrrKlangDevice();
		if (!sounds) { std::cout << "\n IRRKLANG ERROR SOUNDS DONT WORK\n"; return; }
	}
	void digSound()
	{
		switch (rand() % 3) {
		case 0:
			sounds->play2D("assets\\sounds\\Dig_1.wav");
			break;
		case 1:
			sounds->play2D("assets\\sounds\\Dig_2.wav");
			break;
		case 2:
			sounds->play2D("assets\\sounds\\Dig_0.wav");
			break;
		}
	}

	void openDoor()
	{
		sounds->play2D("assets\\sounds\\Door_Opened.wav");
	}

	void closeDoor()
	{
		sounds->play2D("assets\\sounds\\Door_Closed.wav");
	}

	void doubleJump()
	{
		sounds->play2D("assets\\sounds\\Double_Jump.wav");
	}

	void drown()
	{
		sounds->play2D("assets\\sounds\\Drown.wav");
	}

	void aquastaff()
	{
		sounds->play2D("assets\\sounds\\item_aquastaff.wav");
	}

	void bombExplosion()
	{
		sounds->play2D("assets\\sounds\\item_bomb.wav");
	}

	void gun()
	{
		sounds->play2D("assets\\sounds\\item_gun.wav");
	}

	void eat()
	{
		sounds->play2D("assets\\sounds\\item_jabuk.wav");
	}

	void drink()
	{
		sounds->play2D("assets\\sounds\\item_pot.wav");
	}

	void bow()
	{
		sounds->play2D("assets\\sounds\\item_lok.wav");
	}

	void magicmirror()
	{
		sounds->play2D("assets\\sounds\\item_magicmirror.wav");
	}

	void spacegun()
	{
		sounds->play2D("assets\\sounds\\item_spacegun.wav");
	}

	void star()
	{
		sounds->play2D("assets\\sounds\\item_star.wav");
	}

	void swing()
	{
		sounds->play2D("assets\\sounds\\item_swing.wav");
	}

	void lavabg()
	{
		sounds->play2D("assets\\sounds\\lava.wav");
	}

	void lavapluswater()
	{
		sounds->play2D("assets\\sounds\\lavapluswater.wav");
	}

	void water()
	{
		sounds->play2D("assets\\sounds\\water.wav");
	}

	void manafull()
	{
		sounds->play2D("assets\\sounds\\MaxMana.wav");
	}

	void menuclose()
	{
		sounds->play2D("assets\\sounds\\Menu_Close.wav");
	}

	void menuopen()
	{
		sounds->play2D("assets\\sounds\\Menu_Open.wav");
	}

	void menuclick()
	{
		sounds->play2D("assets\\sounds\\Menu_Tick.wav");
	}

	void normalhit()
	{
		sounds->play2D("assets\\sounds\\NPC_Hit_1.wav");
	}

	void skeletonhit()
	{
		sounds->play2D("assets\\sounds\\NPC_Hit_2.wav");
	}

	void slimehit()
	{
		sounds->play2D("assets\\sounds\\NPC_Hit_8.wav");
	}

	void normaldie()
	{
		sounds->play2D("assets\\sounds\\NPC_Killed_1.wav");
	}

	void zombiedie()
	{
		sounds->play2D("assets\\sounds\\NPC_Killed_2.wav");
	}

	void batdie()
	{
		sounds->play2D("assets\\sounds\\NPC_Killed_4.wav");
	}

	void pickup()
	{
		sounds->play2D("assets\\sounds\\pickup.wav");
	}

	void playerhit()
	{
		switch (rand() % 3) {
		case 0:
			sounds->play2D("assets\\sounds\\Player_Hit_0.wav");
			break;
		case 1 :
			sounds->play2D("assets\\sounds\\Player_Hit_1.wav");
			break;
		case 2:
			sounds->play2D("assets\\sounds\\Player_Hit_2.wav");
			break;
		}
	}

	void playerdie()
	{
		sounds->play2D("assets\\sounds\\Player_Killed.wav");
	}

	void bossroar()
	{
		sounds->play2D("assets\\sounds\\roar.wav");
	}

	void watersplash()
	{
		sounds->play2D("assets\\sounds\\Splash_0.wav");
	}

	void lavasplash()
	{
		sounds->play2D("assets\\sounds\\Splash_1.wav");
	}

	void hardDigSound()
	{
		switch (rand() % 3) {
		case 0:
			sounds->play2D("assets\\sounds\\Tink_0.wav");
			break;
		case 1:
			sounds->play2D("assets\\sounds\\Tink_1.wav");
			break;
		case 2:
			sounds->play2D("assets\\sounds\\Tink_2.wav");
			break;
		}
	}

	void grass()
	{
		sounds->play2D("assets\\sounds\\grass.wav");
	}

	void zombie()
	{
		switch (rand() % 4){
			sounds->play2D("assets\\sounds\\Zombie_0.wav");
			break;
		case 1:
			sounds->play2D("assets\\sounds\\Zombie_1.wav");
			break;
		case 2:
			sounds->play2D("assets\\sounds\\Zombie_2.wav");
			break;
		case 3:
			sounds->play2D("assets\\sounds\\Zombie_3.wav");
			break;
		}
	}

	void frog()
	{
		sounds->play2D("assets\\sounds\\Zombie_13.wav");
	}

	void bird()
	{
		sounds->play2D("assets\\sounds\\Zombie_14.wav");
	}

	void mouse()
	{
		sounds->play2D("assets\\sounds\\Zombie_15.wav");
	}

	void coin()
	{
		switch (rand() % 4) {
			sounds->play2D("assets\\sounds\\Coins_0.wav");
			break;
		case 1:
			sounds->play2D("assets\\sounds\\Coins_1.wav");
			break;
		case 2:
			sounds->play2D("assets\\sounds\\Coins_2.wav");
			break;
		case 3:
			sounds->play2D("assets\\sounds\\Coins_3.wav");
			break;
		}
	}
	
}