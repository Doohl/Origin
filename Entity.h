#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include "EntityData.h"
#include "libtcod.hpp"
#include "Helper.h"
#include "boost/any.hpp"
#include "depend/tinyxml2/tinyxml.h"

#include <string>
#include <vector>

class Game;
class Map;

// Entity class; makes up visible game objects
class Entity {

    public:
        Entity();
        virtual ~Entity();

        /* If enough energy, call this function */
        virtual void DoLogic(Game* game);

        /* Initialize all the variables */
        virtual void init();

        /* Take an XML element and load the entity's properties into attributes */
        virtual void SaveEntity(tinyxml2::XMLElement* element);

        /* Take an XML element and load its attributes into the entity's properties */
        virtual void LoadEntity(tinyxml2::XMLElement* element);

        /* Return property value */
        template<class T> T get_property(std::string index) {
            T property;
            if(find_property(index)) {
                property = boost::any_cast<T>(_properties[index]);
            }
            return property;
        }

        /* Set property values */
        void set_property(std::string index, int val);
        void set_property(std::string index, float val);
        void set_property(std::string index, std::string val);
        void set_property(std::string index, boost::any val);
        void set_properties(std::map<std::string, boost::any> newmap) {
            _properties = newmap;
        }

        /* int property delta */
        void delta_property(std::string index, int delta) {
            if(find_property(index)) {
                int i1 = boost::any_cast<int>(_properties[index]);
                _properties[index] = (i1 + delta);
            }
        }

        /* erase property */
        void property_del(std::string index);

        /* Property locator */
        bool find_property(std::string index) { return (_properties.find(index) != _properties.end()); }

        /* Copy properties from this entity to another */
            // hard_props determines whether or not to copy the 'hard' properties as well, ie color, symbol, etc.
        void copy_props(Entity* entity, bool hard_props);


        //std::string name;   // The name of the entity
        //std::string desc;   // The description of this entity

        e_type etype;       // (EntityData.h) The enumerated "type" of this entity
        //std::string id;     // generic id; tells what subtype this entity is

        int x;  // x position
        int y;  // y position

        char symbol;        // c-string representation of the Entity
        int c_symbol;       // ASCII representation of the Entity (libtcod.hpp)
                            // c_symbol used instead of symbol if c_symbol != 0
        TCODColor color;    // The foreground color of the cell   (libtcod.hpp)
        TCODColor bgcolor;  // The background color of the cell   (libtcod.hpp)
        char layer;         // display layer (higher layers will be displayed on top of lower layers)

        unsigned int flags; // bitfield dictating miscellaneous characteristics (EntityData.h)
        std::vector<std::string> groups; // container of groups the entity pertains to
        bool deleting;      // true if stored in the game DeleteBuff list

    protected:
        std::map<std::string, boost::any> _properties;
};

#endif // ENTITY_H_INCLUDED
