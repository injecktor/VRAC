#include "funcs_handle.hpp"

std::vector<file_t> files;

static file_t get_file(std::string file_str) {
    for (auto& file : files) {
        if (file.file == file_str) {
            return file;
        }
    }
    vasm_flags.last_error_extra_msg = "Couldn't find file: " + file_str;
    throw link_error_t::main_func;
    return {};
}

static func_t get_func(const file_t& file, const std::string& func_str) {
    for (const auto& func : file.exports, file.locals) {
        if (func.name == func_str) {
            return func;
        }
    }
    vasm_flags.last_error_extra_msg = "Couldn't find function " + func_str;
    vasm_flags.last_error_extra_msg += " in file " + file.file;
    throw link_error_t::main_func;
    return {};
}

std::string get_main_func_file_str() {
    std::string main_file;
    for (auto& file : files) {
        for (auto& func : file.exports, file.locals) {
            if (func.name == "main") {
                if (!main_file.empty()) {
                    vasm_flags.last_error_extra_msg = "Several main functions.";
                    throw link_error_t::main_func;
                }
                main_file = file.file;
            }
        }
    }
    if (main_file.empty()) {
        vasm_flags.last_error_extra_msg = "Couldn't find main function.";
        throw link_error_t::main_func;
    }
    return main_file;
}

std::vector<func_info_t> get_used_funcs(const std::string& file_str, const std::string& func_str) {
    std::vector<func_info_t> funcs;
    try {
        file_t file = std::move(get_file(file_str));
        func_t func = std::move(get_func(file, func_str));

    }
    catch (const link_error_t& error) {
        throw error;
    }
    catch (...) {
        throw link_error_t::unknown;
    }
}