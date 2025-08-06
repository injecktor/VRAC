#pragma once
#include <list>

#include "vasm_errors.hpp"
#include "vasm_flags.hpp"
#include "decoder.hpp"
#include "tokenizer.hpp"
#include "file_handle.hpp"
#include "object_file.hpp"

extern std::vector<std::string> vasm_assemble();