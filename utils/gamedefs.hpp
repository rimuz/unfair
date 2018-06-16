#ifndef GAMEDEFS_HPP
#define GAMEDEFS_HPP

#define PIECE_OF_CAKE       0
#define NOOB                1
#define HARD                2
#define PRO                 3
#define HELL                4

#define SPLASH_TIME 4.5f

#define BULLET_RADIUS 15.f
#define BULLET_LIFE_LENGTH 30000.f
#define BULLET_ORIGIN 100

#define MAX_PLAYER_HEALTH 15
#define PLAYER_SPEED 300.f
#define PLAYER_SIZE 35.f
#define HITBOX_BULLET_RADIUS (BULLET_RADIUS*0.7f)
#define PLAYER_HITBOX_SIZE (PLAYER_SIZE*0.7f)
#define PLAYER_BULLET_SPEED 750.f

#define MAX_PLAYER_X (2000.f - (PLAYER_SIZE/2.f))
#define MIN_PLAYER_X (0.f + (PLAYER_SIZE/2.f))
#define MAX_PLAYER_Y MAX_PLAYER_X
#define MIN_PLAYER_Y MIN_PLAYER_X

#define CIRCULAR_HITBOX(x, y) sf::FloatRect rect(-x/2.f*0.7f, -y/2.f*0.7f, x*0.7f, y*0.7f);

#define BOSS_DAMAGED_RED_TIME 0.33f

#define PI 3.141592653589793238L

#define MAX_BOMB_SIZE 2000.f
#define BOMB_SPEED 2000.f

#define SCR_WIDTH 1920.f
#define SCR_HEIGHT 1080.f

#define ANGLE_TO_THETA(angle) ((angle) * 2.f / 360.f * PI)
#define THETA_TO_ANGLE(theta) ((theta) / 2.f * 360.f / PI)

#define GEN_SUBJECT(name, sprite_ptr) ue::Subject* name = new ue::Subject; name->sprite = sprite_ptr;
#define B_TO_IS(b) ((b) ? "YES" : "NO")

namespace ue{
    template <typename T, std::size_t N>
    constexpr std::size_t arr_size(T (&) [N]) noexcept{
        return N;
    }

}

#endif // GAMEDEFS_HPP
