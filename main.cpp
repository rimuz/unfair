#include "engine/scene.hpp"
#include "io/data.hpp"
#include "io/save.hpp"

int main(){
    #ifdef _WIN32
    ue::save::init();
    #endif
    ue::save::load();
    ue::assets::loadAssets();

    ue::Scene* scene = new ue::Scene;
    scene->showSplashScreen();
    scene->startScene();

    scene->clearMemory();

    return 0;
}
