#ifndef GRAVITATION_GUI_H
#define GRAVITATION_GUI_H
#include "Field.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <vector>
#include <omp.h>
#include <cstring>
#include <TGUI/TGUI.hpp>
#include <array>
#include "Utils.h"
#include "TimeIt.h"
#include "Simd.h"

class GUI {
private:
    sf::RenderWindow *window;
    sf::Font *font;
    FPS fps;
    sf::Clock clock;
    Field field;
    void info_text();
    tgui::GuiSFML * widgets;
    tgui::Label::Ptr minimal_distance_label;
    tgui::Slider::Ptr minimal_distance;
    tgui::CheckBox::Ptr texture_mapping;
public:
    GUI(sf::RenderWindow *window, sf::Font *font, tgui::GuiSFML *widgets);

    void main_loop();
};


#endif
