#include "NodeSubStation.hpp"

SubEstacion::SubEstacion(std::string name,
    double x,
    double y, 
    float maxCapacity, 
    float currentCharge, 
    std::string id,
    std::string idEstacionPertenece, 
    sf::Texture *imgEstructure)
    : name(name), x(x), y(y), maxCapacity(maxCapacity), currentCharge(currentCharge), id(id), idEstacionPertenece(idEstacionPertenece)  {

    sf::Vector2f mouseOne(x, y);
    sprite.setTexture(*imgEstructure);
    sprite.setScale(0.2f, 0.2f);
    sprite.setPosition(mouseOne);
}


std::string SubEstacion::getTipo() const { return "SubEstacion"; }
std::string SubEstacion::getname() const { return name + " / Max Capacity: "+ std::to_string(maxCapacity) + " / Current Charge : " +std::to_string(currentCharge); }
std::string SubEstacion::getSingleName() const { return name; }
std::string SubEstacion::_retireveId() const  { return id; }
double SubEstacion::getX() const { return x; }
double SubEstacion::getY() const { return y; }
std::string SubEstacion::getId() const { return id; }
std::string SubEstacion::getIdEstacionPertenece() const { return idEstacionPertenece; }
sf::Sprite SubEstacion::getSprite() const { return sprite; }


float SubEstacion::getmaxCapacity() const { return maxCapacity; }
float SubEstacion::getCurrentCharge() const { return currentCharge; }

void SubEstacion::setIdEstacion(const std::string& newId) {
    idEstacionPertenece = newId;
}


void SubEstacion::setCurrentCharge(const float& newcurrentCharge){
    currentCharge = newcurrentCharge;
}

std::shared_ptr<NodoRed> SubEstacion::clone() const {
    return std::make_shared<SubEstacion>(*this);
}

void SubEstacion::setMaxCapacityCharge(const float& newmaxCharge){
    maxCapacity += newmaxCharge;
}

