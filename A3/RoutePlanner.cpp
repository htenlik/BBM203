#include "RoutePlanner.h"
#include <iostream>
#include <fstream>

// Array to help you out with name of the cities in order
const std::string cities[81] = { 
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik", 
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne", 
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta", 
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya", 
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya", 
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat", 
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir", 
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce" 
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {

    // TO DO:
    // Load map data from file
    map.loadDistanceData(distance_data);

    // Mark all provinces as unvisited
    map.resetVisited();

    // Load priority provinces
    loadPriorityProvinces(priority_data);

    // Load restricted provinces
    loadWeatherRestrictedProvinces(restricted_data);
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    numPriorityProvinces = 0;

    while (std::getline(file, line)) {
        size_t openParen = line.find('(');
        size_t closeParen = line.find(')');

        if (openParen != std::string::npos && closeParen != std::string::npos) {
            int cityIndex = std::stoi(line.substr(openParen + 1, closeParen - openParen - 1));
            priorityProvinces[numPriorityProvinces++] = cityIndex;
        }else {
            int cityIndex = std::stoi(line);
            priorityProvinces[numPriorityProvinces++] = cityIndex;
        }
    }
}

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    numWeatherRestrictedProvinces = 0;

    while (std::getline(file, line)) {
        size_t openParen = line.find('(');
        size_t closeParen = line.find(')');

        if (openParen != std::string::npos && closeParen != std::string::npos) {
            int cityIndex = std::stoi(line.substr(openParen + 1, closeParen - openParen - 1));
            weatherRestrictedProvinces[numWeatherRestrictedProvinces++] = cityIndex;

        }
        else {
            int cityIndex = std::stoi(line);
            weatherRestrictedProvinces[numWeatherRestrictedProvinces++] = cityIndex;
        }
    }
}


// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    // TODO: Your code here
    for (int i = 0; i < numPriorityProvinces; ++i) {
        if (priorityProvinces[i] == province) return true;
    }
    return false;}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    // TODO: Your code here
    for (int i = 0; i < numWeatherRestrictedProvinces; ++i) {
        if (weatherRestrictedProvinces[i] == province) return true;
    }
    return false;}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
    // TODO: Your code here
    map.markAsVisited(startingCity);
    stack.push(startingCity);
    route.push_back(startingCity);

    exploreFromProvince(startingCity);
    displayResults();

}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    // TODO: Your code here
    enqueueNeighbors(province);

    while (!queue.isEmpty() || !stack.isEmpty()) {
        if (!queue.isEmpty()) {
            int nextProvince = queue.dequeue();
            if (!map.isVisited(nextProvince)) {
                map.markAsVisited(nextProvince);
                stack.push(nextProvince);
                route.push_back(nextProvince);
                totalDistanceCovered += map.getDistance(province, nextProvince);

                exploreFromProvince(nextProvince);
            }
        } else {
            backtrack();
        }
    }
}

void RoutePlanner::enqueueNeighbors(int province) {
    // TO DO: Enqueue priority & non-priority neighbors to the queue according to given constraints
    for (int i = 0; i < MAX_SIZE; ++i) {

        if (!map.isVisited(i) && !isWeatherRestricted(i) && map.isWithinRange(province, i, maxDistance)) {
            if (isPriorityProvince(i)) {
                queue.enqueuePriority(i);
            } else {
                queue.enqueue(i);
            }
        }else if (isWeatherRestricted(i)) {
            std::cout << "Province " << cities[i] << " is weather-restricted. Skipping.\n";
        }
    }
}

void RoutePlanner::backtrack() {
    // If you reach a dead-end province 
    // TODO: Your code here
    if (!stack.isEmpty()) {
        stack.pop();
    }
}

bool RoutePlanner::isExplorationComplete() const {
    // TODO: Your code here
    return stack.isEmpty() && queue.isEmpty();
}
void RoutePlanner::displayResults() const {
    std::cout << "----------------------------\n";
    std::cout << "Journey Completed!\n";
    std::cout << "----------------------------\n";
    std::cout << "Total Number of Provinces Visited: " << route.size() << "\n";
    std::cout << "Total Distance Covered: " << totalDistanceCovered << " km\n";

    std::cout << "Route Taken:\n";
    for (size_t i = 0; i < route.size(); ++i) {
        std::cout << cities[route[i]] << (i < route.size() - 1 ? " -> " : " -> End\n");
    }

    int visitedPriorityProvinces = 0;
    std::cout << "\nPriority Provinces Status:\n";
    for (int i = 0; i < numPriorityProvinces; ++i) {
        bool visited = map.isVisited(priorityProvinces[i]);
        std::cout << "- " << cities[priorityProvinces[i]] << " (" << (visited ? "Visited" : "Not Visited") << ")\n";
        if (visited) {
            ++visitedPriorityProvinces;
        }
    }

    std::cout << "\nTotal Priority Provinces Visited: " << visitedPriorityProvinces << " out of " << numPriorityProvinces << "\n";
    if (visitedPriorityProvinces == numPriorityProvinces) {
        std::cout << "Success: All priority provinces were visited.\n";
    } else {
        std::cout << "Warning: Not all priority provinces were visited.\n";
    }
}





