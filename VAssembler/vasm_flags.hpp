#pragma once
#include <string>
#include <queue>

struct vasm_flags_t {
    std::queue<std::string> input_files_path;
    std::string output_path;

    int info_level = -1;
    std::string last_error_extra_msg;

    bool pre_assemble = 1;
    bool assemble = 1;
    bool link = 1;
    bool load = 1;
};

extern vasm_flags_t vasm_flags;