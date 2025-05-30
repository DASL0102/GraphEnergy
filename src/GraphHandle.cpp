#include "GraphHandle.hpp"

#include <vector>
#include <memory> 
#include <graph.hpp>
#include "graph.hpp"
#include <list.hpp>
#include <polygon.hpp>
#include <list.hpp>
#include <set.hpp>
#include <sort.hpp>
#include <queue>
#include <set>
#include <cmath> 




using NodoRedPtr = Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>;
struct Weight {
    using Type = double;
    static constexpr Type ZERO = 0;

    template <typename Arc>
    Type operator()(const Arc& arc) const {
        return arc->get_info().distance;
    }
};


template <typename GraphType, typename NodeType>
std::vector<typename GraphType::Arc*> get_in_arcs(GraphType& graph, NodeType* node);


template <typename GraphType, typename NodeType>
std::vector<NodeType*> get_path_to(GraphType& tree, NodeType* target_node) {
    std::vector<NodeType*> path;
    NodeType* current = target_node;

    while (current) {
        path.push_back(current);
        auto in_arcs = get_in_arcs(tree, current);
        if (in_arcs.empty()) break;  // Llegamos al nodo raíz
        current = in_arcs.front()->get_src_node();
    }

    std::reverse(path.begin(), path.end());
    return path;
}

template <typename GraphType, typename NodeType>
std::vector<typename GraphType::Arc*> get_in_arcs(GraphType& graph, NodeType* node) {
    std::vector<typename GraphType::Arc*> in_arcs;
    graph.for_each_arc([&](auto* arc) {
        if (arc->get_tgt_node() == node) {
            in_arcs.push_back(arc);
        }
    });
    return in_arcs;
}


bool isAlmostEqual(double a, double b) {
    return std::abs(a - b) <= std::numeric_limits<double>::epsilon() * std::max(std::abs(a), std::abs(b));
}


template <typename GraphType, typename NodeType>
std::vector<typename GraphType::Arc*> get_out_arcs(GraphType& graph, NodeType* node) {
    std::vector<typename GraphType::Arc*> out_arcs;
    graph.for_each_arc([&](auto* arc) {
        if (arc->get_src_node() == node) {
            out_arcs.push_back(arc);
        }
    });
    return out_arcs;
}

//********************************* */

std::tuple<
    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>*,
    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>*,
    ArcoInfo
    >  GraphHandle::min_connectivity2(
    GenericGraph<NodoRed>& grafoRed,
    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* node_to_insert
    ) {
       

    if(node_to_insert->get_info()->getTipo() == "Estacion")
    {
        return {nullptr, nullptr, ArcoInfo{}};
    }

    

    if(node_to_insert->get_info()->getTipo() == "SubEstacion")
    {
        return {nullptr, nullptr, ArcoInfo{}};
    }
   

    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* nodo_g2; 
    Designar::Digraph<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass> grafoRed2;
    grafoRed.copy_graph(grafoRed, grafoRed2);

    grafoRed2.for_each_node([&grafoRed2, &node_to_insert, &nodo_g2](auto* node) {
        if (node->get_info() == node_to_insert->get_info()){
            nodo_g2 = node;

        }
    }); 

    grafoRed2.for_each_node([&nodo_g2, &grafoRed2](auto* node) {

 
        
        auto& new_node = nodo_g2->get_info();
        double x1 = new_node->getX();
        double y1 = new_node->getY();

        auto node_in_g = node->get_info();
        double x2 = node_in_g->getX();
        double y2 = node_in_g->getY();

        if(node_in_g->getTipo() != "Estacion")
        {
            double distance = std::sqrt(std::pow(x2 - x1,2) + std::pow(y2 - y1,2));

            ArcoInfo arc_isert = {distance, 2, 0.0};

            grafoRed2.insert_arc(nodo_g2, node, arc_isert);
        }
      
    });


    GenericGraph<NodoRed> Dijkstra_tree = Designar::Dijkstra<GenericGraph<NodoRed>, Weight, std::less<int>>().build_min_path_tree(grafoRed2,nodo_g2);

    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* src_insert;
    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* tgt_insert;
    unsigned short counter = 0;
    double min_distance = 1e12;
    auto* best_arc = static_cast<decltype(Dijkstra_tree.get_first_arc())>(nullptr);
    std::vector<decltype(Dijkstra_tree.get_first_arc())> arcs_to_remove;
   

    Dijkstra_tree.for_each_arc([&](auto* arc) {
        auto distancia = arc->get_info().distance;
        auto node_src = arc->get_src_node()->get_info();
        auto node_tgt = arc->get_tgt_node()->get_info();

        // Solo consideramos arcos salientes desde nodo_g2
        if (node_src == nodo_g2->get_info()) {

            if(node_tgt->getTipo() == "Estacion"){
                std::cout << "estacion";
            }
            else if (distancia < min_distance) {
                // Guardamos el nuevo arco más corto
                min_distance = distancia;
                best_arc = arc;
                src_insert = arc->get_src_node();
                tgt_insert = arc->get_tgt_node();
            }
        }else{
            arcs_to_remove.push_back(arc);
        }

      

        ++counter;
        //std::cout << "Arco #" << counter << ", Distancia: " << distancia << std::endl;
    });
   

    Dijkstra_tree.for_each_arc([&](auto* arc) {
        auto node_src = arc->get_src_node()->get_info();


        if (node_src == nodo_g2->get_info() && arc != best_arc) {
            
            arcs_to_remove.push_back(arc);
        }
    });
    

    for (auto* arc : arcs_to_remove) {
        Dijkstra_tree.remove_arc(arc);
        
    }
    

    auto node_insert1 = grafoRed.search_node([=](auto node)
    {
        return src_insert->get_info()  == node->get_info();

         //auto nodeInfo = node->get_info();
         //if (src_insert->get_info()  == nodeInfo){
         //   return node;     
         //}
         
     });
     

     auto node_insert2 = grafoRed.search_node([=](auto node)
    {
        return tgt_insert->get_info()  == node->get_info();
         //auto nodeInfo = node->get_info();
         //if (tgt_insert->get_info()  == nodeInfo){
         //   return node;     
         //}
         
     });

     ArcoInfo arc_final = {min_distance, 2, 0.0};

     //grafoRed.insert_arc(node_insert2, node_insert1, arc_final);
    //  std::cout<<"rrrrrrrrrrrrrrrrrrrrrrr\n";
    // std::cout<<node_insert1->get_info()->getTipo()<<"----node1111##################\n";
    // std::cout<<node_insert1->get_info()->getname()<<"----node1111##################\n";
    // std::cout<<node_insert1->get_info()->getX()<<"----node1111##################\n";
    // std::cout<<node_insert1->get_info()->getY()<<"----node1111##################\n";

    //     std::cout<<node_insert2->get_info()->getTipo()<<"----node222##################\n";
    // std::cout<<node_insert2->get_info()->getname()<<"----node222##################\n";
    // std::cout<<node_insert2->get_info()->getX()<<"----node222##################\n";
    // std::cout<<node_insert2->get_info()->getY()<<"----node222##################\n";
     return {node_insert1, node_insert2, arc_final};

}



//************************************** */


