#include "Window.h"

#include <core.h>
#include <input.h>
#include <utils.h>
#include <game.h>
#include <Layers.h>
#include <camera.h>

namespace Window {

    std::vector<glm::vec2> resolutions;
    int currRes;
    std::string currResText;

    GLuint mainFB;
    GLuint mainFBT;
    GLuint tmpFB;
    GLuint tmpFBT;

    GLFWwindow* window;
    glm::vec2 res = glm::vec2(1920 * 0.75, 1080 * 0.75);
    bool fullscreen = true;
    float transparency = 1;

    float zoom = 1.0f;

    const OverlayVertex overlayVert[4] = {
    {glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
    {glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
    {glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
    {glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
    };
    const int32_t overlayEl[6] = {
        0, 1, 3,
        0, 2, 3
    };

    uint32_t overlayVA;
    uint32_t overlayVB;
    uint32_t overlayIB;

    bool bloom = true;

    void glfwErrorCallback(int error, const char* description)
    {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
        std::cout << error << " " << description << "\n";
    }

    std::string windowName() {
        switch (rand() % 20) {
        case 0:
            return "Airarret";
        case 1:
            return "Airarret: Also try SNKRX!";
        case 2:
            return "Airarret: Now with more things to kill you!";
        case 3:
            return "Airarret: 1 + 1 = 10";
        case 4:
            return "Airarret: Rise of the Slimes";
            break;
        case 5:
            return "Airarret : Rumors of the Guides' death were greatly exaggerated";
            break;
        case 6:
            return "Airarret: Also try Brotato!";
            break;
        case 7:
            break;
            return "Airarret: Also try Terraria!";
        case 8:
            return "Airarret: Also try         !";
            break;
        case 9:
            return "Airarret 2: Electric Boogaloo";
            break;
        case 10:
            return "Airarret: Better than life";
            break;
        case 11:
            break;
            return "Airarret: NOT THE BEES!!!";
        case 12:
            return "Airarret: Cthulhu is mad... and is missing an eye!";
            break;
        case 13:
            return "Airarret : Airarret : Airarret : ";
            break;
        case 14:
            return "Airarret: What's that purple spiked thing?";
            break;
        case 15:
            return "Airarret: I don't know that-- aaaaa!";
            break;
        case 16:
            return "Airarret: Now in 1D";
            break;
        case 17:
            return "Airarret: Press alt-f4";
            break;
        case 18:
            return "Airarret: Dividing by zero";
            break;
        case 19:
            return "Airarret: Now with SOUND";
            break;
        }
        return "Airarret";
    }

	void init()
	{
        glfwSetErrorCallback(glfwErrorCallback);
        if (!glfwInit()) {
            std::cout << "failed to init glfw" << std::endl;
        }
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_RESIZABLE, true);
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window = glfwCreateWindow(res.x, res.y, windowName().c_str(), NULL, NULL);
        if (window == NULL) {
            std::cout << "failed to create window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "failed to init glad" << std::endl;
        }
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        glfwSetKeyCallback(window, (GLFWkeyfun)input::key_callback);
        glfwSetScrollCallback(window, (GLFWscrollfun)input::scroll_callback);
        glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)input::mousebutton_callback);
        glfwSetWindowSizeCallback(window, (GLFWwindowsizefun)input::windowsize_callback);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
        glfwSetWindowOpacity(window, transparency);
        glfwSetWindowSizeLimits(window, 640, 480, GLFW_DONT_CARE, GLFW_DONT_CARE);
        if (fullscreen) {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
        }

        utils::createFrameBuffer(&mainFB, &mainFBT, res, true);
        utils::createFrameBuffer(&tmpFB, &tmpFBT, res);

        glGenVertexArrays(1, &overlayVA);
        glBindVertexArray(overlayVA);

        glGenBuffers(1, &overlayVB);
        glBindBuffer(GL_ARRAY_BUFFER, overlayVB);
        glBufferData(GL_ARRAY_BUFFER, sizeof(overlayVert), overlayVert, GL_STATIC_DRAW);

        glGenBuffers(1, &overlayIB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, overlayIB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(overlayEl), overlayEl, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OverlayVertex), 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(OverlayVertex), (void*)sizeof(glm::vec3));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        currRes = 0;
        resolutions.push_back(glm::vec2(1024, 734));
        resolutions.push_back(glm::vec2(1152, 826));
        resolutions.push_back(glm::vec2(1280, 688));
        resolutions.push_back(glm::vec2(1280, 734));
        resolutions.push_back(glm::vec2(1280, 765));
        resolutions.push_back(glm::vec2(1280, 918));
        resolutions.push_back(glm::vec2(1280, 979));
        resolutions.push_back(glm::vec2(1360, 734));
        resolutions.push_back(glm::vec2(1366, 734));
        resolutions.push_back(glm::vec2(1440, 860));
        resolutions.push_back(glm::vec2(1600, 860));
        resolutions.push_back(glm::vec2(1600, 979));
        resolutions.push_back(glm::vec2(1600, 1147));
        resolutions.push_back(glm::vec2(1680, 1004));
        resolutions.push_back(glm::vec2(1920, 1032));
        resolutions.push_back(glm::vec2(1920, 1147));
        resolutions.push_back(glm::vec2(1920, 1377));
        resolutions.push_back(glm::vec2(2560, 1377));
	}
	void drawMain()
	{
        glBindFramebuffer(GL_FRAMEBUFFER, mainFB);
        glUseProgram(globals::zoomShaderID);
        glBindVertexArray(overlayVA);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tmpFBT);
        glUniform1i(5, 0);
        glUniform1f(2, zoom);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	void drawOverlays()
	{
        glBindFramebuffer(GL_FRAMEBUFFER, mainFB);
        glUseProgram(globals::displayShaderID);
        glBindVertexArray(overlayVA);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tmpFBT);
        glUniform1i(5, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        if (bloom) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glUseProgram(globals::bloomShaderID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mainFBT);
            glUniform1i(5, 0);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        else {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glUseProgram(globals::displayShaderID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mainFBT);
            glUniform1i(5, 0);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
	}
    void changeResolution(glm::vec2 val) {
        glfwSetWindowSize(window, val.x, val.y);
        glViewport(0, 0, val.x, val.y);
        glDeleteFramebuffers(1, &mainFB);
        glDeleteFramebuffers(1, &tmpFB);
        glDeleteTextures(1, &mainFBT);
        glDeleteTextures(1, &tmpFBT);
        utils::createFrameBuffer(&mainFB, &mainFBT, val);
        utils::createFrameBuffer(&tmpFB, &tmpFBT, val);
        res = val;
        game::resChanged();
    }
    void nextResolution()
    {
        currRes++;
        if (currRes > resolutions.size() - 1) {
            currRes = 0;
        }
        changeResolution(resolutions[currRes]);
    }
    void clean()
    {
        glDeleteFramebuffers(1, &mainFB);
        glDeleteFramebuffers(1, &tmpFB);
        glDeleteTextures(1, &mainFBT);
        glDeleteTextures(1, &tmpFBT);
        glDeleteBuffers(1, &overlayVA);
        glDeleteBuffers(1, &overlayIB);
        glDeleteBuffers(1, &overlayVB);
        glfwDestroyWindow(window);
    }
    bool open()
    {
        return !glfwWindowShouldClose(window);
    }
    void changeTransparency(float val)
    {
        static float lasttransparency = 1;
        if (transparency == lasttransparency) return;
        lasttransparency = transparency = glm::clamp(transparency, 0.1f, 1.0f);
        glfwSetWindowOpacity(window, transparency);
    }
    void toggleFullscreen() {
        if (glfwGetWindowMonitor(window)) {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            int monitorWidth = mode->width;
            int monitorHeight = mode->height;
            int posx = (monitorWidth - resolutions[currRes].x) / 2;
            int posy = (monitorHeight - resolutions[currRes].y) / 2;
            glm::vec2 monitorWH;
            glfwSetWindowMonitor(window, NULL, posx, posy, resolutions[currRes].x, resolutions[currRes].y, GLFW_DONT_CARE);
            fullscreen = false;
        }
        else {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            input::windowsize_callback(window, mode->width, mode->height);
            fullscreen = true;
        }
    }

    glm::vec2 mouseBlockCoordsGlobal(bool round, glm::vec2 clampx, glm::vec2 clampy)
    {
        double x, y;
        glfwGetCursorPos(Window::window, &x, &y);

        y = res.y - y;

        //mouse position from -1 to 1
        x = x / res.x;
        y = y / res.y;
        x = x - 0.5;
        y = y - 0.5;

        //clamp?
        x = glm::clamp((float)x, clampx.x, clampx.y);
        y = glm::clamp((float)y, clampx.x, clampx.y);
        //find out which block on screen we are
        x = Layers::trueBsOnScr.x * x;
        y = Layers::trueBsOnScr.y * y;

        x = -camera::pos.x + x;
        y = -camera::pos.y + y;

        if (round) {
            x = glm::round(x);
            y = glm::round(y);
        }
        return glm::vec2(x, y);
    }
    glm::vec2 mouseBlockCoords(bool round, glm::vec2 clampx, glm::vec2 clampy)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);

