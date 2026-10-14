#include "utils/GeometryWorks/GeometryWorks.hpp"

namespace utils::geometry_works {
double distance(const Point &a, const Point &b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

Point centroid(const Polygon &poly) {
    double x_sum = 0, y_sum = 0;
    for (const auto &point : poly) {
        x_sum += point.x;
        y_sum += point.y;
    }
    return Point(x_sum / poly.size(), y_sum / poly.size());
}

bool pointInPolygon(const Point &p, const Polygon &poly) {
    // For convex polygons only
    bool positive = false, negative = false;
    size_t n = poly.size();
    for (size_t i = 0; i < n; ++i) {
        const Point &a = poly[i], &b = poly[(i + 1) % n];
        double cross_product = (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
        if (cross_product > 0) {
            positive = true;
        } else if (cross_product < 0) {
            negative = true;
        } else {
            // Return true if the point on the edge is considered as inside the polygon, otherwise return
            // false.
            return true;
        }
        if (positive && negative) {
            return false;
        }
    }
    return true;
}

bool circlesIntersect(const Circle &a, const Circle &b) {
    return distance(a.center, b.center) < (a.radius + b.radius);
}

double pointToLineSegmentDistance(const Point &p, const Point &a, const Point &b) {
    Point ab = b - a;
    Point ap = p - a;

    double dot_product = ap.dot(ab);
    double length_ab_squared = ab.length_squared();

    double t = std::max(0.0, std::min(1.0, dot_product / length_ab_squared));

    Point projection = a + ab * t;
    return (p - projection).length();
}

bool circleInsidePolygon(const Circle &circle, const Polygon &poly) {
    // For convex polygons only
    if (!pointInPolygon(circle.center, poly)) {
        return false;
    }

    size_t n = poly.size();
    for (size_t i = 0; i < n; ++i) {
        const Point &a = poly[i], &b = poly[(i + 1) % n];
        double distance = pointToLineSegmentDistance(circle.center, a, b);
        if (distance < circle.radius) {
            return false;
        }
    }
    return true;
}

bool circleIntersectsPolygon(const Circle &circle, const Polygon &poly) {
    // For convex polygons only
    size_t n = poly.size();
    for (size_t i = 0; i < n; ++i) {
        const Point &a = poly[i], &b = poly[(i + 1) % n];
        double segment_length = distance(a, b);
        double projection =
            ((circle.center.x - a.x) * (b.x - a.x) + (circle.center.y - a.y) * (b.y - a.y)) / segment_length;
        if (projection > 0 && projection < segment_length) {
            double height =
                std::abs((b.x - a.x) * (circle.center.y - a.y) - (circle.center.x - a.x) * (b.y - a.y)) /
                segment_length;
            if (height < circle.radius) {
                return true;
            }
        }
    }
    return false;
}

std::vector<Point> reorderVerticesClockwise(const Polygon &vertices) {
    Point center = centroid(vertices);
    std::vector<Point> sorted_vertices(vertices);
    std::sort(sorted_vertices.begin(), sorted_vertices.end(), [&center](const Point &a, const Point &b) {
        return std::atan2(a.y - center.y, a.x - center.x) > std::atan2(b.y - center.y, b.x - center.x);
    });
    return sorted_vertices;
}

bool canPlaceCircle(Circle &circle, const std::vector<Zone> &list_of_quad_vertices,
                    const std::vector<Zone> &list_of_dead_zones, bool allow_intersection,
                    const std::vector<Circle> &arranged_circles) {
    if (!allow_intersection) {
        for (const auto &other_circle : arranged_circles) {
            if (circlesIntersect(circle, other_circle)) {
                return false;
            }
        }
    }

    bool contained_in_any_quad = false;
    for (const auto &quad_vertices : list_of_quad_vertices) {
        if (circleInsidePolygon(circle, quad_vertices.polygon)) {
            contained_in_any_quad = true;
            circle.center.z = quad_vertices.height;
            break;
        }
    }
    if (!contained_in_any_quad) {
        return false;
    }

    for (const auto &dead_zone : list_of_dead_zones) {
        if (circleInsidePolygon(circle, dead_zone.polygon) ||
            circleIntersectsPolygon(circle, dead_zone.polygon)) {
            return false;
        }
    }

    return true;
}

std::vector<Circle> arrangeSpawnPoints(int num_circles, int16_t radius,
                                       const std::vector<Zone> &list_of_quad_vertices,
                                       const std::vector<Zone> &list_of_dead_zones, std::mt19937 &gen,
                                       int max_iterations = 100) {
    std::vector<Circle> arranged_circles;
    int16_t x_min, y_min, x_max, y_max;
    // Calculate the bounding box for all the quad vertices
    x_min = x_max = list_of_quad_vertices[0].polygon[0].x;
    y_min = y_max = list_of_quad_vertices[0].polygon[0].y;
    for (const auto &quad_vertices : list_of_quad_vertices) {
        for (const auto &vertex : quad_vertices.polygon) {
            x_min = std::min(x_min, vertex.x);
            y_min = std::min(y_min, vertex.y);
            x_max = std::max(x_max, vertex.x);
            y_max = std::max(y_max, vertex.y);
        }
    }

    int counter = 0;
    bool allow_intersection = false;
    std::uniform_real_distribution<> x_distribution(x_min, x_max), y_distribution(y_min, y_max);

    while (static_cast<int>(arranged_circles.size()) < num_circles) {
        int16_t x = x_distribution(gen);
        int16_t y = y_distribution(gen);
        Point circle_center(x, y);
        Circle circle(circle_center, radius);

        if (canPlaceCircle(circle, list_of_quad_vertices, list_of_dead_zones, allow_intersection,
                           arranged_circles)) {
            arranged_circles.push_back(circle);
            counter = 0; // Reset the counter when a circle is placed successfully
        } else {
            counter++;
        }

        if (counter > max_iterations) {
            allow_intersection = true;
        }

        if (counter > 100000) {
            std::cout << "mega counter was exceeded!!!" << std::endl;
            break;
        }
    }

    return arranged_circles;
}

Grid createGrid(const std::vector<Zone> &zones, const std::vector<Zone> &dead_zones) {
    int16_t min_x = zones[0].polygon[0].x, max_x = min_x;
    int16_t min_y = zones[0].polygon[0].y, max_y = min_y;

    for (auto zone : zones) {
        for (auto vertex : zone.polygon) {
            if (vertex.x > max_x) {
                max_x = vertex.x;
            }
            if (vertex.x < min_x) {
                min_x = vertex.x;
            }
            if (vertex.y > max_y) {
                max_y = vertex.y;
            }
            if (vertex.y < min_y) {
                min_y = vertex.y;
            }
        }
    }

    int16_t width = (max_x - min_x) / GRID_CELL_SIZE + 1;
    int16_t height = (max_y - min_y) / GRID_CELL_SIZE + 1;

    Grid grid(height, std::vector<CellType>(width, DEAD));

    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); j++) {
            // Check if this cell is in any allowed zones
            bool allowed = false;
            for (auto zone : zones) {
                if (pointInPolygon(Point(min_x + j * GRID_CELL_SIZE + GRID_CELL_SIZE / 2,
                                         min_y + i * GRID_CELL_SIZE + GRID_CELL_SIZE / 2),
                                   zone.polygon)) {
                    allowed = true;
                    break;
                }
            }
            if (allowed) {
                for (auto dead_zone : dead_zones) {
                    if (pointInPolygon(Point(min_x + j * GRID_CELL_SIZE + GRID_CELL_SIZE / 2,
                                             min_y + i * GRID_CELL_SIZE + GRID_CELL_SIZE / 2),
                                       dead_zone.polygon)) {
                        allowed = false;
                        break;
                    }
                }
                if (allowed) {
                    grid[i][j] = ALLOWED;
                }
            }
        }
    }

