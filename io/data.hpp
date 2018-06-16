#ifndef         DATA_HPP
#define         DATA_HPP

#define SPLASH_SPRITE (ue::assets::splash)
#define STANDARD_CURSOR (ue::assets::cursor)
#define CORNERSTONE_FONT (ue::assets::font)
#define GOODDOG_FONT (ue::assets::font2)
#define STANDARD_FONT GOODDOG_FONT

#define BORDER(x) (ue::assets::border[x])
#define CURVE(x) (ue::assets::curve[x])

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace ue{
    namespace assets{
        extern sf::Texture teye, tsplash, tcursor, ttitle, tbullets,
                tred_bullet, tgreen_bullet, tblue_bullet, twhite_bullet,
                tblack_screen, tbosses[11], theart, tbomb;
        extern sf::Sprite splash, cursor, eye, title, red_bullet, green_bullet,
                blue_bullet, white_bullet, black_screen, bosses[11],
                heart, bomb;
        extern sf::Font font, font2;

        extern sf::RectangleShape square_bg;
        extern sf::Text level_bg_txt;
        extern sf::Music menuMusic, bgMusic;

        void loadAssets();
        void colorBG(size_t level);
    }
}
#endif // DATA_HPP
