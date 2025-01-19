#include "RealmShapers.h"
#include <cmath>
#include <algorithm>

ShaperTree::ShaperTree() = default;

ShaperTree::~ShaperTree()
{
    for (auto *shaper : realmShapers)
        delete shaper;
    realmShapers.clear();
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    realmShapers.clear();
    for (auto &player : shapers) {
        realmShapers.push_back(std::move(player));
    }
}

int ShaperTree::getSize()
{
    return realmShapers.size();
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    bool isValid = false;

    if ( index >= 0 && index < realmShapers.size()) {
        isValid = true;
    }

    return isValid;
}

void ShaperTree::insert(RealmShaper *shaper)
{
    if (!findPlayer(shaper->getName()))
    {
        realmShapers.push_back(shaper);
    }
}

int ShaperTree::remove(RealmShaper *shaper)
{
    // Make sure tree protects its form (complate binary tree) after deletion of a node
    // return index if found and removed
    // else
    int index = findIndex(shaper);
    if (index == -1) return -1;

    for (int i = index; i < realmShapers.size() - 1; i++)
    {
        realmShapers[i] = realmShapers[i + 1];
    }

    realmShapers.pop_back();
    delete shaper;
    return index;
}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    // return index in the tree if found
    // else
    for (int i = 0; i < realmShapers.size(); ++i)
    {
        if (*realmShapers[i] == *shaper)
            return i;
    }
    return -1;
}

int ShaperTree::getDepth(RealmShaper *shaper)
{
    // return depth of the node in the tree if found
    // else
    int index = findIndex(shaper);
    if (index == -1) return -1;

    int depth = 0;
    while (index > 0) {
        index = (index - 1) / 2;
        depth++;
    }
    return depth;
}

int ShaperTree::getDepth()
{
    // return total|max depth|height of the tree
    if (realmShapers.empty()) return -1;
    return std::floor(std::log2(realmShapers.size())) + 1;
}


RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    int challengerIndex = findIndex(challenger);
    if (challengerIndex == -1 || challengerIndex == 0)
    {
        return *challenger;
    }

    int parentIndex = (challengerIndex - 1) / 2;
    RealmShaper *opponent = realmShapers[parentIndex];

    if (result)
    {
        challenger->gainHonour();
        opponent->loseHonour();
        std::swap(realmShapers[challengerIndex], realmShapers[parentIndex]);
        std::cout << "[Duel] " << challenger->getName() << " won the duel" << std::endl;
        std::cout << "[Honour] New honour points: ";
        std::cout << challenger->getName() << "-" << challenger->getHonour() << " ";
        std::cout << opponent->getName() << "-" << opponent->getHonour() << std::endl;

        if (opponent->getHonour() <= 0)
        {
            std::cout << "[Duel] " << opponent->getName() << " lost all honour, delete" << std::endl;
            remove(opponent);
            return *challenger;
        }
    }
    else
    {
        challenger->loseHonour();
        opponent->gainHonour();
        std::cout << "[Duel] " << challenger->getName() << " lost the duel" << std::endl;

        std::cout << "[Honour] New honour points: ";
        std::cout << challenger->getName() << " - " << challenger->getHonour() << " ";
        std::cout << opponent->getName() << " - " << opponent->getHonour() << std::endl;

        if (challenger->getHonour() <= 0)
        {
            std::cout << "[Duel] " << challenger->getName() << " lost all honour, delete" << std::endl;
            remove(challenger);
            return RealmShaper("nil", 0);
        }

    }
    return *challenger;

}


RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    RealmShaper *parent = nullptr;

    int index = findIndex(shaper);
    if (index <= 0 || !isValidIndex(index))
        return parent;

    int parentIndex = (index - 1) / 2;
    parent = realmShapers[parentIndex];
    return parent;
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    int lowIndex = findIndex(player_low);
    int highIndex = findIndex(player_high);

    if (!player_low || !player_high)
    {
        return;
    }
    if (player_low == player_high)
    {
        return;
    }
    if (lowIndex == -1 || highIndex == -1)
        return;

    std::swap(realmShapers[lowIndex], realmShapers[highIndex]);}



RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    RealmShaper *foundShaper = nullptr;

    // Return the shaper if found
    // Return nullptr if shaper not found
    for (RealmShaper *player : realmShapers)
    {
        if (*player == shaper)
        {
            foundShaper = player;
            break;
        }
    }
    return foundShaper;
}

RealmShaper *ShaperTree::findPlayer(std::string name)
{
    RealmShaper *foundShaper = nullptr;

    for (RealmShaper *player : realmShapers)
    {
        if (player->getName() == name) {

            return player;
        }
    }
    return foundShaper;
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    // Note: Since SheperTree is not an binary search tree,
    // in-order traversal will not give rankings in correct order
    // for correct order you need to implement level-order traversal
    // still you are to implement this function as well
    inOrderHelper(index, result);
    return result;
}


std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement preOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation
    preOrderHelper(index, result);

    return result;
}


std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement postOrderTraversal in tree
    // Add all to a string vector
    // Return the vector
    postOrderHelper(index, result);

    // Define and implement as many helper functions as necessary for recursive implementation
    return result;
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    if (!isValidIndex(index)) return;

    outFile << realmShapers[index]->getName() << std::endl;
    preOrderTraversal(2 * index + 1, outFile);
    preOrderTraversal(2 * index + 2, outFile);
    // write nodes to output file
    // Define and implement as many helper functions as necessary for recursive implementation
}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    // write nodes to output file
    for (RealmShaper *player : realmShapers)
    {
        outFile << *player << std::endl;
    }
    std::cout << "[Output] Breadth-first traversal written to " ;
    std::cout << &outFile << std::endl;
    // Define and implement as many helper functions as necessary
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    int left =  2 * index + 1;
    int right =  2 * index + 2;

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    std::ofstream outFile(filename);
    for (RealmShaper *player : realmShapers)
    {
        outFile << player->getName() << std::endl;
    }
    outFile.close();
    //std::cout << "[Output] Shapers have been written to " << filename << " according to rankings." << std::endl;
    // Use std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;
}

void ShaperTree::writeToFile(const std::string &filename)
{
    std::ofstream outFile(filename);
    auto result = preOrderTraversal(0);
    for (const auto &name : result)
    {
        outFile << name << std::endl;
    }
    outFile.close();
    //std::cout << "[Output] Tree has been written to " << filename << " in pre-order." << std::endl;
    // Use std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;
}
