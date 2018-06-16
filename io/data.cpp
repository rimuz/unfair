#include "data.hpp"
#include "utils/gamedefs.hpp"
#include "engine/scene.hpp"
#include "save.hpp"

#include <string>

namespace ue{
    namespace assets{
        using uinteger = sf::Uint8;

        constexpr uinteger font_str[] = {
            #include "txt_data/font.txt"
        };
        constexpr std::size_t font_str_sz = arr_size(font_str);

        constexpr uinteger font2_str[] = {
            #include "txt_data/font2.txt"
        };
        constexpr std::size_t font2_str_sz = arr_size(font2_str);

        constexpr uinteger eye_str[] = {
            #include "txt_data/eye.txt"
        };
        constexpr std::size_t eye_str_sz = arr_size(eye_str);

        constexpr uinteger heart_bomb_str[] = {
            #include "txt_data/heart_bomb.txt"
        };
        constexpr std::size_t heart_bomb_str_sz = arr_size(heart_bomb_str);

        constexpr uinteger splash_str[] = {
            #include "txt_data/splash.txt"
        };
        constexpr std::size_t splash_str_sz = arr_size(splash_str);

        constexpr uinteger cursor_str[] = {
            #include "txt_data/cursor.txt"
        };
        constexpr std::size_t cursor_str_sz = arr_size(cursor_str);

        constexpr uinteger title_str[] = {
            #include "txt_data/title.txt"
        };
        constexpr std::size_t title_str_sz = arr_size(title_str);

        constexpr uinteger bullets_str[] = {
            #include "txt_data/bullet.txt"
        };
        constexpr std::size_t bullets_str_sz = arr_size(bullets_str);

        constexpr uinteger black_screen_str[] = {
            #include "txt_data/black_screen.txt"
        };
        constexpr std::size_t black_screen_str_sz = arr_size(black_screen_str);

        constexpr uinteger bosses_str[] = {
            #include "txt_data/bosses.txt"
        };
        constexpr std::size_t bosses_str_sz = arr_size(bosses_str);

        constexpr uinteger menu_music_str[] = {
            #include "txt_data/menu_music.txt"
        };
        constexpr std::size_t menu_music_str_sz = arr_size(menu_music_str);

        constexpr uinteger bg_music_str[] = {
            #include "txt_data/bg_music.txt"
        };
        constexpr std::size_t bg_music_str_sz = arr_size(bg_music_str);


        sf::Texture teye, tsplash, tcursor, ttitle, tbullets,
                tred_bullet, tgreen_bullet, tblue_bullet, twhite_bullet,
                theart, tbomb, tbosses[11], tblack_screen;

        sf::Sprite eye, splash, cursor, title, red_bullet, green_bullet,
                blue_bullet, white_bullet, heart, bomb, black_screen, bosses[11];

        sf::Font font, font2;

        sf::Music menuMusic, bgMusic;

        sf::RectangleShape square_bg(sf::Vector2f(2000.f, 2000.f));
        sf::Text level_bg_txt;

