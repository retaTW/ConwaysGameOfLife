#include "Conway.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

#include <thread>
#include <chrono>

#include <conio.h> // only for windows

using namespace std;

ConwayLifeGame::ConwayLifeGame(int num_row, int num_col,
                               double life_ratio, int iter_times,
                               int sleep_millisec)
        : _current_board_num(0), _num_row(num_row), _num_col(num_col),
          _life_ratio(life_ratio),
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
        init_board[i][0] = init_board[i][_num_col+1] = WALL;
    }
    for (int i = 0; i <= _num_col + 1; i++) {
        init_board[0][i] = init_board[_num_row+1][i] = WALL;
    }
}

void ConwayLifeGame::start() {
    cout << "\033[?25l";
    cout << "\033[2J\033[0;0H";
    while (_iter_times--) {    
        cout << "\033[s";
        _show();
        _update();
        this_thread::sleep_for(std::chrono::milliseconds(_sleep_millisec));
        cout << "\033[u";
        _keystroke_detect();
    }
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
                // unreachable now
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
            case DEAD:
                cout << " ";
                break;
            case ALIVE:
                cout << "\033[7m \033[0m";
                break;
            
            case WALL:
                cout << " ";
                break;
            }
        }
        cout << endl;
    }
}

void ConwayLifeGame::_keystroke_detect() {
    while (::_kbhit()) {
        char key = ::_getch();
        if (key == 'r') {
            _randomly_set();
        }
        else if (key == '?') {
            
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


