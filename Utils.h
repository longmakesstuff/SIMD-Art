#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <ostream>
#include <random>
#include <chrono>
#include <iostream>
#include  <iterator>

#define PI 3.14159265
#define DEG2RAD(x) ((x)*PI/180)
#define RAD2DEG(x) ((x)*180/PI)
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define CENTER sf::Vector2f{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}
#define UGC 0.0000006674f
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


#define LOG_DEBUG(x) do { std::cout << "[DEBUG] " << __FILE__ << ":" << __LINE__ << ":" << __func__ << "() " << x << std::endl; } while (0);

#define LOG_ERROR(x) do { std::cerr << "[ERROR] " << __FILE__ << ":" << __LINE__ << ":" << __func__ << "() " << x << std::endl; } while (0);

#define LOG_WARNING(x) do { std::cerr << "[WARNING] " << __FILE__ << ":" << __LINE__ << ":" << __func__ << "() " << x << std::endl; } while (0);

#define LOG_INFO(x) do { std::cout << "[INFO] " << __FILE__ << ":" << __LINE__ << ":" << __func__ << "() " << x << std::endl; } while (0);

#define NOW std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch()).count()

typedef double fpt;

class FPS {
public:
    FPS() : frame(0), fps(0) {}

    [[nodiscard]] unsigned int getFPS() const { return fps; }

private:
    unsigned long long frame;
    unsigned long long fps;
    sf::Clock clock;

public:
    void update() {
        if (clock.getElapsedTime().asSeconds() >= 1.f) {
            fps = frame;
            frame = 0;
            clock.restart();
        }

        ++frame;
    }
};

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

fpt norm(const sf::Vector2<fpt> &vec);

std::ostream &operator<<(std::ostream &os, const sf::Vector2<fpt> &vector);

sf::Vector2<fpt> operator+(const sf::Vector2<fpt> &lhs, fpt scalar);

sf::Vector2<fpt> operator-(const sf::Vector2<fpt> &lhs, fpt scalar);

sf::Vector2<fpt> operator/(const sf::Vector2<fpt> &lhs, fpt scalar);

sf::Vector2<fpt> operator*(const sf::Vector2<fpt> &lhs, fpt scalar);