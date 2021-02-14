#include "Field.h"


Field::Field(sf::RenderWindow *window, sf::Font *font) : window(window), font(font) {
    pos_x = new fpt[n + 1];
    pos_y = new fpt[n + 1];
    v_x = new fpt[n + 1];
    v_y = new fpt[n + 1];
    masses = new fpt[n + 1];
    colors = new sf::Color[n];
    texture = new sf::Color[WINDOW_HEIGHT * WINDOW_WIDTH];

    v_buffer.create(n);
    v_arr = new sf::Vertex[n];

    if (n % block_size != 0) {
        LOG_ERROR("Illegal count of particles! Must be a divider of 8. Exit now!")
        std::exit(1);
    }
    if (!pos_x || !pos_y || !v_x || !v_y || !masses || !colors || !texture) {
        LOG_ERROR("Cannot allocate enough memory. Exit now!")
        std::exit(1);
    }
    // Loading texture
    sf::Image image;
    if (!image.loadFromFile(texture_file)) {
        LOG_ERROR("Can not load heart.jpg. Exit now")
        std::exit(1);
    }

    for (int32_t y = 0; y < WINDOW_HEIGHT; y++) {
        for (int32_t x = 0; x < WINDOW_WIDTH; x++) {
            texture[y * WINDOW_HEIGHT + x].r = image.getPixel(x, y).r;
            texture[y * WINDOW_HEIGHT + x].g = image.getPixel(x, y).g;
            texture[y * WINDOW_HEIGHT + x].b = image.getPixel(x, y).b;
        }
    }

    std::mt19937 rng;
    std::uniform_real_distribution<fpt> pos(900.0, 1000.0);
    // Initialize random position
    for (uint32_t i = 0; i < n; i++) {
        pos_x[i] = pos(rng);
        pos_y[i] = pos(rng);
        v_x[i] = 0.0;
        v_y[i] = 0.0;
        masses[i] = 10000;
        colors[i].g = 255;
        colors[i].b = 255;
    }

    // Position for mouse
    // We want mouse to dominates
    masses[n] = mouse_mass;
}

void Field::simd_simulate() {
    TimeIt simulation("SIMD Simulation");

    // Delta^2
    fpt dt_2 = dt * dt;
    simd_fpt dt_ = simd_set(dt);
    simd_fpt dt_2_ = simd_set(dt_2);
    simd_fpt c_0_5_ = simd_set(0.5);


    // Maximal distance
    simd_fpt max_distances_ = simd_set(50.0);

    // Loading mass, position of mouses
    simd_fpt mouse_pos_x_ = simd_set(pos_x[n]);
    simd_fpt mouse_pos_y_ = simd_set(pos_y[n]);
    simd_fpt mouse_mass_ = simd_set(masses[n]);

#pragma omp parallel for
    for (uint32_t k = 0; k < n; k += block_size) {
        // Loading current G-Force vectors
        simd_fpt g_force_x_ = simd_set(0.0);
        simd_fpt g_force_y_ = simd_set(0.0);
        simd_fpt new_position_x_ = simd_load(pos_x + k);
        simd_fpt new_position_y_ = simd_load(pos_y + k);

        // Loading current positions
        simd_fpt pos_x_ = simd_load(pos_x + k);
        simd_fpt pos_y_ = simd_load(pos_y + k);

        // Loading current speed vectors
        simd_fpt v_x_ = simd_load(v_x + k);
        simd_fpt v_y_ = simd_load(v_y + k);

        // Loading masses
        simd_fpt masses_ = simd_load(masses + k);

        fpt g_force_x[block_size]{0, 0, 0, 0, 0, 0, 0, 0};
        fpt g_force_y[block_size]{0, 0, 0, 0, 0, 0, 0, 0};

        if (mouse_pressed) {
            // Calculate distance
            simd_fpt diff_pos_x_ = simd_sub(mouse_pos_x_, pos_x_);
            simd_fpt diff_pos_y_ = simd_sub(mouse_pos_y_, pos_y_);

            simd_fpt diff_pos_x_2_ = simd_mul(diff_pos_x_, diff_pos_x_);
            simd_fpt diff_pos_y_2_ = simd_mul(diff_pos_y_, diff_pos_y_);

            simd_fpt distance_ = simd_sqrt(_mm256_add_ps(diff_pos_x_2_, diff_pos_y_2_));
            distance_ = simd_max(distance_, max_distances_);
            simd_fpt distance_3_ = simd_mul(simd_mul(distance_, distance_), distance_);

            // Calculate g-force
            g_force_x_ = simd_mul(mouse_mass_, simd_div(diff_pos_x_, distance_3_));
            g_force_y_ = simd_mul(mouse_mass_, simd_div(diff_pos_y_, distance_3_));

            g_force_x_ = simd_mul(g_force_x_, masses_);
            g_force_y_ = simd_mul(g_force_y_, masses_);

            std::memcpy(g_force_x, (fpt * )&g_force_x_, block_size * sizeof(fpt));
            std::memcpy(g_force_y, (fpt * )&g_force_y_, block_size * sizeof(fpt));
        }

        // Calculating new positions
        //new_position_x_ = simd_div(simd_mul(c_0_5_, g_force_x_), simd_mul(masses_, dt_2_));
        //new_position_x_ = simd_add(new_position_x_, simd_mul(v_x_, dt_));
        //new_position_x_ = simd_add(new_position_x_, pos_x_);

        //new_position_y_ = simd_div(simd_mul(c_0_5_, g_force_y_), simd_mul(masses_, dt_2_));
        //new_position_y_ = simd_add(new_position_y_, simd_mul(v_y_, dt_));
        //new_position_y_ = simd_add(new_position_y_, pos_y_);

        //fpt * new_position_x_s = (fpt *)&new_position_x_;
        //fpt * new_position_y_s = (fpt *)&new_position_y_;

        for (uint32_t i = k; i < k + block_size; i++) {
            fpt new_position_x = pos_x[i] + v_x[i] * dt + 0.5 * g_force_x[i - k] / masses[i] * dt_2;
            fpt new_position_y = pos_y[i] + v_y[i] * dt + 0.5 * g_force_y[i - k] / masses[i] * dt_2;
            //fpt new_position_x = new_position_x_s[i - k];
            //fpt new_position_y = new_position_y_s[i - k];

            if (new_position_x != pos_x[i]) {
                auto new_speed_x = (new_position_x - pos_x[i]) / dt;
                v_x[i] = new_speed_x - new_speed_x * 0.025;
                pos_x[i] = new_position_x;
            }
            if (new_position_y != pos_y[i]) {
                auto new_speed_y = (new_position_y - pos_y[i]) / dt;
                v_y[i] = new_speed_y - new_speed_y * 0.025;
                pos_y[i] = new_position_y;
            }
            if (new_position_x < 0) {
                v_x[i] = -v_x[i];
                pos_x[i] = 0;
            } else if (new_position_x > WINDOW_WIDTH) {
                v_x[i] = -v_x[i];
                pos_x[i] = WINDOW_WIDTH;
            }
            if (new_position_y < 0) {
                v_y[i] = -v_y[i];
                pos_y[i] = 0;
            } else if (new_position_y > WINDOW_HEIGHT) {
                v_y[i] = -v_y[i];
                pos_y[i] = WINDOW_HEIGHT;
            }

            colors[i].r = (255 - norm(sf::Vector2<fpt>{v_x[i], v_y[i]}));

            v_arr[i].position = sf::Vector2f{new_position_x, new_position_y};
            if (!texture_mapping) {
                v_arr[i].color = colors[i];
            } else {
                v_arr[i].color = texture[MIN((uint32_t) pos_y[i] * WINDOW_HEIGHT + (uint32_t) pos_x[i],
                                             WINDOW_HEIGHT * WINDOW_HEIGHT)];
            }

        }
    }
    simulation.end();
}

