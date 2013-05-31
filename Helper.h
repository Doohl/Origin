#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

#include "Libtcod.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <cstdarg>
#include <ctype.h> // char functions


class Helper {

    public:
        /* Split a string into vectors using a separator */
        static std::vector<std::string> Explode(char separator, std::string str);

        /* Turns an integer variable into a string */
        static std::string int2str(int i);

        /* Turns a string into an integer */
        static int str2int(std::string str);

        /* Determines if the string is a proper name (first letter is capital) */
        static bool proper(std::string name);

        /* Returns a list of valid characters */
        static std::vector<char>GetCharacters();

        /* Looks in a vector for a particular thing. Works great with pointers */
        template <class T> static bool Find(std::vector<T> &Vector, T Find);

        /* Looks in a vector for a particular thing, and returns the integer position (returns index -1 if not found)*/
        template <class T> static int Findpos(std::vector<T> &Vector, T Find);

        /* Picks a random object from an infinite array of parameters (works ONLY with pointers!) */
        template <class T> static T Pick(T first, ...);

        /* Merge two vectors */
        template <class T> static std::vector<T> MergeVector(std::vector<T> VectorA, std::vector<T> VectorB);

};


// These functions cannot be implemented in the compiled file for whatever arcane reason. FUKKEN C++!!!

template <class T> bool Helper::Find(std::vector<T> &Vector, T Find) {
    for(int i = 0; i < Vector.size(); i++) {
        if(Vector[i] == Find)
            return true;
    }
    return false;
}

template <class T> int Helper::Findpos(std::vector<T> &Vector, T Find) {
    for(int i = 0; i < Vector.size(); i++) {
        if(Vector[i] == Find)
            return i;
    }
    return -1;
}

template <class T> T Helper::Pick(T first, ...) {
    // Example: Helper::Pick(first, second, third, fourth, NULL);

    // Generate the list of elements to select from
    std::vector<T> Elements;
    T next = first;
    va_list parameters;

    va_start(parameters, first);
    while(next != NULL) {

        Elements.push_back(next);
        next = va_arg(parameters, T);
    }

    va_end(parameters);

    // Select a random element and return it!
    TCODRandom* RandomGen = new TCODRandom();
    return Elements[ RandomGen->getInt(0, Elements.size() - 1) ];
}

template <class T> std::vector<T> Helper::MergeVector(std::vector<T> VectorA, std::vector<T> VectorB) {
    std::vector<T> returnvector;
    returnvector.reserve(VectorA.size() + VectorB.size());
    returnvector.insert( returnvector.end(), VectorA.begin(), VectorA.end() );
    returnvector.insert( returnvector.end(), VectorB.begin(), VectorB.end() );
    return returnvector;
}




#endif // HELPER_H_INCLUDED
