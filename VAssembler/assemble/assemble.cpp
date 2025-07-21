#include "assemble.hpp"

void assemble_init() {
    init_instr();
}

bool file_need_compile(const vasm_file_t& file) {
    return true;
}

void vasm_assemble() {
    vasm_file_t input_file(file_mode_t::read);
    
    std::string input_path;
    std::list<token_t> tokens;

    assemble_init();

    while (!vasm_flags.input_files_path.empty()) {
        input_path = std::move(vasm_flags.input_files_path.back());
        input_file.open(input_path);
        if (file_need_compile(input_file)) {
            try {
                tokens = tokenize(input_file);
                input_file.reopen();
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