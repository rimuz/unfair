#ifndef BOSS_BOSSES_EARTH_H
#define BOSS_BOSSES_EARTH_H

#include "boss/boss.hpp"
#include "io/data.hpp"
#include "engine/scene.hpp"
#include "boss/bullet_patterns.hpp"

#include <limits>

namespace ue{
    class Earth : public Boss{
    public:
        Earth();
        void update() override;
        float           t = 0.f, t2 = 0.f, t3 = 0.f;
        bool            init = true;
    };

    Earth::Earth(){
        name = "The Earth";
        Subject* sub = new Subject;
        sub->sprite = &assets::bosses[5];
        sub->size = { 400.f, 400.f };
        CIRCULAR_HITBOX(400.f, 400.f);
        collidable = newCollidable(rect, sub);

    }

    void Earth::update(){
        t += TPF;
        t2 += TPF;
        t3 += TPF;

        if(t > 0.5f - (GAME.currLevel.diff * 0.02f)){
            static float a = 0.f;

            reset_args();
            bargs.type = BulletType::BLUE;
            bargs.pos = &collidable.subject->location;
            bargs.v = 200.f;
            bargs.maxt = 15.f;
            pargs.n_items = 10 + 2 * GAME.currLevel.diff;
            bargs.angle = a += 360.f / (static_cast<float>(pargs.n_items) * 2.f);
            pargs.iangle = 360.f / static_cast<float>(pargs.n_items);
            bullets::throwPattern();

            if(a > 360.f){
                a = 0.f;
            }

            t = 0.f;
        }

        if(t2 > 0.5f - (GAME.currLevel.diff * 0.02f)){
            static float a = 0.f;

            reset_args();
            bargs.type = BulletType::GREEN;
            bargs.pos = &collidable.subject->location;
            bargs.v = 400.f;
            bargs.maxt = 15.f;
            pargs.n_items = 40 + 5 * GAME.currLevel.diff;
            bargs.angle = a += 360.f / (static_cast<float>(pargs.n_items) * 2.f);
            pargs.iangle = 360.f / static_cast<float>(pargs.n_items);
            bullets::throwPattern();

            if(a > 360.f){
                a = 0.f;
            }

            t2 = 0.f;
        }

        if(t3 > 3.f - (GAME.currLevel.diff * 0.1f)){
            sf::Vector2f diff = GAME.player.subject->location - collidable.subject->location;
            float angle = THETA_TO_ANGLE(std::atan2(diff.y, diff.x));

            reset_args();
            bargs.type = BulletType::RED;
            bargs.pos = &collidable.subject->location;
            bargs.v = 900.f;
            bargs.maxt = 15.f;
            pargs.n_items = 5;
            bargs.angle = angle - 6.f;
            pargs.iangle = 3.f;
            bullets::throwPattern();

            t3 = 0.f;
        }
    }
}

#endif // BOSS_BOSSES_EARTH_H
