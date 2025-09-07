#pragma once

#include "tokenizer.hpp"
#include <queue>
#include <tuple>
#include <array>

struct decode_info_t {
    std::vector<token_t> tokens;
    std::vector<token_type_t> token_types;
    instruction_type_t instr_type;
    std::string instr_str;
    size_t instr_size;
    size_t func_num;
    bool need_addr;
    bool call;
};

using import_request_t = std::unordered_set<std::string>;
using vasm_function_t = std::unordered_map<std::string, std::pair<size_t, size_t>>;
using local_function_t = vasm_function_t;
using export_function_t = vasm_function_t;
using decode_tuple_t = std::tuple<std::list<std::string>, import_request_t, local_function_t, export_function_t, size_t>;

extern decode_tuple_t decode(std::list<token_t>& tokens);