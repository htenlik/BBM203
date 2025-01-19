#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

Map::Map() {
    // TODO: Your code here
    // Initialize all distances to a value representing no direct connection
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            if (i != j){
            distanceMatrix[i][j] =  0;
        }
    }

    }
    // Initialize all provinces as unvisited
    for (bool & i : visited) {
        i = false;
    }
}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    // TODO: Your code here
    // Read each line in the CSV file
    // Read each cell separated by a comma
    // Convert cell to an integer and store in distanceMatrix
    ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }
    string line;
    int row = 0;
    while (getline(file, line)) {
        istringstream ss(line);
        string cell;
        int col = 0;
        while (getline(ss, cell, ',')) {
            distanceMatrix[row][col] = stoi(cell);
            ++col;
        }
        ++row;

    }
}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    // TODO: Your code here
    int distance = getDistance(provinceA, provinceB);
    if(distance <= maxDistance){
        return true;
    }
    return false;
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    // TODO: Your code here
    visited[province] = true;

}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    return visited[province];
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    // TODO: Your code here
    for (bool & i : visited) {
        i = false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    int count = 0;
    for (bool i : visited) {
        if (i) {
            ++count;
        }
    }
    return count;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    // TODO: Your code here
        return distanceMatrix[provinceA][provinceB];
}