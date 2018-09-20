cmake_minimum_required(VERSION 3.0)

project(WrenVM)

set(vm_directory ${CMAKE_CURRENT_LIST_DIR}/src/vm)

set(wren_vm_cpp
    ${vm_directory}/wren_compiler.c
    ${vm_directory}/wren_core.c
    ${vm_directory}/wren_debug.c
    ${vm_directory}/wren_primitive.c
    ${vm_directory}/wren_utils.c
    ${vm_directory}/wren_value.c
    ${vm_directory}/wren_vm.c
    src/optional/wren_opt_meta.c
    src/optional/wren_opt_random.c
    )

set(wren_vm_hpp
    src/include/wren.h
    src/include/wren.hpp
    src/optional/wren_opt_meta.h
    src/optional/wren_opt_random.h
    ${vm_directory}/wren_common.h
    ${vm_directory}/wren_compiler.h
    ${vm_directory}/wren_core.h
    ${vm_directory}/wren_debug.h
    ${vm_directory}/wren_opcodes.h
    ${vm_directory}/wren_primitive.h
    ${vm_directory}/wren_utils.h
    ${vm_directory}/wren_value.h
    ${vm_directory}/wren_vm.h
    )

include_directories(src/vm)
include_directories(src/include)
include_directories(src/optional)

add_library(${PROJECT_NAME} STATIC ${wren_vm_cpp} ${wren_vm_hpp})