std::tuple<
        Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>*,
        Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>*,
        ArcoInfo
    >  GraphHandle::min_connectivity(
        GenericGraph<NodoRed>& grafoRed,
        Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* node_to_insert
    ) {
       

    if(node_to_insert->get_info()->getTipo() == "Estacion"){
        return {nullptr, nullptr, ArcoInfo{}};
    }


    if(node_to_insert->get_info()->getTipo() == "SubEstacion"){
        return {nullptr, nullptr, ArcoInfo{}};
    }

    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* nodo_g2; 
    Designar::Digraph<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass> grafoRed2;
    grafoRed.copy_graph(grafoRed, grafoRed2);

    grafoRed2.for_each_node([&grafoRed2, &node_to_insert, &nodo_g2](auto* node) {
        if (node->get_info() == node_to_insert->get_info()){
            nodo_g2 = node;
        }
    });

    bool NoArcInserted = true;

    grafoRed2.for_each_node([&nodo_g2, &grafoRed2, &grafoRed, &node_to_insert, &NoArcInserted](auto* node) {

        
        auto& new_node = nodo_g2->get_info();
        double x1 = new_node->getX();
        double y1 = new_node->getY();

        auto node_in_g = node->get_info();
        double x2 = node_in_g->getX();
        double y2 = node_in_g->getY();
        

        std::cout<<node_in_g->getTipo()<<std::endl;
       

        if(node_in_g->getTipo() != "Estacion" ){
           

            if (auto casa = std::dynamic_pointer_cast<Casa>(node_in_g)) {
                
                
                if(casa->getIdSubEstacion() != "0"){
                    NoArcInserted = false;
                    double distance = std::sqrt(std::pow(x2 - x1,2) + std::pow(y2 - y1,2));
                    ArcoInfo arc_isert = {distance, 2, 0.0};
                    grafoRed2.insert_arc(nodo_g2, node, arc_isert);
                }
            }else if(auto sub = std::dynamic_pointer_cast<SubEstacion>(node_in_g)){
                // ver si la sub esta conectada a una estacion
               

                if(sub->getIdEstacionPertenece() != "0"){
                    NoArcInserted = false;
                    double distance = std::sqrt(std::pow(x2 - x1,2) + std::pow(y2 - y1,2));
                    ArcoInfo arc_isert = {distance, 2, 0.0};
                    grafoRed2.insert_arc(nodo_g2, node, arc_isert);
                }    
            }    
        }
      
    });
    
    if(NoArcInserted){
        return {nullptr, nullptr, ArcoInfo{}};
    }
    



    GenericGraph<NodoRed> Dijkstra_tree = Designar::Dijkstra<GenericGraph<NodoRed>, Weight, std::less<int>>().build_min_path_tree(grafoRed2,nodo_g2);

    NodoRedPtr* src_insert;
    NodoRedPtr* tgt_insert;

    unsigned short counter = 0;
    double min_distance = 1e12;
    auto* best_arc = static_cast<decltype(Dijkstra_tree.get_first_arc())>(nullptr);
    std::vector<decltype(Dijkstra_tree.get_first_arc())> arcs_to_remove;

    Dijkstra_tree.for_each_arc([&](auto* arc) {
        auto distancia = arc->get_info().distance;
        auto node_src = arc->get_src_node()->get_info();
        auto node_tgt = arc->get_tgt_node()->get_info();

        // Solo consideramos arcos salientes desde nodo_g2
        if (node_src == nodo_g2->get_info()) {

            if(node_tgt->getTipo() == "Estacion"){
                
            }
            else if (distancia < min_distance) {
                
                min_distance = distancia;
                best_arc = arc;
                src_insert = arc->get_src_node();
                tgt_insert = arc->get_tgt_node();
            }
        }else{
            arcs_to_remove.push_back(arc);
        }

        ++counter;
    });

    Dijkstra_tree.for_each_arc([&](auto* arc) {

        auto node_src = arc->get_src_node()->get_info();

        if (node_src == nodo_g2->get_info() && arc != best_arc) {
            
            arcs_to_remove.push_back(arc);
        }
    });

    for (auto* arc : arcs_to_remove) {
        Dijkstra_tree.remove_arc(arc);
    }

    auto node_insert1 = grafoRed.search_node([=](auto node){
        return src_insert->get_info()  == node->get_info();
     });

     auto node_insert2 = grafoRed.search_node([=](auto node){
        return tgt_insert->get_info()  == node->get_info();

     });

     ArcoInfo arc_final = {min_distance, 2, 0.0};
     return {node_insert1, node_insert2, arc_final};
}

void conneteThisArcs(GenericGraph<NodoRed>& grafoRed,TrackNode infoArc){
    
    NodoRedPtr * node_insert1 = std::get<0>(infoArc);
    NodoRedPtr * node_insert2 = std::get<1>(infoArc);
    ArcoInfo arc_final = std::get<2>(infoArc);

    NodoRedPtr *nodo_g1;
    NodoRedPtr *nodo_g3;

    grafoRed.for_each_node([&node_insert1, &nodo_g1](auto* node1) {
        if (node_insert1->get_info() == node1->get_info()){
            nodo_g1 = node1;
        }
    }); 

    grafoRed.for_each_node([&node_insert2, &nodo_g3](auto* node1) {
        if (node_insert2->get_info() == node1->get_info()){
            nodo_g3 = node1;
        }
    }); 
    std::string SubId;
    int consume = 0;
    if (auto casa = std::dynamic_pointer_cast<Casa>(nodo_g3->get_info())) {

        auto casa2 = std::dynamic_pointer_cast<Casa>(nodo_g1->get_info());
        casa2->setIdSubEstacion(casa->getIdSubEstacion());
        SubId = casa->getIdSubEstacion();
        consume = casa2->getConsumption();

    }else if(auto subEstacion = std::dynamic_pointer_cast<SubEstacion>(nodo_g3->get_info())){

        auto casa2 = std::dynamic_pointer_cast<Casa>(nodo_g1->get_info());
        casa2->setIdSubEstacion(subEstacion->getId());
        SubId = subEstacion->getId();
        consume = casa2->getConsumption();
    }
   
    grafoRed.for_each_node([&SubId, &consume, &arc_final](auto* node) {
        if(auto subEstacion = std::dynamic_pointer_cast<SubEstacion>(node->get_info())){
            if(subEstacion->getId() == SubId){
                // std::cout<<subEstacion->getname()<<"########################"<<std::endl;
                subEstacion->setCurrentCharge(subEstacion->getCurrentCharge() - consume);
                if(subEstacion->getCurrentCharge() < 0){
                    std::cout<<"Subestacion "<<subEstacion->getname()<<" no tiene suficiente carga para esta casa \n";
                    arc_final.costo = 0;
                }
            }
        }
    }); 
    
    grafoRed.insert_arc(nodo_g3, nodo_g1, arc_final);
}



