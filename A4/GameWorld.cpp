#include "GameWorld.h"

GameWorld::GameWorld() : mapTree(), shaperTree() {}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(players);
    mapTree.initializeMap(places);
}

Map& GameWorld::getMapTree()
{
    return mapTree;
}
ShaperTree& GameWorld::getShaperTree()
{
    return shaperTree;
}

bool GameWorld::hasAccess(RealmShaper *realmShaper, Isle *isle)
{
    bool hasAccess = false;

    if (!shaperTree.findPlayer(realmShaper->getName())) {
        std::cout << "[Access Control] RealmShaper not found!" << std::endl;
        return hasAccess;
    }

    int playerDepth = shaperTree.getDepth(realmShaper);
    int totalHeight = shaperTree.getDepth();
    int totalMapDepth = mapTree.getDepth();
    int minAccessDepth = mapTree.calculateMinMapDepthAccess(playerDepth, totalHeight, totalMapDepth);

    int isleDepth = mapTree.getIsleDepth(isle);
    if (playerDepth == 0) {
        return true;


    }
    std::cerr << "totalMapDepth: " << mapTree.getDepth() << std::endl;
    if (isleDepth ==3 && minAccessDepth==3) return false;    // Get necessary depth values
    if (isleDepth >= minAccessDepth) hasAccess=true;    // Get necessary depth values
    // Use mapTree.calculateMinMapDepthAccess
    // Use // std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;

    return hasAccess;
}

void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle)
{
    if (!hasAccess(realmShaper, isle)) {
        std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle << std::endl;
        return;
    }


    Item collectedItem = isle->getItem();
    realmShaper->collectItem(collectedItem);

    std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
    std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;

    std::string curName = realmShaper->getCurrentIsle();
    Isle *cur = mapTree.findIsle(curName);
    if (cur) {
        if (cur->decreaseShaperCount()) {
            std::cout << "[Explore Area] Previous Isle " << cur->getName() << " now has " << cur->getShaperCount() << " shapers." << std::endl;
        }
    }

    realmShaper->setCurrentIsle(isle->getName());

    bool isOvercrowded = isle->increaseShaperCount();
    if (isOvercrowded) {
        std::cout << "[Owercrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;
        mapTree.remove(isle);
    }
    // Check if realmShaper has access
    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle << std::endl;
    // If realmShaper has access
    // Visit isle,
    // collect item, x
    // check overcrowding for Isle,
    // delete Isle if necessary

    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
    // Use // std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
    // Use // std::cout << "[Owercrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;

    // You will need to implement a mechanism to keep track of how many realm shapers are at an Isle at the same time
    // There are more than one ways to do this, so it has been left completely to you
    // Use shaperCount, but that alone will not be enough,
    // you will likely need to add attributes that are not currently defined
    // to RealmShaper or Isle or other classes depending on your implementation
}

void GameWorld::craft(RealmShaper *shaper, const std::string &isleName){
    if (!shaper->hasEnoughEnergy()) {
        std::cout << "[Energy] " << shaper->getName() << " does not have enough energy points: " << shaper->getEnergyLevel() << std::endl;
        return;
    }
    std::cout << "[Energy] " << shaper->getName() << " has enough energy points: " << shaper->getEnergyLevel() << std::endl;

    Isle *newIsle = new Isle(isleName);
    mapTree.insert(newIsle);
    newIsle->increaseShaperCount();
    std::cout << "[Craft] " << shaper->getName() << " crafted new Isle " << isleName << std::endl;

    // Use std::cout << "[Energy] " << shaperName << " has enough energy points: " << shaperEnergyLevel << std::endl;
    // Use std::cout << "[Craft] " << shaperName << " crafted new Isle " << isleName << std::endl;
    // Use std::cout << "[Energy] " << shaperName << " does not have enough energy points: " << shaperEnergyLevel << std::endl;
}

void GameWorld::displayGameState()
{
   // mapTree.displayMap();
    //shaperTree.displayTree();
}

// TODO: Implement functions to read and parse Access and Duel logs

void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs)
{
    std::ifstream accessFile(accessLogs);
    std::ifstream duelFile(duelLogs);

    if (!accessFile.is_open() || !duelFile.is_open()) {
        std::cerr << "[Error] Could not open log files." << std::endl;
        return;
    }

    std::string accessLine, duelLine;
    std::getline(duelFile, duelLine);
    int accessCount = 0;


    while (std::getline(accessFile, accessLine)) {
            if (accessCount == 0) {
                accessCount++;
                continue;
            }

        std::istringstream accessStream(accessLine);
        std::string shaperName, isleName;
        accessStream >> shaperName >> isleName;

        RealmShaper *shaper = shaperTree.findPlayer(shaperName);
        Isle *isle = mapTree.findIsle(isleName);

        if (shaper ) {
            if (isle) {
                exploreArea(shaper, isle);
            }else {
                craft(shaper, isleName);
            }
        }
        if (accessCount % 5 == 0 ) {
            if ( std::getline(duelFile, duelLine)) {
                std::istringstream duelStream(duelLine);
                std::string challengerName, result;
                duelStream >> challengerName >> result;

                RealmShaper *challenger = shaperTree.findPlayer(challengerName);
                bool duelResult = (result == "1");

                if (challenger) {
                    shaperTree.duel(challenger, duelResult);
                }

            }

        }
        accessCount++;
    }

    // Process remaining duels
    while (std::getline(duelFile, duelLine)) {
        std::istringstream duelStream(duelLine);
        std::string challengerName, result;
        duelStream >> challengerName >> result;

        RealmShaper *challenger = shaperTree.findPlayer(challengerName);
        bool duelResult = (result == "1");

        if (challenger) {
            shaperTree.duel(challenger, duelResult);

        }
    }

    displayGameState();
    accessFile.close();
    duelFile.close();
    // Read logs
    // For every 5 access, 1 duel happens
    // If there are still duel logs left after every access happens duels happens one after other

    // This function should call exploreArea and craft functions

    // Use displayGameState();
}

void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}