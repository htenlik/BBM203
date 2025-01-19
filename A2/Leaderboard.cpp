#include "Leaderboard.h"

// Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
// variable will point to the highest all-times score, and all other scores will be reachable from it
// via the "next_leaderboard_entry" member variable pointer.
void Leaderboard::read_from_file(const string &filename) {
    // TODO: Your code here
    ifstream file(filename);
    LeaderboardEntry *previous = nullptr;
    string line;
    while (getline(file, line)) {
        stringstream iss(line);
        unsigned long score;
        time_t last_played;
        string player_name;

        if (!(iss >> score >> last_played)) {
            cerr << "Error parsing leaderboard entry: " << line << endl;
            continue;
        }
        getline(iss, player_name);
        player_name = player_name.substr(1);

        LeaderboardEntry *new_entry = new LeaderboardEntry(score, last_played, player_name);

        if (!head_leaderboard_entry) {
            head_leaderboard_entry = new_entry;
        } else if (previous) {
            previous->next = new_entry;
        } else {
            delete new_entry;
            continue;
        }


    previous = new_entry;


        if (score > high_score) {
            high_score = score;
        }
}


    file.close();
}


// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const string &filename) {
    ofstream outfile(filename);
    if (!outfile.is_open()) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }

    LeaderboardEntry *current = head_leaderboard_entry;
    while (current) {
        outfile << current->score << " " << current->last_played << " " << current->player_name << endl;
        current = current->next;
    }

    outfile.close();
}

// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard() {
    cout << "Leaderboard\n-----------" << endl;
    LeaderboardEntry *current = head_leaderboard_entry;
    int rank = 1;

    while (current && rank <= MAX_LEADERBOARD_SIZE) {
        struct tm *time_info = localtime(&current->last_played);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%H:%M:%S/%d.%m.%Y", time_info);

        cout << rank << ". " << current->player_name << " " << current->score << " " << buffer << endl;
        current = current->next;
        rank++;
    }
}

//  Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
//  is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
//  top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    if (!head_leaderboard_entry || new_entry->score > head_leaderboard_entry->score) {
        new_entry->next = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        LeaderboardEntry *current = head_leaderboard_entry;
        while (current->next && current->next->score >= new_entry->score) {
            current = current->next;
        }

        new_entry->next = current->next;
        current->next = new_entry;
    }

    if (new_entry->score > high_score) {
        high_score = new_entry->score;
    }

    LeaderboardEntry *current = head_leaderboard_entry;
    int count = 1;
    while (current && current->next) {

        if (count == MAX_LEADERBOARD_SIZE) {
            LeaderboardEntry *to_delete = current->next;
            while (to_delete) {
                LeaderboardEntry *next = to_delete->next;
                delete to_delete;
                to_delete = next;
            }
            current->next = nullptr;
            break;
        }

        current = current->next;
        count++;
    }}

// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard() {
    LeaderboardEntry *current = head_leaderboard_entry;
    while (current) {
        LeaderboardEntry *next = current->next;

        delete current;
        current = next;
    }
    head_leaderboard_entry = nullptr;

}
