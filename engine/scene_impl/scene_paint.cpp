#include "engine/scene.hpp"
#include "io/data.hpp"
#include "io/save.hpp"

#include <iostream>

namespace ue{
    sf::Image window_icon;
    sf::Vector2u window_icon_sz;
    std::string cheat;

    void Scene::startScene(){
        Scene::curr_scene = this;

        window_icon = assets::teye.copyToImage();
        window_icon_sz = window_icon.getSize();


        sf::ContextSettings settings;
        settings.antialiasingLevel = save::options.aa;
        if(save::options.fullscreen){
            window.create(sf::VideoMode::getDesktopMode(), "Unfair", sf::Style::Fullscreen, settings);
        } else {
            window.create(sf::VideoMode(save::options.winSize.x, save::options.winSize.y), "Unfair", sf::Style::Default, settings);
            if(save::options.winPos.x != -1000.f){
                window.setPosition(save::options.winPos);
            } else {
                save::options.winPos = window.getPosition();
            }
        }
        window.setIcon(window_icon_sz.x, window_icon_sz.y, window_icon.getPixelsPtr());
        window.setMouseCursorVisible(false);
        window.setVerticalSyncEnabled(save::options.vsync);

        sf::Event event;
        sf::Time time;
        while(window.isOpen()){
            mousePosition = sf::Mouse::getPosition(window);
            STANDARD_CURSOR.setPosition(mousePosition.x -16.f, mousePosition.y -16.f);

            while(window.pollEvent(event)){
                switch(event.type){
                    case sf::Event::Closed:{
                        exit_game
                        window.close();
                        break;
                    }

                    case sf::Event::TextEntered:{
                        if (event.text.unicode < 128){
                            char ch = static_cast<char>(event.text.unicode);
                            if(ch >= 65 && ch <= 90){
                                ch += 32; // sets ch lower case!!
                            }

                            if(ch == '\b'){
                                if(cheat.size() > 0){
                                    cheat.pop_back();
                                }
                                break;
                            } else if(ch == '\t'){
                                auto it = cheats.find(cheat);
                                if(it != cheats.end()){
                                    if(Game::curr_game){
                                        it->second(*this, GAME, GAME.currLevel);
                                    }
                                }
                                cheat = "";
                            } else {
                                cheat += ch;
                            }
                        }
                        break;
                    }

                    case sf::Event::LostFocus:{
                        pause();
                        break;
                    }

                    case sf::Event::Resized:{
                        sf::FloatRect newRect(0.f, 0.f, event.size.width, event.size.height);
                        window.setView(sf::View(newRect));

                        save::options.winSize = window.getSize();
                        break;
                    }

                    case sf::Event::KeyPressed:{
                        switch(event.key.code){
                            case sf::Keyboard::Escape:
                            case sf::Keyboard::Pause: {
                                onEscape();
                                break;
                            }

                            case sf::Keyboard::F:
                            case sf::Keyboard::F12:{
                                toggleFullScreen();
                                updateOptions();
                                break;
                            }

                            case sf::Keyboard::F3:{
                                save::options.fps = !save::options.fps;
                                updateOptions();
                                break;
                            }

                            case sf::Keyboard::F4:{
                                save::options.clock = !save::options.clock;
                                updateOptions();
                                break;
                            }

                            case sf::Keyboard::F5:{
                                window.setVerticalSyncEnabled(save::options.vsync = !save::options.vsync);
                                updateOptions();
                                break;
                            }

                            case sf::Keyboard::F6:{
                                save::options.autoaim = !save::options.autoaim;
                                updateOptions();
                                break;
                            }

                            case sf::Keyboard::F7:{
                                save::options.aa *= 2;
                                if(save::options.aa == 0){
                                    save::options.aa += 2;
                                } else if(save::options.aa == 32){
                                    save::options.aa = 0;
                                }
                                recreate();
                                updateOptions();
                                break;
                            }

                            default:{
                                break;
                            }
                        }
                        fireKeyPressed(event);
                        break;
                    }

                    case sf::Event::KeyReleased:{
                        fireKeyReleased(event);
                        break;
                    }

                    case sf::Event::MouseButtonPressed:{
                        fireMousePressed(event);
                        break;
                    }

                    case sf::Event::MouseButtonReleased:{
                        fireMouseReleased(event);
                        break;
                    }

                    case sf::Event::MouseWheelScrolled:{
                        fireWheelMoved(event);
                        break;
                    }

                    case sf::Event::MouseMoved:{
                        fireMouseMoved(event);
                        break;
                    }

                    default:{
                        break;
                    }
                }
            }

            window.clear(showing_game ? std::get<2>(leveldefs::levels_bg[GAME.currLevel.n]) : sf::Color::Black);
            time = clock.restart();
            tpf = time.asSeconds();
            update(tpf);

            if(toClearJobs){
                jobs.clear();
                toClearJobs = false;
            }

            for(size_t i = 0; i < jobsToAdd.size(); i++){
                jobs.push_back(jobsToAdd[i]);
            }
            jobsToAdd.clear();

            bool can = !ue::Game::curr_game || (!GAME.paused && !GAME.stopped);
            for(size_t i = 0; i < jobs.size(); ){
                if(can || jobs[i].priority){
                    jobs[i].remeaningTime -= tpf;
                    if(jobs[i].remeaningTime < 0.f){
                        jobs[i].onEnd(jobs[i]);
                        if(jobs[i].canErase){
                            if(jobs[i].ptr){
                                delete jobs[i].ptr;
                            }
                            jobs.erase(jobs.begin() + i);
                        } else {
                            i++;
                        }
                    } else {
                        i++;
                    }
                } else {
                    i++;
                }
            }

            window.draw(STANDARD_CURSOR);
            window.display();
        }
    }