        y = res.y - y;

        //mouse position from -1 to 1
        x = x / res.x;
        y = y / res.y;
        x = x - 0.5;
        y = y - 0.5;

        //apply zoom
        x /= zoom;
        y /= zoom;

        //clamp?
        x = glm::clamp((float)x, clampx.x, clampx.y);
        y = glm::clamp((float)y, clampx.x, clampx.y);
        //find out which block on screen we are
        x = Layers::trueBsOnScr.x * x;
        y = Layers::trueBsOnScr.y * y;

        x = -camera::pos.x + x;
        y = -camera::pos.y + y;

        if (round) {
            x = glm::round(x);
            y = glm::round(y);
        }
        return glm::vec2(x, y);
    }

    glm::vec2 mouseCoords() {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return glm::vec2(x, y);
    }

    void bindTemporaryBuffer(glm::vec4 clr)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, Window::tmpFB);
        glClearColor(clr.r, clr.g, clr.b, clr.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void swapBuffers()
    {
        glfwSwapBuffers(Window::window);
    }

    void close()
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    glm::vec2 normMouseCoords() {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        x /= res.x;
        x = ((x - 0.5) / zoom) + 0.5;
        x = x * 2 - 1;

        y /= res.y;
        y = ((y - 0.5) / zoom) + 0.5;
        y = y * 2 - 1;

        return glm::vec2(x, -y);
    }
}