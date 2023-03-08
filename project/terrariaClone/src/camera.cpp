#include "camera.h"

#include <Player.h>
#include <globals.h>
#include <utils.h>
#include <world.h>

namespace camera {

    glm::mat4 base = glm::mat4(1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    glm::vec2 pos = glm::vec2(0.0f);
    glm::vec2 vel = glm::vec2(0.0f);
    glm::vec2 tvel = glm::vec2(0.0f);
    float accel = 0;
    glm::vec2 limits;
    float speed = 0.07f;
    std::vector<cameraShake> shakes;

	void updateTowardsPlayer()
	{
        for (int i = shakes.size() - 1; i >= 0; i--) {
           shakes[i].duration--;
            if (shakes[i].duration <= 0) {
                shakes.erase(shakes.begin() + i);
            }
            vel += glm::normalize(glm::vec2(rand() % 100 - 50, rand() % 100 - 50)) * shakes[i].strength;
        }

        pos = -Player::pos;

        //limitamo v world size
        glm::vec2 c = -glm::vec2(pos.x, pos.y);
        if (c.x < Layers::blocksOnScreen.x / 2 + 2) {
            pos.x = -(Layers::blocksOnScreen.x / 2) + 2;
        }
        if (c.x > map::mapX- Layers::blocksOnScreen.x / 2 - 2) {
            pos.x = -map::mapX + Layers::blocksOnScreen.x / 2 - 2;
        }
        if (c.y < Layers::blocksOnScreen.y / 2 + 2) {
            pos.y = -Layers::blocksOnScreen.y / 2 + 2;
        }
        if (c.y > map::mapY - Layers::blocksOnScreen.y / 2 - 2) {
            pos.y = -map::mapY + Layers::blocksOnScreen.y / 2 - 2;
        }

        updateNormal();
	}
    void updateNormal()
    {
        trans = glm::scale(glm::mat4(1.0f), glm::vec3(globals::fullScale.x, globals::fullScale.y, 1.0f));
        trans = glm::translate(trans, glm::vec3(pos, 0.0f));
    }
	void shake(float strength, int duration)
	{
        shakes.push_back({ strength, duration });
	}
}