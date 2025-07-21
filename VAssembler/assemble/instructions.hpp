#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>

enum class instr_type_t {
    unknown,
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
    instr_type_t type;
    instruction_t(std::string cmd, instr_type_t type)
        : cmd(cmd), type(type){};

    bool operator==(const instruction_t& other) const {
        if (cmd == other.cmd && type == other.type) {
            return true;
        }
        return false;
    } 
};

struct vasm_code_t {
    int opcode;
    int alucode;
    vasm_code_t(int opcode, int alucode)
        : opcode(opcode), alucode(alucode){};

    bool operator==(const vasm_code_t& other) const {
        if (opcode == other.opcode && alucode == other.alucode) {
            return true;
        }
        return false;
    } 
};

template<>
class std::hash<instruction_t> {
    public:
    size_t operator()(const instruction_t &instr) const {
        std::string data = instr.cmd + std::to_string(static_cast<size_t>(instr.type));
        return std::hash<std::string>()(data);
    }
};

template<>
class std::hash<vasm_code_t> {
    public:
    size_t operator()(const vasm_code_t &code) const {
        std::string data = std::to_string(code.opcode) + std::to_string(code.alucode);
        return std::hash<std::string>()(data);
    }
};



extern std::unordered_set<std::string> cmd_set;
extern std::unordered_map<std::string, int> reg_map;
extern std::unordered_map<instruction_t, vasm_code_t> instr_map;

extern void init_instr();