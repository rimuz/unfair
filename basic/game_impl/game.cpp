#include "basic/game.hpp"
#include "io/data.hpp"
#include "boss/boss.hpp"
#include "basic/object.hpp"
#include "io/save.hpp"
#include "utils/gamedefs.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <cmath>

namespace ue{
    Game* Game::curr_game = nullptr;

    LifeViewer::LifeViewer(){
        size.x = MAX_PLAYER_HEALTH*30.f + 10.f;
        size.y = 70.f;
        sizeType = SubjectSize::KEEP_PROPORTIONS;
        locType = SubjectLocation::LEFT_TOP;

        sf::Vector2f origin(20.f, 20.f);
        sf::Vector2f hsize(36.f, 36.f); // 38 = 40 - 2 (borders)
    }

    void LifeViewer::paintOn(sf::RenderTarget& target){
        const sf::Vector2f scale {
            realSize.x / size.x,
            realSize.y / size.y
        };

        for(size_t i = 0; i < MAX_PLAYER_HEALTH; i++){
            if(GAME.currLevel.lifes > i){
                assets::heart.setPosition(20.f + i*30.f * scale.x, 15.f * scale.y);
                assets::heart.setScale(scale * 0.25f);
                target.draw(assets::heart);
            }
            if(GAME.currLevel.bombs > i){
                assets::bomb.setPosition(20.f + i*30.f * scale.x, 45.f * scale.y);
                assets::bomb.setScale(scale * 0.25f);
                target.draw(assets::bomb);
            }
        }
    }

    BossLifeViewer::BossLifeViewer(){
        size.x = 600.f;
        size.y = 60.f;
        sizeType = SubjectSize::KEEP_PROPORTIONS;
        locType = SubjectLocation::BOTTOM;

        borderShape.setSize(sf::Vector2f(590.f, 20.f));
        borderShape.setOutlineThickness(5.f);
        borderShape.setFillColor(sf::Color::Transparent);//sf::Color(49, 49, 50));
        borderShape.setOutlineColor(sf::Color::Black);
        borderShape.setPosition(5.f, 5.f);

        innerShape.setFillColor(sf::Color::Red);
        innerShape.setPosition(5.f, 5.f);
    }

    void BossLifeViewer::paintOn(sf::RenderTarget& target){
        innerShape.setSize(sf::Vector2f(life / 100.f * 590.f, 20.f));

        sf::Vector2f scale {
            realSize.x / size.x,
            realSize.y / size.y
        };
        sf::Vector2f pos(realLoc.x + 5.f * scale.x, realLoc.y + 5.f * scale.y);

        borderShape.setPosition(pos);
        borderShape.setScale(scale);

        innerShape.setPosition(pos);
        innerShape.setScale(scale);

        target.draw(borderShape);
        target.draw(innerShape);
    }

    Game::Game(size_t diff){
        curr_game = this;
        save::options.games++;

        size.x = 1280.f;
        size.y = 720.f;
        locType = SubjectLocation::CENTER;
        sizeType = SubjectSize::KEEP_PROPORTIONS;

        sf::FloatRect frect(-PLAYER_HITBOX_SIZE/2.f, -PLAYER_HITBOX_SIZE/2.f, PLAYER_HITBOX_SIZE, PLAYER_HITBOX_SIZE);

        // PLAYER TEXTURE

        Subject* playerSubject = new Subject();
        playerSubject->sprite = &assets::eye;
        playerSubject->size.x = PLAYER_SIZE;
        playerSubject->size.y = PLAYER_SIZE;

        player = newCollidable(frect, playerSubject);
        player.object->isPlayer = true;
        player.subject->location = sf::Vector2f(1000.f, 1500.f);
        player.canClear = false;

        bomb.setFillColor(sf::Color::Transparent);
        bomb.setOutlineColor(sf::Color(255, 255, 255, 100));
        bomb.setPointCount(50);
        bomb.setRadius(0.75f);
        bomb.setOutlineThickness(0.25f);
        bomb.setOrigin(0.75f, 0.75f);
        bomb.setScale(MAX_BOMB_SIZE+1.f, MAX_BOMB_SIZE+1.f);

        sf::Vector2u eyeSize = assets::teye.getSize();
        assets::eye.setOrigin(eyeSize.x/2.f, eyeSize.y/2.f);

        Level level = genLevel(0, diff,
                std::get<leveldefs::HEARTS>(leveldefs::levels[diff]),
                std::get<leveldefs::BOMBS>(leveldefs::levels[diff]), 0);
        loadLevel(level);
    }
}
