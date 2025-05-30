#ifndef NODESUBSTATION_HPP
#define NODESUBSTATION_HPP
#include "NodeRed.hpp"

class SubEstacion : public NodoRed 
{
    private:
        std::string name;
        double x, y;
        float maxCapacity;
        float currentCharge;
        std::string id;
        std::string idEstacionPertenece;
        sf::Sprite sprite; 



    public:
        SubEstacion(std::string name,
            double x,
            double y,
            float maxCapacity,
            float currentCharge,
            std::string id,
            std::string idEstacionPertenece,
            sf::Texture* imgEstructure
    );
        
        
        std::string getTipo() const override;
        std::string getname() const override;
        std::string getSingleName() const override;
        std::string _retireveId() const override;
        double getX() const override;
        double getY() const override;
        sf::Sprite getSprite() const override;

        std::string getId() const;
        std::string getIdEstacionPertenece() const;

       
        float getmaxCapacity() const override;
        float getCurrentCharge() const override;
        void setIdEstacion(const std::string& newId);  
        void setCurrentCharge(const float& newcurrentCharge); 
        void setMaxCapacityCharge(const float& newmaxCharge); 
        std::shared_ptr<NodoRed> clone() const override; 
};

#endif 