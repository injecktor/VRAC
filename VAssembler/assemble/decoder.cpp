#include "decoder.hpp"

#include <climits>
#include <sstream>
#include <iomanip>

std::string get_full_instr_str(std::vector<token_t> instr_tokens) {
    std::string str;
    for (auto&& elem : instr_tokens) {
        str += elem.str;
        str += ' ';
    }
    return str;
}

static std::string decode_instr(std::vector<token_t> instr_tokens, const instruction_type_t& type, size_t instr_size) {
    std::stringstream stream;
    size_t instr = 0;
    auto instr_code_pair = instr_map.find(instruction_t(instr_tokens[0].str, type));
    if (instr_code_pair == instr_map.cend()) {
        vasm_flags.last_error_extra_msg = "Unknown opcode: " + get_full_instr_str(instr_tokens);
        throw 0;
    }
    auto instr_code = instr_code_pair->second;
    switch (type) {
        case instruction_type_t::sl:
            instr = ((instr_code.opcode) << 27) | instr_tokens[1].num;
            break;
        case instruction_type_t::sr:
            instr = ((instr_code.opcode) << 27) | (instr_tokens[1].num << 23);
            break;
        case instruction_type_t::dr:
            instr = ((instr_code.opcode) << 27) | (instr_tokens[1].num << 23) | (instr_tokens[2].num << 19);
            break;
        case instruction_type_t::rl:
            instr = ((instr_code.opcode) << 27) | (instr_tokens[1].num << 23) | (instr_tokens[2].num << 19);
            break;
        case instruction_type_t::asr:
            instr = ((instr_code.opcode) << 27) | ((instr_code.opcode) << 21) | (instr_tokens[1].num << 17);
            break;
        case instruction_type_t::adr:
            instr = ((instr_code.opcode) << 27) | ((instr_code.opcode) << 21) | (instr_tokens[1].num << 17) | (instr_tokens[2].num << 13);
            break;
        case instruction_type_t::arl:
            instr = ((instr_code.opcode) << 27) | ((instr_code.opcode) << 21) | (instr_tokens[1].num << 17) | instr_tokens[2].num;
            break;
        default:
            vasm_flags.last_error_extra_msg = "Unknown instruction type: " + static_cast<size_t>(type);
            throw;
        throw;
    }
    stream << std::setfill('0') << std::hex << std::setw(2) << ((instr & 0xff000000) >> 24) << ' ' 
        << std::setw(2) << ((instr & 0xff0000) >> 16);
    if (instr_size > 2) { 
        stream << std::setfill('0') << std::hex << ' ' << std::setw(2) << ((instr & 0xff00) >> 8);
        if (instr_size == 4) {
            stream << std::setfill('0') << std::hex << ' ' << std::setw(2) << (instr & 0xff);
        }
    }
    return stream.str();
}

void decode(const std::list<token_t>& tokens, vasm_file_t& output) {
    std::vector<token_type_t> token_types;
    std::vector<token_t> instr_tokens;
    std::string full_instr;
    size_t line_number = 0;

    tokenizer_init();

    auto it = tokens.cbegin();
    int expect_operand_count = -1;
    while (it != tokens.cend()) {
        switch(it->type) {
            case token_type_t::command: {
                line_number++;
                token_types.clear();
                instr_tokens.clear();
                instr_tokens.emplace_back(*it);
                it++;
                while (it != tokens.cend() && (it->type == token_type_t::reg || it->type == token_type_t::literal)) {
                    instr_tokens.emplace_back(*it);
                    token_types.emplace_back(it->type);
                    it++;
                }
                full_instr = get_full_instr_str(instr_tokens);
                print_info("Decoding instruction: " + full_instr, 3);
                auto instr_type_pair = instr_type_map.find(instr_type_by_tokens_t(instr_tokens[0].str, token_types));
                if (instr_type_pair == instr_type_map.cend()) {
                    vasm_flags.last_error_extra_msg = "Unknown instruction type of: " + full_instr;
                    throw assemble_error_t::decoder;
                }
                auto instr_type = instr_type_pair->second;
                for (auto&& elem : instr_tokens) {
                    if (elem.type == token_type_t::literal) {
                        auto literal_bits_restriction = 1 << instr_type_info_map.find(instr_type)->second.literal_bits;
                        if ((it->num >= 0 && it->num >= literal_bits_restriction) ||
                            it->num < 0 && std::abs(it->num) > literal_bits_restriction) {
                                vasm_flags.last_error_extra_msg = "Literal limit exceeded: " + elem.num;
                        }
                    }
                }
                try {
                    output.write_line(decode_instr(instr_tokens, instr_type, instr_type_info_map.find(instr_type)->second.instr_size));
                }
                catch (...) {
                    throw assemble_error_t::decoder;
                }

                } break;
            default:
                it++;
        }
    }
    print_info("Decode completed.", 2);
}