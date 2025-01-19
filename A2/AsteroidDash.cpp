#include "AsteroidDash.h"

struct Projectile {
    int row;
    int col;

    Projectile(int start_row, int start_col) : row(start_row), col(start_col) {}

    void move_right() {
        col++;
    }
    void move_left() {
        col--;
    }

};


// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {

    read_player(player_file_name, player_name);
    read_space_grid(space_grid_file_name);
    read_celestial_objects(celestial_objects_file_name);
    leaderboard.read_from_file(leaderboard_file_name);

}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    ifstream file(input_file);
    if (!file.is_open()) {
        cerr << "Error opening file: " << input_file << endl;
        return;
    }

    vector<vector<int>> temp_grid;
    string line;

    while (getline(file, line)) {
        vector<int> row;
        string value_str;

        for (char c : line) {
            if (c != ' ') {
                value_str += c;
                row.push_back(stoi(value_str));
                value_str = "";
            }
        }

        if (!value_str.empty()) {
            row.push_back(stoi(value_str));
        }

        if (!row.empty()) {
            temp_grid.push_back(row);
        }
    }

    if (temp_grid.empty()) {
        cerr << "Error: No valid grid data found in the file." << endl;
        file.close();
        return;
    }
    space_grid = temp_grid;
    file.close();
}


// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    ifstream file(player_file_name);
    int rows, cols;
    file >> rows >> cols;

    vector<vector<bool>> shape;
    string line;
    getline(file,line);
    while (getline(file, line)) {
        vector<bool> row;
        for (char c : line) {
           if (c == '1') {
               row.push_back(true);
           } else if (c == '0') {
               row.push_back(false);
           }
        }
        shape.push_back(row);
    }

    player = new Player(shape, rows, cols, player_name);
    file.close();
}


// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const string &input_file) {
    ifstream file(input_file);
    if (!file.is_open()) {
        cerr << "Error opening file: " << input_file << endl;
        return;
    }

    string line;
    CelestialObject* prev = nullptr;

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        vector<vector<bool>> object_shape;
        int occupiedCell=0;
        bool is_curly_braces = (line[0] == '{');
        bool is_square_braces = (line[0] == '[');
        if (is_curly_braces || is_square_braces) {
            do {
                vector<bool> row;
                for (char c : line) {
                    if (c == '1') {
                        row.push_back(true);
                        occupiedCell++;
                    } else if (c == '0') {
                        row.push_back(false);
                    }
                }
                if (!row.empty()) {
                    object_shape.push_back(row);
                }
            } while (getline(file, line) && !line.empty() && line[0] != 's' && line[0] != 't' && line[0] != 'e');


            int s = 0, t = 0;
            string effect;

            if (line[0] == 's') {
                s = stoi(line.substr(2));
                getline(file,line);
            }

            if (line[0] == 't') {
                t = stoi(line.substr(2));
                getline(file,line);
            }

            if (!line.empty() && line[0] == 'e') {
                effect = line.substr(2);
            }
            CelestialObject* object = nullptr;
            if(is_square_braces) {
                 object = new CelestialObject(object_shape, ASTEROID, s, t);
            }else {
                if(effect == "life"){
                    object = new CelestialObject(object_shape, LIFE_UP, s, t);
                }else if (effect == "ammo"){
                    object = new CelestialObject(object_shape, AMMO, s, t);
                }
            }

            object->occupiedCell = occupiedCell;

            if (!celestial_objects_list_head) {
                celestial_objects_list_head = object;
            } else {
                prev->next_celestial_object = object;
                CelestialObject* temp = prev;
                do {
                    temp->next_celestial_object = prev->next_celestial_object;
                    temp = temp->right_rotation;
                } while (temp != prev);
            }
            prev = object;

        }
    }


    file.close();
}

void AsteroidDash::print_space_grid() const {
    cout << "Tick: " << game_time << endl;
    cout << "Lives: " << player->lives << endl;
    cout << "Ammo: " << player->current_ammo <<endl;
    cout << "Score: " << current_score <<endl;
    cout << "High Score: " <<  leaderboard.high_score<<endl;

    for (const auto &row : space_grid) {
        for (int cell : row) {
            if (cell == 0) {
                cout << unoccupiedCellChar;
            } else {
                cout << occupiedCellChar;
            }
        }
        cout << endl;
    }
    cout << endl;
}


// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {
    if (player->lives == 0) {
        game_time--;
        game_over = true;
        for (auto &row : space_grid) {
            fill(row.begin(), row.end(), 0);
        }
        return;
    }

    current_score += 1;


    for (auto &row : space_grid) {
        fill(row.begin(), row.end(), 0);
    }

    place_player_on_grid();

    CelestialObject* current = celestial_objects_list_head;
    CelestialObject* prev = nullptr;

    while (current) {
        if (game_time >= current->time_of_appearance) {
            int start_col = space_grid[0].size() - (game_time - current->time_of_appearance);
            bool collision = false;
            bool ammo = false;
            bool life = false;

            for (int i = 0; i < current->shape.size(); ++i) {
                for (int j = 0; j < current->shape[i].size(); ++j) {
                    if (current->shape[i][j]) {
                        int row = current->starting_row + i;
                        int col = start_col + j - 1;

                        if (row >= 0 && row < space_grid.size() && col >= 0 && col < space_grid[0].size()) {
                            if (space_grid[row][col] == 1) {

                                collision = (current->object_type == ASTEROID);
                                ammo = (current->object_type == AMMO);
                                life = (current->object_type == LIFE_UP);

                            }
                        }
                    }
                }
            }



            if (collision || ammo || life) {
                if (collision) {
                    player->lives--;
                    if (player->lives == 0) {
                        for (auto &row : space_grid) {
                            fill(row.begin(), row.end(), 0);
                        }
                        game_time--;
                        game_over = true;
                        current_score--;
                    }
                } else if (ammo) {
                    player->current_ammo = player->max_ammo;
                } else if (life) {
                    player->lives++;
                }

                CelestialObject* temp = current;
                current = current->next_celestial_object;

                if (prev) {
                    prev->next_celestial_object = current;
                } else {
                    celestial_objects_list_head = current;
                }


                CelestialObject::delete_rotations(temp);
                delete temp;
                continue;
            }

            place_celestial_object_in_grid(current);

        }

        prev = current;
        current = current->next_celestial_object;
    }

    //shooting
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        int prev_col = it->col;
        int prev_row = it->row;


        it->move_right();

        if (it->col >= space_grid[0].size()) {
            it = projectiles.erase(it);
            continue;
        }

        if (prev_row >= 0 && prev_row < space_grid.size() && prev_col >= 0 && prev_col < space_grid[0].size()) {
            if (!(prev_row < (player->position_row + player->spacecraft_shape.size())
                && prev_row >=player->position_row
                && prev_col >= player->position_col
                && prev_col < player->position_col + player->spacecraft_shape[0].size())) {
                space_grid[prev_row][prev_col] = 0;
                }
        }


        for (int col = prev_col ; col <= it->col ; col++) {
            CelestialObject* current = celestial_objects_list_head;
            while (current) {
                if (current->object_type != ASTEROID) {
                    current = current->next_celestial_object;
                    continue;
                }

                int start_col = space_grid[0].size() - (game_time - current->time_of_appearance);
                int relative_col = col - start_col+1;
                int relative_row = it->row - current->starting_row;


                if (relative_row >= 0 && relative_row < current->shape.size() &&
                    relative_col >= 0 && relative_col < current->shape[0].size()) {
                    if (current->shape[relative_row][relative_col]){
                        erase_celestial_object_from_grid(current);
                        current->shape[relative_row][relative_col] = false;
                        current->update_rotations();
                        current_score += 10;

                        if (current->shape.size()%2) {
                            if (relative_row < current->shape.size() / 2) {
                                current->shape = current->right_rotation->shape;
                            } else if (relative_row > current->shape.size() / 2) {
                                current->shape = current->left_rotation->shape;
                            }
                        }else {
                            if (relative_row < current->shape.size() / 2) {
                                current->shape = current->right_rotation->shape;
                            } else if (relative_row >= current->shape.size() / 2) {
                                current->shape = current->left_rotation->shape;
                            }
                        }

                        current->update_rotations();

                        place_celestial_object_in_grid(current);


                        bool fully_destroyed = true;
                        for (const auto& row : current->shape) {
                            for (bool cell : row) {
                                if (cell) {
                                    fully_destroyed = false;
                                    break;
                                }
                            }
                            if (!fully_destroyed) break;
                        }

                        if (fully_destroyed) {
                            current_score += 100*(current->occupiedCell);
                            if (celestial_objects_list_head == current) {
                                celestial_objects_list_head = current->next_celestial_object;
                            } else {
                                CelestialObject* prev = celestial_objects_list_head;
                                while (prev && prev->next_celestial_object != current) {
                                    prev = prev->next_celestial_object;
                                }
                                if (prev) {
                                    prev->next_celestial_object = current->next_celestial_object;
                                }
                            }
                            CelestialObject::delete_rotations(current);
                            delete current;
                        }
                        it = projectiles.erase(it);
                        break;
                    }
                    }

                current = current->next_celestial_object;
            }
            if (it == projectiles.end()) break;

        }

        if (it != projectiles.end()) {
            space_grid[it->row][it->col] = 1;
            ++it;
        }
    }

}



