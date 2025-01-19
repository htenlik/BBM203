#include "Isle.h"

Isle::Isle(std::string name) : name(name)
{
}

const std::string& Isle::getName() const
{
    return this->name;
}

Item Isle::getItem()
{
    return item;
}

void Isle::setItem(Item item)
{
    this->item = item;
}

int Isle::getShaperCount()
{
    return shaperCount;
}

bool Isle::increaseShaperCount()
{
    bool isFull = true;
    shaperCount++;

    if (shaperCount <= capacity) {
        isFull = false; // Not full
        return isFull;
    }
    // return isFull, True if capacity is exceded, false otherwise

    return isFull;
}

bool Isle::decreaseShaperCount()
{
    bool isEmpty = false;
    if (shaperCount > 0) {
        shaperCount--;
    }
    if (shaperCount == 0) isEmpty=true;

    // return isEmpty, True if shaper count less and equal to 0, false otherwise

    return isEmpty;
}

bool Isle::operator==(const Isle &other) const
{
    return this->name == other.name;
}

bool Isle::operator<(const Isle &other) const
{
    return this->name < other.name;

}

bool Isle::operator>(const Isle &other) const
{
    return this->name > other.name;

}

// Implementation of readFromFile
std::vector<Isle *> Isle::readFromFile(const std::string &filename)
{
    std::vector<Isle *> isles;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return isles;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        isles.push_back(new Isle(line));
    }

    file.close();
// add them to vector
    // return the vector
    // Input format: isleName

    return isles;
}

std::ostream &operator<<(std::ostream &os, const Isle &p)
{
    // Prints to terminal with color
    // This function might cause some problems in terminals that are not Linux based
    // If so, comment out the colors while testing on local machine
    // But open them back up while submitting or using TurBo

    std::string einsteiniumColor = "\033[38;2;6;134;151m";
    std::string goldiumColor = "\033[38;2;255;198;5m";
    std::string amazoniteColor = "\033[38;2;169;254;255m";
    std::string resetColorTag = "\033[0m";

    if (p.item == EINSTEINIUM)
        return (os << einsteiniumColor << p.name << resetColorTag);
    if (p.item == GOLDIUM)
        return (os << goldiumColor << p.name << resetColorTag);
    if (p.item == AMAZONITE)
        return (os << amazoniteColor << p.name << resetColorTag);
    return (os << p.name);
}