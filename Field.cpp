#include "Field.h"


Field::Field(sf::RenderWindow *window, sf::Font *font) : window(window), font(font) {
    pos_x = new fpt[n + 1];
    pos_y = new fpt[n + 1];
    v_x = new fpt[n + 1];
    v_y = new fpt[n + 1];
    g_force_x = new fpt[n + 1];
    g_force_y = new fpt[n + 1];
    masses = new fpt[n + 1];
    colors = new sf::Color[n];
    texture = new sf::Color[WINDOW_HEIGHT * WINDOW_WIDTH];

    v_buffer.create(n);
    v_arr = new sf::Vertex[n];

    if (n % block_size != 0) {
        LOG_ERROR("Illegal count of particles! Must be a divider of 8. Exit now!")
        std::exit(1);
    }
    if (!pos_x || !pos_y || !v_x || !v_y || !g_force_x || !g_force_y || !masses || !colors || !texture) {
        LOG_ERROR("Cannot allocate enough memory. Exit now!")
        std::exit(1);
    }
    // Loading texture
    sf::Image image;
    if (!image.loadFromFile("texture.jpg")) {
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

    std::mt19937 rng;
    std::uniform_real_distribution<fpt> pos(100.0, 200.0);
    // Initialize random position
    for (uint32_t i = 0; i < n; i++) {
        pos_x[i] = pos(rng);
        pos_y[i] = pos(rng);
        v_x[i] = 0.0;
        v_y[i] = 0.0;
        g_force_x[i] = 0.0;
        g_force_y[i] = 0.0;
        masses[i] = 10000;
        colors[i].g = 255;
        colors[i].b = 255;
    }

    // Position for mouse
    // We want mouse to dominates
    masses[n] = 10000000;
}

void Field::intrinsic_simulate() {
    TimeIt simulation("SIMD Simulation");

    // Delta^2
    fpt dt_2 = dt * dt;

    // Maximal distance
    __m256 max_distances_ = _mm256_set1_ps(50.0);

    // Loading mass, position of mouses
    __m256 mouse_pos_x_ = _mm256_set1_ps(pos_x[n]);
    __m256 mouse_pos_y_ = _mm256_set1_ps(pos_y[n]);
    __m256 mouse_mass_ = _mm256_set1_ps(masses[n]);

#pragma omp parallel for
    for (uint32_t k = 0; k < n; k += block_size) {
        if (mouse_pressed) {
            // Loading current positions
            __m256 pos_x_ = _mm256_load_ps(pos_x + k);
            __m256 pos_y_ = _mm256_load_ps(pos_y + k);

            // Loading current speed vectors
            __m256 v_x_ = _mm256_load(v_x + k);
            __m256 v_y_ = _mm256_load(v_y + k);

            // Loading current G-Force vectors
            __m256 g_force_x_ = _mm256_load(g_force_x + k);
            __m256 g_force_y_ = _mm256_load(g_force_y + k);

            // Loading masses
            __m256 masses_ = _mm256_load(masses + k);

            // Calculate distance
            __m256 diff_pos_x_ = _mm256_sub_ps(mouse_pos_x_, pos_x_);
            __m256 diff_pos_y_ = _mm256_sub_ps(mouse_pos_y_, pos_y_);

            __m256 diff_pos_x_2_ = _mm256_mul_ps(diff_pos_x_, diff_pos_x_);
            __m256 diff_pos_y_2_ = _mm256_mul_ps(diff_pos_y_, diff_pos_y_);

            __m256 distance_ = _mm256_sqrt_ps(_mm256_add_ps(diff_pos_x_2_, diff_pos_y_2_));
            distance_ = _mm256_max_ps(distance_, max_distances_);
            __m256 distance_3_ = _mm256_mul_ps(_mm256_mul_ps(distance_, distance_), distance_);

            // Calculate g-force
            g_force_x_ = _mm256_mul_ps(mouse_mass_, _mm256_div_ps(diff_pos_x_, distance_3_));
            g_force_y_ = _mm256_mul_ps(mouse_mass_, _mm256_div_ps(diff_pos_y_, distance_3_));

            g_force_x_ = _mm256_mul_ps(g_force_x_, masses_);
            g_force_y_ = _mm256_mul_ps(g_force_y_, masses_);

            // Copy memory back to array
            std::memcpy(g_force_x + k, (fpt *) &g_force_x_, block_size * sizeof(fpt));
            std::memcpy(g_force_y + k, (fpt *) &g_force_y_, block_size * sizeof(fpt));
        }

        for (uint32_t i = k; i < k + block_size; i++) {
            fpt new_position_x = pos_x[i] + v_x[i] * dt + 0.5 * g_force_x[i] / masses[i] * dt_2;
            fpt new_position_y = pos_y[i] + v_y[i] * dt + 0.5 * g_force_y[i] / masses[i] * dt_2;

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

void Field::simulate() {
    TimeIt simulation("Simulation");
    fpt delta_t_2 = pow(dt, 2);

#pragma omp parallel for
    for (uint32_t i = 0; i < n; i++) {
        if (mouse_pressed) {
            g_force_x[i] = 0;
            g_force_y[i] = 0;

            fpt diff_x = pos_x[n] - pos_x[i];
            fpt diff_y = pos_y[n] - pos_y[i];

            fpt distance = std::sqrt(diff_x * diff_x + diff_y * diff_y);
            distance = MAX(50.0, distance);

            g_force_x[i] = masses[n] * (diff_x / std::pow(distance, 3)) * masses[i];
            g_force_y[i] = masses[n] * (diff_y / std::pow(distance, 3)) * masses[i];
        }

        fpt new_position_x = pos_x[i] + v_x[i] * dt + 0.5 * g_force_x[i] / masses[i] * delta_t_2;
        fpt new_position_y = pos_y[i] + v_y[i] * dt + 0.5 * g_force_y[i] / masses[i] * delta_t_2;

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

        //intrinsic_simulate();
        simulate();
        window->clear(sf::Color::Black);

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
