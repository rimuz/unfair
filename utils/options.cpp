#include "utils/options.hpp"
#include "utils/gamedefs.hpp"
#include "io/save.hpp"
#include "io/data.hpp"
#include "engine/scene.hpp"

namespace ue{
    void OptionsGroup::init(){
        buttons.reserve(7);
        addButton(std::string("MUSIC: ") + std::to_string(static_cast<unsigned>(save::options.volume)) + "%", [](){});
        addButton(std::string("V-SYNC: ") + B_TO_IS(save::options.vsync),
            [this](){
                SCENE.window.setVerticalSyncEnabled(save::options.vsync = !save::options.vsync);
                buttons[1]->text.setString(std::string("V-SYNC: ") + B_TO_IS(save::options.vsync));
                buttons[1]->pack();
                packVertical();
            });
        addButton(std::string("AA: ") + std::to_string(save::options.aa) + "x",
            [this](){
                save::options.aa *= 2;
                if(save::options.aa == 0){
                    save::options.aa += 2;
                } else if(save::options.aa == 32){
                    save::options.aa = 0;
                }

                SCENE.recreate();
                buttons[2]->text.setString(std::string("AA: ") + std::to_string(save::options.aa) + "x");
                buttons[2]->pack();
                packVertical();
            });
        addButton(std::string("FULLSCREEN: ") + B_TO_IS(save::options.fullscreen),
            [this](){
                SCENE.toggleFullScreen();
                buttons[3]->text.setString(std::string("FULLSCREEN: ") + B_TO_IS(save::options.fullscreen));
                buttons[3]->pack();
                packVertical();
            });
        addButton(std::string("SHOW FPS: ") + B_TO_IS(save::options.fps),
            [this](){
                buttons[4]->text.setString(std::string("SHOW FPS: ") + B_TO_IS(save::options.fps = !save::options.fps));
                buttons[4]->pack();
                packVertical();
            });
        addButton(std::string("SHOW CLOCK: ") + B_TO_IS(save::options.clock),
            [this](){
                buttons[5]->text.setString(std::string("SHOW CLOCK: ") + B_TO_IS(save::options.clock = !save::options.clock));
                buttons[5]->pack();
                packVertical();
            });
        addButton(std::string("AUTOAIM: ") + B_TO_IS(save::options.autoaim),
            [this](){
                buttons[6]->text.setString(std::string("AUTOAIM: ") + B_TO_IS(save::options.autoaim = !save::options.autoaim));
                buttons[6]->pack();
                packVertical();
            });
        packVertical();
    }

    void OptionsGroup::update(){
        buttons[0]->text.setString(std::string("MUSIC: ") + std::to_string(static_cast<unsigned>(save::options.volume)) + "%");
        buttons[0]->pack();

        buttons[1]->text.setString(std::string("V-SYNC: ") + B_TO_IS(save::options.vsync));
        buttons[1]->pack();

        buttons[2]->text.setString(std::string("AA: ") + std::to_string(save::options.aa) + "x");
        buttons[2]->pack();

        buttons[3]->text.setString(std::string("FULLSCREEN: ") + B_TO_IS(save::options.fullscreen));
        buttons[3]->pack();

        buttons[4]->text.setString(std::string("SHOW FPS: ") + B_TO_IS(save::options.fps));
        buttons[4]->pack();

        buttons[5]->text.setString(std::string("SHOW CLOCK: ") + B_TO_IS(save::options.clock));
        buttons[5]->pack();

        buttons[6]->text.setString(std::string("AUTOAIM: ") + B_TO_IS(save::options.autoaim));
        buttons[6]->pack();

        packVertical();
    }

    void OptionsGroup::keyPressed(sf::Event& event){
        if(selected == 0){
            if(event.key.code == sf::Keyboard::Right){
                if(save::options.volume != 100){
                    save::options.volume += 1;
                }

                assets::bgMusic.setVolume(save::options.volume);
                assets::menuMusic.setVolume(save::options.volume);

                buttons[0]->text.setString(std::string("MUSIC: ") + std::to_string(static_cast<unsigned>(save::options.volume)) + "%");
                buttons[0]->pack();
                packVertical();
            } else if(event.key.code == sf::Keyboard::Left){
                if(save::options.volume != 0){
                    save::options.volume -= 1;
                }

                assets::bgMusic.setVolume(save::options.volume);
                assets::menuMusic.setVolume(save::options.volume);

                buttons[0]->text.setString(std::string("MUSIC: ") + std::to_string(static_cast<unsigned>(save::options.volume)) + "%");
                buttons[0]->pack();
                packVertical();
            }
        }

        TextButtonGroup::keyPressed(event);
    }
}
