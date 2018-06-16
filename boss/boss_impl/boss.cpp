#include "boss/boss.hpp"
#include "utils/gamedefs.hpp"

#include <time.h>
#include <stdlib.h>

#include <iostream>

#define BOSS(i, clazz)                              \
    case i: {                                       \
        return new clazz;                           \
    }

#include "boss/bosses/Sun.hpp"
#include "boss/bosses/DyinSun.hpp"
#include "boss/bosses/AngrySun.hpp"
#include "boss/bosses/Saturn.hpp"
#include "boss/bosses/Earth.hpp"

#define BOSS_ARRAY_SIZE

constexpr size_t bosses_n[] {
    #include "boss/lists/lvl1.inc"
    ,
    #include "boss/lists/lvl2.inc"
    ,
    #include "boss/lists/lvl3.inc"
    ,
    #include "boss/lists/lvl4.inc"
    ,
    #include "boss/lists/lvl5.inc"
};

#undef BOSS_ARRAY_SIZE

namespace ue{
    Boss::Boss(){}
    void Boss::update(){}
    Boss::~Boss(){}

    Boss* newBoss(size_t n){
        static unsigned int seed = time(nullptr);
        srand(seed++);
        size_t gen = rand() % bosses_n[n];

        switch(n){
            case 0:{
                switch(gen){
                    #include "boss/lists/lvl1.inc"
                }
                break;
            }

            case 1:{
                switch(gen){
                    #include "boss/lists/lvl2.inc"
                }
                break;
            }

            case 2:{
                switch(gen){
                    #include "boss/lists/lvl3.inc"
                }
                break;
            }

            case 3:{
                switch(gen){
                    #include "boss/lists/lvl4.inc"
                }
                break;
            }

            case 4:{
                switch(gen){
                    #include "boss/lists/lvl5.inc"
                }
                break;
            }
        }

        return nullptr;
    }


	Level genLevel(size_t n, size_t difficulty, size_t lifes, size_t bombs, int64_t points){
	    if(lifes > MAX_PLAYER_HEALTH){
            lifes = MAX_PLAYER_HEALTH;
	    }
	    if(bombs > MAX_PLAYER_HEALTH){
            bombs = MAX_PLAYER_HEALTH;
	    }

		Level level;
		level.n = n;
		level.diff = difficulty;
		level.lifes = lifes;
		level.bombs = bombs;
		level.boss = newBoss(n);
		level.boss->collidable.object->isBoss = true;
		level.points = points;

		reset_args();

		return level;
	}
}
