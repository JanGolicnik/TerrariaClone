#include <sounds.h>
#include <globals.h>

namespace sounds {
	void digSound()
	{
		switch (rand() % 3) {
		case 0:
			globals::engine->play2D("Dig_1.wav");
			break;
		case 1:
			globals::engine->play2D("Dig_2.wav");
			break;
		case 2:
			globals::engine->play2D("Dig_0.wav");
			break;
		}
	}
}