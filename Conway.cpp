#include "Conway.h"
#include "conio.h" // a cross-platform <conio.h> for both linux and windows

#include <iostream>
#include <cstdlib>
#include <ctime>

#include <thread>
#include <chrono>

using namespace std;

namespace {
    const char* titles[] = {
                        "_________                                     /\\        ________                        ________   _____  .____    .__  _____       \n"
                        "\\_   ___ \\  ____   ______  _  _______  ___.__.)/_____  /  _____/_____    _____   ____   \\_____  \\_/ ____\\ |    |   |__|/ ____\\____  \n"
                        "/    \\  \\/ /  _ \\ /    \\ \\/ \\/ /\\__  \\<   |  |/  ___/ /   \\  ___\\__  \\  /     \\_/ __ \\   /   |   \\   __\\  |    |   |  \\   __\\/ __ \\ \n"
                        "\\     \\___(  <_> )   |  \\     /  / __ \\\\___  |\\___ \\  \\    \\_\\  \\/ __ \\|  Y Y  \\  ___/  /    |    \\  |    |    |___|  ||  | \\  ___/ \n"
                        " \\______  /\\____/|___|  /\\/\\_/  (____  / ____/____  >  \\______  (____  /__|_|  /\\___  > \\_______  /__|    |_______ \\__||__|  \\___  >\n"
                        "        \\/            \\/             \\/\\/         \\/          \\/     \\/      \\/     \\/          \\/                \\/             \\/ "
                        ,
                        "   _____           _     _ _        _____                         ____   __   _      _  __     \n"
                        "  / ____|         (_)   | ( )      / ____|                       / __ \\ / _| | |    (_)/ _|    \n"
                        " | |     _____   ___  __| |/ ___  | |  __  __ _ _ __ ___   ___  | |  | | |_  | |     _| |_ ___ \n"
                        " | |    / _ \\ \\ / / |/ _` | / __| | | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | |  _| | |    | |  _/ _ \\\n"
                        " | |___| (_) \\ V /| | (_| | \\__ \\ | |__| | (_| | | | | | |  __/ | |__| | |   | |____| | ||  __/\n"
                        "  \\_____\\___/ \\_/ |_|\\__,_| |___/  \\_____|\\__,_|_| |_| |_|\\___|  \\____/|_|   |______|_|_| \\___|\n"
                        "                                                                                               "
    };

    enum Title { GENERAL, COVID };
}

ConwayLifeGame::ConwayLifeGame(int num_row, int num_col,
                               double life_ratio, bool verbose,
                               int iter_times, int sleep_millisec)
        : _current_board_num(0), _num_row(num_row), _num_col(num_col),
          _life_ratio(life_ratio), _verbose(verbose), _drug(false),
          _iter_times(iter_times), _sleep_millisec(sleep_millisec) {
    // +2 for the edges of board
    // cell position index is from 1 to the number of rows/columns
    auto& init_board = _boards[0]
                     = _boards[1]
                     = vector<vector<int8_t>>(num_row+2, vector<int8_t>(num_col+2, DEAD));

    ::srand(time(nullptr));
    int num_life = _num_row * _num_col * _life_ratio;

    // randomly generating lifes
    for (int i = 0; i < num_life; i++) {
        auto& cell = init_board[::rand()%_num_row + 1][::rand()%_num_col + 1];
        // already alive cell does not count, try again
        if (cell == ALIVE){
            i--;
            continue;
        }
        cell = ALIVE;
    }

    // Edge wall
    for (int i = 0; i <= _num_row + 1; i++) {
        _boards[0][i][0] = _boards[0][i][_num_col+1] = _boards[1][i][0] = _boards[1][i][_num_col+1] = WALL;
    }
    for (int i = 0; i <= _num_col + 1; i++) {
        _boards[0][0][i] = _boards[0][_num_row+1][i] = _boards[1][0][i] = _boards[1][_num_row+1][i] = WALL;
    }
}

