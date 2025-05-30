#ifndef TOOLSDISPLAY_HPP
#define TOOLSDISPLAY_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "DynamicForm.hpp"
#include <graphalgorithms.hpp>
#include <buildgraph.hpp>
#include <string>
#include "NodeStation.hpp"
#include "NodeSubStation.hpp"
#include "NodeHouse.hpp"


#include <memory> // --> std::shared_ptr



struct ArcoInfo {
    double distance;            
    int costo;                
    float perdidasEnergia;      
    //int numTransformadores;     
    //std::string tipoCable;      
};

using TrackNode =  std::tuple<
                        Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>*,
                        Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>*,
                        ArcoInfo
                    > ;


template <typename T>
//using GenericGraph = Designar::Digraph<std::shared_ptr<T>, double>;
using GenericGraph = Designar::Digraph<std::shared_ptr<T>, ArcoInfo>;


struct ImagenMapa {
    sf::Sprite sprite;
    std::string tipo;
    std::vector<std::string> datos;
};

struct Boton {
    sf::RectangleShape rect;
    sf::Text texto;
    std::string tipo;
    bool selected;
};

struct ConexionVisual {
    sf::Vector2f puntoA;
    sf::Vector2f puntoB;
    float t = 0.0f;  // Tiempo de interpolación
    bool active = true;


};

void drawLandscape(sf::RenderWindow& window);
    
void drawButtons(sf::RenderWindow& window, const std::vector<Boton>& botones) ;

void drawImage(sf::RenderWindow& window, GenericGraph<NodoRed>&  grafoRed) ;

void drawForm(sf::RenderWindow& window, DynamicForm form, sf::Vector2f mapPosition) ;

void showInfoBottom(sf::RenderWindow& window, const sf::Font& font, Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* seleccionada, std::map<std::string, sf::Texture> imgEstructure, std::pair<int, int> haveMovedWindow  ) ;

std::shared_ptr<NodoRed> detectImageClick( sf::Vector2f mousePos, sf::Vector2f relativePos, GenericGraph<NodoRed>&  grafoRed);

Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* detectImageClick( sf::Vector2f mousePos, GenericGraph<NodoRed>&  grafoRed);


std::string detectarClickBoton(std::vector<Boton>& botones, sf::Vector2f mousePos, sf::Vector2f relativePos) ;
    
std::vector<Boton> createButtons(const sf::Font& font);

void moverBotones(std::vector<Boton>& botones, int dx, int dy);
      
std::pair<int, int> moverView(sf::Vector2f& mapPosition, std::vector<Boton>& botones, std::pair<int, int>& haveMoved);

void agregarImagenSegunTipo(
        sf::Vector2f mouseOne,
        sf::Vector2f relativePos,
        const std::string& tipo,
         sf::Texture* texCasa,
         sf::Texture * texSub,
         sf::Texture * texEst,
        const std::vector<std::string>& datos,
        GenericGraph<NodoRed>& grafoRed,
        sf::RenderWindow& window,
        std::string id = ""

);

double distance(std::shared_ptr<NodoRed> a, std::shared_ptr<NodoRed> b);

void drawArcsLines(sf::RenderWindow& window, GenericGraph<NodoRed>&  grafoRed);

void drawArcsLines(sf::RenderWindow& window, std::vector<TrackNode>& unconnetedArcs,  float flash);

void setFromJson(
        sf::Vector2f mouseOne,
        sf::Vector2f relativePos,
        const std::string& tipo,
        sf::Texture * texCasa,
        sf::Texture * texSub,
        sf::Texture * texEst,
        const std::vector<std::string>& datos,
        GenericGraph<NodoRed>& grafoRed,
        sf::RenderWindow& window,
        std::string id 
    );

#endif