#ifndef BOSS_BULLET_PATTERS_HPP
#define BOSS_BULLET_PATTERS_HPP

#include "boss.hpp"

namespace ue{
    struct bargs_t{
        // where bullets are stored
        std::vector<Collidable>*            bullets = nullptr;
        // the origin of all bullets (or bullets' circles)
        sf::Vector2f*                       pos = nullptr;
        /*
         * angle -> the direction (0-359°)
         * v -> the speed
         * a -> the acceleration
         * maxv -> max speed (when reached, bullet will be deleted)
         * if maxv == 0, then bullet will live forever
        */
        float                               angle = 0.f, v = 0.f, a = 0.f;
        // the color of the bullets
        char                                type = 0;

        /*
         *  radius -> the radius of the circle
         *  ca -> radius growing speed
         *  cv -> radius growing acceleration
         *  cangle -> start angle for circles/spirals
        */
        float radius = 0.f, ca = 0.f, cv = 0.f, cangle = 0.f;
        /*
         * wv -> circle walking speed
         * wa -> circle walking acceleration
         * maxt -> bullets' life time
        */
        float wv = 0.f, wa = 0.f, maxt = 0.f;
    };



    struct pargs_t {
        /*
         * items are lines or bullets
         * if n_bullets = X * n_items,
         * then lines will be generated.
        */
        size_t n_bullets = 0, n_items = 0;
        /*
         * tbi -> time between items
         * tbb -> time between bullets in same item
         * iangle -> incrementing angle for every item
         * icangle -> incrementing cangle for every item
        */
        float tbi = 0.f, tbb = 0.f, iangle = 0.f, icangle = 0.f;
    };

    extern bargs_t bargs;
    extern pargs_t pargs;

    static const bargs_t null_bargs = bargs_t();
    static const pargs_t null_pargs = pargs_t(); // both empty

    inline void reset_args() noexcept{
        bargs = null_bargs;
        pargs = null_pargs;
    };

	namespace bullets{
        void throwBullet(const bargs_t& = bargs);
        void throwPattern(const bargs_t& = bargs, const pargs_t& = pargs);
	}
}

#endif // BOSS_BULLET_PATTERS_HPP
