cmake_minimum_required(VERSION 3.22)
# set(CMAKE_MAKE_PROGRAM "C:\\msys64\\usr\\bin\\make.exe")
project(vasm NONE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++.exe)

add_executable(vasm 
        vasm.cpp
        args_handle.cpp
        vasm_flags.cpp
        vasm_logging.cpp
)

set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_COMPILER} \"${CMAKE_CURRENT_SOURCE_DIR}/vasm.cpp\" \"${CMAKE_CURRENT_SOURCE_DIR}/args_handle.cpp\" \"${CMAKE_CURRENT_SOURCE_DIR}/vasm_flags.cpp\" \"${CMAKE_CURRENT_SOURCE_DIR}/vasm_logging.cpp\" -o \"${CMAKE_CURRENT_SOURCE_DIR}/vasm.exe\"")

target_include_directories(vasm PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(vasm PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        LINKER_LANGUAGE CXX
)