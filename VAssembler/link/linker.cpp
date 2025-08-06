#include "linker.hpp"

#define THROW_ERROR(_text_) do { \
    vasm_flags.last_error_extra_msg = _text_; \
    throw link_error_t::linker; \
} while(false);

struct funcs {
    std::string file;
    std::string name;
    size_t addr;
    size_t size;
};

std::vector<funcs> exports;
std::vector<funcs> imports;
std::vector<funcs> locals;

enum class section_t {
    import,
    export,
    local
};

void parse_section(vasm_file_t& input_file, section_t section) {
    std::string line;
    std::vector<funcs>* funcs;
    size_t first_offset, second_offset;
    switch (section) {
        case section_t::import: 
            funcs = &imports;
        break;
        case section_t::export: 
            funcs = &exports;
        break;
        case section_t::local: 
            funcs = &locals;
        break;
    }
    while (input_file.read_line(line) && line != "END") {
        funcs->push_back({});
        funcs->back().file = input_file.current_file();
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

void check_markers(std::string& line) {
    line.find('&');
}

void vasm_link(std::vector<std::string> objects) {
    vasm_file_t input_file(file_mode_t::read);
    vasm_file_t output_file(file_mode_t::write);
    std::string line;
    size_t total_size = 0;

    try {
        for (const auto& obj : objects) {
            auto& obj = objects.back();
            input_file.open(obj);
            input_file.read_line(line); 
            total_size += std::stoi(line);
            while (input_file.read_line(line)) {
                if (line == "Import functions") {
                    parse_section(input_file, section_t::import);
                } else if (line == "Export functions") {
                    parse_section(input_file, section_t::export);
                } else if (line == "Local functions") {
                    parse_section(input_file, section_t::local);
                } else if (line == "Instructions") {
                    break;
                }
            }
        }
        


        output_file.open(vasm_flags.output_path);
        output_file.write_line("");
        output_file.write_line("");
        while (input_file.read_line(line)) {
            check_markers(line);
        }
    }
    catch (const link_error_t& error) {
        throw error;
    }
    catch (...) {
        throw link_error_t::unknown;
    }
}