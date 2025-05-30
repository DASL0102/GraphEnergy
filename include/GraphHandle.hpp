// GraphHandle.hpp
#ifndef GRAPHHANDLE_HPP
#define GRAPHHANDLE_HPP

#include "ToolsDisplay.hpp"

class GraphHandle {
public:

    std::tuple<
    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>*,
    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>*,
    ArcoInfo
    > min_connectivity(
    GenericGraph<NodoRed>& grafoRed,
    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* node_to_insert
    );

    std::tuple<
    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>*,
    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>*,
    ArcoInfo
    > min_connectivity2(
    GenericGraph<NodoRed>& grafoRed,
    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* node_to_insert
    );

    // void min_connectivity(
    //     GenericGraph<NodoRed>& grafoRed,
    //     Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* node_to_insert
    // );

    void best_connectivity(
        GenericGraph<NodoRed>& grafoRed,
        Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* node_to_insert
    ); 

    void best_connectivity2(
        GenericGraph<NodoRed>& grafoRed
    
    ); 

    void remove_connection(
        GenericGraph<NodoRed>& grafoRed
    );
    
    void connect_graph(
        GenericGraph<NodoRed>& grafoRed,
         std::shared_ptr<NodoRed> a, 
         std::shared_ptr<NodoRed> b
    );

    std::vector<TrackNode> lookingNodes(
         GenericGraph<NodoRed>& grafoRed,
        std::vector<TrackNode> unconnetedArcs
    );

    std::tuple<bool, std::string >check_connectivity_estacion_subestacion(
        GenericGraph<NodoRed>& grafoRed
    );


    std::shared_ptr<NodoRed> find_subestacion(
        GenericGraph<NodoRed>& grafoRed
    );

    GenericGraph<NodoRed> GraphHandlebfs_charge(
        GenericGraph<NodoRed>& grafoRed,
          std::shared_ptr<NodoRed> best_sub
    );
    
    std::vector<TrackNode> lookingNodes( 
        GenericGraph<NodoRed>& grafoRed,
        GenericGraph<NodoRed>& grafoAlone, 
        std::vector<TrackNode> unconnetedArcs
    );
    std::vector<TrackNode> lookingNodes2( 
        GenericGraph<NodoRed>& grafoRed,
        GenericGraph<NodoRed>& grafoAlone, 
        std::vector<TrackNode> unconnetedArcs,
        std::string last_s_id,
        std::shared_ptr<NodoRed> best_subE
    );

    bool check_charge(
        GenericGraph<NodoRed>& grafoRed,
        std::shared_ptr<NodoRed> nodo1,
        std::shared_ptr<NodoRed> nodo2
    );

    GenericGraph<NodoRed> Copiar_G(
        GenericGraph<NodoRed>& grafoRed
    );

    void fix_charge(
        GenericGraph<NodoRed>& grafoRed
    );

    void delete_alone_nodes(
        GenericGraph<NodoRed>& grafoRed
    );

    void delete_one_node_alone(
        GenericGraph<NodoRed>& grafoRed,
        std::string id
    );

    void delete_connected_node_alone(
        GenericGraph<NodoRed>& grafoRed,
        std::string id
    );


    

};

#endif
