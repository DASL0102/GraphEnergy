#include "NodeHouse.hpp"
#include <iostream>
#include <memory>
Casa::Casa(std::string name, 
    double x, 
    double y, 
    float consumption, 
    std::string idSubEstacion, 
    std::string id,  sf::Texture* imgEstructure)
    : name(name), x(x), y(y), consumption(consumption), idSubEstacion(idSubEstacion), id(id) {

    std::cout << "Dirección del sprite: ---1212---" << imgEstructure << std::endl;

    sf::Vector2f mouseOne(x, y);
    std::cout << "Dirección del sprite: ---1212---" << imgEstructure << std::endl;

    sprite.setTexture(*imgEstructure);
    std::cout << "Dirección del sprite: ---1212---" << imgEstructure << std::endl;

    sprite.setScale(0.1f, 0.1f);
    std::cout << "Dirección del sprite: ---1212---" << imgEstructure << std::endl;

    sprite.setPosition(mouseOne);

}


std::string Casa::getId() const { return id; }
std::string Casa::getIdSubEstacion() const { return idSubEstacion; }    
std::string Casa::getTipo() const { return "Casa"; }
std::string Casa::getname() const { return "Casa " + name + "/ Consumo: " + std::to_string(consumption); }
std::string Casa::getSingleName() const { return name; }
std::string Casa::_retireveId() const  { return id; }
double Casa::getX() const { return x; }
double Casa::getY() const { return y; }
sf::Sprite Casa::getSprite() const { return sprite; }


float Casa::getConsumption() const { return consumption; }    

               
float Casa::getmaxCapacity() const { return consumption ;}

float Casa::getCurrentCharge() const { return 0; }

void Casa::setIdSubEstacion(const std::string& newId) {
    idSubEstacion = newId;
}

std::shared_ptr<NodoRed> Casa::clone() const {
    return std::make_shared<Casa>(*this);
}