void Field::naive_simulate() {
    TimeIt simulation("Simulation");
    fpt delta_t_2 = pow(dt, 2);

#pragma omp parallel for
    for (uint32_t i = 0; i < n; i++) {
        fpt g_force_x = 0;
        fpt g_force_y = 0;

        if (mouse_pressed) {
            // Calculate distance
            fpt diff_x = pos_x[n] - pos_x[i];
            fpt diff_y = pos_y[n] - pos_y[i];

            fpt distance = std::sqrt(diff_x * diff_x + diff_y * diff_y);
            distance = MAX(50.0, distance);

            // Calculate G-Forces
            g_force_x = masses[n] * (diff_x / std::pow(distance, 3)) * masses[i];
            g_force_y = masses[n] * (diff_y / std::pow(distance, 3)) * masses[i];

        }

        fpt new_position_x = pos_x[i] + v_x[i] * dt + 0.5 * g_force_x / masses[i] * delta_t_2;
        fpt new_position_y = pos_y[i] + v_y[i] * dt + 0.5 * g_force_y / masses[i] * delta_t_2;

        if (new_position_x != pos_x[i]) {
            auto new_speed_x = (new_position_x - pos_x[i]) / dt;
            v_x[i] = new_speed_x - new_speed_x * 0.025;
            pos_x[i] = new_position_x;
        }
        if (new_position_y != pos_y[i]) {
            auto new_speed_y = (new_position_y - pos_y[i]) / dt;
            v_y[i] = new_speed_y - new_speed_y * 0.025;
            pos_y[i] = new_position_y;
        }
        if (new_position_x < 0) {
            v_x[i] = -v_x[i];
            pos_x[i] = 0;
        } else if (new_position_x > WINDOW_WIDTH) {
            v_x[i] = -v_x[i];
            pos_x[i] = WINDOW_WIDTH;
        }
        if (new_position_y < 0) {
            v_y[i] = -v_y[i];
            pos_y[i] = 0;
        } else if (new_position_y > WINDOW_HEIGHT) {
            v_y[i] = -v_y[i];
            pos_y[i] = WINDOW_HEIGHT;
        }

        colors[i].r = (255 - norm(sf::Vector2<fpt>{v_x[i], v_y[i]}));

        v_arr[i].position = sf::Vector2f{new_position_x, new_position_y};
        if (!texture_mapping) {
            v_arr[i].color = colors[i];
        } else {
            v_arr[i].color = texture[MIN((uint32_t) pos_y[i] * WINDOW_HEIGHT + (uint32_t) pos_x[i],
                                         WINDOW_HEIGHT * WINDOW_HEIGHT)];
        }

    }
    simulation.end();
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

        simd_simulate();
        //naive_simulate();
        window->clear(sf::Color::White);

        TimeIt rendering("Rendering");
        v_buffer.update(v_arr);
        //window->draw(pixels);
        window->draw(v_buffer);
        info_text();
        window->display();
        dt = clock.restart().asSeconds();
        rendering.end();
    }
}

void Field::info_text() {
    std::stringstream ss = std::stringstream{};
    fps.update();
    ss << "FPS: " << fps.getFPS();
    sf::Text fpsText{ss.str(), *font, 10};
    fpsText.setPosition(10, 10);
    fpsText.setFillColor(sf::Color::White);
    window->draw(fpsText);
}
