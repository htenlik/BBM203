#include "Map.h"
#include <functional>

Map::Map()
{
    this->root = nullptr;
}

Map::~Map()
{
    std::function<void(MapNode*)> deleteNodes = [&](MapNode* node) {
        if (!node) return;
        deleteNodes(node->left);
        deleteNodes(node->right);
        delete node;
    };

    deleteNodes(root);
    root = nullptr;
}


void Map::initializeMap(std::vector<Isle *> isles)
{

    initilized = false;
    for (auto &isle : isles) {
        insert(isle);
    }
    initilized = true;
    populateWithItems();
}

MapNode *Map::rotateRight(MapNode *current)
{
    if (!current || !current->left) {
        std::cerr << "[Right Rotation] Called on invalid node!" << std::endl;
        return current;
    }

    MapNode *newRoot = current->left;
    current->left = newRoot->right;
    newRoot->right = current;

    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;    // return necessary new root
    // Use std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
}

MapNode *Map::rotateLeft(MapNode *current)
{
    if (!current || !current->right) {
        std::cerr << "[Left Rotation] Called on invalid node!" << std::endl;
        return current;
    }

    MapNode *newRoot = current->right;
    current->right = newRoot->left;
    newRoot->left = current;

    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;
    // Use std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
}

int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node)
{
    return node ? node->height : 0;
}

MapNode *Map::insert(MapNode *node, Isle *isle) {
    if (!node) {
        return new MapNode(isle);
    }

    if (isle->getName() < node->isle->getName()) {
        node->left = insert(node->left, isle);
    } else if (isle->getName() > node->isle->getName()) {
        node->right = insert(node->right, isle);
    } else {
        return node;
    }

    node->height = std::max(height(node->left), height(node->right)) + 1;

    int balanceFactor = height(node->left) - height(node->right);

    if (balanceFactor>1 || balanceFactor<-1) {
        rebalanceCount++;
        if (rebalanceCount % 3 == 0&&initilized) {
            countgolds = 0;
            postOrderItemDrop(root, countgolds);
            counteinsteins = 0;
            preOrderItemDrop(root, counteinsteins);
            dropItemBFS();
    }

    }
    if (balanceFactor > 1 && isle->getName() < node->left->isle->getName()) {
        return rotateRight(node);
    }

    if (balanceFactor < -1 && isle->getName() > node->right->isle->getName()) {
        return rotateLeft(node);
    }
    if (balanceFactor > 1 && isle->getName() > node->left->isle->getName()) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balanceFactor < -1 && isle->getName() < node->right->isle->getName()) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }


    return node;

}

void Map::insert(Isle *isle)
{
    root = insert((root), isle);
}

MapNode *Map::remove(MapNode *node, Isle *isle)
{
    if (!node) {
        return node;
    }
    if (isle->getName() < node->isle->getName()) {
        node->left = remove(node->left, isle);
    } else if (isle->getName() > node->isle->getName()) {
        node->right = remove(node->right, isle);
    } else {
        if (!node->left && !node->right) {
            delete node;
            node=nullptr;
            return nullptr;
        }
        if (!node->left && node->right) {
            MapNode *temp = node->right;
            delete node;
            node = nullptr;
            return temp;
        }
        if (node->left && !node->right) {
            MapNode *temp = node->left;
            delete node;
            node = nullptr;
            return temp;
        } if (node->left && node->right) {

            MapNode *successorParent = node;
            MapNode *successor = node->left;
            while (successor->right) {
                successorParent = successor;
                successor = successor->right;
            }
            node->isle = new Isle(*successor->isle);
            successorParent->right=successor->left;
            delete successor;
            successor = nullptr;
        }
    }
    if (!node) {
        return node;
    }

    node->height = std::max(height(node->left), height(node->right)) + 1;

    int balanceFactor = height(node->left) - height(node->right);

    if (balanceFactor>1 || balanceFactor<-1) {
        rebalanceCount++;
        if (rebalanceCount % 3 == 0&&initilized) {
            countgolds = 0;
            postOrderItemDrop(root, countgolds);
            counteinsteins = 0;
            preOrderItemDrop(root, counteinsteins);
            dropItemBFS();

        }
    }
    if (balanceFactor > 1 && isle->getName() < node->left->isle->getName()) {
        return rotateRight(node);
    }
    if (balanceFactor < -1 && isle->getName() > node->right->isle->getName()) {

        return rotateLeft(node);
    }
    if (balanceFactor > 1 && isle->getName() > node->left->isle->getName()) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balanceFactor < -1 && isle->getName() < node->right->isle->getName()) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

void Map::remove(Isle *isle)
{
    root = remove(root, isle);
}


void Map::preOrderItemDrop(MapNode *current, int &count)
{
    if (!current) return;
    count++;
    if (count % 5 == 0) {
        current->isle->setItem(Item::EINSTEINIUM);
        std::cout << "[Item Drop] EINSTEINIUM dropped on Isle: " <<current->isle->getName() << std::endl;
        count = 0;
    }

    preOrderItemDrop(current->left, count);

    preOrderItemDrop(current->right, count);
    // Use std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
}

