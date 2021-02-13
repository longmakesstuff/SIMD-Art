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
        auto m_position = sf::Vector2<fpt>(pos_x[i], pos_y[i]);
        auto m_speed = sf::Vector2<fpt>(v_x[i], v_y[i]);

        auto new_position = m_position + m_speed * dt + 0.5 * g_force / m[i] * pow(dt, 2);
        auto new_speed = (new_position - m_position) / dt;

        if(new_speed.x == new_speed.x) {
            v_x[i] = new_speed.x - new_speed.x * 0.025;
        }
        if(new_speed.y == new_speed.y) {
            v_y[i] = new_speed.y - new_speed.y * 0.025;
        }
        if (new_position.x < 0 || new_position.x > WINDOW_WIDTH) {
            v_x[i] = -v_x[i];
        }

        // Collision top or bottom
        if (new_position.y < 0 || new_position.y > WINDOW_HEIGHT) {
            v_y[i] = -v_y[i];
        }
        pos_x[i] = new_position.x;
        pos_y[i] = new_position.y;
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
                    std::cout << "Mouse pressed" << std::endl;
                    this->mouse_pressed = true;
                    fpt x = event.mouseMove.x;
                    fpt y = event.mouseMove.y;
                    pos_x[n] = x;
                    pos_y[n] = y;
                }
            }
            if(event.type == sf::Event::EventType::MouseButtonReleased) {
                if(this->mouse_pressed) {
                    std::cout << "Mouse released" << std::endl;
                    this->mouse_pressed = false;
                    fpt x = event.mouseMove.x;
                    fpt y = event.mouseMove.y;
                    pos_x[n] = x;
                    pos_y[n] = y;
                }
            }
        }

        if(mouse_pressed) {
            fpt x = event.mouseMove.x;
            fpt y = event.mouseMove.y;
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