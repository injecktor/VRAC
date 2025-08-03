#include "imports.hpp"

void create_imports_request(vasm_file_t& output_file, const std::list<std::string>& decoded_strings, 
    const import_request_t& import_labels) {
    if (import_labels.empty()) {
        return;
    }
        
    output_file.write_line("Import requests");
    for (auto&& label : import_labels) {
        output_file.write_line(label);
    }
    output_file.write_line("END");
    print_info("Create imports request completed.", 2);
}