    void Scene::addSubject(Subject* subj){
        toAdd.push_back(subj);
    }

    void Scene::removeSubject(Subject* subj){
        toRemove.push_back(subj);
    }

    void Scene::addTimeJob(TimeJob& job){
        jobsToAdd.push_back(job);
    }

    void Scene::clearTimeJobs(){
        toClearJobs = true;
    }

    void Scene::update(float){
        for(size_t i = 0; i < toRemove.size(); i++){
            auto pos = std::remove(subjects.begin(), subjects.end(), toRemove[i]);
            if(!subjects[pos - subjects.begin()]->permanent){
                delete subjects[pos - subjects.begin()];
            }
            subjects.erase(pos, subjects.end());
        }
        toRemove.clear();

        if(toClear){
            for(size_t i = 0; i < subjects.size(); i++){
                if(!subjects[i]->permanent){
                    delete subjects[i];
                }
            }
            toClear = false;
            subjects.clear();
        }

        for(size_t i = 0; i < toAdd.size(); i++){
            subjects.push_back(toAdd[i]);
        }
        toAdd.clear();

        sf::Vector2u dim = window.getSize();
        const float scaleX = static_cast<float>(dim.x) / 1280.f;
        const float scaleY = static_cast<float>(dim.y) / 720.f;
        for(size_t i = 0; i < subjects.size(); i++){
            paint(subjects[i], dim, scaleX, scaleY);
        }
        if(save::options.clock){
            paint(watch, dim, scaleX, scaleY);
        }

        if(save::options.fps){
            fps->text.setString(std::string("FPS: ") + std::to_string(static_cast<unsigned>(1.f/TPF)));
            fps->pack();
            paint(fps, dim, scaleX, scaleY);
        }
    }

    void Scene::paint(Subject* subj, sf::Vector2u dim, const float scaleX, const float scaleY){
        Subject& sub = *subj;

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
                break;
            }

            case SubjectSize::CLEAR_SCREEN:{
                window.clear();
                return;
            }

            case SubjectSize::CONSTANT:{}
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
                p.y = 100.f * s.y / s.y;
                break;
            }

            case SubjectLocation::SOUTH_POPUP:{
                p.x = (dim.x - s.x) /2.f;
                p.y = dim.y - s.y - (100.f * s.y / s.y);
                break;
            }

            case SubjectLocation::CUSTOM:{
                p.x *= scaleX;
                p.y *= scaleY;
                break;
            }

            case SubjectLocation::INVISIBLE:{
                return;
            }

            case SubjectLocation::CONSTANT:{}
        }

        sub.realLoc = p;
        sub.realSize = s;

        sub.paintOn(window);
    }

    void Scene::clearMemory(){
        toClear = true;
        toAdd.clear();
        toRemove.clear();
    }

    void Scene::toggleFullScreen(){
        save::options.fullscreen = !save::options.fullscreen;

        sf::ContextSettings settings;
        settings.antialiasingLevel = save::options.aa;
        if(save::options.fullscreen){
            save::options.winPos = window.getPosition();
            window.create(sf::VideoMode::getDesktopMode(), "Unfair", sf::Style::Fullscreen, settings);
        } else {
            window.create(sf::VideoMode(save::options.winSize.x, save::options.winSize.y), "Unfair", sf::Style::Default, settings);
            if(save::options.winPos.x != -1000.f){
                window.setPosition(save::options.winPos);
            } else {
                save::options.winPos = window.getPosition();
            }
        }

        window.setIcon(window_icon_sz.x, window_icon_sz.y, window_icon.getPixelsPtr());
        window.setMouseCursorVisible(false);
        window.setVerticalSyncEnabled(save::options.vsync);
    }

    void Scene::updateOptions(){
        optionsMenu->update();
    }

    void Scene::recreate(){
        sf::ContextSettings settings;
        settings.antialiasingLevel = save::options.aa;
        if(save::options.fullscreen){
            window.create(sf::VideoMode::getDesktopMode(), "Unfair", sf::Style::Fullscreen, settings);
        } else {
            window.create(sf::VideoMode(save::options.winSize.x, save::options.winSize.y), "Unfair", sf::Style::Default, settings);
            if(save::options.winPos.x != -1000.f){
                window.setPosition(save::options.winPos);
            } else {
                save::options.winPos = window.getPosition();
            }
        }
        window.setIcon(window_icon_sz.x, window_icon_sz.y, window_icon.getPixelsPtr());
        window.setMouseCursorVisible(false);
        window.setVerticalSyncEnabled(save::options.vsync);
    }

    void Scene::fireKeyPressed(sf::Event& event){
        for(size_t i = 0; i < subjects.size(); i++){
            subjects[i]->keyPressed(event);
        }
    }

    void Scene::fireKeyReleased(sf::Event& event){
        for(size_t i = 0; i < subjects.size(); i++){
            subjects[i]->keyReleased(event);
        }
    }

    void Scene::fireMousePressed(sf::Event& event){
        for(size_t i = 0; i < subjects.size(); i++){
            subjects[i]->mousePressed(event);
        }
    }

    void Scene::fireMouseReleased(sf::Event& event){
        for(size_t i = 0; i < subjects.size(); i++){
            subjects[i]->mouseReleased(event);
        }
    }

    void Scene::fireWheelMoved(sf::Event& event){
        for(size_t i = 0; i < subjects.size(); i++){
            subjects[i]->mouseWheelEvent(event);
        }
    }

    void Scene::fireMouseMoved(sf::Event& event){
        for(size_t i = 0; i < subjects.size(); i++){
            subjects[i]->mouseMoved(event);
        }
    }
}
