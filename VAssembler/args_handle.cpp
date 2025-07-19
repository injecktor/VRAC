#include "args_handle.hpp"

vasm_flags_t vasm_flags;

void parse_args(int argc, char *argv[]) {
    if (argc < 2) {
        throw args_handle_error_t::arg_count;
    }

    bool arg_complete = true;
    std::string arg;
    std::string *arg_dest;
    for (size_t i = 1; i < argc; i++) {
        arg = argv[i];
        if (arg_complete == false) {
            if (arg[0] == '-') {
                throw args_handle_error_t::unexpected_arg;
            }
            *arg_dest = arg;
            arg_complete = true;
            continue;
        }

        if (arg[0] != '-') {
            vasm_flags.input_files_path.emplace(std::move(arg));
            continue;
        }

        if (arg == "-o") {
            arg_complete = false;
            arg_dest = &vasm_flags.output_path;
            continue;
        }

        throw args_handle_error_t::unknown_arg;
    }

    if (arg_complete == false) {
        throw args_handle_error_t::arg_count;
    }

    if (vasm_flags.input_files_path.empty()) {
        throw args_handle_error_t::no_input_file;
    }

    if (vasm_flags.output_path.empty()) {
        vasm_flags.output_path = vasm_flags.input_files_path.front() + '.' + DEFAULT_VASM_EXTENSION;
    }
}