void AsteroidDash::place_player_on_grid() {
    for (int i = 0; i < player->spacecraft_shape.size(); i++) {
        for (int j = 0; j < player->spacecraft_shape[i].size(); ++j) {
            if (player->spacecraft_shape[i][j]) {
                int row = player->position_row + i;
                int col = player->position_col + j;


                if (row >= 0 && row < space_grid.size() && col >= 0 && col < space_grid[row].size()) {
                    space_grid[row][col] = 1;
                }
            }
        }
    }
}


void AsteroidDash::erase_player_from_grid() {
    for (int i = 0; i < player->spacecraft_shape.size(); i++) {
        for (int j = 0; j < player->spacecraft_shape[i].size(); ++j) {
            if (player->spacecraft_shape[i][j]) {
                int row = player->position_row + i;
                int col = player->position_col + j;


                if (row >= 0 && row < space_grid.size() && col >= 0 && col < space_grid[row].size()) {
                    space_grid[row][col] = 0;
                }
            }
        }
    }
}


void AsteroidDash::erase_celestial_object_from_grid(const CelestialObject* celestial_object) {
    int start_col = space_grid[0].size() - (game_time - celestial_object->time_of_appearance);

    for (int i = 0; i < celestial_object->shape.size(); ++i) {
        for (int j = 0; j < celestial_object->shape[i].size(); ++j) {
            if (celestial_object->shape[i][j]) {
                int row = celestial_object->starting_row + i;
                int col = start_col + j - 1;


                if (row >= 0 && row < space_grid.size() && col >= 0 && col < space_grid[row].size()) {
                    space_grid[row][col] = 0;
                }
            }
        }
    }
}
void AsteroidDash::place_celestial_object_in_grid(const CelestialObject* celestial_object) {
    int start_col = space_grid[0].size() - (game_time - celestial_object->time_of_appearance);

    for (int i = 0; i < celestial_object->shape.size(); ++i) {
        for (int j = 0; j < celestial_object->shape[i].size(); ++j) {
            if (celestial_object->shape[i][j]) {
                int row = celestial_object->starting_row + i;
                int col = start_col + j - 1;


                if (row >= 0 && row < space_grid.size() && col >= 0 && col < space_grid[row].size()) {
                    space_grid[row][col] = (celestial_object->object_type == ASTEROID) ? 1 : 2;
                }
            }
        }
    }
}



// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot() {
    if (player->current_ammo <= 0) {
        return;
    }

    int center_row = player->position_row + (player->spacecraft_shape.size() / 2);
    int launch_col = player->position_col + player->spacecraft_shape[0].size()-1;

    projectiles.emplace_back(center_row, launch_col);

    player->current_ammo--;
    }


void AsteroidDash::read_leaderboard() {
    leaderboard.read_from_file(leaderboard_file_name);
}

void AsteroidDash::write_leaderboard() {
    leaderboard.write_to_file(leaderboard_file_name);
}

void AsteroidDash::print_leaderboard() {
    leaderboard.print_leaderboard();
}

void AsteroidDash::update_leaderboard(unsigned long score, const string &player_name) {
    time_t now = time(nullptr);
    LeaderboardEntry *new_entry = new LeaderboardEntry(score, now, player_name);
    leaderboard.insert(new_entry);
}

void AsteroidDash::print_game_statistics(const string& game_over_type) const {
    cout << game_over_type << endl;
    cout << "Tick: " << game_time-1<<  endl;
    cout << "Lives: " << player->lives <<endl;
    cout << "Ammo: " << player->current_ammo <<endl;
    cout << "Score: " << current_score <<endl;
    cout << "High Score: " << leaderboard.high_score <<endl;
    cout << "Player: " << player->player_name <<endl;

    for (const auto &row : space_grid) {
        for (int cell : row) {
            if (cell == 0) {
                cout << unoccupiedCellChar;
            } else {
                cout << occupiedCellChar;
            }
        }
        cout << endl;
    }

    cout << endl;
}


// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {
    delete player;
    CelestialObject* current = celestial_objects_list_head;
    while (current) {
        CelestialObject* next = current->next_celestial_object;
        CelestialObject::delete_rotations(current);
        delete current;
        current = next;
    }
}

