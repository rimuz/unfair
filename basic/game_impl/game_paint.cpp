#include "basic/game.hpp"
#include "io/data.hpp"
#include "io/save.hpp"

namespace ue{
    sf::View sceneView;
    sf::View gameView;

    void Game::paintOn(sf::RenderTarget& window){
        stopped = showingLevel || showingYw;
        in_game = !stopped;

        for(size_t i = 0; i < toRemove.size(); i++){
            collidables.erase(std::remove(collidables.begin(), collidables.end(), toRemove[i]), collidables.end());
            toRemove[i].clear();
        }
        toRemove.clear();

        if(toClear){
            toClear = false;
            for(size_t i = 0; i < collidables.size(); i++){
                collidables[i].clear();
            }
            collidables.clear();
        }

        for(size_t i = 0; i < toAdd.size(); i++){
            collidables.push_back(toAdd[i]);
        }
        toAdd.clear();

        if(!paused && !stopped){
            currLevel.boss->update();
            for(size_t i = 0; i < collidables.size(); i++){
                if(!collidables[i].object->isPlayer && !collidables[i].object->isBoss){
                    move(collidables[i]);
                }
            }
            move(player);
            if(currLevel.boss){
                move(currLevel.boss->collidable);
            }
        }

        sf::Vector2i mousePos = MOUSE_POS;
        const auto winX = static_cast<signed>(window.getSize().x);
        const auto winY = static_cast<signed>(window.getSize().y);

        if(mousePos.x < 0){
            mousePos.x = 0;
        } else if(mousePos.x > winX){
            mousePos.x = winX;
        }

        if(mousePos.y < 0){
            mousePos.y = 0;
        } else if(mousePos.y > winY){
            mousePos.y = winY;
        }

        sf::Vector2u winSize = window.getSize();
        const float scaleX = static_cast<float>(winSize.x) / 1920.f;
        const float scaleY = static_cast<float>(winSize.y) / 1080.f;
        const float scale = scaleX > scaleY ? scaleY : scaleX;
        gameView.setSize(winSize.x / scale, winSize.y / scale);

        sceneView = window.getView();
        window.setView(gameView);

        sf::Vector2f center(player.subject->location);
        sf::Vector2f ldiff;
        sf::Vector2f mapMousePos = window.mapPixelToCoords(mousePos);

        ldiff = center - mapMousePos;
        ldiff.x /= 3.5f;
        ldiff.y /= 3.5f;

        gameView.setCenter(center - ldiff);
        window.setView(gameView);

        sf::Vector2f diff = window.mapPixelToCoords(mousePos) - player.subject->location;
        float angle = THETA_TO_ANGLE(std::atan2(diff.y, diff.x));
        assets::eye.setRotation(angle);

        bool show = true;
        if(invicibility <= 2.0f && invicibility >= 1.7f){
            show = false;
        } else if(invicibility <= 1.4f && invicibility >= 1.1f){
            show = false;
        } else if(invicibility <= 0.8f && invicibility >= 0.5f){
            show = false;
        }
        invicibility -= TPF;

        player.subject->locType = show ? SubjectLocation::CUSTOM : SubjectLocation::INVISIBLE;

        window.draw(assets::square_bg);
        window.draw(assets::level_bg_txt);


        if(bomb.getScale().x <= MAX_BOMB_SIZE ){
            if(!stopped && !paused){
                float add = TPF * BOMB_SPEED;
                sf::Vector2f scale = bomb.getScale();
                scale.x += add;
                scale.y += add;
                bomb.setScale(scale);
            }

            bomb.setPosition(player.subject->location);
            window.draw(bomb);
        }

        if(currLevel.boss->damaged > 0.f){
            currLevel.boss->damaged -= TPF;
            if(currLevel.boss->damaged < 0.f){
                currLevel.boss->collidable.subject->sprite->setColor(sf::Color::White);
            }
        }

        for(size_t i = 0; i < collidables.size(); i++){
            Subject* sub = collidables[i].subject;

            if(sub->locType == SubjectLocation::INVISIBLE){
                continue;
            }

            sub->realSize.x = sub->size.x;
            sub->realSize.y = sub->size.y;
            sub->realLoc.x = sub->location.x;
            sub->realLoc.y = sub->location.y;

            sub->paintOn(window);
        }

        static float delay = 0.f;
        static float autoaimDelay = 0.f;
        delay -= TPF;
        autoaimDelay -= TPF;
        if(!paused && !stopped){
            if(save::options.autoaim){
                if(currLevel.boss && autoaimDelay < 0.f){
                    sf::Vector2f dir = currLevel.boss->collidable.subject->location - player.subject->location;
                    Collidable coll = newBullet(player.subject->location, BulletType::WHITE);
                    coll.movement->direction = normalize(dir, PLAYER_BULLET_SPEED);
                    addCollidable(coll);
                    currLevel.points -= std::get<leveldefs::POINTS_PER_SHOOT>(leveldefs::levels[currLevel.diff]);
                    autoaimDelay = 0.75f;
                }
            } else if(shooting && delay < 0.f){
                Collidable coll = newBullet(player.subject->location, BulletType::WHITE);
                coll.movement->direction = normalize(diff, PLAYER_BULLET_SPEED);
                addCollidable(coll);
                currLevel.points -= std::get<leveldefs::POINTS_PER_SHOOT>(leveldefs::levels[currLevel.diff]);
                delay = 0.35f;
            }
        }

        if(stopped || paused){
            if(matters){
                ellapsed += gameClock.restart();
                matters = false;
            }
        } else {
            if(matters){
                ellapsed += gameClock.restart();
            } else {
                gameClock.restart(); // no saving because it's the paused time
                matters = true;
            }
        }

        bossViewer->life = currLevel.boss->health / currLevel.boss->maxHealth * 100.f;
        SCENE.pointsLevelTime->texts[0]->text.setString(std::string("TIME: ") + showTime(ellapsed));
        SCENE.pointsLevelTime->texts[0]->pack();
        SCENE.pointsLevelTime->texts[1]->text.setString(std::string("MODE: ") + SCENE.chooseDifficultyButtons->buttons[currLevel.diff]->text.getString());
        SCENE.pointsLevelTime->texts[1]->pack();
        SCENE.pointsLevelTime->texts[2]->text.setString(std::string("LEVEL: ") + std::to_string(currLevel.n + 1));
        SCENE.pointsLevelTime->texts[2]->pack();
        SCENE.pointsLevelTime->texts[3]->text.setString(std::string("SCORE: ") + std::to_string(currLevel.points));
        SCENE.pointsLevelTime->texts[3]->pack();
        SCENE.pointsLevelTime->packVertical();

        window.setView(sceneView);
    }

