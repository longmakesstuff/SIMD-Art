#include "Field.h"

Field::Field(sf::RenderWindow *window, sf::Font *font) : window(window), font(font) {
    for(uint32_t i = 0; i <= n; i++) {
        particles.emplace_back(i);
    }
}

void Field::simulate() {
#pragma omp parallel for
    for (uint32_t i = 0; i < n; i++) {
        particles[i].summarize_g_force(std::vector<Particle>{particles.back()}, this->mouse_pressed);
    }

#pragma omp parallel for
    for (uint32_t i = 0; i < n; i++) {
        sf::Vector2<fpt> g_force{g_force_x[i], g_force_y[i]};

        auto new_position_x = pos_x[i] + v_x[i] * dt + 0.5 * g_force_x[i] / m[i] * pow(dt, 2);
        auto new_position_y = pos_y[i] + v_y[i] * dt + 0.5 * g_force_y[i] / m[i] * pow(dt, 2);
        auto new_speed_x = (new_position_x - pos_x[i]) / dt;
        auto new_speed_y = (new_position_y - pos_y[i]) / dt;

        if(new_speed_x == new_speed_x) {
            v_x[i] = new_speed_x - new_speed_x * 0.025;
        }
        if(new_speed_y == new_speed_y) {
            v_y[i] = new_speed_y - new_speed_y * 0.025;
        }
        if (new_position_x < 0 || new_position_x > WINDOW_WIDTH) {
            v_x[i] = -v_x[i];
        }

        if (new_position_y < 0 || new_position_y > WINDOW_HEIGHT) {
            v_y[i] = -v_y[i];
        }
        pos_x[i] = new_position_x;
        pos_y[i] = new_position_y;
    }

}

void Field::info_text() {
    std::stringstream ss = std::stringstream{};
    fps.update();
    ss << "FPS: " << fps.getFPS();
    sf::Text fpsText{ss.str(), *font, 10};
    fpsText.setPosition(10, 25);
    fpsText.setFillColor(sf::Color::White);
    window->draw(fpsText);
}


void Field::run() {
    while (window->isOpen()) {
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::EventType::Closed) {
                std::exit(0);
            }
            if (event.type == sf::Event::EventType::MouseButtonPressed) {
                if(!this->mouse_pressed) {
                    this->mouse_pressed = true;
                }
            }
            if(event.type == sf::Event::EventType::MouseButtonReleased) {
                if(this->mouse_pressed) {
                    this->mouse_pressed = false;
                }
            }
        }

        if(mouse_pressed) {
            fpt x = sf::Mouse::getPosition().x - window->getPosition().x;
            fpt y = sf::Mouse::getPosition().y - window->getPosition().y;
            pos_x[n] = x;
            pos_y[n] = y;
        }

        simulate();

        window->clear(sf::Color::Black);
        for (uint32_t i = 0; i < n; i++) {
            particles[i].draw(*window);
        }

        info_text();

        window->display();
        dt = clock.restart().asSeconds();
    }
}