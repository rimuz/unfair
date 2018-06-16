#ifndef UTILS_OPTIONS_HPP
#define UTILS_OPTIONS_HPP

#include "basic/subject.hpp"

namespace ue{
    class OptionsGroup : public TextButtonGroup{
    public:
        void init();
        void update();
        void keyPressed(sf::Event& event) override;
    };
}
#endif // UTILS_OPTIONS_HPP
