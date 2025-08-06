#pragma once

#include <queue>
#include <string>
#include <vector>

#include "file_handle.hpp"
#include "vasm_errors.hpp"
#include "vasm_flags.hpp"

extern void vasm_link(std::vector<std::string> objects);