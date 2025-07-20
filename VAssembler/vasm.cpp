#include "vasm.hpp"

int main(int argc, char *argv[]) {
    try {
        parse_args(argc, argv);
    }
    catch (const args_handle_error_t& error) {
        std::string error_str;
        switch (error) {
            case args_handle_error_t::arg_count:
                error_str = "Too few arguments.";
                break;
            case args_handle_error_t::unexpected_arg:
                error_str = "Unexpected argument.";
                break;
            case args_handle_error_t::unknown_arg:
                error_str = "Unknown argument.";
                break;
            case args_handle_error_t::no_input_file:
                error_str = "No input file.";
                break;
            case args_handle_error_t::duplicated_flag:
                error_str = "Duplicated flag.";
                break;
        }
        print_err(error_str);
        return 1;
    }

    try {
        vasm_pre_assemble();
    }
    catch (const pre_assemble_error_t& error) {

    }

    try {
        vasm_assemble();
    }
    catch (const assemble_error_t& error) {

    }

    try {
        // vasm_link();
    }
    catch (const link_error_t& error) {

    }

    try {
        // vasm_load();
    }
    catch (const load_error_t& error) {

    }

    print_info("Success", 2);
    return 0;
}