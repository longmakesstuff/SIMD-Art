#include "Utils.h"

std::ostream &operator<<(std::ostream &os, const sf::Vector2<fpt> &vector) {
    os << "[x = " << vector.x << ", y = " << vector.y << "]";
    return os;
}

fpt norm(const sf::Vector2<fpt> &vec) {
    return std::sqrt(std::pow(vec.x, 2.0f) + std::pow(vec.y, 2.0f));
}

sf::Vector2<fpt> operator+(const sf::Vector2<fpt> &lhs, fpt scalar) {
    sf::Vector2<fpt> ret(lhs);
    ret.x += scalar;
    ret.y += scalar;
    return ret;
}

sf::Vector2<fpt> operator-(const sf::Vector2<fpt> &lhs, fpt scalar) {
    sf::Vector2<fpt> ret(lhs);
    ret.x -= scalar;
    ret.y -= scalar;
    return ret;
}

sf::Vector2<fpt> operator/(const sf::Vector2<fpt> &lhs, fpt scalar) {
    sf::Vector2<fpt> ret(lhs);
    ret.x /= scalar;
    ret.y /= scalar;
    return ret;
}

sf::Vector2<fpt> operator*(const sf::Vector2<fpt> &lhs, fpt scalar) {
    sf::Vector2<fpt> ret(lhs);
    ret.x *= scalar;
    ret.y *= scalar;
    return ret;
}