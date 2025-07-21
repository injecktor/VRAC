#include "decoder.hpp"

void decode(const std::list<token_t>& tokens) {
    vasm_file_t output(file_mode_t::write);
    std::vector<token_type_t> types;
    token_t cmd_token;

    std::unordered_map<instr_type_by_tokens_t, instr_type_t>::const_iterator instr_pair;

    tokenizer_init();

    auto it = tokens.cbegin();
    int expect_operand_count = -1;
    while (it != tokens.cend()) {
        switch(it->type) {
            case token_type_t::command:
                cmd_token = *it;
                types.clear();
                it++;
                while (it != tokens.cend() && (it->type == token_type_t::reg || it->type == token_type_t::literal)) {
                    types.push_back(it->type);
                    it++;
                }
                instr_pair = instr_type_map.find(instr_type_by_tokens_t(cmd_token.str, types));
                if (instr_pair == instr_type_map.cend()) {
                    vasm_flags.last_error_extra_msg = "Unknown instruction type of: " + cmd_token.str;
                    for (auto&& elem : types) {
                        vasm_flags.last_error_extra_msg += " " + std::to_string(static_cast<size_t>(elem));
                    }
                    throw assemble_error_t::decoder;
                }
                print_info("Parsed instruction: " + cmd_token.str, 3);


                // auto code = instr_map.find(instruction_t(token.str, ));
                break;
            default:
                it++;
        }
    }
    
}