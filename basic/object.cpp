#include "object.hpp"
#include "engine/scene.hpp"
#include "io/data.hpp"
#include "basic/game.hpp"
#include "utils/gamedefs.hpp"

#include <cmath>
#include <iostream>

namespace ue{
    void Movement::update() noexcept{
        if(enabled){
            float wv = wv0 + (wa * t);
            float cv = cv0 + (ca * t);
            float v = v0 + (a * t);
            float theta;

            angle += wv * TPF;

            if (angle >= 360){
                angle -= 360;
            } else if (angle < 0){
                angle += 360;
            }

            theta = ANGLE_TO_THETA(angle);

            sf::Vector2f newPos (
                origin.x + (radius * std::cos(theta)),
                origin.y + (radius * std::sin(theta))
            );

            _totDirection += absoluteDirection * v * TPF;

            radius += cv * TPF;
            direction = (newPos - subject->location + _totDirection) / TPF;
        }
    }

    void Collidable::clear(){
        if(canClear){
            delete subject;
            delete object;
            if(movement){
                delete movement;
            }
        }
    }

    bool Collidable::operator==(Collidable& other){
        return object == other.object && subject == other.subject && movement == other.movement;
    }

    bool Collidable::operator==(const Collidable& other){
        return object == other.object && subject == other.subject && movement == other.movement;
    }

    Collidable newBullet(sf::Vector2f& location, char bulletType, Movement* mov){
        Subject* subj = new Subject;
        switch(bulletType){
            case BulletType::RED:{
                subj->sprite = &assets::red_bullet;
                break;
            }

            case BulletType::GREEN:{
                subj->sprite = &assets::green_bullet;
                break;
            }

            case BulletType::BLUE:{
                subj->sprite = &assets::blue_bullet;
                break;
            }

            case BulletType::WHITE:{
                subj->sprite = &assets::white_bullet;
                break;
            }
        }
        subj->location.x = location.x;
        subj->location.y = location.y;
        subj->size.x = BULLET_RADIUS*2;
        subj->size.y = BULLET_RADIUS*2;
        sf::FloatRect rect(-HITBOX_BULLET_RADIUS, -HITBOX_BULLET_RADIUS, HITBOX_BULLET_RADIUS*2, HITBOX_BULLET_RADIUS*2);

        Collidable coll = newCollidable(rect, subj, mov);
        coll.object->isBullet = true;
        coll.object->dmgBoss = bulletType == BulletType::WHITE;

        return coll;
    }

    Collidable newCollidable(sf::FloatRect& rect, Subject* subj, Movement* mov){
        Object* obj = new Object;
        obj->hitbox = rect;

        obj->n_points = 4;

        obj->points = new sf::Vector2f[4];

        obj->points[0].x = rect.left;
        obj->points[0].y = rect.top;

        obj->points[1].x = rect.left + rect.width;
        obj->points[1].y = rect.top;

        obj->points[2].x = rect.left;
        obj->points[2].y = rect.top + rect.height;

        obj->points[3].x = rect.left + rect.width;
        obj->points[3].y = rect.top + rect.height;

        Collidable coll;
        coll.object = obj;
        coll.subject = subj;
        coll.movement = mov;
        coll.movement->subject = coll.subject;

        return coll;
    }

    sf::Vector2f normalize(sf::Vector2f& vec, float scale){
        float length = std::sqrt((vec.x * vec.x) + (vec.y * vec.y));
        return sf::Vector2f(vec.x / length * scale, vec.y / length *scale);
    }
}
