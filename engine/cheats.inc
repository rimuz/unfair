// DON'T INCLUDE THIS OUT OF FILE SCENE.CPP

cheatMap cheats  = {
        {
            "nxtlvl",

            [](Scene&, Game& game, Level&){
                if(game.showingLevel){
                    return;
                }

                game.nextLevel();
            }

        },

        {
            "prvlvl",

            [](Scene&, Game& game, Level& currLevel){
                if(game.showingLevel){
                    return;
                }

                if(game.showingLevel){
                    return;
                }

                if(currLevel.n == 0){
                    return;
                } else if(currLevel.n == 1){
                    Level level = genLevel(0, currLevel.diff,
                    std::get<leveldefs::HEARTS>(leveldefs::levels[currLevel.diff]),
                    std::get<leveldefs::BOMBS>(leveldefs::levels[currLevel.diff]), 0);

                    game.loadLevel(level);
                    return;
                }

                currLevel.n -= 2;
                game.nextLevel();
            }

        },

        {
            "imrich",

            [](Scene&, Game& game, Level& currLevel){
                if(game.showingLevel){
                    return;
                }

                currLevel.points = 99999;
            }

        },

        {
            "hesoyam",

            [](Scene&, Game& game, Level& currLevel){
                if(game.showingLevel){
                    return;
                }

                currLevel.lifes = MAX_PLAYER_HEALTH;
                currLevel.bombs = MAX_PLAYER_HEALTH;
            }

        },

        {
            "lover",

            [](Scene&, Game& game, Level& currLevel){
                if(game.showingLevel){
                    return;
                }

                currLevel.lifes = MAX_PLAYER_HEALTH;
                currLevel.inf_lifes = true;
            }

        },

        {
            "bomberman",

            [](Scene&, Game& game, Level& currLevel){
                if(game.showingLevel){
                    return;
                }

                currLevel.bombs = MAX_PLAYER_HEALTH;
                currLevel.inf_bombs = true;
            }

        },

        {
            "godmode",

            [](Scene&, Game& game, Level& currLevel){
                if(game.showingLevel){
                    return;
                }

                currLevel.lifes = MAX_PLAYER_HEALTH;
                currLevel.inf_lifes = true;
                currLevel.bombs = MAX_PLAYER_HEALTH;
                currLevel.inf_bombs = true;
                currLevel.points = 99999;
            }

        },

        {
            "suicide",

            [](Scene&, Game& game, Level& currLevel){
                if(game.showingLevel){
                    return;
                }

                currLevel.lifes = 1;
                game.dmg();
            }

        },

        {
            "cake",

            [](Scene&, Game& game, Level& currLevel){
                if(game.showingLevel){
                    return;
                }

                currLevel.diff = 0;
            }

        },

        {
            "noob",

            [](Scene&, Game& game, Level& currLevel){
                if(game.showingLevel){
                    return;
                }

                currLevel.diff = 1;
            }

        },

        {
            "hard",

            [](Scene&, Game& game, Level& currLevel){
                if(game.showingLevel){
                    return;
                }

                currLevel.diff = 2;
            }

        },

        {
            "pro",

            [](Scene&, Game& game, Level& currLevel){
                if(game.showingLevel){
                    return;
                }

                currLevel.diff = 3;
            }

        },

        {
            "hell",

            [](Scene&, Game& game, Level& currLevel){
                if(game.showingLevel){
                    return;
                }

                currLevel.diff = 4;
            }

        },

        {
            "realpro",

            [](Scene&, Game& game, Level& currLevel){
                if(game.showingLevel){
                    return;
                }

                currLevel.lifes = 1;
                currLevel.bombs = 1;
                currLevel.points = 1;
                currLevel.diff = 4;
            }

        },

        {
            "dmg",
            [](Scene&, Game& game, Level&){
                if(game.showingLevel){
                    return;
                }

                game.dmg();
            }
        },

        {
            "l1",

            [](Scene&, Game& game, Level& currLevel){
                if(game.showingLevel){
                    return;
                }

                Level level = genLevel(0, currLevel.diff,
                std::get<leveldefs::HEARTS>(leveldefs::levels[currLevel.diff]),
                std::get<leveldefs::BOMBS>(leveldefs::levels[currLevel.diff]), 0);

                game.loadLevel(level);
            }
        },

        {
            "l2",

            [](Scene&, Game& game, Level& currLevel){
                if(game.showingLevel){
                    return;
                }

                currLevel.n = 0;
                game.nextLevel();
            }
        },

        {
            "l3",

            [](Scene&, Game& game, Level& currLevel){
                if(game.showingLevel){
                    return;
                }

                currLevel.n = 1;
                game.nextLevel();
            }
        },

        {
            "l4",

            [](Scene&, Game& game, Level& currLevel){
                if(game.showingLevel){
                    return;
                }

                currLevel.n = 2;
                game.nextLevel();
            }
        },

        {
            "l5",

            [](Scene&, Game& game, Level& currLevel){
                if(game.showingLevel){
                    return;
                }

                currLevel.n = 3;
                game.nextLevel();
            }
        },
    };
