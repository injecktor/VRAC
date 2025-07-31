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
    label,
    command,
    reg,
    literal,
    exp,
    func,
    cia
};

struct token_t {
    token_type_t type;
    std::string str;
    int num;
};

struct instr_type_by_tokens_t {
    std::string str;
    std::vector<token_type_t> types;
    instr_type_by_tokens_t(std::string str, std::vector<token_type_t> types)
        : str(str), types(types){};

    bool operator==(const instr_type_by_tokens_t& other) const {
        if (str == other.str && types == other.types) {
            return true;
        }
        return false;
    } 
};

template<>
class std::hash<instr_type_by_tokens_t> {
    public:
    size_t operator()(const instr_type_by_tokens_t &instr) const {
        std::string data = instr.str;
        for (auto&& elem : instr.types) {
            data += std::to_string(static_cast<size_t>(elem));
        }
        return std::hash<std::string>()(data);
    }
};

extern std::unordered_map<instr_type_by_tokens_t, instruction_type_t> instr_type_map;

extern void tokenizer_init();
extern std::list<token_t> tokenize(vasm_file_t& file);