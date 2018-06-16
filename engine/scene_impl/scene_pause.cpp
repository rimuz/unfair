#include "engine/scene.hpp"
#include "io/save.hpp"

namespace ue{
    void Scene::togglePauseGame(){
        if(Game::curr_game){
            GAME.paused = !GAME.paused;
            if(GAME.paused){
                if(GAME.showingLevel){
                    removeLevelScreen();
                } else {
                    removeHUD();
                }
                pauseButtons->selected = 0;

                addSubject(blackScreen);
                addSubject(pauseText);
                addSubject(pauseButtons);
            } else {
                if(GAME.showingLevel){
                    showLevelScreen(GAME.currLevel.n, GAME.currLevel.boss->name);
                } else {
                    showHUD();
                }
                pauseButtons->selected = 0;

                removeSubject(blackScreen);
                removeSubject(pauseText);
                removeSubject(pauseButtons);
            }
        }
    }

    void Scene::leaveGame(){
        GAME.showingLevel = false;
        if(GAME.paused){
            togglePauseGame();
        }

        removeHUD();

        clearTimeJobs();
        clearMemory();
    }


    void Scene::pause(){
        if(Game::curr_game && in_game && !GAME.paused){
            togglePauseGame();
        }
    }

    void Scene::updateDifficulties(){
        bool enabledNoob = false;
        bool enabledHard = false;
        bool enabledPro = false;
        bool enabledHell = false;

        for(save::Score& sc : save::score){
            switch(sc.mode){
                case 4:
                case 3:{
                    enabledHell = true;
                    break;
                }

                case 2:{
                    enabledPro = true;
                    break;
                }
                case 1:{
                    enabledHard = true;
                    break;
                }

                case 0:{
                    enabledNoob = true;
                    break;
                }
            }
        }

        if(enabledNoob){
            chooseDifficultyButtons->buttons[1]->enable("NOOB");
        } else {
            chooseDifficultyButtons->buttons[1]->disable("NOOB (LOCKED)");
        }

        if(enabledHard){
            chooseDifficultyButtons->buttons[2]->enable("HARD");
        } else {
            chooseDifficultyButtons->buttons[2]->disable("HARD (LOCKED)");
        }

        if(enabledPro){
            chooseDifficultyButtons->buttons[3]->enable("PRO");
        } else{
            chooseDifficultyButtons->buttons[3]->disable("PRO (LOCKED)");
        }

        if(enabledHell){
            chooseDifficultyButtons->buttons[4]->enable("HELL");
        } else {
            chooseDifficultyButtons->buttons[4]->disable("HELL (LOCKED)");
        }
    }
}
