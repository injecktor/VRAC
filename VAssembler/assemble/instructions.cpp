#include "instructions.hpp"


std::unordered_set<std::string> cmd_set;
std::unordered_map<std::string, int> reg_map;
std::unordered_map<instruction_t, vasm_code_t> instr_map;

inline static void instr_map_insert(std::string cmd, instr_type_t type, int opcode, int alucode) {
    instr_map.insert(std::make_pair(instruction_t(cmd, type), vasm_code_t(opcode, alucode)));
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
    instr_map_insert("j", instr_type_t::sr, 0b10000, 0);
    instr_map_insert("j", instr_type_t::sl, 0b1001, 0);
    instr_map_insert("je", instr_type_t::sr, 0b10010, 0);
    instr_map_insert("je", instr_type_t::sl, 0b10011, 0);
    instr_map_insert("jne", instr_type_t::sr, 0b10100, 0);
    instr_map_insert("jne", instr_type_t::sl, 0b10101, 0);
    instr_map_insert("jg", instr_type_t::sr, 0b10110, 0);
    instr_map_insert("jg", instr_type_t::sl, 0b10111, 0);
    instr_map_insert("jge", instr_type_t::sr, 0b11000, 0);
    instr_map_insert("jge", instr_type_t::sl, 0b11001, 0);
    instr_map_insert("jl", instr_type_t::sr, 0b11010, 0);
    instr_map_insert("jl", instr_type_t::sl, 0b11011, 0);
    instr_map_insert("jle", instr_type_t::sr, 0b11100, 0);
    instr_map_insert("jle", instr_type_t::sl, 0b11101, 0);

    instr_map_insert("sw", instr_type_t::sr, 0b00001, 0);
    instr_map_insert("shw", instr_type_t::sr, 0b00010, 0);
    instr_map_insert("sb", instr_type_t::sr, 0b00011, 0);

    instr_map_insert("lw", instr_type_t::sr, 0b00100, 0);
    instr_map_insert("lhw", instr_type_t::sr, 0b00101, 0);
    instr_map_insert("lb", instr_type_t::sr, 0b00110, 0);

    instr_map_insert("cpw", instr_type_t::dr, 0b00111, 0);
    instr_map_insert("cpw", instr_type_t::rl, 0b01000, 0);
    instr_map_insert("cphw", instr_type_t::dr, 0b01001, 0);
    instr_map_insert("cphw", instr_type_t::rl, 0b01010, 0);
    instr_map_insert("cpb", instr_type_t::dr, 0b01011, 0);
    instr_map_insert("cpb", instr_type_t::rl, 0b01100, 0);

    instr_map_insert("add", instr_type_t::adr, 0, 0b000000);
    instr_map_insert("add", instr_type_t::arl, 0, 0b000001);
    instr_map_insert("min", instr_type_t::adr, 0, 0b000110);
    instr_map_insert("min", instr_type_t::arl, 0, 0b000111);
    instr_map_insert("mul", instr_type_t::adr, 0, 0b001100);
    instr_map_insert("mul", instr_type_t::arl, 0, 0b001101);
    instr_map_insert("div", instr_type_t::adr, 0, 0b010010);
    instr_map_insert("div", instr_type_t::arl, 0, 0b010011);
    instr_map_insert("or", instr_type_t::adr, 0, 0b011110);
    instr_map_insert("or", instr_type_t::arl, 0, 0b011111);
    instr_map_insert("nor", instr_type_t::adr, 0, 0b100000);
    instr_map_insert("nor", instr_type_t::arl, 0, 0b100001);
    instr_map_insert("xor", instr_type_t::adr, 0, 0b100010);
    instr_map_insert("xor", instr_type_t::arl, 0, 0b100011);
    instr_map_insert("and", instr_type_t::adr, 0, 0b100100);
    instr_map_insert("and", instr_type_t::arl, 0, 0b100101);
    instr_map_insert("nand", instr_type_t::adr, 0, 0b100110);
    instr_map_insert("nand", instr_type_t::arl, 0, 0b100111);
    instr_map_insert("xnand", instr_type_t::adr, 0, 0b100010);
    instr_map_insert("xnand", instr_type_t::arl, 0, 0b100011);
    instr_map_insert("shl", instr_type_t::adr, 0, 0b011000);
    instr_map_insert("shl", instr_type_t::arl, 0, 0b011001);
    instr_map_insert("shr", instr_type_t::adr, 0, 0b011010);
    instr_map_insert("shr", instr_type_t::arl, 0, 0b011011);
    instr_map_insert("cmp", instr_type_t::adr, 0, 0b101100);
    instr_map_insert("cmp", instr_type_t::arl, 0, 0b101101);
    instr_map_insert("mod", instr_type_t::adr, 0, 0b110010);
    instr_map_insert("mod", instr_type_t::arl, 0, 0b110011);

    instr_map_insert("adds", instr_type_t::adr, 0, 0b000010);
    instr_map_insert("adds", instr_type_t::arl, 0, 0b000011);
    instr_map_insert("mins", instr_type_t::adr, 0, 0b001000);
    instr_map_insert("mins", instr_type_t::arl, 0, 0b001001);
    instr_map_insert("muls", instr_type_t::adr, 0, 0b001110);
    instr_map_insert("muls", instr_type_t::arl, 0, 0b001111);
    instr_map_insert("divs", instr_type_t::adr, 0, 0b010100);
    instr_map_insert("divs", instr_type_t::arl, 0, 0b010101);
    instr_map_insert("abs", instr_type_t::asr, 0, 0b101000);
    instr_map_insert("abs", instr_type_t::adr, 0, 0b101001);
    instr_map_insert("shrs", instr_type_t::adr, 0, 0b011100);
    instr_map_insert("shrs", instr_type_t::arl, 0, 0b011101);
    instr_map_insert("cmps", instr_type_t::adr, 0, 0b101110);
    instr_map_insert("cmps", instr_type_t::arl, 0, 0b101111);
    instr_map_insert("mods", instr_type_t::adr, 0, 0b101110);
    instr_map_insert("mods", instr_type_t::arl, 0, 0b101111);

    instr_map_insert("addf", instr_type_t::adr, 0, 0b000100);
    instr_map_insert("addf", instr_type_t::arl, 0, 0b000101);
    instr_map_insert("minf", instr_type_t::adr, 0, 0b001010);
    instr_map_insert("minf", instr_type_t::arl, 0, 0b001011);
    instr_map_insert("mulf", instr_type_t::adr, 0, 0b010000);
    instr_map_insert("mulf", instr_type_t::arl, 0, 0b010001);
    instr_map_insert("divf", instr_type_t::adr, 0, 0b010110);
    instr_map_insert("divf", instr_type_t::arl, 0, 0b010111);
    instr_map_insert("absf", instr_type_t::asr, 0, 0b101010);
    instr_map_insert("absf", instr_type_t::adr, 0, 0b101011);
    instr_map_insert("cmpf", instr_type_t::adr, 0, 0b110000);
    instr_map_insert("cmpf", instr_type_t::arl, 0, 0b110001);
}