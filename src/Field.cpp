#include <random>
#include "simdart/Field.h"


Field::Field() {
    // Sanity check
    if (n % block_size != 0) {
        std::exit(1);
    }

    // Allocating memory
    pos_x = new fpt[n + 1];
    pos_y = new fpt[n + 1];
    v_x = new fpt[n + 1];
    v_y = new fpt[n + 1];
    masses = new fpt[n + 1];
    colors = new sf::Color[n];
    texture = new sf::Color[WINDOW_HEIGHT * WINDOW_WIDTH];
    vertices = new sf::Vertex[n];
    vertex_buffer.create(n);

    // Check memory's status
    if (!pos_x || !pos_y || !v_x || !v_y || !masses || !colors || !texture) {
        std::exit(1);
    }

    load_texture();

    // Initialise particles' position
    std::mt19937 rng{};
    std::uniform_real_distribution<fpt> pos(400.0, 600.0);
    // Initialize random position
    for (uint32_t i = 0; i < n; i++) {
        pos_x[i] = pos(rng);
        pos_y[i] = pos(rng);
        v_x[i] = 0.0;
        v_y[i] = 0.0;
        masses[i] = particle_mass;
        colors[i].g = 125;
        colors[i].b = 125;
    }

    // Position for mouse
    // We want mouse to dominates
    masses[n] = mouse_mass;
}

Field::~Field() {
    delete pos_x;
    delete pos_y;
    delete v_x;
    delete v_y;
    delete masses;
    delete colors;
    delete texture;
    delete vertices;
}

void Field::load_texture() {
    // Loading texture
    sf::Image image;
    if (!image.loadFromFile(texture_file)) {
        std::exit(1);
    }

    if (image.getSize().x != WINDOW_WIDTH || image.getSize().y != WINDOW_HEIGHT) {
        std::exit(1);
    }

    for (uint32_t y = 0; y < WINDOW_HEIGHT; y++) {
        for (uint32_t x = 0; x < WINDOW_WIDTH; x++) {
            texture[y * WINDOW_HEIGHT + x].r = image.getPixel(x, y).r;
            texture[y * WINDOW_HEIGHT + x].g = image.getPixel(x, y).g;
            texture[y * WINDOW_HEIGHT + x].b = image.getPixel(x, y).b;
        }
    }
}

