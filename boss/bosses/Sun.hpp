#ifndef BOSS_BOSSES_SUN_H
#define BOSS_BOSSES_SUN_H

#include "boss/boss.hpp"
#include "io/data.hpp"
#include "engine/scene.hpp"
#include "boss/bullet_patterns.hpp"

namespace ue{
    class Sun : public Boss{
    public:
        Sun();
        void update() override;
        float           t = 0.f,
                        t2 = 0.f,
                        t3 = 0.f;
    };

    Sun::Sun(){
        name = "The Sun";
        Subject* sub = new Subject;
        sub->sprite = &assets::bosses[0];
        sub->size = { 400.f, 400.f };
        CIRCULAR_HITBOX(400.f, 400.f);
        collidable = newCollidable(rect, sub);
    }

    void Sun::update(){
        t += TPF;
        t2 += TPF;
        t3 += TPF;

        if(t > (0.5f - (GAME.currLevel.diff * 0.04f))){
            static float a = 0.f;

            bargs.type = BulletType::RED;
            bargs.v = 250.f;
            bargs.a = 10.f;
            bargs.pos = &collidable.subject->location;
            pargs.n_items = 50;
            bargs.angle = a += 2.f;
            bargs.maxt = 15.f;
            pargs.iangle = 360.f / static_cast<float>(pargs.n_items);
            bullets::throwPattern();

            t = 0.f;
        }

        if(t2 > (3.f - GAME.currLevel.diff * 0.4f)){
            static float a = 0.f;

            reset_args();
            bargs.type = BulletType::BLUE;
            bargs.v = 500.f;
            bargs.a = -70.f;
            bargs.pos = &collidable.subject->location;
            pargs.n_items = 30 + 7 * GAME.currLevel.diff;
            bargs.angle = a += 5.f;
            bargs.maxt = 14.28;
            pargs.iangle = 360.f / static_cast<float>(pargs.n_items);
            bullets::throwPattern();

            t2 = 0.f;
        }

        if(t3 > (2.f - GAME.currLevel.diff * 0.1f)){
            static float a = 0.f;

            bargs.type = BulletType::GREEN;
            bargs.v = 500.f;
            bargs.a = 50.f;
            bargs.pos = &collidable.subject->location;
            pargs.n_items = 30 + 7 * GAME.currLevel.diff;
            bargs.angle = a += 3.5f;
            bargs.maxt = 10.f;
            pargs.iangle = 360.f / static_cast<float>(pargs.n_items);
            bullets::throwPattern();

            t3 = 0.f;
        }
    }
}

#endif // BOSS_BOSSES_SUN_H
