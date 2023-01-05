#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <sstream>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int client_main() {
    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        return -1;
    }

    // Create a hint structure
    int port = 54000;
    std::string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    // Connect to the server
    int connResult = connect(sock, (sockaddr *)&hint, sizeof(sockaddr_in));
    if (connResult == -1) {
        return 1;
    }

    char buf[4096];
    std::string userInput;

    // Game intro
    int bytesReceived = recv(sock, buf, 4096, 0);
    std::cout << std::string(buf, bytesReceived) << "\n";

    do {
        // Enter text
        std::cout << "> ";
        getline(std::cin, userInput);

        // Send to server
        int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
        if (sendRes == -1) {
            std::cout << "Cound not send to server\n";
            continue;
        }

        // Wait for response
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);

        if (bytesReceived == 0) {
            break;
        }

        // Display Result
        std::cout << std::string(buf, bytesReceived) << "\n";

    } while (true);

    // Close socket
    close(sock);
    return 0;
}
