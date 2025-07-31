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
            instr = ((instr_code.opcode) << 27) | ((instr_code.alucode) << 21) | (instr_tokens[1].num << 17);
            break;
        case instruction_type_t::adr:
            instr = ((instr_code.opcode) << 27) | ((instr_code.alucode) << 21) | (instr_tokens[1].num << 17) | (instr_tokens[2].num << 13);
            break;
        case instruction_type_t::arl:
            instr = ((instr_code.opcode) << 27) | ((instr_code.alucode) << 21) | (instr_tokens[1].num << 17) | instr_tokens[2].num;
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

std::list<std::string> decode(const std::list<token_t>& tokens) {
    std::list<std::string> decoded_instr;
    std::vector<decode_info_t> decode_info;
    decode_info_t *current_decode_info;
    size_t line_number = 0;

    auto tokens_it = tokens.cbegin();
    while (tokens_it != tokens.cend()) {
        switch(tokens_it->type) {
            case token_type_t::command: {
                decode_info.push_back({});
                current_decode_info = &decode_info.back();
                current_decode_info->tokens.emplace_back(*tokens_it);
                tokens_it++;
                while (tokens_it != tokens.cend() && (tokens_it->type == token_type_t::reg || tokens_it->type == token_type_t::literal || tokens_it->type == token_type_t::cia)) {
                    current_decode_info->tokens.emplace_back(*tokens_it);
                    if (tokens_it->type == token_type_t::cia) {
                        current_decode_info->token_types.emplace_back(token_type_t::literal);
                    } else {
                        current_decode_info->token_types.emplace_back(tokens_it->type);
                    }
                    tokens_it++;
                }
                current_decode_info->instr_str = get_full_instr_str(current_decode_info->tokens);
                print_info("Decoding instruction: " + current_decode_info->instr_str, 3);
                auto instr_type_pair = instr_type_map.find(instr_type_by_tokens_t(current_decode_info->tokens[0].str, current_decode_info->token_types));
                if (instr_type_pair == instr_type_map.cend()) {
                    vasm_flags.last_error_extra_msg = "Unknown instruction type of: " + current_decode_info->instr_str;
                    throw assemble_error_t::decoder;
                }
                current_decode_info->instr_type = instr_type_pair->second;
                auto instr_type_info_pair = instr_type_info_map.find(current_decode_info->instr_type);
                if (instr_type_info_pair == instr_type_info_map.cend()) {
                    vasm_flags.last_error_extra_msg = "Couldn't find instruction type information: " + current_decode_info->instr_str;
                    throw assemble_error_t::decoder;
                }
                current_decode_info->instr_size = instr_type_info_pair->second.instr_size;
                for (auto&& elem : current_decode_info->tokens) {
                    if (elem.type == token_type_t::literal) {
                        auto literal_bits_restriction = 1 << instr_type_info_pair->second.literal_bits;
                        if ((tokens_it->num >= 0 && tokens_it->num >= literal_bits_restriction) ||
                            (tokens_it->num < 0 && std::abs(tokens_it->num) > literal_bits_restriction)) {
                                vasm_flags.last_error_extra_msg = "Literal limit exceeded: " + elem.num;
                        }
                    }
                }
            } break;
            default:
                tokens_it++;
        }
    }
    
    size_t instr_common_size = 0;
    for (int i = 0; i < decode_info.size(); ++i) {
        auto &cur_decode_info = decode_info[i];
        if (cur_decode_info.token_types.back() == token_type_t::literal) {
            auto &last_token = cur_decode_info.tokens.back();
            if ((last_token.str.substr(0, 3)) == "cia") {
                int instr_count = last_token.num + i;
                if (instr_count >= decode_info.size() || instr_count < 0) {
                    vasm_flags.last_error_extra_msg = "Cia out of range";
                    throw assemble_error_t::decoder;
                }

                int new_literal = instr_common_size;
                size_t j = i;
                if (last_token.num >= 0) {
                    while (j < instr_count) {
                        new_literal += decode_info[j].instr_size;
                        j++;
                    }
                } else {
                    while (j > instr_count) {
                        new_literal -= decode_info[j].instr_size;
                        j--;
                    }
                }

                last_token.num = new_literal;
                print_info(cur_decode_info.instr_str + ": cia converted to " + std::to_string(new_literal), 3);
            }
        }
        decoded_instr.emplace_back(decode_instr(cur_decode_info.tokens, cur_decode_info.instr_type, cur_decode_info.instr_size));
        instr_common_size += cur_decode_info.instr_size;
    }
    print_info("Decode completed.", 2);
    return std::move(decoded_instr);
}