    return grid;
}

int scanHorizontal(Grid &grid, size_t i, size_t j) {
    int length = 0;
    while (j < grid[i].size() && grid[i][j] == ALLOWED) {
        length++;
        j++;
    }
    return length;
}

int scanVertical(Grid &grid, size_t i, size_t j) {
    int length = 0;
    while (i < grid.size() && grid[i][j] == ALLOWED) {
        length++;
        i++;
    }
    return length;
}

Rectangle findLongestRectangleInPolygons(const std::vector<Zone> &zones,
                                         const std::vector<Zone> &dead_zones) {
    Grid grid = createGrid(zones, dead_zones);

    int16_t min_x = zones[0].polygon[0].x;
    int16_t min_y = zones[0].polygon[0].y;

    for (auto zone : zones) {
        for (auto vertex : zone.polygon) {
            if (vertex.x < min_x) {
                min_x = vertex.x;
            }
            if (vertex.y < min_y) {
                min_y = vertex.y;
            }
        }
    }

    Rectangle rect(zones[0].polygon[0], 0, 0);
    int max_length = 0;

    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); j++) {
            int horizontal_length = scanHorizontal(grid, i, j);
            int vertical_length = scanVertical(grid, i, j);

            if ((horizontal_length > max_length || vertical_length > max_length)) {
                if (horizontal_length > vertical_length) {
                    vertical_length = 6;
                } else {
                    horizontal_length = 6;
                }
                rect = Rectangle(Point(min_x + j * GRID_CELL_SIZE, min_y + i * GRID_CELL_SIZE),
                                 horizontal_length * GRID_CELL_SIZE, vertical_length * GRID_CELL_SIZE);
                max_length = horizontal_length > vertical_length ? horizontal_length : vertical_length;
            }
        }
    }

    return rect;
}
} // namespace utils::geometry_works
