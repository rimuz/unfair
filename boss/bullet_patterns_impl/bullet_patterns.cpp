#include "boss/bullet_patterns.hpp"
#include "utils/gamedefs.hpp"
#include "basic/game.hpp"

#include <iostream>
#include <limits>

namespace ue{
    bargs_t bargs = bargs_t();
    pargs_t pargs = pargs_t();

	namespace bullets{
	    void throwBullet(const bargs_t& bargs){
            float theta = ANGLE_TO_THETA(bargs.angle);
            sf::Vector2f absoluteDirection = { std::cos(theta), std::sin(theta) };

            Movement* const mov = new Movement;
            mov->v0 = bargs.v;
            mov->a = bargs.a;
            mov->wv0 = bargs.wv;
            mov->wa = bargs.wa;
            mov->cv0 = bargs.cv;
            mov->ca = bargs.ca;
            mov->angle = bargs.cangle;
            mov->radius = bargs.radius;
            mov->origin = *bargs.pos;
            mov->maxt = bargs.maxt;
            mov->absoluteDirection = absoluteDirection;
            mov->enabled = true;

            theta = ANGLE_TO_THETA(bargs.cangle); // reused float theta

            sf::Vector2f pos;
            pos.x = bargs.pos->x + (bargs.radius * std::cos(theta));
            pos.y = bargs.pos->y + (bargs.radius * std::sin(theta));

            Collidable bullet = newBullet(*bargs.pos, bargs.type, mov);
            if(bargs.bullets){
                bargs.bullets->push_back(bullet);
            }
            GAME.addCollidable(bullet);
	    }

	    void throwPattern(const bargs_t& _bargs, const pargs_t& _pargs){
	        bargs_t bargs = _bargs;
	        pargs_t pargs = _pargs;

            if(!pargs.n_bullets){
                pargs.n_bullets = pargs.n_items;
            }

	        if(pargs.tbi){
                TimeJob job;
                if(pargs.n_items){
                    job.ptr = new size_t(0);
                }
                job.remeaningTime = pargs.tbi;
                job.onEnd = [bargs, pargs](TimeJob& job) mutable{
                    job.remeaningTime = pargs.tbi;

                    if(pargs.n_bullets != pargs.n_items){
                        TimeJob inner;
                        float tbb = pargs.tbb;
                        size_t bullets_each_item = pargs.n_bullets / pargs.n_items;

                        if(bullets_each_item != std::numeric_limits<size_t>::max()){
                            inner.ptr = new size_t(0);
                        }

                        inner.remeaningTime = tbb;
                        inner.onEnd = [bargs, tbb, bullets_each_item](TimeJob& j){
                            j.remeaningTime = tbb;
                            throwBullet(bargs);
                            if(j.ptr){
                                (*j.ptr)++;
                                if(*j.ptr >= bullets_each_item){
                                    j.canErase = true;
                                }
                            }
                        };
                        inner.canErase = false;

                        SCENE.addTimeJob(inner);
                    } else {
                        throwBullet(bargs);
                    }

                    bargs.angle += pargs.iangle;
                    bargs.cangle += pargs.icangle;

                    if(job.ptr){
                        (*job.ptr)++;
                        if(*job.ptr >= pargs.n_items){
                            job.canErase = true;
                        }
                    }
                };
                job.canErase = false;

                SCENE.addTimeJob(job);
	        } else {
                for(size_t i = 0; i < pargs.n_items; i++){
                    if(pargs.n_bullets != pargs.n_items){
                        TimeJob inner;
                        float tbb = pargs.tbb;
                        size_t bullets_each_item = pargs.n_bullets / pargs.n_items;

                        if(bullets_each_item != std::numeric_limits<size_t>::max()){
                            inner.ptr = new size_t(0);
                        }
                        inner.onEnd = [bargs, tbb, bullets_each_item](TimeJob& j){
                            j.remeaningTime = tbb;
                            throwBullet(bargs);
                            if(j.ptr){
                                (*j.ptr)++;
                                if(*j.ptr >= bullets_each_item){
                                    j.canErase = true;
                                }
                            }
                        };
                        inner.canErase = false;

                        SCENE.addTimeJob(inner);
                    } else {
                        throwBullet(bargs);
                    }

                    bargs.angle += pargs.iangle;
                    bargs.cangle += pargs.icangle;
                }
	        }
	    }
	}
}
