#include <string>
#include <queue>

#include "error_handle.hpp"

struct vasm_flags {
    std::queue<std::string> input_files_path;
    std::string output_path;
};

extern void parse_args(int argc, char *argv[]);