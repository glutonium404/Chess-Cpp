#pragma once

#include <string>

struct Coordinate {
    int row = 0;
    int col = 0;

    Coordinate(int row, int col);

    bool operator==(const Coordinate& other) const;
    bool operator!=(const Coordinate& other) const;

    Coordinate operator+(const Coordinate& other) const;
    Coordinate operator-(const Coordinate& other) const;
    Coordinate operator*(const int scalar) const;
    void operator+=(const Coordinate& other);

    bool is_valid() const;
    std::string log() const;
    Coordinate getStepValues() const;

    static bool is_valid(const int row, const int col);
};
