#include "input.h"

#include <ui.h>
#include <layers.h>
#include <utils.h>

namespace input {
    
    std::unordered_map<int, std::string> keyNames = {
    {GLFW_KEY_SPACE, "space"},
    {GLFW_KEY_APOSTROPHE, "'"},
    {GLFW_KEY_COMMA, ","},
    {GLFW_KEY_MINUS, "-"},
    {GLFW_KEY_PERIOD, "."},
    {GLFW_KEY_SLASH, "/"},
    {GLFW_KEY_0, "0"},
    {GLFW_KEY_1, "1"},
    {GLFW_KEY_2, "2"},
    {GLFW_KEY_3, "3"},
    {GLFW_KEY_4, "4"},
    {GLFW_KEY_5, "5"},
    {GLFW_KEY_6, "6"},
    {GLFW_KEY_7, "7"},
    {GLFW_KEY_8, "8"},
    {GLFW_KEY_9, "9"},
    {GLFW_KEY_SEMICOLON, ";"},
    {GLFW_KEY_EQUAL, "="},
    {GLFW_KEY_A, "A"},
    {GLFW_KEY_B, "B"},
    {GLFW_KEY_C, "C"},
    {GLFW_KEY_D, "D"},
    {GLFW_KEY_E, "E"},
    {GLFW_KEY_F, "F"},
    {GLFW_KEY_G, "G"},
    {GLFW_KEY_H, "H"},
    {GLFW_KEY_I, "I"},
    {GLFW_KEY_J, "J"},
    {GLFW_KEY_K, "K"},
    {GLFW_KEY_L, "L"},
    {GLFW_KEY_M, "M"},
    {GLFW_KEY_N, "N"},
    {GLFW_KEY_O, "O"},
    {GLFW_KEY_P, "P"},
    {GLFW_KEY_Q, "Q"},
    {GLFW_KEY_R, "R"},
    {GLFW_KEY_S, "S"},
    {GLFW_KEY_T, "T"},
    {GLFW_KEY_U, "U"},
    {GLFW_KEY_V, "V"},
    {GLFW_KEY_W, "W"},
    {GLFW_KEY_X, "X"},
    {GLFW_KEY_Y, "Y"},
    {GLFW_KEY_Z, "Z"},
    {GLFW_KEY_LEFT_BRACKET, "("},
    {GLFW_KEY_BACKSLASH, "\\"},
    {GLFW_KEY_RIGHT_BRACKET, ")"},
    {GLFW_KEY_ESCAPE, "escape"},
    {GLFW_KEY_ENTER, "enter"},
    {GLFW_KEY_TAB, "tab"},
    {GLFW_KEY_BACKSPACE, "backspace"},
    {GLFW_KEY_INSERT, "insert"},
    {GLFW_KEY_DELETE, "del"},
    {GLFW_KEY_RIGHT, "rightArrow"},
    {GLFW_KEY_LEFT, "leftArrow"},
    {GLFW_KEY_DOWN, "downArrow"},
    {GLFW_KEY_UP, "upArrow"},
    {GLFW_KEY_PAGE_UP, "pageUp"},
    {GLFW_KEY_PAGE_DOWN, "pageDown"},
    {GLFW_KEY_HOME, "home"},
    {GLFW_KEY_END, "end"},
    {GLFW_KEY_CAPS_LOCK, "capsLock"},
    {GLFW_KEY_SCROLL_LOCK, "scrollLock"},
    {GLFW_KEY_NUM_LOCK, "numLock"},
    {GLFW_KEY_PRINT_SCREEN, "printScreen"},
    {GLFW_KEY_PAUSE, "pause"},
    {GLFW_KEY_F1, "f1"},
    {GLFW_KEY_F2, "f2"},
    {GLFW_KEY_F3, "f3"},
    {GLFW_KEY_F4, "f4"},
    {GLFW_KEY_F5, "f5"},
    {GLFW_KEY_F6, "f6"},
    {GLFW_KEY_F7, "f7"},
    {GLFW_KEY_F8, "f8"},
    {GLFW_KEY_F9, "f9"},
    {GLFW_KEY_F10, "f10"},
    {GLFW_KEY_F11, "f11"},
    {GLFW_KEY_F12, "f12"},
    {GLFW_KEY_F13, "f13"},
    {GLFW_KEY_F14, "f14"},
    {GLFW_KEY_F15, "f15"},
    {GLFW_KEY_F16, "f16"},
    {GLFW_KEY_F17, "f17"},
    {GLFW_KEY_F18, "f18"},
    {GLFW_KEY_F19, "f19"},
    {GLFW_KEY_F20, "f20"},
    {GLFW_KEY_F21, "f21"},
    {GLFW_KEY_F22, "f22"},
    {GLFW_KEY_F23, "f23"},
    {GLFW_KEY_F24, "f24"},
    {GLFW_KEY_F25, "f25"},
    {GLFW_MOUSE_BUTTON_LEFT, "mouseLeft"},
    {GLFW_MOUSE_BUTTON_RIGHT, "mouseRight"},
    {GLFW_MOUSE_BUTTON_MIDDLE, "mouseMiddle" },
    {GLFW_MOUSE_BUTTON_1, "mouseBtn1"},
    {GLFW_MOUSE_BUTTON_2, "mouseBtn2"},
    {GLFW_MOUSE_BUTTON_3, "mouseBtn3"},
    {GLFW_MOUSE_BUTTON_4, "mouseBtn4"},
    {GLFW_MOUSE_BUTTON_5, "mouseBtn5" },
    {GLFW_MOUSE_BUTTON_6, "mouseBtn6" },
    {GLFW_MOUSE_BUTTON_7, "mouseBtn7" },
    {GLFW_MOUSE_BUTTON_8, "mouseBtn8" }
    };

