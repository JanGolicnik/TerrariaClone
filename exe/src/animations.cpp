#include "animations.h"

#include <globals.h>
#include <blocks.h>
#include <textures.h>

namespace animations {
	std::unordered_map<std::string, anim> animations;
	std::unordered_map<int, running_anim> runningAnims;
	int frame = 0;
	void createAnim(std::string animation, glm::vec4 base, std::vector<keyframe> frames, int endframe, bool loop, int loopframe)
	{
		anim a;
		a.base = base;
		a.endframe = endframe;
		a.frames = frames;
		a.loop = loop;
		a.loopframe = loopframe;
		animations.insert(std::make_pair(animation, a));
	}

	void watchAnim(const std::string name, int* id, bool overwrite, bool flippedY, bool flippedX)
	{
		static int idgen = 0;
		if (animations.count(name) >= 1) {
			if (runningAnims.count(*id) >= 1) {
				if (!overwrite) {
					return;
				}
				else {
					removeAnim(id);
				}
			}
			idgen++;
			*id = idgen;
			running_anim tmp;
			tmp.startframe = frame;
			tmp.anim = name;
			tmp.ccoords = animations[name].base;
			tmp.flippedX = flippedX;
			tmp.flippedY = flippedY;
			runningAnims.insert(std::pair<int, running_anim>(*id, tmp));
		}
	}

	void removeAnim(int* id)
	{
		runningAnims.erase(*id);
		*id = -1;
	}

	void updateAnims()
	{
		for (auto& anim : runningAnims) {
			bool run = true;
			if (animations[anim.second.anim].endframe <= frame - anim.second.startframe) {
				if (animations[anim.second.anim].loop) {
					anim.second.startframe = frame - animations[anim.second.anim].frames[animations[anim.second.anim].loopframe].startframe;
				}
				else {
					anim.second.ccoords = animations[anim.second.anim].base;
					run = false;
				}
			}
			if (run) {
				for (int i = 0; i < animations[anim.second.anim].frames.size(); i++) {
					if (animations[anim.second.anim].frames[i].startframe <= frame - anim.second.startframe) {
						switch (animations[anim.second.anim].frames[i].type) {
						case ft_ABSOLUTE:
							anim.second.ccoords = animations[anim.second.anim].frames[i].off / glm::vec4(globals::spritesheetWH.x, globals::spritesheetWH.y, globals::spritesheetWH.x, globals::spritesheetWH.y);
							break;
						case ft_OFFSET1:
							anim.second.ccoords = animations[anim.second.anim].base + animations[anim.second.anim].frames[i].off / glm::vec4(globals::spritesheetWH.x, globals::spritesheetWH.y, globals::spritesheetWH.x, globals::spritesheetWH.y);
							break;
						case ft_OFFSET8:
							anim.second.ccoords = animations[anim.second.anim].base + (animations[anim.second.anim].frames[i].off * glm::vec4(8)) / glm::vec4(globals::spritesheetWH.x, globals::spritesheetWH.y, globals::spritesheetWH.x, globals::spritesheetWH.y);
							break;
						case ft_OFFSET16:
							anim.second.ccoords = animations[anim.second.anim].base + (animations[anim.second.anim].frames[i].off * glm::vec4(16)) / glm::vec4(globals::spritesheetWH.x, globals::spritesheetWH.y, globals::spritesheetWH.x, globals::spritesheetWH.y);
							break;
						}
					}
					else {
						break;
					}
				}
			}
		}
		frame++;
	}

	glm::vec4 getFrame(int id)
	{
		if (runningAnims.count(id) >= 1) {
			glm::vec4 tmp = runningAnims[id].ccoords;
			if (runningAnims[id].flippedX) {
				float tmp2 = tmp.r;
				tmp.r = tmp.b;
				tmp.b = tmp2;
			}
			if (runningAnims[id].flippedY) {
				float tmp2 = tmp.g;
				tmp.g = tmp.a;
				tmp.a = tmp2;
			}
			return tmp;
		}
		else
			return textures::nametocoords["err"]->coords;
	}
	void clear()
	{
		animations.clear();
		runningAnims.clear();
	}
	void clearRunning()
	{
		runningAnims.clear();
	}
}