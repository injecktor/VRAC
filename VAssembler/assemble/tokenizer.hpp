#pragma once

#include <string>
#include <list>
#include <fstream>
#include <unordered_set>
#include <unordered_map>

#include "instructions.hpp"
#include "vasm_flags.hpp"
#include "vasm_errors.hpp"
#include "file_handle.hpp"

enum class token_type_t {
    none,
    marker,
    command,
    reg,
    literal,
    func
};

struct token_t {
    token_type_t type;
    std::string str;
    int num;
};

extern std::list<token_t> tokenize(vasm_file_t& file);