void GraphHandle::best_connectivity(
    GenericGraph<NodoRed>& grafoRed,
    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* node_to_insert
) {

    if (node_to_insert->get_info()->getTipo() != "Casa") return;

    Designar::Digraph<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass> grafoRed2;
    grafoRed.copy_graph(grafoRed, grafoRed2);

    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* nodo_g2 = nullptr;
    grafoRed2.for_each_node([&](auto* node) {
        if (node->get_info() == node_to_insert->get_info()) {
            nodo_g2 = node;
        }
    });

    if (!nodo_g2) {
        std::cerr << "Nodo no encontrado en copia." << std::endl;
        return;
    }

    grafoRed2.for_each_node([&](auto* node) {
        if (node != nodo_g2) {
            double dx = nodo_g2->get_info()->getX() - node->get_info()->getX();
            double dy = nodo_g2->get_info()->getY() - node->get_info()->getY();
            double dist = std::sqrt(dx*dx + dy*dy);

            ArcoInfo arc_inset = {dist, 0, 0.0};
            grafoRed2.insert_arc(nodo_g2, node, arc_inset);
        }
    });

    auto dijkstra_tree = Designar::Dijkstra<GenericGraph<NodoRed>, Weight, std::less<int>>()
    .build_min_path_tree(grafoRed2, nodo_g2);

    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* closest_node = nullptr;
    double min_distance = 1e12;

    dijkstra_tree.for_each_node([&](auto* node) {
        if (node->get_info()->getTipo() == "SubEstacion") {
            auto path = get_path_to(dijkstra_tree, node);
            double dist_total = 0.0;

            for (size_t i = 0; i < path.size() - 1; ++i) {
                auto* src = path[i];
                auto* tgt = path[i+1];
                dijkstra_tree.for_each_arc([&](auto* arc) {
                    if (arc->get_src_node() == src && arc->get_tgt_node() == tgt) {
                        dist_total += arc->get_info().distance;
                    }
                });
            }

            if (dist_total < min_distance) {
                min_distance = dist_total;
                closest_node = (path.size() >= 2) ? path[1] : node;
            }
        }
    });

    if (closest_node) {
        auto* casa_real = grafoRed.search_node([&](auto n) {
            return n->get_info() == node_to_insert->get_info();
        });

        auto* destino_real = grafoRed.search_node([&](auto n) {
            return n->get_info() == closest_node->get_info();
        });

        if (casa_real && destino_real) {
            double dx = casa_real->get_info()->getX() - destino_real->get_info()->getX();
            double dy = casa_real->get_info()->getY() - destino_real->get_info()->getY();
            double dist = std::sqrt(dx*dx + dy*dy);
            ArcoInfo arc_final = {dist, 0, 0.0};
            grafoRed.insert_arc(casa_real, destino_real, arc_final);
        }
    } else {
        std::cerr << "No se encontró subestación conectable desde esta casa." << std::endl;
    }

}


void GraphHandle::best_connectivity2(
    GenericGraph<NodoRed>& grafoRed
) {

    GraphHandle handler;
    handler.remove_connection(grafoRed);
    Designar::Digraph<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass> grafoRed_aux;
    grafoRed.copy_graph(grafoRed, grafoRed_aux);

    grafoRed_aux.for_each_node([&grafoRed_aux, &grafoRed](Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* node_to_insert) {
        if (node_to_insert->get_info()->getTipo() != "Casa") return;

        Designar::Digraph<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass> grafoRed2;
        grafoRed_aux.copy_graph(grafoRed_aux, grafoRed2);

        Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* nodo_g2 = nullptr;
        grafoRed2.for_each_node([&](auto* node) {
            if (node->get_info() == node_to_insert->get_info()) {
                nodo_g2 = node;
            }
        });

        if (!nodo_g2) {
            std::cerr << "Nodo no encontrado en copia." << std::endl;
            return;
        }

        grafoRed2.for_each_node([&](auto* node) {
            if (node != nodo_g2) {
                double dx = nodo_g2->get_info()->getX() - node->get_info()->getX();
                double dy = nodo_g2->get_info()->getY() - node->get_info()->getY();
                double dist = std::sqrt(dx*dx + dy*dy);

                ArcoInfo arc_inset = {dist, 0, 0.0};
                grafoRed2.insert_arc(nodo_g2, node, arc_inset);
            }
        });

        auto dijkstra_tree = Designar::Dijkstra<GenericGraph<NodoRed>, Weight, std::less<int>>()
        .build_min_path_tree(grafoRed2, nodo_g2);

        Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* closest_node = nullptr;
        double min_distance = 1e12;

        dijkstra_tree.for_each_node([&](auto* node) {
            if (node->get_info()->getTipo() == "SubEstacion") {
                auto path = get_path_to(dijkstra_tree, node);
                double dist_total = 0.0;

                for (size_t i = 0; i < path.size() - 1; ++i) {
                    auto* src = path[i];
                    auto* tgt = path[i+1];
                    dijkstra_tree.for_each_arc([&](auto* arc) {
                        if (arc->get_src_node() == src && arc->get_tgt_node() == tgt) {
                            dist_total += arc->get_info().distance;
                        }
                    });
                }

                if (dist_total < min_distance) {
                    min_distance = dist_total;
                    closest_node = (path.size() >= 2) ? path[1] : node;
                }
            }
        });

        if (closest_node) {
            auto* casa_real = grafoRed_aux.search_node([&](auto n) {
                return n->get_info() == node_to_insert->get_info();
            });

            auto* destino_real = grafoRed_aux.search_node([&](auto n) {
                return n->get_info() == closest_node->get_info();
            });

            if (casa_real && destino_real) {
                double dx = casa_real->get_info()->getX() - destino_real->get_info()->getX();
                double dy = casa_real->get_info()->getY() - destino_real->get_info()->getY();
                double dist = std::sqrt(dx*dx + dy*dy);
                ArcoInfo arc_final = {dist, 0, 0.0};
                grafoRed_aux.insert_arc(casa_real, destino_real, arc_final);

                auto* casa_real_final = grafoRed.search_node([&](auto n) {
                    return n->get_info() == node_to_insert->get_info();
                });
    
                auto* destino_real_final = grafoRed.search_node([&](auto n) {
                    return n->get_info() == closest_node->get_info();
                });


                grafoRed.insert_arc(destino_real_final, casa_real_final, arc_final);
            }
        } else {
            std::cerr << "No se encontró subestación conectable desde esta casa." << std::endl;
        }



    });

    //grafoRed = grafoRed_aux;


}

 // Para std::shared_ptr

void GraphHandle::remove_connection(GenericGraph<NodoRed>& grafoRed) {
   
    using ArcType = Designar::DigraphArc<
        Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>,
        std::shared_ptr<NodoRed>,
        ArcoInfo,
        Designar::EmptyClass
    >;
    
    std::vector<ArcType*> arcs_to_remove;

    grafoRed.for_each_arc([&](auto* arc) {
        if (arc->get_src_node()->get_info()->getTipo() != "Estacion") {
            arcs_to_remove.push_back(arc);
        }
    });

    for (auto* arc : arcs_to_remove) {
        grafoRed.remove_arc(arc);
    }
}

