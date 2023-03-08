#include "textures.h"

#include <globals.h>

namespace textures {
	std::unordered_map<std::string, std::shared_ptr<Sprite>> nametocoords;
	GLuint spriteSheet;
	std::unordered_map<std::string, int> nameToID;
	std::unordered_map<int, std::string> idtoname;
	std::unordered_map<int, std::shared_ptr<Sprite>> idtocoords;
	extern int texidcounter = 0;

	void clean()
	{
		nametocoords.clear();
		idtoname.clear();
		nameToID.clear();
		idtocoords.clear();
		glDeleteTextures(1, &spriteSheet);
	}
}