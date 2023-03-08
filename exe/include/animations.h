#pragma once

#include <core.h>

namespace animations
{
    struct keyframe {
        int startframe;
        glm::vec4 off;
        frametype type;
    };

    struct anim {
        glm::vec4 base;
        int endframe;
        std::vector<keyframe> frames;
        int loopframe;
        bool loop;
    };

    struct running_anim {
        std::string anim;
        glm::vec4 ccoords;
        int startframe;
        bool flippedY;
        bool flippedX;
    };

     void createAnim(std::string animation, glm::vec4 base, std::vector<keyframe> frames, int endframe, bool loop = true, int loopframe = 0);
     void watchAnim(std::string anim, int* id, bool overwrite = true, bool flippedY = false, bool flippedX = false);
     void removeAnim(int* id);
     void updateAnims();
     glm::vec4 getFrame(int id);

     void clear();
     void clearRunning();
};

