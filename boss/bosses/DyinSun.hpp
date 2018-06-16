#ifndef BOSS_BOSSES_DYINSUN_H
#define BOSS_BOSSES_DYINSUN_H

#include "boss/boss.hpp"
#include "io/data.hpp"
#include "engine/scene.hpp"
#include "boss/bullet_patterns.hpp"

namespace ue{
    class DyinSun : public Boss{
    public:
        DyinSun();
        void update() override;
        float           t = -0.1f,
                        t2 = -0.4f,
                        t3 = -0.8f;
    };

    DyinSun::DyinSun(){
        name = "The dyin' Sun";
        Subject* sub = new Subject;
        sub->sprite = &assets::bosses[2];
        sub->size = { 400.f, 400.f };
        CIRCULAR_HITBOX(400.f, 400.f);
        collidable = newCollidable(rect, sub);
    }

    void DyinSun::update(){
        t += TPF;
        t2 += TPF;
        t3 += TPF;

        if(t > (0.4f - (GAME.currLevel.diff * 0.04f))){
            static float a = 90.f;

            reset_args();
            bargs.type = BulletType::BLUE;
            bargs.v = 500.f;
            bargs.maxt = 6.66f;
            bargs.pos = &collidable.subject->location;
            pargs.n_items = 20 + 5 * GAME.currLevel.diff;
            pargs.iangle = 360.f / static_cast<float>(pargs.n_items);
            bargs.angle = a += 25.f;
            bullets::throwPattern();
            t = 0.f;
        }


        if(t2 > (0.7f - (GAME.currLevel.diff * 0.04f))){
            static float a = 35.f;

            reset_args();
            bargs.type = BulletType::GREEN;
            bargs.v = 500.f;
            bargs.maxt = 6.66f;
            bargs.pos = &collidable.subject->location;
            pargs.n_items = 30 + 5 * GAME.currLevel.diff;
            pargs.iangle = 360.f / static_cast<float>(pargs.n_items);
            bargs.angle = a += 9.f;
            bullets::throwPattern();
            t2 = 0.f;
        }

        if(t3 > (0.5f - (GAME.currLevel.diff * 0.04f))){
            static float a = 12.f;

            reset_args();
            bargs.type = BulletType::RED;
            bargs.v = 500.f;
            bargs.maxt = 6.66f;
            bargs.pos = &collidable.subject->location;
            pargs.n_items = 50 + 5 * GAME.currLevel.diff;
            pargs.iangle = 360.f / static_cast<float>(pargs.n_items);
            bargs.angle = a += 3.f;
            bullets::throwPattern();
            t3 = 0.f;
        }
    }
}

#endif // BOSS_BOSSES_DYINSUN_H
