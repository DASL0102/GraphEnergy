#ifndef NODERED_HPP
#define NODERED_HPP

#include <string>
#include <SFML/Graphics.hpp>
#include <memory>

class NodoRed {
public:
    virtual ~NodoRed() = default;  

    virtual sf::Sprite getSprite() const = 0;
    virtual std::string getTipo() const = 0;
    virtual std::string getname() const = 0;
    virtual std::string getSingleName() const = 0;
    virtual std::string _retireveId() const = 0;
    virtual double getX() const = 0;
    virtual double getY() const = 0;   
    virtual void showUbication() const;

    virtual float getmaxCapacity() const  = 0 ;
    virtual float getCurrentCharge() const  = 0 ;
    virtual std::shared_ptr<NodoRed> clone() const = 0;
    //virtual float getCurrentCharge() const = 0;
};

#endif 