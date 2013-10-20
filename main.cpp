#include "libtcod.hpp"
#include "Game.h"
#include "tinyxml.h"
#include <iostream>

int main() {

    Game game; // Create new game instance
    game.Start(); // Let the game run and do its thing

    /*
    tinyxml2::XMLDocument doc;
    doc.LoadFile("rsc/xml/items/melee.xml");
    //std::vector<std::string> stuph;
    tinyxml2::XMLElement* elem = doc.FirstChildElement();

    while(elem) {
        std::cout << elem->FirstChildElement("id")->GetText() << std::endl;
        std::cout << elem->FirstChildElement("name")->GetText() << std::endl;
        std::cout << elem->FirstChildElement("desc")->GetText() << std::endl;
        std::cout << elem->FirstChildElement("groups")->GetText() << std::endl;
        std::cout << elem->FirstChildElement("symbol")->GetText() << std::endl;
        std::cout << elem->FirstChildElement("color")->GetText() << std::endl;
        std::cout << elem->FirstChildElement("rarity")->GetText() << std::endl;
        std::cout << elem->FirstChildElement("weight")->GetText() << std::endl;
        std::cout << elem->FirstChildElement("volume")->GetText() << std::endl;
        std::cout << elem->FirstChildElement("damage")->FirstChildElement("blunt")->GetText() << std::endl;
        std::cout << elem->FirstChildElement("damage")->FirstChildElement("cut")->GetText() << std::endl;
        std::cout << elem->FirstChildElement("damage")->FirstChildElement("pierce")->GetText() << std::endl;
        if(elem == doc.LastChild()) {
            elem = NULL;
        } else {
            elem = elem->NextSiblingElement();
        }
    }
    //while(elem) {
     //   elem =
    /*
    while(elem != NULL) {

    std::cout << elem->GetText() << std::endl;
    elem = elem->NextSiblingElement();
    std::cout << elem->GetText();
    */

    return 0;

}
