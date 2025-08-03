#include "exports.hpp"

void create_exports(vasm_file_t& output_file, const export_label_t& export_labels) {
    if (export_labels.empty()) {
        return;
    }

    output_file.write_line("Export labels");
    for (auto&& label : export_labels) {
        std::stringstream stream;
        stream << label.first << " " << label.second.first << " " << label.second.second;
        output_file.write_line(stream.str());
    }
    output_file.write_line("END");
    print_info("Create exports completed.", 2);
}