void GraphHandle::connect_graph(GenericGraph<NodoRed>& grafoRed, std::shared_ptr<NodoRed> a, std::shared_ptr<NodoRed> b) {

    


    auto* destino_real = grafoRed.search_node([&](auto node) {
            return node->get_info() == a;
    });

    auto* destino_real_2 = grafoRed.search_node([&](auto node_) {
            return node_->get_info() == b;
        }
    );

    if (!destino_real || !destino_real_2) {
        std::cerr << "Error: No se encontraron ambos nodos en el grafo\n";
       
        return;
    }
    if(destino_real->get_info()->getTipo()  == "Estacion"){
        if(destino_real_2->get_info()->getTipo()  != "SubEstacion"){
            
            std::cerr << "Error: No se puede conectar estacion a subestacion\n";
            return;
        }    
    }

    if(destino_real->get_info()->getTipo()  == "Casa"){
        if(destino_real_2->get_info()->getTipo()  != "Casa"){
            std::cerr << "Error: Solo se puede conectar casa a casa\n";
            
            return;
        }    
    }

    if(destino_real->get_info()->getTipo()  == "SubEstacion"){
        if(destino_real_2->get_info()->getTipo()  != "Casa"){
            std::cout << "Error: Solo se puede conectar subestacion a casa\n";
            return;
        }    
    }

    
    if(destino_real->get_info()->getTipo()  == "Casa" && destino_real_2->get_info()->getTipo()  == "Casa"){
       
        if (auto casa = std::dynamic_pointer_cast<Casa>(destino_real->get_info())) {
           
            if(casa->getIdSubEstacion() == "0"){
                std::cout<<"la primer casa no tiene id asignado\n";
                return;
            }

            if (auto casa2 = std::dynamic_pointer_cast<Casa>(destino_real_2->get_info())) {
                
                if(casa->getIdSubEstacion() != "0" && casa2->getIdSubEstacion() != "0"){
                    std::cout << "ambas casas tienen id asignado ** casa1: "<<casa->getIdSubEstacion()<<" casa2: "<<casa2->getIdSubEstacion()<<"\n";
                    return;
                }
            }
            
        }

     
    }   
    
    if(destino_real->get_info()->getTipo()  == "SubEstacion" && destino_real_2->get_info()->getTipo()  == "Casa"){
        
        if (auto casa = std::dynamic_pointer_cast<Casa>(destino_real_2->get_info())) {
           
            if(casa->getIdSubEstacion() != "0"){
                std::cout<<"trato de conectar a una casa con id asignado\n";
                return;
            }

            
        }

        if(auto sub_estacion = std::dynamic_pointer_cast<SubEstacion>(destino_real->get_info())){
            if(sub_estacion->getIdEstacionPertenece() == "0"){
                std::cout<<"estacion con id 0\n";
                return;
            }
        }

     
    }  
    if(destino_real->get_info()->getTipo()  == "Estacion" && destino_real_2->get_info()->getTipo()  == "SubEstacion"){
        std::cout<<"entro 1 casa!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        if(auto sub_estacion = std::dynamic_pointer_cast<SubEstacion>(destino_real_2->get_info())){
            std::cout<<"entro 2 casa\n";
            if(sub_estacion->getIdEstacionPertenece() != "0"){
                std::cout<<"trato de conectar a una SubEstacion con id asignado\n";
                return;
            }

            
        }
    }  



    bool flag = check_charge(grafoRed, a, b);
    if(!flag){
        std::cout << "No hay suficiente carga\n" << "False" << std::endl;
        return;
    }else{
        std::cout << "suficiente carga\n" << "true" << std::endl;
    }
 


   
    // std::cout<< destino_real->get_info()->getTipo()<<" ************\n";
    // std::cout<< destino_real_2->get_info()->getTipo()<<" ************\n";


    if (destino_real->get_info()->getTipo() == "SubEstacion" && destino_real_2->get_info()->getTipo() == "Casa") {
       
        auto casaDestino = std::dynamic_pointer_cast<Casa>(destino_real_2->get_info());
        auto subEstacion = std::dynamic_pointer_cast<SubEstacion>(destino_real->get_info());
        
        if (casaDestino && subEstacion) {
            casaDestino->setIdSubEstacion(subEstacion->getId());
        } else {
            std::cerr << "Error: Tipos de nodos incompatibles." << std::endl;
        }
    }else if(destino_real->get_info()->getTipo() == "Casa" && destino_real_2->get_info()->getTipo() == "Casa"){ 
       
        auto casaDestino = std::dynamic_pointer_cast<Casa>(destino_real->get_info());
        auto casaDestino2 = std::dynamic_pointer_cast<Casa>(destino_real_2->get_info());
        
        if (casaDestino && casaDestino2) {
            // std::cout <<"este es el id subestacion: "<< casaDestino->getIdSubEstacion()<<std::endl;
            // std::cout <<"este es el id subestacion: "<< casaDestino2->getIdSubEstacion()<<std::endl;
            casaDestino2->setIdSubEstacion(casaDestino->getIdSubEstacion());
        } else {
            std::cerr << "Error: Tipos de nodos incompatibles." << std::endl;
        }
    }else if(destino_real->get_info()->getTipo() == "Estacion" && destino_real_2->get_info()->getTipo() == "SubEstacion"){
       
        auto estacion = std::dynamic_pointer_cast<Estacion>(destino_real->get_info());
        auto subEstacion = std::dynamic_pointer_cast<SubEstacion>(destino_real_2->get_info());
        
        if (estacion && subEstacion) {
            subEstacion->setIdEstacion(estacion->getId());
        } else {
            std::cerr << "Error: Tipos de nodos incompatibles." << std::endl;
        }
    }


    

    

    ArcoInfo arc_inset = {distance(a, b) , 1, 0.0};

    auto arc_inserted = grafoRed.insert_arc( destino_real, destino_real_2, arc_inset );

    bool has_cycle = Designar::has_cycle(grafoRed);
    if (has_cycle) {
        grafoRed.remove_arc(arc_inserted);
        return;
    }
}

bool GraphHandle::check_charge(
    GenericGraph<NodoRed>& grafoRed,
    std::shared_ptr<NodoRed> nodo1,
    std::shared_ptr<NodoRed> nodo2
) {

    auto* destino_real = grafoRed.search_node([&](auto node) {
        return node->get_info() == nodo1;
    });
    
    auto* destino_real_2 = grafoRed.search_node([&](auto node_) {
        return node_->get_info() == nodo2;
    });
    
    
    if (!destino_real || !destino_real_2) {
        return false;
    }
    
    // nodo 1 es una subestacion y nodo 2 es una casa
    if (auto sub_estacion = std::dynamic_pointer_cast<SubEstacion>(destino_real->get_info())) {
        float charge = sub_estacion->getCurrentCharge();
        if (charge <= 0) {
            return false;
        }
    
        if (auto casa = std::dynamic_pointer_cast<Casa>(destino_real_2->get_info())) {
            float casa_consumption = casa->getConsumption();
            if (casa_consumption <= charge) {
                float new_charge = charge - casa_consumption;
                sub_estacion->setCurrentCharge(new_charge);
                return true;
            }
            return false;
        }
    }
    
    // nodo 1 es una casa y nodo 2 es una casa
    if (auto casa = std::dynamic_pointer_cast<Casa>(destino_real->get_info())) {
        auto casa2 = std::dynamic_pointer_cast<Casa>(destino_real_2->get_info());
        if (!casa2) {
            return false;
        }
    
        auto sub_estacion_id = casa->getIdSubEstacion();
    
        auto* node_found = grafoRed.search_node([&](auto node) {
            auto node_info = node->get_info();
            if (auto sub_est = std::dynamic_pointer_cast<SubEstacion>(node_info)) {
                return sub_est->getId() == sub_estacion_id;
            }
            return false;
        });
    
        if (!node_found) {
            return false;
        }
    
        auto sub_est = std::dynamic_pointer_cast<SubEstacion>(node_found->get_info());
        if (!sub_est) {
            return false;
        }
    
        float casa_consumption = casa2->getConsumption();
        float current_charge = sub_est->getCurrentCharge();
    
        if (casa_consumption <= current_charge) {
            float new_charge = current_charge - casa_consumption;
            sub_est->setCurrentCharge(new_charge);
            std::cout << "Casa insertada y se restó a la subestación consumo: " 
                      << casa_consumption << " sub_est: " << sub_est->getCurrentCharge() << std::endl;
            return true;
        }
    
        std::cout << "Carga insuficiente en la subestación: " << sub_est->getId()<< "carga actual: " << sub_est->getCurrentCharge() << "casa consumo: " << casa_consumption << "" << std::endl;
        return false;
    }

    if (auto estacion = std::dynamic_pointer_cast<Estacion>(destino_real->get_info())) {
        float charge = estacion->getCurrentCharge();
        if (charge <= 0) {
            return false;
        }
    
        if (auto sub_estacion = std::dynamic_pointer_cast<SubEstacion>(destino_real_2->get_info())) {
            float sub_estacion_consumption = sub_estacion->getmaxCapacity();
            if (sub_estacion_consumption <= charge) {
                float new_charge = charge - sub_estacion_consumption;
                estacion->setCurrentCharge(new_charge);
                return true;
            }
            return false;
        }
    }

    
    
    return true;

}

