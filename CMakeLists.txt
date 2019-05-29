# This file is part of 43S.
#
# 43S is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# 43S is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with 43S.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.13)

project(WP43S C)
set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "-march=x86-64 -Wextra -Wall -std=c11 -m64 -fshort-enums -fomit-frame-pointer -DTFM_NO_ASM -DTFM_X86_64 -MMD")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DOSX")
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g -DEXTRA_INFO_ON_CALC_ERROR")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O2 -s")



find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 		REQUIRED gtk+-3.0)
pkg_check_modules(FREETYPE 	REQUIRED freetype2)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/decNumberICU)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/wp43s)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/wp43s/mathematics)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/wp43s/browsers)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/wp43s/logicalOps)


set(TTF2RASTERFONTS_SRC
        src/ttf2RasterFonts/ttf2RasterFonts.c)

set(DECIMAL_SRC
        decNumberICU/decContext.c 	decNumberICU/decContext.h
        decNumberICU/decDouble.c 	decNumberICU/decDouble.h
        decNumberICU/decimal128.c	decNumberICU/decimal128.h
        decNumberICU/decimal64.c 	decNumberICU/decimal64.h
        decNumberICU/decNumber.c 	decNumberICU/decNumber.h	decNumberICU/decNumberLocal.h
        decNumberICU/decQuad.c 		decNumberICU/decQuad.h)

set(GENCONST_SRC
        src/generateConstants/generateConstants.c)

file(GLOB WP43S_SRC
        src/wp43s/*.h 				src/wp43s/*.c
        src/wp43s/mathematics/*.h	src/wp43s/mathematics/*.c
        src/wp43s/browsers/*.h		src/wp43s/browsers/*.c
        src/wp43s/logicalOps/*.h	src/wp43s/logicalOps/*.c)

set(TESTSUITE_SRC
        src/testSuite/testSuite.c)

set_source_files_properties(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/wp43s/constantPointers.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/wp43s/constantPointers.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/wp43s/rasterFontsData.c
        PROPERTIES GENERATED TRUE)

#
# Generate Raster Fonts
#
add_executable(ttf2RasterFonts ${TTF2RASTERFONTS_SRC})

target_compile_options(ttf2RasterFonts 		PRIVATE ${GTK3_CFLAGS_OTHER})
target_include_directories(ttf2RasterFonts	PRIVATE ${GTK3_INCLUDE_DIRS})
target_link_libraries(ttf2RasterFonts 		PRIVATE ${GTK3_LIBRARIES} ${FREETYPE_LIBRARIES})
target_link_directories(ttf2RasterFonts 	PRIVATE ${GTK3_LIBRARY_DIRS})

add_custom_command(
        OUTPUT 				${CMAKE_CURRENT_SOURCE_DIR}/src/wp43s/rasterFontsData.c
        COMMAND 			${CMAKE_CURRENT_BINARY_DIR}/ttf2RasterFonts
        WORKING_DIRECTORY 	${CMAKE_CURRENT_SOURCE_DIR})

#
# Generare Constants
#
add_executable(generateConstants ${GENCONST_SRC} ${DECIMAL_SRC})

add_custom_command(
        OUTPUT 				${CMAKE_CURRENT_SOURCE_DIR}/src/wp43s/constantPointers.c 
							${CMAKE_CURRENT_SOURCE_DIR}/src/wp43s/constantPointers.h
        COMMAND 			${CMAKE_CURRENT_BINARY_DIR}/generateConstants
        WORKING_DIRECTORY 	${CMAKE_CURRENT_SOURCE_DIR})

#
# wp43s
# 
add_executable(wp43s
        ${WP43S_SRC}
        ${DECIMAL_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/wp43s/constantPointers.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/wp43s/constantPointers.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/wp43s/rasterFontsData.c)

add_dependencies(wp43s generateConstants ttf2RasterFonts)
file(COPY 			${CMAKE_CURRENT_SOURCE_DIR}/wp43s_pre.css 
	 DESTINATION 	"${CMAKE_BINARY_DIR}")

set_target_properties(wp43s PROPERTIES COMPILE_FLAGS "-DPC_BUILD")
target_compile_options(wp43s PRIVATE ${GTK3_CFLAGS_OTHER})
target_include_directories(wp43s PRIVATE ${GTK3_INCLUDE_DIRS})
target_link_libraries(wp43s PRIVATE ${GTK3_LIBRARIES})
target_link_directories(wp43s PRIVATE ${GTK3_LIBRARY_DIRS})

#
# Test Suite
#
add_executable(testSuite
        ${TESTSUITE_SRC}
        ${WP43S_SRC}
        ${DECIMAL_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/wp43s/constantPointers.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/wp43s/constantPointers.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/wp43s/rasterFontsData.c)

add_dependencies(testSuite generateConstants ttf2RasterFonts)

set_target_properties(testSuite PROPERTIES COMPILE_FLAGS "-DPC_BUILD -DTESTSUITE_BUILD")
target_compile_options(testSuite PRIVATE ${GTK3_CFLAGS_OTHER})
target_include_directories(testSuite PRIVATE ${GTK3_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src/testSuite)
target_link_libraries(testSuite PRIVATE ${GTK3_LIBRARIES})
target_link_directories(testSuite PRIVATE ${GTK3_LIBRARY_DIRS})

