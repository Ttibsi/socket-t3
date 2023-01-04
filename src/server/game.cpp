#include <random>
#include <sstream>
#include <string>
#include <typeinfo>

#include "game.hpp"
#include "token.hpp"

enum class State { Win, Lose, No_state };

board_t start_game(int clientSocket) {
    std::vector<std::string> intro = {
        "TIC TAC TOE\n",
        "This game works by entering two integers as co-ordinates for the\n"
        "place on the board to fill.\n",
        "Enter first coordinates to start, ex `(0,1)`\n",
    };

    for (std::string elem : intro) {
        sent_to_client(clientSocket, elem);
    }

    board_t board = construct_board();
    return board;
}

board_t ai_player(board_t b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 9);
    int loc = distr(gen);

    while (true) {
        if (b[loc].value != " ") {
            continue;
        }
        std::cout << "Generated value: " << loc << "\n";
        b[loc].value = token_as_str(Token::O);
        return b;
    }
}

State has_game_ended(board_t board) {
    // horizontal
    if (board[0].value == board[1].value && board[1].value == board[2].value) {
        std::cout << "game end 1\n";
        if (board[0].value == token_as_str(TOKEN)) {
            return State::Win;
        } else if (typeid(board[0].value).name() == std::string("Token")) {
            return State::Lose;
        }
    }

    if (board[3].value == board[4].value && board[4].value == board[5].value) {
        std::cout << "game end 2\n";
        if (board[3].value == token_as_str(TOKEN)) {
            return State::Win;
        } else if (typeid(board[3].value).name() == std::string("Token")) {
            return State::Lose;
        }
    }

    if (board[6].value == board[7].value && board[7].value == board[8].value) {
        std::cout << "game end 3\n";
        if (board[6].value == token_as_str(TOKEN)) {
            return State::Win;
        } else if (typeid(board[6].value).name() == std::string("Token")) {
            return State::Lose;
        }
    }

    // Vertical
    if (board[0].value == board[3].value && board[3].value == board[6].value) {
        std::cout << "game end 4\n";
        if (board[0].value == token_as_str(TOKEN)) {
            return State::Win;
        } else if (typeid(board[0].value).name() == std::string("Token")) {
            return State::Lose;
        }
    }

    if (board[1].value == board[4].value && board[4].value == board[7].value) {
        std::cout << "game end 5\n";
        if (board[1].value == token_as_str(TOKEN)) {
            return State::Win;
        } else if (typeid(board[1].value).name() == std::string("Token")) {
            return State::Lose;
        }
    }

    if (board[2].value == board[5].value && board[5].value == board[8].value) {
        std::cout << "game end 6\n";
        if (board[2].value == token_as_str(TOKEN)) {
            return State::Win;
        } else if (typeid(board[2].value).name() == std::string("Token")) {
            return State::Lose;
        }
    }

    return State::No_state;
};

bool play_game(board_t board, int clientSocket, int bytesRecv, char *buf) {
    auto [b, success] = place_counter(board, std::string(buf, 0, bytesRecv));
    if (success) {
        board = ai_player(b);
    } else {
        std::cout << "Not successful\n";
        sent_to_client(clientSocket, std::string("Invalid location"));
    }
    sent_to_client(clientSocket, print_board(board));

    if (has_game_ended(board) == State::Win) {
        std::cout << "Win\n";
        return true;
    } else if (has_game_ended(board) == State::Lose) {
        std::cout << "Lose\n";
        return true;
    }

    return false;
};