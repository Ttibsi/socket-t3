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

#include "game.hpp"

void sent_to_client(int sockfd, std::string_view data) noexcept {
    (void)send(sockfd, data.data(), data.size(), 0);
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

    // While receiving, display message
    char buf[4096];

    board_t board = start_game(clientSocket);
    sent_to_client(clientSocket, print_board(board));
    // THis is each move
    while (true) {
        // clear buffer
        memset(buf, 0, 4096);

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
        auto [end_game, returned_board] =
            play_game(board, clientSocket, bytesRecv, buf);
        if (end_game == true) {
            break;
        }
        board = returned_board;
    }

    // Close socket
    close(clientSocket);

    // TODO: Find how to kill client process when server closes - signals?

    return 0;
}
