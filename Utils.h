#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <ostream>
#include <random>
#include <chrono>
#include <iostream>
#include <iterator>
#include <immintrin.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define LOG_DEBUG(x) do { std::cout << "[DEBUG] " << __FILE__ << ":" << __LINE__ << ":" << __func__ << "() " << x << std::endl; } while (0);

#define LOG_ERROR(x) do { std::cerr << "[ERROR] " << __FILE__ << ":" << __LINE__ << ":" << __func__ << "() " << x << std::endl; } while (0);

#define LOG_WARNING(x) do { std::cerr << "[WARNING] " << __FILE__ << ":" << __LINE__ << ":" << __func__ << "() " << x << std::endl; } while (0);

#define LOG_INFO(x) do { std::cout << "[INFO] " << __FILE__ << ":" << __LINE__ << ":" << __func__ << "() " << x << std::endl; } while (0);

#define NOW std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch()).count()

typedef float fpt;

constexpr uint32_t WINDOW_WIDTH = 1000;
constexpr uint32_t WINDOW_HEIGHT = 1000;

class FPS {
public:
    FPS() : frame(0), fps(0) {}

    [[nodiscard]] unsigned int getFPS() const { return fps; }

private:
    unsigned long long frame;
    unsigned long long fps;
    sf::Clock clock;

public:
    void update();
};

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator &g) {
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

inline fpt norm(const sf::Vector2<fpt> &vec) {
    return std::sqrt(std::pow(vec.x, 2.0f) + std::pow(vec.y, 2.0f));
}

inline std::ostream &operator<<(std::ostream &os, const sf::Vector2<fpt> &vector) {
    os << "[x = " << vector.x << ", y = " << vector.y << "]";
    return os;
}

inline sf::Vector2<fpt> operator+(const sf::Vector2<fpt> &lhs, fpt scalar) {
    sf::Vector2<fpt> ret(lhs);
    ret.x += scalar;
    ret.y += scalar;
    return ret;
}

inline sf::Vector2<fpt> operator-(const sf::Vector2<fpt> &lhs, fpt scalar) {
    sf::Vector2<fpt> ret(lhs);
    ret.x -= scalar;
    ret.y -= scalar;
    return ret;
}

inline sf::Vector2<fpt> operator/(const sf::Vector2<fpt> &lhs, fpt scalar) {
    sf::Vector2<fpt> ret(lhs);
    ret.x /= scalar;
    ret.y /= scalar;
    return ret;
}

inline sf::Vector2<fpt> operator*(const sf::Vector2<fpt> &lhs, fpt scalar) {
    sf::Vector2<fpt> ret(lhs);
    ret.x *= scalar;
    ret.y *= scalar;
    return ret;
}

inline fpt clamp(fpt value, fpt min, fpt max) {
    float result;
    if (value > max)
        result = max;
    else if (value < min)
        result = min;
    else
        result = value;
    return result;
}