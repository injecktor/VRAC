#include <vector>
#include <string>

#include "vasm_errors.hpp"

struct file_t {
    std::string file;
    std::vector<func_t> exports;
    std::vector<func_t> imports;
    std::vector<func_t> locals;
};

struct func_t {
    std::string name;
    size_t addr;
    size_t size;
};

struct func_info_t {
    func_t func;
    std::string file;
};

extern std::vector<file_t> files;

extern std::string get_main_func_file_str();

extern std::vector<func_info_t> get_used_funcs(const std::string& file, const std::string& func);