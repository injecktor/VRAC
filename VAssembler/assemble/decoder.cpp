#include "decoder.hpp"

#include <climits>
#include <sstream>
#include <iomanip>

#define THROW_ERROR(_line_number_, _text_) do { \
    vasm_flags.last_error_extra_msg = std::to_string(_line_number_); \
    vasm_flags.last_error_extra_msg += ": "; \
    vasm_flags.last_error_extra_msg += _text_; \
    throw assemble_error_t::decoder; \
} while(false);

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
            instr = ((instr_code.opcode) << 27) | (instr_tokens[1].num & 0xffff);
            break;
        case instruction_type_t::sr:
            instr = ((instr_code.opcode) << 27) | (instr_tokens[1].num << 23);
            break;
        case instruction_type_t::dr:
            instr = ((instr_code.opcode) << 27) | (instr_tokens[1].num << 23) | (instr_tokens[2].num << 19);
            break;
        case instruction_type_t::rl:
            instr = ((instr_code.opcode) << 27) | (instr_tokens[1].num << 23) | (instr_tokens[2].num & 0xffff);
            break;
        case instruction_type_t::asr:
            instr = ((instr_code.opcode) << 27) | ((instr_code.alucode) << 21) | (instr_tokens[1].num << 17);
            break;
        case instruction_type_t::adr:
            instr = ((instr_code.opcode) << 27) | ((instr_code.alucode) << 21) | (instr_tokens[1].num << 17) | (instr_tokens[2].num << 13);
            break;
        case instruction_type_t::arl:
            instr = ((instr_code.opcode) << 27) | ((instr_code.alucode) << 21) | (instr_tokens[1].num << 17) | (instr_tokens[2].num & 0xffff);
            break;
        default:
            vasm_flags.last_error_extra_msg = "Unknown instruction type: " + static_cast<size_t>(type);
            throw;
        throw;
    }
    stream << std::setfill('0') << std::hex << std::setw(2) << ((instr & 0xff000000) >> 24) << ' ' 
        << std::setw(2) << ((instr & 0xff0000) >> 16);
    if (instr_tokens.back().type == token_type_t::label && instr_tokens.back().num == -1) {
        stream << " &" << instr_tokens.back().str;
    } else if (instr_size > 2) {
        stream << std::setfill('0') << std::hex << ' ' << std::setw(2) << ((instr & 0xff00) >> 8);
        if (instr_size == 4) {
            stream << std::setfill('0') << std::hex << ' ' << std::setw(2) << (instr & 0xff);
        }
    }
    return stream.str();
}

