#include "GUI.h"


GUI::GUI(sf::RenderWindow *window, sf::Font *font) : window(window), font(font) {}

void GUI::info_text() {
    std::stringstream ss = std::stringstream{};
    fps.update();
    ss << "FPS: " << fps.getFPS();
    sf::Text fpsText{ss.str(), *font, 10};
    fpsText.setPosition(10, 10);
    fpsText.setFillColor(sf::Color::Black);
    window->draw(fpsText);
}

void GUI::main_loop() {
    while(window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::EventType::Closed) {
                std::exit(0);
            }
            if (event.type == sf::Event::EventType::MouseButtonPressed) {
                if(!field.isMousePressed()) {
                    field.setMousePressed(true);
                }
            }
            if (event.type == sf::Event::EventType::MouseButtonReleased) {
                if(field.isMousePressed()) {
                    field.setMousePressed(false);
                }
            }
        }
        this->field.run(this->window);
        info_text();
    }
}
