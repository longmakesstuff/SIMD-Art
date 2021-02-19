#pragma once

#include <cmath>
#include <fmt/core.h>
#include <SFML/System.hpp>

typedef float fpt;

constexpr uint32_t WINDOW_WIDTH = 1000;
constexpr uint32_t WINDOW_HEIGHT = 1000;

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

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
    if (value > max) {
        result = max;
    } else if (value < min) {
        result = min;
    } else {
        result = value;
    }
    return result;
}
