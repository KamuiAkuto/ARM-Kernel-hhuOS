cmake_minimum_required(VERSION 3.0.0)

set(OS_NAME "myos")

set (CMAKE_SYSTEM_NAME Generic)
set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set (CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

#set (CMAKE_EXE_LINKER_FLAGS "-T ../linker/script.ld -nostdlib -g -O0 -Wl,--whole-archive -Wl,-M")
set (CMAKE_EXE_LINKER_FLAGS "-T ../linker/script.ld -nostdlib -g -O0 -Wl,--whole-archive")

set (CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> -o <OBJECT> <SOURCE>")
set (CMAKE_ASM_LINK_EXECUTABLE "<CMAKE_LINKER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

set (CMAKE_C_FLAGS "-ffreestanding -fno-exceptions -fno-rtti")
set (CMAKE_CXX_FLAGS "-ffreestanding -fno-exceptions -fno-rtti")

project("${OS_NAME}" C ASM CXX)

include_directories(${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE SOURCES_B "boot/*.c")
set(files ${SOURCES} ${SOURCES_B})
add_library(lib ${files})

add_executable("${OS_NAME}.elf" ./boot/boot.asm)

target_link_libraries("${OS_NAME}.elf" lib)

set_target_properties("${OS_NAME}.elf" PROPERTIES LINK_DEPENDS "../linker/script.ld")
