#include "tokenizer.hpp"

std::unordered_map<instr_type_by_tokens_t, instr_type_t> instr_type_map;

void tokenizer_init() {
    instr_type_map.clear();
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("j", std::vector{token_type_t::reg}), instr_type_t::sr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("j", std::vector{token_type_t::literal}), instr_type_t::sl));

    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("sw", std::vector{token_type_t::reg}), instr_type_t::sr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("shw", std::vector{token_type_t::reg}), instr_type_t::sr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("sb", std::vector{token_type_t::reg}), instr_type_t::sr));

    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("lw", std::vector{token_type_t::reg}), instr_type_t::sr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("lhw", std::vector{token_type_t::reg}), instr_type_t::sr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("lb", std::vector{token_type_t::reg}), instr_type_t::sr));

    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("cpw", std::vector{token_type_t::reg, token_type_t::reg}), instr_type_t::dr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("cpw", std::vector{token_type_t::reg, token_type_t::literal}), instr_type_t::rl));

    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("cphw", std::vector{token_type_t::reg, token_type_t::reg}), instr_type_t::dr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("cphw", std::vector{token_type_t::reg, token_type_t::literal}), instr_type_t::rl));

    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("cpb", std::vector{token_type_t::reg, token_type_t::reg}), instr_type_t::dr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("cpb", std::vector{token_type_t::reg, token_type_t::literal}), instr_type_t::rl));
}

static token_t get_token(const std::string& str) {
    token_t token;
    std::unordered_map<std::string, int>::iterator map_iterator;
    int tmp = 0;
    token.type = token_type_t::none;
    if (cmd_set.find(str) != cmd_set.end()) {
        token.type = token_type_t::command;
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

std::list<token_t> tokenize(vasm_file_t& file) {
    std::list<token_t> tokens;
    token_t token_tmp;
    std::string line, token_str;
    char chr;
    while (file.read_line(line)) {
        size_t offset = 0, offset_tmp;
        while (offset < line.length()) {
            if ((offset_tmp = line.find(' ', offset)) == std::string::npos) {
                offset_tmp = line.length();
            }
            token_str = line.substr(offset, offset_tmp - offset);
            if (!token_str.empty()) {
                print_info("Tokenizing: " + token_str, 3); 
                token_tmp = std::move(get_token(token_str));
                if (token_tmp.type == token_type_t::none) {
                    vasm_flags.last_error_extra_msg = token_str;
                    throw assemble_error_t::tokenizer;
                }
                tokens.emplace_back(std::move(token_tmp));
            }
            offset = offset_tmp + 1;
        }
    }
    print_info("Tokenizer complete.", 2);
    return tokens;
}