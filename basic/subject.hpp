#ifndef         _BASIC_SUBJECT_HPP_
#define         _BASIC_SUBJECT_HPP_

#include <SFML/Graphics.hpp>
#include <functional>

namespace ue{
    enum class SubjectLocation{
        LEFT_TOP,           TOP,                RIGHT_TOP,
        LEFT,               CENTER,             RIGHT,
        LEFT_BOTTOM,        BOTTOM,             RIGHT_BOTTOM,
        FILL,   INVISIBLE,  POPUP, SOUTH_POPUP, CUSTOM, CONSTANT,
    };

    enum class SubjectSize{
        CONSTANT,   RESIZE,     KEEP_PROPORTIONS,       CLEAR_SCREEN
    };

    class Subject{
    public:
        virtual void paintOn(sf::RenderTarget& window);

        virtual void keyPressed(sf::Event& event);
        virtual void keyReleased(sf::Event& event);
        virtual void mousePressed(sf::Event& event);
        virtual void mouseReleased(sf::Event& event);
        virtual void mouseWheelEvent(sf::Event& event);
        virtual void mouseMoved(sf::Event& event);

        sf::Vector2f location;
        sf::Vector2f size;

        sf::Vector2f realLoc;
        sf::Vector2f realSize;

        sf::Sprite* sprite = nullptr;
        SubjectLocation locType = SubjectLocation::CENTER;
        SubjectSize sizeType = SubjectSize::KEEP_PROPORTIONS;

        bool permanent = false;
        virtual ~Subject();
    };

    class Text : public Subject{
    public:
        sf::Text text;

        virtual void paintOn(sf::RenderTarget& window);
        void pack();
    };

    class Group : public Subject{
    public:
        std::vector<Subject*> subjects;
        bool align = false; // if true align = ORIZONTAL

        void addSubject(Subject* subj);
        void removeSubject(Subject* subj);
        void clearMemory();
        void quickClear();

        virtual void keyPressed(sf::Event& event);
        virtual void keyReleased(sf::Event& event);
        virtual void mousePressed(sf::Event& event);
        virtual void mouseReleased(sf::Event& event);
        virtual void mouseWheelEvent(sf::Event& event);
        virtual void mouseMoved(sf::Event& event);

        void packVertical();
        void packOrizontal();

        virtual void paintOn(sf::RenderTarget& window);

        virtual ~Group();
    private:
        bool toClear = false;
        std::vector<Subject*> toRemove;
        std::vector<Subject*> toAdd;
    };

    class Button {
    public:
        bool selected = false;
        bool autonomous = false;
        std::function<void(void)> action;

        virtual void select() = 0;
        virtual void deselect() = 0;
        virtual bool isSelected();

        virtual ~Button();
    };

    class TextButton : public Text, public Button{
    public:
        TextButton();
        bool enabled = true;

        void select();
        void deselect();

        void disable(std::string text);
        void enable(std::string text);

        virtual void mousePressed(sf::Event& event);
        virtual void mouseMoved(sf::Event& event);
        virtual void paintOn(sf::RenderTarget& window);

        virtual ~TextButton();
    };

    class TextGroup : public Group{
    public:
        std::vector<Text*> texts;

        void addText(std::string text, size_t size = 50, sf::Color color = sf::Color::White);

        virtual void paintOn(sf::RenderTarget& window);

        virtual ~TextGroup();
    };

    class TextButtonGroup : public Group{
    public:
        std::vector<TextButton*> buttons;
        int selected = 0; // MUST be signed int, because '-1' is used internally as 'last button'
        sf::Vector2i last;

        virtual void addButton(std::string text, std::function<void(void)>&& action);

        virtual void paintOn(sf::RenderTarget& window);

        virtual void keyPressed(sf::Event& event);
        virtual void mouseMoved(sf::Event& event);

        virtual ~TextButtonGroup();
    };

    class ScrollableTextButtonGroup : public TextButtonGroup {
    public:
        size_t center = 5;

        virtual void addButton(std::string text, std::function<void(void)>&& action);

        virtual void paintOn(sf::RenderTarget& window);

        virtual void keyPressed(sf::Event& event);
        virtual void mouseMoved(sf::Event& event);

        virtual void pack();
    };
}
#endif // _BASIC_SUBJECT_HPP_
