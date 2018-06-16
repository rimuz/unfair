#include "basic/subject.hpp"

namespace ue{
	void Text::paintOn(sf::RenderTarget& window){
        sf::FloatRect rect = text.getLocalBounds();

        text.setScale(realSize.x / rect.width, realSize.y / rect.height);

		/*
		 * The following line of code might seem strange, but
		 * it fixes a bug of the SFML library.
		*/
        float diff = (rect.height - text.getCharacterSize()) * text.getScale().y;
        text.setPosition(realLoc.x, realLoc.y + diff);

        window.draw(text);
    }

    void Text::pack(){
        sf::FloatRect s = text.getLocalBounds();
        size.x = s.width;
        size.y = s.height;
    }
}