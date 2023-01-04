#ifndef SERVER_H
#define SERVER_H

#include <string_view>

int server_main();
void sent_to_client(int sockfd, std::string_view data) noexcept;

#endif
