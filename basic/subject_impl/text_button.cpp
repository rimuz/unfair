#include "basic/subject.hpp"
#include "io/data.hpp"
#include "engine/scene.hpp"

namespace ue{
	 bool Button::isSelected(){
        return selected;
    }

    Button::~Button(){}

    TextButton::TextButton(){
        autonomous = true;

        text.setFont(STANDARD_FONT);
        text.setCharacterSize(50);

        deselect();
    }

    void TextButton::select(){
        selected = true;
        text.setFillColor(sf::Color::Blue);
    }

    void TextButton::deselect(){
        selected = false;
        text.setFillColor(sf::Color::Red);
    }

    void TextButton::disable(std::string t){
        enabled = false;

        text.setFillColor(sf::Color(120, 120, 120));
        text.setString(t);
        pack();
    }

    void TextButton::enable(std::string t){
        enabled = true;

        text.setFillColor(sf::Color::Red);
        text.setString(t);
        pack();
    }

    void TextButton::mousePressed(sf::Event& event){
        sf::FloatRect rect = text.getGlobalBounds();
        if(rect.contains(event.mouseButton.x, event.mouseButton.y) && enabled){
            action();
        }
    }

    void TextButton::mouseMoved(sf::Event&){
        if(autonomous){
            sf::FloatRect rect = text.getGlobalBounds();
            sf::Vector2i& pos = MOUSE_POS;

            bool s = rect.contains(pos.x, pos.y);
            if(s){
                select();
            } else {
                deselect();
            }
        }
    }

    void TextButton::paintOn(sf::RenderTarget& window){
        Text::paintOn(window);
    }

    TextButton::~TextButton(){}
}
