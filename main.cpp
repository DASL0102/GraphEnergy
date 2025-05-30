#include <SFML/Graphics.hpp>
#include <iostream>
#include "GraphHandle.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include "jsonManagment.hpp"

using json = nlohmann::json;

const int FORM_HEIGHT = 300;
const int FORM_WIDTH = 300;

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<NodoRed>& nodo) {
    if (nodo) {
        os << nodo->getname();  // usas el nombre personalizado
        os << nodo->getX();
        os << nodo->getY();
    } else {
        os << "nullptr";
    }
    return os;
}


std::ostream& operator<<(std::ostream& os, const ArcoInfo& arco) {
    os << "L:" << arco.distance;  // Muestra la longitud (ajusta según tus campos)
    // Si quieres más  detalles, puedes hacer:
    // os << "L:" << arco.length << ", R:" << arco.resistance;
    return os;
}


// Tamaño original base
const sf::Vector2u originalWindowSize(1000, 700);
sf::Vector2f mapPosition(originalWindowSize.x / 2.f, originalWindowSize.y / 2.f);

sf::View getLetterboxView(sf::View view, int windowWidth, int windowHeight) {
    float windowRatio = windowWidth / static_cast<float>(windowHeight);
    float viewRatio = view.getSize().x / view.getSize().y;
    float sizeX = 1.f, sizeY = 1.f;
    float posX = 0.f, posY = 0.f;

    if (windowRatio > viewRatio) {
        sizeX = viewRatio / windowRatio;
        posX = (1.f - sizeX) / 2.f;
    } else {
        sizeY = windowRatio / viewRatio;
        posY = (1.f - sizeY) / 2.f;
    }

    view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
    return view;
}