void Map::postOrderItemDrop(MapNode *current, int &count)
{
    if (!current) return;
    postOrderItemDrop(current->left, count);

    postOrderItemDrop(current->right, count);

    count++;
    if (count % 3 == 0) {
        current->isle->setItem(Item::GOLDIUM);
        std::cout << "[Item Drop] GOLDIUM dropped on Isle: " << current->isle->getName()<< std::endl;
        count = 0;
    }    // Use  std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
}

MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    if (!node) {
        return nullptr;
    }

    std::queue<MapNode *> q;
    q.push(node);

    while (!q.empty()) {
        MapNode *current = q.front();
        q.pop();

        if (current->isle->getItem()==Item::EMPTY) {
            return current;
        }

        if (current->left) {
            q.push(current->left);
        }
        if (current->right) {
            q.push(current->right);
        }
    }

    return nullptr;
}

void Map::dropItemBFS()
{
    if (!root) {
        std::cout << "[BFS Drop] No eligible Isle found for AMAZONITE drop." << std::endl;
        return;
    }

    MapNode *targetNode = findFirstEmptyIsle(root);

    if (targetNode) {
        targetNode->isle->setItem(Item::AMAZONITE);
        std::cout << "[BFS Drop] AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
    } else {
        std::cout << "[BFS Drop] No eligible Isle found for AMAZONITE drop." << std::endl;
    }
    // Use std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
    // Use std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;
}

void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

int Map::getDepth(MapNode *node)
{
    if (!node) {
        return -1;
    }
    int depth = 0;
    MapNode *current = root;
    while (current && current != node) {
        if (node->isle->getName() < current->isle->getName()) {
            current = current->left;
        } else {
            current = current->right;
        }
        depth++;
    }
    return current ? depth : -1;
}

// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle) {
    MapNode *current = root;
    int depth = 0;

    while (current) {
        if (isle->getName() == current->isle->getName()) {
            return depth;
        } else if (isle->getName() < current->isle->getName()) {
            current = current->left;
        } else {
            current = current->right;
        }
        depth++;
    }

    return -1;
}

int Map::getDepth() {
    std::function<int(MapNode *)> calculateDepth = [&](MapNode *node) -> int {
        if (!node) {
            return 0;
        }
        int leftDepth = calculateDepth(node->left);
        int rightDepth = calculateDepth(node->right);
        return std::max(leftDepth, rightDepth) + 1;
    };

    return calculateDepth(root);
}

void Map::populateWithItems()
{
    int countgold = 0;
    postOrderItemDrop(root, countgold);
    int counteinstein = 0;
    preOrderItemDrop(root, counteinstein);
    }

Isle *Map::findIsle(Isle isle)
{
    MapNode *current = root;

    while (current) {
        if (isle.getName() == current->isle->getName()) {
            return current->isle;
        } else if (isle.getName() < current->isle->getName()) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return nullptr;
}

Isle *Map::findIsle(std::string name)
{
    MapNode *current = root;

    while (current) {
        if (name == current->isle->getName()) {
            return current->isle;
        } else if (name < current->isle->getName()) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return nullptr;
}

MapNode *Map::findNode(Isle isle)
{
    MapNode *current = root;

    while (current) {
        if (isle.getName() == current->isle->getName()) {
            return current;
        } else if (isle.getName() < current->isle->getName()) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return nullptr;
}

MapNode *Map::findNode(std::string name)
{
    MapNode *current = root;

    while (current) {
        if (name == current->isle->getName()) {
            return current;
        } else if (name < current->isle->getName()) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return nullptr;
}

void Map::display(MapNode *current, int depth, int state)
{
    // SOURCE:

    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"
              << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}

void Map::writeToFile(const std::string &filename) {
    if (!root) {
        std::cerr << "[Output] Map is empty. Nothing to write." << std::endl;
        return;
    }

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "[Output] Could not open file: " << filename << std::endl;
        return;
    }

    std::queue<MapNode *> q;
    q.push(root);

    while (!q.empty()) {
        MapNode *current = q.front();
        q.pop();

        outFile << current->isle->getName()  << std::endl;

        if (current->left) {
            q.push(current->left);
        }
        if (current->right) {
            q.push(current->right);
        }
    }

    outFile.close();
}


void Map::writeIslesToFile(const std::string &filename) {
    if (!root) {
        return;
    }

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        return;
    }

    // In-order traversal to collect Isles
    std::function<void(MapNode *)> inOrderTraversal = [&](MapNode *node) {
        if (!node) return;

        inOrderTraversal(node->left);
        outFile << node->isle->getName() << std::endl;
        inOrderTraversal(node->right);
    };

    inOrderTraversal(root);

    outFile.close();
    //std::cout << "[Output] Isles have been written to " << filename << " in alphabetical order." << std::endl;
    // Use std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;
}