    std::unordered_map<int, keyState> keys;
    std::unordered_map<interactionKey, int> map;

    int scroll;

    bool shift;
    bool ctrl;
    bool alt;

    bool capturingInput = false;
    int capturingAmount = 0;
    std::function<void(int)> inputFunction;

    void captureNextKey(std::function<void(int)>func) {
        capturingInput = true;
        inputFunction = func;
        capturingAmount = 1;
    }

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

        scroll = 0;
    }

    bool pressed(interactionKey key)
    {
        int k = map[key];
        if (keys.count(k) <= 0) return false;
        return keys[k] == keyState::DOWN;
    }

    bool held(interactionKey key)
    {
        int k = map[key];
        if (keys.count(k) <= 0) return false;
        return keys[k] == keyState::HELD;
    }

    bool released(interactionKey key)
    {
        int k = map[key];
        if (keys.count(k) <= 0) return false;
        return keys[k] == keyState::UP;
    }

    bool rpressed(int key)
    {
        if (keys.count(key) <= 0) return false;
        return keys[key] == keyState::DOWN;
    }

    bool rheld(int key)
    {
        if (keys.count(key) <= 0) return false;
        return keys[key] == keyState::HELD;
    }

    bool rreleased(int key)
    {
        if (keys.count(key) <= 0) return false;
        return keys[key] == keyState::UP;
    }

    void resetMap()
    {
        map[k_UP] = GLFW_KEY_W;
        map[k_LEFT] = GLFW_KEY_A;
        map[k_RIGHT] = GLFW_KEY_D;
        map[k_DOWN] = GLFW_KEY_S;
        map[k_JUMP] = GLFW_KEY_SPACE;
        map[k_PRIMARY] = GLFW_MOUSE_BUTTON_LEFT;
        map[k_SECONDARY] = GLFW_MOUSE_BUTTON_RIGHT;
        map[k_MIDDLE] = GLFW_MOUSE_BUTTON_MIDDLE;
        map[k_MOUSE4] = GLFW_MOUSE_BUTTON_4;
        map[k_MOUSE5] = GLFW_MOUSE_BUTTON_5;
        map[k_HEAL] = GLFW_KEY_H;
        map[k_MANA] = GLFW_KEY_J;
        map[k_INVENTORY] = GLFW_KEY_ESCAPE;
        map[k_1] = GLFW_KEY_1;
        map[k_2] = GLFW_KEY_2;
        map[k_3] = GLFW_KEY_3;
        map[k_4] = GLFW_KEY_4;
        map[k_5] = GLFW_KEY_5;
        map[k_6] = GLFW_KEY_6;
        map[k_7] = GLFW_KEY_7;
        map[k_8] = GLFW_KEY_8;
        map[k_9] = GLFW_KEY_9;
        map[k_0] = GLFW_KEY_0;
        map[k_THROW] = GLFW_KEY_T;
        map[k_ZOOMIN] = GLFW_KEY_UP;
        map[k_ZOOMOUT] = GLFW_KEY_DOWN;
    }

    void saveMap(std::ofstream* file) {
        int size = map.size();
        file->write((char*)&size, sizeof(size));
        for (auto& key : map) {
            file->write((char*)&key.first, sizeof(key.first));
            file->write((char*)&key.second, sizeof(key.second));
        }
    }

    void loadMap(std::ifstream *file) {
        int size = 0;
        file->read((char*)&size, sizeof(size));
        for (int i = 0; i < size; i++) {
            int first;
            int second;
            file->read((char*)&first, sizeof(first));
            file->read((char*)&second, sizeof(second));
            map[(interactionKey)first] = second;
        }
    }

    void remapKey(interactionKey ik, int key)
    {
        map[ik] = key;
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (capturingInput) {
            if (capturingAmount > 0) {
                if (inputFunction) {
                    inputFunction(key);
                    capturingAmount--;
                    return;
                }
            }
            else {
                capturingInput = false;
                return;
            }
        }

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

        if (action == GLFW_PRESS) {
            if (capturingInput) {
                if (capturingAmount > 0) {
                    if (inputFunction) {
                        inputFunction(button);
                        capturingAmount--;
                        return;
                    }
                }
                else {
                    capturingInput = false;
                    return;
                }
            }
        }

        switch (action) {
        case GLFW_RELEASE:
            keys[button] = UP;
            break;
        case GLFW_PRESS:
            keys[button] = DOWN;
            break;
        case GLFW_REPEAT:
            keys[button] = HELD;
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