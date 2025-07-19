#include <string>
#include <queue>

#include "error_handle.hpp"

#define DEFAULT_VASM_EXTENSION "vasm"

struct vasm_flags_t {
    std::queue<std::string> input_files_path;
    std::string output_path;
};

extern vasm_flags_t vasm_flags;

extern void parse_args(int argc, char *argv[]);