std::tuple<bool, std::string > GraphHandle::check_connectivity_estacion_subestacion(GenericGraph<NodoRed>& grafoRed) {
    bool all_connected = true;
    std::string message = "";

    // Verificar que todas las Estaciones estén conectadas a SubEstaciones
    grafoRed.for_each_node([&](auto* node) {
        if (node->get_info()->getTipo() == "Estacion") {
            bool connected_to_substation = false;
            
            
            auto in_arcs = get_in_arcs(grafoRed, node);
            for (auto* arc : in_arcs) {
                if (arc->get_src_node()->get_info()->getTipo() == "SubEstacion") {
                    connected_to_substation = true;
                    break;
                }
            }
            
        
            if (!connected_to_substation) {
                auto out_arcs = get_out_arcs(grafoRed, node);
                for (auto* arc : out_arcs) {
                    if (arc->get_tgt_node()->get_info()->getTipo() == "SubEstacion") {
                        connected_to_substation = true;
                        break;
                    }
                }
            }

            if (!connected_to_substation) {
                std::cout << "[ERROR] Estación '" << node->get_info()->getname() 
                          << "' no está conectada a ninguna SubEstación" << std::endl;
                all_connected = false;
            }
        }
    });

    
    grafoRed.for_each_node([&](auto* node) {
        if (node->get_info()->getTipo() == "SubEstacion") {
            bool connected_to_station = false;
            
            
            auto in_arcs = get_in_arcs(grafoRed, node);
            for (auto* arc : in_arcs) 
            {
                if (arc->get_src_node()->get_info()->getTipo() == "Estacion") 
                {
                    connected_to_station = true;
                    break;
                }
            }
            
     
            if (!connected_to_station) {
                auto out_arcs = get_out_arcs(grafoRed, node);
                for (auto* arc : out_arcs) {
                    if (arc->get_tgt_node()->get_info()->getTipo() == "Estacion") {
                        connected_to_station = true;
                        break;
                    }
                }
            }

            if (!connected_to_station) 
            {
                std::cout << "[ERROR] SubEstación '" << node->get_info()->getname() 
                          << "' no está conectada a ninguna Estación" << std::endl;

                message = "No se conectaron todas las Estaciones a SubEstaciones";
                all_connected = false;
            }
        }
    });

    return {all_connected, message};
 
}

std::vector<TrackNode> GraphHandle::lookingNodes( GenericGraph<NodoRed>& grafoRed, std::vector<TrackNode> unconnetedArcs) {
    
    grafoRed.for_each_node([&unconnetedArcs, &grafoRed ,this](auto* node) {

        auto in_arcs = get_in_arcs(grafoRed, node);
        int count = 0;
        for (auto* arc : in_arcs){
            if(arc != nullptr){
                count += 1;
            }
           
        }
        if(count == 0){
            if(node->get_info()->getTipo() != "Estacion" and node->get_info()->getTipo() != "SubEstacion"){
               
                // std::cout<<node->get_info()->getTipo()<<"----node##################\n";
                // std::cout<<node->get_info()->getname()<<"----node########11111111##########\n";
                unconnetedArcs.push_back(min_connectivity(grafoRed, node));
                //std::cout<<node->get_info()->getname()<<"----node########11111111##########\n";

                //elimina si es mullptr
                if(std::get<0>(unconnetedArcs.back()) == nullptr){
                    unconnetedArcs.pop_back();
                }            
            }
        }
    });

    int distance = 90000000;
    TrackNode insert_this_arc_graphRed;

    for(auto arc : unconnetedArcs){

        if(distance > std::get<2>(arc).distance){
            distance = std::get<2>(arc).distance;
            insert_this_arc_graphRed = arc;
        }
    }
    if(distance != 90000000){
        conneteThisArcs(grafoRed, insert_this_arc_graphRed);
    }
    return unconnetedArcs;
}

