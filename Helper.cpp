#include "Helper.h"
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