void Field::simd_simulate() {
    TimeIt simulation("SIMD Simulation");

    // Some constants
    fpt dt_2 = dt * dt;
    simd_fpt dt_ = simd_set(dt);
    simd_fpt dt_2_ = simd_set(dt_2);

    // Loading mass, position of mouse
    simd_fpt mouse_pos_x_ = simd_set(pos_x[n]);
    simd_fpt mouse_pos_y_ = simd_set(pos_y[n]);
    simd_fpt mouse_mass_ = simd_set(masses[n]);

    constexpr uint32_t batch_size = 20;

#pragma omp parallel for
    for (uint32_t block = 0; block < n; block += block_size * batch_size) {
        for (uint32_t batch_idx = 0; batch_idx < batch_size; batch_idx += 1) {
            uint32_t k = block + block_size * batch_idx;

            // Loading current G-Force vectors
            simd_fpt g_force_x_ = simd_set(0.0);
            simd_fpt g_force_y_ = simd_set(0.0);
            simd_fpt new_position_x_ = simd_load(pos_x + k);
            simd_fpt new_position_y_ = simd_load(pos_y + k);
            // Loading current speed vectors
            simd_fpt v_x_ = simd_load(v_x + k);
            simd_fpt v_y_ = simd_load(v_y + k);
            // Loading current positions
            simd_fpt pos_x_ = simd_load(pos_x + k);
            simd_fpt pos_y_ = simd_load(pos_y + k);


            // Loading masses
            simd_fpt masses_ = simd_load(masses + k);

            if (mouse_pressed) {
                // Calculate distance
                simd_fpt diff_pos_x_ = mouse_pos_x_ - pos_x_;
                simd_fpt diff_pos_y_ = mouse_pos_y_ - pos_y_;

                simd_fpt diff_pos_x_2_ = diff_pos_x_ * diff_pos_x_;
                simd_fpt diff_pos_y_2_ = diff_pos_y_ * diff_pos_y_;

                simd_fpt distance_ = simd_sqrt(diff_pos_x_2_ + diff_pos_y_2_);
                distance_ = simd_max(distance_, max_distances_);
                simd_fpt distance_3_ = distance_ * distance_ * distance_;

                // Calculate g-force
                g_force_x_ = mouse_mass_ * (diff_pos_x_ / distance_3_) * masses_;
                g_force_y_ = mouse_mass_ * (diff_pos_y_ / distance_3_) * masses_;
            }

            /*
             * This one is quite cheating
             * I calculate if the sum of every position deltas equal to zero
             * if delta is equal to zero I will have to divide by zero to obtain new speed, hence
             * arithmetic exception. This works well enough for my case
             */
            simd_fpt new_position_x_s_ = pos_x_ + v_x_ * dt_ + c_0_5_ * g_force_x_ / masses_ * dt_2_;
            simd_fpt pos_diff_x = new_position_x_s_ - pos_x_;

            if (reduce_add_ps(pos_diff_x) != 0.0f) {
                simd_fpt new_position_y_s_ = pos_y_ + v_y_ * dt_ + c_0_5_ * g_force_y_ / masses_ * dt_2_;

                // Wall collision
                new_position_x_s_ = simd_min(new_position_x_s_, width);
                new_position_x_s_ = simd_max(new_position_x_s_, zeros);
                new_position_y_s_ = simd_min(new_position_y_s_, height);
                new_position_y_s_ = simd_max(new_position_y_s_, zeros);

                simd_fpt pos_diff_y = new_position_y_s_ - pos_y_;

                simd_fpt new_speed_x = pos_diff_x / dt_;
                new_speed_x = new_speed_x - new_speed_x * drag_;

                simd_fpt new_speed_y = pos_diff_y / dt_;
                new_speed_y = new_speed_y - new_speed_y * drag_;

                std::memcpy(v_x + k, (fpt *) &new_speed_x, block_size * sizeof(fpt));
                std::memcpy(pos_x + k, (fpt *) &new_position_x_s_, block_size * sizeof(fpt));

                std::memcpy(v_y + k, (fpt *) &new_speed_y, block_size * sizeof(fpt));
                std::memcpy(pos_y + k, (fpt *) &new_position_y_s_, block_size * sizeof(fpt));
            }
        }
    }

#pragma omp parallel for
    for (uint32_t i = 0; i < n; i++) {
        vertices[i].position.x = pos_x[i];
        vertices[i].position.y = pos_y[i];
        if (!texture_mapping) {
            // Update graphic with new data
            colors[i].r = (255 - norm(sf::Vector2<fpt>{v_x[i], v_y[i]}));
            vertices[i].color = colors[i];
        } else {
            vertices[i].color = texture[std::min((uint32_t) pos_y[i] * WINDOW_HEIGHT + (uint32_t) pos_x[i], WINDOW_HEIGHT * WINDOW_HEIGHT)];
        }
    }
    simulation.end();
}