std::vector<TrackNode> GraphHandle::lookingNodes2( GenericGraph<NodoRed>& grafoRed,GenericGraph<NodoRed>& grafoAlone, std::vector<TrackNode> unconnetedArcs, std::string last_s_id, std::shared_ptr<NodoRed> best_subE) {
    std::cout<<"here\n";
    int contadordev = 0;

    std::vector<Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>*> unnconnetedNodes;

        grafoRed.for_each_node([&contadordev,&unnconnetedNodes, &grafoRed, &grafoAlone, this](auto* node) {
            auto in_arcs = get_in_arcs(grafoRed, node);
            int count = 0;
            for (auto* arc : in_arcs){
                if(arc != nullptr){
                    count += 1;
                }
            }

            if(count == 0){

                unnconnetedNodes.push_back(node);
                std::cout << node->get_info()->getX() << " " << node->get_info()->getY() << std::endl;
            }
            
        });

        std::cout << "=== Contenido de unnconnetedNodes No ordenados ===" << std::endl;    
    for (const auto* node : unnconnetedNodes) {
        
        std::cout << "(" << node->get_info()->getX() << ", " << node->get_info()->getY() << ")"<< std::endl;
                 
    }

        std::sort(
            unnconnetedNodes.begin(),
            unnconnetedNodes.end(),
            [best_subE](const auto* a, const auto* b) {  // Capturamos nodoReferencia
                // Calcular distancia de 'a' al nodoReferencia
                double distA = std::hypot(
                    a->get_info()->getX() - best_subE->getX(),
                    a->get_info()->getY() - best_subE->getY()
                );
                // Calcular distancia de 'b' al nodoReferencia
                double distB = std::hypot(
                    b->get_info()->getX() - best_subE->getX(),
                    b->get_info()->getY() - best_subE->getY()
                );
                // Ordenar de menor a mayor distancia
                return distA < distB;
            }
        );

    std::cout << "=== Contenido de unnconnetedNodes ordenados ===" << std::endl;    
    for (const auto* node : unnconnetedNodes) {
        
        std::cout << "(" << node->get_info()->getX() << ", " << node->get_info()->getY() << ")"<< std::endl;
                 
    }



    //grafoRed.for_each_node([&contadordev,&unconnetedArcs, &grafoRed, &grafoAlone, this](auto* node) {

    for (const auto* node : unnconnetedNodes) {        

        auto in_arcs = get_in_arcs(grafoRed, node);
        int count = 0;
        for (auto* arc : in_arcs){
            if(arc != nullptr){
                count += 1;
            }
        }
        //auto* nodo_send = grafoRed.search_node([&](auto node_) {
        //    return node_->get_info() == node->get_info();
        //}
        //);

        if(count == 0){
            if(node->get_info()->getTipo() != "Estacion" and node->get_info()->getTipo() != "SubEstacion"){
                contadordev += 1;
                auto nodo = node->get_info();

            
                const auto& node1 = grafoAlone.insert_node(nodo);  

                

                // std::cout<<estacion->get_info()->getTipo()<<"----node##################\n";
               

               
                // std::cout<<node->get_info()->getTipo()<<"----node##################\n";
                // std::cout<<node->get_info()->getname()<<"----node##################\n";

                auto result = min_connectivity2(grafoAlone, node1);
                
                auto node1_insert = std::get<0>(result);  // Primer elemento
                auto node2_insert = std::get<1>(result);  // Segundo elemento
                auto arc_insert   = std::get<2>(result);  // Tercer elemento

               

                // std::cout << "Dentro de lookin devuelve: " << node1_insert->get_info()->getname() << std::endl;
                // std::cout << "Dentro de lookin devuelve: " << node2_insert->get_info()->getname() << std::endl;


                auto n1 = grafoRed.search_node([&](auto node_) {
                    return node_->get_info()->getname() == node1_insert->get_info()->getname()
                           && node_->get_info()->getTipo() == node1_insert->get_info()->getTipo()
                           && isAlmostEqual(node_->get_info()->getX(), node1_insert->get_info()->getX())
                           && isAlmostEqual(node_->get_info()->getY(), node1_insert->get_info()->getY());
                });

                auto n2 = grafoRed.search_node([&](auto node_) {
                   return node_->get_info()->getname() == node2_insert->get_info()->getname()
                           && node_->get_info()->getTipo() == node2_insert->get_info()->getTipo()
                           && isAlmostEqual(node_->get_info()->getX(), node2_insert->get_info()->getX())
                           && isAlmostEqual(node_->get_info()->getY(), node2_insert->get_info()->getY());
                });
                 
               

                // grafoRed.for_each_node([&](auto* node) {
                //     std::cout << "Dentro de lookin3 grafored: " << node->get_info()->getname() << std::endl;
                // });




                unconnetedArcs.push_back({n1, n2, arc_insert});
            }
        }
    //});
    }

    // std::cout << "=== Contenido de unconetedArcs ===" << std::endl;
    
        
        Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* sub_estacion_nodo = nullptr;

        auto estacion_ptr = find_subestacion(grafoAlone);
        if (estacion_ptr) {
            sub_estacion_nodo = grafoRed.search_node([&](auto node) {
                return node->get_info() == estacion_ptr;
            });
        }


        //std::cout << "unconetedArcs.size(): " << unconnetedArcs.size() << std::endl;
        for (const auto& arc : unconnetedArcs) {
            auto source = std::get<0>(arc);
            auto target = std::get<1>(arc);
            
            
            if (auto casa = std::dynamic_pointer_cast<Casa>(target->get_info())) {
                auto casa2 = std::dynamic_pointer_cast<Casa>(source->get_info());
        
                casa2->setIdSubEstacion(casa->getIdSubEstacion());
            }else if(auto subEstacion = std::dynamic_pointer_cast<SubEstacion>(target->get_info())){
                auto casa2 = std::dynamic_pointer_cast<Casa>(source->get_info());
                sub_estacion_nodo = target;
        
                casa2->setIdSubEstacion(subEstacion->getId());
        
            }

           
   
        

        
        //std::cout << "Info nodo: "<< sub_estacion_nodo->get_info()->getname() << std::endl;
        

        if(sub_estacion_nodo != nullptr){
            
            auto subEstacion_nodo_cast = std::dynamic_pointer_cast<SubEstacion>(sub_estacion_nodo->get_info());
            
            auto casa_nodo_cast = std::dynamic_pointer_cast<Casa>(source->get_info());
            float charge = subEstacion_nodo_cast->getCurrentCharge() - casa_nodo_cast->getConsumption();
            
           
            if(casa_nodo_cast->getConsumption() <= subEstacion_nodo_cast->getCurrentCharge()){
                
                grafoRed.insert_arc(target, source, std::get<2>(arc));
                subEstacion_nodo_cast->setCurrentCharge(charge);
            }else{
                casa_nodo_cast->setIdSubEstacion("0");

                std::vector<TrackNode> unconetedArcs_aux;
                
                auto nodo =  find_subestacion(grafoRed);
                
                //std::cout<< "nodo--------------: " << nodo->getname() << std::endl;
                auto grafo = GraphHandlebfs_charge(grafoRed,nodo);

                //std::cout << "contador: "<< contador<< "hasta aqui llego ********\n";  
                auto sb_ptr =  std::dynamic_pointer_cast<SubEstacion>(find_subestacion(grafo));
                
                if(sb_ptr != nullptr){
                    
                    if(last_s_id != sb_ptr->getId()){
                        unconetedArcs_aux = lookingNodes2(grafoRed, grafo, unconetedArcs_aux, sb_ptr->getId(), nodo);
                    }else{
                        return unconetedArcs_aux;
                    }

                    
                }    
            }
    
        
        }
        else{
            std::cout << "Subestacion no encontrada #" << std::endl;
        }
        
        // std::cout<<"charge----: " << charge << std::endl;
        // std::cout<<"carga retar: "<< casa_nodo_cast->getConsumption() << std::endl;
        
        



        std::cout << "Arco: ";
        
        // Verificar si los punteros son válidos antes de acceder
        if (source) {
            std::cout << "Source: " << source->get_info()->getname()  <<"\n";  
        } else {
            std::cout << "Source: nullptr";
        }

        if (target) {
            std::cout << " | Target: " << target->get_info()->getname() << std::endl;
        } else {
            std::cout << " | Target: nullptr" << std::endl;
        }
    }
    std::cout << "Total de arcos desconectados: " << unconnetedArcs.size() << std::endl;

   
    
    return unconnetedArcs;
}




std::shared_ptr<NodoRed> GraphHandle::find_subestacion(GenericGraph<NodoRed>& grafoRed) {
    std::vector<GenericGraph<NodoRed>::Node*> subestaciones;

    grafoRed.for_each_node([&](auto* node) {
        if (node->get_info()->getTipo() == "SubEstacion") {
            subestaciones.push_back(node);
        }
    });

    GenericGraph<NodoRed>::Node* best_sub = nullptr;
    double best_charge = 0;
    
    for(auto& subestacion : subestaciones) {
        auto nodo_shared = subestacion->get_info();
        auto* subestacion_info = dynamic_cast<SubEstacion*>(nodo_shared.get());
        
        if(subestacion_info != nullptr) {
            auto charge = subestacion_info->getCurrentCharge();
            
            if (charge > best_charge || best_sub == nullptr) {
                best_charge = charge;
                best_sub = subestacion;
                std::cout << "Carga subestacion: " << charge << std::endl;
            }
        }
    }

    return best_sub ? best_sub->get_info() : nullptr;
}


GenericGraph<NodoRed> GraphHandle::GraphHandlebfs_charge(
    GenericGraph<NodoRed>& grafoRed,
    std::shared_ptr<NodoRed> best_sub
) {
    
    Designar::Digraph<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass> grafoRed_aux;
    
 
    auto* best_sub_in_graph = grafoRed.search_node([&](auto node) {
        return node->get_info() == best_sub;
    });
    
    if (best_sub_in_graph == nullptr) {
       
        return grafoRed_aux;
    }
    
    grafoRed_aux.insert_node(best_sub);
    
    std::queue<decltype(best_sub_in_graph)> q;
    std::set<std::shared_ptr<NodoRed>> visited;
    
    q.push(best_sub_in_graph);
    visited.insert(best_sub);
    
    while (!q.empty()) {
        auto* current_node = q.front();
        q.pop();
        
        auto current_info = current_node->get_info();
        
        
        auto out_arcs = get_out_arcs(grafoRed, current_node);
        
        for (auto* arc : out_arcs) {
            auto* target_node = arc->get_tgt_node();
            auto target_info = target_node->get_info();
        

            if (visited.find(target_info) == visited.end()) {
                grafoRed_aux.insert_node(target_info);
                visited.insert(target_info);
                q.push(target_node); 
            }
            
            auto arc_info = arc->get_info();
            
            auto* current_aux_node = grafoRed_aux.search_node([&](auto node) {
                return node->get_info() == current_info;
            });
            
            auto* target_aux_node = grafoRed_aux.search_node([&](auto node) {
                return node->get_info() == target_info;
            });
            
            if (current_aux_node != nullptr && target_aux_node != nullptr) {
                grafoRed_aux.insert_arc(current_aux_node, target_aux_node, arc_info);
            }
        }
    }

   
    return grafoRed_aux;
}

