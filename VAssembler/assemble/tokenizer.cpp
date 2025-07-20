#include "tokenizer.hpp"

static token_t get_token(const std::string& str) {
    token_t token;
    std::unordered_map<std::string, int>::iterator map_iterator;
    int tmp = 0;
    token.type = token_type_t::none;
    if (instr_set.find(str) != instr_set.end()) {
        token.type = token_type_t::instruction;
        token.str = str;
    } else if ((map_iterator = reg_map.find(str)) != reg_map.end()) {
        token.type = token_type_t::reg;
        token.num = map_iterator->second;
    } else {
        token.type = token_type_t::literal;
        try {
            tmp = stoi(str);
        }
        catch(...) {
            token.type = token_type_t::none;
        }
        token.num = tmp;
    }
    return token;
}

std::list<token_t> tokenize(std::ifstream& file) {
    std::list<token_t> tokens;
    token_t token_tmp;
    std::string line, token_str;
    while (std::getline(file, line)) {
        size_t offset = 0, offset_tmp;
        while ((offset_tmp = line.find(' ', offset)) != std::string::npos) {
            token_str = line.substr(offset + 1, offset_tmp - offset);
            token_tmp = std::move(get_token(token_str));
            if (token_tmp.type == token_type_t::none) {
                vasm_flags.last_error_extra_msg = token_str;
                throw assemble_error_t::tokenizer;
            }
            tokens.emplace_back();
        }
    }
    return tokens;
}