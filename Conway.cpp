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

    #if defined(_WIN32) || defined(_WIN64)
    enum KeyboardArrow { UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77};
    #elif defined(__linux__)
    enum KeyboardArrow { UP = 'A', DOWN = 'B', LEFT = 'D', RIGHT = 'C'};
    #endif
}

ConwayLifeGame::ConwayLifeGame(int num_row, int num_col,
                               double life_ratio, bool verbose,
                               int iter_times, int sleep_millisec)
        : _current_board_num(0), _num_row(num_row), _num_col(num_col), _canvas_row(1), _canvas_col(1),
          _life_ratio(life_ratio), _verbose(verbose), _drug(false), _circular(false), _canvas(false),
          _init_iter_times(iter_times), _current_iter_times(iter_times), _sleep_millisec(sleep_millisec) {
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

    // Edge
    for (int i = 0; i <= _num_row + 1; i++) {
        _boards[0][i][0] = _boards[0][i][_num_col+1] = _boards[1][i][0] = _boards[1][i][_num_col+1] = EDGE;
    }
    for (int i = 0; i <= _num_col + 1; i++) {
        _boards[0][0][i] = _boards[0][_num_row+1][i] = _boards[1][0][i] = _boards[1][_num_row+1][i] = EDGE;
    }
}

void ConwayLifeGame::start() {
    cout << "\033[?25l"; // hide cursor
    cout << "\033[2J\033[0;0H"; // clean screen
    if (_verbose) {
        _show_title();
    }
    cout << "\033[s"; // store cursor position
    
    while (_current_iter_times--) {
        
        _show_board();
        if (_verbose) {
            _show_attribute();
           //this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
        this_thread::sleep_for(std::chrono::milliseconds(_sleep_millisec));
        _keystroke_detect();
        _update();
        cout << "\033[u"; // restore cursor position
    }

    // TODO: this shouldn't be placed here, but not working in _keystroke_dectect, fix later
    ::keyboard_recovery();
    cout << "\033[2J\033[0;0H"; // clean screen
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
            int nb_row = row + i;
            int nb_col = col + j;
            if (_circular) {
                if (nb_row < 1)
                    nb_row = _num_row;
                else if (nb_row > _num_row)
                    nb_row = 1;

                if (nb_col < 1)
                    nb_col = _num_col;
                else if (nb_col > _num_col)
                    nb_col = 1;
            }
            // itself, not neighbor
            if (i == 0 && j == 0)
                continue;
            if (current_board[nb_row][nb_col] == ALIVE)
                count++;
        }
    }
    return count;
}

void ConwayLifeGame::_show_board() {
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
                case EDGE: {
                    if (_drug) {
                        static const char* colors[] = {"41", "1;43", "43", "42", "46", "44", "45"};
                        static int color_index = 0;
                        cout << "\033[" << colors[color_index++] << "m \033[0m";
                        color_index %= 7;
                        //cout << "\033[1;" << 40 + ::rand()%10 << "m \033[0m";
                    }
                    else if (_circular) {
                        cout << "\033[1;42mO\033[0m";
                    }
                    else {
                        cout << "\033[1;41mX\033[0m";
                    }
                    break;
                }
            }
        }
        cout << endl;
    }
}

void ConwayLifeGame::_show_attribute() {
    cout << "\033[1;4mLength\033[0m:   " << _num_row << " | "
         << "\033[1;4mWidth\033[0m:    " << _num_col << " | "
         << "\033[1;4mCircular\033[0m: " << ((_circular)? "on ":"off") << endl;

    cout << "\033[1;4mSpeed\033[0m:  ";
    if (_sleep_millisec <= 100) {
        cout << "\033[1;37;41mFAST\033[0m  ";
    }
    else if (_sleep_millisec <= 300) {
        cout << "MIDDLE";
    }
    else {
        cout << "\033[37;42mSLOW\033[0m  ";
    }
    cout << " | " << "\033[1;4mCanvas\033[0m: " << ((_canvas)? "\033[37;42mon\033[0m ":"\033[1;37;41moff\033[0m") << endl;

    cout << "\033[0m";
}

void ConwayLifeGame::_show_title() {
    if (_drug) {
        cout << "\033[33;1;5;41m";
    }
    cout << ::titles[GENERAL]
         << "\033[0m" << endl;
}

void ConwayLifeGame::_resize_board(){
    int key = ::get_char();
    if (key == ::KeyboardArrow::UP) {
        if (_num_row <= 5) {
            return;
        }
        _boards[0][_num_row].swap(_boards[0][_num_row+1]);
        _boards[0].pop_back();
        _boards[1][_num_row].swap(_boards[1][_num_row+1]);
        _boards[1].pop_back();
        _num_row--;
    }
    else if (key == ::KeyboardArrow::DOWN) {
        _num_row++;
        _boards[0].push_back(vector<int8_t>(_num_col+2, DEAD));
        _boards[0][_num_row].swap(_boards[0][_num_row+1]);
        _boards[0][_num_row][0] = _boards[0][_num_row][_num_col+1] = EDGE;

        _boards[1].push_back(vector<int8_t>(_num_col+2, DEAD));
        _boards[1][_num_row].swap(_boards[1][_num_row+1]);
        _boards[1][_num_row][0] = _boards[1][_num_row][_num_col+1] = EDGE;
    }
    else if (key == ::KeyboardArrow::LEFT) {
        if (_num_col <= 5) {
            return;
        }
        for (auto& row : _boards[0]) {
            row.pop_back();
            row.back() = EDGE;
        }
        for (auto& row : _boards[1]) {
            row.pop_back();
            row.back() = EDGE;
        }
        _num_col--;
    }
    else if (key == ::KeyboardArrow::RIGHT) {
        _num_col++;
        _boards[0][0].push_back(EDGE);
        _boards[1][0].push_back(EDGE);
        for (int i = 1; i <= _num_row; i++) {
            _boards[0][i].back() = DEAD;
            _boards[0][i].push_back(EDGE);
            _boards[1][i].back() = DEAD;
            _boards[1][i].push_back(EDGE);
        }
        _boards[0][_num_row+1].push_back(EDGE);
        _boards[1][_num_row+1].push_back(EDGE);
    }
    cout << "\033[2J\033[0;0H";
    if (_verbose) {
        _show_title();
    }
}

