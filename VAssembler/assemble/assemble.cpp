#include "assemble.hpp"

void assemble_init() {
    init_instr();
    tokenizer_init();
}

bool file_need_compile(const vasm_file_t& file) {
    return true;
}

void write_to_file(vasm_file_t &output_file, const std::list<std::string>& decoded_strings) {
    for (auto&& str : decoded_strings) {
        output_file.write_line(str);
    }
}

void vasm_assemble() {
    vasm_file_t input_file(file_mode_t::read);
    vasm_file_t output_file(file_mode_t::write);
    
    std::string input_path;
    std::list<token_t> tokens;
    std::list<std::string> decoded_strings;

    assemble_init();

    while (!vasm_flags.input_files_path.empty()) {
        input_path = std::move(vasm_flags.input_files_path.back());
        if (!input_file.open(input_path)) {
            throw vasm_error_t::file_doesnt_exist;
        }
        if (file_need_compile(input_file)) {
            try {
                input_file.reopen();
                tokens = std::move(tokenize(input_file));
                vasm_file_t::delete_file(vasm_flags.output_path);
                output_file.open(vasm_flags.output_path);
                output_file.write_line(VASM_HEX_FORMAT_STR);
                decoded_strings = std::move(decode(tokens));
                create_exports(tokens);
                create_imports_request(tokens);
                write_to_file(output_file, decoded_strings);
                
                output_file.close();
            }
            catch (const assemble_error_t& error) {
                input_file.close();
                output_file.delete_file();
                throw error;
            }
            catch (...) {
                input_file.close();
                output_file.delete_file();
                throw assemble_error_t::unknown;
            }
        }
        vasm_flags.input_files_path.pop();
    }
    input_file.close();
}