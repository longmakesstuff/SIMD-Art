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
    tgui::CheckBox::Ptr texture_mapping;
    tgui::Label::Ptr minimal_distance_label;
    tgui::Slider::Ptr minimal_distance;
    tgui::Label::Ptr drag_coefficient_label;
    tgui::Slider::Ptr drag_coefficient;
    tgui::Label::Ptr mouse_mass_label;
    tgui::Slider::Ptr mouse_mass;
    tgui::Label::Ptr particles_mass_label;
    tgui::Slider::Ptr particles_mass;
public:
    GUI(sf::RenderWindow *window, sf::Font *font, tgui::GuiSFML *widgets);

    void main_loop();
};


#endif
