#include "linker.hpp"

enum class section_t {
    import,
    export,
    local
};

void parse_section(file_t& last_file, vasm_file_t& input_file, section_t section) {
    std::string line;
    std::vector<func_t>* funcs;
    size_t first_offset, second_offset;
    switch (section) {
        case section_t::import: 
            funcs = &last_file.imports;
        break;
        case section_t::export: 
            funcs = &last_file.exports;
        break;
        case section_t::local: 
            funcs = &last_file.locals;
        break;
    }
    while (input_file.read_line(line) && line != "END") {
        funcs->push_back({});
        first_offset = line.find(' ');
        funcs->back().name = line.substr(0, first_offset);
        first_offset++;
        second_offset = line.find(' ', first_offset);
        funcs->back().addr = std::stoi(line.substr(first_offset, second_offset));
        funcs->back().size = std::stoi(line.substr(second_offset + 1));
    }
    if (line != "END") {
        vasm_flags.last_error_extra_msg = "Couldn't find end of section.";
        throw link_error_t::section;
    }
}

void vasm_link(std::vector<std::string> objects) {
    vasm_file_t input_file(file_mode_t::read);
    vasm_file_t output_file(file_mode_t::write);
    std::string line;
    size_t total_size = 0;

    try {
        for (const auto& obj : objects) {
            files.push_back({});
            file_t& last_file = files.back();
            last_file.file = obj;
            input_file.open(obj);
            input_file.read_line(line);
            total_size += std::stoi(line);
            while (input_file.read_line(line)) {
                if (line == "Import functions") {
                    parse_section(last_file, input_file, section_t::import);
                } else if (line == "Export functions") {
                    parse_section(last_file, input_file, section_t::export);
                } else if (line == "Local functions") {
                    parse_section(last_file, input_file, section_t::local);
                } else if (line == "Instructions") {
                    break;
                }
            }
            input_file.close();
        }
        auto main_file_str = std::move(get_main_func_file_str());
        auto funcs = get_used_funcs(main_file_str, "main");


        output_file.open(vasm_flags.output_path);
        output_file.write_line("");
        output_file.write_line("");
    }
    catch (const link_error_t& error) {
        throw error;
    }
    catch (...) {
        throw link_error_t::unknown;
    }
}