#ifndef GRAVITATION_GUI_H
#define GRAVITATION_GUI_H
#include "Field.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <vector>
#include <omp.h>
#include <cstring>
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

public:
    GUI(sf::RenderWindow *window, sf::Font *font);

    void main_loop();
};


#endif
