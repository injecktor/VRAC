#include "object_file.hpp"

void create_imports_request(vasm_file_t& output_file, const import_request_t& import_functions) {
    if (import_functions.empty()) {
        return;
    }
        
    output_file.write_line("Import functions");
    for (auto&& label : import_functions) {
        output_file.write_line(label);
    }
    output_file.write_line("END");
    print_info("Create imports request completed.", 2);
}

void create_locals(vasm_file_t& output_file, const local_function_t& local_functions) {
    if (local_functions.empty()) {
        return;
    }

    output_file.write_line("Local functions");
    for (auto&& func : local_functions) {
        std::stringstream stream;
        stream << func.first << " " << func.second.first << " " << func.second.second;
        output_file.write_line(stream.str());
    }
    output_file.write_line("END");
    print_info("Create exports completed.", 2);
}

void create_exports(vasm_file_t& output_file, const export_function_t& export_functions) {
    if (export_functions.empty()) {
        return;
    }

    output_file.write_line("Export functions");
    for (auto&& func : export_functions) {
        std::stringstream stream;
        stream << func.first << " " << func.second.first << " " << func.second.second;
        output_file.write_line(stream.str());
    }
    output_file.write_line("END");
    print_info("Create exports completed.", 2);
}

void write_instr(vasm_file_t &output_file, const std::list<std::string>& decoded_strings) {
    output_file.write_line("Instructions");
    for (auto&& str : decoded_strings) {
        output_file.write_line(str);
    }
}

void create_object_file(vasm_file_t& output_file, std::string path, const decode_tuple_t& decode_info) {
    vasm_file_t::delete_file(path);
    if (!output_file.open(path)) {
        vasm_flags.last_error_extra_msg = path;
        throw vasm_error_t::cant_open_file;
    }
    output_file.write_line(VASM_HEX_FORMAT_STR);
    output_file.write_line(std::to_string(std::get<4>(decode_info)));
    create_imports_request(output_file, std::get<1>(decode_info));
    create_locals(output_file, std::get<2>(decode_info));
    create_exports(output_file, std::get<3>(decode_info));
    write_instr(output_file, std::get<0>(decode_info));
    
    output_file.close();
}