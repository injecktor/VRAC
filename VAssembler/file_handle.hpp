#pragma once

#include <string>

#include "vasm_logging.hpp"

#if defined(WIN32) || defined(_WIN32)
#include <fstream>
#include <filesystem>
#elif defined(linux)

#endif

enum class file_mode_t {
    read,
    write
};

class vasm_file_t {
public:
    vasm_file_t() = delete;
    vasm_file_t(file_mode_t mode);
    bool open(std::string path);
    void close();
    bool reopen();
    bool read_line(std::string& str);
    bool write_line(const std::string& str);
    bool delete_file();
    bool is_open();
    ~vasm_file_t();

    static bool delete_file(std::string path);

private:
    std::string path;
    file_mode_t mode;
    std::ifstream input;
    std::ofstream output;
};