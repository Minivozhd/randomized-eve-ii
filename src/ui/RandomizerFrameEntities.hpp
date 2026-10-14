#pragma once

#include <string>

typedef struct EnemyListItem {
    std::string name;
    bool drop;
    int id;
} enemylistitem_t;

typedef struct ItemListItem {
    std::string name;
    bool drop;
    bool shop;
    int id;
} itemlistitem_t;
