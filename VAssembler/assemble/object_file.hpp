#pragma once

#include <sstream>

#include "decoder.hpp"
#include "file_handle.hpp"

extern void create_object_file(vasm_file_t& output_file, std::string path, const decode_tuple_t& decode_info);

