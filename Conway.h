#ifndef __CONWAY_H__
#define __CONWAY_H__

#include <vector>
#include <utility>
#include <vector>

class ConwayLifeGame {
public:
    ConwayLifeGame(int num_row = 33, int num_col = 100,
                   double life_ratio = 0.1, int iter_times = -1,
                   int sleep_millisec = 100);

    ConwayLifeGame(const ConwayLifeGame&) = delete;
    ConwayLifeGame(ConwayLifeGame&&) = delete;

    /*
    ConwayLifeGame(const std::vector<std::pair<int, int>>& life_loc_list) {
    }
    */

    ConwayLifeGame& operator=(const ConwayLifeGame&) = delete;
    ConwayLifeGame& operator=(ConwayLifeGame&&) = delete;

    void start();
private:
    using int8_t = char;

    enum State { DEAD, ALIVE, WALL };

    std::vector<std::vector<int8_t>> _boards[2];
    int8_t _current_board_num;
    int _num_row;
    int _num_col;
    double _life_ratio;
    int _iter_times;
    int _sleep_millisec;

    void _update();
    int _count_alive_neighbor(int row, int col);
    void _show();
    void _keystroke_detect();
    void _randomly_set();
};

#endif
