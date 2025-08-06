#include "tokenizer.hpp"

#define RETURN_TOKEN(_token_) do { \
    last_token_type = (_token_).type; \
    return token; \
} while (false);

std::unordered_map<instr_type_by_tokens_t, instruction_type_t> instr_type_map;

void tokenizer_init() {
    instr_type_map.clear();
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("j", std::vector{token_type_t::reg}), instruction_type_t::sr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("j", std::vector{token_type_t::literal}), instruction_type_t::sl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("je", std::vector{token_type_t::reg}), instruction_type_t::sr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("je", std::vector{token_type_t::literal}), instruction_type_t::sl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("jne", std::vector{token_type_t::reg}), instruction_type_t::sr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("jne", std::vector{token_type_t::literal}), instruction_type_t::sl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("jg", std::vector{token_type_t::reg}), instruction_type_t::sr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("jg", std::vector{token_type_t::literal}), instruction_type_t::sl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("jge", std::vector{token_type_t::reg}), instruction_type_t::sr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("jge", std::vector{token_type_t::literal}), instruction_type_t::sl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("jl", std::vector{token_type_t::reg}), instruction_type_t::sr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("jl", std::vector{token_type_t::literal}), instruction_type_t::sl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("jle", std::vector{token_type_t::reg}), instruction_type_t::sr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("jle", std::vector{token_type_t::literal}), instruction_type_t::sl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("push", std::vector{token_type_t::reg}), instruction_type_t::sr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("push", std::vector{token_type_t::literal}), instruction_type_t::sl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("pop", std::vector{token_type_t::reg}), instruction_type_t::sr));

    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("sw", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::dr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("shw", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::dr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("sb", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::dr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("lw", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::dr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("lhw", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::dr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("lb", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::dr));

    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("cpw", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::dr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("cpw", std::vector{token_type_t::reg, token_type_t::literal}), instruction_type_t::rl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("cphw", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::dr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("cphw", std::vector{token_type_t::reg, token_type_t::literal}), instruction_type_t::rl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("cpb", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::dr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("cpb", std::vector{token_type_t::reg, token_type_t::literal}), instruction_type_t::rl));

    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("add", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::adr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("add", std::vector{token_type_t::reg, token_type_t::literal}), instruction_type_t::arl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("min", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::adr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("min", std::vector{token_type_t::reg, token_type_t::literal}), instruction_type_t::arl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("mul", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::adr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("mul", std::vector{token_type_t::reg, token_type_t::literal}), instruction_type_t::arl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("div", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::adr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("div", std::vector{token_type_t::reg, token_type_t::literal}), instruction_type_t::arl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("or", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::adr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("or", std::vector{token_type_t::reg, token_type_t::literal}), instruction_type_t::arl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("nor", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::adr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("nor", std::vector{token_type_t::reg, token_type_t::literal}), instruction_type_t::arl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("xor", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::adr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("xor", std::vector{token_type_t::reg, token_type_t::literal}), instruction_type_t::arl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("and", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::adr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("and", std::vector{token_type_t::reg, token_type_t::literal}), instruction_type_t::arl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("nand", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::adr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("nand", std::vector{token_type_t::reg, token_type_t::literal}), instruction_type_t::arl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("shl", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::adr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("shl", std::vector{token_type_t::reg, token_type_t::literal}), instruction_type_t::arl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("shr", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::adr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("shr", std::vector{token_type_t::reg, token_type_t::literal}), instruction_type_t::arl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("cmp", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::adr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("cmp", std::vector{token_type_t::reg, token_type_t::literal}), instruction_type_t::arl));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("mod", std::vector{token_type_t::reg, token_type_t::reg}), instruction_type_t::adr));
    instr_type_map.insert(std::make_pair(instr_type_by_tokens_t("mod", std::vector{token_type_t::reg, token_type_t::literal}), instruction_type_t::arl));
}

static token_t get_token(const std::string& str) {
    static token_type_t last_token_type = token_type_t::none;
    std::unordered_map<std::string, int>::iterator map_iterator;
    std::string str_tmp;
    token_t token;
    int tmp = 0;
    token.str = str;
    if (last_token_type == token_type_t::import) {
        token.type = token_type_t::label;
    } else if (cmd_set.find(str) != cmd_set.end()) {
        token.type = token_type_t::command;
    } else if ((map_iterator = reg_map.find(str)) != reg_map.end()) {
        token.type = token_type_t::reg;
        token.num = map_iterator->second;
    } else if (str[0] == ';') {
        token.type = token_type_t::comment;
    } else if (str.substr(0, 3) == "cia") {
        token.type = token_type_t::cia;
        if (str.length() > 3) {
            str_tmp = str.substr(3);
            try {
                tmp = stoi(str_tmp, nullptr, 10);
            }
            catch(...) {
                token.type = token_type_t::none;
            }
            token.num = tmp;
        } else {
            token.num = 0;
        }
    } else if (str == "import") {
        token.type = token_type_t::import;
    } else if (str == "export") {
        token.type = token_type_t::export_def;
    } else if (str == "func") {
        token.type = token_type_t::func;
    } else if (str == "call") {
        token.type = token_type_t::call;
    } else {
        token.type = token_type_t::literal;
        try {
            size_t index;
            tmp = stoi(str, &index, 0);
            if (index != str.length()) {
                throw 0;
            }
            token.num = tmp;
        }
        catch(...) {
            if (str[str.length() - 1] == ':') {
                if (str.length() < 2) {
                    token.type = token_type_t::none;
                    RETURN_TOKEN(token);
                }
                token.type = token_type_t::label_def;
                token.str = str.substr(0, str.length() - 1);
            } else {
                token.type = token_type_t::label;
                token.num = -1;
            }
        }
    }
    RETURN_TOKEN(token);
}

std::list<token_t> tokenize(vasm_file_t& file) {
    std::list<token_t> tokens;
    token_t token_tmp;
    std::string line, token_str;
    size_t line_number = 1;
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
                } else if (token_tmp.type == token_type_t::comment) {
                    break;
                }
                token_tmp.line_number = line_number;
                tokens.emplace_back(std::move(token_tmp));
            }
            offset = offset_tmp + 1;
        }
        line_number++;
    }
    print_info("Tokenizer completed.", 2);
    return tokens;
}