#include "clock.hpp"
#include "basic/object.hpp"

#include <ctime>

namespace ue{
    Clock::Clock(){
        circle.setRadius(50.f);
        circle.setFillColor(sf::Color(150, 150, 150, 255));
        circle.setOutlineColor(sf::Color(100, 100, 100, 250));
        circle.setOutlineThickness(4.f);

        seconds.setSize(sf::Vector2f(2, 45));
        sf::Vector2f origin(1.f, 35.f);
        seconds.setOrigin(origin);
        seconds.setFillColor(sf::Color(255, 0, 0, 250));

        minutes.setSize(sf::Vector2f(4, 40));
        origin = sf::Vector2f(2.f, 40.f);
        minutes.setOrigin(origin);
        minutes.setFillColor(sf::Color(0, 0, 0, 250));

        hours.setSize(sf::Vector2f(6, 30));
        origin = sf::Vector2f(3.f, 30.f);
        hours.setOrigin(origin);
        hours.setFillColor(sf::Color(0, 0, 0, 250));

        size.x = 75.f;
        size.y = 75.f;

        locType = SubjectLocation::RIGHT_TOP;
        sizeType = SubjectSize::KEEP_PROPORTIONS;
    }

    void Clock::paintOn(sf::RenderTarget& window){
        std::time_t t = std::time(0);   // get time now
        struct tm* now = std::localtime(&t);
        unsigned h = now->tm_hour;
        if(h > 11){
            h -= 12;
        }
        const sf::Vector2f scale {
            realSize.x / 100.f,
            realSize.y / 100.f
        };

        seconds.setPosition(sf::Vector2f(realLoc.x + 45.f * scale.x, 50.f * scale.y));
        seconds.setRotation(now->tm_sec * 6.f);
        seconds.setScale(scale);

        minutes.setPosition(sf::Vector2f(realLoc.x + 45.f * scale.x, 50.f * scale.y));
        minutes.setRotation(now->tm_min * 6.f);
        minutes.setScale(scale);

        hours.setPosition(sf::Vector2f(realLoc.x + 45.f * scale.x, 50.f * scale.y));
        hours.setRotation(h * 30.f + (now->tm_min / 60.f) * 30.f);
        hours.setScale(scale);

        circle.setPosition(realLoc.x - 5.f * scale.x, 5.f * scale.y); // realLoc.y == 0 always true
        circle.setScale(scale);

        window.draw(circle);
        window.draw(minutes);
        window.draw(hours);
        window.draw(seconds);
    }

    Clock::~Clock(){
        delete sprite;
    }

    std::string showTime(const sf::Time& time){
        std::time_t secs = time.asSeconds();
        struct tm* t = std::localtime(&secs);
        return std::to_string(t->tm_hour-1) + std::string(":") + std::to_string(t->tm_min) + std::string(":") + std::to_string(t->tm_sec);
    }
}
