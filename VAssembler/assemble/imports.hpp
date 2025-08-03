#pragma once

#include <sstream>

#include "tokenizer.hpp"
#include "decoder.hpp"

extern void create_imports_request(vasm_file_t& output_file, const std::list<std::string>& decoded_strings, 
    const import_request_t& import_labels);