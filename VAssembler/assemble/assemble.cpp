#include "assemble.hpp"

void assemble_init() {
    init_instr();
}

bool file_need_compile(const std::ifstream& file) {
    return true;
}

void vasm_assemble() {
    std::ifstream input_file;
    std::ofstream output_file(vasm_flags.output_path);
    std::string input_path;
    std::list<token_t> tokens;

    assemble_init();

    while (!vasm_flags.input_files_path.empty()) {
        input_path = std::move(vasm_flags.input_files_path.back());
        input_file.open(input_path);
        if (file_need_compile(input_file)) {
            try {
                tokens = tokenize(input_file);
                create_exports(tokens);
                create_imports_request(tokens);
                decode(tokens);
            }
            catch (const assemble_error_t& error) {
                throw error;
            }
            catch (...) {
                throw assemble_error_t::unknown;
            }
        }
        vasm_flags.input_files_path.pop();
    }
}