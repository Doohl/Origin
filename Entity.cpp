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

std::string Entity::SaveEntity(Game* g) {
    return ";";
}

void Entity::LoadEntity(std::string tokenized_data, Game* g) {
}
