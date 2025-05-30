
#ifndef TOOLS_DISPLAY_HPP  // ← Si no está definido...
#define TOOLS_DISPLAY_HPP  // ← ... lo definimos

#include <nlohmann/json.hpp>
#include <graphalgorithms.hpp>
#include <buildgraph.hpp>
#include "NodeStation.hpp"
#include "NodeSubStation.hpp"
#include "NodeHouse.hpp"
#include "ToolsDisplay.hpp"



using json = nlohmann::json;

bool jsonWrite(std::string archive_name, json jsonGraphRed);

int saveJson(GenericGraph<NodoRed>& grafoRed, std::string dataArchiveName);

bool jsonLoad(std::string archiveName,
        sf::Vector2f relativePos,
        sf::Texture * texCasa,
        sf::Texture * texSub,
        sf::Texture * texEst,
        GenericGraph<NodoRed>& grafoRed,
        sf::RenderWindow& window
    );

#endif