void ConwayLifeGame::_keystroke_detect() {
    while (::keyboard_hit()) {
        int key = ::get_char();
        // restart
        if (key == 'r') {
            _current_iter_times = _init_iter_times;
            _randomly_set_board();
        }
        // finish
        else if (key == 'f') {
            _current_iter_times = 0;
            // TODO: this doesn't work here, fix later
            // ::keyboard_recovery();
        }
        // DRUG MODE ON/OFF
        else if (key == 'd') {
            _drug = !_drug;
            if (_verbose) {
                cout << "\033[0;0H";
                _show_title();
            }
        }
        // verbose mode on/off
        else if (key == 'v') {
            _verbose = ! _verbose;
            cout << "\033[2J\033[0;0H";
            if (_verbose) {
                _show_title();
            }
            cout << "\033[s";
        }
        else if (key == 'c') {
            _circular = ! _circular;
        }
        // speed up
        else if (key == '+') {
            _sleep_millisec -= (_sleep_millisec > 50)? 50 : _sleep_millisec;
        }
        // speed down
        else if (key == '-') {
            _sleep_millisec += (_sleep_millisec+50 > 500)? (500 - _sleep_millisec) : 50;
        }
        // pause
        else if (key == ' ') {
            ::get_char();
        }
        else if (key == 's') {
            _canvas = true;
            _set_life();
        }
        // resize board
        // on Windows, up arrow is encoded to [0, 72] or [224, 72] two bytes
        // on Linux, up arrow is encoded to [27(033), '[', 'A'] three bytes
        #if defined(_WIN32) || defined(_WIN64)
        else if (key == 0 || key == 224) {
        #elif defined(__linux__)
        else if (key == '\033') {
            ::get_char(); // character '['
        #endif
            _resize_board();
        }
    }
    return;
}

void ConwayLifeGame::_randomly_set_board() {
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

void ConwayLifeGame::_set_life() {
    cout <<"\033[u";
    _show_board();
    if (_verbose) {
        _show_attribute();
    }

    if (_canvas_row > _num_row) {
        _canvas_row = _num_row;
    }
    if (_canvas_col > _num_col) {
        _canvas_col = _num_col;
    }
    cout <<"\033[u\033[" << _canvas_row << "B" << "\033[" << _canvas_col << "C\033[?25h";
    cout.flush();

    int key;
    while (true) {
        key = ::get_char();
        if (key == 's') {
            cout << "\033[?25l";
            _canvas = false;
            break;
        }
        else if (key == ' ') {
            _boards[_current_board_num][_canvas_row][_canvas_col] = (_boards[_current_board_num][_canvas_row][_canvas_col] == ALIVE)? DEAD : ALIVE;
            cout << "\033[u";
            _show_board();
            cout <<"\033[u\033[" << _canvas_row << "B" << "\033[" << _canvas_col << "C";
            cout.flush();
        }
        else if (key == 'k') {
            _kill_all();
            cout << "\033[u";
            _show_board();
            cout <<"\033[u\033[" << _canvas_row << "B" << "\033[" << _canvas_col << "C";
            cout.flush();
        }
        // on Windows, up arrow is encoded to [0, 72] or [224, 72] two bytes
        // on Linux, up arrow is encoded to [27(033), '[', 'A'] three bytes
        #if defined(_WIN32) || defined(_WIN64)
        else if (key == 0 || key == 224) {
        #elif defined(__linux__)
        else if (key == '\033') {
            ::get_char(); // character '['
        #endif
            key = ::get_char();
            if (key == ::KeyboardArrow::UP) {
                if (_canvas_row == 1) {
                    continue;
                }
                _canvas_row--;
                cout << "\033[1A";
                cout.flush();
            }
            else if (key == ::KeyboardArrow::DOWN) {
                if (_canvas_row == _num_row) {
                    continue;
                }
                _canvas_row++;
                cout << "\033[1B";
                cout.flush();
            }
            else if (key == ::KeyboardArrow::LEFT) {
                if (_canvas_col == 1) {
                    continue;
                }
                _canvas_col--;
                cout << "\033[1D";
                cout.flush();
            }
            else if (key == ::KeyboardArrow::RIGHT) {
                if (_canvas_col == _num_col) {
                    continue;
                }
                _canvas_col++;
                cout << "\033[1C";
                cout.flush();
            }
        }
    }
}

void ConwayLifeGame::_kill_all() {
    for (int i = 1; i <= _num_row; i++) {
        for (int j = 1; j <= _num_col; j++) {
            _boards[_current_board_num][i][j] = DEAD;
        }
    }
}
