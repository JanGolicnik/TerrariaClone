#include <componentsystems.h>
#include <game.h>

bool uiSystem::mouseClicked;
bool uiSystem::mouseRightClicked;
int uiSystem::body;
int drawSystem::mainDrawable;
int drawSystem::behindBackground;
int drawSystem::front;

int main() {
    game::init();
    game::swapTo = STARTMENU;
    game::run();
    game::endall();
    return 0;
}
