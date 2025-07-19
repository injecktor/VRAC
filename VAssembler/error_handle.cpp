#include "error_handle.hpp"

#define ERROR_MESSAGE "[ERROR]: "
#define INFO_MESSAGE "[INFO]: "

size_t info_level = 0;

void print_err(const std::string& str) {
    std::cout << ERROR_MESSAGE << str << std::endl;
}

void print_info(const std::string& str, size_t required_info_level) {
    if (required_info_level == 0) {
        required_info_level = 1;
    }
    if (required_info_level > 4) {
        required_info_level = 4;
    }
    if (info_level >= required_info_level) {
        std::cout << INFO_MESSAGE << str << std::endl;
    }
}