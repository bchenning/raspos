# RaspOS
# Copyright (C) 2022  Benny Henning
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
# Contact: benny.henning@web.de

CMAKE_MINIMUM_REQUIRED(VERSION 3.13)

add_subdirectory(src/kernel)
add_subdirectory(src/libc)

project(RaspOS LANGUAGES C CXX ASM)

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

