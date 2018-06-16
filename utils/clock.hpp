#ifndef     CLOCK_HPP
#define     CLOCK_HPP

#include <SFML/Graphics.hpp>
#include "basic/subject.hpp"

namespace ue{
    class Clock : public Subject{
    public:
        Clock();
        virtual void paintOn(sf::RenderTarget& window);
        virtual ~Clock();

        sf::CircleShape circle;
        sf::RectangleShape hours, minutes, seconds;
    };

    std::string showTime(const sf::Time& time);
}
#endif // CLOCK_HPP
