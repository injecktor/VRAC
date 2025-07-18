#include "args_handle.hpp"

void parse_args(int argc, char *argv[]) {
    if (argc < 2) {
        throw args_handle_error::too_few_args;
    }

    std::string arg;
    for (size_t i = 1; i < argc; i++) {
        arg = argv[i];
        std::cout << arg << std::endl;
    }
}