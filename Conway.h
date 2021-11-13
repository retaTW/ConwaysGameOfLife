#ifndef __CONWAY_H__
#define __CONWAY_H__

#include <vector>
#include <utility>
#include <vector>

class ConwayLifeGame {
public:
    ConwayLifeGame(int num_row = 30, int num_col = 130,
                   double life_ratio = 0.2, bool verbose = true,
                   int iter_times = -1, int sleep_millisec = 200);

    ConwayLifeGame(const ConwayLifeGame&) = delete;
    ConwayLifeGame(ConwayLifeGame&&) = delete;

    /*
    ConwayLifeGame(const std::vector<std::pair<int, int>>& life_loc_list) {
    }
    */

    ConwayLifeGame& operator=(const ConwayLifeGame&) = delete;
    ConwayLifeGame& operator=(ConwayLifeGame&&) = delete;

    void start();
    void set_drug(bool are_you_high);
private:
    using int8_t = char;

    enum State { DEAD, ALIVE, EDGE };

    // board and life
    std::vector<std::vector<int8_t>> _boards[2];
    int8_t _current_board_num;
    int _num_row;
    int _num_col;
    int _canvas_row;
    int _canvas_col;
    double _life_ratio;

    // flags
    bool _verbose;
    bool _drug;
    bool _circular;
    bool _canvas;

    // animation
    const int _init_iter_times;
    int _current_iter_times;
    int _sleep_millisec;

    void _update();
    int _count_alive_neighbor(int row, int col);
    void _show_board();
    void _show_attribute();
    void _show_title();
    void _resize_board();
    void _keystroke_detect();
    void _randomly_set_board();
    void _set_life();
    void _kill_all();
};

#endif
