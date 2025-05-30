#include "jsonManagment.hpp"
#include "NodeRed.hpp"
#include <graphalgorithms.hpp>
#include <buildgraph.hpp>


    double distance_(Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>*a, Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* b) {
        float a_x = a->get_info()->getX();
        float a_y = a->get_info()->getY();
        float b_x = b->get_info()->getX();
        float b_y = b->get_info()->getY();
        return sqrt(pow(a_x - b_x, 2) + pow(b_y - a_y, 2));
    }


void dont_check_connect_graph(std::string id_source, std::string id_target, int costo,  GenericGraph<NodoRed>& grafoRed) {

    
    auto* destino_real = grafoRed.search_node([&id_source](auto node) {
            return node->get_info()->_retireveId() == id_source;
    });

    auto* destino_real_2 = grafoRed.search_node([&id_target](auto node_) {
            return node_->get_info()->_retireveId() == id_target ;
        }
    );

    if(destino_real == nullptr or destino_real_2 == nullptr ){
        std::cerr << "Error copiando los nodos." << std::endl;

        return;
    }

    ArcoInfo arc_inset = {distance_(destino_real, destino_real_2) , costo, 0.0};

    auto arc_inserted = grafoRed.insert_arc( destino_real, destino_real_2, arc_inset );

    bool has_cycle = Designar::has_cycle(grafoRed);
    if (has_cycle) {
        grafoRed.remove_arc(arc_inserted);
        return;
    }
}



bool jsonRead(std::string archive_name, json& jsonGraphRed){

    std::ifstream file("src/maps/" + archive_name +".json");
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo." << std::endl;
        return false;
    }
    file >> jsonGraphRed;
    file.close();
    return true;

}

bool jsonLoad(std::string archiveName,
        sf::Vector2f relativePos,
        sf::Texture * texCasa,
        sf::Texture * texSub,
        sf::Texture * texEst,
        GenericGraph<NodoRed>& grafoRed,
        sf::RenderWindow& window
    ){     

    json dataGraph;

    if(!jsonRead(archiveName , dataGraph)){
        return false;
    }

    grafoRed.clear();

    
    for (const auto& nodo : dataGraph["nodos"]) {
        // 1. Extraer y convertir datos del nodo
        std::string tipo = nodo["tipo"].get<std::string>();
        std::string name = nodo["name"].get<std::string>();
        std::string id = nodo["id"].get<std::string>();
        double energy = nodo["energy"].get<double>();
        double x = nodo["x"].get<double>();
        double y = nodo["y"].get<double>();

        double current_energy = nodo["currentEnergy"].get<double>();
        std::string id_pertenece = nodo["idpertenece"].get<std::string>();
        

        if(tipo == "SubEstacion"){
                tipo = "Subestacion";
        }
    
        // 2. Preparar parámetros para la función
        sf::Vector2f posicion(static_cast<float>(y), static_cast<float>(x));
        
        std::vector<std::string> datos_nodo = {
            name,
            std::to_string(energy),
            std::to_string(current_energy),
            id_pertenece
        };

        setFromJson( posicion, relativePos , tipo, texCasa, texSub, texEst, datos_nodo, grafoRed, window,id );
    }
    


    // 4. Imprimir arcos
    try {

        //std::cout << "\n=== ARCOS ===" << std::endl;
        for (const auto& arco : dataGraph["arcos"]) {
                std::string id_source = arco["id_source"].get<std::string>();
                std::string id_target = arco["id_target"].get<std::string>();
                int costo = arco["costo"].get<int>();
                dont_check_connect_graph(id_source, id_target, costo,grafoRed);
        }

    }catch(...) {
            std::cerr << "Error desconocido procesando arco" << std::endl;
            return false;
    }   
    return true;

}

bool jsonWrite(std::string archive_name, json jsonGraphRed){
        std::ofstream archivo(archive_name);
        if (archivo.is_open()) {
                archivo << jsonGraphRed.dump(4); // 4 espacios de indentación
                archivo.close();
                std::cout << "Archivo JSON generado correctamente: red_electrica.json\n";
        } else {
                std::cerr << "Error al crear el archivo\n";
                return 1;
        }

        // Mostrar el JSON en consola
        std::cout << "Contenido generado:\n";
        std::cout << jsonGraphRed.dump(4) << std::endl;
        return true;
}


int saveJson(GenericGraph<NodoRed>& grafoRed, std::string dataArchiveName){

    json redElectrica;
            // Convertir nodos a JSON
    json jsonNodos = json::array();
    json jsonaArcs = json::array();
        
    grafoRed.for_each_node([&jsonNodos](auto* node) {
        std::string type = node->get_info()->getTipo();
        std::string name = node->get_info()->getSingleName();
        std::string id = node->get_info()->_retireveId();
        double energy = (node->get_info()->getmaxCapacity());
        float x = (node->get_info()->getY());
        float y = (node->get_info()->getX());

        if (auto casa = std::dynamic_pointer_cast<Casa>(node->get_info())) {
            std::string idpertenece = casa->getIdSubEstacion();
            jsonNodos.push_back({
                {"tipo", type},
                {"name", name},
                {"id", id},
                {"energy", energy},
                {"x", x},
                {"y", y},
                {"idpertenece", idpertenece},
                {"currentEnergy", 0}
            });




        } else if (auto subestacion = std::dynamic_pointer_cast<SubEstacion>(node->get_info())) {
           std::string idpertenece = subestacion->getIdEstacionPertenece();
           float currentEnergy = subestacion->getCurrentCharge();

            jsonNodos.push_back({
                {"tipo", type},
                {"name", name},
                {"id", id},
                {"energy", energy},
                {"x", x},
                {"y", y},
                {"idpertenece", idpertenece},
                {"currentEnergy", currentEnergy}
            });

        } else if (auto estacion = std::dynamic_pointer_cast<Estacion>(node->get_info())) {
           float currentEnergy = estacion->getCurrentCharge();


           jsonNodos.push_back({
                {"tipo", type},
                {"name", name},
                {"id", id},
                {"energy", energy},
                {"x", x},
                {"y", y},
                {"idpertenece", "0"},
                {"currentEnergy", currentEnergy}
            });
        }
        
        // jsonNodos.push_back({
        //         {"tipo", type},
        //         {"name", name},
        //         {"id", id},
        //         {"energy", energy},
        //         {"x", x},
        //         {"y", y}
        // });
    }); 

    grafoRed.for_each_arc([&jsonaArcs](auto* arc) {

        int costo  = arc->get_info().costo;
        std::string id_source = arc->get_src_node()->get_info()->_retireveId();
        std::string id_target = arc->get_tgt_node()->get_info()->_retireveId();

        
        jsonaArcs.push_back({
                {"id_source", id_source},
                {"id_target", id_target},
                {"costo", costo},
        });
    }); 

    // Añadir nodos y arcos al JSON principal

    redElectrica["nodos"] = jsonNodos;
    redElectrica["arcos"] = jsonaArcs;

    // Guardar en un archivo

    return jsonWrite("src/maps/" + dataArchiveName +".json", redElectrica);
        
}

