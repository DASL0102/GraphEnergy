#include "NodeStation.hpp"

Estacion::Estacion(std::string name,
     double x, 
     double y, 
     float maxCapacity, 
     float currentCharge, 
     std::string id, 
     sf::Texture* imgEstructure)
    : name(name), x(x), y(y), maxCapacity(maxCapacity), currentCharge(currentCharge), id(id) {
    
    sf::Vector2f mouseOne(x, y);
    sprite.setTexture(*imgEstructure);
    sprite.setScale(0.2f, 0.2f);
    sprite.setPosition(mouseOne);
   
}


std::string Estacion::getTipo() const { return "Estacion"; }
std::string Estacion::getname() const { return name + " / Max Capacity: "+ std::to_string(maxCapacity) + " / Current Charge : " +std::to_string(currentCharge); }
std::string Estacion::getSingleName() const { return name; }
std::string Estacion::_retireveId() const { return id; }
double Estacion::getX() const { return x; }
double Estacion::getY() const { return y; }
std::string Estacion::getId() const { return id; }
sf::Sprite Estacion::getSprite() const { return sprite; }

float Estacion::getmaxCapacity() const { return maxCapacity; }

float Estacion::getCurrentCharge() const { return currentCharge; }

void Estacion::setCurrentCharge(const float& newcurrentCharge){
    currentCharge = newcurrentCharge;
}

std::shared_ptr<NodoRed> Estacion::clone() const {
    return std::make_shared<Estacion>(*this);
}