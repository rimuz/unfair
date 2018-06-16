#include "engine/scene.hpp"
#include "io/data.hpp"
#include "io/save.hpp"
#include "utils/gamedefs.hpp"

#include <sstream>
#include <iomanip>
#include <iostream>

namespace ue{
	void Scene::showStartScreen(){
	    in_game = false;
	    showing_game = false;

	    if(assets::bgMusic.getStatus() == sf::Music::Playing){
            assets::bgMusic.stop();
        }

        TimeJob music;
        music.onEnd = [](TimeJob&){
            if(assets::menuMusic.getStatus() != sf::Music::Playing){
                assets::menuMusic.play();
            }
        };
        music.remeaningTime = 1.f;
        music.priority = true;
        addTimeJob(music);

        clearMemory();

        addSubject(startMenu);
        addSubject(title);
        addSubject(versionText);

        onEscape = [this](){ exit_game };
    }

    void Scene::showSplashScreen(){
        clearMemory();

        Subject* img = new Subject;
        img->sprite = &ue::assets::splash;

        img->locType = SubjectLocation::CENTER;
        img->sizeType = SubjectSize::KEEP_PROPORTIONS;

        img->size.x = 400;
        img->size.y = 300;

        addSubject(img);

        TimeJob job;
        #ifndef DEBUG_MODE
        job.remeaningTime = SPLASH_TIME;
        #endif
        job.onEnd = [this] (TimeJob&){
            showStartScreen();
        };
        jobs.push_back(job);
        TimeJob& jobRef = jobs.back();

        onEscape = [&jobRef](){ jobRef.remeaningTime = 0.f; };
    }

    void Scene::showGameScreen(size_t diff){
        in_game = false;
	    showing_game = true;

	    clearMemory();

        Game* game = new Game(diff);
        game->viewer = viewer;
        game->bossViewer = bossViewer;

        addSubject(game);
        showHUD();

        onEscape = [this](){ togglePauseGame(); };

        if(assets::menuMusic.getStatus() == sf::Music::Playing){
             assets::menuMusic.stop();
        }

        TimeJob music;
        music.onEnd = [](TimeJob&){
            if(assets::bgMusic.getStatus() != sf::Music::Playing){
                assets::bgMusic.play();
            }
        };
        music.remeaningTime = 1.f;
        music.priority = true;
        addTimeJob(music);
    }

    void Scene::showSelectDifficultyScreen(){
        clearMemory();
        chooseDifficultyButtons->selected = 0;

        addSubject(difficultyText);
        addSubject(chooseDifficultyButtons);

        onEscape = [this](){ showStartScreen(); };
    }

    void Scene::showHUD(){
        addSubject(viewer);
        addSubject(bossViewer);
        addSubject(pointsGroup);
    }

    void Scene::removeHUD(){
        removeSubject(viewer);
        removeSubject(bossViewer);
        removeSubject(pointsGroup);
    }

    void Scene::showLevelScreen(size_t n, std::string& name){
        if(GAME.paused){
            togglePauseGame();
        }

        levelText->text.setString(std::string("LEVEL ") + (char) (n+48+1));
        bossNameText->text.setString(name);
        levelText->pack();
        bossNameText->pack();
        bossGroup->packVertical();

        TimeJob job;
        job.onEnd = [this](TimeJob&){
            in_game = false;
            if(!GAME.paused){
                removeHUD();
            }
            addSubject(blackScreen);
            addSubject(levelText);
            addSubject(pressSpaceText);
            addSubject(bossGroup);
        };
        job.priority = true;
        addTimeJob(job);
    }

    void Scene::removeLevelScreen(){
        if(!GAME.paused){
            showHUD();
        }

        removeSubject(blackScreen);
        removeSubject(levelText);
        removeSubject(pressSpaceText);
        removeSubject(bossGroup);
    }

    void Scene::showScoreScreen(){
        clearMemory();
        scores->selected = 0;
        scores->center = 5;
        scores->buttons.clear();

        std::sort(save::score.begin(), save::score.end(), [] (save::Score& a, save::Score& b) { return a.score > b.score; });
        for(auto& sc : save::score){
            std::string score = std::to_string(sc.score);
            std::string space_bef_score(20 - score.size(), ' ');
            scores->addButton(chooseDifficultyButtons->buttons[sc.mode]->text.getString() +
                              ((sc.mode == 0) ? "         " : "                        ") + showTime(sf::seconds(static_cast<float>(sc.time)))
                              + space_bef_score + score, [](){});
        }
        int remaining = 10 - save::score.size();
        if(remaining > 0){
            for(int i = 0; i < remaining; i++){
                scores->addButton("-----\t\t\t-:-:-\t\t---", [](){});
            }
        }
        scores->pack();

        addSubject(scores);
        addSubject(scoreScreenText);

        onEscape = [this](){ showStartScreen(); };
    }

