#include "assemble.hpp"

bool file_need_compile() {
    
}

void vasm_assemble() {
    std::ifstream file;
    std::string path;
    while (!vasm_flags.input_files_path.empty()) {
        path = std::move(vasm_flags.input_files_path.back());
        file.open(path);
        if (file_need_compile()) {

        }
    }
}