#include <argparse/argparse.hpp>
#include <iostream>
#include <string>

#include "client/client.hpp"
#include "server/server.hpp"

int main(int argc, char *argv[]) {
    std::string version = "v0.1.0";
    argparse::ArgumentParser program("avatar-of-gaia", version);

    program.add_argument("-c", "--client")
        .help("Launch game client")
        .implicit_value(true)
        .default_value(true);

    program.add_argument("-s", "--server")
        .help("Launch game server")
        .implicit_value(true)
        .default_value(false);

    program.parse_args(argc, argv);

    if (program["--server"] == true) {
        server_main();
    } else if (program["--client"] == true) {
        client_main();
    } else {
        std::cout << "You must specify an entrypoint\n";
    }
}
