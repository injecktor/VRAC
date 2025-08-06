#include "assemble.hpp"

void assemble_init() {
    init_instr();
    tokenizer_init();
}

bool file_need_compile(const vasm_file_t& file) {
    return true;
}

std::vector<std::string> vasm_assemble() {
    vasm_file_t input_file(file_mode_t::read);
    vasm_file_t output_file(file_mode_t::write);
    std::vector<std::string> compiled_paths;
    std::string input_path;
    std::string output_path;
    std::list<token_t> tokens;
    decode_tuple_t decode_info;

    assemble_init();

    while (!vasm_flags.input_files_path.empty()) {
        input_path = std::move(vasm_flags.input_files_path.back());
        if (!input_file.open(input_path)) {
            vasm_flags.last_error_extra_msg = input_path;
            throw vasm_error_t::file_doesnt_exist;
        }
        output_path = input_path;
        if (file_need_compile(input_file)) {
            try {
                output_path += DEFAULT_VASM_OBJ_EXTENSION;
                input_file.reopen();
                tokens = std::move(tokenize(input_file));
                decode_info = std::move(decode(tokens));
                create_object_file(output_file, output_path, decode_info);
            }
            catch (const assemble_error_t& error) {
                input_file.close();
                output_file.delete_file();
                throw error;
            }
            catch (const vasm_error_t& error) {
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
        compiled_paths.emplace_back(output_path);
        vasm_flags.input_files_path.pop();
    }
    input_file.close();
    return compiled_paths;
}