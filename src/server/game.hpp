#ifndef GAME_H
#define GAME_H

#include "board.hpp"
#include "server.hpp"
#include "token.hpp"

board_t start_game(int clientSocket);
std::tuple<bool, board_t> play_game(board_t board, int clientSocket,
                                    int bytesRecv, char *buf);

#endif
