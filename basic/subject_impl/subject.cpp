#include "basic/subject.hpp"
#include "engine/scene.hpp"
#include "io/data.hpp"
#include <iostream>

namespace ue{
    void Subject::paintOn(sf::RenderTarget& window){
        if(sprite){
            sf::FloatRect rect = sprite->getLocalBounds();

            sprite->setPosition(realLoc.x, realLoc.y);
            sprite->setScale(realSize.x / rect.width, realSize.y / rect.height);

            window.draw(*sprite);
        }
    }

    Subject::~Subject(){}

    void Subject::keyPressed(sf::Event&){}

    void Subject::keyReleased(sf::Event&){}

    void Subject::mousePressed(sf::Event&){}

    void Subject::mouseReleased(sf::Event&){}

    void Subject::mouseWheelEvent(sf::Event&){}

    void Subject::mouseMoved(sf::Event&){}
}
