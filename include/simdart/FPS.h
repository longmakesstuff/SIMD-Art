#ifndef CMAKE_EXAMPLE_FPS_H
#define CMAKE_EXAMPLE_FPS_H

#include <SFML/System/Clock.hpp>

class FPS {
public:
    FPS() : frame(0), fps(0) {}

    [[nodiscard]] unsigned int getFPS() const { return fps; }

private:
    unsigned long long frame;
    unsigned long long fps;
    sf::Clock clock{};

public:
    void update();
};

#endif
