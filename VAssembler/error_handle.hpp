#include <iostream>
#include <vector>
#include <string>

extern size_t info_level;

enum class vasm_error_t {

};

enum class args_handle_error_t {
    arg_count,
    unexpected_arg,
    unknown_arg,
    no_input_file
};

extern void print_err(const std::string& str);
extern void print_info(const std::string& str, size_t required_info_level = 1);