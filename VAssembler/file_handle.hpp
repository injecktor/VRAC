#pragma once

#include <string>

#include "vasm_logging.hpp"

#if defined(WIN32) || defined(_WIN32)
#include <fstream>
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
    void write_line(const std::string& str);
    ~vasm_file_t();

private:
    std::string path;
    file_mode_t mode;
    std::ifstream input;
    std::ofstream output;
};

// class linux_file_t : public vasm_file_t {
// public:
//     linux_file_t() = delete;
//     explicit linux_file_t() = delete;
//     int open(std::string path) override final;
//     int close() override final;
//     bool read_line() override final;
// };

#if defined(WIN32) || defined(_WIN32)

#elif defined(linux)

#endif