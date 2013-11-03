#include "Helper.h"
#include "depend/tinyxml2/tinyxml.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>

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

std::string Helper::Implode(char separator, std::vector<std::string> vect) {

    std::string returnstring;

    for(int i = 0; i < vect.size(); i++) {
        returnstring += vect[i];
        if(i != vect.size()-1) returnstring += separator;
    }
    return returnstring;
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

bool Helper::strIsInt(std::string str) {
   std::stringstream out(str);
   int dummy_int;
   if(!(out >> dummy_int)) {
        return false;
   } else {
        return true;
   }
}

bool Helper::strIsFloat(std::string str) {
   std::stringstream out(str);
   float dummy_float;
   if(!(out >> dummy_float)) {
        return false;
   } else {
        return true;
   }
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
    char specialchars[21] = {'!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '=', '_', '+', ';', '"', ',', '.', '\\', '|'};

    for(char x = 'a'; x <= 'z'; x++) { // a and b reserved for hand slots
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

void Helper::toUpper(std::string& str) {
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
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
std::vector< std::map< std::string, std::string > > Helper::SimpleXMLParse(const char* file) {
    std::vector< std::map< std::string, std::string > > returnvect;

    tinyxml2::XMLDocument doc;
    doc.LoadFile(file);

    for(tinyxml2::XMLElement* elem = doc.FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
        std::map<std::string, std::string> entity;

        tinyxml2::XMLAttribute* id_attribute = elem->FindAttribute("id");
        if(id_attribute)
            entity["id"] = std::string(id_attribute->Value());

        for(tinyxml2::XMLElement* sub_elem = elem->FirstChildElement(); sub_elem != NULL; sub_elem = sub_elem->NextSiblingElement()) {

            if(std::string(sub_elem->Name()) == "damage" || std::string(sub_elem->Name()) == "defence") {

                tinyxml2::XMLElement* child_elem = sub_elem->FirstChildElement("blunt");
                if(child_elem)
                    entity["blunt_"+std::string(sub_elem->Name())] = child_elem->GetText();
                child_elem = sub_elem->FirstChildElement("cut");
                if(child_elem)
                    entity["cut_"+std::string(sub_elem->Name())] = child_elem->GetText();
                child_elem = sub_elem->FirstChildElement("pierce");
                if(child_elem)
                    entity["pierce_"+std::string(sub_elem->Name())] = child_elem->GetText();

            } else {
                entity[std::string(sub_elem->Name())] = std::string(sub_elem->GetText());
            }
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

std::string Helper::SingleParse(const char* file) {
    std::ifstream t(file);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

void Helper::ofstream_put(std::string path, std::string data) {
    std::ofstream file;
    file.open(path.c_str());
    file << data;
    file.close();
}

void Helper::Smart_MKDir(std::string path) {
    #if defined(_WIN32)
        // Default Win32 MS DOS mkdir command
        Helper::replace_all(path, "/", "\\"); // replace all forward slashes with backslash; MS DOS does not use forward slashes
        std::string win32cmd = "mkdir " + path;
        system(win32cmd.c_str());
    #else
        // Default Unix-based mkdir command with -p option
        std::string win32cmd = "mkdir -p " + path;
        system(win32cmd.c_str());
    #endif
}

bool Helper::fexists(std::string path) {
    std::ifstream ifile(path.c_str());
    return ifile;
}
