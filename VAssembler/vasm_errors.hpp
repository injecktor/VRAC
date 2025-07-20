#pragma once
#include "vasm_logging.hpp"

enum class args_handle_error_t {
    arg_count,
    unexpected_arg,
    unknown_arg,
    no_input_file,
    duplicated_flag
};

enum class pre_assemble_error_t {

};

enum class assemble_error_t {
    unknown,
    tokenizer,

};

enum class link_error_t {

};

enum class load_error_t {

};