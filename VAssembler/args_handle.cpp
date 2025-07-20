#include "args_handle.hpp"

#define THROW(_error_, _msg_) do { \
    vasm_flags.last_error_extra_msg = _msg_; \
    throw _error_; \
} while (false);

#define CHECK_FLAG_UNIQUE(_flag_) do { \
    if (!check_flag_unique(_flag_)) { \
        THROW(args_handle_error_t::duplicated_flag, _flag_); \
    } \
} while (false);

static bool check_flag_unique(std::string flag) {
    static std::unordered_set<std::string> used_unique_flags;
    if (used_unique_flags.find(flag) == used_unique_flags.end()) {
        used_unique_flags.insert(flag);
        return true;
    }
    return false;
}

void parse_args(int argc, char *argv[]) {
    if (argc < 2) {
        THROW(args_handle_error_t::arg_count, "");
    }

    bool arg_complete = true;
    std::string arg;
    std::string *second_arg_str = nullptr;
    int *second_arg_int = nullptr;

    for (size_t i = 1; i < argc; i++) {
        arg = argv[i];
        if (arg_complete == false) {
            if (arg[0] == '-') {
                THROW(args_handle_error_t::unexpected_arg, arg);
            }

            if (second_arg_str) {
                *second_arg_str = arg;
            }
            if (second_arg_int) {
                try {
                    *second_arg_int = std::stoi(arg);
                }
                catch (const std::invalid_argument& ia) {
                    THROW(args_handle_error_t::unexpected_arg, arg);
                }
                catch (const std::out_of_range& oor) {
                    THROW(args_handle_error_t::unexpected_arg, arg);
                }
            }
            second_arg_str = nullptr;
            second_arg_int = nullptr;
            arg_complete = true;
            continue;
        }

        if (arg[0] != '-') {
            vasm_flags.input_files_path.emplace(std::move(arg));
            continue;
        }

        if (arg == "-o") {
            CHECK_FLAG_UNIQUE(arg);
            arg_complete = false;
            second_arg_str = &vasm_flags.output_path;
            continue;
        }

        if (arg == "-v") {
            CHECK_FLAG_UNIQUE(arg);
            arg_complete = false;
            second_arg_int = &vasm_flags.info_level;
            continue;
        }

        THROW(args_handle_error_t::unknown_arg, arg);
    }

    if (arg_complete == false) {
        THROW(args_handle_error_t::arg_count, arg);
    }

    if (vasm_flags.input_files_path.empty()) {
        THROW(args_handle_error_t::no_input_file, "");
    }

    if (vasm_flags.output_path.empty()) {
        vasm_flags.output_path = vasm_flags.input_files_path.front() + DEFAULT_VASM_EXTENSION;
    }

    if (vasm_flags.info_level < 0) {
        vasm_flags.info_level = DEFAULT_INFO_LEVEL;
    }
}