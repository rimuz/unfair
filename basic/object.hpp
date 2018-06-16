#ifndef         OBJECT_HPP
#define         OBJECT_HPP

#include <functional>
#include "subject.hpp"
#include "utils/gamedefs.hpp"

namespace ue{
    class Collidable;

    class Object {
    public:
        sf::FloatRect hitbox;

        sf::Vector2f* points = nullptr;
        size_t n_points = 0;

        bool blocks = false;
        bool isPlayer = false;
        bool isBoss = false;
        bool isBullet = false;
        bool dmgBoss = false; // true if damages the boss
    };

    // ottimizzare quiiii: si puo togliere tutti i derivati di movement e fare un solo movement che diventa translablespiralmovement
    class Movement{
    public:
        Subject*            subject = nullptr; // weak pointer (don't try to delete on this)!!!
        sf::Vector2f        direction, origin, absoluteDirection;
        bool enabled = false;
        float               v0 = 0.f,
                            a = 0.f,
                            wv0 = 0.f,
                            wa = 0.f,
                            cv0 = 0.f,
                            ca = 0.f,
                            radius = 0.f,
                            angle = 0.f,
                            t = 0.f,
                            maxt = 0.f;
        void update() noexcept;
    private:
        sf::Vector2f _totDirection;
    };


    class Collidable {
        public:
        Subject* subject = nullptr;
        Object* object = nullptr;
        Movement* movement = nullptr;
        bool canClear = true;

        void clear();

        bool operator==(Collidable& other);
        bool operator==(const Collidable& other);
    };

    namespace BulletType{
        enum bullet_type{
            RED = 0,
            GREEN,
            BLUE,
            WHITE, // player's
        };
    }

    Collidable newBullet(sf::Vector2f& location, char bulletType = BulletType::WHITE, Movement* mov = new Movement);
    Collidable newCollidable(sf::FloatRect& rect, Subject* subj, Movement* mov = new Movement);
    sf::Vector2f normalize(sf::Vector2f& vec, float scale = 1);
}

#endif // OBJECT_HPP
