#include "vasm.hpp"

int main(int argc, char *argv[]) {
    std::string error_str;
    try {
        parse_args(argc, argv);
        vasm_pre_assemble();
        vasm_assemble();
        // vasm_link();
        // vasm_load();
        print_info("Success", 2);
    }
    catch (const vasm_error_t& error) {
        switch (error) {
            case vasm_error_t::file_doesnt_exist:
                error_str = "File doesn't exist.";
                break;
        }
    }
    catch (const args_handle_error_t& error) {
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
    catch (const pre_assemble_error_t& error) {

    }
    catch (const assemble_error_t& error) {
        switch (error) {
            case assemble_error_t::unknown:
                error_str = "Assemble error.";
                break;
            case assemble_error_t::tokenizer:
                error_str = "Tokenizer error.";
                break;
            case assemble_error_t::decoder:
                error_str = "Decoder error.";
                break;
        }
        print_err(error_str);
        return 1;
    }
    catch (const link_error_t& error) {

    }
    catch (const load_error_t& error) {

    }
    catch (...) {
        print_err("Uncaught error.");
    }

    return 0;
}