    void Scene::showStatsScreen(){
        clearMemory();

        int64_t min = 0, max = 0, avg = 0;
        float wl = 0;
        if(save::score.size()){
            std::sort(save::score.begin(), save::score.end(), [] (save::Score& a, save::Score& b) { return a.score > b.score; });

            min = save::score.back().score;
            max = save::score.front().score;

            for(size_t i = 0; i < save::score.size(); i++){
                avg += save::score[i].score;
            }
            avg /= save::score.size();
        }
        if(save::options.losses){
            wl = static_cast<float>(save::options.wins) / save::options.losses;
        } else {
            wl = static_cast<float>(save::options.wins);
        }
        std::stringstream wl_ss;
        wl_ss << std::fixed << std::setprecision(2) << wl;

        statsGroup->texts[0]->text.setString(std::string("TOTAL GAMES: ") + std::to_string(save::options.games));
        statsGroup->texts[0]->pack();
        statsGroup->texts[1]->text.setString(std::string("WINS: ") + std::to_string(save::options.wins));
        statsGroup->texts[1]->pack();
        statsGroup->texts[2]->text.setString(std::string("LOSSES: ") + std::to_string(save::options.losses));
        statsGroup->texts[2]->pack();
        statsGroup->texts[3]->text.setString(std::string("W/L: ") + wl_ss.str());
        statsGroup->texts[3]->pack();
        statsGroup->texts[4]->text.setString(std::string("MAX SCORE: ") + std::to_string(max));
        statsGroup->texts[4]->pack();
        statsGroup->texts[5]->text.setString(std::string("MIN SCORE: ") + std::to_string(min));
        statsGroup->texts[5]->pack();
        statsGroup->texts[6]->text.setString(std::string("AVERAGE SCORE: ") + std::to_string(avg));
        statsGroup->texts[6]->pack();

        statsGroup->packVertical();

        addSubject(statsText);
        addSubject(statsGroup);

        onEscape = [this](){ showStartScreen(); };
    }

    void Scene::removeScoreScreen(){
        removeSubject(scores);
        removeSubject(scoreScreenText);
    }

    void Scene::showEndScreen(Level& level, bool win){
        in_game = false;
        ywButtons->selected = 0;

        if(win){
            save::options.wins++;
        } else {
            save::options.losses++;
        }

        if(GAME.paused){
            togglePauseGame();
        }

        removeHUD();

        ywText->text.setFillColor(win ? sf::Color::Yellow : sf::Color::Red);
        ywText->text.setString(win ? "You won!" : "You died!");
        ywText->pack();

        ywStats->texts[0]->text.setString(std::string("MODE: ") + chooseDifficultyButtons->buttons[GAME.currLevel.diff]->text.getString());
        ywStats->texts[0]->pack();
        ywStats->texts[1]->text.setString(std::string("TIME: ") + showTime(GAME.ellapsed));
        ywStats->texts[1]->pack();
        ywStats->texts[2]->text.setString(std::string("SCORE: ") + std::to_string(level.points));
        ywStats->texts[2]->pack();
        ywStats->texts[3]->text.setString(std::string("HEARTS: ") + std::to_string(level.lifes));
        ywStats->texts[3]->pack();
        ywStats->texts[4]->text.setString(std::string("BOMBS: ") + std::to_string(level.bombs));
        ywStats->texts[4]->pack();
        ywStats->texts[5]->text.setString(std::string("LEVEL: ") + std::to_string(level.n+1));
        ywStats->texts[5]->pack();
        ywStats->texts[6]->text.setString(std::string("BOSS: ") + level.boss->name);
        ywStats->texts[6]->pack();

        ywStats->packVertical();

        addSubject(blackScreen);
        addSubject(ywText);
        addSubject(ywStats);
        addSubject(ywButtons);

        onEscape = [this](){ leaveGame(); showStartScreen(); };
    }

    std::vector<Subject*> beforeOptions;
    std::function<void(void)> onEscapeBeforeOptions;
    bool showing_game_before;

    void Scene::showOptionsScreen(){
        beforeOptions = subjects;
        onEscapeBeforeOptions = onEscape;
        subjects.clear();
        optionsMenu->selected = 0;
        addSubject(optionsText);
        addSubject(optionsMenu);

        onEscape = [this](){ removeOptionsScreen(); };
        showing_game_before = showing_game;
        showing_game = false;
    }

    void Scene::removeOptionsScreen(){
        subjects = beforeOptions;
        onEscape = onEscapeBeforeOptions;
        showing_game = showing_game_before;
    }
}
