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

if(DEFINED ARM)
    set(ISA_DIR "${CMAKE_CURRENT_LIST_DIR}/arm")
else()
    set(ISA_DIR "${CMAKE_CURRENT_LIST_DIR}/aarch64")
endif()

set(CMAKE_TOOLCHAIN_FILE "${ISA_DIR}/toolchain.cmake" PARENT_SCOPE)
include("${ISA_DIR}/flags.cmake")

file(GLOB ISA_KERNEL_SRCS "${ISA_DIR}/*.c" "${ISA_DIR}/*.S")
set(LINK_LAYOUT "${ISA_DIR}/layout.lds" PARENT_SCOPE)

file(GLOB GENERIC_KERNEL_SRCS "${CMAKE_CURRENT_LIST_DIR}/*.c")

set(KERNEL_SRCS "${GENERIC_KERNEL_SRCS}" "${ISA_KERNEL_SRCS}" PARENT_SCOPE)
