#include "../include/casilla.hpp"

// Constructor
Casilla::Casilla(int x, int y) {
    coords_.first = x;
    coords_.second = y;
    tipo_ = 0;
}

// Constructor por defecto
Casilla::Casilla() {
    coords_.first = 0;
    coords_.second = 0;
    tipo_ = 0;
}

// Destructor
Casilla::~Casilla() {
}
