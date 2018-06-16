#ifndef BOSS_BOSSES_ANGRYSUN_H
#define BOSS_BOSSES_ANGRYSUN_H

#include "boss/boss.hpp"
#include "io/data.hpp"
#include "engine/scene.hpp"
#include "boss/bullet_patterns.hpp"

namespace ue{
    class AngrySun : public Boss{
    public:
        AngrySun();
        void update() override;
        float           t = -0.1f,
                        t2 = -0.4f,
                        t3 = -0.8f,
                        t4 = 0.f;
    };

    AngrySun::AngrySun(){
        name = "The angry Sun";
        Subject* sub = new Subject;
        sub->sprite = &assets::bosses[1];
        sub->size = { 400.f, 400.f };
        CIRCULAR_HITBOX(400.f, 400.f);
        collidable = newCollidable(rect, sub);
    }

    void AngrySun::update(){
        t += TPF;
        t2 += TPF;
        t3 += TPF;
        t4 += TPF;

        if(t > (2.f - (GAME.currLevel.diff * 0.2f))){
            static float a = 90.f;

            bargs.type = BulletType::BLUE;
            bargs.v = 800.f;
            bargs.a = -150.f;
            bargs.maxt = 10.66f;
            bargs.pos = &collidable.subject->location;
            pargs.n_items = 25 + 5 * GAME.currLevel.diff;
            pargs.iangle = 360.f / static_cast<float>(pargs.n_items);
            bargs.angle = a += 15.f;
            bullets::throwPattern();
            t = 0.f;
        }


        if(t2 > (2.f - (GAME.currLevel.diff * 0.2f))){
            static float a = 35.f;

            bargs.type = BulletType::BLUE;
            bargs.v = 700.f;
            bargs.a = -150.f;
            bargs.maxt = 9.33f;
            bargs.pos = &collidable.subject->location;
            pargs.n_items = 25 + 5 * GAME.currLevel.diff;
            pargs.iangle = 360.f / static_cast<float>(pargs.n_items);
            bargs.angle = a += 7.f;
            bullets::throwPattern();
            t2 = 0.f;
        }

        if(t3 > (2.f - (GAME.currLevel.diff * 0.2f))){
            static float a = 12.f;

            bargs.type = BulletType::BLUE;
            bargs.v = 800.f;
            bargs.a = -200.f;
            bargs.maxt = 8.f;
            bargs.pos = &collidable.subject->location;
            pargs.n_items = 25 + 5 * GAME.currLevel.diff;
            pargs.iangle = 360.f / static_cast<float>(pargs.n_items);
            bargs.angle = a += 2.f;
            bullets::throwPattern();
            t3 = 0.f;
        }

        if(t4 > (0.5f - (GAME.currLevel.diff * 0.04f))){
            static float a = 0.f;

            reset_args();
            bargs.type = BulletType::RED;
            bargs.v = 250.f;
            bargs.a = 10.f;
            bargs.pos = &collidable.subject->location;
            pargs.n_items = 50;
            bargs.angle = a += 2.f;
            pargs.iangle = 360.f / static_cast<float>(pargs.n_items);
            bullets::throwPattern();

            t4 = 0.f;
        }
    }
}

#endif // BOSS_BOSSES_ANGRYSUN_H