void ConwayLifeGame::start() {
    cout << "\033[?25l";
    cout << "\033[2J\033[0;0H";
    if (_verbose) {
        _show_title();
    }
    cout << "\033[s";
    
    while (_iter_times--) {
        _show();
        if (_verbose) {
            _show_setting();
           //this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
        _update();
        this_thread::sleep_for(std::chrono::milliseconds(_sleep_millisec));
        cout << "\033[u";
        _keystroke_detect();
    }
    // TODO: this shouldn't be placed here, but not working in _keystroke_dectect, fix later
    ::keyboard_recovery();
}

void ConwayLifeGame::set_drug(bool high) {
    _drug = high;
}

void ConwayLifeGame::_update() {
    auto& current_board = _boards[_current_board_num];
    int updated_board_num = (_current_board_num + 1) % 2;
    auto& updated_board = _boards[updated_board_num];

    for (int i = 1; i <= _num_row; i++) {
        for (int j = 1, num_neighbor; j <= _num_col; j++) {
            const auto& current_cell = current_board[i][j];
            auto& updated_cell = updated_board[i][j];
            num_neighbor = _count_alive_neighbor(i, j);
            if (current_cell == ALIVE) {
                if (num_neighbor < 2 || num_neighbor > 3) {
                    updated_cell = DEAD;
                }
                else {
                    updated_cell = ALIVE;
                }
            }
            else if (current_cell == DEAD) {
                if (num_neighbor == 3) {
                    updated_cell = ALIVE;
                }
                else {
                    updated_cell = DEAD;
                }
            }
            else {
                //
            }
        }
    }

    _current_board_num = updated_board_num;
}

inline int ConwayLifeGame::_count_alive_neighbor(int row, int col) {
    int count = 0;
    auto& current_board = _boards[_current_board_num];
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            // itself, not neighbor
            if (i == 0 && j == 0)
                continue;
            if (current_board[row+i][col+j] == ALIVE)
                count++;
        }
    }
    return count;
}

void ConwayLifeGame::_show() {
    auto& current_board = _boards[_current_board_num];
    for (auto& row : current_board) {
        for (auto& cell : row) {
            switch (cell) {
                case DEAD: {
                    cout << " ";
                    break;
                }
                case ALIVE: {
                    cout << "\033[7m \033[0m";
                    break;
                }
                case WALL: {
                    static const char* colors[] = {"41", "1;43", "43", "42", "46", "44", "45"};
                    static int color_index = 0;
                    if (_drug) {
                        cout << "\033[" << colors[color_index++] << "m \033[0m";
                        color_index %= 7;
                        //cout << "\033[1;" << 40 + ::rand()%10 << "m \033[0m";
                    }
                    else {
                        cout << "\033[7m+\033[0m";
                    }
                    break;
                }
            }
        }
        cout << endl;
    }
}

void ConwayLifeGame::_show_setting() {
    cout << "\033[1;4mLength\033[0m: " << _num_row << endl
         << "\033[1;4mWidth\033[0m:  " << _num_col << endl
         << "\033[0m";
}

void ConwayLifeGame::_show_title() {
    if (_drug) {
        cout << "\033[33;1;41m";
    }
    cout << ::titles[GENERAL]
         << "\033[0m" << endl;
}

void ConwayLifeGame::_keystroke_detect() {
    while (::keyboard_hit()) {
        char key = ::get_char();
        if (key == 'r') {
            _randomly_set();
        }
        else if (key == 'f') {
            _iter_times = 0;
            cout << "\033[2J\033[0;0H";
            // TODO: this doesn't work here, fix later
            // ::keyboard_recovery();
        }
        // DRUG ON/OFF
        else if (key == 'd') {
            _drug = !_drug;
            if (_verbose) {
                cout << "\033[2J\033[0;0H";
                _show_title();
            }
        }
    }
    return;
}

void ConwayLifeGame::_randomly_set() {
    int num_life = _num_row * _num_col * _life_ratio;
    // randomly generating lifes
    auto& current_board = _boards[_current_board_num];
    for (int i = 1; i <= _num_row; i++) {
        for (int j = 1; j <= _num_col; j++) {
            current_board[i][j] = DEAD;
        }
    }
    for (int i = 0; i < num_life; i++) {
        auto& cell = current_board[::rand()%_num_row + 1][::rand()%_num_col + 1];
        // already alive cell does not count, try again
        if (cell == ALIVE){
            i--;
            continue;
        }
        cell = ALIVE;
    }
}
