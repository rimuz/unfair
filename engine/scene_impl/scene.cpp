#include <SFML/System.hpp>

#include <iostream>
#include <string>

#include "engine/scene.hpp"
#include "io/data.hpp"
#include "io/save.hpp"
#include "utils/options.hpp"
#include "version.h"

#include <cstdlib>

namespace ue{
    Scene* Scene::curr_scene = nullptr;
    bool in_game = false;
    bool showing_game = false;
    #include "engine/cheats.inc"

    namespace leveldefs{
        std::tuple<ui, ui, ui, ui, f, f, f, f, f, f> levels[5] = {
            /*
                HEARTS, BOMBS, HEARTS_PRIZE, BOMBS_PRIZE,
                +DMG, +POINTS_PER_DMG, +POINTS_PER_BOSS_DEATH,
                -POINTS_PER_SHOOT, -POINTS_PER_SECOND, -POINTS_PER_BOMB
            */
            std::make_tuple(5, 2, 2, 1, 1.f, 10.f, 15.f, 5.f, 1.f, 50.f),
            std::make_tuple(3, 2, 2, 1, 0.75f, 12.f, 17.f, 4.f, 1.f, 45.f),
            std::make_tuple(3, 2, 2, 1, 0.65f, 15.f, 20.f, 3.f, 1.f, 40.f),
            std::make_tuple(2, 1, 1, 1, 0.55f, 17.f, 25.f, 2.f, 1.f, 35.f),
            std::make_tuple(1, 1, 1, 1, 0.5f, 20.f, 30.f, 1.f, 0.5f, 30.f),
        };

        std::tuple<sf::Color, sf::Color, sf::Color> levels_bg[5] = {
            std::make_tuple(sf::Color(75, 75, 75), sf::Color(90, 90, 90), sf::Color(50, 50, 50)),
            std::make_tuple(sf::Color(75, 75, 75), sf::Color(110, 110, 110), sf::Color(50, 50, 50)),
            std::make_tuple(sf::Color(75, 75, 75), sf::Color(130, 130, 130), sf::Color(50, 50, 50)),
            std::make_tuple(sf::Color(75, 75, 75), sf::Color(150, 150, 150), sf::Color(50, 50, 50)),
            std::make_tuple(sf::Color(75, 75, 75), sf::Color(170, 170, 170), sf::Color(50, 50, 50)),
        };
    }

