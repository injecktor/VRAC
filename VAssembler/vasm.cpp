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
        }
        print_err(error_str);
        return 1;
    }
    return 0;
}