void Field::naive_simulate() {
    TimeIt simulation("Naive Simulation");
    fpt delta_t_2 = pow(dt, 2);

    // Iterate each particle
#pragma omp parallel for
    for (uint32_t i = 0; i < n; i++) {
        fpt g_force_x = 0;
        fpt g_force_y = 0;

        if (mouse_pressed) {
            // Calculate distance
            fpt diff_x = pos_x[n] - pos_x[i];
            fpt diff_y = pos_y[n] - pos_y[i];

            fpt distance = std::sqrt(diff_x * diff_x + diff_y * diff_y);
            distance = MAX(minimal_distance, distance);

            // Calculate G-Forces
            g_force_x = masses[n] * (diff_x / std::pow(distance, 3)) * masses[i];
            g_force_y = masses[n] * (diff_y / std::pow(distance, 3)) * masses[i];

        }

        // Calculate new position
        fpt new_position_x = pos_x[i] + v_x[i] * dt + 0.5 * g_force_x / masses[i] * delta_t_2;
        fpt new_position_y = pos_y[i] + v_y[i] * dt + 0.5 * g_force_y / masses[i] * delta_t_2;

        // Update positions and speed
        if (new_position_x != pos_x[i]) {
            auto new_speed_x = (new_position_x - pos_x[i]) / dt;
            v_x[i] = new_speed_x - new_speed_x * drag_coefficient;
            pos_x[i] = new_position_x;
        }
        if (new_position_y != pos_y[i]) {
            auto new_speed_y = (new_position_y - pos_y[i]) / dt;
            v_y[i] = new_speed_y - new_speed_y * drag_coefficient;
            pos_y[i] = new_position_y;
        }

        // Wall collision detect
        if (pos_x[i] < 0) {
            v_x[i] = -v_x[i];
            pos_x[i] = 0;
        } else if (pos_x[i] > WINDOW_WIDTH) {
            v_x[i] = -v_x[i];
            pos_x[i] = WINDOW_WIDTH;
        }
        if (pos_y[i] < 0) {
            v_y[i] = -v_y[i];
            pos_y[i] = 0;
        } else if (pos_y[i] > WINDOW_HEIGHT) {
            v_y[i] = -v_y[i];
            pos_y[i] = WINDOW_HEIGHT;
        }

        // Update graphic
        colors[i].r = clamp(norm(sf::Vector2<fpt>{v_x[i], v_y[i]}), 0, 255);
        vertices[i].position = sf::Vector2f{new_position_x, new_position_y};
        if (!texture_mapping) {
            vertices[i].color = colors[i];
        } else {
            vertices[i].color = texture[MIN((uint32_t) pos_y[i] * WINDOW_HEIGHT + (uint32_t) pos_x[i],
                                            WINDOW_HEIGHT * WINDOW_HEIGHT)];
        }

    }
    simulation.end();
}

void Field::run(sf::RenderWindow *window) {
    if (mouse_pressed) {
        fpt x = sf::Mouse::getPosition().x - window->getPosition().x;
        fpt y = sf::Mouse::getPosition().y - window->getPosition().y;
        pos_x[n] = x;
        pos_y[n] = y;
    }

    simd_simulate();
    //naive_simulate();
    TimeIt rendering("Rendering");
    vertex_buffer.update(vertices);
    window->draw(vertex_buffer);
    dt = clock.restart().asSeconds();
    window->display();
    rendering.end();
}

bool Field::isMousePressed() const {
    return mouse_pressed;
}

void Field::setMousePressed(bool mousePressed) {
    mouse_pressed = mousePressed;
}

fpt Field::getMinimalDistance() const {
    return minimal_distance;
}

void Field::setMinimalDistance(fpt minimalDistance) {
    minimal_distance = minimalDistance;
}

bool Field::isTextureMapping() const {
    return texture_mapping;
}

void Field::setTextureMapping(bool textureMapping) {
    texture_mapping = textureMapping;
}

fpt Field::getDragCoefficient() const {
    return drag_coefficient;
}

void Field::setDragCoefficient(fpt dragCoefficient) {
    drag_coefficient = dragCoefficient;
}

uint32_t Field::getMouseMass() const {
    return mouse_mass;
}

void Field::setMouseMass(uint32_t mouseMass) {
    mouse_mass = mouseMass;
}

uint32_t Field::getParticleMass() const {
    return particle_mass;
}

void Field::setParticleMass(uint32_t particleMass) {
    particle_mass = particleMass;
}

const std::string &Field::getTextureFile() const {
    return texture_file;
}

void Field::setTextureFile(const std::string &textureFile) {
    texture_file = textureFile;
    load_texture();
}
