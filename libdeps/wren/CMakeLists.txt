cmake_minimum_required(VERSION 3.0)

project(WrenVM C)

set(vm_directory ${WrenVM_SOURCE_DIR}/src/vm)

file(GLOB_RECURSE wren_vm_cpp
    ${WrenVM_SOURCE_DIR}/src/*.c
)

file(GLOB_RECURSE wren_vm_hpp
    ${WrenVM_SOURCE_DIR}/src/*.h
)

include_directories(
    ${WrenVM_SOURCE_DIR}/src/vm
    ${WrenVM_SOURCE_DIR}/src/include
    ${WrenVM_SOURCE_DIR}/src/optional
)

add_library(${PROJECT_NAME} STATIC ${wren_vm_cpp} ${wren_vm_hpp})
set_property(TARGET WrenVM PROPERTY C_STANDARD 99)
