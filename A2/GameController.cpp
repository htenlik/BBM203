#include "GameController.h"

// Simply instantiates the game
GameController::GameController(
        const string &space_grid_file_name,
        const string &celestial_objects_file_name,
        const string &leaderboard_file_name,
        const string &player_file_name,
        const string &player_name

) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name,
                            player_name);
    // TODO: Your code here, if you want to perform extra initializations

}

// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const string &commands_file) {
    std::ifstream infile(commands_file);
    if (!infile.is_open()) {
        std::cerr << "Error opening commands file: " << commands_file << std::endl;
        return;
    }
    game->read_leaderboard();

    std::string current_command;
    while (std::getline(infile, current_command)) {
        if (game->game_over) {
            game->game_time++;
            game->current_score--;
            game->print_game_statistics("GAME OVER!");
            game->print_leaderboard();

            return;
        }
        game->update_space_grid();

        // Execute the command
        if (current_command == "PRINT_GRID") {
            game->print_space_grid();
        } else if (current_command == "MOVE_UP") {
            game->player->move_up();
        } else if (current_command == "MOVE_DOWN") {
            game->player->move_down(game->space_grid.size());
        } else if (current_command == "MOVE_LEFT") {
            game->player->move_left();
        } else if (current_command == "MOVE_RIGHT") {
            game->player->move_right(game->space_grid[0].size());
        } else if (current_command == "SHOOT") {
            game->shoot();
        } else if (current_command == "NOP") {
            // No operation;
        } else {
            std::cerr << "Unknown command: " << current_command << std::endl;
        }

        game->game_time++;

    }
    game->current_score++;
    game->print_game_statistics("GAME FINISHED! No more commands!");
    game->print_leaderboard();


    game->update_leaderboard(game->current_score, game->player->player_name);

    game->write_leaderboard();


    infile.close();
}

// Destructor to delete dynamically allocated member variables here
GameController::~GameController() {
    delete game;
}
