#pragma once

#include "tokenizer.hpp"

struct decode_info_t {
    std::vector<token_t> tokens;
    std::vector<token_type_t> token_types;
    instruction_type_t instr_type;
    std::string instr_str;
    size_t instr_size;
};

extern std::list<std::string> decode(const std::list<token_t>& tokens);