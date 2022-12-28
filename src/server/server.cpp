#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <sstream>
#include <string.h>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "Message.hpp"
#include "board.hpp"

void sent_to_client(int sockfd, std::string_view data) noexcept {
    (void)send(sockfd, data.data(), data.size(), 0);
}

board_t start_game(int clientSocket) {
    std::vector<std::string> intro = {
        "TIC TAC TOE\n",
        "This game works by entering two integers as co-ordinates for the\n"
        "place on the board to fill\n\n",
    };

    for (std::string elem : intro) {
        sent_to_client(clientSocket, elem);
    }

    board_t board = construct_board();
    return board;
}

int server_main() {
    // Create socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1) {
        std::cerr << "Can't create socket";
        return -1;
    }

    // Bind to IP/port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening, (sockaddr *)&hint, sizeof(hint)) == -1) {
        std::cerr << "Can't bind to port";
        return -2;
    }

    // Mark for listening
    if (listen(listening, SOMAXCONN) == -1) {
        std::cerr << "Can't listen";
        return -3;
    }

    // Accept a call
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(listening, (sockaddr *)&client, &clientSize);

    if (clientSocket == -1) {
        std::cerr << "Problem with client connection";
        return -4;
    }

    // Close listener
    close(listening);
    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr *)&client, sizeof(client), host,
                             NI_MAXHOST, svc, NI_MAXSERV, 0);
    if (result) {
        std::cout << host << " connected on " << svc << "\n";
    } else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on " << ntohs(client.sin_port) << "\n";
    }

    // While receiving, display message + echo message
    char buf[4096];

    board_t board = start_game(clientSocket);

    // THis is each move
    while (true) {
        // clear buffer
        memset(buf, 0, 4096);
        sent_to_client(clientSocket, print_board(board));

        // wait for message
        int bytesRecv = recv(clientSocket, buf, 4096, 0);
        if (bytesRecv == -1) {
            std::cerr << "connection issue\n";
            break;
        } else if (bytesRecv == 0) {
            std::cout << "Client disconnected\n";
            break;
        }

        std::cout << "Received: " << std::string(buf, 0, bytesRecv)
                  << std::endl;
        board = place_counter(board, std::string(buf, 0, bytesRecv));
        sent_to_client(clientSocket, print_board(board));
    }

    // Close socket
    close(clientSocket);

    return 0;
}
