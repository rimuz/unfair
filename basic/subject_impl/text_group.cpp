#include "basic/subject.hpp"
#include "io/data.hpp"

namespace ue{
	void TextGroup::addText(std::string text, size_t size, sf::Color color){
        Text* t = new Text;
        t->text.setString(text);
        t->text.setCharacterSize(size);
        t->text.setFillColor(color);
        t->text.setFont(STANDARD_FONT);

        t->pack();
        t->locType = SubjectLocation::CONSTANT;
        t->sizeType = SubjectSize::KEEP_PROPORTIONS;

        Group::addSubject(t);
        texts.push_back(t);
    }

	void TextGroup::paintOn(sf::RenderTarget& window){
        unsigned int n = 0;
        for(size_t i = 0; i < texts.size(); i++){
            Text& text = *texts[i];

            if(align){
                text.location.y = (realSize.y - text.realSize.y) /2;
                text.location.x = n;
                n += text.realSize.x;
            } else {
                text.location.x = (realSize.x - text.realSize.x) /2;
                text.location.y = n;
                n += text.realSize.y;
            }
        }

        Group::paintOn(window);
    }

    TextGroup::~TextGroup(){}
}
