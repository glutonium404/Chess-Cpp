#include "Coordinate.hpp"

bool Coordinate::is_valid() const {
    return !(row < 1 || row > 8 || col < 1 || col > 8);
}

bool Coordinate::is_valid(const int row, const int col) {
    return !(row < 1 || row > 8 || col < 1 || col > 8);
}

std::string Coordinate::log() const {
        std::string msg = "{";
        msg += std::to_string(row);
        msg += ", ";
        msg += std::to_string(col);;
        msg += "}";
        return msg;
    }

