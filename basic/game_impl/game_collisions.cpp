#include "basic/game.hpp"
#include "utils/gamedefs.hpp"
#include "io/data.hpp"

#include <iostream>

namespace ue{
    void Game::move(Collidable& coll){
        if(coll.movement){
            coll.movement->update();

            sf::Vector2f direction = coll.movement->direction * TPF;

            coll.movement->t += TPF;
            if(coll.movement->maxt && coll.movement->t >= coll.movement->maxt){
                removeCollidable(coll);
            }

            if(coll.object->isPlayer){
                coll.subject->location.x += direction.x;
                coll.subject->location.y += direction.y;

                if(coll.subject->location.x > MAX_PLAYER_X){
                    coll.subject->location.x = MAX_PLAYER_X;
                } else if(coll.subject->location.x < MIN_PLAYER_X){
                    coll.subject->location.x = MIN_PLAYER_X;
                }

                if(coll.subject->location.y > MAX_PLAYER_Y){
                    coll.subject->location.y = MAX_PLAYER_Y;
                } else if(coll.subject->location.y < MIN_PLAYER_Y){
                    coll.subject->location.y = MIN_PLAYER_Y;
                }
                checkCollisions(coll);
            } else if(coll.object->isBoss){
                coll.subject->location.x += direction.x;
                coll.subject->location.y += direction.y;
                checkCollisions(coll);
            } else {
                coll.subject->location.x += direction.x;
                coll.subject->location.y += direction.y;
            }
        }
    }

    bool collides(Collidable& c1, Collidable& c2){
        for(size_t i = 0; i < c1.object->n_points; i++){
            sf::Vector2f point = c1.subject->location + c1.object->points[i];
            if(c2.object->hitbox.contains(point - c2.subject->location)){
                return true;
            }
        }

        for(size_t i = 0; i < c2.object->n_points; i++){
            sf::Vector2f point = c2.subject->location + c2.object->points[i];
            if(c1.object->hitbox.contains(point - c1.subject->location)){
                return true;
            }
        }

        return false;
    }

    void Game::checkCollisions(Collidable& coll){ // coll only can be player or boss!!
        for(size_t i = 0; i < collidables.size(); i++){
            Collidable& c = collidables[i];
            if(c == coll){
                continue;
            }

            if(collides(coll, c)){
                if(c.object->isBullet){
                    if(c.object->dmgBoss){
                        if(coll.object->isBoss){
                            removeCollidable(c);
                            dmgBoss();
                            break;
                        }
                    } else if(coll.object->isPlayer){
                        dmg();
                    }
                } else if(coll.object->isPlayer && c.object->isBoss){
                    dmg();
                } else if(coll.object->isBoss && c.object->isPlayer){
                    dmg();
                }
            }
        }
    }
}
