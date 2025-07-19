#include "vasm.hpp"

int main(int argc, char *argv[]) {
    try {
        parse_args(argc, argv);
    }
    catch (const args_handle_error& error) {
        std::cout << "error" << std::endl;
    }
    return 0;
}