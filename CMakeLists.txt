CMAKE_MINIMUM_REQUIRED(VERSION 3.13)

add_subdirectory(src/kernel)
add_subdirectory(src/libc)

project(Raspos LANGUAGES C CXX ASM)

message(STATUS "Detected Kernel Source files: ${KERNEL_SRCS}")
message(STATUS "Detected libc Source files: ${LIBC_SRCS}")

add_executable(kernel.elf ${KERNEL_SRCS})

target_link_options(kernel.elf PRIVATE -T "${LINK_LAYOUT}" -nostdlib)

add_custom_target(
    kernel.img ALL
    ${CMAKE_OBJCOPY} kernel.elf -O binary kernel.img
)
add_dependencies(kernel.img kernel.elf)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(
        kernel.img
        PROPERTIES EXCLUDE_FROM_ALL TRUE
    )
endif()

