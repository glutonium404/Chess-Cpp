#include "Coordinate.hpp"

Coordinate::Coordinate(int row, int col): row(row), col(col) {}

bool Coordinate::operator==(const Coordinate& other) const {
    return other.col == col && other.row == row;
}

bool Coordinate::operator!=(const Coordinate& other) const {
    return other.col != col || other.row != row;
}

bool Coordinate::is_valid() const {
    return !(row < 1 || row > 8 || col < 1 || col > 8);
}

bool Coordinate::is_valid(const int row, const int col) {
    return !(row < 1 || row > 8 || col < 1 || col > 8);
}


Coordinate Coordinate::operator+(const Coordinate& other) const {
    return Coordinate(row + other.row, col + other.col);
}

Coordinate Coordinate::operator-(const Coordinate& other) const {
    return Coordinate(row - other.row, col - other.col);
}

Coordinate Coordinate::operator*(const int scalar) const {
    return Coordinate(row * scalar, col * scalar);
}

void Coordinate::operator+=(const Coordinate& other) {
    row += other.row;
    col += other.col;
}

Coordinate Coordinate::getStepValues() const {
    return {
        (row > 0) - (row < 0),
        (col > 0) - (col < 0),
    };
}

std::string Coordinate::log() const {
    std::string msg = "{";
    msg += std::to_string(row);
    msg += ", ";
    msg += std::to_string(col);;
    msg += "}";
    return msg;
}

