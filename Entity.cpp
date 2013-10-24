#include "Game.h"
#include "Entity.h"

Entity::Entity() {
    c_symbol = 0;
    deleting = false;
}

void Entity::init() {
}

Entity::~Entity() {
}

void Entity::DoLogic(Game* game) {
}
