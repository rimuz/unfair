#ifndef         GAME_HPP
#define         GAME_HPP

#include <vector>
#include "engine/scene.hpp"
#include "subject.hpp"
#include "object.hpp"
#include "boss/boss.hpp"

#define GAME (*ue::Game::curr_game)

namespace ue{
    class Game;
}

namespace ue{
    class LifeViewer : public Subject{
    public:
        LifeViewer();
        void paintOn(sf::RenderTarget& target);
    };

    class BossLifeViewer : public Subject{
    public:
        BossLifeViewer();
        sf::RectangleShape innerShape, borderShape;
        float life = 0.f; // life percent!!!
        void paintOn(sf::RenderTarget& target);
    };


    class Game : public Subject{
    public:
        static Game* curr_game;

        Game(size_t diff);

        void paintOn(sf::RenderTarget& window);

        void keyPressed(sf::Event& event);
        void keyReleased(sf::Event& event);
        void mousePressed(sf::Event& event);
        void mouseReleased(sf::Event& event);

        void clearMemory();
        void addCollidable(Collidable& coll);
        void removeCollidable(Collidable& coll);

        void move(Collidable& coll);

        void dmg();
        void dmgBoss();
        void bomb_start();

        void nextLevel();
        void loadLevel(Level& level);

        void resetMovements();

        void checkCollisions(Collidable& coll);

        sf::CircleShape bomb;
        Collidable player;
        std::vector<Collidable> collidables;
        std::vector<Collidable> toRemove;
        std::vector<Collidable> toAdd;

        Level currLevel;
        unsigned health = 0, armor = 0;
        bool toClear = false, paused = false, stopped = false, showingLevel = false, showingYw = false;
        float invicibility = 0.f;
        LifeViewer* viewer;
        BossLifeViewer* bossViewer;
        bool up = false, left = false, down = false, right = false, shooting = false;

        sf::Time ellapsed;
        sf::Clock gameClock;
        bool matters = false;
    };
}

#endif // GAME_HPP
