#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>

enum class instruction_type_t {
    sl,
    sr,
    dr,
    rl,
    asr,
    adr,
    arl
};

struct instruction_t {
    std::string cmd;
    instruction_type_t type;
    instruction_t(std::string cmd, instruction_type_t type)
        : cmd(cmd), type(type){};

    bool operator==(const instruction_t& other) const {
        if (cmd == other.cmd && type == other.type) {
            return true;
        }
        return false;
    } 
};

struct instruction_code_t {
    size_t opcode;
    size_t alucode;
    instruction_code_t(size_t opcode, size_t alucode)
        : opcode(opcode), alucode(alucode){};
};

struct instruction_type_info_t {
    size_t literal_bits;
    size_t instr_size;
    instruction_type_info_t(size_t literal_bits, size_t instr_size)
        : literal_bits(literal_bits), instr_size(instr_size){};
};

template<>
class std::hash<instruction_t> {
    public:
    size_t operator()(const instruction_t &instr) const {
        std::string data = instr.cmd + std::to_string(static_cast<size_t>(instr.type));
        return std::hash<std::string>()(data);
    }
};

extern std::unordered_set<std::string> cmd_set;
extern std::unordered_map<std::string, int> reg_map;
extern std::unordered_map<instruction_t, instruction_code_t> instr_map;
extern std::unordered_map<instruction_type_t, instruction_type_info_t> instr_type_info_map;

extern void init_instr();