#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <tuple>
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
            Elem elem = {i, j, " "};
            b.push_back(elem);
        }
    }

    return b;
}

inline std::string print_board(board_t b) {
    std::string ret =
        std::string("=============\n") + "| " + b[0].value + " | " +
        b[1].value + " | " + b[2].value + " |\n" + "=============\n" + "| " +
        b[3].value + " | " + b[4].value + " | " + b[5].value + " |\n" +
        "=============\n" + "| " + b[6].value + " | " + b[7].value + " | " +
        b[8].value + " |\n" + "=============\n";

    return ret;
}

inline bool check_value(std::string in) {
    std::regex exp(R"((\(?)([0-2]),(\ ?)([0-2])(\)?))");
    if (regex_match(in, exp))
        return true;
    return false;
}

inline std::tuple<board_t, bool> place_counter(board_t b, std::string input) {
    bool correct = check_value(input);
    if (!(correct)) {
        std::cout << "Erroneous entry\n";
        return std::tuple(b, false);
    }

    std::regex r(R"(([0-2]),(\ )*([0-2]))");
    std::smatch sm;
    int counter_location = 0;

    if (std::regex_search(input, sm, r)) {
        for (long unsigned int i = 1; i < sm.size(); i++) {
            if (i == 2) {
                continue;
            } else if (i == 1) {
                counter_location += stoi(sm[i]) * 3;
            } else {
                counter_location += stoi(sm[i]);
            }
        }
    }

    if (b[counter_location].value == " ") {
        b[counter_location].value = token_as_str(TOKEN);
        return std::tuple(b, true);
    } else {
        std::cout << "Error: Input already taken\n";
        return std::tuple(b, false);
    }
}

#endif
