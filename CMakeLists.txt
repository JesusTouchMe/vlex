cmake_minimum_required(VERSION 3.26)

project(vlex)

include(CMakeParseArguments)

set(SOURCES
    "source/main.cpp"

    "source/lexer/SourceLocation.cpp"
    "source/lexer/Token.cpp"
    "source/lexer/Lexer.cpp"

    "source/parser/Parser.cpp"

    "source/generator/Generator.cpp"
    
    "source/diagnostic/Diagnostic.cpp"
)

set(HEADERS
    "include/lexer/SourceLocation.h"
    "include/lexer/Token.h"
    "include/lexer/Lexer.h"

    "include/parser/Parser.h"

    "include/generator/Generator.h"

    "include/diagnostic/Diagnostic.h"
    "include/templates/StringLiteralParseCPP.h"
)

add_executable(vlex ${SOURCES} ${HEADERS})
target_include_directories(vlex PRIVATE include)
target_compile_features(vlex PRIVATE cxx_std_20)

function(vlex_generate target cfg src inc)
    set(oneValueArgs FILE_NAME_STYLE FILE_PREFIX)
    cmake_parse_arguments(VLEX "" "${oneValueArgs}" "" ${ARGN})

    if (NOT VLEX_FILE_NAME_STYLE)
        set(VLEX_NAME_STYLE "PascalCase")
    endif()

    if (NOT VLEX_FILE_PREFIX)
        set(VLEX_FILE_PREFIX "")
    endif()

    if (VLEX_FILE_NAME_STYLE STREQUAL "snake_case")
        set(lexer lexer)
        set(token token)
        set(source_location source_location)
    elseif (VLEX_FILE_NAME_STYLE STREQUAL "PascalCase")
        set(lexer Lexer)
        set(token Token)
        set(source_location SourceLocation)
    else()
        message(FATAL_ERROR "Unknown VLEX_FILE_NAME_STYLE: ${VLEX_FILE_NAME_STYLE}")
    endif()

    set(lexer ${VLEX_FILE_PREFIX}${lexer})
    set(token ${VLEX_FILE_PREFIX}${token})
    set(source_location ${VLEX_FILE_PREFIX}${source_location})

    get_target_property(working_dir ${target} SOURCE_DIR)

    if (IS_ABSOLUTE ${src})
    else()
        set(src ${working_dir}/${src})
    endif()

    if (IS_ABSOLUTE ${inc})
    else()
        set(inc ${working_dir}/${inc})
    endif()

    add_custom_command(
        OUTPUT ${inc}/${lexer}.h ${src}/${lexer}.cpp ${inc}/${source_location}.h ${src}/${source_location}.cpp ${inc}/${token}.h ${src}/${token}.cpp
        PRE_BUILD
        DEPENDS ${cfg}
        COMMAND vlex ${cfg} ${src} ${inc} ${VLEX_FILE_NAME_STYLE} ${VLEX_FILE_PREFIX} PascalCase ""
        WORKING_DIRECTORY ${working_dir}
    )
    target_sources(${target} PRIVATE ${inc}/${lexer}.h ${src}/${lexer}.cpp ${inc}/${source_location}.h ${src}/${source_location}.cpp ${inc}/${token}.h ${src}/${token}.cpp)
endfunction()