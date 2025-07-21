#pragma once

#include <string>
#include <iostream>

#include "vasm_config.hpp"
#include "vasm_flags.hpp"

extern void print_err(const std::string& str);
extern void print_info(const std::string& str, size_t required_info_level = 1);