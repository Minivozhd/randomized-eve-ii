#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#define GRID_CELL_SIZE 256

namespace utils::geometry_works {
struct Point {
    int16_t x, y, z;
    Point(int16_t x = 0, int16_t y = 0, int16_t z = 0) : x(x), y(y), z(z) {}

    Point operator-(const Point &other) const {
        return Point(x - other.x, y - other.y, z - other.z);
    }

    Point operator+(const Point &other) const {
        return Point(x + other.x, y + other.y, z + other.z);
    }

    Point operator*(double scalar) const {
        return Point(static_cast<int16_t>(x * scalar), static_cast<int16_t>(y * scalar),
                     static_cast<int16_t>(z * scalar));
    }

    int32_t dot(const Point &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    int32_t length_squared() const {
        return x * x + y * y + z * z;
    }

    double length() const {
        return std::sqrt(static_cast<double>(length_squared()));
    }
};

struct Circle {
    Point center;
    int16_t radius;
    Circle(const Point &center, int16_t radius) : center(center), radius(radius) {}
};

using Polygon = std::vector<Point>;

struct Zone {
    Polygon polygon;
    int16_t height;

    // TODO: add another base height: 0x0080 or smthng
    Zone(Polygon _polygon = Polygon{}, int16_t _height = 0x0000) : polygon(_polygon), height(_height) {}
};

enum CellType { DEAD = 0, ALLOWED = 1, BOUNDARY = 2 };

using Grid = std::vector<std::vector<CellType>>;

struct Rectangle {
    Point topLeft;
    int width, length;
    Rectangle(Point topLeft, int width, int length) : topLeft(topLeft), width(width), length(length) {}
};

Grid createGrid(const std::vector<Zone> &, const std::vector<Zone> &);

std::vector<Point> reorderVerticesClockwise(const Polygon &);
bool canPlaceCircle(Circle &, const std::vector<Zone> &, const std::vector<Zone> &, bool,
                    const std::vector<Circle> &);
std::vector<Circle> arrangeSpawnPoints(int, int16_t, const std::vector<Zone> &, const std::vector<Zone> &,
                                       std::mt19937 &, int);

Rectangle findLongestRectangleInPolygons(const std::vector<Zone> &, const std::vector<Zone> &);
} // namespace utils::geometry_works
