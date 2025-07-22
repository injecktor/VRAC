#include "file_handle.hpp"

vasm_file_t::vasm_file_t(file_mode_t mode) : mode(mode) {};

bool vasm_file_t::open(std::string path) {
    close(); 
    this->path = path;

    print_info("Opening: " + path, 3);
    try {
        switch (mode) {
            case file_mode_t::read:
                input.open(path);
                break;
            case file_mode_t::write:
                output.open(path, std::ios_base::app);
                break;
            default:
                print_err("Unknown file mode");
                return false;
        }
    }
    catch (...) {
        vasm_flags.last_error_extra_msg = "Failed opening: " + path;
        return false;
    }
    print_info("Opened file: " + path, 2);
    return true;
}

void vasm_file_t::close() {
    if (input.is_open()) {
        input.close();
    }
    if (output.is_open()) {
        output.close();
    }
}

bool vasm_file_t::reopen() {
    return open(path);
}

bool vasm_file_t::read_line(std::string& str) {
    if (input.is_open() && std::getline(input, str)) {
        return true;
    }
    return false;
}

void vasm_file_t::write_line(const std::string& str) {
    output << str << std::endl;
}

vasm_file_t::~vasm_file_t() {
    close();
}