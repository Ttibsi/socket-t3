#ifndef BOARD_H
#define BOARD_H

#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "token.hpp"

struct Elem {
    int x_cord;
    int y_cord;
    std::string value;
};

using board_t = std::vector<Elem>;

inline board_t construct_board() {
    std::vector<Elem> b = {};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Elem elem = {i, j, ""};
            b.push_back(elem);
        }
    }

    return b;
}

inline std::string print_board(board_t b) {
    std::string ret = std::string("==========\n") + "| " + b[0].value + " | " +
                      b[1].value + " | " + b[2].value + " |\n" +
                      "==========\n" + "| " + b[3].value + " | " + b[4].value +
                      " | " + b[5].value + " |\n" + "==========\n" + "| " +
                      b[6].value + " | " + b[7].value + " | " + b[8].value +
                      " |\n" + "==========\n";

    return ret;
}

inline board_t place_counter(board_t b, std::string input) {
    std::regex exp("[0-2]");
    std::smatch sm;
    std::regex_match(input, sm, exp);
    int counter_location = 0;

    if (sm.size() > 0) {
        counter_location += stoi(sm[0]);
    }

    b[counter_location].value = token_as_str(TOKEN);

    return b;
}

#endif
