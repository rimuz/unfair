#ifndef         BOSS_HPP
#define         BOSS_HPP

#include "basic/object.hpp"

#include <vector>

namespace ue{
    class Boss{
    public:
        Boss();

        Boss(Boss& rhs);
        Boss(Boss&& rhs);

        Boss& operator=(Boss& rhs) = delete;
        Boss& operator=(Boss&& rhs) = delete;

        Collidable collidable;
        std::vector<Collidable> bullets;
        std::string name;
        float maxHealth = 100.f;
        float health = 100.f;

        float damaged = 0.f;

        virtual void update();
        virtual ~Boss();
    };

    struct Level {
        Boss* boss = nullptr;
        size_t diff = 0, lifes = 0, bombs = 0, n = 0;
        bool inf_lifes = false, inf_bombs = false;
        int64_t points = 0;
    };

    Level genLevel(size_t n, size_t difficulty, size_t lifes, size_t bombs, int64_t points);
}

#endif // BOSS_HPP
