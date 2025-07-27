#include "instructions.hpp"


std::unordered_set<std::string> cmd_set;
std::unordered_map<std::string, int> reg_map;
std::unordered_map<instruction_t, instruction_code_t> instr_map;
std::unordered_map<instruction_type_t, instruction_type_info_t> instr_type_info_map;

inline static void instr_map_insert(std::string cmd, instruction_type_t type, size_t opcode, size_t alucode) {
    instr_map.insert(std::make_pair(instruction_t(cmd, type), instruction_code_t(opcode, alucode)));
}

void init_instr() {
    cmd_set.clear();
    cmd_set.insert("j");
    cmd_set.insert("je");
    cmd_set.insert("jne");
    cmd_set.insert("jg");
    cmd_set.insert("jge");
    cmd_set.insert("jl");
    cmd_set.insert("jle");
    cmd_set.insert("sw");
    cmd_set.insert("shw");
    cmd_set.insert("sb");
    cmd_set.insert("lw");
    cmd_set.insert("lhw");
    cmd_set.insert("lb");
    cmd_set.insert("cpw");
    cmd_set.insert("cphw");
    cmd_set.insert("cpb");
    cmd_set.insert("add");
    cmd_set.insert("min");
    cmd_set.insert("mul");
    cmd_set.insert("div");
    cmd_set.insert("or");
    cmd_set.insert("nor");
    cmd_set.insert("xor");
    cmd_set.insert("and");
    cmd_set.insert("nand");
    cmd_set.insert("xnand");
    cmd_set.insert("shl");
    cmd_set.insert("shr");
    cmd_set.insert("cmp");
    cmd_set.insert("mod");
    cmd_set.insert("adds");
    cmd_set.insert("mins");
    cmd_set.insert("muls");
    cmd_set.insert("divs");
    cmd_set.insert("abs");
    cmd_set.insert("shrs");
    cmd_set.insert("cmps");
    cmd_set.insert("mods");
    cmd_set.insert("addf");
    cmd_set.insert("minf");
    cmd_set.insert("mulf");
    cmd_set.insert("divf");
    cmd_set.insert("absf");
    cmd_set.insert("cmpf");

    reg_map.clear();
    reg_map.insert(std::make_pair("r0", 0));
    reg_map.insert(std::make_pair("r1", 1));
    reg_map.insert(std::make_pair("r2", 2));
    reg_map.insert(std::make_pair("r3", 3));
    reg_map.insert(std::make_pair("r4", 4));
    reg_map.insert(std::make_pair("r5", 5));
    reg_map.insert(std::make_pair("r6", 6));
    reg_map.insert(std::make_pair("r7", 7));
    reg_map.insert(std::make_pair("r8", 8));
    reg_map.insert(std::make_pair("r9", 9));
    reg_map.insert(std::make_pair("r10", 10));
    reg_map.insert(std::make_pair("r11", 11));
    reg_map.insert(std::make_pair("r12", 12));
    reg_map.insert(std::make_pair("r13", 13));
    reg_map.insert(std::make_pair("r14", 14));
    reg_map.insert(std::make_pair("r15", 15));

    instr_map.clear();
    instr_map_insert("j", instruction_type_t::sr, 0b10000, 0);
    instr_map_insert("j", instruction_type_t::sl, 0b10001, 0);
    instr_map_insert("je", instruction_type_t::sr, 0b10010, 0);
    instr_map_insert("je", instruction_type_t::sl, 0b10011, 0);
    instr_map_insert("jne", instruction_type_t::sr, 0b10100, 0);
    instr_map_insert("jne", instruction_type_t::sl, 0b10101, 0);
    instr_map_insert("jg", instruction_type_t::sr, 0b10110, 0);
    instr_map_insert("jg", instruction_type_t::sl, 0b10111, 0);
    instr_map_insert("jge", instruction_type_t::sr, 0b11000, 0);
    instr_map_insert("jge", instruction_type_t::sl, 0b11001, 0);
    instr_map_insert("jl", instruction_type_t::sr, 0b11010, 0);
    instr_map_insert("jl", instruction_type_t::sl, 0b11011, 0);
    instr_map_insert("jle", instruction_type_t::sr, 0b11100, 0);
    instr_map_insert("jle", instruction_type_t::sl, 0b11101, 0);

    instr_map_insert("sw", instruction_type_t::dr, 0b00001, 0);
    instr_map_insert("shw", instruction_type_t::dr, 0b00010, 0);
    instr_map_insert("sb", instruction_type_t::dr, 0b00011, 0);

    instr_map_insert("lw", instruction_type_t::dr, 0b00100, 0);
    instr_map_insert("lhw", instruction_type_t::dr, 0b00101, 0);
    instr_map_insert("lb", instruction_type_t::dr, 0b00110, 0);

    instr_map_insert("cpw", instruction_type_t::dr, 0b00111, 0);
    instr_map_insert("cpw", instruction_type_t::rl, 0b01000, 0);
    instr_map_insert("cphw", instruction_type_t::dr, 0b01001, 0);
    instr_map_insert("cphw", instruction_type_t::rl, 0b01010, 0);
    instr_map_insert("cpb", instruction_type_t::dr, 0b01011, 0);
    instr_map_insert("cpb", instruction_type_t::rl, 0b01100, 0);

    instr_map_insert("add", instruction_type_t::adr, 0, 0b000000);
    instr_map_insert("add", instruction_type_t::arl, 0, 0b000001);
    instr_map_insert("min", instruction_type_t::adr, 0, 0b000110);
    instr_map_insert("min", instruction_type_t::arl, 0, 0b000111);
    instr_map_insert("mul", instruction_type_t::adr, 0, 0b001100);
    instr_map_insert("mul", instruction_type_t::arl, 0, 0b001101);
    instr_map_insert("div", instruction_type_t::adr, 0, 0b010010);
    instr_map_insert("div", instruction_type_t::arl, 0, 0b010011);
    instr_map_insert("or", instruction_type_t::adr, 0, 0b011110);
    instr_map_insert("or", instruction_type_t::arl, 0, 0b011111);
    instr_map_insert("nor", instruction_type_t::adr, 0, 0b100000);
    instr_map_insert("nor", instruction_type_t::arl, 0, 0b100001);
    instr_map_insert("xor", instruction_type_t::adr, 0, 0b100010);
    instr_map_insert("xor", instruction_type_t::arl, 0, 0b100011);
    instr_map_insert("and", instruction_type_t::adr, 0, 0b100100);
    instr_map_insert("and", instruction_type_t::arl, 0, 0b100101);
    instr_map_insert("nand", instruction_type_t::adr, 0, 0b100110);
    instr_map_insert("nand", instruction_type_t::arl, 0, 0b100111);
    instr_map_insert("shl", instruction_type_t::adr, 0, 0b011000);
    instr_map_insert("shl", instruction_type_t::arl, 0, 0b011001);
    instr_map_insert("shr", instruction_type_t::adr, 0, 0b011010);
    instr_map_insert("shr", instruction_type_t::arl, 0, 0b011011);
    instr_map_insert("cmp", instruction_type_t::adr, 0, 0b101100);
    instr_map_insert("cmp", instruction_type_t::arl, 0, 0b101101);
    instr_map_insert("mod", instruction_type_t::adr, 0, 0b110010);
    instr_map_insert("mod", instruction_type_t::arl, 0, 0b110011);

    instr_map_insert("adds", instruction_type_t::adr, 0, 0b000010);
    instr_map_insert("adds", instruction_type_t::arl, 0, 0b000011);
    instr_map_insert("mins", instruction_type_t::adr, 0, 0b001000);
    instr_map_insert("mins", instruction_type_t::arl, 0, 0b001001);
    instr_map_insert("muls", instruction_type_t::adr, 0, 0b001110);
    instr_map_insert("muls", instruction_type_t::arl, 0, 0b001111);
    instr_map_insert("divs", instruction_type_t::adr, 0, 0b010100);
    instr_map_insert("divs", instruction_type_t::arl, 0, 0b010101);
    instr_map_insert("abs", instruction_type_t::asr, 0, 0b101000);
    instr_map_insert("abs", instruction_type_t::adr, 0, 0b101001);
    instr_map_insert("shrs", instruction_type_t::adr, 0, 0b011100);
    instr_map_insert("shrs", instruction_type_t::arl, 0, 0b011101);
    instr_map_insert("cmps", instruction_type_t::adr, 0, 0b101110);
    instr_map_insert("cmps", instruction_type_t::arl, 0, 0b101111);
    instr_map_insert("mods", instruction_type_t::adr, 0, 0b101110);
    instr_map_insert("mods", instruction_type_t::arl, 0, 0b101111);

    instr_map_insert("addf", instruction_type_t::adr, 0, 0b000100);
    instr_map_insert("addf", instruction_type_t::arl, 0, 0b000101);
    instr_map_insert("minf", instruction_type_t::adr, 0, 0b001010);
    instr_map_insert("minf", instruction_type_t::arl, 0, 0b001011);
    instr_map_insert("mulf", instruction_type_t::adr, 0, 0b010000);
    instr_map_insert("mulf", instruction_type_t::arl, 0, 0b010001);
    instr_map_insert("divf", instruction_type_t::adr, 0, 0b010110);
    instr_map_insert("divf", instruction_type_t::arl, 0, 0b010111);
    instr_map_insert("absf", instruction_type_t::asr, 0, 0b101010);
    instr_map_insert("absf", instruction_type_t::adr, 0, 0b101011);
    instr_map_insert("cmpf", instruction_type_t::adr, 0, 0b110000);
    instr_map_insert("cmpf", instruction_type_t::arl, 0, 0b110001);

    instr_type_info_map.clear();
    instr_type_info_map.insert(std::make_pair(instruction_type_t::sl, instruction_type_info_t(16, 4)));
    instr_type_info_map.insert(std::make_pair(instruction_type_t::sr, instruction_type_info_t(0, 2)));
    instr_type_info_map.insert(std::make_pair(instruction_type_t::dr, instruction_type_info_t(0, 2)));
    instr_type_info_map.insert(std::make_pair(instruction_type_t::rl, instruction_type_info_t(16, 4)));
    instr_type_info_map.insert(std::make_pair(instruction_type_t::asr, instruction_type_info_t(0, 2)));
    instr_type_info_map.insert(std::make_pair(instruction_type_t::adr, instruction_type_info_t(0, 3)));
    instr_type_info_map.insert(std::make_pair(instruction_type_t::arl, instruction_type_info_t(16, 4)));
}