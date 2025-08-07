cmake_minimum_required(VERSION 3.22)
project(vasm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++.exe)
set(CMAKE_EXECUTABLE_SUFFIX ".exe")

add_executable(vasm 
        vasm.cpp
        args_handle.cpp
        vasm_flags.cpp
        vasm_logging.cpp
        file_handle.cpp

        pre_assemble/pre_assemble.cpp

        assemble/assemble.cpp
        assemble/decoder.cpp
        assemble/object_file.cpp
        assemble/tokenizer.cpp
        assemble/instructions.cpp

        link/linker.cpp
)

target_include_directories(vasm PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        pre_assemble/
        assemble/
        link/
)

set_target_properties(vasm PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)