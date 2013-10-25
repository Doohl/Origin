#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

#include "Libtcod.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstdarg>
#include <map>
#include <ctype.h> // char functions

#include "boost/shared_ptr.hpp"
#include "depend/AMEFProtocol/AMEFDecoder.h"
#include "depend/AMEFProtocol/AMEFEncoder.h"

#define TURF_PARSE long(1 << (0))
#define ITEM_PARSE long(1 << (1))
#define MOB_PARSE long(1 << (2))
#define AREA_PARSE long(1 << (3))
#define EFFECT_PARSE long(1 << (4))

class Helper {

    public:

       /** Type conversion functions */
        /* Split a string into vectors using a separator (or, string->vector) */
        static std::vector<std::string> Explode(char separator, std::string str);

        /* Implode a vector into a string using a separator (vector->string) */
        static std::string Implode(char separator, std::vector<std::string> vect);

        /* Turns an integer variable into a string */
        static std::string int2str(int i);

        /* Turns a string into an integer */
        static int str2int(std::string str);

        /* Turns a string into a float */
        static float str2float(std::string str);

        /* Returns the first char of input string */
        static char str2char(std::string str);

       /** Miscellaneous functions */

        /* Determines if the string is a proper name (first letter is capital) */
        static bool proper(std::string name);

        /* Returns a list of valid characters */
        static std::vector<char>GetCharacters();

        /* Makes the string uppercase */
        static void toUpper(std::string& str);

        /* Replaces all instances of 'from' with 'to' in 'str' */
        static void replace_all(std::string& str, std::string from, std::string to);

        /* Looks in a vector for a particular thing. Works great with pointers */
        template <class T> static bool Find(std::vector<T> &Vector, T Find);

        /* Looks in a vector for a particular thing, and returns the integer position (returns index -1 if not found)*/
        template <class T> static int Findpos(std::vector<T> &Vector, T Find);

        /* Picks a random object from an infinite array of parameters (works ONLY with pointers!) */
        template <class T> static T Pick(T first, ...);

        /* Merge two vectors */
        template <class T> static std::vector<T> MergeVector(std::vector<T> VectorA, std::vector<T> VectorB);

        /* Remove instance from vector */
        template <class T> static void Remove(std::vector<T> &Vector, T entry);

       /** IO / File functions */

        /* Parses an XML file and returns a vector of maps representing deserialized values for different entities */
        static std::vector< std::map< std::string, std::string > > SimpleXMLParse(const char* file, unsigned int parse_flags);

        /* Parses a TXT file and returns a string vector of the file broken down by \n tokens */
        static std::vector<std::string> SimpleParse(const char* file);

        /* Parses a file and returns a solid string of the contents */
        static std::string SingleParse(const char* file);

        /* Outputs/overwrites a file of this path */
        static void ofstream_put(std::string path, std::string data);

        /* Makes a directory in Unix and Windows */
        static void Smart_MKDir(std::string path);

        /* Checks to see if this file exists */
        static bool fexists(std::string path);
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

template <class T> void Remove(std::vector<T> &Vector, T entry) {
    for(int i = 0; i < Vector.size(); i++) {
        T index = Vector[i];
        if(index == entry)
            Vector.erase(Vector.begin()+i);
    }
}




#endif // HELPER_H_INCLUDED
