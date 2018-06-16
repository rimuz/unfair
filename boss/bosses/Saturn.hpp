#ifndef BOSS_BOSSES_SATURN_H
#define BOSS_BOSSES_SATURN_H

#include "boss/boss.hpp"
#include "io/data.hpp"
#include "engine/scene.hpp"
#include "boss/bullet_patterns.hpp"

#include <limits>

namespace ue{
    class Saturn : public Boss{
    public:
        Saturn();
        void update() override;
        float           t = 0.f, t2 = 0.f;
        bool            init = true;
    };

    Saturn::Saturn(){
        name = "Saturn";
        Subject* sub = new Subject;
        sub->sprite = &assets::bosses[10];
        sub->size = { 600.f, 300.f };
        CIRCULAR_HITBOX(600.f, 300.f);
        collidable = newCollidable(rect, sub);

    }

    void Saturn::update(){
        t += TPF;
        t2 += TPF;

        if(init){
            static float a = 0.f;

            for(size_t i = 0; i < 6; i++){
                reset_args();
                bargs.type = BulletType::RED;
                bargs.pos = &collidable.subject->location;
                bargs.v = 300.f;
                bargs.angle = (60.f * i) + (a += 3.f);
                bargs.maxt = 15.f;
                pargs.iangle = 5.f;
                pargs.tbi = .1f;
                pargs.n_items = std::numeric_limits<size_t>::max();
                bullets::throwPattern();
            }
            init = false;
        }

        if(t > 0.5f - (GAME.currLevel.diff * 0.02f)){
            static float a = 0.f;

            reset_args();
            bargs.type = BulletType::BLUE;
            bargs.pos = &collidable.subject->location;
            bargs.v = 200.f;
            bargs.maxt = 15.f;
            pargs.n_items = 55 + 5 * GAME.currLevel.diff;
            bargs.angle = a += 360.f / (static_cast<float>(pargs.n_items) * 2.f);
            pargs.iangle = 360.f / static_cast<float>(pargs.n_items);
            bullets::throwPattern();

            t = 0.f;
        }

        if(t2 > 2.f - (GAME.currLevel.diff * 0.1f)){
            static float a = 0.f;

            reset_args();
            bargs.type = BulletType::GREEN;
            bargs.pos = &collidable.subject->location;
            bargs.v = 600.f;
            bargs.maxt = 15.f;
            pargs.n_items = 45 + 5 * GAME.currLevel.diff;
            bargs.angle = a += 360.f / (static_cast<float>(pargs.n_items) * 2.f);
            pargs.iangle = 360.f / static_cast<float>(pargs.n_items);
            bullets::throwPattern();

            t2 = 0.f;
        }
    }
}

#endif // BOSS_BOSSES_SATURN_H
