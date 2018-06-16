#ifndef SAVE_HPP
#define SAVE_HPP

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

#define exit_game ue::save::save();std::exit(0);

namespace ue{
    namespace save{
        struct Score{
            Score() = default;
            Score(uint8_t m, int64_t s, uint32_t t) : mode(m), score(s), time(t){};

            uint8_t mode;
            int64_t score;
            uint32_t time;
        };

        struct Options{
            Options() : winSize(1280, 720), winPos(-1000, -1000) {}

            bool        vsync = false,
                        fullscreen = true,
                        fps = false,
                        clock = true,
                        autoaim = false;

            uint8_t     aa = 8, volume = 50;

            uint32_t    games = 0,
                        wins = 0,
                        losses = 0;

            sf::Vector2u winSize;
            sf::Vector2i winPos;
        };

        extern std::string scores_path;
        extern std::string options_path;
        extern std::vector<Score> score;
        extern Options options;

        #ifdef _WIN32
        void init();
        #endif
        void load();
        void save();
    }
}
#endif // SAVE_HPP
