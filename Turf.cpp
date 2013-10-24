#include "Turf.h"
#include "Map.h"
#include <iostream>
#include <algorithm>

bool layercomparison(Entity* a, Entity* b) {
    return (a->layer < b->layer);
}

Turf::Turf() {
    etype = turf;
    lastscreenx = -1000;
    lastscreeny = -1000;
    contents.resize(0);
    mobs.resize(0);
    items.resize(0);
    layer = 1;
    deleting = false;
}

Turf::Turf(std::string n, unsigned int f, char s, TCODColor c) {
    etype = turf;

    name = n;
    flags = f;
    symbol = s;
    color = c;
    lastscreenx = -1000;
    lastscreeny = -1000;
    contents.resize(0);
    mobs.resize(0);
    items.resize(0);
    layer = 1;
}

Turf::~Turf() { // contents contains pointers; gotta deallocate that memory!
    deleting = true;
    for(int i = 0; i < contents.size(); i++) {
        delete contents[i]; // release pointer
    }
    // don't need to release other lists; deleting pointers in contents should delete the other pointers as well
}

void Turf::Copy_To(Turf& t) {
    t.contents = contents;
    t.mobs = mobs;
    t.items = items;
}

void Turf::RemoveContents(Entity* remove, bool dodel) {
    for(int i = 0; i < contents.size(); i++) {
        if(contents[i] == remove) {
            if(dodel) {
                delete contents[i]; // delete entity from memory
            }
            contents.erase(contents.begin() + i); // erase the position
        }
    }
}

void Turf::RemoveMobs(Mob* remove, bool dodel) {
    for(int i = 0; i < mobs.size(); i++) {
        if(mobs[i] == remove) {
            if(dodel) {
                delete mobs[i]; // delete entity from memory
            }
            mobs.erase(mobs.begin() + i); // erase the position
        }
    }
}

void Turf::RemoveItems(Item* remove, bool dodel) {
    for(int i = 0; i < items.size(); i++) {
        if(items[i] == remove) {
            if(dodel) {
                delete items[i]; // delete entity from memory
            }
            items.erase(items.begin() + i); // erase the position
        }
    }
}

void Turf::LayerContents() {
    std::sort(contents.begin(), contents.end(), layercomparison);
}


