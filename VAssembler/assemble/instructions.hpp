#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>

struct instruction_t {
    int opcode;
    int alucode;
};

extern std::unordered_set<std::string> instr_set;
extern std::unordered_map<std::string, int> reg_map;

extern void init_instr();