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
    if (input.is_open() && std::getline(input, str, '\n')) {
        return true;
    }
    return false;
}

bool vasm_file_t::write_line(const std::string& str) {
    try {
        output << str << std::endl;
    }
    catch (...) {
        vasm_flags.last_error_extra_msg = "Failed writing: " + str;
        vasm_flags.last_error_extra_msg = " in: " + path;
        return false;
    }
    return true;
}

bool vasm_file_t::delete_file() {
    if (mode == file_mode_t::write && path != "") {
        close();
        return !std::filesystem::remove(path);
    }
    return false;
}

bool vasm_file_t::is_open() {
    return input.is_open() || output.is_open();
}

bool vasm_file_t::set_line_cursor(size_t line_number) {
    if (!reopen()) {
        return false;
    }
    if (!line_number) {
        return true;
    }
    std::string tmp;
    for (size_t i = 0; i < line_number - 1; i++) {
        read_line(tmp);
    }
    return true;
}

std::string vasm_file_t::current_file() {
    if (input.is_open()) {
        return path;
    }
    return "";
}

bool vasm_file_t::delete_file(std::string path) {
    return !std::filesystem::remove(path);
}

vasm_file_t::~vasm_file_t() {
    close();
}