        void loadAssets(){
            // TEXTURES
            tsplash.loadFromMemory(splash_str, splash_str_sz);
            tsplash.setSmooth(true);

            tcursor.loadFromMemory(cursor_str, cursor_str_sz);

            teye.loadFromMemory(eye_str, eye_str_sz);
            teye.setSmooth(true);

            ttitle.loadFromMemory(title_str, title_str_sz);
            ttitle.setSmooth(true);

            tred_bullet.loadFromMemory(bullets_str, bullets_str_sz, sf::IntRect(0, 0, 200, 200));
            tred_bullet.setSmooth(true);

            tgreen_bullet.loadFromMemory(bullets_str, bullets_str_sz, sf::IntRect(200, 0, 200, 200));
            tgreen_bullet.setSmooth(true);

            tblue_bullet.loadFromMemory(bullets_str, bullets_str_sz, sf::IntRect(400, 0, 200, 200));
            tblue_bullet.setSmooth(true);

            twhite_bullet.loadFromMemory(bullets_str, bullets_str_sz, sf::IntRect(600, 0, 200, 200));
            twhite_bullet.setSmooth(true);

            tblack_screen.loadFromMemory(black_screen_str, black_screen_str_sz);
            tblack_screen.setSmooth(true);

            theart.loadFromMemory(heart_bomb_str, heart_bomb_str_sz, sf::IntRect(0, 0, 100, 100));
            theart.setSmooth(true);

            tbomb.loadFromMemory(heart_bomb_str, heart_bomb_str_sz, sf::IntRect(100, 0, 100, 100));
            tbomb.setSmooth(true);

            tbosses[0].loadFromMemory(bosses_str, bosses_str_sz, sf::IntRect(0, 0, 278, 278));
            tbosses[0].setSmooth(true);

            tbosses[1].loadFromMemory(bosses_str, bosses_str_sz, sf::IntRect(278, 0, 278, 278));
            tbosses[1].setSmooth(true);

            tbosses[2].loadFromMemory(bosses_str, bosses_str_sz, sf::IntRect(556, 0, 278, 278));
            tbosses[2].setSmooth(true);

            tbosses[3].loadFromMemory(bosses_str, bosses_str_sz, sf::IntRect(0, 278, 278, 278));
            tbosses[3].setSmooth(true);

            tbosses[4].loadFromMemory(bosses_str, bosses_str_sz, sf::IntRect(278, 278, 278, 278));
            tbosses[4].setSmooth(true);

            tbosses[5].loadFromMemory(bosses_str, bosses_str_sz, sf::IntRect(556, 278, 278, 278));
            tbosses[5].setSmooth(true);

            tbosses[6].loadFromMemory(bosses_str, bosses_str_sz, sf::IntRect(0, 556, 278, 278));
            tbosses[6].setSmooth(true);

            tbosses[7].loadFromMemory(bosses_str, bosses_str_sz, sf::IntRect(278, 556, 278, 278));
            tbosses[7].setSmooth(true);

            tbosses[8].loadFromMemory(bosses_str, bosses_str_sz, sf::IntRect(556, 556, 278, 278));
            tbosses[8].setSmooth(true);

            tbosses[9].loadFromMemory(bosses_str, bosses_str_sz, sf::IntRect(0, 834, 278, 278));
            tbosses[9].setSmooth(true);

            tbosses[10].loadFromMemory(bosses_str, bosses_str_sz, sf::IntRect(0, 1112, 666, 278));
            tbosses[10].setSmooth(true);

            // SPRITES
            eye.setTexture(teye);
            splash.setTexture(tsplash);
            cursor.setTexture(tcursor);
            title.setTexture(ttitle);

            sf::Vector2f origin(BULLET_ORIGIN, BULLET_ORIGIN);

            red_bullet.setTexture(tred_bullet);
            red_bullet.setOrigin(origin);

            green_bullet.setTexture(tgreen_bullet);
            green_bullet.setOrigin(origin);

            blue_bullet.setTexture(tblue_bullet);
            blue_bullet.setOrigin(origin);

            white_bullet.setTexture(twhite_bullet);
            white_bullet.setOrigin(origin);

            black_screen.setTexture(tblack_screen);

            heart.setTexture(theart);
            heart.setScale(0.5f, 0.5f);

            bomb.setTexture(tbomb);
            bomb.setScale(0.5f, 0.5f);

            sf::Vector2u txtSize;
            for(size_t i = 0; i < 11; i++){
                txtSize = tbosses[i].getSize();
                origin = {
                    static_cast<float>(txtSize.x) / 2.f,
                    static_cast<float>(txtSize.y) / 2.f
                };

                bosses[i].setTexture(tbosses[i]);
                bosses[i].setOrigin(origin);
            }

            origin = {50.f, 50.f};

            // FONTS
            font.loadFromMemory(font_str, font_str_sz);
            font2.loadFromMemory(font2_str, font2_str_sz);

            // BG
            square_bg.setFillColor(sf::Color::White);
            square_bg.setOutlineThickness(15.f);

            level_bg_txt.setCharacterSize(500);
            level_bg_txt.setFont(CORNERSTONE_FONT);

            // MUSIC
            menuMusic.openFromMemory(menu_music_str, menu_music_str_sz);
            menuMusic.setLoop(true);
            menuMusic.setVolume(save::options.volume);

            bgMusic.openFromMemory(bg_music_str, bg_music_str_sz);
            bgMusic.setLoop(true);
            bgMusic.setVolume(save::options.volume);
        }


        void colorBG(size_t level){
            square_bg.setFillColor(std::get<0>(leveldefs::levels_bg[level]));
            square_bg.setOutlineColor(std::get<1>(leveldefs::levels_bg[level]));

            level_bg_txt.setString(std::to_string(level+1));
            level_bg_txt.setFillColor(std::get<1>(leveldefs::levels_bg[level]));
            sf::FloatRect rect = level_bg_txt.getLocalBounds();
            float diff = (rect.height - level_bg_txt.getCharacterSize()) * level_bg_txt.getScale().y;
            level_bg_txt.setPosition((2000.f - rect.width) /2.f, (2000.f - rect.height) /2.f + diff);
        }
    }
}
