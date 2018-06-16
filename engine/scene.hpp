#ifndef         SCENE_HPP
#define         SCENE_HPP

#define         SCENE           (*ue::Scene::curr_scene)
#define         MOUSE_POS       ue::Scene::curr_scene->mousePosition
#define         TPF             ue::Scene::curr_scene->tpf

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <string>
#include <unordered_map>
#include <tuple>

#include "basic/subject.hpp"
#include "boss/boss.hpp"
#include "basic/game.hpp"
#include "utils/clock.hpp"
#include "utils/options.hpp"

namespace ue{
    class Game;
    class Scene;
    class LifeViewer;
    class BossLifeViewer;

    using cheatMap = const std::unordered_map<std::string, std::function<void(Scene&, Game&, Level&)>>;

    namespace leveldefs{
        enum lvl_defs{
            HEARTS, BOMBS, HEARTS_PRIZE, BOMBS_PRIZE, DMG, POINTS_PER_DMG,
            POINTS_PER_BOSS_DEATH, POINTS_PER_SHOOT, POINTS_PER_SECOND, POINTS_PER_BOMB
        };

        using ui = unsigned;
        using f = float;
        extern std::tuple<ui, ui, ui, ui, f, f, f, f, f, f> levels[];
        extern std::tuple<sf::Color, sf::Color, sf::Color> levels_bg[];
    }

    extern std::string cheat;
    extern cheatMap cheats;
    extern bool in_game;
    extern bool showing_game;

    class TimeJob {
    public:
        bool canErase = true, priority = false;
        float remeaningTime = 0;
        std::function<void(TimeJob&)> onEnd = [](TimeJob&){};
        size_t* ptr = nullptr; // if not nullptr automatically deleted a the end!!
    };

    class Scene{
		using TextPtr = Text*;
		using BtnPtr = TextButton*;
		using TextButtonGroupPtr = TextButtonGroup*;
		using ScrTextButtonGroupPtr = ScrollableTextButtonGroup*;
		using TextGroupPtr = TextGroup*;
		using SubjectPtr = Subject*;
		using GroupPtr = Group*;
		using LifeViewerPtr = LifeViewer*;
		using BossLifeViewerPtr = BossLifeViewer*;
		using OptionsGroupPtr  = OptionsGroup*;
		using ClockPtr = Clock*;

    public:
        static Scene* curr_scene;
        float tpf;

        sf::Vector2i mousePosition;
        sf::RenderWindow window;
        std::vector<SubjectPtr> subjects;
        std::function<void(void)> onEscape = [](){};

        Scene();

        void startScene();

        void addSubject(SubjectPtr subj);
        void removeSubject(SubjectPtr subj);
        void safeRemoveSubject(SubjectPtr subj);
        void clearMemory();

        void addTimeJob(TimeJob& job);
        void clearTimeJobs();

        void showSplashScreen();
        void showStartScreen();

        void showGameScreen(size_t diff);
		void showSelectDifficultyScreen();
		void showEndScreen(Level& level, bool win);
		void showLevelScreen(size_t n, std::string& name);
		void showScoreScreen();
		void showStatsScreen();

        void showHUD();
		void showOptionsScreen();

		void removeOptionsScreen();
        void removeLevelScreen();
        void removeHUD();
        void removeScoreScreen();

        void togglePauseGame();
        void pause();
        void leaveGame();

        void updateDifficulties();
        void updateOptions();

        void toggleFullScreen();
        void recreate();

        TextPtr 				difficultyText, versionText, pauseText, levelText,
								bossText, bossNameText, pressSpaceText,
								ywText, scoreScreenText, fps, optionsText, statsText;

		TextButtonGroupPtr 		startMenu, pauseButtons, ywButtons,
								chooseDifficultyButtons;

        ScrTextButtonGroupPtr 	scores;

        TextGroupPtr 			ywStats, pointsLevelTime, statsGroup;

        SubjectPtr 				blackScreen, space, title, clearScreen;

        GroupPtr 				bossGroup, pointsGroup;

        OptionsGroupPtr         optionsMenu;

        LifeViewerPtr			viewer;

		BossLifeViewerPtr 		bossViewer;

		ClockPtr 				watch;
    private:
        sf::Clock clock;
        std::vector<SubjectPtr> 		toRemove;
        std::vector<SubjectPtr> 		toAdd;
        std::vector<TimeJob> 			jobsToAdd;
        std::vector<TimeJob> 			jobs;
        bool toClear = false, toClearJobs = false;

        void update(float tpf);
        void paint(SubjectPtr subj, sf::Vector2u dim, const float scaleX, const float scaleY);

        void fireKeyPressed(sf::Event& event);
        void fireKeyReleased(sf::Event& event);
        void fireMousePressed(sf::Event& event);
        void fireMouseReleased(sf::Event& event);
        void fireWheelMoved(sf::Event& event);
        void fireMouseMoved(sf::Event& event);
    };
}
#endif // SCENE_HPP