    Scene::Scene(){
        title = new Subject;
        title->sprite = &ue::assets::title;

        title->locType = SubjectLocation::POPUP;
        title->sizeType = SubjectSize::KEEP_PROPORTIONS;

        title->size.x = 737.f * 0.5f;
        title->size.y = 198.f * 0.5f;

        title->permanent = true;

        startMenu = new TextButtonGroup;

        startMenu->addButton("PLAY", [this](){ showSelectDifficultyScreen(); });
        startMenu->addButton("STATS", [this](){ showStatsScreen(); });
        startMenu->addButton("SCORES", [this](){ showScoreScreen(); });
        //startMenu->addButton("CHALLENGES", [](){});
        startMenu->addButton("OPTIONS", [this](){ showOptionsScreen(); });
        startMenu->addButton("EXIT", [](){ exit_game });

        //startMenu->buttons[3]->disable(startMenu->buttons[3]->text.getString());

        startMenu->locType = SubjectLocation::CENTER;
        startMenu->sizeType = SubjectSize::KEEP_PROPORTIONS;
        startMenu->packVertical();

        startMenu->permanent = true;

        versionText = new Text;
        versionText->text.setString(
                    std::string("V")
                    + std::to_string(AutoVersion::MAJOR) + "."
                    + std::to_string(AutoVersion::MINOR) + "."
                    + std::to_string(AutoVersion::BUILD) + " "
                    + AutoVersion::STATUS_SHORT + " (build "
                    + std::to_string(AutoVersion::BUILDS_COUNT) + ")"
        );
        #ifdef DEBUG_MODE
        versionText->text.setFillColor(sf::Color::Yellow);
        #else
        versionText->text.setFillColor(sf::Color::White);
        #endif
        versionText->text.setCharacterSize(30);
        versionText->text.setFont(STANDARD_FONT);
        versionText->pack();

        versionText->locType = SubjectLocation::RIGHT_BOTTOM;
        versionText->sizeType = SubjectSize::KEEP_PROPORTIONS;

        versionText->permanent = true;

        difficultyText = new Text;
        difficultyText->text.setString("CHOOSE A DIFFICULTY");
        difficultyText->text.setFillColor(sf::Color::White);
        difficultyText->text.setCharacterSize(70);
        difficultyText->text.setFont(STANDARD_FONT);
        difficultyText->pack();

        difficultyText->locType = SubjectLocation::POPUP;
        difficultyText->sizeType = SubjectSize::KEEP_PROPORTIONS;

        difficultyText->permanent = true;

        chooseDifficultyButtons = new TextButtonGroup;
        chooseDifficultyButtons->addButton("PIECE OF CAKE", [this](){ showGameScreen(PIECE_OF_CAKE); });
        chooseDifficultyButtons->addButton("NOOB", [this](){ showGameScreen(NOOB); });
        chooseDifficultyButtons->addButton("HARD", [this](){ showGameScreen(HARD); });
        chooseDifficultyButtons->addButton("PRO", [this](){ showGameScreen(PRO); });
        chooseDifficultyButtons->addButton("HELL", [this](){ showGameScreen(HELL); });

        updateDifficulties();

        chooseDifficultyButtons->packVertical();

        chooseDifficultyButtons->locType = SubjectLocation::CENTER;
        chooseDifficultyButtons->sizeType = SubjectSize::KEEP_PROPORTIONS;

        chooseDifficultyButtons->permanent = true;

        pauseText = new Text;
        pauseText->text.setString("PAUSED");
        pauseText->text.setFillColor(sf::Color::White);
        pauseText->text.setCharacterSize(60);
        pauseText->text.setFont(STANDARD_FONT);
        pauseText->pack();

        pauseText->locType = SubjectLocation::POPUP;
        pauseText->sizeType = SubjectSize::KEEP_PROPORTIONS;

        pauseText->permanent = true;

        pauseButtons = new TextButtonGroup;

        pauseButtons->addButton("CONTINUE", [this](){ togglePauseGame(); });
        pauseButtons->addButton("RESTART", [this](){ leaveGame(); showGameScreen(GAME.currLevel.diff); });
        pauseButtons->addButton("OPTIONS", [this](){ showOptionsScreen(); });
        pauseButtons->addButton("MENU", [this](){ leaveGame(); showStartScreen(); });
        pauseButtons->addButton("EXIT", [](){ exit_game });

        pauseButtons->locType = SubjectLocation::CENTER;
        pauseButtons->sizeType = SubjectSize::KEEP_PROPORTIONS;
        pauseButtons->packVertical();

        pauseButtons->permanent = true;

        space = new Subject;
        space->size.y = 5;
        space->locType = SubjectLocation::TOP;
        space->sizeType = SubjectSize::KEEP_PROPORTIONS;

        space->permanent = true;

        pointsLevelTime = new TextGroup;
        pointsLevelTime->addText("TIME: 0:0:0", 30, sf::Color::Green);
        pointsLevelTime->addText("MODE: PIECE OF CAKE", 30, sf::Color::Red);
        pointsLevelTime->addText("LEVEL: 1", 30, sf::Color::White);
        pointsLevelTime->addText("POINTS: 100", 30, sf::Color::Yellow);
        pointsLevelTime->packVertical();

        pointsLevelTime->locType = SubjectLocation::BOTTOM;
        pointsLevelTime->sizeType = SubjectSize::KEEP_PROPORTIONS;

        pointsLevelTime->permanent = true;

        pointsGroup = new Group;
        pointsGroup->addSubject(space);
        pointsGroup->addSubject(pointsLevelTime);
        pointsGroup->packVertical();

        pointsGroup->locType = SubjectLocation::TOP;
        pointsGroup->sizeType = SubjectSize::KEEP_PROPORTIONS;

        pointsGroup->permanent = true;

        viewer = new LifeViewer;
        bossViewer = new BossLifeViewer;

        viewer->permanent = true;
        bossViewer->permanent = true;

        levelText = new Text;
        levelText->text.setString("LEVEL X");
        levelText->text.setFillColor(sf::Color::White);
        levelText->text.setCharacterSize(50);
        levelText->text.setFont(STANDARD_FONT);
        levelText->pack();

        levelText->locType = SubjectLocation::POPUP;
        levelText->sizeType = SubjectSize::KEEP_PROPORTIONS;

        levelText->permanent = true;

        blackScreen = new Subject;

        blackScreen->sprite = &assets::black_screen;
        blackScreen->locType = SubjectLocation::FILL;

        blackScreen->permanent = true;

        bossText = new Text;
        bossText->text.setString("BOSS:");
        bossText->text.setFillColor(sf::Color::White);
        bossText->text.setCharacterSize(50);
        bossText->text.setFont(STANDARD_FONT);
        bossText->pack();

        bossText->locType = SubjectLocation::TOP;
        bossText->sizeType = SubjectSize::KEEP_PROPORTIONS;

        bossText->permanent = true;

        bossNameText = new Text;
        bossNameText->text.setString("NULL");
        bossNameText->text.setFillColor(sf::Color::Red);
        bossNameText->text.setCharacterSize(90);
        bossNameText->text.setFont(STANDARD_FONT);
        bossNameText->pack();

        bossNameText->locType = SubjectLocation::BOTTOM;
        bossNameText->sizeType = SubjectSize::KEEP_PROPORTIONS;

        bossNameText->permanent = true;

        bossGroup = new Group;
        bossGroup->addSubject(bossText);
        bossGroup->addSubject(bossNameText);
        bossGroup->packVertical();
        bossGroup->sizeType = SubjectSize::KEEP_PROPORTIONS;
        bossGroup->locType = SubjectLocation::CENTER;

        bossGroup->permanent = true;
        pressSpaceText = new Text;
        pressSpaceText->text.setString("PRESS ENTER TO CONTINUE");
        pressSpaceText->text.setFillColor(sf::Color::White);
        pressSpaceText->text.setCharacterSize(30);
        pressSpaceText->text.setFont(STANDARD_FONT);
        pressSpaceText->pack();

        pressSpaceText->locType = SubjectLocation::SOUTH_POPUP;
        pressSpaceText->sizeType = SubjectSize::KEEP_PROPORTIONS;

        pressSpaceText->permanent = true;

        ywText = new Text;
        ywText->text.setCharacterSize(80);
        ywText->text.setFont(STANDARD_FONT);
        ywText->locType = SubjectLocation::POPUP;
        ywText->sizeType = SubjectSize::KEEP_PROPORTIONS;
        ywText->permanent = true;

        ywStats = new TextGroup;
        ywStats->addText("DIFFICULTY: PIECE_OF_CAKE", 40);
        ywStats->addText("TIME: 100", 40, sf::Color(180, 180, 180));
        ywStats->addText("SCORE: 100", 40);
        ywStats->addText("HEARTS: 100", 40, sf::Color(180, 180, 180));
        ywStats->addText("BOMBS: 100", 40);
        ywStats->addText("LEVEL: 100", 40, sf::Color(180, 180, 180));
        ywStats->addText("BOSS: 100", 40);
        ywStats->packVertical();

        ywStats->locType = SubjectLocation::CENTER;
        ywStats->sizeType = SubjectSize::KEEP_PROPORTIONS;

        ywStats->permanent = true;

        ywButtons = new TextButtonGroup;
        ywButtons->addButton("RESTART", [this](){ leaveGame(); showGameScreen(GAME.currLevel.diff); });
        ywButtons->addButton("MENU",  [this](){ leaveGame(); showStartScreen(); });

        ywButtons->packVertical();
        ywButtons->sizeType = SubjectSize::KEEP_PROPORTIONS;
        ywButtons->locType = SubjectLocation::SOUTH_POPUP;

        ywButtons->permanent = true;

        scores = new ScrollableTextButtonGroup;

        scores->permanent = true;

        scores->locType = SubjectLocation::CENTER;
        scores->sizeType = SubjectSize::KEEP_PROPORTIONS;

        scoreScreenText = new Text;
        scoreScreenText->text.setString("SCORES:");
        scoreScreenText->text.setFillColor(sf::Color::White);
        scoreScreenText->text.setCharacterSize(40);
        scoreScreenText->text.setFont(STANDARD_FONT);
        scoreScreenText->pack();

        scoreScreenText->locType = SubjectLocation::POPUP;
        scoreScreenText->sizeType = SubjectSize::KEEP_PROPORTIONS;

        scoreScreenText->permanent = true;

        watch = new Clock;

        optionsMenu = new OptionsGroup;
        optionsMenu->init();
        optionsMenu->locType = SubjectLocation::CENTER;
        optionsMenu->sizeType = SubjectSize::KEEP_PROPORTIONS;

        optionsMenu->permanent = true;

        optionsText = new Text;
        optionsText->text.setString("OPTIONS:");
        optionsText->text.setFillColor(sf::Color::White);
        optionsText->text.setCharacterSize(40);
        optionsText->text.setFont(STANDARD_FONT);
        optionsText->pack();

        optionsText->locType = SubjectLocation::POPUP;
        optionsText->sizeType = SubjectSize::KEEP_PROPORTIONS;

        optionsText->permanent = true;

        statsText = new Text;
        statsText->text.setString("STATS:");
        statsText->text.setFillColor(sf::Color::Yellow);
        statsText->text.setCharacterSize(40);
        statsText->text.setFont(STANDARD_FONT);
        statsText->pack();

        statsText->locType = SubjectLocation::POPUP;
        statsText->sizeType = SubjectSize::KEEP_PROPORTIONS;

        statsText->permanent = true;

        statsGroup = new TextGroup;
        statsGroup->addText("TOTAL TRIES: ", 40);
        statsGroup->addText("WINS: ", 40, sf::Color(180, 180, 180));
        statsGroup->addText("LOSSES: ", 40);
        statsGroup->addText("W/L: ", 40, sf::Color(180, 180, 180));
        statsGroup->addText("MAX SCORE: ",40);
        statsGroup->addText("MIN SCORE: ", 40, sf::Color(180, 180, 180));
        statsGroup->addText("AVERAGE SCORE: ", 40);
        statsGroup->packVertical();

        statsGroup->locType = SubjectLocation::CENTER;
        statsGroup->sizeType = SubjectSize::KEEP_PROPORTIONS;

        statsGroup->permanent = true;

        fps = new Text;
        fps->text.setCharacterSize(30);
        fps->text.setFillColor(sf::Color::Yellow);
        fps->text.setFont(STANDARD_FONT);
        fps->pack();

        fps->locType = SubjectLocation::LEFT_BOTTOM;
        fps->sizeType = SubjectSize::KEEP_PROPORTIONS;

        fps->permanent = true;

        clearScreen = new Subject;
        clearScreen->sizeType = SubjectSize::CLEAR_SCREEN;

        clearScreen->permanent = true;
    }
}
