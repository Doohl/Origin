#include "Helper.h"
#include "tinyxml.h"
#include <iostream>

std::vector<std::string> Helper::Explode(char separator, std::string str) {

    std::vector<std::string> ReturnVector;

    std::string curstring = "";
    for(int i = 0; i < str.size(); i++) {

        if(str[i] != separator) { // if the character selected is not a separator
            curstring += str[i];
            if(i == str.size() - 1) // last loop
                ReturnVector.push_back(curstring);
        }
        else { // if it is a separator, add curstring to vector and move on
            ReturnVector.push_back(curstring);
            curstring = "";
        }
    }
    return ReturnVector;
}

std::string Helper::int2str(int i) {
    std::stringstream out;
    out << i;
    return out.str();
}


int Helper::str2int(std::string str) {
    int returnint;
    std::stringstream out(str);
    out >> returnint;
    return returnint;
}

float Helper::str2float(std::string str) {
    float returnfloat;
    std::istringstream(str) >> returnfloat;
    return returnfloat;
}

char Helper::str2char(std::string str) {
    return str[0];
}

bool Helper::proper(std::string name) {
    char c = name[0];
    return isupper(c);
}

std::vector<char> Helper::GetCharacters() {
    std::vector<char> ReturnVec;
    char specialchars[21] = {'~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '=', '_', '+', ';', '"', ',', '.', '\\', '|'};

    for(char x = 'b'; x <= 'z'; x++) { // a and b reserved for hand slots
        ReturnVec.push_back(x);
    }
    for(char x = 'A'; x <= 'Z'; x++) {
        ReturnVec.push_back(x);
    }
    for(char x = '0'; x <= '9'; x++) {
        ReturnVec.push_back(x);
    }
    for(int i = 0; i < sizeof(specialchars); i++) {
        ReturnVec.push_back(specialchars[i]);
    }
    return ReturnVec;
}

void Helper::replace_all(std::string& str, const std::string from, const std::string to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

/* Simple XML parser function */
std::vector< std::map< std::string, std::string > > Helper::SimpleXMLParse(const char* file, unsigned int parse_flags) {
    std::vector< std::map< std::string, std::string > > returnvect;

    tinyxml2::XMLDocument doc;
    doc.LoadFile(file);
    tinyxml2::XMLElement* elem = doc.FirstChildElement();

    while(elem) {
        std::map<std::string, std::string> entity;
        std::cout << elem->FirstChildElement("id")->GetText() << std::endl;

        tinyxml2::XMLElement* child_elem = NULL;

       /* Generic entity properties: */
        child_elem = elem->FirstChildElement("id");
        if(child_elem)
            entity["id"] = child_elem->GetText();
        child_elem = elem->FirstChildElement("name");
        if(child_elem)
            entity["name"] = child_elem->GetText();
        child_elem = elem->FirstChildElement("desc");
        if(child_elem)
            entity["desc"] = child_elem->GetText();
        child_elem = elem->FirstChildElement("groups");
        if(child_elem)
            entity["groups"] = child_elem->GetText();
        child_elem = elem->FirstChildElement("symbol");
        if(child_elem)
            entity["symbol"] = child_elem->GetText();
        child_elem = elem->FirstChildElement("color");
        if(child_elem)
            entity["color"] = child_elem->GetText();

       /* Item properties (Item.h): */
        if(parse_flags & ITEM_PARSE) {
            child_elem = elem->FirstChildElement("rarity");
            if(child_elem)
                entity["rarity"] = child_elem->GetText();
            child_elem = elem->FirstChildElement("weight");
            if(child_elem)
                entity["weight"] = child_elem->GetText();
            child_elem = elem->FirstChildElement("volume");
            if(child_elem)
                entity["volume"] = child_elem->GetText();
            child_elem = elem->FirstChildElement("value");
            if(child_elem)
                entity["value"] = child_elem->GetText();

            // Weapon-related
            child_elem = elem->FirstChildElement("pliancy");
            if(child_elem)
                entity["pliancy"] = child_elem->GetText();
            child_elem = elem->FirstChildElement("speed");
            if(child_elem)
                entity["speed"] = child_elem->GetText();

            // Wearable-related
            child_elem = elem->FirstChildElement("capacity");
            if(child_elem)
                entity["capacity"] = child_elem->GetText();
        }
        if(parse_flags & (ITEM_PARSE | MOB_PARSE)) {

            tinyxml2::XMLElement* damage_parent = elem->FirstChildElement("damage");

            // Damage numbers
            if(damage_parent) {
                child_elem = damage_parent->FirstChildElement("blunt");
                if(child_elem)
                    entity["blunt"] = child_elem->GetText();
                child_elem = damage_parent->FirstChildElement("cut");
                if(child_elem)
                    entity["cut"] = child_elem->GetText();
                child_elem = damage_parent->FirstChildElement("pierce");
                if(child_elem)
                    entity["pierce"] = child_elem->GetText();
            }
        }

       /* Mob properties (Mob.h): */
        if(parse_flags & MOB_PARSE) {

            child_elem = elem->FirstChildElement("hp");
            if(child_elem)
                entity["hp"] = child_elem->GetText();
            child_elem = elem->FirstChildElement("speed");
            if(child_elem)
                entity["speed"] = child_elem->GetText();
            child_elem = elem->FirstChildElement("friendlies");
            if(child_elem)
                entity["friendlies"] = child_elem->GetText();
            child_elem = elem->FirstChildElement("hostile");
            if(child_elem)
                entity["hostile"] = child_elem->GetText();
            child_elem = elem->FirstChildElement("seerange");
            if(child_elem)
                entity["seerange"] = child_elem->GetText();
            child_elem = elem->FirstChildElement("attacktext");
            if(child_elem)
                entity["attacktext"] = child_elem->GetText();
            child_elem = elem->FirstChildElement("attackspeed");
            if(child_elem)
                entity["attackspeed"] = child_elem->GetText();
        }

        if(elem == doc.LastChild()) {
            elem = NULL;
        } else {
            elem = elem->NextSiblingElement();
        }
        returnvect.push_back(entity);
    }
    return returnvect;
}

std::vector<std::string> Helper::SimpleParse(const char* file) {
    std::vector<std::string> returnvect;

    std::string curstring;
    std::ifstream in(file);

    if(in.is_open()) {
        while(std::getline(in, curstring)) {
            returnvect.push_back(curstring);
        }
        in.close();
    }

    return returnvect;
}
