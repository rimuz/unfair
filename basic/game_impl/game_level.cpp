#include "basic/game.hpp"
#include "io/save.hpp"
#include "io/data.hpp"

#include <iostream>

namespace ue{
	void Game::dmg(){
        if(!stopped){
            if(invicibility <= 0.f){
                invicibility = 2.f;
                if(!currLevel.inf_lifes){
                    currLevel.lifes--;
                }
            }

            if(currLevel.lifes == 0){
                SCENE.showEndScreen(currLevel, false);
                showingYw = true;
                stopped = true;
            }
        }
    }

    void Game::dmgBoss(){
        if(!stopped){
            currLevel.boss->health -= std::get<leveldefs::DMG>(leveldefs::levels[currLevel.diff]);
            currLevel.points += std::get<leveldefs::POINTS_PER_DMG>(leveldefs::levels[currLevel.diff]);
            currLevel.boss->damaged = BOSS_DAMAGED_RED_TIME;
            currLevel.boss->collidable.subject->sprite->setColor(sf::Color::Red);
            if(currLevel.boss->health <= 0.f){
                nextLevel();
            }
        }
    }

    void Game::bomb_start(){
        if(!stopped && bomb.getScale().x > MAX_BOMB_SIZE && currLevel.bombs > 0){
            currLevel.points -= std::get<leveldefs::POINTS_PER_BOMB>(leveldefs::levels[currLevel.diff]);

            bomb.setScale(1.f, 1.f);
            if(!currLevel.inf_bombs){
                currLevel.bombs --;
            }
            for(size_t i = 0; i < collidables.size(); ){
                Collidable& coll = collidables[i];
                if(coll.object->isBullet && !coll.object->dmgBoss){
                    collidables.erase(collidables.begin() + i);
                } else {
                    i++;
                }
            }
        }
    }

    void Game::nextLevel(){
        currLevel.points += std::get<leveldefs::POINTS_PER_BOSS_DEATH>(leveldefs::levels[currLevel.diff]);
        if(currLevel.n != 4){
            Level level = genLevel(currLevel.n+1, currLevel.diff,
                    currLevel.lifes + std::get<leveldefs::HEARTS_PRIZE>(leveldefs::levels[currLevel.diff]),
                    currLevel.bombs + std::get<leveldefs::BOMBS_PRIZE>(leveldefs::levels[currLevel.diff]),
                    currLevel.points
            );
            loadLevel(level);
        } else {
            SCENE.showEndScreen(currLevel, true);
            save::score.emplace_back(currLevel.diff, currLevel.points, static_cast<uint32_t>(GAME.ellapsed.asSeconds()));
            SCENE.updateDifficulties();
            showingYw = true;
            stopped = true;
        }
    }

    void Game::loadLevel(Level& level){
        if(currLevel.boss){
            removeCollidable(currLevel.boss->collidable);
            delete currLevel.boss;
            currLevel.boss = nullptr;
        }
        currLevel = level;
        currLevel.boss->collidable.subject->location = sf::Vector2f(1000.f, 500.f);

        SCENE.clearTimeJobs();
        clearMemory();

        sf::FloatRect rect1(-100.f, -100.f, 2200.f, 100.f);
        sf::FloatRect rect2(2000.f, 0.f, 100.f, 2000.f);
        sf::FloatRect rect3(-100.f, 2000.f, 2200.f, 100.f);
        sf::FloatRect rect4(-100.f, 0.f, 100.f, 2000.f);

        Collidable wall1 = newCollidable(rect1, new Subject);
        Collidable wall2 = newCollidable(rect2, new Subject);
        Collidable wall3 = newCollidable(rect3, new Subject);
        Collidable wall4 = newCollidable(rect4, new Subject);

        wall1.object->blocks = true;
        wall2.object->blocks = true;
        wall3.object->blocks = true;
        wall4.object->blocks = true;

        TimeJob job;
        job.canErase = false;
        job.remeaningTime = 1.f;
        job.onEnd = [this](TimeJob& job){
            currLevel.points -= std::get<leveldefs::POINTS_PER_SECOND>(leveldefs::levels[currLevel.diff]);
            job.remeaningTime = 1.f;
        };
        SCENE.addTimeJob(job);

        addCollidable(wall1);
        addCollidable(wall2);
        addCollidable(wall3);
        addCollidable(wall4);

        addCollidable(player);
        addCollidable(currLevel.boss->collidable);

        assets::colorBG(currLevel.n);

        currLevel.boss->collidable.subject->sprite->setColor(sf::Color::White);

        SCENE.showLevelScreen(currLevel.n, currLevel.boss->name);
        showingLevel = true; // better after showLevelScreen();
    }
}