    void Game::addCollidable(Collidable& coll){
        toAdd.push_back(coll);
    }

    void Game::removeCollidable(Collidable& coll){
        toRemove.push_back(coll);
    }

    void Game::clearMemory(){
        toClear = true;
    }

    void Game::keyPressed(sf::Event& event){
        switch(event.key.code){
            case sf::Keyboard::W:
            case sf::Keyboard::Up:{
                if(!up){
                    up = true;
                    player.movement->direction.y -= PLAYER_SPEED;
                }
                break;
            }

            case sf::Keyboard::A:
            case sf::Keyboard::Left:{
                if(!left){
                    left = true;
                    player.movement->direction.x -= PLAYER_SPEED;
                }
                break;
            }

            case sf::Keyboard::S:
            case sf::Keyboard::Down:{
                if(!down){
                    down = true;
                    player.movement->direction.y += PLAYER_SPEED;
                }
                break;
            }

            case sf::Keyboard::D:
            case sf::Keyboard::Right:{
                if(!right){
                    right = true;
                    player.movement->direction.x += PLAYER_SPEED;
                }
                break;
            }

            case sf::Keyboard::Space:
            case sf::Keyboard::Q:{
                bomb_start();
                break;
            }

            case sf::Keyboard::Return:{
                if(showingLevel && !paused){
                    showingLevel = false;
                    SCENE.removeLevelScreen();
                    return;
                }
            }

            default:{
                break;
            }
        }
    }

    void Game::keyReleased(sf::Event& event){
        switch(event.key.code){
            case sf::Keyboard::W:
            case sf::Keyboard::Up:{
                if(up){
                    up = false;
                    player.movement->direction.y += PLAYER_SPEED;
                }
                break;
            }

            case sf::Keyboard::A:
            case sf::Keyboard::Left:{
                if(left){
                    left = false;
                    player.movement->direction.x += PLAYER_SPEED;
                }
                break;
            }

            case sf::Keyboard::S:
            case sf::Keyboard::Down:{
                if(down){
                    down = false;
                    player.movement->direction.y -= PLAYER_SPEED;
                }
                break;
            }

            case sf::Keyboard::D:
            case sf::Keyboard::Right:{
                if(right){
                    right = false;
                    player.movement->direction.x -= PLAYER_SPEED;
                }
                break;
            }

            default: {
                break;
            }
        }
    }


    void Game::resetMovements(){
        up = false;
        down = false;
        left = false;
        right = false;
        shooting = false;
    }

    void Game::mousePressed(sf::Event& event){
        if(event.mouseButton.button){
            bomb_start();
        } else {
            shooting = true;
        }
    }

    void Game::mouseReleased(sf::Event& event){
        if(!event.mouseButton.button){
            shooting = false;
        }
    }
}
