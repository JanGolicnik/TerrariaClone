#pragma once
#include <core.h>

namespace AnimManager
{
    struct keyframe {
        int startframe;
        const char* tex;
    };

    struct anim {
        int endframe;
        std::vector<keyframe> frames;
        int loopframe;
        bool loop;
    };

    struct runninganim {
        const char* anim;
        const char* csprite;
        int startframe;
    };

    extern std::unordered_map<const char*, anim> animations;
    extern std::unordered_map<int, runninganim> runningAnims;
    extern int frame;

     void createAnim(const char* animation, std::vector<keyframe> frames, int endframe, bool loop = true, int loopframe = 0);
     void watchAnim(const char* anim, int* id);
     void removeAnim(int id);
     void updateAnims();
     const char* getFrame(int id);
};

