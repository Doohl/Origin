#include "Game.h"
#include "Entity.h"

Entity::Entity() {
    c_symbol = 0;
    symbol = '~';
    deleting = false;
}

void Entity::init() {
}

Entity::~Entity() {
}

void Entity::DoLogic(Game* game) {
}

void Entity::SaveEntity(tinyxml2::XMLElement* element) {
    std::map<std::string, boost::any>::iterator it;
    for(it = _properties.begin(); it != _properties.end(); ++it) {

        std::string index = it->first;
        boost::any any_container = it->second;

        if(any_container.type() == typeid(std::string)) {
            element->SetAttribute(index.c_str(), (boost::any_cast<std::string>(any_container)).c_str());
        } else if(any_container.type() == typeid(int)) {
            element->SetAttribute(index.c_str(), boost::any_cast<int>(any_container));
        }
    }
    std::string color_str = Helper::int2str(static_cast<int>(color.r)) + ",";
    color_str += Helper::int2str(static_cast<int>(color.g)) + ",";
    color_str += Helper::int2str(static_cast<int>(color.b));

    element->SetAttribute("symbol", std::string(1, symbol).c_str());
    element->SetAttribute("color", color_str.c_str());
}

void Entity::LoadEntity(tinyxml2::XMLElement* element) {
    for(const tinyxml2::XMLAttribute* attribute = element->FirstAttribute(); attribute != NULL; attribute = attribute->Next()) {
        std::string index = std::string(attribute->Name());

        if(index == "symbol") {
            symbol = Helper::str2char(std::string(attribute->Value()));
        } else if(index == "color") {
            std::vector<std::string> color_rgb = Helper::Explode(',', std::string(attribute->Value()));
            color = TCODColor(Helper::str2int(color_rgb[0]), Helper::str2int(color_rgb[1]), Helper::str2int(color_rgb[2]));
        }
        else {
            if(attribute->IntValue() != 0) {
                set_property(index, attribute->IntValue());
            } else {
                std::string value = std::string(attribute->Value());
                set_property(index, value);
            }
        }
    }
}

void Entity::copy_props(Entity* entity, bool hard_props) {
    std::map<std::string, boost::any>::iterator it;
    for(it = _properties.begin(); it != _properties.end(); ++it) {
        std::string index = it->first;
        boost::any any_container = it->second;
        entity->set_property(index, any_container);
    }

    if(hard_props) {
        entity->color = color;
        entity->bgcolor = bgcolor;
        entity->symbol = symbol;
        entity->c_symbol = c_symbol;
        entity->layer = layer;
        entity->etype = etype;
        entity->groups = groups;
    }
}

void Entity::set_property(std::string index, int val) {
    _properties[index] = val;
}

void Entity::set_property(std::string index, float val) {
    _properties[index] = val;
}

void Entity::set_property(std::string index, std::string val) {
    _properties[index] = val;
}

void Entity::set_property(std::string index, boost::any val) {
    _properties[index] = val;
}

void Entity::property_del(std::string index) {
    std::map<std::string, boost::any>::iterator any_it = _properties.find(index);
    if(any_it != _properties.end()) {
        _properties.erase(any_it);
    }
}
