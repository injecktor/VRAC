#include "assemble.hpp"

static vasm_file_t input_file(file_mode_t::read);
static vasm_file_t output_file(file_mode_t::write);

void assemble_init() {
    init_instr();
    tokenizer_init();
}

bool file_need_compile(const vasm_file_t& file) {
    return true;
}

void write_instr(vasm_file_t &output_file, const std::list<std::string>& decoded_strings) {
    output_file.write_line("Instructions");
    for (auto&& str : decoded_strings) {
        output_file.write_line(str);
    }
}

void create_object_file(const decode_tuple_t& decode_info) {
    vasm_file_t::delete_file(vasm_flags.output_path);
    if (!output_file.open(vasm_flags.output_path)) {
        vasm_flags.last_error_extra_msg = vasm_flags.output_path;
        throw vasm_error_t::cant_open_file;
    }
    output_file.write_line(VASM_HEX_FORMAT_STR);
    create_exports(output_file, std::get<2>(decode_info));
    create_imports_request(output_file, std::get<0>(decode_info), std::get<1>(decode_info));
    write_instr(output_file, std::get<0>(decode_info));
    
    output_file.close();
}

void vasm_assemble() {
    std::string input_path;
    std::list<token_t> tokens;
    decode_tuple_t decode_info;

    assemble_init();

    while (!vasm_flags.input_files_path.empty()) {
        input_path = std::move(vasm_flags.input_files_path.back());
        if (!input_file.open(input_path)) {
            vasm_flags.last_error_extra_msg = input_path;
            throw vasm_error_t::file_doesnt_exist;
        }
        if (file_need_compile(input_file)) {
            try {
                input_file.reopen();
                tokens = std::move(tokenize(input_file));
                decode_info = std::move(decode(tokens));
                create_object_file(decode_info);
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
        vasm_flags.input_files_path.pop();
    }
    input_file.close();
}