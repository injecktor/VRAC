#include "vasm_logging.hpp"

void print_err(const std::string& str) {
    std::cout << ERROR_MESSAGE << str << std::endl;
    if (!vasm_flags.last_error_extra_msg.empty()) {
        print_info(vasm_flags.last_error_extra_msg, 1);
    }
}

void print_info(const std::string& str, size_t required_info_level) {
    if (required_info_level == 0) {
        required_info_level = 1;
    }
    if (required_info_level > INFO_LEVEL_MAX) {
        required_info_level = INFO_LEVEL_MAX;
    }
    if (vasm_flags.info_level >= required_info_level) {
        std::cout << INFO_MESSAGE << str << std::endl;
    }
}