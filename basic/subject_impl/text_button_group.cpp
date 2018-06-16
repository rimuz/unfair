#include "basic/subject.hpp"
#include "engine/scene.hpp"

namespace ue{
     void TextButtonGroup::paintOn(sf::RenderTarget& window){
        unsigned int n = 0;
        for(size_t i = 0; i < buttons.size(); i++){
            TextButton& btn = *buttons[i];

            if(align){
                btn.location.y = (realSize.y - btn.realSize.y) /2;
                btn.location.x = n;
                n += btn.realSize.x;
            } else {
                btn.location.x = (realSize.x - btn.realSize.x) /2;
                btn.location.y = n;
                n += btn.realSize.y;
            }

            if(i == static_cast<unsigned>(selected)){
                btn.select();
            } else if(buttons[i]->enabled){
                btn.deselect();
            }
        }

        Group::paintOn(window);
    }

    void TextButtonGroup::keyPressed(sf::Event& event){
        if(align){
            if(event.key.code == sf::Keyboard::Left){
                do {
                    selected--;
                    if(selected < 0){
                        selected = buttons.size() -1;
                    }
                } while(!buttons[selected]->enabled);
            } else if(event.key.code == sf::Keyboard::Right){
                do {
                    selected++;
                    if(static_cast<unsigned>(selected) >= buttons.size()){
                        selected = 0;
                    }
                } while(!buttons[selected]->enabled);
            }
        } else {
            if(event.key.code == sf::Keyboard::Up){
                do {
                    selected--;
                    if(selected < 0){
                        selected = buttons.size() -1;
                    }
                } while(!buttons[selected]->enabled);
            } else if(event.key.code == sf::Keyboard::Down){
                do {
                    selected++;
                    if(static_cast<unsigned>(selected) >= buttons.size()){
                        selected = 0;
                    }
                } while(!buttons[selected]->enabled);
            }
        }

        if(event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space){
            buttons[selected]->action();
        }
    }

    void TextButtonGroup::mouseMoved(sf::Event&){
        sf::Vector2i& pos = MOUSE_POS;

        if(pos == last){
            return;
        }
        last = pos;

        for(size_t i = 0; i < buttons.size(); i++){
            sf::FloatRect rect = buttons[i]->text.getGlobalBounds();

            if(rect.contains(pos.x, pos.y) && buttons[i]->enabled){
                selected = i;
                break;
            }
        }
    }

    void TextButtonGroup::addButton(std::string text, std::function<void(void)>&& action){
        TextButton* btn = new TextButton();

        btn->text.setString(text);
        btn->pack();
        btn->action = std::move(action);
        btn->autonomous = false;

        btn->locType = SubjectLocation::CONSTANT;
        btn->sizeType = SubjectSize::KEEP_PROPORTIONS;

        Group::addSubject(btn);
        buttons.push_back(btn);
    }

    TextButtonGroup::~TextButtonGroup(){}
}
