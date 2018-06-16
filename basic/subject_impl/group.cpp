#include "basic/subject.hpp"

namespace ue{
    void Group::addSubject(Subject* subj){
        subjects.push_back(subj);
    }

    void Group::removeSubject(Subject* subj){
        toRemove.push_back(subj);
    }

    void Group::clearMemory(){
        toClear = true;
        toAdd.clear();
        toRemove.clear();
    }

    void Group::keyPressed(sf::Event& event){
        for(size_t i = 0; i < subjects.size(); i++){
            subjects[i]->keyPressed(event);
        }
    }

    void Group::keyReleased(sf::Event& event){
        for(size_t i = 0; i < subjects.size(); i++){
            subjects[i]->keyReleased(event);
        }
    }

    void Group::mousePressed(sf::Event& event){
        for(size_t i = 0; i < subjects.size(); i++){
            subjects[i]->mousePressed(event);
        }
    }

    void Group::mouseReleased(sf::Event& event){
        for(size_t i = 0; i < subjects.size(); i++){
            subjects[i]->mouseReleased(event);
        }
    }

    void Group::mouseWheelEvent(sf::Event& event){
        for(size_t i = 0; i < subjects.size(); i++){
            subjects[i]->mouseWheelEvent(event);
        }
    }

    void Group::mouseMoved(sf::Event& event){
        for(size_t i = 0; i < subjects.size(); i++){
            subjects[i]->mouseMoved(event);
        }
    }

    void Group::paintOn(sf::RenderTarget& window){
        for(size_t i = 0; i < toRemove.size(); i++){
            subjects.erase(std::remove(subjects.begin(), subjects.end(), toRemove[i]), subjects.end());
        }
        toRemove.clear();

        if(toClear){
            toClear = false;
            subjects.clear();
        }

        for(size_t i = 0; i < toAdd.size(); i++){
            subjects.push_back(toAdd[i]);
        }
        toAdd.clear();

        sf::Vector2f dim = realSize;
        sf::Vector2u win = window.getSize();

        const float scaleX = static_cast<float>(win.x) / 1280.f;
        const float scaleY = static_cast<float>(win.y) / 720.f;

        for(size_t i = 0; i < subjects.size(); i++){
            Subject& sub = *subjects[i];

            sf::Vector2f p = sub.location;
            sf::Vector2f s = sub.size;

            switch(sub.sizeType){
                case SubjectSize::RESIZE:{
                    s.x *= scaleX;
                    s.y *= scaleY;
                    break;
                }

                case SubjectSize::KEEP_PROPORTIONS:{
                    if(scaleX > scaleY){
                        s.x *= scaleY;
                        s.y *= scaleY;
                    } else {
                        s.x *= scaleX;
                        s.y *= scaleX;
                    }
                }

                case SubjectSize::CONSTANT:{
                    break;
                }

                case SubjectSize::CLEAR_SCREEN:{
                    window.clear();
                    continue;
                }
            }

            switch(sub.locType){
                case SubjectLocation::LEFT_TOP:{
                    p.x = 0.f;
                    p.y = 0.f;
                    break;
                }

                case SubjectLocation::TOP:{
                    p.x = (dim.x - s.x) /2.f;
                    p.y = 0.f;
                    break;
                }

                case SubjectLocation::RIGHT_TOP:{
                    p.x = dim.x - s.x;
                    p.y = 0.f;
                    break;
                }

                case SubjectLocation::LEFT:{
                    p.x = 0.f;
                    p.y = (dim.y - s.y) /2.f;
                    break;
                }

                case SubjectLocation::RIGHT:{
                    p.x = dim.x - s.x;
                    p.y = (dim.y - s.y) /2.f;
                    break;
                }


                case SubjectLocation::CENTER:{
                    p.x = (dim.x - s.x) /2.f;
                    p.y = (dim.y - s.y) /2.f;
                    break;
                }

                case SubjectLocation::LEFT_BOTTOM:{
                    p.x = 0.f;
                    p.y = dim.y - s.y;
                    break;
                }

                case SubjectLocation::BOTTOM:{
                    p.x = (dim.x - s.x) /2.f;
                    p.y = dim.y - s.y;
                    break;
                }

                case SubjectLocation::RIGHT_BOTTOM:{
                    p.x = dim.x - s.x;
                    p.y = dim.y - s.y;
                    break;
                }

                case SubjectLocation::FILL:{
                    p.x = 0.f;
                    p.y = 0.f;
                    s.x = dim.x;
                    s.y = dim.y;
                    break;
                }

                case SubjectLocation::POPUP:{
                    p.x = (dim.x - s.x) /2.f;
                    p.y = 100.f * s.y / sub.size.y;
                    break;
                }

                case SubjectLocation::SOUTH_POPUP:{
                    p.x = (dim.x - s.x) /2.f;
                    p.y = dim.y - s.y - (100.f * s.y / sub.size.y);
                    break;
                }

                case SubjectLocation::CUSTOM:{
                    p.x *= scaleX;
                    p.y *= scaleY;
                    break;
                }

                case SubjectLocation::INVISIBLE:{
                    continue;
                }

                case SubjectLocation::CONSTANT:{}
            }

            p.x += realLoc.x;
            p.y += realLoc.y;

            sub.realLoc = p;
            sub.realSize = s;

            sub.paintOn(window);
        }
    }

    void Group::packVertical(){
        align = false;
        if(!subjects.empty()){
            size.x = 0.f;
            size.y = 0.f;

            for(size_t i = 0; i < subjects.size(); i++){
                size.y += subjects[i]->size.y;
                if(subjects[i]->size.x > size.x){
                    size.x = subjects[i]->size.x;
                }
            }
        }
    }


    void Group::quickClear(){
        for(size_t i = 0; i < subjects.size(); i++){
            if(!subjects[i]->permanent){
                delete subjects[i];
            }
        }

        subjects.clear();
    }

    void Group::packOrizontal(){
        align = true;
        if(!subjects.empty()){
            size.x = 0.f;
            size.y = 0.f;

            for(size_t i = 0; i < subjects.size(); i++){
                size.x += subjects[i]->size.x;
                if(subjects[i]->size.y > size.y){
                    size.y = subjects[i]->size.y;
                }
            }
        }
    }

    Group::~Group(){
        for(size_t i = 0; i < subjects.size(); i++){
            if(!subjects[i]->permanent){
                delete subjects[i];
            }
        }
    }
}
