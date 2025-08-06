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

static std::string decode_instr(std::vector<token_t> instr_tokens, const instruction_type_t& type, size_t instr_size, bool need_addr, bool call) {
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
    if (need_addr) {
        auto& last_token = instr_tokens.back();
        stream << " &" << last_token.num;
        if (call) {
            stream << " #" << last_token.str;
        }
    } else if (instr_size > 2) {
        stream << ' ' << std::setw(2) << ((instr & 0xff00) >> 8);
        if (instr_size == 4) {
            stream << ' ' << std::setw(2) << (instr & 0xff);
        }
    }
    return stream.str();
}

decode_tuple_t decode(std::list<token_t>& tokens) {
    std::list<std::string> decoded_instr;
    std::vector<decode_info_t> decode_info;
    std::vector<std::unordered_map<std::string, size_t>> labels;
    local_function_t local_funcs;
    export_function_t export_funcs;
    vasm_function_t::iterator last_func;
    import_request_t import_funcs;
    size_t instr_total_size = 0;
    size_t function_count = 0;
    
    auto tokens_it = tokens.begin();
    while (tokens_it != tokens.end()) {
        switch(tokens_it->type) {
            case token_type_t::command: {
                decode_info.push_back({});
                auto &current_decode_info = decode_info.back();
                if (!function_count) {
                    THROW_ERROR(tokens_it->line_number, "Instructions outside function are forbidden.");
                }
                current_decode_info.func_num = function_count - 1;
                current_decode_info.need_addr = false;
                current_decode_info.call = false;
                current_decode_info.tokens.emplace_back(*tokens_it);
                tokens_it++;
                while (tokens_it != tokens.end() && (tokens_it->type == token_type_t::reg || tokens_it->type == token_type_t::literal 
                        || tokens_it->type == token_type_t::cia || tokens_it->type == token_type_t::label)) {
                    current_decode_info.tokens.emplace_back(*tokens_it);
                    if (tokens_it->type == token_type_t::cia || tokens_it->type == token_type_t::label) {
                        current_decode_info.call = tokens_it->call;
                        current_decode_info.need_addr = true;
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
                instr_total_size += current_decode_info.instr_size;
            } break;
            case token_type_t::label_def: {
                if (!function_count) {
                    THROW_ERROR(tokens_it->line_number, "Instructions outside function are forbidden.");
                }
                auto res = labels.back().insert(std::make_pair(tokens_it->str, instr_total_size));
                if (!res.second) {
                    vasm_flags.last_error_extra_msg = "Non unique label: " + tokens_it->str;
                    throw assemble_error_t::decoder;
                }

                tokens_it++;
            } break;
            case token_type_t::import: {
                if (!function_count) {
                    THROW_ERROR(tokens_it->line_number, "Instructions outside function are forbidden.");
                }
                tokens_it++;
                if (tokens_it == tokens.end() || tokens_it->type != token_type_t::label) {
                    THROW_ERROR(tokens_it->line_number, "Expected import label.");
                }
                
                tokens_it++;
            } break;
            case token_type_t::call: {
                if (!function_count) {
                    THROW_ERROR(tokens_it->line_number, "Instructions outside function are forbidden.");
                }
                tokens_it++;
                if (tokens_it == tokens.end() || tokens_it->type != token_type_t::label) {
                    THROW_ERROR(tokens_it->line_number, "Expected label.");
                }

                auto next_tiken = tokens_it;
                next_tiken++;
                tokens.insert(next_tiken, token_t(token_type_t::command, "push", 0, tokens_it->line_number));
                tokens.insert(next_tiken, token_t(token_type_t::cia, "cia+2", 2, tokens_it->line_number));
                tokens.insert(next_tiken, token_t(token_type_t::command, "j", 0, tokens_it->line_number));
                tokens.insert(next_tiken, token_t(token_type_t::label, tokens_it->str, 0, tokens_it->line_number, true));

                tokens_it++;
            } break;
            case token_type_t::export_def: {
                if (function_count) {
                    last_func->second.second = instr_total_size - last_func->second.first;
                }

                tokens_it++;
                if (tokens_it == tokens.end() || tokens_it->type != token_type_t::func) {
                    THROW_ERROR(tokens_it->line_number, "Expected func.");
                }
                tokens_it++;
                if (tokens_it == tokens.end() || tokens_it->type != token_type_t::label_def) {
                    THROW_ERROR(tokens_it->line_number, "Expected func.");
                }

                auto res = export_funcs.insert(std::make_pair(tokens_it->str, std::make_pair(instr_total_size, 0)));
                if (!res.second) {
                    vasm_flags.last_error_extra_msg = "Non unique label: " + tokens_it->str;
                    throw assemble_error_t::decoder;
                }
                last_func = res.first;

                labels.push_back({});
                function_count++;
                tokens_it++;
            } break;
            case token_type_t::func: {
                if (function_count) {
                    last_func->second.second = instr_total_size - last_func->second.first;
                }

                tokens_it++;
                if (tokens_it == tokens.end() || tokens_it->type != token_type_t::label_def) {
                    THROW_ERROR(tokens_it->line_number, "Expected func.");
                }

                auto res = local_funcs.insert(std::make_pair(tokens_it->str, std::make_pair(instr_total_size, 0)));
                if (!res.second) {
                    vasm_flags.last_error_extra_msg = "Non unique label: " + tokens_it->str;
                    throw assemble_error_t::decoder;
                }
                last_func = res.first;

                labels.push_back({});
                function_count++;
                tokens_it++;
            } break;
            default: {
                THROW_ERROR(tokens_it->line_number, tokens_it->str);
            }
        }
    }
    last_func->second.second = instr_total_size - last_func->second.first;
    
    instr_total_size = 0;
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

                    int new_literal = instr_total_size;
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
                    auto local_labels_pair = labels[cur_decode_info.func_num].find(last_token.str);
                    auto local_funcs_pair = local_funcs.find(last_token.str);
                    auto export_funcs_pair = export_funcs.find(last_token.str);
                    if (local_funcs_pair != local_funcs.end()) {
                        last_token.num = local_funcs_pair->second.first;
                    } else if (export_funcs_pair != export_funcs.end()) {
                        last_token.num = export_funcs_pair->second.first;
                    } else if (last_token.str.find('.') != std::string::npos) {
                        if (!last_token.call) {
                            THROW_ERROR(cur_decode_info.tokens.cbegin()->line_number, "Can't jump to import function: " + last_token.str);
                        }
                        import_funcs.insert(last_token.str);
                    } else {
                        THROW_ERROR(cur_decode_info.tokens.cbegin()->line_number, "Unknown label: " + last_token.str);
                    }
                } break;
            }
        }
        try {
            decoded_instr.emplace_back(decode_instr(cur_decode_info.tokens, cur_decode_info.instr_type, 
                cur_decode_info.instr_size, cur_decode_info.need_addr, cur_decode_info.call));
        }
        catch (...) {
            vasm_flags.last_error_extra_msg = std::to_string(cur_decode_info.tokens.cbegin()->line_number) + vasm_flags.last_error_extra_msg;
            throw assemble_error_t::decoder;
        }
        instr_total_size += cur_decode_info.instr_size;
    }
    print_info("Decode completed.", 2);
    return std::make_tuple(decoded_instr, import_funcs, local_funcs, export_funcs, instr_total_size);
}