GenericGraph<NodoRed> GraphHandle::Copiar_G(GenericGraph<NodoRed>& grafoRed) {
    GenericGraph<NodoRed> grafo_saved; 
    std::unordered_map<
        Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>*,
        Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>*
    > node_map; 

    grafoRed.for_each_node([&](auto* original_node) {
        auto cloned_data = original_node->get_info()->clone(); 
        auto new_node = grafo_saved.insert_node(cloned_data);
        // std::cout<<"##############"<<&original_node->get_info()<<std::endl;
        // std::cout<<"##############/////"<<&original_node<<std::endl;
        //std::cout<<"##############"<<&original_node->get_info()<<std::endl;
        //std::cout<<"##############"<<&original_node->get_info()<<std::endl;
        //std::cout<<"##############"<<&cloned_data<<std::endl;
        //std::cout<<"##############"<<&original_node<<std::endl;
        //std::cout<<"##############"<<&new_node<<std::endl;
        //std::cout<<"##############------------#####################"<<std::endl;

        node_map[original_node] = new_node;
    });
    

 
    

    // grafo_saved.for_each_node([&](auto* original_node) {

    //     std::cout<<"##############"<<&original_node->get_info()<<std::endl;
    //     std::cout<<"##############/////"<<&original_node<<std::endl;

    // });

    grafoRed.for_each_arc([&](auto* original_arc) {
        auto src_copy = node_map.at(original_arc->get_src_node());
        auto tgt_copy = node_map.at(original_arc->get_tgt_node());
        grafo_saved.insert_arc(src_copy, tgt_copy, original_arc->get_info());
    });

    return grafo_saved;
}


void GraphHandle::fix_charge(
    GenericGraph<NodoRed>& grafoRed
){

    std::vector<Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>*> overflow_subestaciones;

    grafoRed.for_each_node([&](auto* node) {
        if (node->get_info()->getTipo() == "SubEstacion") {
            auto subestacion = std::dynamic_pointer_cast<SubEstacion>(node->get_info());
            if (subestacion && subestacion->getCurrentCharge() < 0) {
                overflow_subestaciones.push_back(node);
            }
        }
    });

    // if (overflow_subestaciones.empty()) {
    //     return false;
    // }

    //std::cout << "Subestaciones con carga negativa encontradas: " << overflow_subestaciones.size() << std::endl;

    for (auto* node : overflow_subestaciones) {
        auto subestacion = std::dynamic_pointer_cast<SubEstacion>(node->get_info());
        std::string id_estacion = subestacion->getIdEstacionPertenece();

        
        grafoRed.for_each_node([&](auto node_) {
            
        
        //grafoRed.search_node([&](auto node_) {

            std::string tipo = node_->get_info()->getTipo();
            
            if(auto estacion = std::dynamic_pointer_cast<Estacion>(node_->get_info())){
                std::string id = estacion->getId();
                if (node_->get_info()->getTipo() == "Estacion" && id == id_estacion) {

                    auto estacion = std::dynamic_pointer_cast<Estacion>(node_->get_info());
                    float estacion_current_charge = estacion->getCurrentCharge();
                    float subestacion_current_charge = abs(subestacion->getCurrentCharge());
                    //std::cout<<"Llego hasta aqui????????? \n";

                    if(estacion_current_charge - subestacion_current_charge >= 0){

                        //std::cout<<"Llego hasta aqui 2????????? \n";
                        estacion->setCurrentCharge(estacion_current_charge - subestacion_current_charge);
                        subestacion->setCurrentCharge(0);
                        subestacion->setMaxCapacityCharge(subestacion_current_charge);
    
                        std::vector<ArcoInfo> arcos;

                        grafoRed.for_each_arc([&](auto* arc) {
                           
                            auto* src_node = arc->get_src_node();
                            auto* tgt_node = arc->get_tgt_node();                           

                            if(!arc->get_info().costo){
                                if(src_node->get_info()->getTipo() == "SubEstacion" && tgt_node->get_info()->getTipo() == "Casa"){
                                    auto sub = std::dynamic_pointer_cast<SubEstacion>(src_node->get_info());
                                    auto casa = std::dynamic_pointer_cast<Casa>(tgt_node->get_info());
                                    if(sub->getId() == subestacion->getId() && casa->getIdSubEstacion() == subestacion->getId()){
                                        
                                        arc->get_info().costo = 1;

                                    }
                                }
                                else if(src_node->get_info()->getTipo() == "Casa" && tgt_node->get_info()->getTipo() == "Casa"){
                                    auto casa1 = std::dynamic_pointer_cast<Casa>(src_node->get_info());
                                    auto casa2 = std::dynamic_pointer_cast<Casa>(tgt_node->get_info());
                                    if(casa1->getIdSubEstacion() == subestacion->getId() && casa2->getIdSubEstacion() == subestacion->getId()){
                                        
                                        arc->get_info().costo = 1;
                                    }
                                }   
                                
                            }
                        });

                
                    }else{
                        std::cout<< "Retorno false porque la estacion no tiene carga";
                    }
    
                   
                }
            }
            //return true;
            
        });

    }

   
}


void GraphHandle::delete_alone_nodes(
    GenericGraph<NodoRed>& grafoRed
){

    std::vector<Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>*> nodes_to_delete;

    

    
    std::cout << "Entro en la funcion eliminar nodos solitarios" << std::endl;
    grafoRed.for_each_node([&](auto* node) {
        std::cout << "Iteracion" << node->get_info()->getTipo() << std::endl;
        auto in_arcs = get_in_arcs(grafoRed, node);
        auto out_arcs = get_out_arcs(grafoRed, node);

        if (in_arcs.empty() && out_arcs.empty()) {
            nodes_to_delete.push_back(node);
        }
    });

    for (auto* node : nodes_to_delete) {
        grafoRed.remove_node(node);
    }

    std::cout << "salio en la funcion eliminar nodos solitarios" << std::endl;

}

void GraphHandle::delete_one_node_alone(
    GenericGraph<NodoRed>& grafoRed,
    std::string id
) {
    
    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* node_to_delete = nullptr;

    grafoRed.for_each_node([&](auto* node) {
        if (node && node->get_info()) {
            if (auto sub_estacion = std::dynamic_pointer_cast<SubEstacion>(node->get_info())){
                if(sub_estacion->getId() == id){
                    node_to_delete = node;
                    return; 
                }
            }
            else if(auto casa = std::dynamic_pointer_cast<Casa>(node->get_info())){
                if(casa->getId() == id){
                    node_to_delete = node;
                    return;
                }
            }
            else if(auto estacion = std::dynamic_pointer_cast<Estacion>(node->get_info())){
                if(estacion->getId() == id){
                    node_to_delete = node;
                    return; 
                }
            }
           
     
            
        }
    });

    
    if (!node_to_delete) {
        std::cout << "No se encontró ningún nodo con ID: " << id << std::endl;
        return;
    }

   
    auto in_arcs = get_in_arcs(grafoRed, node_to_delete);
    auto out_arcs = get_out_arcs(grafoRed, node_to_delete);

    if (!in_arcs.empty() || !out_arcs.empty()) {
        std::cout << "El nodo con ID " << id << " tiene conexiones y no puede ser eliminado" << std::endl;
        return;
    }


    grafoRed.remove_node(node_to_delete);
    std::cout << "Nodo con ID " << id << " eliminado exitosamente" << std::endl;
}

