#include "NodeRed.hpp"
#include <iostream>


void NodoRed::showUbication() const {
    std::cout << "Ubication: (" << getX() << ", " << getY() << ")" << std::endl;
}