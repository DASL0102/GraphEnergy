#ifndef NODESTATION_HPP
#define NODESTATION_HPP
#include "NodeRed.hpp"
class Estacion : public NodoRed 
{
    private:
        std::string name;
        double x, y;
        float maxCapacity;
        float currentCharge;
        std::string id;
        sf::Sprite sprite; 

    public:
        Estacion(std::string name, 
            double x, 
            double y, 
            float maxCapacity, 
            float currentCharge, 
            std::string id, 
            sf::Texture* imgEstructure
        );
        
       
        std::string getId() const;
        std::string getTipo() const override;
        std::string getname() const override;
        std::string getSingleName() const override;
        std::string _retireveId() const override;
        double getX() const override;
        double getY() const override;
        sf::Sprite getSprite() const override;

       
        float getmaxCapacity() const override;
        float getCurrentCharge() const override;
        void setCurrentCharge(const float& newcurrentCharge);  
        std::shared_ptr<NodoRed> clone() const override; 
};

#endif 