int main(){


    GraphHandle handler;
    GenericGraph<NodoRed> grafoRed; 
    GenericGraph<NodoRed> grafo_saved;
    GenericGraph<NodoRed> grafo_p;

    float flash = 0;
    std::pair<int, int> haveMovedWindow = std::make_pair(0,0);

    sf::RenderWindow window(sf::VideoMode(originalWindowSize.x, originalWindowSize.y), "Planificador de Red Electrica", sf::Style::Default);
    sf::View view(sf::FloatRect(0, 0, originalWindowSize.x, originalWindowSize.y));
    view.setCenter(mapPosition);
    window.setView(view);

    sf::Vector2f mapPosition(500, 350);

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
        std::cerr << "Error al cargar la fuente" << std::endl;
        return 1;
    }
    
    std::vector<Boton> botones = createButtons(font);
    std::vector<TrackNode> unconetedArcs;

    DynamicForm form(FORM_HEIGHT, FORM_WIDTH, 350, 150);


    std::string select = "";
    bool esperandoClick = false;
    sf::Vector2f clickPosicionMapa;


    sf::Texture texCasa, texSubestacion, texEstacion;
    if (!texCasa.loadFromFile("src/img/casa.png")) { 
        std::cerr << "Error al cargar textura casa.png" << std::endl; 
    }
    if (!texSubestacion.loadFromFile("src/img/subestacion.png")) { 
        std::cerr << "Error al cargar textura subestacion.png" << std::endl; 
    }
    if (!texEstacion.loadFromFile("src/img/estacion.png")) { 
        std::cerr << "Error al cargar textura estacion.png" << std::endl; 
    }
    std::map<std::string, sf::Texture> imgEstructure;

    imgEstructure["Casa"] = texCasa;
    imgEstructure["SubEstacion"] =texSubestacion;
    imgEstructure["Estacion"] = texEstacion;


    std::shared_ptr<NodoRed> seleccion1 = nullptr;
    std::shared_ptr<NodoRed> seleccion2 = nullptr;


    while (window.isOpen()){
        sf::Event event;

        while (window.pollEvent(event)){

            if (event.type == sf::Event::Closed){
                window.close();
            }
            


            if (form.visible()) {

                if (form.isFormFilled(event, window, select)) {
                   
                    std::vector<std::string> dataForm = form.getFormData();
                   
                    if(select == "Grabar Doc."){ 
                        //Add teh graph to an archive
                        saveJson(grafoRed, dataForm[0]);
                        

                    }else if(select == "Cargar Doc."){
                        //Load a graph from an archive

                        if(!jsonLoad(dataForm[0], mapPosition, &texCasa, &texSubestacion, &texEstacion, grafoRed, window)){
                            form.show();
                            form.setinfoSubmitText("Ese archivo no existe.");
                        }

                    }
                    else if(select == "Eliminar"){
                        form.show();
                        std::string id_to_delete = dataForm[0];

                        handler.delete_connected_node_alone(grafoRed, id_to_delete);
                        //handler.delete_one_node_alone(grafoRed, id_to_delete);

                    }
                    else if(select == "E. Nodo"){
                        form.show();
                        std::string id_to_delete = dataForm[0];

                        //handler.delete_connected_node_alone(grafoRed, id_to_delete);
                        handler.delete_one_node_alone(grafoRed, id_to_delete);

                    }
                    else{
                        agregarImagenSegunTipo( clickPosicionMapa, mapPosition , select, &texCasa, &texSubestacion, &texEstacion, dataForm, grafoRed, window);
                    }    
                    
                    esperandoClick = false;
                }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
                    form.toggle();
                    esperandoClick = false;
                }
            } else if (event.type == sf::Event::MouseButtonPressed ) {

                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                std::string clickedButton = detectarClickBoton(botones, mousePos,mapPosition);
                
                if(clickedButton != ""){
                    unconetedArcs.clear();

                }

                if(event.mouseButton.button == sf::Mouse::Left){
                    if (!clickedButton.empty()) {
                        select = clickedButton;
                        esperandoClick = true;

                        // Manejo específico  para el nuevo botón
                        if (clickedButton == "C. Carga") {
                            
                            bool flaf_sub = false;
                            bool flaf_est = false;
                            grafoRed.for_each_node([&flaf_est, &flaf_sub](auto* node) {
                                if (node->get_info()->getTipo() == "Estacion") {
                                    flaf_est = true;
                                   
                                }

                                if(node->get_info()->getTipo() == "SubEstacion"){
                                    flaf_sub = true;
                                }
                            });

                            if(flaf_est && flaf_sub){
                                auto connectivity = handler.check_connectivity_estacion_subestacion(grafoRed);
                                if(std::get<0>(connectivity)){
                                    auto nodo =  handler.find_subestacion(grafoRed);
                                    auto grafo = handler.GraphHandlebfs_charge(grafoRed,nodo);
                                    unconetedArcs = handler.lookingNodes2(grafoRed, grafo, unconetedArcs, "0", nodo);
                                    //Designar::DotGraph<GenericGraph<NodoRed>>().write_graph(grafoRed, "grafoAlone3.dot");
                                }
                            }
                            
                        }else if(clickedButton == "C. Distancia"){
                            do{
                                unconetedArcs.clear();
                                unconetedArcs = handler.lookingNodes(grafoRed, unconetedArcs);
                            }while(unconetedArcs.size() > 0);
                            //Designar::DotGraph<GenericGraph<NodoRed>>().write_graph(grafoRed, "grafoAlone3.dot");
                        }else if(clickedButton == "Guardar"){
                            grafo_saved.clear();
                            grafo_saved = handler.Copiar_G(grafoRed);
                            

                            Designar::DotGraph<GenericGraph<NodoRed>>().write_graph(grafoRed, "grafoAlone4.dot");

                        }else if(clickedButton == "Volver"){
                            grafoRed.clear();
                            grafoRed = handler.Copiar_G(grafo_saved);;
                            
                            //Designar::DotGraph<GenericGraph<NodoRed>>().write_graph(grafoRed, "grafoAlone5.dot");
                      

                        }else if(clickedButton == "Fix Distancia"){
                            handler.fix_charge(grafoRed);

                        }else if(select == "Grabar Doc."){
                            form.createForm(select, window);

                        }else if(select == "Cargar Doc."){
                            form.createForm(select, window);

                        }else if(select == "Eliminar"){
                            form.createForm(select, window); 

                        }else if(select == "E. Nodo"){
                            form.createForm(select, window);
     
                        }
                        else if(clickedButton == "E. Todo"){
                            std::cout << "Entro en E. Todo" << std::endl;
                            handler.delete_alone_nodes(grafoRed);
                        }

                    }else if (esperandoClick && !select.empty()) {
                        //save the position for node

                        if(select == "Casa" or select == "Subestacion" or select == "Estacion"){
                            clickPosicionMapa = mousePos;
                            form.createForm(select, window);
                        }
                    }
                    
                }else{

                    if(select == "C. Manual"){
                        sf::Vector2f mousePosMapaManual = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                        std::shared_ptr<NodoRed> seleccionado = detectImageClick( mousePosMapaManual, mapPosition, grafoRed);
                        if (seleccionado != nullptr) {
                            if (seleccion1 == nullptr) {
                                seleccion1 = seleccionado;
                            } else if (seleccion2 == nullptr && seleccionado != seleccion1) {
                                seleccion2 = seleccionado;
                    
                                handler.connect_graph(grafoRed, seleccion1, seleccion2);
                                // Limpiar selección
                                seleccion1 = nullptr;
                                seleccion2 = nullptr;
                            }
                        }
                    }    
                }
                
            }

            
        }


         
        

    window.clear(sf::Color::Black);

    // Aplicar vista centrada
    //window.setView(sf::View(mapPosition, sf::Vector2f(1000, 700)));

    moverView(mapPosition, botones, haveMovedWindow);

    //drawArcsLines(window, unconetedArcs, flash);


            if (event.type == sf::Event::Resized) {
                view = sf::View(sf::FloatRect(0, 0, originalWindowSize.x, originalWindowSize.y));
                view.setCenter(mapPosition);
                view = getLetterboxView(view, event.size.width, event.size.height);
            }
    view.setCenter(mapPosition);

    window.setView(view);
    drawLandscape(window);
    drawArcsLines(window, grafoRed);
    drawImage(window, grafoRed);
    drawButtons(window, botones);
    drawForm(window, form, mapPosition);
    window.setView(view);

    flash += 1;
    if(flash > 100) flash = 0;

    // Mostrar  info inferior
    sf::Vector2f mousePosMapa = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass> * img_selected = detectImageClick(mousePosMapa, grafoRed);


    if(img_selected != 0){
        showInfoBottom(window, font, img_selected, imgEstructure, haveMovedWindow);
    }

    window.display();
    }

    return 0;
}
