#ifndef MESSAGE_H
#define MESSAGE_H

#include <sstream>
#include <string>
#include <vector>

struct Message {
    std::vector<std::string> content;

    Message() {}
    Message(std::string raw);
    std::string serialise();
};

inline Message::Message(std::string raw) {
    std::vector<std::string> ret;

    std::istringstream iss(raw);
    for (std::string s; iss >> s;)
        ret.push_back(s);

    content = {ret.begin() + 1, ret.end()};
}

#endif
