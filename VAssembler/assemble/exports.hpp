#pragma once

#include <unordered_map>
#include <sstream>

#include "vasm_errors.hpp"
#include "vasm_flags.hpp"
#include "file_handle.hpp"
#include "decoder.hpp"

extern void create_exports(vasm_file_t& output_file, const export_label_t& export_labels);