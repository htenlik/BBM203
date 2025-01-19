#include "CelestialObject.h"
#include <iostream>

#include "CelestialObject.h"

vector<vector<bool>> rotateClockwise(const vector<vector<bool>> &shape) {
    size_t rows = shape.size();
    size_t cols = shape[0].size();
    vector<vector<bool>> rotated(cols, vector<bool>(rows));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            rotated[j][rows - 1 - i] = shape[i][j];
        }
    }
    return rotated;
}

// Constructor with rotation calculation
CelestialObject::CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row, int time_of_appearance)
        : shape(shape), right_rotation(nullptr), left_rotation(nullptr), next_celestial_object(nullptr),
          object_type(type), starting_row(start_row), time_of_appearance(time_of_appearance) {

    update_rotations();
}

// Copy constructor for CelestialObject
CelestialObject::CelestialObject(const CelestialObject *other)
        : shape(other->shape),
          object_type(other->object_type),
          starting_row(other->starting_row),
          time_of_appearance(other->time_of_appearance)

{
    right_rotation = nullptr;
    left_rotation = nullptr;
    next_celestial_object=nullptr;
}

void CelestialObject::update_rotations() {

    if (this->right_rotation != nullptr && this->right_rotation != this) {
        delete_rotations(this);
    }

    CelestialObject* current = this;

    vector<vector<bool>> currentShape = shape;

    do {
        currentShape = rotateClockwise(currentShape);

        if (currentShape == shape) break;

        auto *newRotation = new CelestialObject(*current);
        newRotation->shape = currentShape;

        current->right_rotation = newRotation;
        newRotation->left_rotation = current;

        current = newRotation;
    } while (true);

    current->right_rotation = this;
    this->left_rotation = current;
}





// Function to delete rotations of a given celestial object. It should free the dynamically allocated
// memory for each rotation.
void CelestialObject::delete_rotations(CelestialObject *target) {
    if (!target) return;

    CelestialObject *current = target->right_rotation;
    while (current != target) {
        CelestialObject *toDelete = current;
        current = current->right_rotation;
        delete toDelete;
    }

    target->right_rotation = target;
    target->left_rotation = target;
}

