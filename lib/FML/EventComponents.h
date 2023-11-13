#pragma once

struct Collision {
    Collision(size_t id, size_t owner) {
        this->id = id;
        this->owner = owner;
    }
    size_t owner;
    size_t id;
};