#include "Field.h"

Field::Field(sf::RenderWindow *window, sf::Font *font) : window(window), font(font) {

    sf::Image image;
    if(!image.loadFromFile("texture.jpg")) {
        LOG_ERROR("Can not load texture.jpg. Exit now")
        std::exit(1);
    }

    for (int32_t y = 0; y < WINDOW_HEIGHT; y++) {
        for (int32_t x = 0; x < WINDOW_WIDTH; x++) {
            texture[y * WINDOW_HEIGHT + x].r = image.getPixel(x, y).r;
            texture[y * WINDOW_HEIGHT + x].g = image.getPixel(x, y).g;
            texture[y * WINDOW_HEIGHT + x].b = image.getPixel(x, y).b;
        }
    }
}

void Field::simulate() {
    if(mouse_pressed) {
#pragma omp parallel for
        for (uint32_t i = 0; i < n; i++) {
            sf::Vector2<fpt> g_force(0, 0);
            sf::Vector2<fpt> this_p(pos_x[i], pos_y[i]);
            fpt this_m = m[i];

            fpt other_m = m[n];
            sf::Vector2<fpt> other_p(pos_x[n], pos_y[n]);
            sf::Vector2<fpt> direction = other_p - this_p;
            fpt distance = MAX(100.0, norm(direction));
            g_force += other_m * (direction / std::pow(distance, 3));

            g_force *= this_m * UGC;
            g_force_x[i] = g_force.x;
            g_force_y[i] = g_force.y;
        }
    }

#pragma omp parallel for
    for (uint32_t i = 0; i < n; i++) {
        auto new_position_x = pos_x[i] + v_x[i] * dt + 0.5 * g_force_x[i] / m[i] * pow(dt, 2);
        auto new_position_y = pos_y[i] + v_y[i] * dt + 0.5 * g_force_y[i] / m[i] * pow(dt, 2);
        if(new_position_x != pos_x[i]) {
            auto new_speed_x = (new_position_x - pos_x[i]) / dt;
            v_x[i] = new_speed_x - new_speed_x * 0.025;
            pos_x[i] = new_position_x;
        }
        if(new_position_y != pos_y[i]) {
            auto new_speed_y = (new_position_y - pos_y[i]) / dt;
            v_y[i] = new_speed_y - new_speed_y * 0.025;
            pos_y[i] = new_position_y;
        }
        if (new_position_x < 0 || new_position_x > WINDOW_WIDTH) {
            v_x[i] = -v_x[i];
        }
        if (new_position_y < 0 || new_position_y > WINDOW_HEIGHT) {
            v_y[i] = -v_y[i];
        }
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
                if (!this->mouse_pressed) {
                    this->mouse_pressed = true;
                }
            }
            if (event.type == sf::Event::EventType::MouseButtonReleased) {
                if (this->mouse_pressed) {
                    this->mouse_pressed = false;
                }
            }
        }

        if (mouse_pressed) {
            fpt x = sf::Mouse::getPosition().x - window->getPosition().x;
            fpt y = sf::Mouse::getPosition().y - window->getPosition().y;
            pos_x[n] = x;
            pos_y[n] = y;
        }

        simulate();
        window->clear(sf::Color::Black);

        if(!texture_mapping) {
            for (uint32_t i = 0; i < n; i++) {
                sf::CircleShape circle(1);
                colors[i].r = (255 - norm(sf::Vector2<fpt>{v_x[i], v_y[i]}));
                circle.setFillColor(colors[i]);
                circle.setPosition(pos_x[i], pos_y[i]);
                window->draw(circle);
            }
        }else {
            for (uint32_t i = 0; i < n; i++) {
                sf::CircleShape circle(1);
                uint32_t color_pos = (uint32_t )pos_y[i] * WINDOW_HEIGHT + (uint32_t)pos_x[i];
                circle.setFillColor(texture[MIN(color_pos, WINDOW_HEIGHT * WINDOW_WIDTH)]);
                circle.setPosition(pos_x[i], pos_y[i]);
                window->draw(circle);
            }
        }

        info_text();

        window->display();
        dt = clock.restart().asSeconds();
    }
}