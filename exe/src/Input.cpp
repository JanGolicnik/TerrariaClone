#include "input.h"

#include <ui.h>
#include <layers.h>
#include <utils.h>

namespace input {
    
    std::unordered_map<int, keyState> keys;
    std::unordered_map<int, keyState> mousebuttons;
    int scroll;

    bool shift;
    bool ctrl;
    bool alt;

    void clear()
    {
        for (auto key = std::begin(keys); key != std::end(keys);) {
            if (key->second == keyState::UP) {
                key = keys.erase(key);
                continue;
            }
            else if (key->second == keyState::DOWN) {
                keys[key->first] = keyState::HELD;
            }
            ++key;
        }

        for (auto key = std::begin(mousebuttons); key != std::end(mousebuttons);) {
            if (key->second == keyState::UP) {
                key = mousebuttons.erase(key);
                continue;
            }
            else if (key->second == keyState::DOWN) {
                mousebuttons[key->first] = keyState::HELD;
            }
            ++key;
        } 
        scroll = 0;
    }

    bool pressed(int key)
    {
        if (keys.count(key) <= 0) return false;
        return keys[key] == keyState::DOWN;
    }

    bool held(int key)
    {
        if (keys.count(key) <= 0) return false;
        return keys[key] == keyState::HELD;
    }

    bool released(int key)
    {
        if (keys.count(key) <= 0) return false;
        return keys[key] == keyState::UP;
    }

    bool mousePressed(int button)
    {
        if (mousebuttons.count(button) <= 0) return false;
        return mousebuttons[button] == DOWN;
    }

    bool mouseHeld(int button)
    {
        if (mousebuttons.count(button) <= 0) return false;
        return mousebuttons[button] == HELD;
    }

    bool mouseReleased(int button)
    {
        if (mousebuttons.count(button) <= 0) return false;
        return mousebuttons[button] == UP;
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if ((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) && action == GLFW_PRESS) {
            shift = true;
        }
        if ((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) && action == GLFW_RELEASE) {
            shift = false;
        }
        
        if (action == GLFW_PRESS) {
            if (UI::addLetter(key)) {
                return;
            }
        }

        switch (action) {
        case GLFW_RELEASE:
            keys[key] = UP;
            break;
        case GLFW_PRESS:
            keys[key] = DOWN;
            break;
        case GLFW_REPEAT:
            keys[key] = HELD;
            break;
        }
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
        
        scroll += yoffset;
    }

    void mousebutton_callback(GLFWwindow* window, int button, int action, int mods) {

        switch (action) {
        case GLFW_RELEASE:
            mousebuttons[button] = UP;
            break;
        case GLFW_PRESS:
            mousebuttons[button] = DOWN;
            break;
        case GLFW_REPEAT:
            mousebuttons[button] = HELD;
        }
    }

    void windowsize_callback(GLFWwindow* window, int width, int height) {
        glfwSetWindowSize(globals::window, width, height);
        glViewport(0, 0, width, height);
        glDeleteFramebuffers(1, &globals::mainFB);
        glDeleteFramebuffers(1, &globals::tmpFB);
        glDeleteTextures(1, &globals::mainFBT);
        glDeleteTextures(1, &globals::tmpFBT);
        utils::createFrameBuffer(&globals::mainFB, &globals::mainFBT, { width, height });
        utils::createFrameBuffer(&globals::tmpFB, &globals::tmpFBT, { width, height });
        globals::resX = width;
        globals::resY = height;
        globals::fullScale = glm::vec2(globals::blocksizepx / (float)globals::resX, globals::blocksizepx / (float)globals::resY);
        Layers::reschanged();
    }
}