#include <random>
#include <sstream>
#include <string>
#include <tuple>
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
    std::mt19937 mt{std::random_device{}()};

    while (true) {
        std::uniform_int_distribution dice{1, 6};
        int loc = dice(mt);

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
        if (board[0].value == token_as_str(TOKEN)) {
            return State::Win;
        } else if (board[0].value == token_as_str(OPP_TOKEN)) {
            return State::Lose;
        }
    }

    if (board[3].value == board[4].value && board[4].value == board[5].value) {
        if (board[3].value == token_as_str(TOKEN)) {
            return State::Win;
        } else if (board[3].value == token_as_str(OPP_TOKEN)) {
            return State::Lose;
        }
    }

    if (board[6].value == board[7].value && board[7].value == board[8].value) {
        if (board[6].value == token_as_str(TOKEN)) {
            return State::Win;
        } else if (board[6].value == token_as_str(OPP_TOKEN)) {
            return State::Lose;
        }
    }

    // Vertical
    if (board[0].value == board[3].value && board[3].value == board[6].value) {
        if (board[0].value == token_as_str(TOKEN)) {
            return State::Win;
        } else if (board[0].value == token_as_str(OPP_TOKEN)) {
            return State::Lose;
        }
    }

    if (board[1].value == board[4].value && board[4].value == board[7].value) {
        if (board[1].value == token_as_str(TOKEN)) {
            return State::Win;
        } else if (board[1].value == token_as_str(OPP_TOKEN)) {
            return State::Lose;
        }
    }

    if (board[2].value == board[5].value && board[5].value == board[8].value) {
        if (board[2].value == token_as_str(TOKEN)) {
            return State::Win;
        } else if (board[2].value == token_as_str(OPP_TOKEN)) {
            return State::Lose;
        }
    }

    // Diagonal
    if (board[0].value == board[4].value && board[4].value == board[8].value) {
        if (board[0].value == token_as_str(TOKEN)) {
            return State::Win;
        } else if (board[0].value == token_as_str(OPP_TOKEN)) {
            return State::Lose;
        }
    }

    if (board[2].value == board[4].value && board[4].value == board[6].value) {
        if (board[2].value == token_as_str(TOKEN)) {
            return State::Win;
        } else if (board[2].value == token_as_str(OPP_TOKEN)) {
            return State::Lose;
        }
    }

    return State::No_state;
};

void game_end(bool win, int clientSocket) {
    std::cout << "Game over. Player " << ((win) ? "won" : "lost") << "\n";

    std::string end_tag = "Game over, you have ";
    end_tag.append((win) ? "won" : "lost");
    end_tag.append(". Thanks for playing.\n");

    sent_to_client(clientSocket, end_tag);
}

std::tuple<bool, board_t> play_game(board_t board, int clientSocket,
                                    int bytesRecv, char *buf) {
    auto [b, success] = place_counter(board, std::string(buf, 0, bytesRecv));
    if (success) {
        board = ai_player(b);
    } else {
        std::cout << "Not successful\n";
        sent_to_client(clientSocket, std::string("Invalid location\n"));
    }
    sent_to_client(clientSocket, print_board(board));

    if (has_game_ended(board) == State::Win) {
        game_end(true, clientSocket);
        return std::tuple{true, board};
    } else if (has_game_ended(board) == State::Lose) {
        game_end(false, clientSocket);
        return std::tuple{true, board};
    }

    return std::tuple{false, board};
};
