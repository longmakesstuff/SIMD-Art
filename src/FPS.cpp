#include "simdart/FPS.h"

void FPS::update() {
    if (clock.getElapsedTime().asSeconds() >= 1.f) {
        fps = frame;
        frame = 0;
        clock.restart();
    }

    ++frame;
}