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

set(CMAKE_SYSTEM_NAME       Generic)
set(CMAKE_SYSTEM_PROCESSOR  arm)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_AR                arm-none-eabi-ar${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_ASM_COMPILER      arm-none-eabi-as${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_C_COMPILER        arm-none-eabi-gcc${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_CXX_COMPILER      arm-none-eabi-g++${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_LINKER            arm-none-eabi-ld${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_OBJCOPY           arm-none-eabi-objcopy${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_RANLIB            arm-none-eabi-ranlib${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_SIZE              arm-none-eabi-size${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_STRIP             arm-none-eabi-strip${CMAKE_EXECUTABLE_SUFFIX})

set(CMAKE_C_FLAGS           "-mcpu=arm1176jzf-s -fpic -ffreestanding")
set(CMAKE_CXX_FLAGS         "${CMAKE_C_FLAGS}")

set(CMAKE_ASM_FLAGS_DEBUG   "-g" CACHE INTERNAL "")
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "")
set(CMAKE_C_FLAGS_DEBUG     "-Os -g" CACHE INTERNAL "")
set(CMAKE_C_FLAGS_RELEASE   "-Os -DNDEBUG" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" CACHE INTERNAL "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
