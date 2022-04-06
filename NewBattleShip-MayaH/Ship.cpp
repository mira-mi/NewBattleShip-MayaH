#include <cstdio>
#include "battleship.h"

Ship::Ship(const Ship& rhs) {
    shipNumber = rhs.shipNumber;
    name = rhs.name;
    size = rhs.size;
    status = rhs.status;
    firedUpon = rhs.firedUpon;
}

Ship& Ship::operator=(const Ship& rhs) {
    if (this != &rhs) {
        shipNumber = rhs.shipNumber;
        name = rhs.name;
        size = rhs.size;
        status = rhs.status;
        firedUpon = rhs.firedUpon;
    }
    return *this;
}

bool Ship::operator==(const Ship& rhs) {
    if ((name != rhs.name) || (size != rhs.size) || (status != rhs.status)) {
        return false;
    }
    return true;
}

bool Ship::operator!=(const Ship& rhs) {
    return !(*this == rhs);
}