decode_tuple_t decode(std::list<token_t>& tokens) {
    std::list<token_type_t> prev_token_types;
    std::list<std::string> decoded_instr;
    std::vector<decode_info_t> decode_info;
    std::unordered_map<std::string, size_t> local_labels;
    export_label_t export_labels;
    export_label_t::iterator last_export_label;
    import_request_t import_labels;
    std::queue<token_type_t> wanted_tokens;
    size_t instr_common_size = 0;
    bool label_need_size = false;
    
    auto tokens_it = tokens.begin();
    while (tokens_it != tokens.end()) {
        bool found_wanted_token = true;
        while (!wanted_tokens.empty()) {
            found_wanted_token = false;
            if (tokens_it->type == wanted_tokens.back()) {
                while (!wanted_tokens.empty()) {
                    wanted_tokens.pop();
                }
                found_wanted_token = true;
                break;
            }
            wanted_tokens.pop();
        }
        if (!found_wanted_token) {
            vasm_flags.last_error_extra_msg = "Couldn't find wanted token.";
            throw assemble_error_t::decoder;
        }
        switch(tokens_it->type) {
            case token_type_t::command: {
                decode_info.push_back({});
                auto &current_decode_info = decode_info.back();
                current_decode_info.tokens.emplace_back(*tokens_it);
                tokens_it++;
                while (tokens_it != tokens.end() && (tokens_it->type == token_type_t::reg || tokens_it->type == token_type_t::literal 
                        || tokens_it->type == token_type_t::cia || tokens_it->type == token_type_t::label)) {
                    current_decode_info.tokens.emplace_back(*tokens_it);
                    if (tokens_it->type == token_type_t::cia || tokens_it->type == token_type_t::label) {
                        current_decode_info.token_types.emplace_back(token_type_t::literal);
                    } else {
                        current_decode_info.token_types.emplace_back(tokens_it->type);
                    }
                    tokens_it++;
                }
                current_decode_info.instr_str = get_full_instr_str(current_decode_info.tokens);
                print_info("Decoding instruction: " + current_decode_info.instr_str, 3);
                auto instr_type_pair = instr_type_map.find(instr_type_by_tokens_t(current_decode_info.tokens[0].str, current_decode_info.token_types));
                if (instr_type_pair == instr_type_map.cend()) {
                    THROW_ERROR(tokens_it->line_number, "Unknown instruction type of: " + current_decode_info.instr_str);
                }
                current_decode_info.instr_type = instr_type_pair->second;
                auto instr_type_info_pair = instr_type_info_map.find(current_decode_info.instr_type);
                if (instr_type_info_pair == instr_type_info_map.cend()) {
                    THROW_ERROR(tokens_it->line_number, "Couldn't find instruction type information: " + current_decode_info.instr_str);
                }
                current_decode_info.instr_size = instr_type_info_pair->second.instr_size;
                for (auto&& elem : current_decode_info.tokens) {
                    if (elem.type == token_type_t::literal) {
                        auto literal_bits_restriction = 1 << instr_type_info_pair->second.literal_bits;
                        if ((elem.num >= 0 && elem.num >= literal_bits_restriction) ||
                            (elem.num < 0 && std::abs(elem.num) > literal_bits_restriction)) {
                                THROW_ERROR(tokens_it->line_number, "Literal limit exceeded: " + elem.num);
                        }
                    }
                }
                instr_common_size += current_decode_info.instr_size;
            } break;
            case token_type_t::label_def: {
                tokens_it->num = instr_common_size;
                if (prev_token_types.size() == 2 && prev_token_types.front() == token_type_t::export_def 
                    && prev_token_types.back() == token_type_t::func) {
                    auto res = export_labels.insert(std::make_pair(tokens_it->str, std::make_pair(instr_common_size, 0)));
                    if (!res.second) {
                        vasm_flags.last_error_extra_msg = "Non unique label: " + tokens_it->str;
                        throw assemble_error_t::decoder;
                    }
                    last_export_label = res.first;
                    label_need_size = true;
                } else {
                    auto res = local_labels.insert(std::make_pair(tokens_it->str, instr_common_size));
                    if (!res.second) {
                        vasm_flags.last_error_extra_msg = "Non unique label: " + tokens_it->str;
                        throw assemble_error_t::decoder;
                    }
                }
                prev_token_types.clear();
                tokens_it++;
            } break;
            case token_type_t::import: {
                wanted_tokens.emplace(token_type_t::import_label);
                tokens_it++;
            } break;
            case token_type_t::import_label: {
                tokens_it++;
            } break;
            case token_type_t::export_def: {
                wanted_tokens.emplace(token_type_t::func);
                prev_token_types.emplace_back(token_type_t::export_def);
                tokens_it++;
            } break;
            case token_type_t::func: {
                if (label_need_size) {
                    last_export_label->second.second = instr_common_size - last_export_label->second.first;
                    label_need_size = false;
                }
                wanted_tokens.emplace(token_type_t::label_def);
                prev_token_types.emplace_back(token_type_t::func);
                tokens_it++;
            } break;
            default: {
                vasm_flags.last_error_extra_msg = tokens_it->str;
                throw assemble_error_t::decoder;
                THROW_ERROR(tokens_it->line_number, tokens_it->str);
            }
        }
    }
    if (!wanted_tokens.empty()) {
        vasm_flags.last_error_extra_msg = "Expected more tokens.";
        throw assemble_error_t::decoder;
    }
    
    instr_common_size = 0;
    for (int i = 0; i < decode_info.size(); ++i) {
        auto &cur_decode_info = decode_info[i];
        auto &last_token = cur_decode_info.tokens.back();
        if (cur_decode_info.token_types.back() == token_type_t::literal) {
            switch (last_token.type) {
                case token_type_t::cia: {
                    int instr_count = last_token.num + i;
                    if (instr_count >= decode_info.size() || instr_count < 0) {
                        THROW_ERROR(cur_decode_info.tokens.cbegin()->line_number, "Cia out of range");
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
                    print_info(cur_decode_info.instr_str + ": " + last_token.str + " converted to " + std::to_string(new_literal), 3);
                } break;
                case token_type_t::label: {
                    auto local_labels_pair = local_labels.find(last_token.str);
                    auto export_labels_pair = export_labels.find(last_token.str);
                    if (local_labels_pair != local_labels.end()) {
                        last_token.num = local_labels_pair->second;
                    } else if (export_labels_pair != export_labels.end()) {
                        last_token.num = export_labels_pair->second.first;
                    } else if (last_token.str.find('.') != std::string::npos) {
                        import_labels.insert(last_token.str);
                    } else {
                        THROW_ERROR(cur_decode_info.tokens.cbegin()->line_number, "Unknown label: " + last_token.str);
                    }
                } break;
            }
        }
        try {
            decoded_instr.emplace_back(decode_instr(cur_decode_info.tokens, cur_decode_info.instr_type, cur_decode_info.instr_size));
        }
        catch (...) {
            vasm_flags.last_error_extra_msg = std::to_string(cur_decode_info.tokens.cbegin()->line_number) + vasm_flags.last_error_extra_msg;
            throw assemble_error_t::decoder;
        }
        instr_common_size += cur_decode_info.instr_size;
    }
    print_info("Decode completed.", 2);
    return std::make_tuple(decoded_instr, import_labels, export_labels);
}