void GraphHandle::delete_connected_node_alone(
    GenericGraph<NodoRed>& grafoRed,
    std::string id
){
    using NodeType = Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>;
    using ArcType = Designar::DigraphArc<NodeType, std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>;

    
    NodeType* node_to_delete = nullptr;
    std::string parent_id = "";
    enum NodeTypeEnum { ESTACION, SUBESTACION, CASA, NONE } node_type = NONE;
    
    grafoRed.for_each_node([&](auto* node) {
        if (!node || !node->get_info()) return;
        
        if (auto estacion = std::dynamic_pointer_cast<Estacion>(node->get_info())) {
            if (estacion->getId() == id) {
                node_to_delete = node;
                node_type = ESTACION;
            }
        }
        else if (auto subestacion = std::dynamic_pointer_cast<SubEstacion>(node->get_info())) {
            if (subestacion->getId() == id) {
                node_to_delete = node;
                parent_id = subestacion->getIdEstacionPertenece();
                node_type = SUBESTACION;
            }
        }
        else if (auto casa = std::dynamic_pointer_cast<Casa>(node->get_info())) {
            if (casa->getId() == id) {
                node_to_delete = node;
                parent_id = casa->getIdSubEstacion();
                node_type = CASA;
            }
        }
    });

    if (!node_to_delete) {
        std::cerr << "Nodo con id " << id << " no encontrado" << std::endl;
        return;
    }

  
    std::set<NodeType*> nodes_to_disconnect;
    std::set<ArcType*> arcs_to_delete;
    float total_consumo = 0.0f;
    float capacity_to_return = 0.0f;

    // Caso 1: Eliminación de Casa 
    if (node_type == CASA) {
        std::queue<NodeType*> queue;
        queue.push(node_to_delete);
        nodes_to_disconnect.insert(node_to_delete);

        while (!queue.empty()) {
            NodeType* current = queue.front();
            queue.pop();

          
            if (auto casa = std::dynamic_pointer_cast<Casa>(current->get_info())) {
                total_consumo += casa->getConsumption();
            }

          
            auto out_arcs = get_out_arcs(grafoRed, current);
            for (auto* arc : out_arcs) {
                if (!arc) continue;
                
                NodeType* target = arc->get_tgt_node();
                if (nodes_to_disconnect.find(target) == nodes_to_disconnect.end()) {
                    if (std::dynamic_pointer_cast<Casa>(target->get_info())) {
                        nodes_to_disconnect.insert(target);
                        queue.push(target);
                    }
                }
                arcs_to_delete.insert(arc);
            }

           
            auto in_arcs = get_in_arcs(grafoRed, current);
            for (auto* arc : in_arcs) {
                if (!arc) continue;
                
                NodeType* source = arc->get_src_node();
                if (!std::dynamic_pointer_cast<SubEstacion>(source->get_info())) {
                    arcs_to_delete.insert(arc);
                }
            }
        }
    }
    // Caso 2: Eliminación de SubEstación 
    else if (node_type == SUBESTACION) {
        if (auto subestacion = std::dynamic_pointer_cast<SubEstacion>(node_to_delete->get_info())) {
            capacity_to_return = subestacion->getmaxCapacity();
        }

        std::queue<NodeType*> queue;
        queue.push(node_to_delete);
        
        while (!queue.empty()) {
            NodeType* current = queue.front();
            queue.pop();

            auto out_arcs = get_out_arcs(grafoRed, current);
            for (auto* arc : out_arcs) {
                if (!arc) continue;
                
                NodeType* target = arc->get_tgt_node();
                arcs_to_delete.insert(arc);
                
              
               
                queue.push(target);
                
            }
        }

        
        auto in_arcs = get_in_arcs(grafoRed, node_to_delete);
        arcs_to_delete.insert(in_arcs.begin(), in_arcs.end());
    }
    // Caso 3: Eliminación de Estación 
    else if (node_type == ESTACION) {
        // BFS para encontrar toda la jerarquía de arcos
        std::queue<NodeType*> queue;
        queue.push(node_to_delete);
        
        while (!queue.empty()) {
            NodeType* current = queue.front();
            queue.pop();

            auto out_arcs = get_out_arcs(grafoRed, current);
            for (auto* arc : out_arcs) {
                if (!arc) continue;
                
                NodeType* target = arc->get_tgt_node();
                arcs_to_delete.insert(arc);
                
              
               
                queue.push(target);
                
            }
        }

      
        auto in_arcs = get_in_arcs(grafoRed, node_to_delete);
        arcs_to_delete.insert(in_arcs.begin(), in_arcs.end());
    }

    // Devolver capacidades/consumos 
    if (node_type == CASA || node_type == SUBESTACION) {
        grafoRed.for_each_node([&](auto* node) {
            if (!node || !node->get_info()) return;
            
            if (node_type == CASA) {
                if (auto subestacion = std::dynamic_pointer_cast<SubEstacion>(node->get_info())) {
                    if (subestacion->getId() == parent_id) {
                        float current = subestacion->getCurrentCharge();
                        subestacion->setCurrentCharge(current + total_consumo);
                    }
                }
            }
            else if (node_type == SUBESTACION) {
                if (auto estacion = std::dynamic_pointer_cast<Estacion>(node->get_info())) {
                    if (estacion->getId() == parent_id) {
                        float current = estacion->getCurrentCharge();
                        estacion->setCurrentCharge(current + capacity_to_return);
                    }
                }
            }
        });
    }

    
    for (auto* arc : arcs_to_delete) {
        try {
            grafoRed.remove_arc(arc);
        } catch (const std::exception& e) {
            std::cerr << "Error eliminando arco: " << e.what() << std::endl;
        }
    }

  
    try {
        grafoRed.remove_node(node_to_delete);
        
        // Reporte de operación
        std::cout << "Nodo " << id << " eliminado correctamente" << std::endl;
        std::cout << "Se eliminaron " << arcs_to_delete.size() << " arcos de conexión" << std::endl;
        
        if (node_type == CASA) {
            std::cout << "Se devolvió " << total_consumo 
                      << " de consumo a la subestación " << parent_id << std::endl;
        }
        else if (node_type == SUBESTACION) {
            std::cout << "Se devolvió " << capacity_to_return 
                      << " de capacidad a la estación " << parent_id << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error eliminando nodo: " << e.what() << std::endl;
    }


    grafoRed.for_each_node([&](auto* node) {
        auto arc_in = get_in_arcs(grafoRed, node);
        auto arc_out = get_out_arcs(grafoRed, node);
        
        if (arc_in.empty() && arc_out.empty()) {

            if (auto subestacion = std::dynamic_pointer_cast<SubEstacion>(node->get_info())) {
                subestacion->setIdEstacion("0");
                subestacion->setCurrentCharge(subestacion->getmaxCapacity());

            }else if (auto casa = std::dynamic_pointer_cast<Casa>(node->get_info())){
                casa->setIdSubEstacion("0");
            }
        }
    });
}