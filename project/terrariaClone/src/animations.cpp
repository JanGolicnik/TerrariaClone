#include "animations.h"

namespace AnimManager {
	std::unordered_map<const char*, anim> animations;
	std::unordered_map<int, runninganim> runningAnims;
	int frame;
	void createAnim(const char* animation, std::vector<keyframe> frames, int endframe, bool loop, int loopframe)
	{
		anim a;
		a.endframe = endframe;
		a.frames = frames;
		a.loop = loop;
		a.loopframe = loopframe;
		animations.insert(std::pair < const char*, anim>(animation, a));
	}

	void watchAnim(const char* name, int* id)
	{
		static int idgen = 0;
		if (animations.count(name) >= 1) {
			if (runningAnims.count(*id) >= 1) {
				removeAnim(*id);
			}
			idgen++;
			*id = idgen;
			runninganim tmp;
			tmp.startframe = frame;
			tmp.anim = name;
			tmp.csprite = animations[name].frames[0].tex;
			runningAnims.insert(std::pair<int, runninganim>(*id, tmp));
		}
	}

	void removeAnim(int id)
	{
		runningAnims.erase(id);
	}

	void updateAnims()
	{
		for (auto& anim : runningAnims) {
			if (animations[anim.second.anim].loop) {
				if (animations[anim.second.anim].endframe <= frame - anim.second.startframe) {
					anim.second.startframe = frame - animations[anim.second.anim].frames[animations[anim.second.anim].loopframe].startframe;
				}
			}
			for (int i = 0; i < animations[anim.second.anim].frames.size(); i++) {
				if (animations[anim.second.anim].frames[i].startframe <= frame - anim.second.startframe) {
					anim.second.csprite = animations[anim.second.anim].frames[i].tex;
				}
				else {
					break;
				}
			}
		}
		frame++;
	}

	const char* getFrame(int id)
	{
		if (runningAnims.count(id) >= 1) {
			return runningAnims[id].csprite;
		}
	}
}